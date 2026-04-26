#include "qinmo/net/EpollPoller.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

std::unique_ptr<Poller> Poller::create(EventLoop* eventLoop)
{
#ifdef USE_EPOLL
    return std::unique_ptr<Poller>(new poller::EpollPoller(eventLoop));

#elif defined(USE_POLL)
    return ;

#endif
}

} // namespace poller
} // namespace qinmo