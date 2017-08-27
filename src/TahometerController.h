/*
 * TahometerController.h
 *
 *  Created on: 02.07.2017
 *      Author: jane
 */

#ifndef TAHOMETERCONTROLLER_H_
#define TAHOMETERCONTROLLER_H_

#include "DeviceInfo.h"
#include "SettingsManager.h"

namespace tankController{

class TahometerController{
public:
	TahometerController(DeviceInfo* deviceInfo, SettingsManager* settingsManager_);
private:
	static void leftEncoderCounter();
	static void rightEncoderCounter();
};

}


#endif /* TAHOMETERCONTROLLER_H_ */
