#include "../include/qinmo/base/Timestamp.h"

#include <ctime>

qinmo::Timestamp::Timestamp() : microsecond_(0) {}

qinmo::Timestamp::Timestamp(qinmo::Timestamp::micro microsecondsSinceEpoch)
    : microsecond_(microsecondsSinceEpoch)
{ }

qinmo::Timestamp::micro qinmo::Timestamp::getMicroseconds() const
{
    return microsecond_;
}

qinmo::Timestamp::micro qinmo::Timestamp::getSeconds() const
{
    return microsecond_ / MicToSec;
}

std::string qinmo::Timestamp::toString(bool local) const
{
    time_t t = getSeconds();
    std::tm tm;
    if (local)
        ::localtime_r(&t, &tm);
    else
        ::gmtime_r(&t, &tm);

    char buf[32] = "";
    int len = std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

    return std::string(buf, len);
}

std::string qinmo::Timestamp::toStringMicroseconds(bool local) const
{
    time_t t = getSeconds();
    std::tm tm;
    if (local)
        ::localtime_r(&t, &tm);
    else
        ::gmtime_r(&t, &tm);

    char buf[32] = "";
    int len = std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

    if (0 == len) return "";
    len += std::snprintf(buf + len, sizeof(buf) - len, ".%06lld", getMicroseconds() % MicToSec);

    return std::string(buf, len);
}

qinmo::Timestamp qinmo::Timestamp::now()
{
    return Timestamp(std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
}

bool qinmo::operator<(const qinmo::Timestamp &a, const qinmo::Timestamp &b)
{
    return a.getMicroseconds() < b.getMicroseconds();
}

bool qinmo::operator==(const qinmo::Timestamp &a, const qinmo::Timestamp &b)
{
    return a.getMicroseconds() == b.getMicroseconds();
}