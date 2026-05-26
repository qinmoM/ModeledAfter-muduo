#include "qinmo/base/ThreadConfig.h"

namespace qinmo
{
/// @namespace qinmo::detail
namespace detail
{

thread_local ThreadIDType cached_thread_id = g_ThreadIDTypeEmpty;

ThreadIDType getTid()
{
    if (g_ThreadIDTypeEmpty == cached_thread_id)
        cached_thread_id = tid();

    return cached_thread_id;
}

uint32_t getTid32()
{
    return static_cast<uint32_t>(getTid());
}

} // namespace detail
} // namespace qinmo