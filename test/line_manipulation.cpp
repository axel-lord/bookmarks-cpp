#include "bookmarks/line_manipulation.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

TEST_CASE("split_by_linebreak", "[text]")
{
    using d_l = decltype(bm::split_by_linebreak(std::string_view{}));
    using d_p = std::pair<std::string_view, d_l>;

    // order: input, output
    std::array data = {
        d_p{"hello\nthere", d_l{"hello", "there"}},
        d_p{"nice\nto\nmeet\nyou!", d_l{"nice", "to", "meet", "you!"}},
        d_p{"\nhello\n\nthere!\n\n\n", d_l{"hello", "there!"}},
        d_p{"", d_l{}},
        d_p{"\n", d_l{}},
        d_p{"\n\n\n", d_l{}}};

    for (auto&& [in, out] : data)
        CHECK(bm::split_by_linebreak(in) == out);
}

TEST_CASE("to_line, to_bookmark", "[bookmark][text]")
{
    using namespace std::literals::string_view_literals;

    SECTION("Bookmarks are correctly created from lines")
    {
        using d_p = std::pair<std::string_view, decltype(bm::to_bookmark(""))>;
        using b_m = bm::bookmark;

        // order: input, output
        std::array data = {
            d_p{"<url> https://hello.there <info> Hello There! <tag> ",
                b_m{"https://hello.there", "Hello There!", ""}},
            d_p{"<url> https://hello.there    <info>     <tag> ",
                b_m{"https://hello.there", "", ""}},
            d_p{"<url> https://hello.there <info> Hello There! <tag> wow <,> hello   ",
                b_m{"https://hello.there", "Hello There!", "wow <,> hello"}},
            d_p{"<url>  <info>  <tag>    ", b_m{"", "", ""}},
            d_p{"<url> There! <tag> ", std::nullopt},
            d_p{"", std::nullopt},
            d_p{"<url>  https://hello.there <info> ", std::nullopt},
        };

        for (auto&& [in, out] : data)
            CHECK(bm::to_bookmark(in) == out);
    }

    SECTION("Lines are correctly created from bookmarks")
    {
        std::array data = {
            std::pair{
                bm::bookmark{"http://hello.there", "Hello There!", "Hello <,> There"},
                "<url>http://hello.there<info>Hello There!<tag>Hello <,> There"sv},
            std::pair{
                bm::bookmark{"http://hello.there", "Hello There!", ""},
                "<url>http://hello.there<info>Hello There!<tag>"sv},
            std::pair{bm::bookmark{"http://nice", "", ""}, "<url>http://nice<info><tag>"sv},
            std::pair{bm::bookmark{}, "<url><info><tag>"sv},
        };

        for (auto&& [in, out] : data)
            CHECK(bm::to_line(in) == out);
    }

    SECTION("Bookmarks are retain information when converted to lines and then back")
    {
        std::array data = {
            bm::bookmark{"https://hello.there", "Hello there!", "Hello <,> There"},
            bm::bookmark{"https://nice", "Nice Site!", "wow <,> much"},
            bm::bookmark{"", "", ""},
            bm::bookmark{"hello", "", ""},
            bm::bookmark{"file://document.js", "Some Document.", "important <,> cool"},
        };

        for (auto&& value : data)
        {
            auto l = bm::to_line(value);
            CHECK(bm::to_bookmark(l) == value);
        }
            
    }
}

TEST_CASE("build_bookmark_vector", "[bookmark][text]")
{
    spdlog::set_level(spdlog::level::off);
    using namespace std::literals;
    using d_p = std::pair<std::string_view, std::vector<bm::bookmark>>;

    std::array bookmarks_text = {
        "<url> https://hello.there <info> Hello There! <tag> "sv,
        "<url> https://there    <info>     <tag> "sv,
        "<url> https://hello <info> Hello There! <tag> wow <,> hello   "sv};

    std::array<bm::bookmark, size(bookmarks_text)> bookmarks;

    auto data_point_1 = d_p{"", {}};
    auto data_point_2 =
        d_p{R"(
#UNSORTED_BEGIN
<url> https://hello.there <info> Hello There! <tag>
#UNSORTED_END)",
            std::vector{bm::bookmark{"https://hello.there", "Hello There!", ""}}};

    // order: input, output
    std::array data = {data_point_1, data_point_2};

    for (auto&& [in, out] : data)
        CHECK(bm::build_bookmark_vector(in) == out);
}