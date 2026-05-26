#pragma once

#include "../base/Timestamp.h"
#include <functional>
#include <atomic>

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

using TimerFunc = std::function<void()>;

class Timer
{
public:
    Timer(TimerFunc func, Timestamp when, double intervalSeconds);
    ~Timer();

public:
    uint64_t getSequence() const;
    Timestamp getTimestamp() const;
    int64_t getInterval() const;

    void run();
    void reset();

private:
    static std::atomic<uint64_t> s_currSeq;

    TimerFunc func_;
    qinmo::Timestamp time_;
    uint64_t seq_;
    int64_t interval_;

};
} // namespace net
} // namespace qinmo