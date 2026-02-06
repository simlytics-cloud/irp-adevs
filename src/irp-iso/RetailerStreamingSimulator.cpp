//
// Created by Robert Kewley on 7/9/23.
//

#include "RetailerStreamingSimulator.h"

#include <utility>

using namespace adevs;

Simulator<PortValue<IrpEvent *, std::string>, long>* RetailerStreamingSimulator::onInitSim(SimulationInit &initSim) {
    LongSimTime* longSimTime = dynamic_cast<LongSimTime *>(initSim.getEventTime());
    long initialTime = longSimTime->getT();
    retailer->currentTime = initialTime;
    return new Simulator<PortValue<IrpEvent *, std::string>, long>(retailer);
}

RetailerStreamingSimulator::RetailerStreamingSimulator(DevsSfCoordinator *coordinator,
    TIME_TYPE timeType, std::string modelName, Retailer* _retailer):
    DevsSfSimulator(coordinator, timeType, std::move(modelName)), retailer(_retailer) {}

void RetailerStreamingSimulator::onExecuteTransition(ExecuteTransition &executeTransition) {
    const std::vector<PortValueMessageBase*>& inputs = executeTransition.getInputs();
    if (inputs.empty()) {
        sim->computeNextState();
    } else {
        // create a bag of events
        Bag<Event<PortValue<IrpEvent *, std::string>, long>> b1;
        for (auto input : inputs) {
            auto* irpPortValueMessage = dynamic_cast<PortValueMessage<IrpEvent*, std::string>*>(input);
            IrpEvent* irpEvent = irpPortValueMessage->getValue();
            PortValue<IrpEvent *, std::string> portValue(irpPortValueMessage->getPortName(), irpEvent);
            // Create an Event with IrpEvent at Retailer receiveDelivery port
            Event<PortValue<IrpEvent *, std::string>, long> event(retailer, portValue);
            b1.insert(event);
        }

        sim->computeNextState(b1, extractTime(executeTransition.getEventTime()));
        b1.clear();
    }
}

OutputReport* RetailerStreamingSimulator::onSendOutput(RequestOutput &sendOutput) {
    adevs::Bag<PortValue<IrpEvent *, std::string>> outputBag = adevs::Bag<PortValue<IrpEvent *, std::string>>();
    retailer->output_func(outputBag);
    std::vector<PortValueMessageBase*> portValueList;
    adevs::Bag<PortValue<IrpEvent *, std::string>>::const_iterator it = outputBag.begin();
    for (; it != outputBag.end(); it++) {
        IrpEvent *irpEvent = (*it).value;
        auto *portValueMessage = new PortValueMessage<IrpEvent*>("dailyInventoryCost",
            "iso.example.irpsystem.irpdomain.ImmutableInventoryCost", irpEvent);
        portValueList.push_back(portValueMessage);
    }
    
    return new OutputReport(
            sendOutput.getSimulationId(),
            generate_uuid(),
            modelName,
            sendOutput.getSenderId(),
            sendOutput.getEventTime(),
            new DoubleSimTime(sim->nextEventTime()),
            portValueList
    );
}

ModelTerminated* RetailerStreamingSimulator::onSimulationDone(SimulationTerminate& simulationDone)
{
    return new ModelTerminated(
            simulationDone.getSimulationId(),
            generate_uuid(),
            modelName,
            simulationDone.getSenderId()
    );
}




