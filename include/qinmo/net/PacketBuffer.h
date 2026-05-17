#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include "../base/StringView.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

class PacketBuffer
{
public:
    /// @param headerSize application-layer protocol header length
    /// @param bodySize message length
    /// @note if headerSize is 0, it is a normal buffer
    explicit PacketBuffer(uint8_t headerSize, std::size_t bodySize = 1024);
    ~PacketBuffer();

public:
    /// @brief all length of protocol header
    uint8_t getHeadSize() const;
    /// @brief length of readable message body
    std::size_t getReadableSize() const;
    /// @brief length of writable message body
    std::size_t getWritableSize() const;


    /// @brief return protocol header value in host-endian by offset and size
    /// @param offset value Local at the offset from the start
    uint8_t headGet8(uint8_t offset) const;
    uint16_t headGet16(uint8_t offset) const;
    uint32_t headGet32(uint8_t offset) const;
    uint64_t headGet64(uint8_t offset) const;
    const qinmo::StringView headGetString(uint8_t offset, uint8_t len) const;
    const qinmo::StringView headGetAll() const;

    /// @brief set value in net-endian at the give offset
    /// @param offset value Local at the offset from the start
    void headSet8(uint8_t offset, uint8_t n) const;
    void headSet16(uint8_t offset, uint16_t n) const;
    void headSet32(uint8_t offset, uint32_t n) const;
    void headSet64(uint8_t offset, uint64_t n) const;
    void headSetString(uint8_t offset, const char* str, uint8_t len) const;


    /// @brief get value in host-endian without advancing the pointer
    int8_t peek8() const;
    int16_t peek16() const;
    int32_t peek32() const;
    int64_t peek64() const;
    std::string peekString(std::size_t len) const;
    const qinmo::StringView peekAll() const;

    /// @brief get value in host-endian within advancing the pointer
    int8_t retrieve8();
    int16_t retrieve16();
    int32_t retrieve32();
    int64_t retrieve64();
    std::string retrieveString(std::size_t len);
    std::string retrieveAll();

    /// @brief append value in net-endian at end of buffer
    void append(const char* data, std::size_t len);
    void append8(int8_t n);
    void append16(int16_t n);
    void append32(int32_t n);
    void append64(int64_t n);
    void appendString(qinmo::StringView str);

    /// @brief read / write data between file desriptor and buffer
    /// @param savedErrno error code
    /// @note auto resize
    ssize_t readFd(int fd, int& savedErrno);
    ssize_t writeFd(int fd, int& savedErrno);

private:
    void retrieve(std::size_t len);
    void ensureWrite(std::size_t len);

    char* begin();
    const char* begin() const;

private:
    std::vector<char> buf_;
    std::size_t rIndex_;
    std::size_t wIndex_;

    const uint8_t headerSize_;

};

} // namespace net
} // namespace qinmo