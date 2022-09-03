#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <string_view>

namespace bm
{

struct bookmark
{
    ::std::string_view url;
    ::std::string_view description;
    ::std::string_view tags;
};

[[nodiscard]] inline auto& operator<<(::std::ostream& os, const bookmark& b);

} // namespace bm

template <> struct ::fmt::formatter<bm::bookmark>
{
    constexpr auto parse(auto& ctx)
    {
        return ctx.begin();
    }

    auto format(bm::bookmark const& b, auto& ctx)
    {
        return fmt::format_to(ctx.out(), "[{}] \"{}\" ({})", b.url, b.description, b.tags);
    }
};

[[nodiscard]] inline auto&
bm::operator<<(::std::ostream& os, const bookmark& b)
{
    fmt::print(os, "{}", b);
    return os;
}
