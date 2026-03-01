#pragma once

#include "adevs.h"
#include "IrpEvent.h"
#include "DeliveryRoute.h"
#include <string>

class VehicleTestGenerator : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long>
{
  public:
    inline static const std::string toAcceptDeliveryRoute{"toAcceptDeliveryRoute"};

    VehicleTestGenerator(const DeliveryRoute& route, long sendTime);

    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    long currentTime;
    bool sent;
    const DeliveryRoute route;
    const long sendTime;
};
