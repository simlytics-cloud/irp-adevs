
#include <iostream>
#include <vector>
#include <cassert>
#include "../irpdomain/IrpEvent.h"
#include "../irpdomain/Delivery.h"
#include "../irpdomain/DeliveryRoute.h"

int main() {
    // Create a DeliveryRoute
    std::vector<Delivery> deliveries;
    deliveries.emplace_back(1, Coordinate(10.0, 20.0), 100.0);
    deliveries.emplace_back(2, Coordinate(30.0, 40.0), 200.0);
    
    DeliveryRoute route(42, deliveries);
    
    // Test toJson
    json j = route.toJson();
    std::cout << "Serialized JSON: " << j.dump(4) << std::endl;
    
    assert(j["@class"] == "iso.example.irpsystem.irpdomain.ImmutableDeliveryRoute");
    assert(j["vehicleId"] == 42);
    assert(j["deliveries"].is_array());
    assert(j["deliveries"].size() == 2);
    assert(j["deliveries"][0]["retailerId"] == 1);
    assert(j["deliveries"][1]["retailerId"] == 2);
    
    // Test fromJson
    IrpEvent* event = IrpEvent::fromJson(j);
    DeliveryRoute* route2 = dynamic_cast<DeliveryRoute*>(event);
    
    assert(route2 != nullptr);
    assert(route2->vehicleId == 42);
    assert(route2->deliveries.size() == 2);
    assert(route2->deliveries[0].retailerId == 1);
    assert(route2->deliveries[1].retailerId == 2);
    assert(route2->deliveries[0].retailerLocation.x == 10.0);
    assert(route2->deliveries[1].productAmount == 200.0);
    
    // Test fromJson without @class (using inferred fields)
    json j2 = j;
    j2.erase("@class");
    IrpEvent* event2 = IrpEvent::fromJson(j2);
    DeliveryRoute* route3 = dynamic_cast<DeliveryRoute*>(event2);
    assert(route3 != nullptr);
    assert(route3->vehicleId == 42);
    
    delete event;
    delete event2;
    
    std::cout << "DeliveryRoute test passed!" << std::endl;
    return 0;
}
