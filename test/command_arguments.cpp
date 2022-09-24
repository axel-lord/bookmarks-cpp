#include "bookmarks/command_arguments.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <utility>

TEST_CASE("parse_arguments", "[commands]")
{
    using namespace std::literals;
    using d_o = decltype(bm::commands::parse_arguments(""sv));
    using d_p = std::pair<std::string_view, d_o>;

    std::array data = {
        d_p{"hello"sv, d_o{"hello"sv, ""sv}},
        d_p{"hello there"sv, d_o{"hello"sv, "there"sv}},
        d_p{"hello there nice"sv, d_o{"hello"sv, "there nice"sv}},
        d_p{" hello "sv, d_o{"hello"sv, ""sv}},
    };

    for (auto const& [input, output] : data)
    {
        auto const [r_cmd, r_args] = bm::commands::parse_arguments(input);
        auto const [o_cmd, o_args] = output;

        CHECK(r_cmd == o_cmd);
        CHECK(r_args == o_args);
    }
        
}