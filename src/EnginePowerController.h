/*
 * EnginePowerController.h
 *
 *  Created on: 05.07.2017
 *      Author: jane
 *
 *
 *      Класс управления PWM двигателей
 */

#ifndef ENGINEPOWERCONTROLLER_H_
#define ENGINEPOWERCONTROLLER_H_

#include "PwmPCA9685Driver.h"
#include "SettingsManager.h"
#include "DeviceInfo.h"
#include "EngineControlData.h"

namespace tankController{

class EnginePowerController{
public:
	EnginePowerController(PwmPCA9685Driver* pwmPCA9685Driver, SettingsManager* settingsManager, DeviceInfo* deviceInfo);

	/**
	 * Подать указанную мощность на двигатель
	 */
	bool immediatelySetEnginePwer(int engineIndex, unsigned char newDirection, int newPower);
	bool immediatelySetEnginePwer(EngineControlData *engine, unsigned char newDirection, int newPower);

	/**
	 * Загрузить для двух (всех) дыигателей каналы PWM. при загрузке выполняется останов ВСЕХ старых и новых каналов,
	 * если изменился хотя бы один
	 */
	bool loadPwmChannels();
private:
	PwmPCA9685Driver* pwmPCA9685Driver;
	SettingsManager* settingsManager;
	DeviceInfo* deviceInfo;
};

}


#endif /* ENGINEPOWERCONTROLLER_H_ */
