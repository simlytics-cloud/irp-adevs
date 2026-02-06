#pragma once

#include "IrpEvent.h"

class InventoryCost : public IrpEvent {
  public:
    const int day;
    const int retailerId;
    const double cost;

    InventoryCost() = delete;

    constexpr InventoryCost(int dayIn, int retailerIdIn, double costIn)
        : day(dayIn),
          retailerId(retailerIdIn),
          cost(costIn) {}

    json toJsonValue() override
    {
        return json{
            {"@class", "iso.example.irpsystem.irpdomain.ImmutableInventoryCost"},
            {"day", day},
            {"retailerId", retailerId},
            {"cost", cost},
        };
    }
};
