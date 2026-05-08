#pragma once

#include "../base/Timestamp.h"
#include "PollerConfig.h"
#include "../base/Logger.h"
#include <functional>
#include <atomic>
#include <memory>
#include <vector>

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

class EventLoop;

class Channel;
using ChannelList = std::vector<Channel*>;

/**
 * @brief file descriptor and event encapsulation class
 * @note only used to observing file descriptor, do not auto release
 */
class Channel
{
public:
    /// @brief event callback
    using EventCallback = std::function<void()>;
    /// @brief event callback with timestamp
    using TimeEventCallback = std::function<void(qinmo::Timestamp)>;

public:
    Channel(EventLoop* eventLoop, int fd);

    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;

    Channel(Channel&&) = delete;
    Channel& operator=(Channel&&) = delete;

public:
    /// @brief get information of channel
    int fd() const;
    poller::EventsType events() const;
    bool isInLoop() const;
    EventLoop* eventLoop() const;

    /// @brief only used for returned events in Poller
    void set_revents(poller::EventsType revt);
    /// @brief remove current file descriptor events in EventLoop
    void remove();

    /// @brief set trigger event
    /// @param ev lambda, function object or callback
    void setReadEvent(TimeEventCallback ev);
    void setWriteEvent(EventCallback ev);
    void setCloseEvent(EventCallback ev);
    void setErrorEvent(EventCallback ev);

    /// @brief enable and disable event
    void enableRead();
    void disableRead();
    void enableWrite();
    void disableWrite();
    void disableAll();

    /// @brief determine what events in current channel
    /// @return true if event exists
    bool isNone() const;
    bool isRead() const;
    bool isWrite() const;
    bool isAll() const;

    /// @brief trigger all revents
    void handle(Timestamp timestamp);

private:
    /// @brief automatic update listening events in the EventLoop
    void updateInEventLoop();

private:
    EventLoop* loop_;
    std::atomic<bool> isInLoop_;

    // std::weak_ptr<void> tie_;

    int fd_;
    poller::EventsType events_;
    poller::EventsType revents_;

    TimeEventCallback readEvent_;
    EventCallback writeEvent_;
    EventCallback closeEvent_;
    EventCallback errorEvent_;

};

} // namespace net
} // namespace qinmo