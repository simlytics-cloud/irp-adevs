#pragma once

#include "devs/JsonSerializable.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Coordinate : public JsonSerializable {
  public:
    const double x;
    const double y;

    Coordinate() = delete;

    constexpr Coordinate(double xIn, double yIn)
        : x(xIn), y(yIn) {}

    json toJsonValue() override
    {
        return json{
            {"@class", "iso.example.irpsystem.irpdomain.ImmutableCoordinate"},
            {"x", x},
            {"y", y},
        };
    }
};
