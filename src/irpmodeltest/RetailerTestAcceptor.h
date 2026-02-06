#pragma once

#include "adevs.h"
#include "IrpEvent.h"

#include <string>

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