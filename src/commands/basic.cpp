#include "bookmarks/command_context.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "util/literal_suffixes.hpp"
#include "util/string_view_functions.hpp"

#include <fmt/color.h>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

namespace bm::commands
{

using namespace bm::util::literals;

constexpr auto DEFAULT_SHOW_ARGS = std::pair{0_z, 25_z};

void
show(command_context ctx)
{
    namespace views      = ranges::views;
    auto const arguments = bm::split_by_delimiter(ctx.arguments, ' ');

    auto const parse_from_and_amount = [&]()
    {
        auto const parse_arg_n = [&](auto const n, auto const default_value) -> std::size_t
        {
            auto const arg = bm::util::to_number<unsigned int>(arguments[n]);
            if (!arg)
            {
                fmt::print(
                    fg(fmt::color::yellow),
                    "Could not convert \"{}\" to an integer defaulting to \"{}\".\n",
                    arguments[n],
                    default_value
                );
                return default_value;
            }
            return *arg;
        };

        switch (size(arguments))
        {
        case 0:
            return DEFAULT_SHOW_ARGS;
        case 1:
            return std::pair{parse_arg_n(0_z, DEFAULT_SHOW_ARGS.first), DEFAULT_SHOW_ARGS.second};
        case 2:
            return std::pair{
                parse_arg_n(0_z, DEFAULT_SHOW_ARGS.first),
                parse_arg_n(1_z, DEFAULT_SHOW_ARGS.second)};
        default:
            fmt::print(
                fg(fmt::color::yellow),
                "Too many arguments passed ({}) defaulting to \"{}\", \"{}\".\n",
                size(arguments),
                DEFAULT_SHOW_ARGS.first,
                DEFAULT_SHOW_ARGS.second
            );
            return DEFAULT_SHOW_ARGS;
        };
    };

    auto const [from, amount] = parse_from_and_amount();

    if (from >= size(ctx.current))
    {
        fmt::print(fg(fmt::color::yellow), "There are no bookmarks within given range.\n");
        return;
    }

    auto const print_bookmark = [&](bm::bookmark b)
    {
        fmt::print("{}\n", b);
    };

    fmt::print(
        "Showing {} to {}\n",
        fmt::styled(from, fmt::emphasis::bold),
        fmt::styled(from + amount, fmt::emphasis::bold)
    );

    ranges::for_each(ctx.current.subspan(from) | views::take(amount), print_bookmark);
}
void
count(command_context ctx)
{
    fmt::print("{} bookmarks.\n", styled(size(ctx.current), fmt::emphasis::bold));
}

void
reset(command_context ctx)
{
    ctx.current = std::span{ctx.bookmarks};
    fmt::print("All bookmarks shown.\n");
}
} // namespace bm::commands
