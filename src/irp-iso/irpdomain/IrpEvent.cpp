#include "IrpEvent.h"

#include <stdexcept>
#include <string>

#include "Coordinate.h"
#include "Delivery.h"
#include "DeliveryRoute.h"
#include "DeliverySchedule.h"
#include "InventoryCost.h"
#include "VehicleCost.h"

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
        if (className == "iso.example.irpsystem.irpdomain.ImmutableVehicleCost") {
            return new VehicleCost(
                j.at("day").get<int>(),
                j.at("vehicleId").get<int>(),
                j.at("cost").get<double>());
        }
        if (className == "iso.example.irpsystem.irpdomain.ImmutableDeliveryRoute") {
            std::vector<Delivery> deliveries;
            for (const auto& dj : j.at("deliveries")) {
                const auto& coordJson = dj.at("retailerLocation");
                deliveries.emplace_back(
                    dj.at("retailerId").get<int>(),
                    Coordinate(coordJson.at("x").get<double>(), coordJson.at("y").get<double>()),
                    dj.at("productAmount").get<double>());
            }
            return new DeliveryRoute(j.at("vehicleId").get<int>(), deliveries);
        }
        if (className == "iso.example.irpsystem.irpdomain.ImmutableDeliverySchedule") {
            std::map<int, std::map<int, DeliveryRoute>> schedule;
            for (auto const& [dayStr, vehicleMapJson] : j.at("deliveriesByDayByVehicle").items()) {
                int day = std::stoi(dayStr);
                std::map<int, DeliveryRoute> vehicleMap;
                for (auto const& [vehicleIdStr, routeJson] : vehicleMapJson.items()) {
                    int vehicleId = std::stoi(vehicleIdStr);
                    std::vector<Delivery> deliveries;
                    for (const auto& dj : routeJson.at("deliveries")) {
                        const auto& coordJson = dj.at("retailerLocation");
                        deliveries.emplace_back(
                            dj.at("retailerId").get<int>(),
                            Coordinate(coordJson.at("x").get<double>(), coordJson.at("y").get<double>()),
                            dj.at("productAmount").get<double>());
                    }
                    vehicleMap.emplace(std::piecewise_construct,
                                       std::forward_as_tuple(vehicleId),
                                       std::forward_as_tuple(vehicleId, deliveries));
                }
                schedule.emplace(day, vehicleMap);
            }
            return new DeliverySchedule(schedule);
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
        if (j.contains("retailerId")) {
            return new InventoryCost(
                j.at("day").get<int>(),
                j.at("retailerId").get<int>(),
                j.at("cost").get<double>());
        }
        if (j.contains("vehicleId")) {
            return new VehicleCost(
                j.at("day").get<int>(),
                j.at("vehicleId").get<int>(),
                j.at("cost").get<double>());
        }
    }
    if (j.contains("vehicleId") && j.contains("deliveries")) {
        std::vector<Delivery> deliveries;
        for (const auto& dj : j.at("deliveries")) {
            const auto& coordJson = dj.at("retailerLocation");
            deliveries.emplace_back(
                dj.at("retailerId").get<int>(),
                Coordinate(coordJson.at("x").get<double>(), coordJson.at("y").get<double>()),
                dj.at("productAmount").get<double>());
        }
        return new DeliveryRoute(j.at("vehicleId").get<int>(), deliveries);
    }
    if (j.contains("deliveriesByDayByVehicle")) {
        std::map<int, std::map<int, DeliveryRoute>> schedule;
        for (auto const& [dayStr, vehicleMapJson] : j.at("deliveriesByDayByVehicle").items()) {
            int day = std::stoi(dayStr);
            std::map<int, DeliveryRoute> vehicleMap;
            for (auto const& [vehicleIdStr, routeJson] : vehicleMapJson.items()) {
                int vehicleId = std::stoi(vehicleIdStr);
                std::vector<Delivery> deliveries;
                for (const auto& dj : routeJson.at("deliveries")) {
                    const auto& coordJson = dj.at("retailerLocation");
                    deliveries.emplace_back(
                        dj.at("retailerId").get<int>(),
                        Coordinate(coordJson.at("x").get<double>(), coordJson.at("y").get<double>()),
                        dj.at("productAmount").get<double>());
                }
                vehicleMap.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(vehicleId),
                                   std::forward_as_tuple(vehicleId, deliveries));
            }
            schedule.emplace(day, vehicleMap);
        }
        return new DeliverySchedule(schedule);
    }

    throw std::runtime_error("IrpEvent::fromJson: unsupported json payload");
}
