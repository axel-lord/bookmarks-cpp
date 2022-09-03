#pragma once

#include <algorithm>
#include <concepts>
#include <string_view>

namespace bm::util
{

[[nodiscard]] inline auto
left_trim(const std::string_view in, const std::string_view trim_set)
{
    return in.substr(std::min(in.find_first_not_of(trim_set), in.size()));
}

[[nodiscard]] inline auto
right_trim(const std::string_view in, const std::string_view trim_set)
{
    return in.substr(0, std::min(in.find_last_not_of(trim_set) + 1, in.size()));
}

[[nodiscard]] inline auto
trim(const std::string_view in, const std::string_view trim_set)
{
    return left_trim(right_trim(in, trim_set), trim_set);
}

[[nodiscard]] inline auto
indice_substring(const std::string_view in, std::size_t lower, std::size_t upper)
{
    return in.substr(lower, upper - lower);
}

} // namespace bm::util