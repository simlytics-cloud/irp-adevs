#include "adevs.h"

#include "../irpmodel/Retailer.h"
#include "FacilityProps.h"
#include "Coordinate.h"


// Use your generator class name/header here.
// Your project view shows ReceiverTestGenerator.{h,cc}; adjust if needed.
#include "ReceiverTestGenerator.h"

#include "RetailerTestAcceptor.h"

#include <iostream>

#include "../irpmodel/IrpData.h"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "data"
#endif

int main()
{
    std::string dataPath = std::string(TEST_DATA_DIR) + "/S_abs1n5_2_L3.json";
    const IrpData irp_data = IrpData::fromJsonFile(dataPath);
    std::cout << "Loaded IrpData: numNodes=" << irp_data.numNodes
              << ", numTimePeriods=" << irp_data.numTimePeriods << "\n";

    // --- Build components from IrpData ---
    const int retailerIdx = 1; // "retailer1"
    const auto& rd = irp_data.retailers[retailerIdx];

    const Coordinate retailerCoord(rd.x, rd.y);
    const FacilityProps props(retailerCoord, rd.startingInventory, rd.inventoryCost);

    const int retailerId = rd.id;

    // Retailer
    Retailer *retailer = new Retailer(
        props,
        retailerId,
        /*minInventory*/
        rd.minInventory,
        /*maxInventory*/
        rd.maxInventory,
        /*dailyConsumption*/
        rd.dailyConsumption
    );

    // Generator: two deliveries at noon day 1 and noon day 2
    // (Constructor signature depends on your ReceiverTestGenerator implementation.)
    ReceiverTestGenerator *gen = new ReceiverTestGenerator(
        retailerId,
        /*amountDay1*/
        35.0,
        /*amountDay2*/
        17.5
    );

    // Acceptor: expects 2 InventoryCost events with known costs
    // These expected costs must match your Retailer::output_func calculation.
    // Day 1: (startingInventory - dailyConsumption + amountDay1) * inventoryCost
    // (70.0 - 35.0 + 35.0) * 0.03 = 70.0 * 0.03 = 2.1
    // Day 2: (70.0 - 35.0 + 17.5) * 0.03 = 52.5 * 0.03 = 1.575
    RetailerTestAcceptor *acceptor = new RetailerTestAcceptor(
        retailerId,
        /*expectedDay1Cost*/
        (rd.startingInventory - rd.dailyConsumption + 35.0) * rd.inventoryCost,
        /*expectedDay2Cost*/
        (rd.startingInventory - rd.dailyConsumption + 17.5) * rd.inventoryCost
    );

    // --- Build digraph ---
    auto* model = new adevs::Digraph<IrpEvent*, std::string ,long>();
    model->add(gen);
    model->add(retailer);
    model->add(acceptor);

    // --- Couplings ---
    // Generator delivers to Retailer
    model->couple(gen, ReceiverTestGenerator::toReceiveDelivery, retailer, Retailer::receiveDelivery);

    // Retailer sends daily inventory cost to Acceptor
    model->couple(retailer, Retailer::dailyInventoryCost, acceptor, Retailer::dailyInventoryCost);

    // --- Run simulation ---
    adevs::Simulator<adevs::PortValue<IrpEvent*, std::string>, long> sim(model);

    // Run for 2 days (48 hours)
    while (sim.nextEventTime() < 2 * 60 * 24)
    {
        sim.execNextEvent();
    }

    std::cout << "Simulation completed.\n";

    // --- Cleanup ---

    return 0;
}
