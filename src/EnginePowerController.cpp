/*
 * EnginePowerController.cpp
 *
 *  Created on: 05.07.2017
 *      Author: jane
 */

#include "EnginePowerController.h"
#include "PwmPCA9685Driver.h"
#include "DeviceInfo.h"
#include "SettingsManager.h"
#include "constUtils.h"
#include "EngineControlData.h"
#include "Logger.h"

namespace tankController{

EnginePowerController::EnginePowerController(PwmPCA9685Driver* pwmPCA9685Driver,
		SettingsManager* settingsManager,
		DeviceInfo* deviceInfo){
	this->pwmPCA9685Driver = pwmPCA9685Driver;
	this->settingsManager = settingsManager;
	this->deviceInfo = deviceInfo;
}
//--------------------------------------------------------------------------

bool EnginePowerController::immediatelySetEnginePwer(EngineControlData *engine, unsigned char newDirection, int newPower){
	  if ((newDirection != M168_DIRECTION_FORWARD) && (newDirection != M168_DIRECTION_BACKWARD)) {
	    return false;
	  }

	  // Если направление двигателя изменилось, то  сначала сначала погасим канал, работавший в предыдущем направлении
	  if (engine->engineDirection != newDirection) {
	    if (engine->engineDirection == M168_DIRECTION_FORWARD) {
	    	pwmPCA9685Driver->setChannelValue(engine->forwardPWMChannel, 0);
	    } else if (engine->engineDirection == M168_DIRECTION_BACKWARD) {
	    	pwmPCA9685Driver->setChannelValue(engine->backwardPWMChannel, 0);
	    }
	  }

	  int engineChannel;
	  if (newDirection == M168_DIRECTION_FORWARD) {
	    engineChannel = engine->forwardPWMChannel;
	  } else {
	    engineChannel = engine->backwardPWMChannel;
	  }

	  engine->engineDirection = newDirection;
	  engine->currentPower = newPower;

	  pwmPCA9685Driver->setChannelValue(engineChannel, newPower);

	  return true;
}
//--------------------------------------------------------------------------

bool EnginePowerController::immediatelySetEnginePwer(int engineIndex, unsigned char newDirection, int newPower){
	  EngineControlData* engine = &deviceInfo->engineControlData[engineIndex];
	  return immediatelySetEnginePwer(engine, newDirection, newPower);
}
//--------------------------------------------------------------------------

bool EnginePowerController::loadPwmChannels(){
	if ((settingsManager->globalSettings->leftBackwardPWMChannel != deviceInfo->engineControlData[LEFT_ENGINE_INDEX].backwardPWMChannel) ||
			(settingsManager->globalSettings->leftForwardPWMChannel != deviceInfo->engineControlData[LEFT_ENGINE_INDEX].forwardPWMChannel) ||
			(settingsManager->globalSettings->rightBackwardPWMChannel != deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].backwardPWMChannel) ||
			(settingsManager->globalSettings->rightForwardPWMChannel != deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].forwardPWMChannel)) {

		if (deviceInfo->engineControlData[LEFT_ENGINE_INDEX].backwardPWMChannel != -1) {
			pwmPCA9685Driver->setChannelValue(deviceInfo->engineControlData[LEFT_ENGINE_INDEX].backwardPWMChannel, 0);
		}
		if (deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].backwardPWMChannel != -1) {
			pwmPCA9685Driver->setChannelValue(deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].backwardPWMChannel, 0);
		}
		if (deviceInfo->engineControlData[LEFT_ENGINE_INDEX].backwardPWMChannel != -1) {
			pwmPCA9685Driver->setChannelValue(deviceInfo->engineControlData[LEFT_ENGINE_INDEX].forwardPWMChannel, 0);
		}
		if (deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].backwardPWMChannel != -1) {
			pwmPCA9685Driver->setChannelValue(deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].forwardPWMChannel, 0);
		}

		pwmPCA9685Driver->setChannelValue(settingsManager->globalSettings->leftBackwardPWMChannel, 0);
		pwmPCA9685Driver->setChannelValue(settingsManager->globalSettings->leftForwardPWMChannel, 0);
		pwmPCA9685Driver->setChannelValue(settingsManager->globalSettings->rightBackwardPWMChannel, 0);
		pwmPCA9685Driver->setChannelValue(settingsManager->globalSettings->rightForwardPWMChannel, 0);

		deviceInfo->engineControlData[LEFT_ENGINE_INDEX].backwardPWMChannel = settingsManager->globalSettings->leftBackwardPWMChannel;
		deviceInfo->engineControlData[LEFT_ENGINE_INDEX].forwardPWMChannel = settingsManager->globalSettings->leftForwardPWMChannel;
		deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].backwardPWMChannel = settingsManager->globalSettings->rightBackwardPWMChannel;
		deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].forwardPWMChannel = settingsManager->globalSettings->rightForwardPWMChannel;

		LOGGER.succed("engine PWM channels applied");
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------

}


