#pragma once

#include "Channel.h"
#include "qinmo/base/ThreadConfig.h"
#include "TimerManager.h"
#include <mutex>

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

class Poller;

class EventLoop
{
public:
    using Functor = std::function<void()>;

public:
    EventLoop();
    ~EventLoop();

    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;

    EventLoop(EventLoop&&) = delete;
    EventLoop& operator=(EventLoop&&) = delete;

public:
    void loop();
    void quit();

    void setTimeoutMs(int ms);

    bool hasChannel(Channel& channel) const;
    void setChannel(Channel& channel);
    void removeChannel(Channel& channel);

    void wakeup();
    void wakeupRead();

    void runInLoop(Functor func);
    void queueInLoop(Functor func);

    TimerID timerAt(Timestamp timestamp, TimerFunc func);
    TimerID timerAfter(double seconds, TimerFunc func);
    TimerID timerRepeatAt(Timestamp timestamp, double intervalSeconds, TimerFunc func);
    TimerID timerRepeatAfter(double beginSeconds, double intervalSeconds, TimerFunc func);
    void timerCancel(TimerID id);

    bool isInCurrentThread() const;

private:
    void pendingFunctors();

private:
    std::unique_ptr<Poller> poller_;
    Timestamp pollerReturnTime_;
    std::atomic<int> timeoutMs_;

    int wakeupfd_;
    std::unique_ptr<Channel> wakeupChannel_;

    TimerManager timerManager_;

    ChannelList activeChannels_;

    std::vector<Functor> pendingFunctors_;
    std::mutex mutex_;

    std::atomic<bool> quit_;
    std::atomic<bool> looping_;
    std::atomic<bool> pending_;
    const qinmo::detail::ThreadIDType tid_;

};
} // namespace net
} // namespace qinmo