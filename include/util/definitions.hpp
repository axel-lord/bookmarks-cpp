#pragma once
#include <cstddef>
#include <string_view>

namespace bm::util
{
constexpr auto URL_TAG  = std::string_view{"<url>"};
constexpr auto INFO_TAG = std::string_view{"<info>"};
} // namespace bm::util

namespace bm::util::EXIT_VALUE
{
constexpr int SUCCESS              = 0;
constexpr int USAGE_FAILURE        = 1;
constexpr int FILE_READ_FAILURE    = 2;
constexpr int FILE_CONTENT_INVALID = 3;
}; // namespace bm::util::EXIT_VALUE

namespace bm::util::literals
{
[[nodiscard]] consteval std::size_t operator""_z(unsigned long long v)
{
    return v;
}
} // namespace bm::util::literals