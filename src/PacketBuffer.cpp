#include "qinmo/net/PacketBuffer.h"
#include "qinmo/net/detail/Wrapper.h"

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmp::net
namespace net
{

PacketBuffer::PacketBuffer(uint8_t headerSize, std::size_t bodySize)
    : headerSize_(headerSize)
    , buf_(headerSize + bodySize)
    , rIndex_(headerSize)
    , wIndex_(headerSize)
{ }

PacketBuffer::~PacketBuffer() { }

uint8_t PacketBuffer::getHeadSize() const
{
    return headerSize_;
}

std::size_t PacketBuffer::getReadableSize() const
{
    return wIndex_ - rIndex_;
}

std::size_t PacketBuffer::getWritableSize() const
{
    return buf_.size() - wIndex_;
}



bool PacketBuffer::headGet8(uint8_t offset, uint8_t& target) const
{
    if (offset + sizeof(target) > headerSize_)
        return false;

    memcpy(&target, begin() + offset, sizeof(target));
    return true;
}

bool PacketBuffer::headGet16(uint8_t offset, uint16_t& target) const
{
    if (offset + sizeof(target) > headerSize_)
        return false;

    memcpy(&target, begin() + offset, sizeof(target));
    target = detail::netToHost16(target);
    return true;
}

bool PacketBuffer::headGet32(uint8_t offset, uint32_t& target) const
{
    if (offset + sizeof(target) > headerSize_)
        return false;

    memcpy(&target, begin() + offset, sizeof(target));
    target = detail::netToHost32(target);
    return true;
}

bool PacketBuffer::headGet64(uint8_t offset, uint64_t& target) const
{
    if (offset + sizeof(target) > headerSize_)
        return false;

    memcpy(&target, begin() + offset, sizeof(target));
    target = detail::netToHost64(target);
    return true;
}

bool PacketBuffer::headGetString(uint8_t offset, uint8_t len, std::string& str) const
{
    if (offset + len > headerSize_)
        return false;

    str.assign(begin() + offset, begin() + offset + len);
    return true;
}

bool PacketBuffer::headGetAll(std::string& str) const
{
    str.assign(begin(), headerSize_);
    return true;
}


bool PacketBuffer::headSet8(uint8_t offset, uint8_t n)
{
    if (offset + sizeof(n) > headerSize_)
        return false;

    memcpy(begin() + offset, &n, sizeof(n));
    return true;
}

bool PacketBuffer::headSet16(uint8_t offset, uint16_t n)
{
    if (offset + sizeof(n) > headerSize_)
        return false;

    n = detail::hostToNet16(n);
    memcpy(begin() + offset, &n, sizeof(n));
    return true;
}

bool PacketBuffer::headSet32(uint8_t offset, uint32_t n)
{
    if (offset + sizeof(n) > headerSize_)
        return false;

    n = detail::hostToNet32(n);
    memcpy(begin() + offset, &n, sizeof(n));
    return true;
}

bool PacketBuffer::headSet64(uint8_t offset, uint64_t n)
{
    if (offset + sizeof(n) > headerSize_)
        return false;

    n = detail::hostToNet64(n);
    memcpy(begin() + offset, &n, sizeof(n));
    return true;
}

bool PacketBuffer::headSetString(uint8_t offset, const std::string& str)
{
    if (offset + str.size() > headerSize_)
        return false;

    memcpy(begin() + offset, str.data(), str.size());
    return true;
}



bool PacketBuffer::peek8(int8_t& target) const
{
    if (getReadableSize() < sizeof(target))
        return false;

    memcpy(&target, &buf_[headerSize_ + rIndex_], sizeof(target));
    return true;
}

bool PacketBuffer::peek16(int16_t& target) const
{
    if (getReadableSize() < sizeof(target))
        return false;

    memcpy(&target, &buf_[headerSize_ + rIndex_], sizeof(target));
    target = detail::netToHost16(target);
    return true;
}

bool PacketBuffer::peek32(int32_t& target) const
{
    if (getReadableSize() < sizeof(target))
        return false;

    memcpy(&target, &buf_[headerSize_ + rIndex_], sizeof(target));
    target = detail::netToHost32(target);
    return true;
}

bool PacketBuffer::peek64(int64_t& target) const
{
    if (getReadableSize() < sizeof(target))
        return false;

    memcpy(&target, &buf_[headerSize_ + rIndex_], sizeof(target));
    target = detail::netToHost64(target);
    return true;
}

bool PacketBuffer::peekString(std::size_t len, std::string& str) const
{
    if (getReadableSize() < len)
        return false;

    const char* dataPtr = &buf_[headerSize_ + rIndex_];
    str.assign(dataPtr, dataPtr + len);
    return true;
}

bool PacketBuffer::peekAll(std::string& str) const
{
    const char* dataPtr = &buf_[headerSize_ + rIndex_];
    str.assign(dataPtr, dataPtr + getReadableSize());
    return true;
}



bool PacketBuffer::retrieve8(int8_t& target)
{
    if (!peek8(target))
        return false;

    retrieve(sizeof(target));
    return true;
}

bool PacketBuffer::retrieve16(int16_t& target)
{
    if (!peek16(target))
        return false;

    retrieve(sizeof(target));
    return true;
}

bool PacketBuffer::retrieve32(int32_t& target)
{
    if (!peek32(target))
        return false;

    retrieve(sizeof(target));
    return true;
}

bool PacketBuffer::retrieve64(int64_t& target)
{
    if (!peek64(target))
        return false;

    retrieve(sizeof(target));
    return true;
}

bool PacketBuffer::retrieveString(std::size_t len, std::string& str)
{
    if (!peekString(len, str))
        return false;

    retrieve(len);
    return true;
}

bool PacketBuffer::retrieveAll(std::string& str)
{
    if (!peekAll(str))
        return false;

    retrieve(getReadableSize());
    return true;
}



void PacketBuffer::append(const char* data, std::size_t len)
{
    ensureWrite(len);
    memcpy(begin() + wIndex_, data, len);
    wIndex_ += len;
}

void PacketBuffer::append8(int8_t n)
{
    return append(static_cast<const char*>(static_cast<const void*>(&n)), sizeof(n));
}

void PacketBuffer::append16(int16_t n)
{
    n = detail::hostToNet16(n);
    return append(static_cast<const char*>(static_cast<const void*>(&n)), sizeof(n));
}

void PacketBuffer::append32(int32_t n)
{
    n = detail::hostToNet32(n);
    return append(static_cast<const char*>(static_cast<const void*>(&n)), sizeof(n));
}

void PacketBuffer::append64(int64_t n)
{
    n = detail::hostToNet64(n);
    return append(static_cast<const char*>(static_cast<const void*>(&n)), sizeof(n));
}

void PacketBuffer::appendString(const std::string& str)
{
    return append(str.data(), str.size());
}


void PacketBuffer::retrieve(std::size_t len)
{
    if (len == getReadableSize())
        rIndex_ = wIndex_ = headerSize_;
    else
        rIndex_ += len;
}

void PacketBuffer::ensureWrite(std::size_t len)
{
    if (getWritableSize() >= len)
        return;
    
    if (rIndex_ - headerSize_ + getWritableSize() < len)
        return buf_.resize(buf_.size() + len - getWritableSize());

    memcpy(begin() + headerSize_, begin() + rIndex_, getReadableSize());
    std::size_t temp = getReadableSize();
    rIndex_ = headerSize_;
    wIndex_ = temp + rIndex_;
}

char* PacketBuffer::begin()
{
    return &buf_[0];
}

const char* PacketBuffer::begin() const
{
    return &buf_[0];
}


} // namespace net
} // namespace qinmo