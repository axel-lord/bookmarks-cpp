#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <string_view>

namespace bm
{

//! A struct for representing bookmarks.
struct bookmark
{
    std::string_view url; //!< URL the bookmark points to.
    std::string_view description; //!< Description/Displayname of the bookmark.
    std::string_view tags; //!< list of tags for the bookmark separated by "<,>".

    friend auto operator<=>(const bookmark&, const bookmark&) = default;
};

[[nodiscard]] inline auto& operator<<(std::ostream& os, const bookmark& b);

} // namespace bm

template <> struct fmt::formatter<bm::bookmark>
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
bm::operator<<(std::ostream& os, const bookmark& b)
{
    fmt::print(os, "{}", b);
    return os;
}
