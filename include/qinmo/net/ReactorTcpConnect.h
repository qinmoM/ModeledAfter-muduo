#pragma once

#include "TcpConnect.h"
#include "EventLoopThreadPool.h"
#include "PacketBuffer.h"
#include <memory>

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

class ReactorTcpConnect;



using RTcpConnPtr = std::shared_ptr<ReactorTcpConnect>;

/// @brief new connection callback
using ConnectFunc = std::function<void(const RTcpConnPtr&)>;
/// @brief close callback
using CloseFunc = std::function<void(const RTcpConnPtr&)>;
/// @brief send complete callback
using WriteCompleteFunc = std::function<void(const RTcpConnPtr&)>;
/// @brief new message callback
using MessageFunc = std::function<void(const RTcpConnPtr&, PacketBuffer&, Timestamp)>;
/// @brief send buffer full callback
using HighWaterMarkFunc = std::function<void(const RTcpConnPtr&, std::size_t)>;



class ReactorTcpConnect
    // Inherit this template class  when  needing to create shared_ptr via 'this' pointer inside the class
    : public std::enable_shared_from_this<ReactorTcpConnect>
{
public:
    ReactorTcpConnect();
    ~ReactorTcpConnect();

    ReactorTcpConnect(const ReactorTcpConnect&) = delete;
    ReactorTcpConnect& operator=(const ReactorTcpConnect&) = delete;

    ReactorTcpConnect(ReactorTcpConnect&&) = delete;
    ReactorTcpConnect& operator=(ReactorTcpConnect&&) = delete;

public:
    ;

private:
    enum class RTcpConnState : char
    {
        Disconnected = 0,
        Connecting,
        Connected,
        Disconnecting
    };

private:
    EventLoop* loop_;
    std::atomic<RTcpConnPtr> state_;

    TcpConnect sock_;
    Channel channel_;
    InetAddr localAddr_;
    InetAddr peerAddr_;

    ConnectFunc connectFunc_;
    MessageFunc messageFunc_;
    WriteCompleteFunc writeCompleteFunc_;
    CloseFunc closeFunc_;
    HighWaterMarkFunc highWaterMarkFunc_;
    std::size_t waterMark_;

    PacketBuffer inputBuffer_;
    PacketBuffer outputBuffer_;

};

;
} // namespace net
} // namespace qinmo