#pragma once

#include "Poller.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{
/// @namespace qinmo::net::poll
namespace poll
{

class EpollPoller : public Poller
{
public:
    EpollPoller(EventLoop* eventLoop);

public:
    Timestamp poll(int timeoutMs, ChannelList& list) override;

    void setChannel(Channel& channel) override;
    void removeChannel(Channel& channel) override;

};
}
}
}