#include "bookmarks/bookmark.hpp"
#include "bookmarks/line_manipulation.hpp"
#include "bookmarks/structure_constants.hpp"
#include "util/exit_values.hpp"
#include "util/literal_suffixes.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <span>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>


using namespace std::literals;
using namespace bm::util::literals;

namespace EXIT_VALUE = bm::util::EXIT_VALUE;

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

static inline auto
run_app(std::string_view const bookmark_view)
{
    auto bookmarks = bm::build_bookmark_vector(bookmark_view);
    auto current   = std::span{bookmarks};

    auto command_map =
        std::unordered_map<std::string_view, std::function<void(std::string_view)>>{};
    command_map.emplace(
        "show",
        [&current](std::string_view v)
        {
            auto arguments = bm::split_by_delimiter(v, ' ');
            auto from      = 0_z;
            auto amount    = 25_z;
            switch (size(arguments))
            {
            case 1:
            {
                auto arg = bm::to_number<unsigned int>(arguments[0]);
                if (!arg)
                {
                    fmt::print("Could not convert \"{}\" to an integer.\n", arguments[0]);
                    return;
                }
                from = *arg;
                break;
            }
            };

            from = std::min(from, size(current));
            amount = std::min(from + amount, size(current)) - from;

            for (auto&& b : current.subspan(from, amount))
            {
                fmt::print("{}\n", b);
            }
        }
    );

    while (true)
    {
        fmt::print("{}\n", "Enter Command:");
        auto buffer = std::string{};
        getline(std::cin, buffer);

        auto command = std::string_view{buffer};

        if (command.empty())
            continue;

        if (command == "exit"sv)
            break;

        auto arguments = std::string_view{};
        if (auto space_index = command.find(' '); space_index != command.npos)
        {
            arguments = command.substr(std::min(space_index + 1, size(command)));
            command   = command.substr(0, space_index);
        }

        fmt::print("[{}] [{}]\n", command, arguments);
        if (!command_map.contains(command))
        {
            fmt::print("Could not find command \"{}\".\n", command);
            continue;
        }

        command_map[command](arguments);
    }
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

    if (argc != 2)
    {
        fmt::print("Usage: {} [FILE]\n", argv[0]);
        return EXIT_VALUE::USAGE_FAILURE;
    }

    auto opt_logger = create_logger(fmt::format("{}.log.txt", argv[0]));
    if (!opt_logger)
        return EXIT_VALUE::LOG_CREATION_FAILURE;
    (*opt_logger)->set_level(spdlog::level::trace);
    spdlog::set_default_logger(*opt_logger);

    auto const text = bm::read_file(argv[1]);
    if (!text)
    {
        fmt::print("Could not open \"{}\"\n", argv[1]);
        return EXIT_VALUE::FILE_READ_FAILURE;
    }

    run_app(text->view());

    return EXIT_VALUE::SUCCESS;
}
