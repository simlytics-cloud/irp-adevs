#include "VehicleTestGenerator.h"

VehicleTestGenerator::VehicleTestGenerator(const DeliveryRoute& routeIn, long sendTimeIn)
    : currentTime(0),
      sent(false),
      route(routeIn),
      sendTime(sendTimeIn)
{
}

void VehicleTestGenerator::delta_int()
{
    currentTime += ta();
    sent = true;
}

void VehicleTestGenerator::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    currentTime += e;
    (void)xb;
}

void VehicleTestGenerator::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
    delta_int();
    delta_ext(0, xb);
}

void VehicleTestGenerator::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb)
{
    if (!sent) {
        auto* irp_event = static_cast<IrpEvent*>(new DeliveryRoute(route));
        adevs::PortValue<IrpEvent*, std::string> pv(toAcceptDeliveryRoute, irp_event);
        yb.insert(pv);
    }
}

long VehicleTestGenerator::ta()
{
    if (sent) return adevs_inf<long>();
    if (currentTime >= sendTime) return 0;
    return sendTime - currentTime;
}

void VehicleTestGenerator::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
    for (auto it = g.begin(); it != g.end(); ++it)
    {
        delete (*it).value;
    }
    g.clear();
}
