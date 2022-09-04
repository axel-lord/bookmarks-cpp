#pragma once

#include <string_view>

namespace bm::constants
{
constexpr auto URL_TAG  = ::std::string_view{"<url>"};
constexpr auto INFO_TAG = ::std::string_view{"<info>"};
constexpr auto TAGS_TAG = ::std::string_view{"<tag>"};

constexpr auto BOOKMARKS_BEGIN = ::std::string_view{"#UNSORTED_BEGIN"};
constexpr auto BOOKMARKS_END   = ::std::string_view{"#UNSORTED_END"};
} // namespace bm::util