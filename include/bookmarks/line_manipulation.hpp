#pragma once

#include "../util/string_view_functions.hpp"
#include "./bookmark.hpp"
#include "./structure_constants.hpp"

#include <charconv>
#include <optional>
#include <range/v3/view.hpp>
#include <range/v3/view/transform.hpp>
#include <span>
#include <spdlog/spdlog.h>
#include <string_view>
#include <vector>

namespace bm
{

//! Initial size of the vector returned by split_by_linebreak.
constexpr std::size_t INITIAL_LINE_VECTOR_SIZE = 8192;

/*! Read the contents of a file to memory.
 *
 * @param filename path to the file to be read.
 *
 * @return an optional stringstream depending on wether the file could be read from or not.
 */
[[nodiscard]] inline auto
read_file(const char* filename) -> std::optional<std::ostringstream>
{
    auto text = std::optional<std::ostringstream>{};

    if (auto file = std::ifstream{filename}) [[likely]]
    {
        text.emplace();
        *text << file.rdbuf();
    }

    return text;
}

/*! Split text into a container.
 *
 * multiples of the character are treated as one.
 * leading or trailing are ignored.
 *
 * @param in text to split.
 * @param delim character to split on.
 *
 * @return a container of text lines, it may be empty or contain only one item.
 */
[[nodiscard]] inline auto
split_by_delimiter(std::string_view in, char const delim, std::size_t const reserved_size = 16)
{
    using namespace std::literals::string_view_literals;

    auto build_store = std::vector<decltype(in)>{};
    build_store.reserve(reserved_size);
    while (in.size())
    {
        in = bm::util::left_trim(in, std::string_view{&delim, 1});
        if (!in.size())
            break;

        auto lf_index = in.find(delim);
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
    return split_by_delimiter(in, '\n', INITIAL_LINE_VECTOR_SIZE);
}

/*! Create a bookmark from a line of text.
 *
 * @param line some text to parse into a bookmark.
 *
 * @return an optional bookmark, it has a value depending on parse success.
 */
[[nodiscard]] inline auto
to_bookmark(const std::string_view line) -> std::optional<bookmark>
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
        get_substring(url_tag_end, info_tag_pos), get_substring(info_tag_end, tags_tag_pos),
        get_substring(tags_tag_end, size(line))};
}

/*! Create a line of text from a bookmark.
 *
 * Creating a bookmark from the line should be avoided
 * if the lifetime of the line cannot be guaranteed.
 *
 * @param bookmark bookmark to build line from.
 *
 * @return a line containing a serialized bookmark.
 */
[[nodiscard]] auto inline to_line(bm::bookmark bookmark)
{
    return fmt::format("<url>{}<info>{}<tag>{}", bookmark.url, bookmark.description, bookmark.tags);
}

/*! Build a bookmark container from a text containing bookmarks.
 *
 * @param logger logger to use.
 * @param bookmark_view text containing bookmarks.
 *
 * @return a container of bookmarks created from given text.
 */
[[nodiscard]] static inline auto
build_bookmark_vector(std::string_view const bookmark_view)
{
    namespace views = ranges::views;

    auto const lines = bm::split_by_linebreak(bookmark_view);

    auto const is_not_bookmark_begin = [](auto const line)
    {
        return line != bm::constants::BOOKMARKS_BEGIN;
    };
    auto const is_not_bookmark_end = [](auto const line)
    {
        return line != bm::constants::BOOKMARKS_END;
    };

    auto const has_value = [](auto const optional_bookmark)
    {
        return !!optional_bookmark;
    };

    auto const dereference = [](auto const pointer_like)
    {
        return *pointer_like;
    };

    auto bookmarks = lines | views::drop_while(is_not_bookmark_begin) | views::drop(1) |
                     views::take_while(is_not_bookmark_end) | views::transform(bm::to_bookmark) |
                     views::filter(has_value) | views::transform(dereference);

    // allows for reserving capacity.
    auto bookmark_vector = std::vector<bm::bookmark>{};
    bookmark_vector.reserve(lines.capacity());

    for (auto const& b : bookmarks)
        bookmark_vector.push_back(b);

    return bookmark_vector;
}

} // namespace bm
