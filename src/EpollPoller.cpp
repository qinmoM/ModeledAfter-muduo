#include "qinmo/net/EpollPoller.h"
#include "qinmo/net/detail/Wrapper.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{
/// @namespace qinmo::net::poller
namespace poller
{

EpollPoller::EpollPoller(EventLoop* eventLoop)
    : Poller(eventLoop)
    , epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , eventsbuf_(16)
{
    QINMO_DEBUG("EpollPoller.epoll create fd=", epollfd_);
}

EpollPoller::~EpollPoller()
{
    qinmo::detail::close(epollfd_);
}

Timestamp EpollPoller::poll(int timeoutMs, ChannelList& list)
{
    int size = eventsbuf_.size();
    int len = ::epoll_wait(epollfd_, &*(eventsbuf_.begin()), size, timeoutMs);
    Timestamp time = Timestamp::now();
    sleep(4);

    if (len > 0)
    {
        for (int i = 0; i < len; ++i)
        {
            list.push_back(static_cast<Channel*>(eventsbuf_[i].data.ptr));
            list.back()->setRevents(eventsbuf_[i].events);
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
        QINMO_DEBUG("setChannel add:", channel.events(), ", fd=", channel.fd());
        epollCTL(EPOLL_CTL_ADD, channel);
    }
    else
    {
        QINMO_DEBUG("setChannel ctl:", channel.events(), ", fd=", channel.fd());
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

} // namespace poller
} // namespace net
} // namespace qinmo