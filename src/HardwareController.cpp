/*
 * HardwareController.cpp
 *
 *  Created on: 26.06.2017
 *      Author: jane
 */

#include "HardwareController.h"
#include "Logger.h"
#include "Wire.h"
#include "DeviceInfo.h"
#include "PwmPCA9685Driver.h"
#include "SettingsManager.h"
#include "DCEngineController.h"
#include "EngineControlData.h"
#include "constUtils.h"
#include "EngineStatisticData.h"

namespace tankController{

HardwareController::HardwareController(SettingsManager* settingsManager){
	this->settingsManager = settingsManager;
	deviceInfo = new DeviceInfo(settingsManager);

	portExt0Low = 0;
	portExt0High = 0;
	portExt1Low = 0xFF;
	portExt1High = 0xFF;

	LOGGER.info("init HardwareController started");

	tahometerController = new TahometerController(deviceInfo, settingsManager);

	initI2C();

	// Тест аналоговых входов
	if (deviceInfo->portExtender0Found){
		testAnalogInputs();
	}

	// Переводим в режим ввода порт 2
	if (deviceInfo->portExtender1Found){
		writePortExtender1();
	}

	// Останавливаем все ШИМ
	if (deviceInfo->pwmFound){
		pwmPCA9685Driver = new PwmPCA9685Driver(0x40);
		pwmPCA9685Driver->setPwmFrequency(settingsManager->globalSettings->pwmFrequency);
		pwmPCA9685Driver->setChannelValue(11, 2048);

		enginePowerController = new EnginePowerController(pwmPCA9685Driver, settingsManager, deviceInfo);
		initPIDController();

//		enginePowerController->immediatelySetEnginePwer(0, M168_DIRECTION_FORWARD, 700);
		dcEngineController->setEngineSpeed(&deviceInfo->engineControlData[LEFT_ENGINE_INDEX], M168_DIRECTION_FORWARD, 3000);
//		deviceInfo->engineControlData[LEFT_ENGINE_INDEX].requiredEncoderFrequency = 12000;
//		deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].requiredEncoderFrequency = 5000;
	}

	LOGGER.finished("init HardwareController done");
}
//-----------------------------------------------------------------------------

void HardwareController::initPIDController(){
	LOGGER.info("initializing engine PID regulator...");

	dcEngineController = new DCEngineController(settingsManager, deviceInfo, enginePowerController);

	dcEngineController->addEngine(&deviceInfo->engineControlData[LEFT_ENGINE_INDEX]);
	dcEngineController->addEngine(&deviceInfo->engineControlData[RIGHT_ENGINE_INDEX]);

	// PWM каналы для двигателей. Реальные значения будут загружены при инициализации
	deviceInfo->engineControlData[LEFT_ENGINE_INDEX].backwardPWMChannel = -1;
	deviceInfo->engineControlData[LEFT_ENGINE_INDEX].forwardPWMChannel = -1;
	deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].forwardPWMChannel = -1;
	deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].forwardPWMChannel = -1;

	enginePowerController->loadPwmChannels();

	bool result = dcEngineController->start();

	LOGGER.finishedOrError(result, "PID init finished regulator...");

}
//-----------------------------------------------------------------------------

bool HardwareController::checkI2CDevice(unsigned char deviceAddr){
	Wire.beginTransmission(deviceAddr);
	return Wire.endTransmission() == 0;
}
//-----------------------------------------------------------------------------

void HardwareController::writePortExtender0(){
	Wire.beginTransmission(EXTENDER_0_ADDR);
	Wire.write(portExt0Low);
	Wire.write(portExt0High);
	Wire.endTransmission();
}
//-----------------------------------------------------------------------------

void HardwareController::writePortExtender1(){
	Wire.beginTransmission(EXTENDER_1_ADDR);
	Wire.write(portExt1Low);
	Wire.write(portExt1High);
	Wire.endTransmission();
}
//-----------------------------------------------------------------------------

int HardwareController::readPortExtender1(){
	if (deviceInfo->portExtender1Found){
		Wire.requestFrom(EXTENDER_1_ADDR, 2);
		while (Wire.available() < 2){
		  delay(0);
		}

		return  ((int)Wire.read()) | (((int)Wire.read()) << 8);
	} else {
		return 0;
	}
}
//-----------------------------------------------------------------------------

void HardwareController::initI2C(){
	Wire.begin(4, 5);
	Wire.setClock(200000);

	delay(5);

	deviceInfo->portExtender0Found = checkI2CDevice(EXTENDER_0_ADDR);
	delay(5);
	deviceInfo->portExtender1Found = checkI2CDevice(EXTENDER_1_ADDR);
	delay(5);
	deviceInfo->pwmFound = checkI2CDevice(PWM_ADDR);
	delay(5);

	LOGGER.succedOrError(deviceInfo->portExtender0Found, "port extender 20h " + String(deviceInfo->portExtender0Found ? "" : "not ") + "found");
	LOGGER.succedOrError(deviceInfo->portExtender1Found, "port extender 21h " + String(deviceInfo->portExtender1Found ? "" : "not ") + "found");
	LOGGER.succedOrError(deviceInfo->pwmFound, "PWM driver " + String(deviceInfo->pwmFound ? "" : "not ") + "found");
}
//-----------------------------------------------------------------------------


int HardwareController::readAnalogChannel(unsigned char channel){
	portExt0Low = portExt0Low & 0xf0 | channel;
	writePortExtender0();
	delay(0);
	delayMicroseconds(10);
	return analogRead(0);
}
//-----------------------------------------------------------------------------

void HardwareController::testAnalogInputs(){
	LOGGER.add("test analog inputs...");
	if (deviceInfo->portExtender0Found){
		for (char i = 0; i < 13; i++){
			LOGGER.add("channel " + String((int)i) + " = " + String(readAnalogChannel(i)));
		}
	}
}
//-----------------------------------------------------------------------------

int HardwareController::calcSmaPeak(int newValue, int priorValue){
	//return newValue;
	return (int)(((float)priorValue) * smaPeakCoef2 + ((float)newValue) * smaPeakCoef1);
}
//---------------------------------------------------------------------------

int HardwareController::calcSmaAvg(int newValue, int priorValue){
	//return newValue;
	return (int)(((float)priorValue) * smaAvgCoef2 + ((float)newValue) * smaAvgCoef1);
}
//---------------------------------------------------------------------------

void HardwareController::refreshTelemetryData(){
	smaPeakCoef1 = (1.0 / settingsManager->globalSettings->SMAPeakValues);
	smaPeakCoef2 = 1 - smaPeakCoef1;
	smaAvgCoef1 = (1.0 / settingsManager->globalSettings->SMAAvgValues);
	smaAvgCoef2 = 1 - smaAvgCoef1;

	int temp = readAnalogChannel(settingsManager->globalSettings->mainVoltageAnalogChannel) << 1;
	deviceInfo->mainVoltage = calcSmaAvg(temp, deviceInfo->mainVoltage);

	temp = readAnalogChannel(settingsManager->globalSettings->mainCurrentAnalogChannel);
	deviceInfo->mainCurrentAvg = calcSmaAvg(temp, deviceInfo->mainCurrentAvg);
	deviceInfo->mainCurrentPeak = calcSmaPeak(temp, deviceInfo->mainCurrentPeak);

	temp = readAnalogChannel(settingsManager->globalSettings->mbTemprAnalogChannel) >> 1;
	deviceInfo->mbTempr = calcSmaAvg(temp, deviceInfo->mbTempr);

	temp = readAnalogChannel(settingsManager->globalSettings->leftEngineCurrentAnalogChannel);
	deviceInfo->leftEngineCurrentAvg = calcSmaAvg(temp, deviceInfo->leftEngineCurrentAvg);
	deviceInfo->leftEngineCurrentPeak = calcSmaPeak(temp, deviceInfo->leftEngineCurrentPeak);

	temp = readAnalogChannel(settingsManager->globalSettings->leftEngineTemprAnalogChannel) << 1;
	deviceInfo->leftEngineTempr = calcSmaAvg(temp, deviceInfo->leftEngineTempr);

	temp = readAnalogChannel(settingsManager->globalSettings->leftDriverTemprAnalogChannel) >> 1;
	deviceInfo->leftDriverTempr = calcSmaAvg(temp, deviceInfo->leftDriverTempr);

	temp = readAnalogChannel(settingsManager->globalSettings->leftEngineVoltageAnalogChannel) << 1;
	deviceInfo->leftEngineVoltage = calcSmaAvg(temp, deviceInfo->leftEngineVoltage);

	temp = readAnalogChannel(settingsManager->globalSettings->rightEngineCurrentAnalogChannel);
	deviceInfo->rightEngineCurrentAvg = calcSmaAvg(temp, deviceInfo->rightEngineCurrentAvg);
	deviceInfo->rightEngineCurrentPeak = calcSmaPeak(temp, deviceInfo->rightEngineCurrentPeak);

	temp = readAnalogChannel(settingsManager->globalSettings->rightEngineTemprAnalogChannel) >> 1;
	deviceInfo->rightEngineTempr = calcSmaAvg(temp, deviceInfo->rightEngineTempr);

	temp = readAnalogChannel(settingsManager->globalSettings->rightDriverTemprAnalogChannel) >> 1;
	deviceInfo->rightDriverTempr = calcSmaAvg(temp, deviceInfo->rightDriverTempr);

	temp = readAnalogChannel(settingsManager->globalSettings->rightEngineVoltageAnalogChannel) << 1;
	deviceInfo->rightEngineVoltage = calcSmaAvg(temp, deviceInfo->rightEngineVoltage);

	deviceInfo->telemetryCounter++;
}
//---------------------------------------------------------------------------

void HardwareController::loogEngineStatistics(int engineIndex){
	EngineControlData *engineData = &deviceInfo->engineControlData[engineIndex];
	int voltage;
	int drvTempr;
	int engTempr;
	int current;
	if (engineIndex == LEFT_ENGINE_INDEX){
		voltage = deviceInfo->leftEngineVoltage;
		drvTempr = deviceInfo->leftDriverTempr;
		engTempr = deviceInfo->leftEngineTempr;
	} else if (engineIndex == RIGHT_ENGINE_INDEX) {
		voltage = deviceInfo->rightEngineVoltage;
		drvTempr = deviceInfo->rightDriverTempr;
		engTempr = deviceInfo->rightEngineTempr;
	}
	deviceInfo->addEngineStatisticData(engineIndex,
			engineData->requiredEncoderFrequency >> 6, engineData->currentEncoderFrequency >> 6,
			170 + ((engineData->previousRotateError) >> 6),
			170 + (engineData->pCorr >> 5), 170 + (engineData->dCorr >> 4), 170 + (engineData->integralSummator * settingsManager->globalSettings->coefI) ,engineData->currentPower >> 4,
			voltage / 3, 0, drvTempr >> 1, engTempr >> 1);

}
//---------------------------------------------------------------------------

void HardwareController::loop(){
	if (deviceInfo->portExtender0Found){
		unsigned long curMillis = millis();
		if ((curMillis < lastTelemetricReadTime) || ((curMillis - lastTelemetricReadTime) >= (unsigned long)settingsManager->globalSettings->telemetryIntervalMs)){
			lastTelemetricReadTime = curMillis;
			refreshTelemetryData();
		}

		dcEngineController->loop();

		if (dcEngineController->isCycleDone()){
			loogEngineStatistics(LEFT_ENGINE_INDEX);
			loogEngineStatistics(RIGHT_ENGINE_INDEX);
		}
	}
}
//-----------------------------------------------------------------------------

}


