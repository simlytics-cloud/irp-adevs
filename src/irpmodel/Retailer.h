#pragma once

#include "adevs.h"
#include "FacilityProps.h"
#include "IrpEvent.h"

constexpr long minutesPerDay = 24 * 60;

class Retailer: public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long> {
  public:
    // Ports.  Note using string as port identifier instead of the default int
    inline static const std::string receiveDelivery{"receiveDelivery"};
    inline static const std::string dailyInventoryCost{"dailyInventoryCost"};

    Retailer(const FacilityProps& facilityPropertiesIn,
             int retailerIdIn,
             double minInventoryIn,
             double maxInventoryIn,
             double dailyConsumptionIn);

    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;
  private:
    enum class RetailerEventType {OPEN, CLOSE};
    struct RetailerEvent {
      RetailerEventType eventType;
      long time;
    };
    // SysMLv2 attributes
    const FacilityProps facilityProperties;  // readonly
    double currentInventory;                 // mutable
    RetailerEvent nextEvent;
    long currentTime;
    const int retailerId;                    // readonly
    const double minInventory;               // readonly
    const double maxInventory;               // readonly
    const double dailyConsumption;           // readonly
};

