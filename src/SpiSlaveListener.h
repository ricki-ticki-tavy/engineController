//
// Created by dsporykhin on 28.08.17.
//

#ifndef ENGINECONTROLLER_SPISLAVELISTENER_H
#define ENGINECONTROLLER_SPISLAVELISTENER_H

#include "EnginePowerController.h"

namespace tankController {

    class SpiSlaveListener {
    private:
        DeviceInfo *deviceInfo;
        EnginePowerController *enginePowerController;

        static void onData(uint8_t * data, size_t len);
    public:

        SpiSlaveListener(DeviceInfo*, EnginePowerController*);
        void init();

    };
}

#endif //ENGINECONTROLLER_SPISLAVELISTENER_H
