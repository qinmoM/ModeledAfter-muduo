#pragma once

#include "detail/Wrapper.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>

namespace qinmo
{
namespace net
{

class InetAddr
{
public:
    explicit InetAddr(bool isIPv4 = true);

public:
    bool isIPv4() const;
    bool isIPv6() const;
    uint16_t getPort() const;
    const sockaddr* getSockaddr() const;

    bool setIP(const StringView& str);
    void setPort(uint16_t port);

private:
    union SocketAddrType
    {
        struct sockaddr_in addr4_;
        struct sockaddr_in6 addr6_;
    };

private:
    SocketAddrType addr_;

};

} // namespace net
} // namespace qinmo