/*
 * EngineStatisticRecord.h
 *
 *  Created on: 08.07.2017
 *      Author: jane
 *
 *  Статистика по двигателям. Запись лога
 */

#ifndef ENGINESTATISTICRECORD_H_
#define ENGINESTATISTICRECORD_H_

#include "GlobalSettings.h"

namespace tankController{

typedef struct __attribute__((packed)) EngineStatisticRecord{
	unsigned int requiredEncoderValue;
	unsigned int currEncoderValue;
	int error;
	int pCorr;
	int iCorr;
	int dCorr;
	unsigned int power;
	unsigned int voltage;
	unsigned int current;
	int drvTempr;
	int engTempr;
};
//********************************************************

class EngineStatisticData{
public:

	int elementBufferSize;

	EngineStatisticRecord *statistics;

	int engineStatisticsHeadIndex;
	int engineStatisticsTaleIndex;

	EngineStatisticData(GlobalSettings* globalSettings);

	/**
	 * Добавить информацию в лог дыигателя
	 */
	void addEngineStatisticData(int requiredEncoderValue, int currEncoderValue, int error,
			int pCorr, int dCorr, int iCorr, int power, int voltage, int current, int drvTempr, int engTempr);
};

}


#endif /* ENGINESTATISTICRECORD_H_ */
