#pragma once

/// @namespace qinmo
namespace qinmo
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
}