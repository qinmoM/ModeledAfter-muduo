#pragma once

#include "TcpConnect.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

/// @brief encapsulate listen socket
/// @note example: TcpListen listen = TcpListen::createRaw(InetAddr(), SOCK_NONBLOCK | SOCK_CLOEXEC);
class TcpListen
{
public:
    /// @brief create a listen socket
    /// @return use move construct
    /// @note better to check whether the returned value is valid: call isValid()
    static TcpListen createRaw(const InetAddr& addr, int flags = 0);
    /// @brief equal to createRaw(addr, SOCK_NONBLOCK | SOCK_CLOEXEC)
    /// @note better to check whether the returned value is valid: call isValid()
    static TcpListen createNonBlockOrDie(const InetAddr& addr);

public:
    /// @return return true it has been initialized
    bool isValid() const;
    bool isBind() const;
    bool isListen() const;
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
    TcpConnect accept();
    /// @note isValid function return false After call close
    bool close();

    /// @brief set whether to enable port reuse
    bool setReusePort(bool enable);
    /// @brief set whether to enable addres reuse
    bool setReuseAddr(bool enable);

private:
    enum class TcpListenState : char
    {
        Created = 0,
        Bound = 1,
        Listening = 2
    };

private:
    SocketTCP sock_;
    TcpListenState state_;

};
} // namespace net
} // namespace qinmo