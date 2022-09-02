#pragma once

#include "../util/definitions.hpp"
#include "../util/string_view_functions.hpp"
#include "./bookmark.hpp"

#include <optional>
#include <string_view>
#include <vector>

namespace bm
{
[[nodiscard]] inline auto
split_by_linebreak(std::string_view in)
{
    using namespace std::literals::string_view_literals;

    auto build_store = std::vector<decltype(in)>{};
    while (in.size())
    {
        in = bm::util::left_trim(in, "\n"sv);
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

[[nodiscard]] inline auto
line_to_bookmark([[maybe_unused]] std::string_view line) -> std::optional<bookmark>
{
    auto url_tag_pos = line.find(util::URL_TAG);
    if (url_tag_pos == line.npos)
        return std::nullopt;
}
} // namespace bm