#pragma once

#include "adevs.h"
#include "IrpEvent.h"
#include "Delivery.h"
#include "VehicleCost.h"
#include <vector>
#include <string>

class VehicleTestAcceptor : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long>
{
  public:
    VehicleTestAcceptor(const std::vector<Delivery>& expectedDeliveries,
                        int expectedCostDay,
                        double expectedCost);

    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    const std::vector<Delivery> expectedDeliveries;
    const int expectedCostDay;
    const double expectedCost;

    int deliveriesSeen = 0;
    bool costSeen = false;

    static bool nearlyEqual(double a, double b, double relEps = 1e-9, double absEps = 1e-12);
};
