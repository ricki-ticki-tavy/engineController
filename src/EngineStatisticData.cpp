/*
 * EngineStatisticData.cpp
 *
 *  Created on: 09.07.2017
 *      Author: jane
 */

#include "EngineStatisticData.h"
#include "GlobalSettings.h"
#include <Arduino.h>

namespace tankController{

EngineStatisticData::EngineStatisticData(GlobalSettings* globalSettings){
	engineStatisticsHeadIndex = 0;
	engineStatisticsTaleIndex = 0;

	elementBufferSize = globalSettings->enginesLogBuffer;

	statistics = (EngineStatisticRecord*)malloc(sizeof(EngineStatisticRecord) * elementBufferSize);
}
//---------------------------------------------------------------------------------------------


void EngineStatisticData::addEngineStatisticData(int requiredEncoderValue, int currEncoderValue, int error,
		int pCorr, int dCorr, int iCorr, int power, int voltage, int current, int drvTempr, int engTempr){

	// Запишем данные
	statistics[engineStatisticsHeadIndex].requiredEncoderValue = requiredEncoderValue;
	statistics[engineStatisticsHeadIndex].currEncoderValue = currEncoderValue;
	statistics[engineStatisticsHeadIndex].error = error;

	statistics[engineStatisticsHeadIndex].pCorr = pCorr;
	statistics[engineStatisticsHeadIndex].dCorr = dCorr;
	statistics[engineStatisticsHeadIndex].iCorr = iCorr;

	statistics[engineStatisticsHeadIndex].power = power;

	statistics[engineStatisticsHeadIndex].voltage = voltage;
	statistics[engineStatisticsHeadIndex].current = current;

	statistics[engineStatisticsHeadIndex].drvTempr = drvTempr;
	statistics[engineStatisticsHeadIndex].engTempr = engTempr;

	// закругление головы
	if (++engineStatisticsHeadIndex >= elementBufferSize) {
		engineStatisticsHeadIndex = 0;
	}

	// проверим не надо ли резать хвост, если нет места для головы
	if (engineStatisticsHeadIndex == engineStatisticsTaleIndex) {
		// Голова догнала хвост. Режем хвост
		if (++engineStatisticsTaleIndex >= elementBufferSize) {
			engineStatisticsTaleIndex = 0;
		}
	}
}
//---------------------------------------------------------------------------------------------


}


