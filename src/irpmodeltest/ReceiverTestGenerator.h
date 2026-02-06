#pragma once

#include "adevs.h"
#include "IrpEvent.h"

#include <string>

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