#include "bookmarks/command_context.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "util/literal_suffixes.hpp"
#include "util/re.hpp"

#include <range/v3/algorithm/for_each.hpp>
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
        fmt::print("filter needs arguments, none were passed.\n");
        return;
    }

    fmt::print("/{}/\n", ctx.arguments);

    auto const arguments = bm::split_by_delimiter(ctx.arguments, ' ');
    ctx.bookmark_buffer.clear();

    auto const search_re = bm::util::compile_regex(ctx.arguments);
    if (!search_re)
    {
        fmt::print("Could not compile regex \"{}\".\n", ctx.arguments);
        return;
    }

    auto const contains_arguments = [&](bm::bookmark b)
    {
        return bool{
            std::regex_search(begin(b.url), end(b.url), *search_re) ||
            std::regex_search(begin(b.description), end(b.description), *search_re)};
    };

    auto const add_to_buffer = [&](bm::bookmark b)
    {
        ctx.bookmark_buffer.push_back(b);
    };

    ranges::for_each(ctx.bookmarks | views::filter(contains_arguments), add_to_buffer);
    ctx.current = std::span{ctx.bookmark_buffer};
}

void
fuzzy(command_context ctx)
{
    using namespace std::literals::string_view_literals;
    using namespace bm::util::literals;

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