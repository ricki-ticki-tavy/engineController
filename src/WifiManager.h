/*
 * WifiManager.h
 *
 *  Created on: 26.06.2017
 *      Author: jane
 */

#ifndef WIFIMANAGER_H_
#define WIFIMANAGER_H_

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "WebApplication.h"
#include "SettingsManager.h"
#include "PwmPCA9685Driver.h"
#include "EnginePowerController.h"

namespace tankController{

class WifiManager{
private:
	ESP8266WebServer* server;
	ESP8266HTTPUpdateServer* httpUpdater;
	WebApplication* webApplication;

	DeviceInfo* deviceInfo;
	EnginePowerController* enginePowerController;
	PwmPCA9685Driver* pwmPCA9685Driver;

public:
	WifiManager(DeviceInfo* deviceInfo,
			EnginePowerController* enginePowerController,
			PwmPCA9685Driver* pwmPCA9685Driver);

	void init();
	void runNetwork();

	void loop();
};

}


#endif /* WIFIMANAGER_H_ */
