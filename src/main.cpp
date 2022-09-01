#include "util/definitions.hpp"

#include <vector>
#include <optional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <format>

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

[[nodiscard]] auto
split_by_lines(std::string_view in)
{
    auto build_store = std::vector<decltype(in)>{};
    while (in.size())
    {
        while (in.size() && in[0] == '\n')
            in.remove_prefix(1);
        if (!in.size())
            break;

        auto lf_index = in.find('\n');
        if (lf_index == in.npos)
        {
            build_store.emplace_back(in);
            break;
        }

        build_store.emplace_back(in.substr(0, lf_index));
        in.remove_prefix(lf_index);
    }
    return build_store;
}

int
main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << *argv << " [FILE]\n";
        return EXIT_VALUE::USAGE_FAILURE;
    }

    auto text = read_bookmark_file(argv[1]);
    if (!text)
    {
        std::cout << "Could not open \"" << argv[1] << "\"\n";
        return EXIT_VALUE::FILE_READ_FAILURE;
    }

    auto out_filename = std::format("{}.2.txt", argv[1]);

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

    return EXIT_VALUE::SUCCESS;
}
