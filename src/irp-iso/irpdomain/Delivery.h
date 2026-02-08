
#pragma once

#include "IrpEvent.h"
#include "Coordinate.h"

/**
 * @class Delivery
 * @brief Represents a delivery event that contains information about a retailer,
 *        the delivery location, and the amount of product delivered. Inherits
 *        from the base class IrpEvent for use in event-driven systems.
 *
 * @details
 * The Delivery class holds the immutable information of a specific delivery
 * event. Each instance is characterized by a retailer ID, the location of
 * the retailer, and the quantity of the product being delivered. This class
 * is designed to support event-driven architectures where delivery events
 * are represented as payloads for further processing.
 *
 * @note The class is marked as non-default constructible to enforce explicit
 *       initialization of its members.
 *
 * @inherits IrpEvent
 *
 * @constructor
 * - `Delivery(int retailerIdIn, const Coordinate& retailerLocationIn, double productAmountIn)`
 *   Initializes a Delivery object with the given retailer ID, retailer location,
 *   and product amount. All parameters are mandatory and ensure the object is
 *   fully initialized upon construction.
 *
 * @member retailerId
 * ID of the retailer associated with the delivery.
 *
 * @member retailerLocation
 * Coordinate representing the location of the retailer receiving the delivery.
 *
 * @member productAmount
 * The amount of product being delivered as part of the event.
 */
class Delivery : public IrpEvent {
  public:
    const int retailerId;
    const Coordinate retailerLocation;
    const double productAmount;

    Delivery() = delete;

    constexpr Delivery(int retailerIdIn, const Coordinate& retailerLocationIn, double productAmountIn)
        : retailerId(retailerIdIn),
          retailerLocation(retailerLocationIn),
          productAmount(productAmountIn) {}
};
