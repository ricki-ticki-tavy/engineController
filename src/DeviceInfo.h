/*
 * DeviceInfo.h
 *
 *  Created on: 30.06.2017
 *      Author: jane
 */

#ifndef DEVICEINFO_H_
#define DEVICEINFO_H_

#include "EngineControlData.h"
#include "EngineStatisticData.h"
#include "SettingsManager.h"

namespace tankController{

class DeviceInfo{
public:
	bool portExtender0Found;
	bool portExtender1Found;
	bool pwmFound;
	bool leftEngineEncoderFound;
	bool rightEngineEncoderFound;

	int mainCurrentPeak;
	int mainCurrentAvg;
	int mainVoltage;
	int mbTempr;

	int leftEngineCurrentPeak;
	int leftEngineCurrentAvg;
	int leftEngineVoltage;
	int leftDriverTempr;
	int leftEngineTempr;

	int rightEngineCurrentPeak;
	int rightEngineCurrentAvg;
	int rightEngineVoltage;
	int rightDriverTempr;
	int rightEngineTempr;

	unsigned long telemetryCounter;

	EngineControlData engineControlData[2];

	EngineStatisticData *engineStatistic[2];

	SettingsManager *settingsManager;

	DeviceInfo(SettingsManager *settingsManager);

	/**
	 * Добавить информацию в лог дыигателя
	 */
	void addEngineStatisticData(char engineIndex, int requiredEncoderValue, int currEncoderValue, int error,
			int pCorr, int dCorr, int iCorr, int power, int voltage, int current, int drvTempr, int engTempr);
};

}


#endif /* DEVICEINFO_H_ */
