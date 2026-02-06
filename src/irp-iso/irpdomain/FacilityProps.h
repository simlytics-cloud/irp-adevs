#pragma once

#include "Coordinate.h"

class FacilityProps : public JsonSerializable {
  public:
    const Coordinate coordinate;
    const double startingInventory;
    const double inventoryCost;

    FacilityProps() = delete;

    constexpr FacilityProps(const Coordinate& coordinateIn,
                            double startingInventoryIn,
                            double inventoryCostIn)
        : coordinate(coordinateIn),
          startingInventory(startingInventoryIn),
          inventoryCost(inventoryCostIn) {}

    json toJsonValue() override
    {
        return json{
            {"@class", "iso.example.irpsystem.irpdomain.ImmutableFacilityProps"},
            {"coordinate", json{
                {"@class", "iso.example.irpsystem.irpdomain.ImmutableCoordinate"},
                {"x", coordinate.x},
                {"y", coordinate.y},
            }},
            {"startingInventory", startingInventory},
            {"inventoryCost", inventoryCost},
        };
    }
};
