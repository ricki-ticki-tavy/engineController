/*
 * WifiManager.cpp
 *
 *  Created on: 26.06.2017
 *      Author: jane
 */

#include "WifiManager.h"
#include "Logger.h"
#include "SettingsManager.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "WebApplication.h"
#include <Arduino.h>
#include "PwmPCA9685Driver.h"
#include "EnginePowerController.h"


namespace tankController{

WifiManager::WifiManager(DeviceInfo* deviceInfo,
		EnginePowerController* enginePowerController,
		PwmPCA9685Driver* pwmPCA9685Driver){
	this->deviceInfo = deviceInfo;
	this->enginePowerController = enginePowerController;
	this->pwmPCA9685Driver = pwmPCA9685Driver;

	LOGGER.info("init WifiManager started");

	if (deviceInfo->settingsManager->globalSettings->startWifi != deviceInfo->settingsManager->globalSettings->wifiStarted){
		// нужно либо стопить либо запускать wifi
		if (!deviceInfo->settingsManager->globalSettings->startWifi) {
			// надо гасить сеть

		} else {
			// Включение сети
			LOGGER.info("init WiFi AP");

			WiFi.mode(WIFI_AP);
			WiFi.softAP(deviceInfo->settingsManager->globalSettings->wifiSSID, deviceInfo->settingsManager->globalSettings->wifiPassword);

			LOGGER.finished("init WiFi AP done");
		}

		//  Сохраним настройки
		deviceInfo->settingsManager->globalSettings->wifiStarted = deviceInfo->settingsManager->globalSettings->startWifi;
		deviceInfo->settingsManager->saveSettings();
	}

	// выполним запуск служб, если сеть включена
	if (deviceInfo->settingsManager->globalSettings->startWifi) {
		runNetwork();
	}

	LOGGER.finished("init WifiManager done");

}
//---------------------------------------------------------------------

//---------------------------------------------------------------------

void WifiManager::runNetwork(){
	LOGGER.info("starting network...");
	LOGGER.add("start DNS");
	MDNS.begin(deviceInfo->settingsManager->globalSettings->hostName);
	MDNS.addService("http", "tcp", 80);

	LOGGER.add("start Web server");
	server = new ESP8266WebServer(80) ;

	LOGGER.add("start Updater");
	httpUpdater = new ESP8266HTTPUpdateServer();
	httpUpdater->setup(server);

	LOGGER.add("start Webapplication");
	webApplication = new WebApplication(server, deviceInfo, enginePowerController, pwmPCA9685Driver);

	LOGGER.add("web begin");
	server->begin();

	LOGGER.finished("network started");

}
//---------------------------------------------------------------------

//void WifiManager::init(){
//	LOGGER.info("init WifiManager started");
//
//	if (settingsManager->globalSettings->startWifi != settingsManager->globalSettings->wifiStarted){
//		// нужно либо стопить либо запускать wifi
//		if (!settingsManager->globalSettings->startWifi) {
//			// надо гасить сеть
//
//		} else {
//			// Включение сети
//			LOGGER.info("init WiFi AP");
//
//			WiFi.mode(WIFI_AP);
//			WiFi.softAP(settingsManager->globalSettings->wifiSSID, settingsManager->globalSettings->wifiPassword);
//
//			LOGGER.finished("init WiFi AP done");
//		}
//
//		//  Сохраним настройки
//		settingsManager->globalSettings->wifiStarted = settingsManager->globalSettings->startWifi;
//		settingsManager->saveSettings();
//	}
//
//	// выполним запуск служб, если сеть включена
//	if (settingsManager->globalSettings->startWifi) {
//		runNetwork();
//	}
//
//	LOGGER.finished("init WifiManager done");

//}
//---------------------------------------------------------------------

void WifiManager::loop(){
	if (deviceInfo->settingsManager->globalSettings->startWifi) {
		server->handleClient();
	}
}

}


