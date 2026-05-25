#include "qinmo/net/EventLoopThread.h"
#include "qinmo/net/EventLoop.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

EventLoopThread::EventLoopThread(EventLoopThreadInitFunc func)
    : thread_( [this]() { threadFunc(); } )
    , func_(std::move(func))
    , loop_(nullptr)
    , started_(false)
{ }

EventLoopThread::~EventLoopThread()
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (loop_) loop_->quit();
    }

    thread_.join();
}

EventLoop* EventLoopThread::start()
{
    if (started_.load()) return loop_;

    started_.store(true);
    thread_.start();

    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() -> bool { return nullptr != loop_; });
    }

    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop eventLoop;

    if (func_) func_(&eventLoop);

    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &eventLoop;
    }
    cv_.notify_one();
    eventLoop.loop();

    // loop quit
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = nullptr;
    }
}

} // namespace net
} // namespace qinmo