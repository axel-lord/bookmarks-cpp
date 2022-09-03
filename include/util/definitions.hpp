#pragma once
#include <cstddef>
#include <string_view>

namespace bm::util
{
constexpr auto URL_TAG  = ::std::string_view{"<url>"};
constexpr auto INFO_TAG = ::std::string_view{"<info>"};
constexpr auto TAGS_TAG = ::std::string_view{"<tag>"};

constexpr auto BOOKMARKS_BEGIN = ::std::string_view{"#UNSORTED_BEGIN"};
constexpr auto BOOKMARKS_END   = ::std::string_view{"#UNSORTED_END"};
} // namespace bm::util

namespace bm::util::EXIT_VALUE
{
constexpr auto SUCCESS              = 0;
constexpr auto USAGE_FAILURE        = 1;
constexpr auto FILE_READ_FAILURE    = 2;
constexpr auto FILE_CONTENT_INVALID = 3;
}; // namespace bm::util::EXIT_VALUE

namespace bm::util::literals
{
[[nodiscard]] consteval ::std::size_t operator""_z(unsigned long long v)
{
    return v;
}
} // namespace bm::util::literals