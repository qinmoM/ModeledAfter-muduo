#pragma once

#include "InetAddr.h"
#include <functional>

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

/// @brief encapsulate TCP socket
/// @note
///   default constructor is empty.
///   you must specify whether to Create a new instance or Attach to an existing one.
///
///     - SocketTCP sock1 = SocketTCP::create(InetAddr())
///
///     - SocketTCP sock2(SocketTCP::attach(1))
class SocketTCP
{
public:
    /// @brief create a new socket
    /// @param addr only access protocol family
    /// @return using the move constructor
    /// @note better to check whether the returned value is valid : Call function isValid()
    static SocketTCP create(const InetAddr& addr, int flags = 0);
    /// @brief attach an existing socket
    /// @param fd file descriptor
    /// @return using the move constructor
    /// @note must check whether the returned value is valid : Call function isValid()
    static SocketTCP attach(const int fd);

public:
    SocketTCP();
    ~SocketTCP();

    SocketTCP(SocketTCP&& other) noexcept;
    SocketTCP& operator=(SocketTCP&& other) noexcept;

    SocketTCP(const SocketTCP&) = delete;
    SocketTCP& operator=(const SocketTCP&) = delete;

public:
    /// @return return true it has been initialized
    bool isValid() const;
    /// @brief get current file descriptor
    int getfd() const;
    /// @brief get local address
    /// @note need to Check if the returned InetAddr is valid : Call functions isIPv4() and isIPv6()
    InetAddr getLocalAddr() const;
    /// @brief get peer address
    /// @note need to Check if the returned InetAddr is valid : Call functions isIPv4() and isIPv6()
    InetAddr getPeerAddr() const;

    ssize_t recv(char* buf, size_t len);
    ssize_t send(char* buf, size_t len);
    /// @brief bind local address
    bool bind(const InetAddr& addr);
    /// @brief listen client socket
    bool listen(int num = 128);
    /// @return a new SocketTCP object
    /// @note
    /// Thought getPeerAddr obtain address if you input nullptr
    ///
    /// must check whether InetAddr and SocketTCP is invalid
    SocketTCP accept(InetAddr& addr, int flags = 0);
    /// @brief connect server
    /// @param addr server address
    bool connect(const InetAddr& addr);
    bool close();

    /// @brief set whether to enable port reuse
    bool portReuse(bool enable);

private:
    SocketTCP(int fd);

private:
    int sockfd_;

};

} // namespace net
} // namespace qinmo