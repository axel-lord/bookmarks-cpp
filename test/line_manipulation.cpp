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
        d_p{"hello\nthere",            d_l{"hello", "there"}            },
        d_p{"nice\nto\nmeet\nyou!",    d_l{"nice", "to", "meet", "you!"}},
        d_p{"\nhello\n\nthere!\n\n\n", d_l{"hello", "there!"}           },
        d_p{"",                        d_l{}                            },
        d_p{"\n",                      d_l{}                            },
        d_p{"\n\n\n",                  d_l{}                            }
    };

    for (auto&& [in, out] : data)
        CHECK(bm::split_by_linebreak(in) == out);
}

TEST_CASE("line_to_bookmark", "[bookmark]")
{
    using d_p = std::pair<std::string_view, decltype(bm::line_to_bookmark(""))>;

    // order: input, output
    std::array data = {
        d_p{"<url> https://hello.there <info> Hello There! <tag> ",
            bm::bookmark{"https://hello.there", "Hello There!", ""}}
    };

    for (auto&& [in, out] : data)
        CHECK(bm::line_to_bookmark(in) == out);
}