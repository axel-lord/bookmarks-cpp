#include "bookmarks/bookmark.hpp"
#include "bookmarks/command_arguments.hpp"
#include "bookmarks/command_context.hpp"
#include "bookmarks/commands.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "bookmarks/structure_constants.hpp"
#include "util/exit_values.hpp"
#include "util/literal_suffixes.hpp"
#include "util/re.hpp"

#include <algorithm>
#include <cctype>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <range/v3/algorithm/for_each.hpp>
#include <regex>
#include <span>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string_view>
#include <unordered_map>
#include <utility>

using namespace std::literals;
using namespace bm::util::literals;

namespace EXIT_VALUE = bm::util::EXIT_VALUE;

[[nodiscard]] static inline auto
create_logger(const std::string& filename) noexcept
{
    auto internal_creation = [&]
    {
        return spdlog::basic_logger_mt("file_log", filename);
    };
    try
    {
        auto logger = internal_creation();
        return std::optional{logger};
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        fmt::print("Log initialization failed: {}\n", ex.what());
    }
    return std::optional<decltype(internal_creation())>{std::nullopt};
}

using bookmark_container = decltype(bm::build_bookmark_vector(""sv));

static inline auto
get_input()
{
    auto buffer = std::string{};
    getline(std::cin, buffer);
    return buffer;
}

static inline auto
run_app(std::string_view const bookmark_view)
{
    auto bookmarks       = bm::build_bookmark_vector(bookmark_view);
    auto bookmark_buffer = std::vector<bm::bookmark>{};
    auto current         = std::span{bookmarks};
    auto current_dir     = std::filesystem::absolute("./");

    auto const directory = [](bm::commands::command_context ctx)
    {
        fmt::print("{}\n", styled(ctx.current_dir.string(), fmt::emphasis::bold));
    };

    auto const cmap = bm::commands::command_map{
        {"show"sv, bm::commands::show},
        {"filter"sv, bm::commands::filter},
        {"count"sv, bm::commands::count},
        {"reset"sv, bm::commands::reset},
        {"fuzzy"sv, bm::commands::fuzzy},
        {"regex"sv, bm::commands::regex},
        {"fs-dir"sv, directory},
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
            continue;

        auto const [command, arguments] = bm::commands::parse_arguments(buffer);

        if (command == "exit"sv) // special command to ensure exit is possible
            break;

        if (!cmap.contains(command))
        {
            fmt::print(
                "{} \"{}\"\n",
                styled("Could not find command", fg(fmt::color::yellow)),
                styled(command, fmt::emphasis::bold)
            );
            continue;
        }

        cmap.at(command)({arguments, bookmarks, bookmark_buffer, current, cmap, current_dir});
    }
}

int
main(int const argc, char const* const* const argv)
{
    if (!argc)
    {
        fmt::print(
            "{}\n", "Program (Bookmark Manager) executed in a manner that is not supported!"sv
        );
        return EXIT_VALUE::UNSUPPORTED_EXECUTION;
    }

    if (argc != 2)
    {
        fmt::print("Usage: {} [FILE]\n", argv[0]);
        return EXIT_VALUE::USAGE_FAILURE;
    }

    auto opt_logger = create_logger(fmt::format("{}.log.txt", argv[0]));
    if (!opt_logger)
        return EXIT_VALUE::LOG_CREATION_FAILURE;
    (*opt_logger)->set_level(spdlog::level::trace);
    spdlog::set_default_logger(*opt_logger);

    auto const text = bm::read_file(argv[1]);
    if (!text)
    {
        fmt::print("Could not open \"{}\"\n", argv[1]);
        return EXIT_VALUE::FILE_READ_FAILURE;
    }

    run_app(text->view());

    return EXIT_VALUE::SUCCESS;
}
