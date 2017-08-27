/*
 * DCEngineController.h
 *
 *  Created on: 04.07.2017
 *      Author: jane
 */

#ifndef DCENGINECONTROLLER_H_
#define DCENGINECONTROLLER_H_

#include "SettingsManager.h"
#include "DeviceInfo.h"
#include "EnginePowerController.h"

namespace tankController{

class DCEngineController {
  private:
     long lastSpeedDetectionTime = 0; // когда последний раз считалась тика скорости вращения
     unsigned long lastEngineCorrectionWorkCycle = 0;
     EngineControlData *engines[2];
     unsigned char engineCounter = 0;
     bool started = false;
     uint64_t lastTimeCycle = 0;
     String CANT_ADD_ENGINE_MSG = "Can't add engine.";

     SettingsManager* settingsManager;
     DeviceInfo* deviceInfo;
     EnginePowerController* enginePowerController;

     void detectEncoders(void);
     bool cycleDone; // просто семафор, что выполнен цикл контроля и коррекции

     void calculateEnginesEncoderFrequencies(void);
     double pidCalcUt(EngineControlData *engine);
     void checkAndCorrectEngineSpeedUsingPID(EngineControlData *engine);
  public:
     DCEngineController(SettingsManager* settingsManager, DeviceInfo* deviceInfo, EnginePowerController* enginePowerController);

     /**
      *  Добавить в массив обслуживаемых двигателей еще один двигатель
      **/
     bool addEngine(EngineControlData *engine);

     bool start(void);

     /**
      *  Установить скорость вращения двигателя в RPM
      **/
     bool setEngineSpeed(EngineControlData *engine, unsigned char newDirection, int newSpeed);
     void loop(void);
     bool isCycleDone();
     void enginesCorrectionWorkCycle(void);

};

}

#endif /* DCENGINECONTROLLER_H_ */
