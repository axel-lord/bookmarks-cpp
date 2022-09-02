#include <array>
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <utility>
#include <vector>

using namespace std::literals::string_view_literals;

#include "bookmarks/line_manipulation.hpp"

TEST_CASE("Lines are split correctly", "[text]")
{
    using d_l = decltype(bm::split_by_lines(std::string_view{}));
    using d_p = std::pair<std::string_view, d_l>;

    std::array data = {
        d_p{"hello\nthere",            d_l{"hello", "there"}            },
        d_p{"nice\nto\nmeet\nyou!",    d_l{"nice", "to", "meet", "you!"}},
        d_p{"\nhello\n\nthere!\n\n\n", d_l{"hello", "there!"}           },
        d_p{"",                        d_l{}                            },
        d_p{"\n",                      d_l{}                            },
        d_p{"\n\n\n",                  d_l{}                            }
    };

    for (auto&& [in, out] : data)
    {
        CHECK(bm::split_by_lines(in) == out);
    }
}