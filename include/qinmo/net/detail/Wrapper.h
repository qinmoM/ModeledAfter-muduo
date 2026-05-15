/**
 * @brief sockaddr-related functions wrapper class
 * 
 * This is an internal header file, you should not include this.
 */

#pragma once

#include "qinmo/base/detail/Common.h"
#include "../../base/StringView.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <endian.h>
#include <stdint.h>
#include <netinet/tcp.h>

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{
/// @namespace qinmo::net::detail
/// @warning For internal use only, do NOT use it from outside the library
namespace detail
{

using sockaddr_in = struct ::sockaddr_in;
using sockaddr_in6 = struct ::sockaddr_in6;
union sockaddr
{
    sockaddr_in addr4_;
    sockaddr_in6 addr6_;
};

/// @brief convert sockaddr pointer to/from sockaddr_in/sockaddr_in6 pointer
/// @tparam from type of t parameter
/// @tparam to target type
/// @note sockaddr_in and sockaddr_in6 are mutually non-convertible
template <typename from, typename to>
to* sockaddr_cast(from* t)
{
    static_assert(false, "sockaddr_cast : unspecialized types.");
    return nullptr;
}
/// @details template specialization
template <>
inline const sockaddr* sockaddr_cast(const sockaddr_in* addr) { return static_cast<const sockaddr*>(static_cast<const void*>(addr)); }
template <>
inline const sockaddr* sockaddr_cast(const sockaddr_in6* addr) { return static_cast<const sockaddr*>(static_cast<const void*>(addr)); }
template <>
inline sockaddr* sockaddr_cast(sockaddr_in* addr) { return static_cast<sockaddr*>(static_cast<void*>(addr)); }
template <>
inline sockaddr* sockaddr_cast(sockaddr_in6* addr) { return static_cast<sockaddr*>(static_cast<void*>(addr)); }
template <>
inline const sockaddr_in* sockaddr_cast(const sockaddr* addr) { return &addr->addr4_; }
template <>
inline const sockaddr_in6* sockaddr_cast(const sockaddr* addr) { return &addr->addr6_; }
template <>
inline sockaddr_in* sockaddr_cast(sockaddr* addr) { return &addr->addr4_; }
template <>
inline sockaddr_in6* sockaddr_cast(sockaddr* addr) { return &addr->addr6_; }
/// @brief detail::sockaddr convert to generic sockaddr
template <>
inline ::sockaddr* sockaddr_cast(sockaddr* addr) { return static_cast<::sockaddr*>(static_cast<void*>(addr)); }
template <>
inline const ::sockaddr* sockaddr_cast(const sockaddr* addr) { return static_cast<const ::sockaddr*>(static_cast<const void*>(addr)); }
template <>
inline sockaddr* sockaddr_cast(::sockaddr* addr) { return static_cast<sockaddr*>(static_cast<void*>(addr)); }
template <>
inline const sockaddr* sockaddr_cast(const ::sockaddr* addr) { return static_cast<const sockaddr*>(static_cast<const void*>(addr)); }

/// @brief network to host byte order, supporting 16-bit, 32-bit and 64-bit unsigned intergers
/// @param net network order
inline uint16_t netToHost16(uint16_t net) { return be16toh(net); }
inline uint32_t netToHost32(uint32_t net) { return be32toh(net); }
inline uint64_t netToHost64(uint64_t net) { return be64toh(net); }

/// @brief host to network byte order, supporting 16-bit, 32-bit and 64-bit unsigned intergers
/// @param host host order
inline uint16_t hostToNet16(uint16_t host) { return htobe16(host); }
inline uint32_t hostToNet32(uint32_t host) { return htobe32(host); }
inline uint64_t hostToNet64(uint64_t host) { return htobe64(host); }

/// @brief initialize this memory block to zero
/// @param buf pointer
/// @param len length
inline void zeroMemory(void* buf, size_t len) { ::bzero(buf, len); }

/// @brief convert dotted decimal to network byte order
/// @param cp dotted decimal string
/// @param addr net address
/// @return return true when successful, otherwise false
inline bool pton4(StringView cp, in_addr& addr) { return 1 == ::inet_pton(AF_INET, cp.data(), &addr); }
inline bool pton6(StringView cp, in6_addr& addr) { return 1 == ::inet_pton(AF_INET6, cp.data(), &addr); }
/// @brief convert network byte order to dotted decimal
/// @param addr net address
/// @return return dotted decimal string
inline std::string ntop4(const in_addr& addr) { char buf[INET_ADDRSTRLEN]; return ::inet_ntop(AF_INET, &addr, buf, sizeof(buf)); }
inline std::string ntop6(const in6_addr& addr) { char buf[INET6_ADDRSTRLEN]; return ::inet_ntop(AF_INET6, &addr, buf, sizeof(buf)); }

inline bool getsockname(int sockfd, sockaddr& addr) { socklen_t len = sizeof(addr); return ::getsockname(sockfd, sockaddr_cast<sockaddr, ::sockaddr>(&addr), &len); }
inline bool getpeername(int sockfd, sockaddr& addr) { socklen_t len = sizeof(sockaddr_in6); return ::getpeername(sockfd, sockaddr_cast<sockaddr, ::sockaddr>(&addr), &len); }

inline int socket(int af, int type, int protocol = 0) { return ::socket(af, type, protocol); }
inline bool bind(int sockfd, const sockaddr& addr) { return 0 == ::bind(sockfd, sockaddr_cast<const sockaddr, const ::sockaddr>(&addr), sizeof(addr)); }
inline bool listen(int sockfd, int num = 128){ return 0 == ::listen(sockfd, num); };
inline int accept(int sockfd, sockaddr& addr, int flags = 0) { socklen_t len = sizeof(addr); return ::accept4(sockfd, sockaddr_cast<sockaddr, ::sockaddr>(&addr), &len, flags); }
inline bool connect(int sockfd, const sockaddr& addr) { return 0 == ::connect(sockfd, sockaddr_cast<const sockaddr, const ::sockaddr>(&addr), sizeof(addr)); }
inline ssize_t send(int sockfd, const void* buf, size_t count) { return ::send(sockfd, buf, count, 0); }
inline ssize_t recv(int sockfd, void* buf, size_t count) { return ::recv(sockfd, buf, count, 0); }
inline ssize_t sendto(int sockfd, const void* buf, size_t count, const sockaddr& addr) { return ::sendto(sockfd, buf, count, 0,sockaddr_cast<const sockaddr, const ::sockaddr>(&addr), sizeof(sockaddr)); }
inline ssize_t recvfrom(int sockfd, void* buf, size_t count, sockaddr& addr, unsigned int& len) { return ::recvfrom(sockfd, buf, count, 0, sockaddr_cast<sockaddr, ::sockaddr>(&addr), &len); }

inline bool isAddrReuse(int sockfd) { int opt = 0; socklen_t len = sizeof(opt); return 0 == ::getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, &len) && 1 == opt; }

inline bool setAddrReuse(int sockfd, bool enable) { int opt = enable ? 1 : 0; return 0 == ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); }
inline bool setPortReuse(int sockfd, bool enable) { int opt = enable ? 1 : 0; return 0 == ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)); }
inline bool setTcpNoDelay(int sockfd, bool enable) { int opt = enable ? 1 : 0; return 0 == ::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)); }
inline bool setKeepAlive(int sockfd, bool enable) { int opt = enable ? 1 : 0; return 0 == ::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)); }
/// @return 0 if fails
inline int getSocketType(int sockfd) { int opt = 0; socklen_t len = sizeof(opt); return (0 == ::getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &opt, &len) ? opt : 0); }

} // namespace detail
} // namespace net
} // namespace qinmo