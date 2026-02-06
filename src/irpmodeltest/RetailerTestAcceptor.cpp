#include "RetailerTestAcceptor.h"

#include "InventoryCost.h"

#include <stdexcept>
#include <cmath>

RetailerTestAcceptor::RetailerTestAcceptor(int retailerIdIn,
                                           double expectedDay1CostIn,
                                           double expectedDay2CostIn)
    : seenCount(0),
      retailerId(retailerIdIn),
      expectedDay1Cost(expectedDay1CostIn),
      expectedDay2Cost(expectedDay2CostIn)
{
}

void RetailerTestAcceptor::delta_int()
{
    // No internal events
}

bool RetailerTestAcceptor::nearlyEqual(double a, double b, double relEps, double absEps)
{
    const double diff = std::fabs(a - b);
    if (diff <= absEps) return true;
    return diff <= relEps * std::max(std::fabs(a), std::fabs(b));
}

void RetailerTestAcceptor::checkCostEvent(const InventoryCost& ic) {
    if (ic.retailerId != retailerId)
        throw std::invalid_argument("RetailerTestAcceptor: wrong retailerId on InventoryCost");

    if (seenCount == 0)
    {
        if (ic.day != 1)
            throw std::invalid_argument("RetailerTestAcceptor: first InventoryCost must have day == 1");
        if (!nearlyEqual(ic.cost, expectedDay1Cost))
            throw std::invalid_argument("RetailerTestAcceptor: day 1 cost does not match expected value");
        receivedDay1Report = true;;
    }
    else if (seenCount == 1)
    {
        if (ic.day != 2)
            throw std::invalid_argument("RetailerTestAcceptor: second InventoryCost must have day == 2");
        if (!nearlyEqual(ic.cost, expectedDay2Cost))
            throw std::invalid_argument("RetailerTestAcceptor: day 2 cost does not match expected value");
        if (!receivedDay1Report)
            throw std::invalid_argument("RetailerTestAcceptor: received day 2 cost before day 1 report");
        std::cout << "Retailer Test passed!" << std::endl;
    }
    else
    {
        throw std::invalid_argument("RetailerTestAcceptor: received more than two InventoryCost events");
    }
}

void RetailerTestAcceptor::delta_ext(
    long e,
    const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    (void)e;

    for (auto it = xb.begin(); it != xb.end(); ++it)
    {
        const auto& pv = *it;
        const IrpEvent* evt = pv.value;
        if (evt == nullptr)
            throw std::invalid_argument("RetailerTestAcceptor: received null event pointer");

        const auto* ic = dynamic_cast<const InventoryCost*>(evt);
        if (ic == nullptr)
            throw std::invalid_argument("RetailerTestAcceptor: illegal argument (expected InventoryCost)");

        checkCostEvent(*ic);
        ++seenCount;
    }
}


void RetailerTestAcceptor::delta_conf(
    const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    delta_int();
    delta_ext(0, xb);
}

void RetailerTestAcceptor::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb)
{
    // No output by design
    (void)yb;
}

long RetailerTestAcceptor::ta()
{
    return adevs_inf<long>();
}

void RetailerTestAcceptor::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
    // No outputs allocated by this model
    g.clear();
}