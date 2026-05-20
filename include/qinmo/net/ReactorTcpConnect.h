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

/// @brief connecting callback
using ConnectFunc = std::function<void(const RTcpConnPtr&)>;
/// @brief disconnection callback
using DisconnectFunc = std::function<void(const RTcpConnPtr&)>;
/// @brief send complete callback
using WriteCompleteFunc = std::function<void(const RTcpConnPtr&)>;
/// @brief new message callback
using MessageFunc = std::function<void(const RTcpConnPtr&, PacketBuffer&, Timestamp)>;
/// @brief send buffer full callback
using HighWaterMarkFunc = std::function<void(const RTcpConnPtr&, std::size_t)>;

/// @brief close callback
/// @warning for intarnal use only, do NOT use it from outside the library
using CloseFunc = std::function<void()>;



class ReactorTcpConnect
    // Inherit this template class  when  needing to create shared_ptr via 'this' pointer inside the class
    : public std::enable_shared_from_this<ReactorTcpConnect>
{
public:
    ReactorTcpConnect(EventLoop* loop, TcpConnect&& sock, const InetAddr& localAddr, const InetAddr& peerAddr);
    ~ReactorTcpConnect();

    ReactorTcpConnect(const ReactorTcpConnect&) = delete;
    ReactorTcpConnect& operator=(const ReactorTcpConnect&) = delete;

    ReactorTcpConnect(ReactorTcpConnect&&) = delete;
    ReactorTcpConnect& operator=(ReactorTcpConnect&&) = delete;

public:
    int getfd() const;
    EventLoop* getLoop() const;

    void connectEstablished();
    void connectDestroyed();

    void send(const std::string& str);
    void send(PacketBuffer& buf);

    void setTcpNoDelay(bool enable);
    /// @note do nothing when in write event
    void shutdown();

    void setConnectFunc(const ConnectFunc& f);
    void setDisconnectFunc(const DisconnectFunc& f);
    void setWriteCompleteFunc(const WriteCompleteFunc& f);
    void setMessageFunc(const MessageFunc& f);
    void setHighWaterMarkFunc(std::size_t waterMark, const HighWaterMarkFunc& f);
    /// @warning internal use only
    void setCloseFunc(const CloseFunc& f);

private:
    enum class RTcpConnState : char
    {
        Disconnected = 0,
        Connecting,
        Connected,
        Disconnecting
    };
    qinmo::StringView getStateStr() const;

    void handleRead(Timestamp time);
    void handleWrite();
    void handleClose();
    void handleError();

private:
    EventLoop* loop_;
    RTcpConnState state_;

    TcpConnect sock_;
    Channel channel_;
    InetAddr localAddr_;
    InetAddr peerAddr_;

    ConnectFunc connectFunc_;
    DisconnectFunc disconnectFunc_;
    MessageFunc messageFunc_;
    WriteCompleteFunc writeCompleteFunc_;
    HighWaterMarkFunc highWaterMarkFunc_;
    CloseFunc closeFunc_;
    std::size_t waterMark_;

    PacketBuffer inputBuffer_;
    PacketBuffer outputBuffer_;

};

;
} // namespace net
} // namespace qinmo