#pragma once

#include "detail/Common.h"

namespace qinmo
{
/// @namespace qinmo::detail
namespace detail
{

#ifdef __linux__
    using ThreadIDType = pid_t;

    static constexpr ThreadIDType threadIDTypeEmpty = 0;
#else
    #error "Platform not supported"
#endif



ThreadIDType getTid();

} // namespace detail
} // qinmo