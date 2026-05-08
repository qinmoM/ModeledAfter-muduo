/**
 * @brief network address class
 */

#pragma once

#include "detail/Wrapper.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

/**
 * @brief Warpper of sockaddr_in and sockaddr_in6
 */
class InetAddr
{
public:
    explicit InetAddr(bool isIPv4 = true);
    explicit InetAddr(const detail::sockaddr& addr);

public:
    /// @brief get family
    bool isIPv4() const;
    bool isIPv6() const;
    /// @brief get port
    /// @return network order
    uint16_t getPort() const;
    /// @brief get IP address
    std::string getIP() const;
    /// @brief get sockaddr
    const sockaddr& getSockaddr() const;

    /// @brief set the IP address
    /// @param str dotted decimal notation
    /// @return return true when successful, otherwise false
    bool setIP(const StringView& str);
    /// @brief set the port, always succeed
    /// @param port host order, 32 bit
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