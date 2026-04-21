#pragma once

#include "../base/Timestamp.h"
#include "PollerConfig.h"
#include <functional>

class EventLoop;

namespace qinmo
{
/// @namespace qinmo::net
namespace net
{

class Channel
{
public:
    using EventCallback = std::function<void()>;
    using TimeEventCallback = std::function<void(qinmo::Timestamp)>;

    int fd() const;
    poller::EventsType events() const;
    void set_revents(poller::EventsType revt);

    void setReadEvent(TimeEventCallback ev);
    void setWriteEvent(EventCallback ev);
    void setCloseEvent(EventCallback ev);
    void setErrorEvent(EventCallback ev);

    void enableRead();
    void disableRead();
    void enableWrite();
    void disableWrite();
    void disableAll();

    bool isNone() const;
    bool isRead() const;
    bool isWrite() const;
    bool isAll() const;

private:
    EventLoop* loop_;

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