#include "bookmarks/bookmark.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "util/definitions.hpp"

#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <sstream>
#include <vector>
#include <spdlog/spdlog.h>

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

int
main(int argc, char** argv)
{
    if (argc != 2)
    {
        fmt::print("Usage: {} [FILE]\n", *argv);
        return EXIT_VALUE::USAGE_FAILURE;
    }

    auto text = read_bookmark_file(argv[1]);
    if (!text)
    {
        fmt::print("Could not open \"{}\"\n", argv[1]);
        return EXIT_VALUE::FILE_READ_FAILURE;
    }

    auto text_view = text->view();

    auto out_filename = fmt::format("{}.2.txt", argv[1]);
    auto lines        = bm::split_by_linebreak(text->view());

    auto is_not_bookmark_begin = [](auto line)
    {
        return line != bm::util::BOOKMARKS_BEGIN;
    };

    auto is_not_bookmark_end = [](auto line)
    {
        return line != bm::util::BOOKMARKS_END;
    };

    auto bookmark_lines = lines | std::views::drop_while(is_not_bookmark_begin) |
                          std::views::drop(1) | std::views::take_while(is_not_bookmark_end);

    for (auto i = 0_z; auto&& l : bookmark_lines | std::views::take(3))
    {
        auto v = bm::line_to_bookmark(l);
        if (v)
            spdlog::debug("Line: {}, {}\n", i, *v);
        else
            spdlog::warn("Could not parse line too bookmark: [{}]", l);
        ++i;
    }

    fmt::print("Bookmark Test: {}\n", bm::bookmark{"https://", "Generic", "nice <,> man"});

    return EXIT_VALUE::SUCCESS;
}
