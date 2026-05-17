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
    explicit PacketBuffer(uint16_t headerSize, std::size_t bodySize = 1024);
    ~PacketBuffer();

public:
    /// @brief return header pointer
    const qinmo::StringView begin() const;


    /// @brief 
    const qinmo::StringView peekAll() const;
    int8_t peek8() const;
    int16_t peek16() const;
    int32_t peek32() const;
    int64_t peek64() const;
    std::string peekString(std::size_t len) const;

    std::string retrieveAll();
    int8_t retrieve8();
    int16_t retrieve16();
    int32_t retrieve32();
    int64_t retrieve64();
    std::string retrieveString(std::size_t len);


    void append(const char* data, std::size_t len);
    void append8(int8_t n);
    void append16(int16_t n);
    void append32(int32_t n);
    void append64(int64_t n);
    void appendString(const std::string& str);

    ssize_t readFd(int fd, int& savedErrno);
    ssize_t writeFd(int fd, int& savedErrno);

private:
    std::size_t readableSize() const;
    std::size_t writableSize() const;

    void retrieve(std::size_t len);
    void ensureWrite(std::size_t len);

    char* begin();

private:
    std::vector<char> buf_;
    std::size_t rIndex_;
    std::size_t wIndex_;

    uint16_t headerSize_;
    uint16_t hrIndex_;
    uint16_t hwIndex_;

};

} // namespace net
} // namespace qinmo