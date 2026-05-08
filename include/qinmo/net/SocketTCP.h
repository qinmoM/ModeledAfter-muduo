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
    /// @param addr 
    /// @return  using the move constructor
    static SocketTCP create(const InetAddr& addr);
    static SocketTCP attach(const int fd);

    SocketTCP();
    ~SocketTCP();

    SocketTCP(SocketTCP&& other) noexcept;
    SocketTCP& operator=(SocketTCP&& other) noexcept;

    SocketTCP(const SocketTCP&) = delete;
    SocketTCP& operator=(const SocketTCP&) = delete;

public:
    bool valid() const;
    int getfd() const;
    InetAddr getLocalAddr() const;
    InetAddr getPeerAddr() const;

    bool bind(const InetAddr& addr);
    bool listen(int num = 128);
    SocketTCP accept();
    bool close();

    bool portReuse(bool enable);

private:
    SocketTCP(int fd);

private:
    int sockfd_;

};

} // namespace net
} // namespace qinmo