#include <cctype>
#include <charconv>
#include <optional>
#include <string_view>

namespace bm::util
{

[[nodiscard]] std::string_view
indice_substring(const std::string_view in, std::size_t lower, std::size_t upper)
{
    return in.substr(lower, upper - lower);
}
[[nodiscard]] std::string_view
left_trim(const std::string_view in, const std::string_view trim_set)
{
    return in.substr(std::min(in.find_first_not_of(trim_set), in.size()));
}
[[nodiscard]] std::string_view
right_trim(const std::string_view in, const std::string_view trim_set)
{
    return in.substr(0, std::min(in.find_last_not_of(trim_set) + 1, in.size()));
}
[[nodiscard]] std::string_view
trim(const std::string_view in, const std::string_view trim_set)
{
    return left_trim(right_trim(in, trim_set), trim_set);
}
} // namespace bm::util
