#include "qinmo/net/InetAddr.h"

namespace qinmo::net
{

InetAddr::InetAddr(bool isIPv4)
{
    detail::zeroMemory(&addr_, sizeof(addr_));

    if (isIPv4)
        addr_.addr4_.sin_family = AF_INET;
    else
        addr_.addr6_.sin6_family = AF_INET6;
}

bool InetAddr::isIPv4() const
{
    return AF_INET == addr_.addr4_.sin_family;
}

bool InetAddr::isIPv6() const
{
    return AF_INET6 == addr_.addr6_.sin6_family;
}

uint16_t InetAddr::getPort() const
{
    if (isIPv4())
        return detail::netToHost16(addr_.addr4_.sin_port);
    else
        return detail::netToHost16(addr_.addr6_.sin6_port);
}

std::string InetAddr::getIP() const
{
    if (isIPv4())
        return detail::ntop4(addr_.addr4_.sin_addr);
    else
        return detail::ntop6(addr_.addr6_.sin6_addr);
}

const sockaddr& InetAddr::getSockaddr() const
{
    if (isIPv4())
        return *detail::sockaddr_cast<const sockaddr_in, const sockaddr>(&(addr_.addr4_));
    else
        return *detail::sockaddr_cast<const sockaddr_in6, const sockaddr>(&(addr_.addr6_));
}

bool InetAddr::setIP(const StringView& str)
{
    if (isIPv4())
        return 1 == detail::pton4(str, addr_.addr4_.sin_addr);
    else
        return 1 == detail::pton6(str, addr_.addr6_.sin6_addr);
}

void InetAddr::setPort(uint16_t port)
{
    if (isIPv4())
        addr_.addr4_.sin_port = detail::hostToNet16(port);
    else
        addr_.addr6_.sin6_port = detail::hostToNet16(port);
}

} // namespace qinmo::net