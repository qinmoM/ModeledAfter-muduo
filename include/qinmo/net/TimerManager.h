#pragma once

#include "Channel.h"
#include "Timer.h"
#include "TimerID.h"
#include <set>
#include <unordered_map>

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

class TimerManager
{
public:
    explicit TimerManager(EventLoop* loop);
    ~TimerManager();

    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;

    TimerManager(TimerManager&&) = delete;
    TimerManager& operator=(TimerManager&&) = delete;

public:
    TimerID addEvent(TimerFunc func, Timestamp when, double interval);
    void cancelEvent(TimerID id);

private:
    void handleRead(Timestamp time);
    void resetTimerfd(Timestamp time);

private:
    using TimerEntry = std::pair<qinmo::Timestamp, uint64_t>;


    EventLoop* loop_;

    int fd_;
    Channel channel_;

    std::unordered_map<uint64_t, std::unique_ptr<Timer>> timers_;
    std::set<TimerEntry> timersOrder_;
    std::set<uint64_t> timersCancel_;

};
} // namespace net
} // namespace qinmo