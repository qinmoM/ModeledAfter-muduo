#pragma onc

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

private:
    TimerID(uint64_t seq) : seq_(seq) { }

    uint64_t seq_;

    friend class TimerManager;
};
} // namespace net
} // namespace qinmo