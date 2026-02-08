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

/**
 * Updates the state of the retailer when an internal event occurs.
 *
 * This function adjusts the current time based on the next time advance value.
 * It updates the retailer's state by either switching the event type from OPEN
 * to CLOSE, or from CLOSE to OPEN, and performs inventory adjustments and validations.
 *
 * Behavior:
 * - If the next event is of type OPEN:
 *   - Sets the next event type to CLOSE and calculates its scheduled time.
 * - If the next event is of type CLOSE:
 *   - Reduces the current inventory based on daily consumption.
 *   - Sets the next event type to OPEN and calculates its scheduled time.
 *   - Ensures the inventory stays within the defined bounds (minInventory, maxInventory).
 *   - Throws an exception if the inventory is below the minimum or above the maximum.
 *
 * Exceptions:
 * - Throws std::invalid_argument if the currentInventory falls below the minInventory.
 * - Throws std::invalid_argument if the currentInventory exceeds the maxInventory.
 */
void Retailer::delta_int()
{
  currentTime = currentTime + ta();
  if (nextEvent.eventType == RetailerEventType::OPEN) {
    // TODO Implement retailer opening behavior
  } else {
    // TODO Implement retailer closing behavior
  }
}

/**
 * Processes external events when the retailer is in a OPEN state.
 *
 * This function handles external events that represent deliveries. It updates
 * the current inventory by adding the product amounts from valid delivery events.
 * If an invalid or unexpected event type is encountered, an exception is thrown.
 *
 * Behavior:
 * - Increments the current simulation time by the elapsed time since the last event.
 * - Iterates over the incoming events and processes each one:
 *   - Checks if the event is a valid delivery.
 *   - Adds the product amount from each delivery event to the current inventory.
 *   - Throws an exception if the event type is not a delivery.
 *
 * Exceptions:
 * - Throws std::invalid_argument if the event is not of type Delivery.
 *
 * @param e The elapsed time since the last event.
 * @param xb A collection of incoming external events, where each event specifies the port and associated event information.
 */
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
      // TODO Implement behavior upon receipt of a Delivery
    }
    else
    {
      throw std::invalid_argument("Retailer::delta_ext received illegal argument: expected Delivery");
    }
  }
}

/**
 * Updates the state of the retailer when a confluent event occurs.
 *
 * This function handles simultaneous internal and external events by first processing the
 * external event using `delta_ext` and then transitioning the system through the internal
 * logic via `delta_int`. This ensures that the retailer's state is updated consistently
 * in the presence of both types of events.
 *
 * @param xb A bag of external inputs, where each input contains a port identifier and
 *           a pointer to an `IrpEvent` object. This represents external messages or
 *           interactions relevant to the retailer's operations.
 */
void Retailer::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
{
  // TODO Implement confluent state transition
}

/**
 * Generates output events for the retailer based on the current state.
 *
 * This method is responsible for producing an output event when the next event type is
 * `CLOSE`. It calculates the inventory cost for the day and creates a new `InventoryCost`
 * event. The event is then inserted into the output bag to be dispatched to the
 * corresponding output port.
 *
 * Behavior:
 * - If the next event type is `CLOSE`, the following occurs:
 *   - The current day is computed from the simulation time.
 *   - The inventory cost is calculated based on the current inventory, daily consumption,
 *     and the inventory cost rate from the facility's properties.
 *   - A new `InventoryCost` event is created with the calculated cost, retailer ID, and
 *     the current day.
 *   - The event is wrapped in a `PortValue` object and added to the output bag for the
 *     port `dailyInventoryCost`.
 *
 * @param yb A reference to the output bag where generated events will be inserted.
 *           The bag contains `PortValue` objects, which pair the port identifier
 *           with the event to be sent.
 */
void Retailer::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) {
  // TODO Implement output function
}

/**
 * Calculates the time remaining until the next scheduled event for the retailer.
 *
 * The function determines the difference between the scheduled time of the next
 * event and the current time. This value represents the time advancement required
 * before the next internal transition occurs in the simulation.
 *
 * @return The time difference between the next scheduled event and the current time.
 */
long Retailer::ta()
{
  // TODO Update implementation for correct Retailer time acvance
  return std::numeric_limits<long>::max();
}

/**
 * Cleans up memory resources associated with the output bag.
 *
 * This function iterates through all elements in the given output bag,
 * deallocates the dynamically allocated memory for each event value,
 * and then clears the bag to ensure no dangling pointers remain.
 *
 * @param g The output bag containing port-event pairs where the event values
 *          are dynamically allocated instances of IrpEvent.
 *          This function will release their memory and empty the bag.
 */
void Retailer::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g)
{
  for (auto it = g.begin(); it != g.end(); ++it)
  {
    delete (*it).value;
  }
  g.clear();
}