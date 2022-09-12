#include "bookmarks/command_context.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "util/literal_suffixes.hpp"
#include "util/string_view_functions.hpp"

#include <range/v3/algorithm/for_each.hpp>

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
                    "Could not convert \"{}\" to an integer defaulting to \"{}\".\n", arguments[n],
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
                "Too many arguments passed ({}) defaulting to \"{}\", \"{}\".\n", size(arguments),
                DEFAULT_SHOW_ARGS.first, DEFAULT_SHOW_ARGS.second
            );
            return DEFAULT_SHOW_ARGS;
        };
    };

    auto const [from, amount] = parse_from_and_amount();

    for (auto const& b : ctx.current | views::drop(from) | views::take(amount))
    {
        fmt::print("{}\n", b);
    }
}
void
count(command_context ctx)
{
    fmt::print("{} bookmarks.\n", size(ctx.current));
}

void
reset(command_context ctx)
{
    ctx.current = std::span{ctx.bookmarks};
}
} // namespace bm::commands
