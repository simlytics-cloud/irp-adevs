#include "ManufacturerTestAcceptor.h"
#include "InventoryCost.h"
#include "DeliveryRoute.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

ManufacturerTestAcceptor::ManufacturerTestAcceptor(const std::vector<double>& expectedCostsIn, int expectedRouteCountIn)
    : expectedCosts(expectedCostsIn),
      expectedRouteCount(expectedRouteCountIn)
{
}

void ManufacturerTestAcceptor::delta_int() {}

bool ManufacturerTestAcceptor::nearlyEqual(double a, double b, double relEps, double absEps)
{
    const double diff = std::fabs(a - b);
    if (diff <= absEps) return true;
    return diff <= relEps * std::max(std::fabs(a), std::fabs(b));
}

void ManufacturerTestAcceptor::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    (void)e;
    for (auto const& pv : xb) {
        if (auto* ic = dynamic_cast<const InventoryCost*>(pv.value)) {
            if (costIndex >= (int)expectedCosts.size()) {
                throw std::runtime_error("Received more InventoryCost events than expected");
            }
            if (!nearlyEqual(ic->cost, expectedCosts[costIndex])) {
                std::cerr << "Cost mismatch at index " << costIndex << ": expected " << expectedCosts[costIndex] << ", got " << ic->cost << std::endl;
                throw std::runtime_error("InventoryCost mismatch");
            }
            costIndex++;
            if (costIndex == (int)expectedCosts.size() && routeCount == expectedRouteCount) {
                std::cout << "Manufacturer Test passed!" << std::endl;
            }
        } else if (dynamic_cast<const DeliveryRoute*>(pv.value)) {
            routeCount++;
            if (costIndex == (int)expectedCosts.size() && routeCount == expectedRouteCount) {
                std::cout << "Manufacturer Test passed!" << std::endl;
            }
        }
    }
}

void ManufacturerTestAcceptor::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    delta_int();
    delta_ext(0, xb);
}

void ManufacturerTestAcceptor::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb)
{
    (void)yb;
}

long ManufacturerTestAcceptor::ta()
{
    return adevs_inf<long>();
}

void ManufacturerTestAcceptor::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
    g.clear();
}
