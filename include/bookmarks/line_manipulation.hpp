#pragma once

#include "bookmarks/bookmark.hpp"

#include <filesystem>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace bm
{

/*! Read the contents of a file to memory.
 *
 * @param filepath path to the file to be read.
 *
 * @return an optional stringstream depending on wether the file could be read from or not.
 */
[[nodiscard]] std::optional<std::ostringstream> read_file(std::filesystem::path const& filepath);

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
[[nodiscard]] std::vector<std::string_view> split_by_delimiter(
    std::string_view in, char const delim, std::size_t const reserved_size = 16
);

/*! Split text into a container of it's lines.
 *
 * Empty lines are ignored.
 *
 * @param in text to split.
 *
 * @return a container of text lines, it may be empty or contain only one item.
 */
[[nodiscard]] std::vector<std::string_view> split_by_linebreak(std::string_view in);

/*! Create a bookmark from a line of text.
 *
 * @param line some text to parse into a bookmark.
 *
 * @return an optional bookmark, it has a value depending on parse success.
 */
[[nodiscard]] std::optional<bookmark> to_bookmark(const std::string_view line);

/*! Create a line of text from a bookmark.
 *
 * Creating a bookmark from the line should be avoided
 * if the lifetime of the line cannot be guaranteed.
 *
 * @param bookmark bookmark to build line from.
 *
 * @return a line containing a serialized bookmark.
 */
[[nodiscard]] std::string to_line(bm::bookmark bookmark);

/*! Build a bookmark container from a text containing bookmarks.
 *
 * @param bookmark_view text containing bookmarks.
 *
 * @return a container of bookmarks created from given text.
 */
[[nodiscard]] std::vector<bookmark> build_bookmark_vector(std::string_view const bookmark_view);

} // namespace bm
