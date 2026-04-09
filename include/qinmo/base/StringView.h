#pragma once

#include <string>
#include <string.h>
#include <stdint.h>

namespace qinmo
{

class StringView
{
public:
    StringView() : ptr_(nullptr), len_(0) { }
    StringView(const char* ptr, uint32_t len) : ptr_(ptr), len_(len) { }
    StringView(const std::string& string)
        : ptr_(string.c_str())
        , len_(string.size())
    { }
    StringView(const char* ptr)
        : ptr_(ptr)
        , len_(static_cast<uint32_t>((nullptr == ptr ? 0 : ::strlen(ptr))))
    { }
    StringView(const unsigned char* ptr)
        : ptr_(static_cast<const char*>(static_cast<const void*>(ptr)))
        , len_(static_cast<uint32_t>((nullptr == ptr ? 0 : ::strlen(static_cast<const char*>(static_cast<const void*>(ptr))))))
    { }
    StringView(const unsigned char* ptr, uint32_t len)
        : ptr_(static_cast<const char*>(static_cast<const void*>(ptr)))
        , len_(len)
    { }

public:
    const char* data() const { return ptr_; }
    uint32_t size() const { return len_; }
    bool empty() const { return !len_; }
    void set(const char* ptr) { ptr_ = ptr; len_ = (nullptr == ptr ? 0 : ::strlen(ptr)); }
    void set(const char* ptr, int len) { ptr_ = ptr; len_ = len; }
    void clear() { ptr_ = nullptr; len_ = 0; }

    std::string to_string() const { return std::string(ptr_, len_); }

    const char& operator[](uint32_t index) const { return *(ptr_ + index); }

private:
    const char* ptr_;
    uint32_t len_;

};

bool operator==(const StringView& a, const StringView& b) { return 0 == strncmp(a.data(), b.data(), a.size()); }
bool operator!=(const StringView& a, const StringView& b) { return !(a == b); }

} // namespace qinmo