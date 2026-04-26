#include "qinmo/base/ThreadConfig.h"

namespace qinmo
{
/// @namespace qinmo::detail
namespace detail
{

thread_local ThreadType cached_thread_id = threadTypeEmpty;

ThreadType getTid()
{
    if (threadTypeEmpty == cached_thread_id)
        cached_thread_id = tid();

    return cached_thread_id;
}

} // namespace detail
} // namespace qinmo