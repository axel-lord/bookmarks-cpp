#include "./fs_misc.hpp"
#include "bookmarks/command_context.hpp"

namespace bm::commands::file
{
void
change(command_context ctx)
{
    if (ctx.arguments.empty())
    {
        fmt::print(fg(fmt::color::yellow), "Usage: fs change PATH");
        return;
    }

    // not const in order to move from
    auto new_path = ensure_directory(build_location_path(ctx.current_dir, ctx.arguments));
    if (!new_path)
    {
        fmt::print(fg(fmt::color::yellow), "\"{}\" is not a valid path change.", ctx.arguments);
        return;
    }

    // c++23 use and_then
    ctx.current_dir = std::move(*new_path);
}
} // namespace bm::commands::file
