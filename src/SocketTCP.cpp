#include "qinmo/net/SocketTCP.h"
#include <stdexcept>

namespace qinmo::net
{

SocketTCP SocketTCP::create(const InetAddr& addr)
{
    return SocketTCP(detail::socket((addr.isIPv4() ? AF_INET : AF_INET6), SOCK_STREAM));
}

SocketTCP SocketTCP::attach(const int fd)
{
    return SocketTCP(fd);
}

SocketTCP::SocketTCP() : sockfd_(-1) { }

SocketTCP::SocketTCP(int fd)
{
    sockfd_ = fd;
}

SocketTCP::~SocketTCP()
{
    close();
}

SocketTCP::SocketTCP(SocketTCP&& other) noexcept
{
    sockfd_ = other.sockfd_;
    other.sockfd_ = -1;
}

SocketTCP& SocketTCP::operator=(SocketTCP&& other) noexcept
{
    sockfd_ = other.sockfd_;
    other.sockfd_ = -1;
    return *this;
}

bool SocketTCP::valid() const
{
    return -1 != sockfd_;
}

int SocketTCP::getfd() const
{
    return sockfd_;
}

InetAddr SocketTCP::getLocalAddr() const
{
    detail::sockaddr addr;
    if (!detail::getsockname(sockfd_, addr))
        std::runtime_error("Failed to get address. | SocketTCP::getLocalAddr");

    return InetAddr(addr);
}

InetAddr SocketTCP::getPeerAddr() const
{
    detail::sockaddr addr;
    if (!detail::getpeername(sockfd_, addr))
        std::runtime_error("Failed to get address. | SocketTCP::getPeerAddr");

    return InetAddr(addr);
}

bool SocketTCP::bind(const InetAddr& addr)
{
    if (-1 == sockfd_ || 0 != detail::bind(sockfd_, addr.getSockaddr()))
        return false;

    return true;
}

bool SocketTCP::listen(int num)
{
    if (-1 == sockfd_ || detail::listen(num))
        return false;

    return true;
}

bool SocketTCP::close()
{
    if (-1 == sockfd_ || -1 == qinmo::detail::close(sockfd_))
        return false;

    sockfd_ = -1;
    return true;
}

}