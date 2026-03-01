#include "VehicleStreamingSimulator.h"

#include <utility>

using namespace adevs;

VehicleStreamingSimulator::VehicleStreamingSimulator(DevsSfCoordinator *coordinator,
    long timeType, std::string modelName, Vehicle* _vehicle):
    DevsSfSimulator(coordinator, timeType, std::move(modelName), _vehicle) {
  vehicle = static_cast<Vehicle*>(model);
}
