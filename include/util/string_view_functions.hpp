#pragma once

#include <algorithm>
#include <charconv>
#include <optional>
#include <string_view>

namespace bm::util
{

/*! Convert the contents of a string view to a number.
 *
 * @param view string to convert.
 *
 * @return a number or nullopt in case of failure.
 */
template <typename T>
[[nodiscard]] std::optional<T>
to_number(std::string_view view)
{
    if (view.empty())
        return std::nullopt;
    if (T value; std::from_chars(view.data(), view.data() + size(view), value).ec == std::errc{})
        return value;
    return std::nullopt;
}

/*! Convert a character to uppercase.
 *
 * @param chracter letter to convert.
 *
 * @return given letter but uppercase if possible else given letter.
 */
[[nodiscard]] constexpr char
to_upper(char c)
{
    //return static_cast<char>(std::toupper(static_cast<int>(character)));
	if (c < 'a' || c > 'z')
		return c;

	if constexpr ('A' > 'a')
		return c + ('A' - 'a');
	else
		return c - ('a' - 'A');
}

/*! Convert a character to lowercase.
 *
 * @param chracter letter to convert.
 *
 * @return given letter but lowercase if possible else given letter.
 */
[[nodiscard]] constexpr char
to_lower(char c)
{
    //return static_cast<char>(std::tolower(static_cast<int>(character)));
	if (c < 'A' || c > 'Z')
		return c;

	if constexpr ('A' > 'a')
		return c - ('A' - 'a');
	else
		return c + ('a' - 'A');
}


/*! Get a substring of a given piece of text based on indices.
 *
 * @param in text to get substring of.
 * @param lower index of first character of desired substring.
 * @param upper index of character one past last desired character in substring.
 *
 * @return a substring of given text with it's bounds matched to given indices.
 */
[[nodiscard]] std::string_view
indice_substring(const std::string_view in, std::size_t lower, std::size_t upper);

/*! Trim given characters from the start of a piece of text.
 *
 * @param in text from which to trim.
 * @param trim_set sequence of characters to trim.
 *
 * @return a substring of the given text with leading characters in trim_set removed.
 */
[[nodiscard]] std::string_view
left_trim(const std::string_view in, const std::string_view trim_set);

/*! Trim given characters from the end of a piece of text.
 *
 * @param in text from which to trim.
 * @param trim_set sequence of characters to trim.
 *
 * @return a substring of the given text with trailing characters in trim_set removed.
 */
[[nodiscard]] std::string_view
right_trim(const std::string_view in, const std::string_view trim_set);

/*! Trim given characters from the start and end of a piece of text.
 *
 * @param in text from which to trim.
 * @param trim_set sequence of characters to trim.
 *
 * @return a substring of the given text with leading and trailing characters in trim_set removed.
 */
[[nodiscard]] std::string_view
trim(const std::string_view in, const std::string_view trim_set);

} // namespace bm::util
