#include "bookmarks/command_arguments.hpp"
#include "bookmarks/command_context.hpp"
#include "bookmarks/fs_commands.hpp"
#include "bookmarks/line_manipulation.hpp"

#include <atomic>
#include <filesystem>
#include <fmt/color.h>
#include <fmt/core.h>
#include <forward_list>
#include <range/v3/algorithm/for_each.hpp>
#include <string_view>
#include <system_error>

namespace bm::commands
{
using namespace std::literals::string_view_literals;

static auto const fs_cmd_map = bm::commands::command_map{
    {"list"sv, file::list},
    {"change"sv, file::change},
    {"load"sv, file::load},
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

    auto const mutate_context = [](command_context ctx, auto args)
    {
        ctx.arguments = args;
        return ctx;
    };

    fs_cmd_map.at(command)(mutate_context(ctx, arguments));
}
} // namespace bm::commands

namespace fs = std::filesystem;

static auto
ensure_directory(fs::path const& p)
{
    if (is_directory(p))
        return std::optional<fs::path>{p};
    else
        return std::optional<fs::path>{std::nullopt};
}

static auto
build_location_path(fs::path const& cwd, fs::path const& input)
{
    if (input.is_absolute())
        return input;
    else
        return absolute(cwd / input);
}

namespace bm::commands::file
{
void
list(command_context ctx)
{
    // if (!ctx.arguments.empty())
    //     fmt::print(fg(fmt::color::yellow), "Usage: fs list");

    auto const get_directory_to_list = [&]()
    {
        if (ctx.arguments.empty())
            return ctx.current_dir;
        else
            return build_location_path(ctx.current_dir, ctx.arguments);
    };

    auto const print_directory_entry = [](fs::directory_entry const& entry)
    {
        constexpr auto format_string = "{}\n";

        auto const& path        = entry.path();
        auto const  path_string = path.filename().string();

        auto file_err = std::error_code{};
        auto dir_err  = std::error_code{};

        if (fs::is_regular_file(path, file_err))
        {
            fmt::print(format_string, path_string);
        }
        else if (file_err)
        {
            fmt::print(
                "{} [error code: {}]\n",
                styled(path_string, fg(fmt::color::hot_pink)),
                file_err.value()
            );
        }
        else if (fs::is_directory(path, dir_err))
        {
            fmt::print(
                fmt::emphasis::bold | fg(fmt::color::lime_green), format_string, path_string
            );
        }
        else if (dir_err)
        {
            fmt::print(
                "{} [error code: {}]\n",
                styled(path_string, fg(fmt::color::orange)),
                dir_err.value()
            );
        }
        else
        {
            fmt::print(fmt::emphasis::italic, format_string, path_string);
        }
    };

    auto const directory = ensure_directory(get_directory_to_list());
    if (!directory)
    {
        fmt::print(
            "\"{}\" {}\n",
            styled(ctx.arguments, fmt::emphasis::bold),
            styled("is not a valid directory."sv, fg(fmt::color::yellow))
        );
        return;
    }

    ranges::for_each(fs::directory_iterator{*directory}, print_directory_entry);
}

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
    };

    // c++23 use and_then
    ctx.current_dir = std::move(*new_path);
}

static auto text_data_chain = std::forward_list<std::ostringstream>{};

void
load(command_context ctx)
{
    static auto text_data = std::ostringstream{};

    auto const path = build_location_path(ctx.current_dir, fs::path{ctx.arguments});

    // insert file content into text_data_chain
    {
        auto read_text = bm::read_file(path);
        if (!read_text)
        {
            // print a message
            fmt::print(
                "{} \"{}\"\n",
                styled("Could not read from"sv, fg(fmt::color::yellow)),
                styled(ctx.arguments, fmt::emphasis::bold)
            );
            return;
        }
        text_data_chain.emplace_front(std::move(*read_text));
    }

    auto const& text = text_data_chain.front();

    auto const bookmark_vector = bm::build_bookmark_vector(text.view());

    if (bookmark_vector.empty())
    {
        fmt::print(
            "{} \"{}\" {}\n",
            styled("File"sv, fg(fmt::color::yellow)),
            styled(ctx.arguments, fmt::emphasis::bold),
            styled("did not contain any bookmarks."sv, fg(fmt::color::yellow))
        );

        // data that did not create any bookmarks can be freed
        // text becomes a dangling reference (does not matter to much thanks to return on next line)
        text_data_chain.pop_front();
        return;
    }

    auto const push_to_bookmarks = [&](bm::bookmark b)
    {
        ctx.bookmarks.emplace_back(b);
    };

    // in case the capacity is not enough,
    // (may save some allocations, in most cases will do nothing)
    ctx.bookmarks.reserve(
        std::max(ctx.bookmarks.size() + bookmark_vector.size(), bookmark_vector.capacity())
    );

    ranges::for_each(bookmark_vector, push_to_bookmarks);

    auto const mutate_context = [](command_context ctx)
    {
        ctx.arguments = ""sv;
        ctx.verbose   = false;
        return ctx;
    };

    if (ctx.verbose)
        fmt::print(
            "Added {} bookmarks from \"{}\"\n",
            styled(size(bookmark_vector), fmt::emphasis::bold),
            path.string()
        );

    ctx.cmap.at("reset"sv)(mutate_context(ctx));
}

} // namespace bm::commands::file
