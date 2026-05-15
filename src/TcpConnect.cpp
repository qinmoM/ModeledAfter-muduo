#include "qinmo/net/TcpConnect.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

TcpConnect TcpConnect::connectRaw(const InetAddr &serverAddr, int flags)
{
    SocketTCP sock = SocketTCP::createRaw(serverAddr, flags);
    if (!sock.isValid() || !sock.connect(serverAddr))
        return TcpConnect();

    return TcpConnect(std::move(sock));
}

TcpConnect TcpConnect::connectNonBlockOrDie(const InetAddr &serverAddr)
{
    return TcpConnect::connectRaw(serverAddr, SOCK_NONBLOCK | SOCK_CLOEXEC);
}

TcpConnect::TcpConnect() : sockfd_() { }

TcpConnect::~TcpConnect() { }


bool TcpConnect::isValid() const
{
    return sockfd_.isValid();
}

int TcpConnect::getfd() const
{
    return sockfd_.getfd();
}

InetAddr TcpConnect::getLocalAddr() const
{
    return sockfd_.getLocalAddr();
}

InetAddr TcpConnect::getPeerAddr() const
{
    return sockfd_.getPeerAddr();
}


ssize_t TcpConnect::recv(char *buf, size_t len)
{
    return sockfd_.recv(buf, len);
}

ssize_t TcpConnect::send(const char *buf, size_t len)
{
    return sockfd_.send(buf, len);
}

bool TcpConnect::close()
{
    return sockfd_.close();
}


bool TcpConnect::setTcpNoDelay(bool enable)
{
    return sockfd_.setTcpNoDelay(enable);
}

bool TcpConnect::setKeepAlive(bool enable)
{
    return sockfd_.setKeepAlive(enable);
}

TcpConnect::TcpConnect(SocketTCP&& sock) : sockfd_(std::move(sock)) { }

} // namespace net
} // namespace qinmo