#include "EpollPoller.h"
#include "qinmo/net/detail/Wrapper.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{
/// @namespace qinmo::net::poll
namespace poll
{

EpollPoller::EpollPoller(EventLoop* eventLoop)
    : Poller(eventLoop)
    , epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , eventsbuf_(16)
{ }

EpollPoller::~EpollPoller()
{
    detail::close(epollfd_);
}

Timestamp EpollPoller::poll(int timeoutMs, ChannelList& list)
{
    int size = eventsbuf_.size();
    int len = ::epoll_wait(epollfd_, &*(eventsbuf_.begin()), size, timeoutMs);
    Timestamp time = Timestamp::now();

    if (len > 0)
    {
        for (int i = 0; i < len; ++i)
        {
            list.push_back(static_cast<Channel*>(eventsbuf_[i].data.ptr));
            list.back()->set_revents(eventsbuf_[i].events);
        }

        if (len == size)
            eventsbuf_.resize(size * 2);
    }

    return time;
}

void EpollPoller::setChannel(Channel& channel)
{
    int fd = channel.fd();

    if (channels_.find(fd) == channels_.end())
    {
        channels_[fd] = &channel;
        epollCTL(EPOLL_CTL_ADD, channel);
    }
    else
    {
        epollCTL(EPOLL_CTL_MOD, channel);
    }
}

void EpollPoller::removeChannel(Channel& channel)
{
    epollCTL(EPOLL_CTL_DEL, channel);
    channels_.erase(channel.fd());
}

void EpollPoller::epollCTL(int op, Channel& channel)
{
    struct epoll_event event;
    detail::zeroMemory(&event, sizeof(event));
    event.data.ptr = &channel;
    event.events = channel.events();

    ::epoll_ctl(epollfd_, op, channel.fd(), &event);
}

}
}
}