#pragma once

#include "adevs.h"
#include "IrpEvent.h"

#include <string>

/**
 * @class RetailerTestAcceptor
 * @brief A simulation component used to validate the inventory cost behavior of a retailer in a discrete-event system.
 *
 * This class is an extension of the adevs::Atomic model that acts as a test acceptor
 * for verifying cost-reported events during simulation. The validation involves checking
 * whether the received inventory cost matches the expected values for specific simulation days.
 *
 * The expected costs for day 1 and day 2 are provided during initialization.
 *
 * The class also ensures the correctness of event ordering and performs error handling for
 * scenarios where unexpected or extra events are received.
 */
class RetailerTestAcceptor
    : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long>
{
  public:
    RetailerTestAcceptor(int retailerIdIn, double expectedDay1CostIn, double expectedDay2CostIn);

    void delta_int() override;
    void delta_ext(long e,
                   const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    int seenCount; // 0 -> expecting day1, 1 -> expecting day2, >=2 -> extra is error
    const int retailerId;
    const double expectedDay1Cost;
    const double expectedDay2Cost;
    bool receivedDay1Report = false;

    static bool nearlyEqual(double a, double b, double relEps = 1e-9, double absEps = 1e-12);
    void checkCostEvent(const class InventoryCost& ic);
};