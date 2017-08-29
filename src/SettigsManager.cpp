/*
 * SettigsManager.cpp
 *
 *  Created on: 25.06.2017
 *      Author: jane
 */

#include "SettingsManager.h"
#include "GlobalSettings.h"
#include <EEPROM.h>
#include <string.h>
#include "Logger.h"
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "WebUtils.h"
#include "PwmPCA9685Driver.h"
#include "EnginePowerController.h"

namespace tankController{

SettingsManager::SettingsManager(){
	EEPROM.begin(sizeof(GlobalSettings));
	globalSettings = new GlobalSettings;

	char* dataPtr = (char*)globalSettings;

	for (int addr = 0; addr < sizeof(GlobalSettings); addr++){
		dataPtr[addr] = EEPROM.read(addr);
	}

	// Проверим, что инициализирована память

	if ((globalSettings->sig1 != 0x55) || (globalSettings->sig2 != 0xAA) || (globalSettings->version != TANK_CONTROLLER_SETTINGS_VERSION)){

		LOGGER.info("EEPROM init start");

		// инитим настройки
		globalSettings->sig1 = 0x55;
		globalSettings->sig2 = 0xAA;

		globalSettings->version = TANK_CONTROLLER_SETTINGS_VERSION;

		globalSettings->startWifi = true;
		globalSettings->wifiStarted = false;

		char* buffer = TANK_CONTROLLER_DEFAULT_WIFI_PASSWORD;
		memcpy(&globalSettings->wifiPassword, buffer, strlen(buffer) + 1);

		char* buffer1 = TANK_CONTROLLER_DEFAULT_WIFI_SSID;
		memcpy(&globalSettings->wifiSSID, buffer1, strlen(buffer1) + 1);

		char* buffer2 = TANK_CONTROLLER_DEFAULT_HOST_NAME;
		memcpy(&globalSettings->hostName, buffer2, strlen(buffer2) + 1);

		globalSettings->coefP = 0.8;
		globalSettings->coefD = -1.6;
		globalSettings->coefI = 0.01;
		globalSettings->coefE = 0.1;
		globalSettings->maxAbsISum = 600.0;
		globalSettings->diffMin = 3;
		globalSettings->pidPeriodMs = 20;

		globalSettings->leftForwardPWMChannel = 0;
		globalSettings->leftBackwardPWMChannel = 1;
		globalSettings->rightForwardPWMChannel = 2;
		globalSettings->rightBackwardPWMChannel = 3;
		globalSettings->pwmFrequency = 870;

		globalSettings->leftEncoderGpioPin = 3;
		globalSettings->rightEncoderGpioPin = 1;


		globalSettings->maxPower = 3800;
		globalSettings->minPower = 800;

		globalSettings->maxRotateSpeed = 6000;

		globalSettings->encoderCountsPerRotate = 100;

		globalSettings->maxContinuousDriverCurrentMa = 3000;
		globalSettings->maxPeakDriverCurrentMa = 4500;
		globalSettings->maxDriverTemperature = 80;
		globalSettings->maxEngineTemperature = 80;
		globalSettings->maxMBCurrentMa = 400;
		globalSettings->minEngineVoltageMv = 6000;
		globalSettings->minTotalVoltageMv = 9200;

		globalSettings->telemetryIntervalMs = 100;
		globalSettings->SMAPeakValues = 2;
		globalSettings->SMAAvgValues = 6;

		globalSettings->enginesEnabled = true;

		/**
		 * Для термодатчиков через 50 к на землю. чредний вывод идет на вход платы
		 */

		globalSettings->mainCurrentAnalogChannel = 3; // 9
		globalSettings->mainVoltageAnalogChannel = 6; // 1
		globalSettings->mbTemprAnalogChannel = 7;     // 2

		globalSettings->leftEngineCurrentAnalogChannel = 0; // 10
		globalSettings->leftEngineVoltageAnalogChannel = 4; // 3
		globalSettings->leftDriverTemprAnalogChannel = 5;   // 4
		globalSettings->leftEngineTemprAnalogChannel = 9;   // 5

		globalSettings->rightEngineCurrentAnalogChannel = 1; //11
		globalSettings->rightEngineVoltageAnalogChannel = 8; // 6
		globalSettings->rightDriverTemprAnalogChannel = 11;  // 7
		globalSettings->rightEngineTemprAnalogChannel = 10; // 8

		globalSettings->enginesLogBuffer = 90;

		saveSettings();
		LOGGER.finished("EEPROM init done");
	}
}
//---------------------------------------------------------

void SettingsManager::saveSettings(){
	LOGGER.info("saving settings...");

	char* dataPtr = (char*)globalSettings;
	for (int addr = 0; addr < sizeof(GlobalSettings); addr++){
		EEPROM.write(addr, dataPtr[addr]);
	}
    EEPROM.commit();

    LOGGER.finished("settings saved");
}
//---------------------------------------------------------


}


