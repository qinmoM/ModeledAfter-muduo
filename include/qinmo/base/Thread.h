#pragma once

#include "ThreadConfig.h"
#include <thread>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace qinmo
{

/**
 * @brief std::thread encapsulation class
 */
class Thread
{
public:
    using ThreadFunc = std::function<void()>;

public:
    /// @param func subthread function
    /// @note not start a thread directly
    explicit Thread(ThreadFunc func);
    ~Thread();

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&&) = delete;
    Thread& operator=(Thread&&) = delete;

public:
    /// @note get curent information
    bool started() const;
    bool joined() const;
    qinmo::detail::ThreadIDType getID() const;

    /// @brief start a thread
    /// @note truly create a thread After called this
    void start();
    /// @brief release resource
    /// @note must call this Before destruct
    void join();

private:
    std::atomic<bool> started_;
    std::atomic<bool> joined_;

    std::thread thread_;
    ThreadFunc func_;

    detail::ThreadIDType id_;
    std::mutex mutex_;
    std::condition_variable cv_;

};
} // namespace qinmo
