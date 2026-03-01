#pragma once

#include <map>
#include "adevs.h"
#include "FacilityProps.h"
#include "IrpEvent.h"
#include "DeliveryRoute.h"

constexpr long minutesPerDay = 24 * 60;

class Manufacturer : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long> {
  public:
    // Ports. Using string as port identifier instead of the default int
    inline static const std::string acceptDeliverySchedule{"acceptDeliverySchedule"};
    inline static const std::string acceptDelivery{"acceptDelivery"};
    inline static const std::string postDeliveryRoute{"postDeliveryRoute"};
    inline static const std::string dailyInventoryCost{"dailyInventoryCost"};

    Manufacturer(const FacilityProps& facilityPropertiesIn,
                 double startingInventoryIn,
                 double dailyProductionIn);

    void initialize(long initialTime);
    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    enum class ManufacturerEventType {LOAD, REPORT};
    struct ManufacturerEvent {
      ManufacturerEventType eventType;
      long time;
    };
    
    // SysMLv2 attributes
    const FacilityProps facilityProperties;  // readonly
    double currentInventory;                 // mutable
    const double startingInventory;          // readonly
    const double dailyProduction;            // readonly

    ManufacturerEvent nextEvent;
    long currentTime;
    std::map<int, std::map<int, DeliveryRoute>> deliverySchedule;
};
