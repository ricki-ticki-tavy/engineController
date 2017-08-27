/*
 * TahometerController.cpp
 *
 *  Created on: 02.07.2017
 *      Author: jane
 */

#include "TahometerController.h"
#include "DeviceInfo.h"
#include  <Arduino.h>
#include "SettingsManager.h"
#include "GlobalSettings.h"
#include "Logger.h"
#include "EngineControlData.h"
#include "constUtils.h"

namespace tankController{

DeviceInfo* taxo_deviceInfo;
SettingsManager* taxo_settingsManager;

TahometerController::TahometerController(DeviceInfo* deviceInfo_, SettingsManager* settingsManager_){
	LOGGER.add("start tahometer");
	taxo_deviceInfo = deviceInfo_;
	taxo_settingsManager = settingsManager_;

	pinMode(taxo_settingsManager->globalSettings->leftEncoderGpioPin, INPUT);
	pinMode(taxo_settingsManager->globalSettings->rightEncoderGpioPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(taxo_settingsManager->globalSettings->leftEncoderGpioPin), leftEncoderCounter, FALLING);
    attachInterrupt(digitalPinToInterrupt(taxo_settingsManager->globalSettings->rightEncoderGpioPin), rightEncoderCounter, FALLING);

    taxo_deviceInfo->engineControlData[0].encoderCounter = 0;
    taxo_deviceInfo->engineControlData[1].encoderCounter = 0;

	LOGGER.add("tahometer obtained IRQ");
}
//-----------------------------------------------------------------------------

void TahometerController::leftEncoderCounter(){
	taxo_deviceInfo->engineControlData[LEFT_ENGINE_INDEX].encoderCounter++;
}
//-----------------------------------------------------------------------------

void TahometerController::rightEncoderCounter(){
	taxo_deviceInfo->engineControlData[RIGHT_ENGINE_INDEX].encoderCounter++;
}


}


