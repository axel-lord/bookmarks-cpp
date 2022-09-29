#include "bookmarks/line_manipulation.hpp"
#include "util/exit_values.hpp"

#include <fmt/color.h>
#include <optional>
#include <span>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string_view>

using namespace std::literals;

namespace EXIT_VALUE = bm::util::EXIT_VALUE;

namespace bm::app
{
void run_app(std::optional<std::string_view> bookmark_view);
void run_app();
} // namespace bm::app

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

        bm::app::run_app(text->view());
    }

    bm::app::run_app();

    return EXIT_VALUE::SUCCESS;
}
