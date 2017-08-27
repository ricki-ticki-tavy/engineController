/*
 * HardwareController.h
 *
 *  Created on: 26.06.2017
 *      Author: jane
 *
 *    Инициализация и управление переферией.
 */

#ifndef HARDWARECONTROLLER_H_
#define HARDWARECONTROLLER_H_

#include "DeviceInfo.h"
#include "PwmPCA9685Driver.h"
#include "SettingsManager.h"
#include "TahometerController.h"
#include "DCEngineController.h"
#include "EnginePowerController.h"

#define EXTENDER_0_ADDR 0x20
#define EXTENDER_1_ADDR 0x21
#define PWM_ADDR 0x40

namespace tankController{

class HardwareController{
public:
	DeviceInfo* deviceInfo;
	EnginePowerController* enginePowerController;
	PwmPCA9685Driver* pwmPCA9685Driver;

	HardwareController(SettingsManager* settingsManager);

	/**
	 * Выбрать аналоговый канал для оцифровки
	 */
	int readAnalogChannel(unsigned char channel);

	/**
	 * Цикл работы с вводом / выводом, телеметрией и прочим
	 */
	void loop();

	void writePortExtender0();
	void writePortExtender1();
	int readPortExtender1();

	unsigned char portExt0High;
private:
	unsigned char portExt0Low;
	unsigned char portExt1Low;
	unsigned char portExt1High;

	float smaPeakCoef1;
	float smaPeakCoef2;
	float smaAvgCoef1;
	float smaAvgCoef2;

	SettingsManager* settingsManager;
	TahometerController* tahometerController;
	DCEngineController* dcEngineController;

	unsigned long lastTelemetricReadTime;

	void initI2C();
	void initDisplay();

	/**
	 * Проврка наличия I2C девайса по адресу
	 */
	bool checkI2CDevice(unsigned char deviceAddr);

	void testAnalogInputs();
	void initPIDController();

	/**
	 * Прочитать значение со средней скользящей
	 */
	int calcSmaPeak(int newValue, int priorValue);
	int calcSmaAvg(int newValue, int priorValue);

	/**
	 * Обновить значение телеметрии
	 */
	void refreshTelemetryData();

	/**
	 * Логировать данные двигателя.
	 */
	void loogEngineStatistics(int engineIndex);

};

//extern HardwareController HARDWARE_CONTROLLER;

}



#endif /* HARDWARECONTROLLER_H_ */
