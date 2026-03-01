#pragma once

#include "IrpEvent.h"

class VehicleCost : public IrpEvent {
  public:
    const int day;
    const int vehicleId;
    const double cost;

    VehicleCost() = delete;

    constexpr VehicleCost(int dayIn, int vehicleIdIn, double costIn)
        : day(dayIn),
          vehicleId(vehicleIdIn),
          cost(costIn) {}

    json toJsonValue() override
    {
        return json{
            {"@class", "iso.example.irpsystem.irpdomain.ImmutableVehicleCost"},
            {"day", day},
            {"vehicleId", vehicleId},
            {"cost", cost},
        };
    }
};
