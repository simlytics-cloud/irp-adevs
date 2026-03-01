#include "adevs.h"

#include "Vehicle.h"
#include "Coordinate.h"
#include "Delivery.h"
#include "DeliveryRoute.h"

#include "VehicleTestGenerator.h"
#include "VehicleTestAcceptor.h"

#include <iostream>
#include <vector>

#include "IrpData.h"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "data"
#endif

int main()
{
    // Load IrpData
    std::string dataPath = std::string(TEST_DATA_DIR) + "/S_abs1n5_2_L3.json";
    const IrpData irp_data = IrpData::fromJsonFile(dataPath);
    std::cout << "Loaded IrpData: numNodes=" << irp_data.numNodes
              << ", numTimePeriods=" << irp_data.numTimePeriods << "\n";

    // Set up a simple route with two deliveries along x-axis
    // Manufacturer (base) from IrpData; speed 60 km/h => 1 km/min; distances are in km assuming coordinates as km
    // Coordinates: base=(base.x, base.y), D1=(base.x+30, base.y), D2=(base.x+60, base.y)
    // Distances: base->D1 = 30, D1->D2 = 30, D2->base = 60; total = 120 km

    const Coordinate base(irp_data.manufacturer.x, irp_data.manufacturer.y);

    std::vector<Delivery> deliveries;
    deliveries.emplace_back(10, Coordinate(base.x + 30.0, base.y), 10.0); // retailerId=10
    deliveries.emplace_back(11, Coordinate(base.x + 60.0, base.y), 5.0);  // retailerId=11

    DeliveryRoute route(1, deliveries);

    // Vehicle parameters
    const int vehicleId = 1;
    const double capacity = irp_data.vehicleCapacity;
    const double startingDailyKm = 0.0;
    const double costPerKm = irp_data.vehicleCostPerKm;
    const double speedKmHr = irp_data.vehicleSpeedKmHr;

    // Construct vehicle with EMPTY route initially so it stays at base until t=360
    DeliveryRoute emptyRoute(1, {});
    Vehicle* vehicle = new Vehicle(vehicleId, capacity, startingDailyKm, emptyRoute, base, costPerKm, speedKmHr);
    vehicle->initialize(0);

    // Generator sends the route at t=360 (06:00)
    VehicleTestGenerator* gen = new VehicleTestGenerator(route, 360);

    // Acceptor expects the two drop deliveries, and one VehicleCost at day 1 with cost = (30+30+60) * costPerKm = 120 * costPerKm
    const int expectedCostDay = 1;
    const double expectedCost = 120.0 * costPerKm;
    VehicleTestAcceptor* acceptor = new VehicleTestAcceptor(deliveries, expectedCostDay, expectedCost);

    // Build digraph
    auto* model = new adevs::Digraph<IrpEvent*, std::string, long>();
    model->add(gen);
    model->add(vehicle);
    model->add(acceptor);

    // Couplings
    model->couple(gen, VehicleTestGenerator::toAcceptDeliveryRoute, vehicle, Vehicle::acceptDeliveryRoute);
    model->couple(vehicle, Vehicle::dropDelivery, acceptor, Vehicle::dropDelivery);
    model->couple(vehicle, Vehicle::dailyDeliveryCost, acceptor, Vehicle::dailyDeliveryCost);

    // Run simulation long enough to cover both deliveries and return
    adevs::Simulator<adevs::PortValue<IrpEvent*, std::string>, long> sim(model);
    while (sim.nextEventTime() < 24 * 60) {
        sim.execNextEvent();
    }

    std::cout << "Vehicle simulation test completed." << std::endl;

    return 0;
}
