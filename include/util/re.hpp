#pragma once
#include <optional>
#include <regex>
#include <utility>

namespace bm::util
{
template <typename T>
concept re_expression = requires(T t)
{
    std::string{t};
};

template <re_expression T>
[[nodiscard]] inline auto
compile_regex(T&& expr)
{
    try
    {
        auto re = std::regex{std::string{std::forward<T>(expr)}};
        return std::optional{re};
    }
    catch (std::regex_error const&)
    {
        return std::optional<std::regex>{std::nullopt};
    }
}
} // namespace bm::util
