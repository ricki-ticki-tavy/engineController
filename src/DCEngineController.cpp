#include "DCEngineController.h"
#include "DeviceInfo.h"
#include "EnginePowerController.h"
#include "Logger.h"
#include "constUtils.h"
#include "EngineStatisticData.h"

namespace tankController{

DCEngineController::DCEngineController(SettingsManager* settingsManager,
		DeviceInfo* deviceInfo,
		EnginePowerController* enginePowerController){
	this->settingsManager = settingsManager;
	this->deviceInfo = deviceInfo;
	this->enginePowerController = enginePowerController;
	deviceInfo->leftEngineEncoderFound = false;
	deviceInfo->rightEngineEncoderFound = false;
}
//-----------------------------------------------------------------------------------

bool DCEngineController::addEngine(EngineControlData *engine){
  if (started) {
	LOGGER.error(CANT_ADD_ENGINE_MSG + " when controller started.");
    return false;
  }

  if (engineCounter < 2){
    engine->engineDirection = M168_DIRECTION_UNDEFINED;
    engine->lowPower = 0;
    engine->encoderFails = 0;
    engine->integralSummator = 0;
    engine->previousRotateError = 0;
    engine->requiredEncoderFrequency = 0;
    engine->encoderCounter = 0;
    engine->currentPwrCorrection = 1;
    engine->smoothEncoderValue = 0;
    engines[engineCounter++] = engine;
	LOGGER.succed("Added engine." + String((int)engineCounter));
    return true;
  } else {
	LOGGER.error(CANT_ADD_ENGINE_MSG + ". Max engines count exceeded");
  }
}
//---------------------------------------------------------------------



/**
 * Определяет у двигателей наличие энкодеров
**/
void DCEngineController::detectEncoders(){
	LOGGER.add("detecting encoders...");

	// дернем двигателями немного
	for (int engineIndex = 0; engineIndex < engineCounter; engineIndex++){
		deviceInfo->engineControlData[engineIndex].encoderCounter = 0;
		enginePowerController->immediatelySetEnginePwer(engineIndex, M168_DIRECTION_FORWARD, settingsManager->globalSettings->maxPower);
	}
	delay(40);
	//  delayMicroseconds(15000);
	for (int engineIndex = 0; engineIndex < engineCounter; engineIndex++){
		enginePowerController->immediatelySetEnginePwer(engineIndex, M168_DIRECTION_FORWARD, 0);
	}

	delay(20);


	// проверим что насчитали энкодеры
	bool headerPrinted = false;
	for (int engineIndex = 0; engineIndex < engineCounter; engineIndex++){
		engines[engineIndex]->isEngineEncoderPresent = engines[engineIndex]->encoderCounter != 0;
		if (!engines[engineIndex]->isEngineEncoderPresent) {
			LOGGER.error("encoder " + String(engineIndex) + " not found");
		}
	}

	//дадим моторам остановиться
	delay(2000);

	// проверка на висячий в воздухе датчик
	for (int engineIndex = 0; engineIndex < engineCounter; engineIndex++){
		engines[engineIndex]->encoderCounter = 0;
	}

	delay(200);
	for (int engineIndex = 0; engineIndex < engineCounter; engineIndex++){
		if (engines[engineIndex]->isEngineEncoderPresent) {
			engines[engineIndex]->isEngineEncoderPresent = engines[engineIndex]->isEngineEncoderPresent && !engines[engineIndex]->encoderCounter;
			LOGGER.succedOrError(engines[engineIndex]->isEngineEncoderPresent, "encoder " + String(engineIndex) + (engines[engineIndex]->isEngineEncoderPresent ? " found" : " has broken wire"));
		}
	}

	// сброс
	for (int engineIndex = 0; engineIndex < engineCounter; engineIndex++){
		engines[engineIndex]->encoderCounter = 0;
	}

}
//-----------------------------------------------------------------------

bool DCEngineController::start(void){
  detectEncoders();
////   os_timer_setfn(&pidCycleTimer, correctionWorkCycle, NULL);
////   os_timer_arm(&pidCycleTimer, settings.pidPeriodMs, true);
  started = true;
  return true;
}
//-------------------------------------------------------------------------

void DCEngineController::calculateEnginesEncoderFrequencies() {
  long leftEncoder;
  long rightEncoder;
  unsigned long nowMs;
  int encoderCounts[2];
  cli();
  for (unsigned char eCounter = 0; eCounter < engineCounter; eCounter++){
    encoderCounts[eCounter] = engines[eCounter]->encoderCounter;
    engines[eCounter]->encoderCounter = 0;
  }
  long now = millis();
  sei();
  long timeEstimated = now - lastSpeedDetectionTime;
  lastSpeedDetectionTime = now;

  for (unsigned char eCounter = 0; eCounter < engineCounter; eCounter++){
    double currentSpeed = (double)encoderCounts[eCounter] * 1000.0 / ((double)timeEstimated);
    		//* settingsManager->globalSettings->encoderCountsPerRotate);

    if (engines[eCounter]->smoothEncoderValue != 0){
      // рассчет со сглаживанием
    	if (engines[eCounter]->smoothEncoderValue != 0){
    		currentSpeed = ((double)engines[eCounter]->currentEncoderFrequency * (double)engines[eCounter]->smoothEncoderValue
    			+ currentSpeed * (double)(100 - engines[eCounter]->smoothEncoderValue)) / 100.0;
    	}
    }
    engines[eCounter]->currentEncoderFrequency = currentSpeed;
  }
}
//-----------------------------------------------------------------------------------


double DCEngineController::pidCalcUt(EngineControlData *engine) {
  if (engine->requiredEncoderFrequency != 0) {
    // скорость не нулевая
    // рассчет ошибки
    double error = (double)engine->requiredEncoderFrequency - (double)engine->currentEncoderFrequency;

    // П составляющая
    double corrP = deviceInfo->settingsManager->globalSettings->coefP * error;

    // Д составляющая
    double corrD = abs(((double)engine->previousRotateError - error)) < deviceInfo->settingsManager->globalSettings->diffMin
    		? 0
    				: deviceInfo->settingsManager->globalSettings->coefD * ((double)engine->previousRotateError - error);

    // И составляющая
    engine->integralSummator += error;
    if (engine->integralSummator < -deviceInfo->settingsManager->globalSettings->maxAbsISum) {
      engine->integralSummator = -deviceInfo->settingsManager->globalSettings->maxAbsISum;
    } else if (engine->integralSummator > deviceInfo->settingsManager->globalSettings->maxAbsISum) {
      engine->integralSummator = deviceInfo->settingsManager->globalSettings->maxAbsISum;
    }
    double corrI = deviceInfo->settingsManager->globalSettings->coefI * (double)engine->integralSummator;

    double corrTotal = (corrP + corrD + corrI) * deviceInfo->settingsManager->globalSettings->coefE;

    engine->previousRotateError = error;
    engine->pCorr = corrP;
    engine->iCorr = corrI;
    engine->dCorr = corrD;

    return corrTotal;
  } else {
    return 0;
  }

}
//-----------------------------------------------------------------------------------

void DCEngineController::checkAndCorrectEngineSpeedUsingPID(EngineControlData *engine) {
  if (engine->requiredEncoderFrequency != 0) {
    double corrValue = pidCalcUt(engine);

    delay(0);

    int newPower = (double)engine->currentPower + corrValue; // - (double) settings.minPower) * encoderCoef + (double) settings.minPower;
    if (newPower < deviceInfo->settingsManager->globalSettings->minPower) {
      newPower = deviceInfo->settingsManager->globalSettings->minPower;
    } else if (newPower > deviceInfo->settingsManager->globalSettings->maxPower) {
      newPower = deviceInfo->settingsManager->globalSettings->maxPower;
      engine->lowPower = true;
    } else {
      engine->lowPower = false;
    }

    if (engine->currentPower != newPower) {
      enginePowerController->immediatelySetEnginePwer(engine, engine->engineDirection, newPower);
    }
  }
}
//-----------------------------------------------------------------------------------

bool DCEngineController::setEngineSpeed(EngineControlData *engine, unsigned char newDirection, int newSpeed) {
  if (newSpeed == 0){
	  enginePowerController->immediatelySetEnginePwer(engine, engine->engineDirection, 0);
	  engine->engineDirection = M168_DIRECTION_UNDEFINED;
	  engine->requiredEncoderFrequency = 0;
  } else {
    if (engine->engineDirection != newDirection){
    	enginePowerController->immediatelySetEnginePwer(engine, newDirection, 1);
    }
    engine->requiredEncoderFrequency = (double)newSpeed * (double)deviceInfo->settingsManager->globalSettings->encoderCountsPerRotate / 60.0;  // требуемое кол-во импульсов с энкодера
  }
  return true;
}
//-----------------------------------------------------------------------------------

/**
 *  Цикл коррекции мощности двигателей
*/
void DCEngineController::enginesCorrectionWorkCycle(void) {
  calculateEnginesEncoderFrequencies();
  for (unsigned char eCounter = 0; eCounter < engineCounter; eCounter++){
	  if (!engines[eCounter]->encoderFails){
		  checkAndCorrectEngineSpeedUsingPID(engines[eCounter]);
	  }
	  delay(0);
  }
}
//-----------------------------------------------------------------------------------

void DCEngineController::loop(void) {
  if ((millis() - lastTimeCycle) >= settingsManager->globalSettings->pidPeriodMs) {
    lastTimeCycle = millis();
    enginesCorrectionWorkCycle();
    cycleDone = true;
  }
}
//-----------------------------------------------------------------------------------

bool DCEngineController::isCycleDone(void) {
  bool done = cycleDone;
  cycleDone = false;
  return done;
}
//-----------------------------------------------------------------------------------


}
