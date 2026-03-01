#pragma once

#include "devs/DevsSfSimulator.h"
#include "devs/msg/time/SimTime.h"
#include "devs/msg/RequestOutput.h"
#include "devs/msg/OutputReport.h"
#include "devs/msg/SimulationInit.h"
#include "Retailer.h"

using namespace adevs;
using namespace devs_message;

class RetailerStreamingSimulator: public DevsSfSimulator<IrpEvent*, std::string, long> {
public:
    explicit RetailerStreamingSimulator(DevsSfCoordinator *coordinator,
        long timeType,
        std::string modelName,
        Retailer* retailer);

protected:
    Retailer* retailer = nullptr;
};
