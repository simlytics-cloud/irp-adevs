#pragma once

#include "devs/DevsSfSimulator.h"
#include "devs/msg/time/SimTime.h"
#include "devs/msg/RequestOutput.h"
#include "devs/msg/OutputReport.h"
#include "devs/msg/SimulationInit.h"
#include "Manufacturer.h"

using namespace adevs;
using namespace devs_message;

class ManufacturerStreamingSimulator: public DevsSfSimulator<IrpEvent*, std::string, long> {
public:
    explicit ManufacturerStreamingSimulator(DevsSfCoordinator *coordinator,
        long timeType,
        std::string modelName,
        Manufacturer* manufacturer);

protected:
    Manufacturer* manufacturer = nullptr;
};
