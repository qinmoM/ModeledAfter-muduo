#pragma once

#include "EventLoopThread.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

/// @brief encapsulate multiple EventLoopThread
class EventLoopThreadLoop
{
public:
    EventLoopThreadLoop(int numSubThread = 0);
    ~EventLoopThreadLoop() = default;

    EventLoopThreadLoop(const EventLoopThreadLoop&) = delete;
    EventLoopThreadLoop& operator()(const EventLoopThreadLoop&) = delete;

    EventLoopThreadLoop(EventLoopThreadLoop&&) = delete;
    EventLoopThreadLoop& operator()(EventLoopThreadLoop&&) = delete;

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

};
} // namespace net
} // namespace qinmo