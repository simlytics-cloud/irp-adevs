
#include <iostream>
#include <map>
#include <cassert>
#include "../irpdomain/IrpEvent.h"
#include "../irpdomain/Delivery.h"
#include "../irpdomain/DeliveryRoute.h"
#include "../irpdomain/DeliverySchedule.h"

int main() {
    // Create some deliveries
    Delivery d1(1, Coordinate(10.0, 20.0), 100.0);
    Delivery d2(2, Coordinate(30.0, 40.0), 200.0);
    
    // Create a DeliveryRoute
    std::vector<Delivery> deliveries;
    deliveries.push_back(d1);
    deliveries.push_back(d2);
    DeliveryRoute route1(101, deliveries);
    
    // Create another route
    std::vector<Delivery> deliveries2;
    deliveries2.push_back(d2);
    DeliveryRoute route2(102, deliveries2);
    
    // Create a DeliverySchedule
    std::map<int, std::map<int, DeliveryRoute>> data;
    data.emplace(std::piecewise_construct,
                 std::forward_as_tuple(1),
                 std::forward_as_tuple());
    data.at(1).emplace(std::piecewise_construct,
                       std::forward_as_tuple(101),
                       std::forward_as_tuple(101, deliveries));
    data.at(1).emplace(std::piecewise_construct,
                       std::forward_as_tuple(102),
                       std::forward_as_tuple(102, deliveries2));
    
    data.emplace(std::piecewise_construct,
                 std::forward_as_tuple(2),
                 std::forward_as_tuple());
    data.at(2).emplace(std::piecewise_construct,
                       std::forward_as_tuple(101),
                       std::forward_as_tuple(101, deliveries2));
    
    DeliverySchedule schedule(data);
    
    // Test toJson
    json j = schedule.toJson();
    std::cout << "Serialized JSON: " << j.dump(4) << std::endl;
    
    assert(j["@class"] == "iso.example.irpsystem.irpdomain.ImmutableDeliverySchedule");
    assert(j["deliveriesByDayByVehicle"].is_object());
    assert(j["deliveriesByDayByVehicle"].contains("1"));
    assert(j["deliveriesByDayByVehicle"]["1"].contains("101"));
    assert(j["deliveriesByDayByVehicle"]["1"]["101"]["vehicleId"] == 101);
    
    // Test fromJson
    IrpEvent* event = IrpEvent::fromJson(j);
    DeliverySchedule* schedule2 = dynamic_cast<DeliverySchedule*>(event);
    
    assert(schedule2 != nullptr);
    assert(schedule2->deliveriesByDayByVehicle.size() == 2);
    assert(schedule2->deliveriesByDayByVehicle.at(1).at(101).vehicleId == 101);
    assert(schedule2->deliveriesByDayByVehicle.at(1).at(101).deliveries.size() == 2);
    assert(schedule2->deliveriesByDayByVehicle.at(2).at(101).vehicleId == 101);
    
    // Test fromJson without @class (using inferred fields)
    json j2 = j;
    j2.erase("@class");
    IrpEvent* event2 = IrpEvent::fromJson(j2);
    DeliverySchedule* schedule3 = dynamic_cast<DeliverySchedule*>(event2);
    assert(schedule3 != nullptr);
    assert(schedule3->deliveriesByDayByVehicle.size() == 2);
    
    delete event;
    delete event2;
    
    std::cout << "DeliverySchedule test passed!" << std::endl;
    return 0;
}
