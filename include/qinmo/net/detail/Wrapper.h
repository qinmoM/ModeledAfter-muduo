/**
 * @brief sockaddr-related functions wrapper class
 * 
 * This is an internal header file, you should not include this.
 */

#pragma once

#include "../../base/StringView.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <endian.h>
#include <stdint.h>

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{
/// @namespace qinmo::net::detail
/// @warning For internal use only, do NOT use from outside the library 
namespace detail
{

using sockaddr = struct ::sockaddr;
using sockaddr_in = struct ::sockaddr_in;
using sockaddr_in6 = struct ::sockaddr_in6;

/// @brief convert sockaddr pointer to/from sockaddr_in/sockaddr_in6 pointer
/// @tparam from type of t parameter
/// @tparam to target type
/// @note sockaddr_in and sockaddr_in6 are mutually non-convertible
template <typename from, typename to>
to* sockaddr_cast(from* t)
{
    static_assert(false, "sockaddr_cast : ");
    return nullptr;
}
/// @details template specialization
template <>
const sockaddr* sockaddr_cast(const sockaddr_in* addr) { return static_cast<const sockaddr*>(static_cast<const void*>(addr)); }
template <>
const sockaddr* sockaddr_cast(const sockaddr_in6* addr) { return static_cast<const sockaddr*>(static_cast<const void*>(addr)); }
template <>
sockaddr* sockaddr_cast(sockaddr_in* addr) { return static_cast<sockaddr*>(static_cast<void*>(addr)); }
template <>
sockaddr* sockaddr_cast(sockaddr_in6* addr) { return static_cast<sockaddr*>(static_cast<void*>(addr)); }
template <>
const sockaddr_in* sockaddr_cast(const sockaddr* addr) { return static_cast<const sockaddr_in*>(static_cast<const void*>(addr)); }
template <>
const sockaddr_in6* sockaddr_cast(const sockaddr* addr) { return static_cast<const sockaddr_in6*>(static_cast<const void*>(addr)); }
template <>
sockaddr_in* sockaddr_cast(sockaddr* addr) { return static_cast<sockaddr_in*>(static_cast<void*>(addr)); }
template <>
sockaddr_in6* sockaddr_cast(sockaddr* addr) { return static_cast<sockaddr_in6*>(static_cast<void*>(addr)); }

/// @brief network to host byte order, supporting 16-bit, 32-bit and 64-bit unsigned intergers
/// @param net network order
inline uint16_t netToHost16(uint16_t net);
inline uint32_t netToHost32(uint32_t net);
inline uint64_t netToHost64(uint64_t net);

/// @brief host to network byte order, supporting 16-bit, 32-bit and 64-bit unsigned intergers
/// @param host host order
inline uint16_t hostToNet16(uint16_t host);
inline uint32_t hostToNet32(uint32_t host);
inline uint64_t hostToNet64(uint64_t host);

/// @brief initialize this memory block to zero
/// @param buf pointer
/// @param len length
void zeroMemory(void* buf, size_t len);

/// @brief convert dotted decimal to network byte order
/// @param cp dotted decimal string
/// @param addr net address
/// @return return true when successful, otherwise false
bool pton4(StringView cp, in_addr& addr);
bool pton6(StringView cp, in6_addr& addr);
/// @brief convert network byte order to dotted decimal
/// @param addr net address
/// @return return dotted decimal string
std::string ntop4(const in_addr& addr);
std::string ntop6(const in6_addr& addr);

int close(int fd);
int portReuse(int sockfd);
ssize_t send(int sockfd, void* buf, size_t count);
ssize_t recv(int sockfd, void* buf, size_t count);
ssize_t sendto(int sockfd, void* buf, size_t count, const sockaddr& addr);
ssize_t recvfrom(int sockfd, void* buf, size_t count, const sockaddr& addr);

} // namespace detail
} // namespace net
} // namespace qinmo