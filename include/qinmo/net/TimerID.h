#pragma once

#include <stdint.h>

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

class TimerID
{
public:
    TimerID() : seq_(0) { }
    ~TimerID() = default;

    TimerID(const TimerID&) = default;
    TimerID& operator=(const TimerID&) = default;

    TimerID(TimerID&&) = default;
    TimerID& operator=(TimerID&&) = default;

    bool operator<(const TimerID& b) const
    {
        return seq_ < b.seq_;
    }

private:
    TimerID(uint64_t seq) : seq_(seq) { }

    uint64_t seq_;

    friend class TimerManager;
};
} // namespace net
} // namespace qinmo