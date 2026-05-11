#include "qinmo/net/SocketTCP.h"
#include <stdexcept>

namespace qinmo::net
{

SocketTCP SocketTCP::create(const InetAddr& addr, int flags)
{
    return SocketTCP(detail::socket((addr.isIPv4() ? AF_INET : AF_INET6), SOCK_STREAM | flags));
}

SocketTCP SocketTCP::attach(const int fd)
{
    SocketTCP sockfd(fd);

    if (SOCK_STREAM != detail::getSocketType(sockfd.getfd()))
        return SocketTCP(-1);

    return sockfd;
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

bool SocketTCP::isValid() const
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
        detail::sockaddr_cast<detail::sockaddr, sockaddr>(&addr)->sa_family = AF_UNSPEC;

    return InetAddr(addr);
}

InetAddr SocketTCP::getPeerAddr() const
{
    detail::sockaddr addr;
    if (!detail::getpeername(sockfd_, addr))
        detail::sockaddr_cast<detail::sockaddr, sockaddr>(&addr)->sa_family = AF_UNSPEC;

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

SocketTCP SocketTCP::accept(InetAddr& addr, int flags)
{
    detail::sockaddr temp;
    int sockfd = detail::accept(sockfd_, temp, flags);
    addr = InetAddr(temp);
    return SocketTCP::attach(sockfd);
}

bool SocketTCP::close()
{
    if (-1 == sockfd_ || -1 == qinmo::detail::close(sockfd_))
        return false;

    sockfd_ = -1;
    return true;
}

}