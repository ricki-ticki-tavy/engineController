/*
 * WebApplication.h
 *
 *  Created on: 27.06.2017
 *      Author: jane
 */

#ifndef WEBAPPLICATION_H_
#define WEBAPPLICATION_H_

#include <ESP8266WebServer.h>
#include "DeviceInfo.h"
#include "SettingsManager.h"
#include "PwmPCA9685Driver.h"
#include "EnginePowerController.h"

namespace tankController{

class WebApplication{
public:
	WebApplication(ESP8266WebServer* server,
			DeviceInfo* deviceInfo_,
			EnginePowerController* enginePowerController,
			PwmPCA9685Driver* pwmPCA9685Driver);

	static void handleLog();
	static void handleTelemetry();
	static void handleTelemetryPage();
	static void handleTelemetryJsPage();
	static void handleSettings();
	static void handleJsSettingsLibrary1();
	static void handleJsSettingsLibrary2();
	static void handleJsSettingsLibrary3();
	static void handleSettingsPage();
	static void handleJsChartLibraryPage();
	static void handleEngineStatisticData();
	static void handleEngineStatistic();
	static void handleJsCommonLibrary();

	static void handleRootPage();

	static void webSaveSettings();

	static void answerPage(const char page[], String mimeType);


	};

}


#endif /* WEBAPPLICATION_H_ */
