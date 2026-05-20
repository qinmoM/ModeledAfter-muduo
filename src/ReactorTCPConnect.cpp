#include "qinmo/net/ReactorTcpConnect.h"
#include "qinmo/base/Logger.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

ReactorTcpConnect::ReactorTcpConnect(EventLoop* loop, TcpConnect&& sock, const InetAddr& localAddr, const InetAddr& peerAddr)
    : loop_(loop)
    , sock_(std::move(sock))
    , channel_(loop, sock_.getfd())
    , localAddr_(localAddr)
    , peerAddr_(peerAddr)
    , state_(RTcpConnState::Connecting)
    , waterMark_(64 * 1024)
    , inputBuffer_(0)
    , outputBuffer_(0)
{
    QINMO_DEBUG("receive TcpConnect fd=", sock_.getfd(), ", now paramfd=", sock.getfd());
    channel_.setReadEvent( [this](Timestamp time) -> void { handleRead(time); } );
    channel_.setWriteEvent( [this]() -> void { handleWrite(); } );
    channel_.setCloseEvent( [this]() -> void { handleClose(); } );
    channel_.setErrorEvent( [this]() -> void { handleError(); } );
    QINMO_INFO("new ReactorTcpConnect connect : fd=", sock_.getfd());

    if (sock_.setKeepAlive(true))
        QINMO_INFO("set keepAlive fd=", sock_.getfd());
    else
        QINMO_DEBUG("Failed to set keepAlive fd=", sock_.getfd());
}

ReactorTcpConnect::~ReactorTcpConnect()
{
    QINMO_INFO("ReactorTcpConnect disconnect : fd=", sock_.getfd(), ", state=", getStateStr().data());
}


int ReactorTcpConnect::getfd() const
{
    return sock_.getfd();
}

EventLoop* ReactorTcpConnect::getLoop() const
{
    return loop_;
}


void ReactorTcpConnect::connectEstablished()
{
    QINMO_TRACE("ReactorTcpConnect:enter fd=", sock_.getfd(), ", established.");
    state_ = RTcpConnState::Connected;
    channel_.tie(shared_from_this());
    channel_.enableRead();

    QINMO_DEBUG("established execute connectFunc() ", (connectFunc_ ? "exists" : "not exists"));
    if (connectFunc_)
        connectFunc_(shared_from_this());
}

void ReactorTcpConnect::connectDestroyed()
{
    QINMO_DEBUG("ReactorTcpConnect:enter destroyed fd=", sock_.getfd(), ", state=", getStateStr().data());
    if (RTcpConnState::Connected == state_)
    {
        state_ = RTcpConnState::Disconnected;
        channel_.disableAll();

        if (disconnectFunc_)
            disconnectFunc_(shared_from_this());
    }

    channel_.remove();
}


void ReactorTcpConnect::setTcpNoDelay(bool enable)
{
    RTcpConnPtr self = shared_from_this();
    loop_->runInLoop(
        [self, enable]() -> void
        {
            self->sock_.setTcpNoDelay(enable);
        }
    );
}

void ReactorTcpConnect::shutdown()
{
    if (RTcpConnState::Connected != state_)
        return;

    state_ = RTcpConnState::Disconnecting;
    RTcpConnPtr self = shared_from_this();
    loop_->runInLoop(
        [self]() -> void
        {
            if (!self->channel_.isWrite())
                self->sock_.shutdownWrite();
        }
    );
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

void ReactorTcpConnect::setHighWaterMarkFunc(std::size_t waterMark, const HighWaterMarkFunc &f)
{
    waterMark_ = waterMark;
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
    QINMO_TRACE("ReactorTcpConnect:enter handleRead. fd=", sock_.getfd(), ", waterMark:", waterMark_);
    int save = 0;
    ssize_t len = inputBuffer_.readFd(sock_.getfd(), save);
    QINMO_DEBUG("read size :", len, ", fd=", sock_.getfd());
    if (0 > len)
        handleError();
    else if (0 == len)
        handleClose();
    else
    {
        if (messageFunc_)
            messageFunc_(shared_from_this(), inputBuffer_, time);
    }
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
    {
        RTcpConnPtr self = shared_from_this();
        loop_->queueInLoop([self]() -> void { self->writeCompleteFunc_(self); });
    }

    if (RTcpConnState::Disconnecting == state_)
        shutdown();
}

void ReactorTcpConnect::handleClose()
{
    if (RTcpConnState::Disconnected == state_)
        return;

    QINMO_INFO("ReactorTcpConnect close. fd=", sock_.getfd(), ", state=", getStateStr().data());
    state_ = RTcpConnState::Disconnected;
    channel_.disableAll();

    RTcpConnPtr connPtr(shared_from_this());
    if (disconnectFunc_) disconnectFunc_(connPtr);
    closeFunc_();
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