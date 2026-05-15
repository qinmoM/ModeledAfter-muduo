#pragma once

#include "qinmo/base/Thread.h"
#include "EventLoop.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

class EventLoopThread
{
public:
    using EventLoopThreadInitFunc = std::function<void(EventLoop*)>;

public:
    /// @param func initialization function
    EventLoopThread(EventLoopThreadInitFunc func = [](EventLoop*) -> void {});
    ~EventLoopThread();

    EventLoopThread(const EventLoopThread&) = delete;
    EventLoopThread& operator=(const EventLoopThread&) = delete;

    EventLoopThread(EventLoopThread&&) = delete;
    EventLoopThread& operator=(EventLoopThread&&) = delete;

public:
    /// @brief start EventLoop and Thread, Get EventLoop pointer
    /// @note
    /// - only return EventLoop pointer when already started
    /// - blocking until EventLoop has created
    /// - EventLoop is stack variable
    EventLoop* start();

private:
    /// @brief pass to the thread
    void threadFunc();

    std::atomic<bool> started_;

    qinmo::Thread thread_;
    EventLoopThreadInitFunc func_;

    EventLoop* loop_;
    std::mutex mutex_;
    std::condition_variable cv_;

};
} // namespace net
} // namespace qinmo