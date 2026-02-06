#pragma once

#include "devs/DevsSfSimulator.h"
#include "devs/msg/time/SimTime.h"
#include "devs/msg/RequestOutput.h"
#include "devs/msg/OutputReport.h"
#include "devs/msg/SimulationInit.h"
#include "Retailer.h"



using namespace adevs;
using namespace devs_message;

class RetailerStreamingSimulator: public DevsSfSimulator<PortValue<IrpEvent *, std::string>, long> {
public:
    explicit RetailerStreamingSimulator(DevsSfCoordinator *coordinator,
        TIME_TYPE timeType,
        std::string modelName,
        Retailer* retailer);

private:
    Simulator<PortValue<IrpEvent *, std::string>, long>* onInitSim(SimulationInit &initSim) override;

protected:
    void onExecuteTransition(ExecuteTransition &executeTransition) override;
    OutputReport* onSendOutput(RequestOutput &sendOutput) override;
    ModelTerminated* onSimulationDone(SimulationTerminate& simulationDone) override;
    Retailer* retailer = nullptr;
};


