#pragma once

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

struct ManufacturerData {
    int id;
    double x;
    double y;
    double startingInventory;
    double dailyProduction;
    double inventoryCost;
};

struct RetailerData {
    int id;
    double x;
    double y;
    double startingInventory;
    double maxInventory;
    double minInventory;
    double dailyConsumption;
    double inventoryCost;
};

struct IrpData {
    int numNodes;
    int numTimePeriods;
    double vehicleCapacity;
    double vehicleCostPerKm;
    double vehicleSpeedKmHr;
    int numVehicles;
    ManufacturerData manufacturer;
    std::vector<RetailerData> retailers;

    static IrpData fromJsonFile(const std::string& path);
};

inline void from_json(const nlohmann::json& j, ManufacturerData& m)
{
    j.at("id").get_to(m.id);
    j.at("x").get_to(m.x);
    j.at("y").get_to(m.y);
    j.at("startingInventory").get_to(m.startingInventory);
    j.at("dailyProduction").get_to(m.dailyProduction);
    j.at("inventoryCost").get_to(m.inventoryCost);
}

inline void from_json(const nlohmann::json& j, RetailerData& r)
{
    j.at("id").get_to(r.id);
    j.at("x").get_to(r.x);
    j.at("y").get_to(r.y);
    j.at("startingInventory").get_to(r.startingInventory);
    j.at("maxInventory").get_to(r.maxInventory);
    j.at("minInventory").get_to(r.minInventory);
    j.at("dailyConsumption").get_to(r.dailyConsumption);
    j.at("inventoryCost").get_to(r.inventoryCost);
}

inline void from_json(const nlohmann::json& j, IrpData& d)
{
    j.at("numNodes").get_to(d.numNodes);
    j.at("numTimePeriods").get_to(d.numTimePeriods);
    j.at("vehicleCapacity").get_to(d.vehicleCapacity);
    j.at("vehicleCostPerKm").get_to(d.vehicleCostPerKm);
    j.at("vehicleSpeekKmHr").get_to(d.vehicleSpeedKmHr);
    j.at("numVehicles").get_to(d.numVehicles);
    j.at("manufacturer").get_to(d.manufacturer);
    j.at("retailers").get_to(d.retailers);
}

inline IrpData IrpData::fromJsonFile(const std::string& path)
{
    std::ifstream input(path);
    if (!input.is_open())
        throw std::runtime_error("IrpData::fromJsonFile: failed to open " + path);

    nlohmann::json j;
    input >> j;
    return j.get<IrpData>();
}
