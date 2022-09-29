#include "./fs_misc.hpp"
#include "bookmarks/command_context.hpp"
#include "bookmarks/data_registry.hpp"
#include "bookmarks/line_manipulation.hpp"

using namespace std::literals::string_view_literals;
namespace fs = std::filesystem;

namespace bm::commands::file
{

void
load(command_context ctx)
{
    auto const path = build_location_path(ctx.current_dir, fs::path{ctx.arguments});

    // insert file content into text_data_chain
    auto const store_file_content = [&]() -> std::optional<std::pair<std::string_view, std::size_t>>
    {
        auto read_text = bm::read_file(path);
        if (!read_text)
        {
            return std::nullopt;
        }

        auto       text_storage = std::make_shared<std::ostringstream>(std::move(*read_text));
        auto const key          = ctx.registry.register_owner(text_storage);
        return std::pair{text_storage->view(), key};
    };

    auto const store_result = store_file_content();
    if (!store_result)
    {
        fmt::print(
            "{} \"{}\"\n",
            styled("Could not read from"sv, fg(fmt::color::yellow)),
            styled(ctx.arguments, fmt::emphasis::bold)
        );
        return;
    }

    auto const& [text, key]    = *store_result;
    auto const bookmark_vector = bm::build_bookmark_vector(text);

    if (bookmark_vector.empty())
    {
        fmt::print(
            "{} \"{}\" {}\n",
            styled("File"sv, fg(fmt::color::yellow)),
            styled(ctx.arguments, fmt::emphasis::bold),
            styled("did not contain any bookmarks."sv, fg(fmt::color::yellow))
        );

        // data that did not create any bookmarks can be freed
        // text becomes a dangling reference (does not matter to much thanks to return on next line)
        ctx.registry.unregister_owner(key);
        return;
    }

    auto const push_to_bookmarks = [&](bm::bookmark b)
    {
        ctx.bookmarks.emplace_back(b);
    };

    // in case the capacity is not enough,
    // (may save some allocations, in most cases will do nothing)
    ctx.bookmarks.reserve(
        std::max(ctx.bookmarks.size() + bookmark_vector.size(), bookmark_vector.capacity())
    );

    ranges::for_each(bookmark_vector, push_to_bookmarks);

    auto const mutate_context = [](command_context context)
    {
        context.arguments = ""sv;
        context.verbose   = false;
        return context;
    };

    if (ctx.verbose)
        fmt::print(
            "Added {} bookmarks from \"{}\"\n",
            styled(size(bookmark_vector), fmt::emphasis::bold),
            path.string()
        );

    ctx.cmap.at("reset"sv)(mutate_context(ctx));
}
} // namespace bm::commands::file
