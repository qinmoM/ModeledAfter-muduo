#pragma once

#include <string>
#include <sstream>

namespace qinmo
{

template <typename... Args>
std::string stringSplice(Args&&... args)
{
    std::ostringstream oss;

    using expander = int[];
    (void)expander{ 0, (oss << std::forward<Args>(args), 0)... };

    return oss.str();
}

}
