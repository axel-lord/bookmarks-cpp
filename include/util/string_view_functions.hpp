#pragma once

#include <algorithm>
#include <concepts>
#include <string_view>

namespace bm::util
{

/*! Get a substring of a given piece of text based on indices.
 *
 * @param in text to get substring of.
 * @param lower index of first character of desired substring.
 * @param upper index of character one past last desired character in substring.
 *
 * @return a substring of given text with it's bounds matched to given indices.
 */
[[nodiscard]] inline auto
indice_substring(const std::string_view in, std::size_t lower, std::size_t upper)
{
    return in.substr(lower, upper - lower);
}

/*! Trim given characters from the start of a piece of text.
 *
 * @param in text from which to trim.
 * @param trim_set sequence of characters to trim.
 *
 * @return a substring of the given text with leading characters in trim_set removed.
 */
[[nodiscard]] inline auto
left_trim(const std::string_view in, const std::string_view trim_set)
{
    return in.substr(std::min(in.find_first_not_of(trim_set), in.size()));
}

/*! Trim given characters from the end of a piece of text.
 *
 * @param in text from which to trim.
 * @param trim_set sequence of characters to trim.
 *
 * @return a substring of the given text with trailing characters in trim_set removed.
 */
[[nodiscard]] inline auto
right_trim(const std::string_view in, const std::string_view trim_set)
{
    return in.substr(0, std::min(in.find_last_not_of(trim_set) + 1, in.size()));
}

/*! Trim given characters from the start and end of a piece of text.
 *
 * @param in text from which to trim.
 * @param trim_set sequence of characters to trim.
 *
 * @return a substring of the given text with leading and trailing characters in trim_set removed.
 */
[[nodiscard]] inline auto
trim(const std::string_view in, const std::string_view trim_set)
{
    return left_trim(right_trim(in, trim_set), trim_set);
}

} // namespace bm::util