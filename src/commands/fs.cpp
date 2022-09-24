#include "bookmarks/command_arguments.hpp"
#include "bookmarks/command_context.hpp"
#include "bookmarks/fs_commands.hpp"

#include <fmt/color.h>
#include <fmt/core.h>
#include <string_view>

namespace bm::commands
{
using namespace std::literals::string_view_literals;

static auto const fs_cmd_map = bm::commands::command_map{
    {"list"sv, file::list},
    {"change"sv, file::change},
};

void
fs(command_context ctx)
{
    auto const [command, arguments] = parse_arguments(ctx.arguments);

    if (!fs_cmd_map.contains(command))
    {
        fmt::print(
            "{} \"{}\"\n",
            styled("Could not find command", fg(fmt::color::yellow)),
            styled(fmt::format("fs {}", command), fmt::emphasis::bold)
        );
        return;
    }

    auto const mutate_context = [arguments](command_context ctx)
    {
        ctx.arguments = arguments;
        return ctx;
    };

    fs_cmd_map.at(command)(mutate_context(ctx));
}
} // namespace bm::commands

namespace bm::commands::file
{
void
list(command_context)
{
}

void
change(command_context)
{
}
} // namespace bm::commands::file