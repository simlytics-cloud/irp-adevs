#pragma once

#include "IrpEvent.h"

/**
 * @class InventoryCost
 * @brief Represents the cost associated with maintaining inventory for a retailer on a given day.
 *
 * This class tracks the day, retailer identifier, and inventory cost. It inherits from IrpEvent,
 * enabling it to be used in event-driven systems.
 *
 * @details
 * The class is designed to be immutable, with its members initialized via the constructor and
 * being constant. The `day` member represents the corresponding day for the recorded cost.
 * The `retailerId` identifies the retailer associated with the cost, and the `cost` specifies
 * the computed cost of maintaining the inventory for the day.
 */
class InventoryCost : public IrpEvent {
  public:
    const int day;
    const int retailerId;
    const double cost;

    constexpr InventoryCost(int dayIn, int retailerIdIn, double costIn)
        : day(dayIn),
          retailerId(retailerIdIn),
          cost(costIn) {}

};
