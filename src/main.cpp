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
#include <string_view>
#include <unordered_map>

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

struct command_context;

using command_map =
    std::unordered_map<std::string_view, std::function<void(std::string_view, command_context)>>;

struct command_context
{
    std::vector<bm::bookmark>& bookmarks;
    std::span<bm::bookmark>&   current;
	command_map const& cmap;
};


namespace commands
{
static auto
show(std::string_view v, command_context ctx)
{
    auto arguments = bm::split_by_delimiter(v, ' ');
    auto from      = 0_z;
    auto amount    = 25_z;
    switch (size(arguments))
    {
    case 0:
        break;
    case 2:
    {
        auto arg = bm::to_number<unsigned int>(arguments[1]);
        if (!arg)
        {
            fmt::print("Could not convert \"{}\" to an integer.\n", arguments[1]);
            return;
        }
        amount = *arg;
    }
        [[fallthrough]];
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
    default:
    {
        fmt::print("Too many arguments passed ({}).", size(arguments));
        return;
    }
    };

    from   = std::min(from, size(ctx.current));
    amount = std::min(from + amount, size(ctx.current)) - from;

    for (auto&& b : ctx.current.subspan(from, amount))
    {
        fmt::print("{}\n", b);
    }
}
} // namespace commands

static inline auto get_input()
{
	auto buffer = std::string{};
	getline(std::cin, buffer);
	return buffer;
}

static inline auto parse_arguments(std::string_view const line)
{
	struct cmd_arg
	{
		std::string_view command;
		std::string_view arguments;
	};

	if (auto space_index = line.find(' '); space_index != line.npos)
	{
		return cmd_arg{line.substr(std::min(space_index + 1, size(line))), line.substr(0, space_index)};
	}

	return cmd_arg{line, ""sv};
}

static inline auto
run_app(std::string_view const bookmark_view)
{
    auto bookmarks = bm::build_bookmark_vector(bookmark_view);
    auto current   = std::span{bookmarks};

    auto const cmap = command_map{{"show"sv, commands::show}};

    while (true)
    {
        fmt::print("{}\n", "Enter Command:");
		auto const buffer = get_input();
        auto const line = std::string_view{buffer};

        if (line.empty())  // nothing entered
            continue;

        if (line.starts_with("exit"sv))  // user wants to exit
            break;
		
		auto const [command, arguments] = parse_arguments(line);

        fmt::print("[{}] [{}]\n", command, arguments);
        if (!cmap.contains(command))
        {
            fmt::print("Could not find command \"{}\".\n", command);
            continue;
        }

        cmap.at(command)(arguments, {bookmarks, current, cmap});
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
