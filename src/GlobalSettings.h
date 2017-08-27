/*
 * GlobalSettings.h
 *
 *  Created on: 25.06.2017
 *      Author: jane
 */

#ifndef GLOBALSETTINGS_H_
#define GLOBALSETTINGS_H_

namespace tankController{

struct GlobalSettings{
	/**
	 * сигнатура, что память инициализарована 0xAA 0x55
	 */
	char sig1;
	char sig2;

	/**
	 * Версия.
	 */
	char version;

	/**
	 * Признак, что WiFi должен быть включен
	 */
	bool startWifi;

	/**
	 * признак, что WiFi уже инициализирован и повторно при запуске его инитить не надо.
	 */
	bool wifiStarted;

	/**
	 * Параметры настройки WiFi
	 */
	char wifiPassword[10];
	char wifiSSID[26];
	char hostName[18];

	/**
	 * Коэффициенты настройки регулятора двигателей
	 */
	double coefP;
	double coefD;
	double coefI;
	double coefE;
	double maxAbsISum;
	int pidPeriodMs;
	int diffMin;

	/**
	 * Настройки каналов для двигателей
	 */
	char leftForwardPWMChannel;
	char leftBackwardPWMChannel;
	char rightForwardPWMChannel;
	char rightBackwardPWMChannel;
	int pwmFrequency;

	char leftEncoderGpioPin;
	char rightEncoderGpioPin;

	/**
	 * Максимально допустимое значение для PWM
	 */
	int maxPower;

	/**
	 * "первое" значение, отличное от нуля, для PWM
	 */
	int minPower;

	/**
	 * максимально допустимая скорость (в RPM)
	 */
	int maxRotateSpeed;

	/**
	 * Кол-во импульсов энкодера на один оборот вала двигателя
	 */
	int encoderCountsPerRotate;

	/**
	 * Предельные значения токов, температур и напряжений, при которых нужно сообщать об ошибке
	 */
	int maxContinuousDriverCurrentMa;
	int maxPeakDriverCurrentMa;
	int maxDriverTemperature;
	int maxEngineTemperature;
	int maxMBCurrentMa;
	int minEngineVoltageMv;
	int minTotalVoltageMv;

	int telemetryIntervalMs;
	int SMAPeakValues;
	int SMAAvgValues;

	bool enginesEnabled;

	char mainCurrentAnalogChannel;
	char mainVoltageAnalogChannel;
	char mbTemprAnalogChannel;

	char leftEngineCurrentAnalogChannel;
	char leftEngineVoltageAnalogChannel;
	char leftDriverTemprAnalogChannel;
	char leftEngineTemprAnalogChannel;

	char rightEngineCurrentAnalogChannel;
	char rightEngineVoltageAnalogChannel;
	char rightDriverTemprAnalogChannel;
	char rightEngineTemprAnalogChannel;

	/**
	 * параметры сбора статистики
	 */

	int enginesLogBuffer;

};

}


#endif /* GLOBALSETTINGS_H_ */
