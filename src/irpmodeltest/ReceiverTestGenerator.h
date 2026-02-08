#pragma once

#include "adevs.h"
#include "IrpEvent.h"

#include <string>

/**
 * @brief A test generator class for simulating deliveries to a receiver using the adevs DEVS (Discrete Event System Specification) framework.
 *
 * This class represents a generator that simulates the sending of delivery events over two days.
 * It generates events at specific times and associates them with a retailer and delivery amount.
 * The generated events are transmitted via a specified port.
 *
 * @details
 * The `ReceiverTestGenerator` is derived from `adevs::Atomic`, representing an atomic DEVS model.
 * It overrides the core DEVS lifecycle methods (`delta_int`, `delta_ext`, `delta_conf`, `output_func`, `ta`, and `gc_output`)
 * for state transitions, external event handling, output generation, and garbage collection.
 *
 * The simulation logic involves creating and emitting delivery events at designated times (noon on day 1 and day 2).
 * The class maintains internal state to track the current time and determines whether additional deliveries need to be generated.
 */
class ReceiverTestGenerator : public adevs::Atomic<adevs::PortValue<IrpEvent*, std::string>, long>
{
  public:
    inline static const std::string toReceiveDelivery{"toReceiveDelivery"};

    ReceiverTestGenerator(int retailerIdIn, double amountDay1In, double amountDay2In);

    void delta_int() override;
    void delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void delta_conf(const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb) override;
    void output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) override;
    long ta() override;
    void gc_output(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& g) override;

  private:
    static constexpr long minutesPerDay = 24 * 60;
    static constexpr long noonMinutes = 12 * 60;

    long currentTime;
    int nextIdx; // 0 -> day1 noon, 1 -> day2 noon, 2 -> done

    const int retailerId;
    const double amountDay1;
    const double amountDay2;
};