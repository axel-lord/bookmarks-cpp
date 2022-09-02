#include "bookmarks/line_manipulation.hpp"
#include "util/definitions.hpp"

#include <fmt/format.h>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

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

    auto out_filename = fmt::format("{}.2.txt", argv[1]);
    // auto out_filename = std::format("{}.2.txt", argv[1]);

    auto transform_iter = [](std::string_view v
                          ) -> std::optional<std::string_view>
    {
        auto b = std::size(bm::util::URL_TAG);
        auto f = v.find(bm::util::INFO_TAG, b);
        if (b > std::size(v)) [[unlikely]]
        {
            std::cout << "[" << v << "]\n";
            return {};
        }

        std::string_view url_v;
        if (f == v.npos) [[unlikely]]
            url_v = v.substr(b);
        else
            url_v = v.substr(b, f - b);

        while (!url_v.empty() && url_v.back() == ' ')
            url_v.remove_suffix(1);
        while (!url_v.empty() && url_v.front() == ' ')
            url_v.remove_prefix(1);

        return url_v;
    };

    [[maybe_unused]] auto lines = bm::split_by_linebreak(text->view());

    return EXIT_VALUE::SUCCESS;
}
