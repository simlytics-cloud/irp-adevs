
#pragma once

#include <vector>
#include "IrpEvent.h"
#include "Delivery.h"

class DeliveryRoute : public IrpEvent {
  public:
    const int vehicleId;
    const std::vector<Delivery> deliveries;

    DeliveryRoute() = delete;

    DeliveryRoute(int vehicleIdIn, const std::vector<Delivery>& deliveriesIn)
        : vehicleId(vehicleIdIn),
          deliveries(deliveriesIn) {}

    json toJsonValue() override
    {
        json deliveriesJson = json::array();
        for (auto d : deliveries) {
            deliveriesJson.push_back(d.toJsonValue());
        }
        return json{
            {"@class", "iso.example.irpsystem.irpdomain.ImmutableDeliveryRoute"},
            {"vehicleId", vehicleId},
            {"deliveries", deliveriesJson},
        };
    }
};
