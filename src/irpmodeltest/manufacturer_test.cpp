#include "adevs.h"
#include "../irpmodel/Manufacturer.h"
#include "FacilityProps.h"
#include "Coordinate.h"
#include "ManufacturerTestGenerator.h"
#include "ManufacturerTestAcceptor.h"
#include "DeliveryRoute.h"
#include "DeliverySchedule.h"
#include <iostream>
#include <vector>
#include <map>

#include "IrpData.h"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "data"
#endif

int main() {
    // 0. Load IrpData
    std::string dataPath = std::string(TEST_DATA_DIR) + "/S_abs1n5_2_L3.json";
    const IrpData irp_data = IrpData::fromJsonFile(dataPath);
    std::cout << "Loaded IrpData: numNodes=" << irp_data.numNodes
              << ", numTimePeriods=" << irp_data.numTimePeriods << "\n";

    // 1. Setup Manufacturer properties from IrpData
    const Coordinate coord(irp_data.manufacturer.x, irp_data.manufacturer.y);
    const double startingInventory = irp_data.manufacturer.startingInventory;
    const double inventoryCostRate = irp_data.manufacturer.inventoryCost;
    const FacilityProps props(coord, startingInventory, inventoryCostRate);
    const double dailyProduction = irp_data.manufacturer.dailyProduction;

    Manufacturer* manufacturer = new Manufacturer(props, startingInventory, dailyProduction);
    manufacturer->initialize(0);

    // 2. Setup Delivery Schedule
    // Day 1, Vehicle 1, one delivery of 30.0 units
    std::vector<Delivery> day1Deliveries;
    day1Deliveries.push_back(Delivery(1, Coordinate(10.0, 10.0), 30.0));
    DeliveryRoute route1(1, day1Deliveries);

    std::map<int, DeliveryRoute> vehicleMap;
    vehicleMap.insert({1, route1});
    std::map<int, std::map<int, DeliveryRoute>> scheduleMap;
    scheduleMap.insert({1, vehicleMap});
    DeliverySchedule schedule(scheduleMap);

    // 3. Setup Generator
    // Send schedule at t=0, and a Delivery of 20.0 units at t=12:00 (720 min)
    const double extraDeliveryAmount = 20.0;
    const long extraDeliveryTime = 12 * 60;
    ManufacturerTestGenerator* gen = new ManufacturerTestGenerator(schedule, extraDeliveryAmount, extraDeliveryTime);

    // 4. Calculate Expected Costs
    // Day 1:
    // t=0: inventory = startingInventory
    // t=6*60 (06:00): LOAD event. Load amount = 30.0. inventory = startingInventory - 30 = 510.0 - 30.0 = 480.0
    // t=12*60 (12:00): EXT Delivery event (extra). inventory = 480.0 + 20.0 = 500.0
    // t=23*60+59 (23:59): REPORT event. 
    //   Cost = (currentInventory + dailyProduction) * rate = (500.0 + 193.0) * 0.03 = 20.79.
    //   Then production is added: inventory = 500.0 + 193.0 = 693.0.
    // Day 2:
    // t=24*60 + 6*60: LOAD event. No deliveries scheduled for Day 2. inventory = 693.0.
    // t=24*60 + 23*60+59: REPORT event.
    //   Cost = (693.0 + 193.0) * 0.03 = 886.0 * 0.03 = 26.58.
    //   Then production is added: inventory = 693.0 + 193.0 = 886.0.

    std::vector<double> expectedCosts = {20.79, 26.58};
    int expectedRouteCount = 2; // Day 1 has a route, and Day 2 LOAD event also triggers an output (empty route if no schedule)

    // 5. Setup Acceptor
    ManufacturerTestAcceptor* acceptor = new ManufacturerTestAcceptor(expectedCosts, expectedRouteCount);

    // 6. Build Model
    auto* model = new adevs::Digraph<IrpEvent*, std::string, long>();
    model->add(gen);
    model->add(manufacturer);
    model->add(acceptor);

    // Couplings
    model->couple(gen, ManufacturerTestGenerator::toAcceptDeliverySchedule, manufacturer, Manufacturer::acceptDeliverySchedule);
    model->couple(gen, ManufacturerTestGenerator::toAcceptDelivery, manufacturer, Manufacturer::acceptDelivery);
    model->couple(manufacturer, Manufacturer::dailyInventoryCost, acceptor, Manufacturer::dailyInventoryCost);
    model->couple(manufacturer, Manufacturer::postDeliveryRoute, acceptor, Manufacturer::postDeliveryRoute);

    // 7. Run Simulation
    adevs::Simulator<adevs::PortValue<IrpEvent*, std::string>, long> sim(model);
    while (sim.nextEventTime() < 2 * 24 * 60) {
        sim.execNextEvent();
    }

    std::cout << "Simulation completed." << std::endl;

    return 0;
}
