#pragma once

#include "Channel.h"

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

// use only for declaring pointer types
class EventLoop;


/**
 * @brief IO multiplexing class
 * @note 
 */
class Poller
{
public:
    /// @brief get instance of Poller subclass (automatically determined at compile-time)
    /// @return unique pointer of Poller subclass
    static std::unique_ptr<Poller> create(EventLoop* eventLoop);

public:
    /// @brief do NOT call the constructor directly, use the supplied 'create' instead
    Poller(EventLoop* eventLoop) : loop_(eventLoop) { }

    virtual ~Poller() = default;

public:
    /// @brief event loop
    /// @param timeoutMs time-out (milliseconds)
    /// @param list return the triggered event (append it to end of list when list is non-empty)
    /// @return current timestamp
    virtual Timestamp poll(int timeoutMs, ChannelList& list) = 0;

    /// @brief return true if the file descriptor already exists
    bool findChannel(Channel& channel) const { return channels_.find(channel.fd()) != channels_.end(); }
    /// @brief update an event
    virtual void setChannel(Channel& channel) = 0;
    /// @brief delete an event
    virtual void removeChannel(Channel& channel) = 0;

protected:
    EventLoop* loop_;
    std::unordered_map<int, Channel*> channels_;

};

}
}