#pragma once

#include "../util/string_view_functions.hpp"
#include "./bookmark.hpp"
#include "./structure_constants.hpp"

#include <optional>
#include <string_view>
#include <vector>

namespace bm
{

/*! Split text into a container of it's lines.
* 
* Empty lines are ignored.
* 
* @param in text to split.
* 
* @return a container of text lines, it may be empty or contain only one item.
*/
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

/*! Create a bookmark from a line of text.
* 
* @param line some text to parse into a bookmark.
* 
* @return an optional bookmark, it has a value depending on parse success.
*/
[[nodiscard]] inline auto
line_to_bookmark(const std::string_view line) -> std::optional<bookmark>
{
    auto url_tag_pos = line.find(constants::URL_TAG);
    if (url_tag_pos == line.npos)
        return std::nullopt;
    auto url_tag_end = url_tag_pos + size(constants::URL_TAG);

    auto info_tag_pos = line.find(constants::INFO_TAG, url_tag_end);
    if (info_tag_pos == line.npos)
        return std::nullopt;
    auto info_tag_end = info_tag_pos + size(constants::INFO_TAG);

    auto tags_tag_pos = line.find(constants::TAGS_TAG, info_tag_end);
    if (tags_tag_pos == line.npos)
        return std::nullopt;
    auto tags_tag_end = tags_tag_pos + size(constants::TAGS_TAG);

    auto get_substring = [line](auto lower, auto upper)
    {
        return util::trim(util::indice_substring(line, lower, upper), "\n\t ");
    };

    return bm::bookmark{
        get_substring(url_tag_end, info_tag_pos),
        get_substring(info_tag_end, tags_tag_pos),
        get_substring(tags_tag_end, size(line))};
}

} // namespace bm