#pragma once

#include "detail/Common.h"

namespace qinmo
{
/// @namespace qinmo::detail
namespace detail
{

#ifdef __linux__
    using ThreadType = pid_t;

    static constexpr ThreadType threadTypeEmpty = 0;
#else
    #error "Platform not supported"
#endif



inline ThreadType getTid();

} // namespace detail
} // qinmo