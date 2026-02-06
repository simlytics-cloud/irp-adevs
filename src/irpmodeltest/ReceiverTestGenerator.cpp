#include "ReceiverTestGenerator.h"

#include "Delivery.h"
#include "Coordinate.h"

ReceiverTestGenerator::ReceiverTestGenerator(int retailerIdIn, double amountDay1In, double amountDay2In)
    : currentTime(0),
      nextIdx(0),
      retailerId(retailerIdIn),
      amountDay1(amountDay1In),
      amountDay2(amountDay2In)
{
}

long ReceiverTestGenerator::ta()
{
  if (nextIdx >= 2) return adevs_inf<long>();

  const long t1 = noonMinutes;                      // day 1 @ 12:00
  const long t2 = minutesPerDay + noonMinutes;      // day 2 @ 12:00
  const long target = (nextIdx == 0) ? t1 : t2;

  if (currentTime >= target) return 0; // fire immediately if we're late
  return target - currentTime;
}

void ReceiverTestGenerator::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb)
{
  if (nextIdx >= 2) return;

  const double amt = (nextIdx == 0) ? amountDay1 : amountDay2;

  // Location is arbitrary here; adjust as needed
  const Coordinate loc(0.0, 0.0);

  // The delivery object itself is the event payload
  auto* irp_event = static_cast<IrpEvent*>(new Delivery(retailerId, loc, amt));
  adevs::PortValue<IrpEvent*, std::string> pv = adevs::PortValue<IrpEvent*, std::string>(toReceiveDelivery, irp_event);

  // Note: "toReceiveDelivery" is a label for routing/port logic in your Network.
  // Actually emit the event
  yb.insert(pv);
}

void ReceiverTestGenerator::delta_int()
{
  currentTime += ta();
  ++nextIdx;
}

void ReceiverTestGenerator::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
  // ReceiverTestGenerator ignores inputs; just advance local clock
  currentTime += e;
  (void)xb;
}

void ReceiverTestGenerator::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
  delta_int();
  delta_ext(0, xb);
}

void ReceiverTestGenerator::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
  for (auto it = g.begin(); it != g.end(); ++it)
  {
    delete (*it).value;
  }
  g.clear();
}