/*
 * tankPerepherialController.cpp
 *
 *  Created on: 25.06.2017
 *      Author: jane
 */

#include "TankPerepherialController.h"
#if defined ( ESP8266 )
  #include <pgmspace.h>
#else
  #include <avr/pgmspace.h>
#endif
#include <Arduino.h>

namespace tankController{

TankPerepherialController* controller;

void controllerSetup(){
	controller = new TankPerepherialController();
}

void controllerLoop(){
	controller->loop();
}

}
