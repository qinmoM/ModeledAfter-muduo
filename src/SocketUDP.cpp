#include "qinmo/net/SocketUDP.h"

namespace qinmo::net
{

SocketUDP SocketUDP::createRaw(const InetAddr& addr, int flags)
{
    if (!addr.isValid())
        return SocketUDP();

    return SocketUDP(detail::socket((addr.isIPv4() ? AF_INET : AF_INET6), SOCK_DGRAM | flags));
}

SocketUDP SocketUDP::createNonBlockOrDie(const InetAddr& addr)
{
    return createRaw(addr, SOCK_NONBLOCK | SOCK_CLOEXEC);
}

SocketUDP SocketUDP::attach(const int fd)
{
    if (SOCK_DGRAM != detail::getSocketType(fd))
        return SocketUDP();

    return SocketUDP(fd);
}


SocketUDP::SocketUDP() { }

SocketUDP::SocketUDP(int fd)
    : sockfd_(fd)
{ }

SocketUDP::~SocketUDP()
{
    close();
}


SocketUDP::SocketUDP(SocketUDP&& other) noexcept
{
    sockfd_ = other.sockfd_;
    other.sockfd_ = -1;
    state_ = other.state_;
    other.state_ = 0;
}

SocketUDP& SocketUDP::operator=(SocketUDP&& other) noexcept
{
    sockfd_ = other.sockfd_;
    other.sockfd_ = -1;
    state_ = other.state_;
    other.state_ = 0;
    return *this;
}


bool SocketUDP::isValid() const
{
    return -1 != sockfd_;
}

int SocketUDP::getfd() const
{
    return sockfd_;
}

bool SocketUDP::isBind() const
{
    return state_ & IsBind;
}

bool SocketUDP::isConnect() const
{
    return state_ & IsConnect;
}

InetAddr SocketUDP::getLocalAddr() const
{
    if (!isBind())
        return InetAddr();

    detail::sockaddr addr;
    detail::zeroMemory(&addr, sizeof(addr));
    if (!detail::getsockname(sockfd_, addr))
        detail::sockaddr_cast<detail::sockaddr, ::sockaddr>(&addr)->sa_family = AF_UNSPEC;

    return InetAddr(addr);
}

InetAddr SocketUDP::getPeerAddr() const
{
    if (!isConnect())
        return InetAddr();

    detail::sockaddr addr;
    detail::zeroMemory(&addr, sizeof(addr));
    if (!detail::getpeername(sockfd_, addr))
        detail::sockaddr_cast<detail::sockaddr, ::sockaddr>(&addr)->sa_family = AF_UNSPEC;

    return InetAddr(addr);
}


ssize_t SocketUDP::recvfrom(char* buf, size_t len, InetAddr& peer)
{
    detail::sockaddr addr = peer.getSockaddr();
    ssize_t len = detail::recvfrom(sockfd_, buf, len, addr);
    peer = InetAddr(addr);
    return len;
}

ssize_t SocketUDP::sendto(const char* buf, size_t len, const InetAddr& peer)
{
    return detail::sendto(sockfd_, buf, len, peer.getSockaddr());
}

bool SocketUDP::bind(const InetAddr& local)
{
    if (-1 == sockfd_ || isBind())
        return false;

    return detail::bind(sockfd_, local.getSockaddr());
}

bool SocketUDP::connect(const InetAddr& peer)
{
    if (-1 == sockfd_ || isConnect())
        return false;

    return detail::connect(sockfd_, peer.getSockaddr());
}

ssize_t SocketUDP::recv(char* buf, size_t len)
{
    if (!isConnect())
    {
        errno = ENOTCONN;
        return -1;
    }

    return detail::recv(sockfd_, buf, len);
}

ssize_t SocketUDP::send(const char* buf, size_t len)
{
    if (!isConnect())
    {
        errno = ENOTCONN;
        return -1;
    }

    return detail::send(sockfd_, buf, len);
}

bool SocketUDP::close()
{
    if (-1 == sockfd_ || !qinmo::detail::close(sockfd_))
        return false;

    sockfd_ = -1;
    return true;
}


bool SocketUDP::setReusePort(bool enable)
{
    return detail::setPortReuse(sockfd_, enable);
}

bool SocketUDP::setReuseAddr(bool enable)
{
    return detail::setAddrReuse(sockfd_, enable);
}

}