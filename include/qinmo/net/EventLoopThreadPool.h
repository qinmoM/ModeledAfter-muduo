#pragma once

#include "EventLoopThread.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

/// @brief encapsulate multiple EventLoopThread
class EventLoopThreadPool
{
public:
    EventLoopThreadPool(EventLoop* mainLoop, unsigned int numSubThread = 0);
    ~EventLoopThreadPool() = default;

    EventLoopThreadPool(const EventLoopThreadPool&) = delete;
    EventLoopThreadPool& operator()(const EventLoopThreadPool&) = delete;

    EventLoopThreadPool(EventLoopThreadPool&&) = delete;
    EventLoopThreadPool& operator()(EventLoopThreadPool&&) = delete;

public:
    /// @brief return true if started all loops.
    bool started() const;
    /// @brief get all EventLoop pointers
    std::vector<EventLoop*> getAllLoops();

    /// @brief start loop
    /// @param func initialization function
    void start(EventLoopThread::EventLoopThreadInitFunc func = [](EventLoop*) -> void {});
    /// @brief fast get next EventLoop by polling
    EventLoop* getNextLoop();

private:
    EventLoop* baseLoop_;

    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;

    std::atomic<bool> started_;
    std::atomic<unsigned int> next_;

};
} // namespace net
} // namespace qinmo