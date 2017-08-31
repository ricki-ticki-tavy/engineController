/*
 * tankPerepherialController.h
 *
 *  главный класс всего проекта. именно тут сводится управление всеми частями
 *
 *
 *  Created on: 25.06.2017
 *      Author: jane
 */

#ifndef TANKPEREPHERIALCONTROLLER_H_
#define TANKPEREPHERIALCONTROLLER_H_

#include "Logger.h"
#include "SettingsManager.h"
#include "HardwareController.h"
#include "WifiManager.h"
#include "SpiSlaveListener.h"

namespace tankController{

class TankPerepherialController{
public:
	SettingsManager* settingsManager;
	WifiManager* wifiManager;
	HardwareController* hardwareController;
	SpiSlaveListener* spiSlaveListener;

	TankPerepherialController();

	void loop();
};

}


#endif /* TANKPEREPHERIALCONTROLLER_H_ */
