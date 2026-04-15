#pragma once

#include <stdint.h>

namespace qinmo
{
/// @namespace qinmo::poller
namespace poller
{


/**
 * Macro USE_EPOLL means use epoll in backend
 * Macro USE_POLL means use poll in backend
 */
#define USE_EPOLL
// #define USE_POLL



#if defined(USE_EPOLL) && defined(USE_POLL)
    #error "No poller defined"
#endif



#ifdef USE_EPOLL

    #include <sys/epoll.h>
    using EventsType = uint32_t;

    static constexpr EventsType NoneEvent = 0;
    static constexpr EventsType ReadEvent = EPOLLIN | EPOLLPRI;
    static constexpr EventsType WriteEvent = EPOLLOUT;

#elif defined(USE_POLL)

    #include <sys/poll.h>
    using EventsType = short int;

    static constexpr EventsType NoneEvent = 0;
    static constexpr EventsType ReadEvent = POLLIN | POLLPRI;
    static constexpr EventsType WriteEvent = POLLOUT;

#else

    #error "No poller defined"

#endif

} // namespace poller
} // namespace qinmo