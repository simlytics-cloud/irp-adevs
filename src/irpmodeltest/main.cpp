#include "adevs.h"

#include "../irpmodel/Retailer.h"
#include "FacilityProps.h"
#include "Coordinate.h"


// Use your generator class name/header here.
// Your project view shows ReceiverTestGenerator.{h,cc}; adjust if needed.
#include "ReceiverTestGenerator.h"

#include "RetailerTestAcceptor.h"

#include <iostream>

/**
 * @brief The main function initializes and runs an inventory simulation
 * for a retailer over a two-day period.
 *
 * The function sets up various components required for the simulation,
 * including the retailer, generator for deliveries, and acceptor for
 * verifying results. It integrates these components into a simulation
 * model, defines couplings among them, and executes a discrete event
 * simulation for a specific duration. The simulation models the behavior
 * of a retailer handling inventory and delivery events, verifying outputs
 * against expected results.
 *
 * @return Returns 0 upon successful execution of the program.
 */
int main()
{

    // --- Build components ---
    const Coordinate retailerCoord(172.0, 334.0);
    const FacilityProps props(retailerCoord, /*startingInventory*/ 70.0, /*inventoryCost*/ 0.03);

    const int retailerId = 2;

    // Retailer
    Retailer *retailer = new Retailer(
        props,
        /*currentInventory*/
        retailerId,
        /*minInventory*/
        0.0,
        /*maxInventory*/
        105.0,
        /*dailyConsumption*/
        35.0
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
    // Here, inventoryCost=1.0 and consumption=0.0, so cost == currentInventory at 4pm.
    RetailerTestAcceptor *acceptor = new RetailerTestAcceptor(
        retailerId,
        /*expectedDay1Cost*/
        2.1,
        /*expectedDay2Cost*/
        1.575
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
