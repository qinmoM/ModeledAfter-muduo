#pragma once

#include "Poller.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{
/// @namespace qinmo::net::poller
namespace poller
{

class EpollPoller : public Poller
{
public:
    EpollPoller(EventLoop* eventLoop);
    ~EpollPoller();

    EpollPoller(const EpollPoller&) = delete;
    EpollPoller& operator()(const EpollPoller&) = delete;

public:
    Timestamp poll(int timeoutMs, ChannelList& list) override;

    void setChannel(Channel& channel) override;
    void removeChannel(Channel& channel) override;

private:
    void epollCTL(int op, Channel& channel);

private:
    int epollfd_;
    std::vector<epoll_event> eventsbuf_;

};
} // namespace poller
} // namespace net
} // namespace qinmo