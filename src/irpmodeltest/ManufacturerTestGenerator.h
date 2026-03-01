#pragma once

#include "adevs.h"
#include "IrpEvent.h"
#include "DeliverySchedule.h"
#include "Delivery.h"

#include <string>

class ManufacturerTestGenerator : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long>
{
  public:
    inline static const std::string toAcceptDeliverySchedule{"toAcceptDeliverySchedule"};
    inline static const std::string toAcceptDelivery{"toAcceptDelivery"};

    ManufacturerTestGenerator(const DeliverySchedule& schedule, double deliveryAmount, long deliveryTime);

    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    long currentTime;
    int nextIdx; // 0 -> schedule, 1 -> delivery, 2 -> done

    const DeliverySchedule schedule;
    const double deliveryAmount;
    const long deliveryTime;
};
