/*
 * SettingsManager.h
 *
 *  Created on: 25.06.2017
 *      Author: jane
 */

#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include "GlobalSettings.h"
#include <ESP8266WebServer.h>
#include "PwmPCA9685Driver.h"

namespace tankController{

#define TANK_CONTROLLER_SETTINGS_VERSION 01
#define TANK_CONTROLLER_DEFAULT_WIFI_PASSWORD "90309030\x00"
#define TANK_CONTROLLER_DEFAULT_WIFI_SSID "tankEngineController\x00"
#define TANK_CONTROLLER_DEFAULT_HOST_NAME "control\x00"

class SettingsManager{
public:
	GlobalSettings *globalSettings;

	SettingsManager();

	void saveSettings();

};

}

#endif /* SETTINGSMANAGER_H_ */
