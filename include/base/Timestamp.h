#pragma once

#include "Operator.h"

#include <string>
#include <chrono>

/// @namespace qinmo
namespace qinmo
{
    class Timestamp : public qinmo::comp_less<Timestamp>,
                      public qinmo::comp_equal<Timestamp>
    {
    public:
        using micro = long long;

        /// @brief construct with 0
        Timestamp();
        /// @brief construct with specific time
        /// @param microsecondsSinceEpoch the number of microseconds since the beginning of the epoch
        Timestamp(micro microsecondsSinceEpoch);

    public:
        /// @brief get time by long long type
        micro getMicroseconds() const;
        micro getSeconds() const;

        /// @brief obtain formatted string to : year-month-day hour:minute:second
        /// @param local whether to use the local time zone, not used by default
        std::string toString(bool local = false) const;
        std::string toStringMicroseconds(bool local = false) const;

        /// @brief get time now
        /// @return qinmo::Timestamp object
        static Timestamp now();

    public:
        // microsecond to second = 1e6
        static const int MicToSec = 1000000;

    private:
        micro microsecond_;
    };
}