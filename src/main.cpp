#include "bookmarks/command_arguments.hpp"
#include "bookmarks/command_context.hpp"
#include "bookmarks/commands.hpp"
#include "bookmarks/data_registry.hpp"
#include "util/exit_values.hpp"
#include "util/literal_suffixes.hpp"

#include <fmt/color.h>
#include <iostream>
#include <optional>
#include <span>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

using namespace std::literals;
using namespace bm::util::literals;

namespace EXIT_VALUE = bm::util::EXIT_VALUE;

namespace bm
{
[[nodiscard]] std::vector<bookmark> build_bookmark_vector(std::string_view const bookmark_view);
[[nodiscard]] std::optional<std::ostringstream> read_file(std::filesystem::path const& filepath);
} // namespace bm

[[nodiscard]] static inline auto
create_logger(const std::string& filename) noexcept
{
    auto internal_creation = [&]
    {
        return spdlog::basic_logger_mt("file_log", filename);
    };
    try
    {
        auto logger = internal_creation();
        return std::optional{logger};
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        fmt::print("Log initialization failed: {}\n", ex.what());
    }
    return std::optional<decltype(internal_creation())>{std::nullopt};
}

using bookmark_container = decltype(bm::build_bookmark_vector(""sv));

static inline auto
get_input()
{
    auto buffer = std::string{};
    getline(std::cin, buffer);
    return buffer;
}

static inline auto
run_app(std::optional<std::string_view> const bookmark_view)
{
    auto bookmarks       = bm::build_bookmark_vector(bookmark_view.value_or(""sv));
    auto bookmark_buffer = std::vector<bm::bookmark>{};
    auto current         = std::span{bookmarks};
    auto current_dir     = std::filesystem::current_path();
    auto registry        = bm::data::registry{};

    auto const cmap = bm::commands::command_map{
        {"show"sv, bm::commands::show},
        {"filter"sv, bm::commands::filter},
        {"count"sv, bm::commands::count},
        {"reset"sv, bm::commands::reset},
        {"fuzzy"sv, bm::commands::fuzzy},
        {"regex"sv, bm::commands::regex},
        {"fs"sv, bm::commands::make_fs_command()},
    };

    bookmark_buffer.reserve(bookmarks.capacity());

    while (true)
    {
        fmt::print(
            "{0} [{1}]\n> ",
            fmt::styled("Enter Command", fg(fmt::color::light_sea_green)),
            current_dir.string()
        );

        auto const buffer = get_input();

        if (buffer.empty()) // nothing entered
        {
            fmt::print(fg(fmt::color::yellow), "Please enter a command.\n");
            continue;
        }

        auto const [command, arguments] = bm::commands::parse_arguments(buffer);

        if (command == "exit"sv) // special command to ensure exit is possible
            return;

        if (!cmap.contains(command))
        {
            fmt::print(
                "{} \"{}\"\n",
                styled("Could not find command", fg(fmt::color::yellow)),
                styled(command, fmt::emphasis::bold)
            );
            continue;
        }

        cmap.at(command)(bm::commands::command_context{
            .arguments       = arguments,
            .bookmarks       = bookmarks,
            .bookmark_buffer = bookmark_buffer,
            .current         = current,
            .cmap            = cmap,
            .current_dir     = current_dir,
            .registry        = registry,
        });
    }
}

static inline auto
run_app()
{
    return run_app(std::nullopt);
}

int
main(int const argc, char const* const* const argv)
{
    if (!argc)
    {
        fmt::print(
            "{}\n", "Program (Bookmark Manager) executed in a manner that is not supported!"sv
        );
        return EXIT_VALUE::UNSUPPORTED_EXECUTION;
    }

    if (argc > 2)
    {
        fmt::print("Usage: {} [FILE]\n", argv[0]);
        return EXIT_VALUE::USAGE_FAILURE;
    }

    auto opt_logger = create_logger(fmt::format("{}.log.txt", argv[0]));
    if (!opt_logger)
        return EXIT_VALUE::LOG_CREATION_FAILURE;
    (*opt_logger)->set_level(spdlog::level::trace);
    spdlog::set_default_logger(*opt_logger);

    if (argc == 2)
    {
        auto const text = bm::read_file(argv[1]);
        if (!text)
        {
            fmt::print("Could not open \"{}\"\n", argv[1]);
            return EXIT_VALUE::FILE_READ_FAILURE;
        }

        run_app(text->view());
    }

    run_app();

    return EXIT_VALUE::SUCCESS;
}
