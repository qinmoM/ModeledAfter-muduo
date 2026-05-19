#include "qinmo/net/ReactorTcpConnect.h"
#include "qinmo/base/Logger.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

ReactorTcpConnect::ReactorTcpConnect(EventLoop* loop, TcpConnect sock, const InetAddr& localAddr, const InetAddr& peerAddr)
    : loop_(loop)
    , channel_(loop, sock.getfd())
    , sock_(std::move(sock))
    , localAddr_(localAddr)
    , peerAddr_(peerAddr)
    , state_(RTcpConnState::Connecting)
    , waterMark_(64 * 1024 * 1024)
    , inputBuffer_(0)
    , outputBuffer_(0)
{
    channel_.setReadEvent( [this](Timestamp time) -> void { handleRead(time); } );
    channel_.setWriteEvent( [this]() -> void { handleWrite(); } );
    channel_.setCloseEvent( [this]() -> void { handleClose(); } );
    channel_.setErrorEvent( [this]() -> void { handleError(); } );
    QINMO_INFO("new ReactorTcpConnect connect : fd=", sock_.getfd());

    sock_.setKeepAlive(true);
}

ReactorTcpConnect::~ReactorTcpConnect()
{
    QINMO_INFO("ReactorTcpConnect disconnect : fd=", sock_.getfd(), ", state=", getStateStr().data());
}

void ReactorTcpConnect::setConnectFunc(const ConnectFunc &f)
{
    connectFunc_ = f;
}

void ReactorTcpConnect::setDisconnectFunc(const DisconnectFunc &f)
{
    disconnectFunc_ = f;
}

void ReactorTcpConnect::setWriteCompleteFunc(const WriteCompleteFunc &f)
{
    writeCompleteFunc_ = f;
}

void ReactorTcpConnect::setMessageFunc(const MessageFunc &f)
{
    messageFunc_ = f;
}

void ReactorTcpConnect::setHighWaterMarkFunc(const HighWaterMarkFunc &f)
{
    highWaterMarkFunc_ = f;
}

void ReactorTcpConnect::setCloseFunc(const CloseFunc &f)
{
    closeFunc_ = f;
}

qinmo::StringView ReactorTcpConnect::getStateStr() const
{
    switch (state_)
    {
    case RTcpConnState::Connected:
        return "Connected";
    case RTcpConnState::Connecting:
        return "Connecting";
    case RTcpConnState::Disconnecting:
        return "Disconnecting";
    case RTcpConnState::Disconnected:
        return "Disconnected";
    default:
        return "**ERROR**type";
    }
}

void ReactorTcpConnect::handleRead(Timestamp time)
{
    int save = 0;
    ssize_t len = inputBuffer_.readFd(sock_.getfd(), save);
    if (0 > len)
        handleError();
    else if (0 == len)
        handleClose();
    else
        messageFunc_(shared_from_this(), inputBuffer_, time);
}

void ReactorTcpConnect::handleWrite()
{
    // maybe close in handleRead
    if (!channel_.isWrite())
    {
        QINMO_WARN("ReactorTcpConnect fd=", sock_.getfd(), " is down, no more writing.");
        return;
    }

    int save = 0;
    ssize_t len = outputBuffer_.writeFd(sock_.getfd(), save);
    if (save != 0)
    {
        if (EAGAIN != save && EWOULDBLOCK != save)
        {
            handleError();
            handleClose();
        }

        return;
    }

    if (0 != outputBuffer_.getReadableSize())
        return;

    channel_.disableWrite();
    if (writeCompleteFunc_)
        loop_->queueInLoop([self = shared_from_this()]() -> void { self->writeCompleteFunc_(self); });

    if (RTcpConnState::Disconnecting == state_)
        shutdownInLoop();
}

void ReactorTcpConnect::handleClose()
{
    if (RTcpConnState::Disconnected == state_)
        return;

    QINMO_INFO("ReactorTcpConnect close. fd=", sock_.getfd(), ", state=", getStateStr());
    state_ = RTcpConnState::Disconnected;
    channel_.disableAll();

    RTcpConnPtr connPtr(shared_from_this());
    if (disconnectFunc_) disconnectFunc_(connPtr);
    closeFunc_(connPtr);
}

void ReactorTcpConnect::handleError()
{
    int opt = 0;
    socklen_t optLen = sizeof(opt);
    if (detail::getSockOpt(sock_.getfd(), SOL_SOCKET, SO_ERROR, &opt, optLen) < 0)
        QINMO_ERROR("fd:", sock_.getfd(), " handleError, and getsockopt() error. errno code:", errno);
    else
        QINMO_ERROR("fd:", sock_.getfd(), " handleError. errno code:", opt);
}

} // namespace net
} // namespace qinmo