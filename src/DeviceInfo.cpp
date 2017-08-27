/*
 * DeviceInfo.cpp
 *
 *  Created on: 09.07.2017
 *      Author: jane
 */

#include "DeviceInfo.h"
#include "EngineControlData.h"
#include "EngineStatisticData.h"
#include "Logger.h"
#include "constUtils.h"
#include "SettingsManager.h"

namespace tankController{

DeviceInfo::DeviceInfo(SettingsManager *settingsManager){
	this->settingsManager = settingsManager;
	telemetryCounter = 0;

	// статистика для движков
	engineStatistic[0] = new EngineStatisticData(settingsManager->globalSettings);
	engineStatistic[1] = new EngineStatisticData(settingsManager->globalSettings);
}
//---------------------------------------------------------------------------------------------

void DeviceInfo::addEngineStatisticData(char engineIndex, int requiredEncoderValue, int currEncoderValue, int error,
		int pCorr, int dCorr, int iCorr, int power, int voltage, int current, int drvTempr, int engTempr){

	engineStatistic[engineIndex]->addEngineStatisticData(requiredEncoderValue, currEncoderValue, error, pCorr,
			dCorr, iCorr, power, voltage, current, drvTempr, engTempr);
}


}

