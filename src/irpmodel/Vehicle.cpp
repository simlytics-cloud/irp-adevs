#include "Vehicle.h"

Vehicle::Vehicle(int vehicleIdIn,
                 double capacityIn,
                 double dailyKmTraveledIn,
                 const DeliveryRoute& deliveryRouteIn,
                 const Coordinate& locationIn,
                 double costPerKmIn,
                 double speedKmHrIn) :
    adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long>(),
    vehicleId(vehicleIdIn),
    capacity(capacityIn),
    dailyKmTraveled(dailyKmTraveledIn),
    deliveries(deliveryRouteIn.deliveries.begin(), deliveryRouteIn.deliveries.end()),
    locX(locationIn.x),
    locY(locationIn.y),
    baseLocX(locationIn.x),
    baseLocY(locationIn.y),
    costPerKm(costPerKmIn),
    speedKmHr(speedKmHrIn),
    nextEvent{0, VehicleEvent::NONE}
{
}

void Vehicle::initialize(long initialTime) {
    currentTime = initialTime;
}

void Vehicle::delta_int() {
    currentTime += ta();
    double d = 0;
    if (nextEvent.type == VehicleEvent::DELIVERY) {
        if (!deliveries.empty()) {
            d = distance(locX, locY, deliveries.front().retailerLocation.x, deliveries.front().retailerLocation.y);
            locX = deliveries.front().retailerLocation.x;
            locY = deliveries.front().retailerLocation.y;
            dailyKmTraveled += d;
            deliveries.pop_front();
        }
    } else if (nextEvent.type == VehicleEvent::RETURN) {
        d = distance(locX, locY, baseLocX, baseLocY);
        locX = baseLocX;
        locY = baseLocY;
        dailyKmTraveled = 0.0;
        deliveries.clear();
    }
    scheduleNextEvent();
}

void Vehicle::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) {
    currentTime += e;
    for (auto const& pv : xb) {
        if (pv.port == acceptDeliveryRoute) {
            const auto* route = dynamic_cast<const DeliveryRoute*>(pv.value);
            if (route != nullptr) {
                deliveries.clear();
                for (const auto& d : route->deliveries) {
                    deliveries.push_back(d);
                }
            }
        }
    }
    scheduleNextEvent();
}

void Vehicle::delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) {
    delta_int();
    delta_ext(0, xb);
}
void Vehicle::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) {
    if (nextEvent.type == VehicleEvent::DELIVERY) {
        if (!deliveries.empty()) {
            // Get the first delivery
            const Delivery& d = deliveries.front();
            // Output to dropDelivery port
            yb.insert(adevs::PortValue<IrpEvent*, std::string>(dropDelivery, new Delivery(d)));
        }
    } else if (nextEvent.type == VehicleEvent::RETURN) {
        if (!deliveries.empty()) {
            double totalUndelivered = 0.0;
            for (const auto& d : deliveries) {
                totalUndelivered += d.productAmount;
            }
            if (totalUndelivered > 0) {
                // Aggregate into one Delivery for the "manufacturer" (retailerId 0)
                yb.insert(adevs::PortValue<IrpEvent*, std::string>(dropDelivery, new Delivery(0, Coordinate(locX, locY), totalUndelivered)));
            }
        }
        // Generate VehicleCost report
        int day = static_cast<int>(nextEvent.time / (24 * 60)) + 1;
        double d = distance(locX, locY, baseLocX, baseLocY);
        double cost = (dailyKmTraveled + d) * costPerKm;
        yb.insert(adevs::PortValue<IrpEvent*, std::string>(dailyDeliveryCost, new VehicleCost(day, vehicleId, cost)));
    }
}
long Vehicle::ta() {
    if (nextEvent.type == VehicleEvent::NONE) {
        return adevs_inf<long>();
    }
    return nextEvent.time - currentTime;
}
void Vehicle::gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) {}

void Vehicle::scheduleNextEvent() {
    bool canScheduleDelivery = false;
    if (!deliveries.empty()) {
        double d = distance(locX, locY, deliveries.front().retailerLocation.x, deliveries.front().retailerLocation.y);
        long arrivalTime = currentTime + static_cast<long>(std::round((d / speedKmHr) * 60.0));
        long closingTime = (currentTime / 1440) * 1440 + 16 * 60;
        if (arrivalTime <= closingTime) {
            nextEvent.type = VehicleEvent::DELIVERY;
            nextEvent.time = arrivalTime;
            canScheduleDelivery = true;
        }
    }

    if (!canScheduleDelivery) {
        if (distance(locX, locY, baseLocX, baseLocY) > 0.001) { // Floating point comparison
            double d = distance(locX, locY, baseLocX, baseLocY);
            nextEvent.type = VehicleEvent::RETURN;
            nextEvent.time = currentTime + static_cast<long>(std::round((d / speedKmHr) * 60.0));
        } else {
            nextEvent.type = VehicleEvent::NONE;
            nextEvent.time = 0; // Not used when type is NONE
        }
    }
}

double Vehicle::distance(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}
