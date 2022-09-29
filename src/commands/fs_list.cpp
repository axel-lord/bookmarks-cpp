#include "./fs_misc.hpp"
#include "bookmarks/command_context.hpp"

#include <filesystem>

using namespace std::literals::string_view_literals;
namespace fs = std::filesystem;

namespace bm::commands::file
{

void
list(command_context ctx)
{
    // if (!ctx.arguments.empty())
    //     fmt::print(fg(fmt::color::yellow), "Usage: fs list");

    auto const get_directory_to_list = [&]()
    {
        if (ctx.arguments.empty())
            return ctx.current_dir;
        else
            return build_location_path(ctx.current_dir, ctx.arguments);
    };

    auto const print_directory_entry = [](fs::directory_entry const& entry)
    {
        constexpr auto format_string = "{}\n";

        auto const& path        = entry.path();
        auto const  path_string = path.filename().string();

        auto file_err = std::error_code{};
        auto dir_err  = std::error_code{};

        if (fs::is_regular_file(path, file_err))
        {
            fmt::print(format_string, path_string);
        }
        else if (file_err)
        {
            fmt::print(
                "{} [error code: {}]\n",
                styled(path_string, fg(fmt::color::hot_pink)),
                file_err.value()
            );
        }
        else if (fs::is_directory(path, dir_err))
        {
            fmt::print(
                fmt::emphasis::bold | fg(fmt::color::lime_green), format_string, path_string
            );
        }
        else if (dir_err)
        {
            fmt::print(
                "{} [error code: {}]\n",
                styled(path_string, fg(fmt::color::orange)),
                dir_err.value()
            );
        }
        else
        {
            fmt::print(fmt::emphasis::italic, format_string, path_string);
        }
    };

    auto const directory = ensure_directory(get_directory_to_list());
    if (!directory)
    {
        fmt::print(
            "\"{}\" {}\n",
            styled(ctx.arguments, fmt::emphasis::bold),
            styled("is not a valid directory."sv, fg(fmt::color::yellow))
        );
        return;
    }

    ranges::for_each(fs::directory_iterator{*directory}, print_directory_entry);
}
} // namespace bm::commands::file
