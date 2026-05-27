#include "qinmo/net/ReactorTcpConnect.h"
#include "qinmo/net/EventLoop.h"
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
    channel_.setReadEvent( [this](Timestamp time) -> void { handleRead(time); } );
    channel_.setWriteEvent( [this]() -> void { handleWrite(); } );
    channel_.setCloseEvent( [this]() -> void { handleClose(); } );
    channel_.setErrorEvent( [this]() -> void { handleError(); } );
    QINMO_INFO("new ReactorTcpConnect connect : fd=", sock_.getfd());

    if (!sock_.setKeepAlive(true))
        QINMO_WARN("ReactorTcpConnect:Create successful buf Failed to set keepAlive fd=", sock_.getfd());
}

ReactorTcpConnect::~ReactorTcpConnect()
{
    QINMO_TRACE("ReactorTcpConnect disconnect : fd=", sock_.getfd(), ", state=", getStateStr().data());
}


int ReactorTcpConnect::getfd() const
{
    return sock_.getfd();
}

bool ReactorTcpConnect::getIsConnect() const
{
    return state_ == RTcpConnState::Connected;
}

EventLoop* ReactorTcpConnect::getLoop() const
{
    return loop_;
}

InetAddr ReactorTcpConnect::getLocalAddr() const
{
    return localAddr_;
}

InetAddr ReactorTcpConnect::getPeerAddr() const
{
    return peerAddr_;
}


void ReactorTcpConnect::connectEstablished()
{
    QINMO_DEBUG("ReactorTcpConnect:enter fd=", sock_.getfd(), ", established.");
    state_ = RTcpConnState::Connected;
    channel_.tie(shared_from_this());
    channel_.enableRead();

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

TimerID ReactorTcpConnect::timerAt(Timestamp timestamp, TimerConnFunc func)
{
    return timerRepeatAt(timestamp, 0.0, std::move(func));
}

TimerID ReactorTcpConnect::timerAfter(double seconds, TimerConnFunc func)
{
    return timerRepeatAt(Timestamp::now() + static_cast<int64_t>(seconds * Timestamp::MicToSec), 0.0, std::move(func));
}

TimerID ReactorTcpConnect::timerRepeatAt(Timestamp timestamp, double intervalSeconds, TimerConnFunc func)
{
    if (!func)
    {
        QINMO_WARN("ReactorTcpConnect.timerRepeatAt: func cannot be nullptr. fd=", sock_.getfd());
        return TimerID();
    }

    std::weak_ptr<ReactorTcpConnect> weakPtr = shared_from_this();
    return loop_->timerRepeatAt(
        timestamp,
        intervalSeconds,
        [weakPtr, func]() -> void
        {
            RTcpConnPtr p = weakPtr.lock();
            if (p)
                func(p);
        }
    );
}

TimerID ReactorTcpConnect::timerRepeatAfter(double beginSeconds, double intervalSeconds, TimerConnFunc func)
{
    return timerRepeatAt(Timestamp::now() + static_cast<int64_t>(beginSeconds * Timestamp::MicToSec), intervalSeconds, std::move(func));
}

void ReactorTcpConnect::timerCancel(TimerID id)
{
    return loop_->timerCancel(id);
}

void ReactorTcpConnect::send(const std::string& str)
{
    if (RTcpConnState::Connected != state_)
        return;

    std::size_t oldSize = outputBuffer_.getReadableSize();
    std::size_t currIndex = 0;

    // try writing directly when nothing in buffer
    if (!channel_.isWrite() && 0 == oldSize)
    {
        size_t len = detail::send(sock_.getfd(), str.data(), str.size());
        if (len > 0)
        {
            if (len == str.size() && writeCompleteFunc_)
            {
                RTcpConnPtr self = shared_from_this();
                loop_->queueInLoop(
                    [self]() -> void
                    {
                        self->writeCompleteFunc_(self);
                    }
                );
                return;
            }
            currIndex = len;
        }
        else
        {
            if (errno != EWOULDBLOCK)
            {
                QINMO_ERROR("ReactorTcpConnect:Failed to send string directly. fd=", sock_.getfd());
                if (errno == EPIPE || errno == ECONNRESET)
                    return;
            }
        }
    }

    // determine whether call highWaterMark
    int newSize = oldSize + str.size() - currIndex;
    if (highWaterMarkFunc_ && waterMark_ > oldSize && waterMark_ <= newSize)
    {
        RTcpConnPtr p = shared_from_this();
        loop_->queueInLoop(
            [p, newSize]() -> void
            {
                p->highWaterMarkFunc_(p, newSize);
            }
        );
    }

    outputBuffer_.appendString(str.substr(currIndex, str.size() - currIndex));

    if (!channel_.isWrite())
        channel_.enableWrite();
}

void ReactorTcpConnect::send(PacketBuffer& buf)
{
    if (RTcpConnState::Connected != state_)
        return;

    std::string str;
    if (!buf.headGetString(0, buf.getHeadSize(), str))
    {
        // it definitely won't happen
        QINMO_ERROR("PacketBuffer.send(buf) error. fd=", sock_.getfd());
        return;
    }
    outputBuffer_.appendString(str);

    if (!buf.peekAll(str))
    {
        // it definitely won't happen
        QINMO_ERROR("PacketBuffer.send(buf) error. fd=", sock_.getfd());
        return;
    }
    outputBuffer_.appendString(str);

    if (!channel_.isWrite())
        channel_.enableWrite();
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
    if (RTcpConnState::Connected != state_ || RTcpConnState::Connecting != state_)
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

void ReactorTcpConnect::close()
{
    if (RTcpConnState::Connecting == state_ || RTcpConnState::Disconnected == state_)
        return;

    RTcpConnPtr self = shared_from_this();
    loop_->queueInLoop([self]() -> void { self->handleClose(); });
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
    int save = 0;
    ssize_t len = inputBuffer_.readFd(sock_.getfd(), save);

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

    RTcpConnPtr self(shared_from_this());
    if (disconnectFunc_) disconnectFunc_(self);
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