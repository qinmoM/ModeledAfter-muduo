#include "qinmo/net/Channel.h"
#include "qinmo/net/EventLoop.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

Channel::Channel(EventLoop* eventLoop, int fd)
    : loop_(eventLoop)
    , fd_(fd)
    , isInLoop_(false)
{ }

int Channel::fd() const
{
    return fd_;
}

poller::EventsType Channel::events() const
{
    return events_;
}

void Channel::set_revents(poller::EventsType revt)
{
    revents_ = revt;
}

EventLoop* Channel::eventLoop() const
{
    return loop_;
}

bool Channel::isInLoop() const
{
    return isInLoop_.load();
}

void Channel::remove()
{
    loop_->removeChannel(*this);
}

void Channel::setReadEvent(TimeEventCallback ev)
{
    readEvent_ = std::move(ev);
}

void Channel::setWriteEvent(EventCallback ev)
{
    writeEvent_ = std::move(ev);
}

void Channel::setCloseEvent(EventCallback ev)
{
    closeEvent_ = std::move(ev);
}

void Channel::setErrorEvent(EventCallback ev)
{
    errorEvent_ = std::move(ev);
}

void Channel::enableRead()
{
    events_ |= poller::ReadEvent;
    updateInEventLoop();
}

void Channel::disableRead()
{
    events_ &= ~poller::ReadEvent;
    updateInEventLoop();
}

void Channel::enableWrite()
{
    events_ |= poller::WriteEvent;
    updateInEventLoop();
}

void Channel::disableWrite()
{
    events_ &= ~poller::WriteEvent;
    updateInEventLoop();
}

void Channel::disableAll()
{
    events_ = poller::NoneEvent;
    updateInEventLoop();
}

bool Channel::isNone() const
{
    return !events_;
}

bool Channel::isRead() const
{
    return events_ & poller::ReadEvent;
}

bool Channel::isWrite() const
{
    return events_ & poller::WriteEvent;
}

bool Channel::isAll() const
{
    return isRead() && isWrite();
}

void Channel::handle(Timestamp timestamp)
{
    isInLoop_.store(true);

    if (closeEvent_ && (  (revents_ & (EPOLLHUP))  &&  !(revents_ & (EPOLLIN))  ) )
        closeEvent_();

    if (errorEvent_ && (  revents_ & (EPOLLERR)  ) )
        errorEvent_();

    if (readEvent_ && (  revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)  ) )
        readEvent_(timestamp);

    if (writeEvent_ && (  revents_ & (EPOLLOUT)  ) )
        writeEvent_();

    isInLoop_.store(false);
}

void Channel::updateInEventLoop()
{
    loop_->setChannel(*this);
}

} // namespace net
} // namespace qinmo