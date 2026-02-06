
#pragma once

#include "IrpEvent.h"
#include "Coordinate.h"

class Delivery : public IrpEvent {
  public:
    const int retailerId;
    const Coordinate retailerLocation;
    const double productAmount;

    Delivery() = delete;

    constexpr Delivery(int retailerIdIn, const Coordinate& retailerLocationIn, double productAmountIn)
        : retailerId(retailerIdIn),
          retailerLocation(retailerLocationIn),
          productAmount(productAmountIn) {}

    json toJsonValue() override
    {
        return json{
            {"@class", "iso.example.irpsystem.irpdomain.ImmutableDelivery"},
            {"retailerId", retailerId},
            {"retailerLocation", json{
                {"@class", "iso.example.irpsystem.irpdomain.ImmutableCoordinate"},
                {"x", retailerLocation.x},
                {"y", retailerLocation.y},
            }},
            {"productAmount", productAmount},
        };
    }
};
