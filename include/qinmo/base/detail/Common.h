#pragma once

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/eventfd.h>

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmo::detail
/// @warning For internal use only, do NOT use from outside the library 
namespace detail
{

inline ssize_t read(int fd, void* ptr, size_t count)
{
    return ::read(fd, ptr, count);
}

inline ssize_t write(int fd, void* ptr, size_t count)
{
    return ::write(fd, ptr, count);
}

/*
                eventfd
*/
inline int eventfd(unsigned int initval, int flags)
{
    return ::eventfd(initval, flags);
}

/*
                syscall
*/
inline long tid()
{
    return ::syscall(SYS_gettid);
}

} // namespace detail
} // namespace qinmo