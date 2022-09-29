#include "bookmarks/command_arguments.hpp"
#include "bookmarks/command_context.hpp"
#include "bookmarks/fs_commands.hpp"

#include <filesystem>

using namespace std::literals::string_view_literals;

namespace bm::commands::file
{
namespace
{
struct filesystem_command
{
    command_map cmap = {
        {"list"sv, list},
        {"change"sv, change},
        {"load"sv, load},
    };

    void operator()(command_context ctx) const
    {
        auto const [command, arguments] = parse_arguments(ctx.arguments);

        if (!cmap.contains(command))
        {
            fmt::print(
                "{} \"{}\"\n",
                styled("Could not find command", fg(fmt::color::yellow)),
                styled(fmt::format("fs {}", command), fmt::emphasis::bold)
            );
            return;
        }

        auto const mutate_context = [](command_context context, auto args)
        {
            context.arguments = args;
            return context;
        };

        cmap.at(command)(mutate_context(ctx, arguments));
    }
};

} // namespace
} // namespace bm::commands::file

namespace bm::commands
{
std::function<void(command_context)>
make_fs_command()
{
    return file::filesystem_command{};
}
} // namespace bm::commands
