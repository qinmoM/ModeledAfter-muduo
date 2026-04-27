#include "qinmo/base/ThreadConfig.h"

namespace qinmo
{
/// @namespace qinmo::detail
namespace detail
{

thread_local ThreadIDType cached_thread_id = threadIDTypeEmpty;

ThreadIDType getTid()
{
    if (threadIDTypeEmpty == cached_thread_id)
        cached_thread_id = tid();

    return cached_thread_id;
}

} // namespace detail
} // namespace qinmo