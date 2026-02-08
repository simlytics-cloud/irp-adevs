#pragma once

/**
 * @class IrpEvent
 * @brief A base class representing immutable events in the IRP (Inventory Routing Problem) domain.
 *
 * IrpEvent serves as an abstract base class for all event types in the system,
 * enabling safe polymorphic behavior and deletion. Subclasses define specific
 * types of IRP-related events.
 */
class IrpEvent  {
  public:
    virtual ~IrpEvent() = default; // enables safe polymorphic deletion

};
