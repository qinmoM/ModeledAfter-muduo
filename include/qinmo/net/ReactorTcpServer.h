#pragma once

#include "TcpListen.h"
#include "ReactorTcpConnect.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

class ReactorTcpServer
{
public:
    ReactorTcpServer(EventLoop* loop, const InetAddr& listenAddr, unsigned int numThread = 0, bool reusePort = false);
    ~ReactorTcpServer();

    ReactorTcpServer(const ReactorTcpServer&) = delete;
    ReactorTcpServer& operator=(const ReactorTcpServer&) = delete;

    ReactorTcpServer(ReactorTcpServer&&) = delete;
    ReactorTcpServer& operator=(ReactorTcpServer&&) = delete;

public:
    /// @brief start listen
    /// @param func the thread initilization function
    /// @note must start loop.loop()
    void start(EventLoopThread::EventLoopThreadInitFunc func = [](EventLoop *) -> void { });

private:
    /// @brief create a new connect, and register to loop
    void newConnect();
    /// @brief remove connect in loop and registry
    void removeConnect();

private:
    EventLoop* loop_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;
    std::unordered_map<int, RTcpConnPtr> rConnects_;

    Channel acceptChannel_;

    TcpListen sock_;
    std::atomic<bool> started_;
    std::atomic<int> numConn_;

};
} // namespace net
} // namespace qinmo