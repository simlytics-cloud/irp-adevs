#include "ManufacturerTestGenerator.h"
#include "Coordinate.h"

ManufacturerTestGenerator::ManufacturerTestGenerator(const DeliverySchedule& scheduleIn, double deliveryAmountIn, long deliveryTimeIn)
    : currentTime(0),
      nextIdx(0),
      schedule(scheduleIn),
      deliveryAmount(deliveryAmountIn),
      deliveryTime(deliveryTimeIn)
{
}

long ManufacturerTestGenerator::ta()
{
    if (nextIdx >= 2) return adevs_inf<long>();

    const long target = (nextIdx == 0) ? 0 : deliveryTime;

    if (currentTime >= target) return 0;
    return target - currentTime;
}

void ManufacturerTestGenerator::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb)
{
    if (nextIdx == 0) {
        auto* irp_event = static_cast<IrpEvent*>(new DeliverySchedule(schedule));
        adevs::PortValue<IrpEvent*, std::string> pv(toAcceptDeliverySchedule, irp_event);
        yb.insert(pv);
    } else if (nextIdx == 1) {
        auto* irp_event = static_cast<IrpEvent*>(new Delivery(0, Coordinate(0.0, 0.0), deliveryAmount));
        adevs::PortValue<IrpEvent*, std::string> pv(toAcceptDelivery, irp_event);
        yb.insert(pv);
    }
}

void ManufacturerTestGenerator::delta_int()
{
    currentTime += ta();
    ++nextIdx;
}

void ManufacturerTestGenerator::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    currentTime += e;
    (void)xb;
}

void ManufacturerTestGenerator::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    delta_int();
    delta_ext(0, xb);
}

void ManufacturerTestGenerator::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
    for (auto it = g.begin(); it != g.end(); ++it)
    {
        delete (*it).value;
    }
    g.clear();
}
