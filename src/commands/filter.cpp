#include "bookmarks/command_context.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "util/literal_suffixes.hpp"
#include "util/re.hpp"
#include "util/string_view_functions.hpp"

#include <fmt/color.h>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <sstream>

namespace bm::commands
{

void
regex(command_context ctx)
{
    namespace views = ranges::views;
    using namespace std::literals::string_view_literals;

    if (ctx.arguments.empty())
    {
        fmt::print(fg(fmt::color::yellow), "Usage: regex PATTERN\n");
        return;
    }

    if (ctx.verbose)
        fmt::print("Bookmarks filtered using regex /{}/\n", styled(ctx.arguments, fmt::emphasis::bold));

    auto const arguments = bm::split_by_delimiter(ctx.arguments, ' ');
    ctx.bookmark_buffer.clear();

    auto const search_re = bm::util::compile_regex(ctx.arguments);
    if (!search_re)
    {
        fmt::print(fg(fmt::color::yellow), "Could not compile regex /{}/.\n", ctx.arguments);
        return;
    }

    auto const contains_arguments = [&](bm::bookmark b)
    {
        return bool{
            std::regex_search(begin(b.url), end(b.url), *search_re) ||
            std::regex_search(begin(b.description), end(b.description), *search_re)};
    };

    auto const add_matching_to_buffer = [&](bm::bookmark b)
    {
        if (contains_arguments(b))
            ctx.bookmark_buffer.push_back(b);
    };

    ranges::for_each(ctx.bookmarks, add_matching_to_buffer);
    ctx.current = std::span{ctx.bookmark_buffer};
}

void
fuzzy(command_context ctx)
{
    using namespace std::literals::string_view_literals;
    using namespace bm::util::literals;

    if (ctx.arguments.empty())
    {
        fmt::print(fg(fmt::color::yellow), "Usage: fuzzy STRING\n");
    }

    auto builder = std::ostringstream{};

    auto const process_character = [&](char character)
    {
        auto const u = util::to_upper(character);
        auto const l = util::to_lower(character);

        if (u == l)
            builder << character << ".*"sv;
        else
            builder << '[' << u << l << ']' << ".*"sv;
    };

    ranges::for_each(ctx.arguments, process_character);

    ctx.arguments = builder.view().substr(0, std::max(size(builder.view()) - 2, 0_z));
    regex(ctx);
}

void
filter(command_context ctx)
{
    auto builder = std::ostringstream{};

    if (ctx.arguments.empty())
    {
        fmt::print(fg(fmt::color::yellow), "Usage: filter STRING\n");
    }

    auto const process_character = [&](char character)
    {
        auto const u = util::to_upper(character);
        auto const l = util::to_lower(character);

        if (u == l)
            builder << character;
        else
            builder << '[' << u << l << ']';
    };

    ranges::for_each(ctx.arguments, process_character);

    ctx.arguments = builder.view();
    regex(ctx);
}

} // namespace bm::commands
