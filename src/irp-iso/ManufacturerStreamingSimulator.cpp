#include "ManufacturerStreamingSimulator.h"

#include <utility>

using namespace adevs;

ManufacturerStreamingSimulator::ManufacturerStreamingSimulator(DevsSfCoordinator *coordinator,
    long timeType, std::string modelName, Manufacturer* _manufacturer):
    DevsSfSimulator(coordinator, timeType, std::move(modelName), _manufacturer) {
  manufacturer = static_cast<Manufacturer*>(model);
}
