#include "qinmo/net/TimerManager.h"
#include "qinmo/net/EventLoop.h"
#include "qinmo/base/Logger.h"
#include "qinmo/net/detail/Wrapper.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

TimerManager::TimerManager(EventLoop* loop)
    : loop_(loop)
    , fd_(qinmo::detail::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC))
    , channel_(loop_, fd_)
    , currTimer_(0)
    , cancelTimer_(0)
{
    if (0 > fd_)
    {
        QINMO_FATAL("TimerManager: Failed to create timerfd.");
        std::terminate();
    }

    channel_.setReadEvent( [this](qinmo::Timestamp time) -> void { handleRead(time); } );
    channel_.enableRead();
}

TimerManager::~TimerManager()
{
    channel_.disableAll();
    channel_.remove();
    qinmo::detail::close(fd_);
}

TimerID TimerManager::addEvent(TimerFunc func, Timestamp when, double interval)
{
    Timer* tPtr = new Timer(func, when, interval);

    loop_->runInLoop(
        [this, tPtr]()
        {
            std::unique_ptr<Timer> timerPtr(tPtr);

            if (timersOrder_.empty() || timersOrder_.begin()->first > timerPtr->getTimestamp())
                resetTimerfd(timerPtr->getTimestamp());

            timersOrder_.insert({ timerPtr->getTimestamp(), timerPtr->getSequence() });
            timers_[timerPtr->getSequence()] = std::move(timerPtr);
        }
    );

    return TimerID(tPtr->getSequence());
}

void TimerManager::cancelEvent(TimerID id)
{
    loop_->runInLoop(
        [this, id]()
        {
            auto it = timers_.find(id.seq_);
            if (it == timers_.end())
                return;

            uint64_t s = timersOrder_.begin()->second;
            timersOrder_.erase(TimerEntry{ it->second->getTimestamp(), id.seq_ });
            if (id.seq_ == currTimer_)
                cancelTimer_ = id.seq_;
            else
                timers_.erase(it);

            if (s != timersOrder_.begin()->second)
                resetTimerfd(timersOrder_.begin()->first);
        }
    );
}

void TimerManager::handleRead(Timestamp time)
{
    {
        uint64_t temp = 0;
        int len = qinmo::detail::read(fd_, &temp, sizeof(temp));
        if (sizeof(temp) != len)
            QINMO_ERROR("TimerManager.handleRead: read bytes is less than 8 byte:", len, ".");
    }

    while (!timersOrder_.empty())
    {
        auto it = timersOrder_.begin();

        if (it->first > time)
            break;

        ;
    }

    // for (const auto& it : timersOrder_)
    // {
    //     timers_.erase(cancelTimer_);
    //     cancelTimer_ = 0;

    //     if (time < it.first)
    //         return;

    //     auto timer = timers_.find(it.second);
    //     if (timer == timers_.end())
    //     {
    //         QINMO_ERROR("Failed to find timer from timersOrder in timers. tfd=", fd_, ", sequence=", it.second);
    //         return;
    //     }

    //     currTimer_ = it.second;
    //     timer->second->run();
    //     currTimer_ = 0;
    // }

    // timers_.erase(cancelTimer_);
    // cancelTimer_ = 0;
}

void TimerManager::resetTimerfd(Timestamp when)
{
    int64_t t = std::max(int64_t(1), (when - Timestamp::now().getMicroseconds()).getMicroseconds());
    struct itimerspec newVal;
    detail::zeroMemory(&newVal, sizeof(newVal));
    struct timespec ts;
    ts.tv_sec = t / Timestamp::MicToSec;
    ts.tv_nsec = (t % Timestamp::MicToSec) * 1000;
    newVal.it_value = ts;
    if (0 > qinmo::detail::timerfd_settime(fd_, 0, &newVal, nullptr))
        QINMO_ERROR("TimerManager.resetTimerfd: Failed to timerfd_settime");
}

} // namespace net
} // namespace qinmo