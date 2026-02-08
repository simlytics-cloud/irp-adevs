#pragma once

#include "Coordinate.h"

/**
 * @class FacilityProps
 * @brief Represents the immutable properties of a facility in a supply chain system.
 *
 * This class is designed to encapsulate critical properties of a facility,
 * such as geographic location, initial inventory levels, and inventory cost.
 * Instances of `FacilityProps` are immutable and must be fully initialized
 * at the time of creation.
 *
 * @details
 * - `FacilityProps` objects are created with a specified coordinate,
 *   starting inventory, and inventory cost using the parameterized constructor.
 * - Default construction of `FacilityProps` is explicitly disabled to ensure
 *   proper initialization.
 *
 * @note
 * This class is designed to be immutable; all member variables are declared
 * as `const`. Any modification to these properties requires creating a new
 * instance of `FacilityProps`.
 */
class FacilityProps {
  public:
    const Coordinate coordinate;
    const double startingInventory;
    const double inventoryCost;

    FacilityProps() = delete;

    constexpr FacilityProps(const Coordinate& coordinateIn,
                            double startingInventoryIn,
                            double inventoryCostIn)
        : coordinate(coordinateIn),
          startingInventory(startingInventoryIn),
          inventoryCost(inventoryCostIn) {}

};
