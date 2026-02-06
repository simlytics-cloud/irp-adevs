#include "IrpEvent.h"

#include <stdexcept>
#include <string>

#include "Coordinate.h"
#include "Delivery.h"
#include "InventoryCost.h"

IrpEvent* IrpEvent::fromJson(const json& j)
{
    const auto classIt = j.find("@class");
    if (classIt != j.end() && classIt->is_string()) {
        const std::string className = classIt->get<std::string>();
        if (className == "iso.example.irpsystem.irpdomain.ImmutableDelivery") {
            const auto& coordJson = j.at("retailerLocation");
            return new Delivery(
                j.at("retailerId").get<int>(),
                Coordinate(coordJson.at("x").get<double>(), coordJson.at("y").get<double>()),
                j.at("productAmount").get<double>());
        }
        if (className == "iso.example.irpsystem.irpdomain.ImmutableInventoryCost") {
            return new InventoryCost(
                j.at("day").get<int>(),
                j.at("retailerId").get<int>(),
                j.at("cost").get<double>());
        }
    }

    if (j.contains("retailerLocation")) {
        const auto& coordJson = j.at("retailerLocation");
        return new Delivery(
            j.at("retailerId").get<int>(),
            Coordinate(coordJson.at("x").get<double>(), coordJson.at("y").get<double>()),
            j.at("productAmount").get<double>());
    }
    if (j.contains("day") && j.contains("cost")) {
        return new InventoryCost(
            j.at("day").get<int>(),
            j.at("retailerId").get<int>(),
            j.at("cost").get<double>());
    }

    throw std::runtime_error("IrpEvent::fromJson: unsupported json payload");
}
