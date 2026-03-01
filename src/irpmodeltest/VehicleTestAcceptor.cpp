#include "VehicleTestAcceptor.h"
#include <stdexcept>
#include <iostream>
#include <cmath>

VehicleTestAcceptor::VehicleTestAcceptor(const std::vector<Delivery>& expectedDeliveriesIn,
                                         int expectedCostDayIn,
                                         double expectedCostIn)
    : expectedDeliveries(expectedDeliveriesIn),
      expectedCostDay(expectedCostDayIn),
      expectedCost(expectedCostIn)
{
}

void VehicleTestAcceptor::delta_int() {}

bool VehicleTestAcceptor::nearlyEqual(double a, double b, double relEps, double absEps)
{
    const double diff = std::fabs(a - b);
    if (diff <= absEps) return true;
    return diff <= relEps * std::max(std::fabs(a), std::fabs(b));
}

void VehicleTestAcceptor::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    (void)e;
    for (auto const& pv : xb) {
        if (auto* d = dynamic_cast<const Delivery*>(pv.value)) {
            if (deliveriesSeen >= (int)expectedDeliveries.size()) {
                throw std::runtime_error("VehicleTestAcceptor: received more deliveries than expected");
            }
            const Delivery& exp = expectedDeliveries[deliveriesSeen];
            if (d->retailerId != exp.retailerId) {
                throw std::runtime_error("VehicleTestAcceptor: retailerId mismatch");
            }
            if (!nearlyEqual(d->retailerLocation.x, exp.retailerLocation.x) ||
                !nearlyEqual(d->retailerLocation.y, exp.retailerLocation.y) ||
                !nearlyEqual(d->productAmount, exp.productAmount)) {
                throw std::runtime_error("VehicleTestAcceptor: delivery fields mismatch");
            }
            deliveriesSeen++;
        } else if (auto* vc = dynamic_cast<const VehicleCost*>(pv.value)) {
            if (costSeen) {
                throw std::runtime_error("VehicleTestAcceptor: multiple VehicleCost events received");
            }
            if (vc->day != expectedCostDay) {
                throw std::runtime_error("VehicleTestAcceptor: VehicleCost day mismatch");
            }
            if (!nearlyEqual(vc->cost, expectedCost, 1e-9, 1e-9)) {
                std::cerr << "Expected cost: " << expectedCost << ", got: " << vc->cost << std::endl;
                throw std::runtime_error("VehicleTestAcceptor: VehicleCost value mismatch");
            }
            costSeen = true;
            if (deliveriesSeen == (int)expectedDeliveries.size()) {
                std::cout << "Vehicle Test passed!" << std::endl;
            }
        }
    }
}

void VehicleTestAcceptor::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    delta_int();
    delta_ext(0, xb);
}

void VehicleTestAcceptor::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb)
{
    (void)yb;
}

long VehicleTestAcceptor::ta()
{
    return adevs_inf<long>();
}

void VehicleTestAcceptor::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
    g.clear();
}
