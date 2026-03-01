#include "Manufacturer.h"
#include "InventoryCost.h"
#include "DeliveryRoute.h"
#include "DeliverySchedule.h"
#include "Delivery.h"

Manufacturer::Manufacturer(const FacilityProps& facilityPropertiesIn,
                           double startingInventoryIn,
                           double dailyProductionIn)
  : facilityProperties(facilityPropertiesIn),
    currentInventory(startingInventoryIn),
    startingInventory(startingInventoryIn),
    dailyProduction(dailyProductionIn),
    nextEvent({ManufacturerEventType::LOAD, 6 * 60}),
    currentTime(0),
    deliverySchedule() {
}

void Manufacturer::initialize(long initialTime) {
  this->currentTime = initialTime;
  this->currentInventory = startingInventory;

  const long currentDayStart = (initialTime / minutesPerDay) * minutesPerDay;
  const long todayLoad = currentDayStart + 6 * 60;
  const long todayReport = currentDayStart + 23 * 60 + 59;

  if (initialTime < todayLoad) {
    this->nextEvent = {ManufacturerEventType::LOAD, todayLoad};
  } else if (initialTime < todayReport) {
    this->nextEvent = {ManufacturerEventType::REPORT, todayReport};
  } else {
    this->nextEvent = {ManufacturerEventType::LOAD, todayLoad + minutesPerDay};
  }
}

long Manufacturer::ta() {
  return nextEvent.time - currentTime;
}

void Manufacturer::delta_int() {
  const long elapsed = ta();
  currentTime += elapsed;

  if (nextEvent.eventType == ManufacturerEventType::REPORT) {
    // End of day report: add production to inventory
    currentInventory += dailyProduction;

    // Schedule next day's LOAD event (06:00 tomorrow)
    const long currentDayStart = (currentTime / minutesPerDay) * minutesPerDay;
    nextEvent = {ManufacturerEventType::LOAD, currentDayStart + minutesPerDay + 6 * 60};
  } else if (nextEvent.eventType == ManufacturerEventType::LOAD) {
    // Load event: decrement inventory by the total load amount for today
    const int day = static_cast<int>(currentTime / minutesPerDay) + 1;
    double loadAmount = 0.0;
    if (deliverySchedule.count(day) > 0) {
      for (auto const& [vehicleId, route] : deliverySchedule.at(day)) {
        for (auto const& delivery : route.deliveries) {
          loadAmount += delivery.productAmount;
        }
      }
    }
    currentInventory -= loadAmount;

    // Schedule today's REPORT event (23:59 today)
    const long currentDayStart = (currentTime / minutesPerDay) * minutesPerDay;
    nextEvent = {ManufacturerEventType::REPORT, currentDayStart + 23 * 60 + 59};
  }
}

void Manufacturer::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) {
  currentTime += e;
  for (auto const& pv : xb) {
    if (pv.port == acceptDeliverySchedule) {
      const auto* schedule = dynamic_cast<const DeliverySchedule*>(pv.value);
      if (schedule != nullptr) {
        // Since deliverySchedule is a map of maps, and the event's deliveriesByDayByVehicle
        // is also a map of maps, we need to be careful if we want to "update" or "replace".
        // The issue description says "put the value in the deliverySchedule variable".
        // If it means replace:
        // deliverySchedule = schedule->deliveriesByDayByVehicle;
        // However, map assignment fails if the value type (map<int, DeliveryRoute>) is not assignable.
        // Let's try to clear and insert.
        deliverySchedule.clear();
        for (auto const& [day, vehicleMap] : schedule->deliveriesByDayByVehicle) {
          for (auto const& [vehicleId, route] : vehicleMap) {
            deliverySchedule[day].insert({vehicleId, route});
          }
        }
      }
    } else if (pv.port == acceptDelivery) {
      const auto* delivery = dynamic_cast<const Delivery*>(pv.value);
      if (delivery != nullptr) {
        currentInventory += delivery->productAmount;
      }
    }
  }
}

void Manufacturer::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) {
  delta_ext(0, xb);
  delta_int();
}

void Manufacturer::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) {
  if (nextEvent.eventType == ManufacturerEventType::REPORT) {
    const int day = static_cast<int>(currentTime / minutesPerDay) + 1;
    const double cost = (currentInventory + dailyProduction) * facilityProperties.inventoryCost;
    auto* irp_event = static_cast<IrpEvent*>(new InventoryCost(day, 0, cost));
    adevs::PortValue<IrpEvent*, std::string> pv(dailyInventoryCost, irp_event);
    yb.insert(pv);
  } else if (nextEvent.eventType == ManufacturerEventType::LOAD) {
    const int day = static_cast<int>(currentTime / minutesPerDay) + 1;
    if (deliverySchedule.count(day) > 0) {
      for (auto const& [vehicleId, route] : deliverySchedule.at(day)) {
        auto* route_event = static_cast<IrpEvent*>(new DeliveryRoute(route));
        adevs::PortValue<IrpEvent*, std::string> pv(postDeliveryRoute, route_event);
        yb.insert(pv);
      }
    }
  }
}

void Manufacturer::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) {
  for (auto it = g.begin(); it != g.end(); ++it) {
    delete (*it).value;
  }
  g.clear();
}
