//
// Created by Robert Kewley on 2/3/26.
//

#include "Retailer.h"

#include "Delivery.h"
#include "InventoryCost.h"

Retailer::Retailer(const FacilityProps& facilityPropertiesIn,
                   int retailerIdIn,
                   double minInventoryIn,
                   double maxInventoryIn,
                   double dailyConsumptionIn)
  : facilityProperties(facilityPropertiesIn),
    currentInventory(facilityProperties.startingInventory),
    nextEvent({RetailerEventType::OPEN, 60 * 6}),
    currentTime(0),
    retailerId(retailerIdIn),
    minInventory(minInventoryIn),
    maxInventory(maxInventoryIn),
    dailyConsumption(dailyConsumptionIn) {
}

// Internal transition: called when an internal event occurs.
// Minimal model: no internal state change.
void Retailer::delta_int()
{
  currentTime = currentTime + ta();
  if (nextEvent.eventType == RetailerEventType::OPEN) {
    nextEvent = RetailerEvent{RetailerEventType::CLOSE, currentTime + 10 * 60};
  } else {
    currentInventory = currentInventory - dailyConsumption;
    nextEvent = RetailerEvent{RetailerEventType::OPEN, currentTime + 14 * 60};
    if (currentInventory < minInventory) {
      throw new std::invalid_argument("Retailer::delta_int: inventory below min");
    } else if (currentInventory > maxInventory) {
      throw new std::invalid_argument("Retailer::delta_int: inventory above max");
    }
  }
}

// External transition: apply elapsed time effects + process inputs.
// Minimal behavior: consume inventory based on elapsed time; ignore inputs for now.
void Retailer::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
  if (nextEvent.eventType != RetailerEventType::CLOSE) {
    throw new std::invalid_argument("Retailer::delta_ext: received external event while closed");
  }
  currentTime += e;
  for (auto it = xb.begin(); it != xb.end(); ++it)
  {
    const adevs::PortValue<IrpEvent*, std::string> pv = *it;
    const IrpEvent* evt = pv.value;
    //if (evt == nullptr)
      //throw std::invalid_argument("Retailer::delta_ext received null event pointer");

    const auto* delivery = dynamic_cast<const Delivery*>(evt);
    if (delivery != nullptr)
    {
      currentInventory += delivery->productAmount;
    }
    else
    {
      throw std::invalid_argument("Retailer::delta_ext received illegal argument: expected Delivery");
    }
  }
}

// Confluent transition: internal + external at the same simulation time.
// Common pattern: do internal transition then treat external with e=0.
void Retailer::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
  delta_ext(0.0, xb);
  delta_int();
}

// Output function: produce outputs right before delta_int / delta_conf.
// Minimal model: no outputs.
void Retailer::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) {
  if (nextEvent.eventType == RetailerEventType::CLOSE) {
    const int day = static_cast<int>(currentTime / minutesPerDay) + 1;

    const double cost = (currentInventory - dailyConsumption) * facilityProperties.inventoryCost;

    auto* irp_event = static_cast<IrpEvent*>(new InventoryCost(day, retailerId, cost));
    adevs::PortValue<IrpEvent*, std::string> pv = adevs::PortValue<IrpEvent*, std::string>(dailyInventoryCost, irp_event);

    yb.insert(pv);
  }
}

// Time advance: how long until next internal event.
// Minimal model: never schedules internal events.
long Retailer::ta()
{
  return nextEvent.time - currentTime;
}

// Garbage collection for outputs created by this model.
// Because X = IrpEvent is by-value here, we don't own heap objects => no-op.
void Retailer::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
  for (auto it = g.begin(); it != g.end(); ++it)
  {
    delete (*it).value;
  }
  g.clear();
}