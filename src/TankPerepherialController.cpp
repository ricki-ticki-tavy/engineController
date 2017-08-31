/*
 * TankPerepherialController.cpp
 *
 *  Created on: 26.06.2017
 *      Author: jane
 */
#include "TankPerepherialController.h"
#include "Logger.h"
#include "SettingsManager.h"
#include "HardwareController.h"
#include "WifiManager.h"
#include <Arduino.h>
#include "DeviceInfo.h"

namespace tankController{

TankPerepherialController::TankPerepherialController(){
	LOGGER.add("Begin log");

	settingsManager = new SettingsManager();

	hardwareController = new HardwareController(settingsManager);

	wifiManager = new WifiManager(hardwareController->deviceInfo, hardwareController->enginePowerController, hardwareController->pwmPCA9685Driver);

	spiSlaveListener = new SpiSlaveListener(hardwareController->deviceInfo, hardwareController->enginePowerController);
	spiSlaveListener->init();

//pinMode(12, OUTPUT);
}

unsigned long ddd = 0;
bool stat = false;
int pw_val = 200;
int pw_add = 50;

void TankPerepherialController::loop(){
	wifiManager->loop();
	hardwareController->loop();

		if ((millis() - ddd) > 40) {
			ddd = millis();
			hardwareController->pwmPCA9685Driver->setChannelValue(4, pw_val);
			if ((pw_val > 4000) || (pw_val < 60)) {
				pw_add = -pw_add;
			}
			pw_val += pw_add;
//
//			int ii = pw_val >> 4;
//			hardwareController->deviceInfo->addEngineStatisticData(0, ii, ii, ii, ii, ii, ii, ii, ii, ii);
//			hardwareController->deviceInfo->addEngineStatisticData(1, ii, ii, ii, ii, ii, ii, ii, ii, ii);
////			deviceInfo->addEngineStatisticData(1, ii, ii, ii, ii, ii, ii, ii);
//
		}


//	if ((millis() - ddd) > 10) {
//		ddd = millis();
//		stat = !stat;
//		digitalWrite(12, stat);
//		hardwareController->pwmPCA9685Driver->setChannelValue(2, pw_val);
//
//		if (pw_val == 200) {
//			pw_val = 2000;
//		} else {
//			pw_val = 200;
//		}
//
//		if (hardwareController->portExt0High) {
//			hardwareController->portExt0High = 0;
//		} else {
//			hardwareController->portExt0High = 0xff;
//		}
//		hardwareController->writePortExtender0();
//	}
}

}


