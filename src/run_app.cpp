#include "bookmarks/command_arguments.hpp"
#include "bookmarks/command_context.hpp"
#include "bookmarks/commands.hpp"
#include "bookmarks/data_registry.hpp"
#include "util/literal_suffixes.hpp"

#include <filesystem>
#include <fmt/color.h>
#include <iostream>
#include <optional>
#include <string_view>

using namespace std::literals;
using namespace bm::util::literals;

namespace bm
{
[[nodiscard]] std::vector<bookmark> build_bookmark_vector(std::string_view const bookmark_view);
[[nodiscard]] std::optional<std::ostringstream> read_file(std::filesystem::path const& filepath);
} // namespace bm

namespace bm::app
{

static inline auto
get_input()
{
    auto buffer = std::string{};
    getline(std::cin, buffer);
    return buffer;
}

void
run_app(std::optional<std::string_view> const bookmark_view)
{
    auto bookmarks       = bm::build_bookmark_vector(bookmark_view.value_or(""sv));
    auto bookmark_buffer = std::vector<bm::bookmark>{};
    auto current         = std::span{bookmarks};
    auto current_dir     = std::filesystem::current_path();
    auto registry        = bm::data::registry{};

    auto const cmap = bm::commands::command_map{
        {"show"sv, bm::commands::show},
        {"filter"sv, bm::commands::filter},
        {"count"sv, bm::commands::count},
        {"reset"sv, bm::commands::reset},
        {"fuzzy"sv, bm::commands::fuzzy},
        {"regex"sv, bm::commands::regex},
        {"fs"sv, bm::commands::make_fs_command()},
    };

    bookmark_buffer.reserve(bookmarks.capacity());

    while (true)
    {
        fmt::print(
            "{0} [{1}]\n> ",
            fmt::styled("Enter Command", fg(fmt::color::light_sea_green)),
            current_dir.string()
        );

        auto const buffer = get_input();

        if (buffer.empty()) // nothing entered
        {
            fmt::print(fg(fmt::color::yellow), "Please enter a command.\n");
            continue;
        }

        auto const [command, arguments] = bm::commands::parse_arguments(buffer);

        if (command == "exit"sv) // special command to ensure exit is possible
            return;

        if (!cmap.contains(command))
        {
            fmt::print(
                "{} \"{}\"\n",
                styled("Could not find command", fg(fmt::color::yellow)),
                styled(command, fmt::emphasis::bold)
            );
            continue;
        }

        cmap.at(command)(bm::commands::command_context{
            .arguments       = arguments,
            .bookmarks       = bookmarks,
            .bookmark_buffer = bookmark_buffer,
            .current         = current,
            .cmap            = cmap,
            .current_dir     = current_dir,
            .registry        = registry,
        });
    }
}

void
run_app()
{
    return run_app(std::nullopt);
}
} // namespace bm::app
