//
// Created by dsporykhin on 30.08.17.
//


#include "spi/SPISlave.h"
#include "SpiSlaveListener.h"
#include "DeviceInfo.h"
#include "EnginePowerController.h"
#include "Logger.h"

namespace tankController {
    SpiSlaveListener::SpiSlaveListener(DeviceInfo *, EnginePowerController *) {
        this->deviceInfo = deviceInfo;
        this->enginePowerController = enginePowerController;
    }

    void SpiSlaveListener::init() {
        SPISlave.onData(onData);
    }

    void SpiSlaveListener::onData(uint8_t *data, size_t len) {
    }

}