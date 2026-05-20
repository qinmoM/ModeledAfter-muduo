#include "qinmo/net/ReactorTcpServer.h"
#include "qinmo/base/Logger.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

ReactorTcpServer::ReactorTcpServer(EventLoop* loop, const InetAddr& listenAddr, unsigned int numThread, bool reusePort)
    : loop_(loop)
    , threadPool_(new EventLoopThreadPool(loop, numThread))
    , sock_(TcpListen::createNonBlockOrDie(listenAddr))
    , acceptChannel_(loop, sock_.getfd())
    , started_(false)
    , numConn_(0)
{
    if (!sock_.isValid())
    {
        QINMO_FATAL("Failed to create socket with nonblockOrDie.");
        std::terminate();
    }
    QINMO_DEBUG("TcpServer:listen socket create fd=", sock_.getfd());

    if (!sock_.setReuseAddr(true))
        QINMO_ERROR("Failed to set reuse address.");
    if (!sock_.setReusePort(reusePort))
        QINMO_WARN("Failed to set reuse port.");

    if (!sock_.bind(listenAddr))
    {
        QINMO_FATAL("Failed to bind.");
        std::terminate();
    }

    QINMO_DEBUG("TcpServer:set listen read event fd=", sock_.getfd());
    acceptChannel_.setReadEvent( [this](Timestamp stamp) -> void { newConnect(stamp); } );
}

ReactorTcpServer::~ReactorTcpServer()
{
    QINMO_INFO("ReactorTcpServer quit.");
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}


void ReactorTcpServer::setConnectFunc(const ConnectFunc& f)
{
    connect_ = f;
}

void ReactorTcpServer::setDisconnectFunc(const DisconnectFunc& f)
{
    disconnect_ = f;
}

void ReactorTcpServer::setMessageFunc(const MessageFunc& f)
{
    message_ = f;
}

void ReactorTcpServer::setWriteCompleteFunc(const WriteCompleteFunc& f)
{
    writeComplete_ = f;
}


void ReactorTcpServer::start(EventLoopThread::EventLoopThreadInitFunc func)
{
    if (started_.load()) return;

    threadPool_->start(func);
    loop_->runInLoop(
        [this]()
        {
            QINMO_DEBUG("start listen.");
            if (sock_.isListen())
            {
                QINMO_WARN("Already in listen state.");
                return;
            }

            if (!sock_.listen())
            {
                QINMO_FATAL("Failed to listen.");
                std::terminate();
            }
            acceptChannel_.enableRead();
        }
    );
}

void ReactorTcpServer::newConnect(Timestamp time)
{
    InetAddr peerAddr;
    TcpConnect cli = sock_.acceptNonBlockOrDie(peerAddr);
    if (!cli.isValid())
    {
        QINMO_ERROR("Failed to accept - client invalid. cfd=", cli.getfd());
        return;
    }
    QINMO_INFO("New client: ", numConn_.load(), ", fd: ", cli.getfd(), ", ip: ", peerAddr.getIP(), ", port: ", peerAddr.getPort(), ".");

    EventLoop* subLoop = threadPool_->getNextLoop();
    InetAddr localAddr = sock_.getLocalAddr();
    if (!localAddr.isValid())
        QINMO_ERROR("Falied to get TcpServer address. lfd=", sock_.getfd(), ", cfd=", cli.getfd());

    RTcpConnPtr conn =
        std::make_shared<ReactorTcpConnect>(
            subLoop,
            std::move(cli),
            localAddr,
            peerAddr
        );
    rConnects_[cli.getfd()] = conn;
    conn->setConnectFunc(connect_);
    conn->setDisconnectFunc(disconnect_);
    conn->setMessageFunc(message_);
    conn->setWriteCompleteFunc(writeComplete_);
    conn->setCloseFunc( [this, conn]() -> void { removeConnect(conn); } );

    subLoop->runInLoop( [this, conn]() -> void { conn->connectEstablished(); } );
}

void ReactorTcpServer::removeConnect(const RTcpConnPtr& conn)
{
    QINMO_INFO("TcpServer remove connection. cfd=", conn->getfd());
    if (0 == rConnects_.erase(conn->getfd()))
        QINMO_ERROR("TcpServer not found fd=", conn->getfd(), " , give up remove.");
    else
        conn->getLoop()->queueInLoop( [conn]() -> void { conn->connectDestroyed(); } );
}

} // namespace net
} // namespace qinmo