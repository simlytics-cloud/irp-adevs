#pragma once

#include "devs/DevsSfSimulator.h"
#include "devs/msg/time/SimTime.h"
#include "Vehicle.h"

using namespace adevs;
using namespace devs_message;

class VehicleStreamingSimulator: public DevsSfSimulator<IrpEvent*, std::string, long> {
public:
    explicit VehicleStreamingSimulator(DevsSfCoordinator *coordinator,
        long timeType,
        std::string modelName,
        Vehicle* vehicle);

protected:
    Vehicle* vehicle = nullptr;
};
