#pragma once

#include <list>
#include "adevs.h"
#include "IrpEvent.h"
#include "DeliveryRoute.h"
#include "Coordinate.h"
#include "Delivery.h"
#include "VehicleCost.h"

struct VehicleEvent {
    enum NextEventType { DELIVERY, RETURN, NONE };
    long time;
    NextEventType type;
};

class Vehicle : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long> {
  public:
    // Ports. Using string as port identifier instead of the default int
    inline static const std::string acceptDeliveryRoute{"acceptDeliveryRoute"};
    inline static const std::string dropDelivery{"dropDelivery"};
    inline static const std::string dailyDeliveryCost{"dailyDeliveryCost"};

    Vehicle(int vehicleIdIn,
            double capacityIn,
            double dailyKmTraveledIn,
            const DeliveryRoute& deliveryRouteIn,
            const Coordinate& locationIn,
            double costPerKmIn,
            double speedKmHrIn);

    void initialize(long initialTime);
    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    void scheduleNextEvent();
    static double distance(double x1, double y1, double x2, double y2);
    // SysMLv2 attributes
    const int vehicleId;              // readonly
    const double capacity;            // readonly
    double dailyKmTraveled;
    std::list<Delivery> deliveries;
    double locX, locY;
    const double baseLocX, baseLocY;
    const double costPerKm;           // readonly
    const double speedKmHr;           // readonly

    long currentTime;
    VehicleEvent nextEvent;
};
