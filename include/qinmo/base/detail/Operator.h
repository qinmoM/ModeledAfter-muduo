#pragma once

/// @namespace qinmo
namespace qinmo
{
/// @namespace qinmo::detail
/// @warning For internal use only, do NOT use it from outside the library
namespace detail
{

/**
 * automatically generate >,>=,<= operator when < is overloaded.
 */
template <typename T>
struct comp_less
{
    friend bool operator>(const T &a, const T &b)
    {
        return b < a;
    }
    friend bool operator<=(const T &a, const T &b)
    {
        return !(b < a);
    }
    friend bool operator>=(const T &a, const T &b)
    {
        return !(a < b);
    }
};

/**
 * automatically generate != operator when == is overloaded.
 */
template <typename T>
struct comp_equal
{
    friend bool operator!=(const T &a, const T &b)
    {
        return !(b == a);
    }
};

} // namespace detail

} // namespace qinmo