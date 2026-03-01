//
// Created by Robert Kewley on 7/9/23.
//

#include "RetailerStreamingSimulator.h"

#include <utility>

using namespace adevs;

RetailerStreamingSimulator::RetailerStreamingSimulator(DevsSfCoordinator *coordinator,
    long timeType, std::string modelName, Retailer* _retailer):
    DevsSfSimulator(coordinator, timeType, std::move(modelName), _retailer) {
  retailer = static_cast<Retailer*>(model);
}


