
#pragma once

#include <map>
#include <string>
#include "IrpEvent.h"
#include "DeliveryRoute.h"

class DeliverySchedule : public IrpEvent {
  public:
    const std::map<int, std::map<int, DeliveryRoute>> deliveriesByDayByVehicle;

    DeliverySchedule() = delete;

    DeliverySchedule(const std::map<int, std::map<int, DeliveryRoute>>& deliveriesByDayByVehicleIn)
        : deliveriesByDayByVehicle(deliveriesByDayByVehicleIn) {}

    json toJsonValue() override
    {
        json dayMapJson = json::object();
        for (auto const& [day, vehicleMap] : deliveriesByDayByVehicle) {
            json vehicleMapJson = json::object();
            for (auto const& [vehicleId, route] : vehicleMap) {
                // Now that we've ensured toJsonValue is non-const and members are accessible
                // we still have a const route from the map iterator if we use auto const&
                // But wait, the user's previous solution for Delivery.h removed const from toJsonValue.
                // If I want to call it on a const object, I need it to be const.
                // Let's check Delivery.h again.
                DeliveryRoute routeCopy = route; 
                vehicleMapJson[std::to_string(vehicleId)] = routeCopy.toJsonValue();
            }
            dayMapJson[std::to_string(day)] = vehicleMapJson;
        }

        return json{
            {"@class", "iso.example.irpsystem.irpdomain.ImmutableDeliverySchedule"},
            {"deliveriesByDayByVehicle", dayMapJson},
        };
    }
};
