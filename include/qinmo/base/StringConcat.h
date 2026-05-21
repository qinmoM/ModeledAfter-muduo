#pragma once

#include <string>
#include <sstream>
#include <iostream>

namespace qinmo
{

template <typename... Args>
std::string concat(Args&&... args)
{
    std::ostringstream oss;

    using Expander = int[];
    (void)Expander{ 0, (oss << std::forward<Args>(args), 0)... };

    return oss.str();
}

template <typename... Args>
void print(Args&&... args)
{
    std::cout << concat(args...);
}

template <typename... Args>
void println(Args&&... args)
{
    std::cout << concat(args...) << '\n';
}

} // namespace qinmo