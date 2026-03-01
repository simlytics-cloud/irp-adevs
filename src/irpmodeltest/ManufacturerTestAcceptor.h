#pragma once

#include "adevs.h"
#include "IrpEvent.h"
#include <vector>
#include <string>

class ManufacturerTestAcceptor : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long>
{
  public:
    ManufacturerTestAcceptor(const std::vector<double>& expectedCosts, int expectedRouteCount);

    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    const std::vector<double> expectedCosts;
    const int expectedRouteCount;

    int costIndex = 0;
    int routeCount = 0;

    static bool nearlyEqual(double a, double b, double relEps = 1e-9, double absEps = 1e-12);
};
