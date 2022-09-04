#include "bookmarks/bookmark.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "bookmarks/structure_constants.hpp"
#include "util/exit_values.hpp"
#include "util/literal_suffixes.hpp"

#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <vector>
#include <span>

using namespace std::literals;
using namespace bm::util::literals;

namespace EXIT_VALUE = bm::util::EXIT_VALUE;

[[nodiscard]] std::optional<std::ostringstream>
read_bookmark_file(const char* filename)
{
    auto text = std::ostringstream{};
    if (auto file = std::ifstream{filename})
        text << file.rdbuf();
    else [[unlikely]]
        return std::nullopt;
    return text;
}

[[nodiscard]] inline auto
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

    auto text = read_bookmark_file(argv[1]);
    if (!text)
    {
        fmt::print("Could not open \"{}\"\n", argv[1]);
        return EXIT_VALUE::FILE_READ_FAILURE;
    }

    auto opt_logger = create_logger(fmt::format("{}.log.txt", argv[0]));
    if (!opt_logger)
        return EXIT_VALUE::LOG_CREATION_FAILURE;
    auto& logger = **opt_logger;
    spdlog::set_level(spdlog::level::debug);

    auto const lines = bm::split_by_linebreak(text->view());
    auto const line_view = std::span(lines);

    auto const is_not_bookmark_begin = [](auto const line)
    {
        return line != bm::constants::BOOKMARKS_BEGIN;
    };
    auto const is_not_bookmark_end = [](auto const line)
    {
        return line != bm::constants::BOOKMARKS_END;
    };

    auto bookmark_lines = line_view | std::views::drop_while(is_not_bookmark_begin) | std::views::drop(1) |
                          std::views::take_while(is_not_bookmark_end);

    for (auto i = 0_z; auto&& l : bookmark_lines | std::views::take(50))
    {
        auto v = bm::line_to_bookmark(l);
        if (v)
            logger.debug("Item: {}, {}\n", i, *v);
        else
            logger.warn("Could not parse line too bookmark: [{}]", l);
        ++i;
    }

    return EXIT_VALUE::SUCCESS;
}
