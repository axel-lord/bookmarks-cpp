#include "bookmarks/line_manipulation.hpp"
#include "util/string_view_functions.hpp"

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

TEST_CASE("left_trim", "[text]")
{
    using d_p = std::tuple<std::string_view, std::string_view, std::string_view>;

    // order: input, output, trim set
    std::array data = {
        d_p{"\nHello There!",     "Hello There!", "\n"   },
        d_p{"\n\n\nHello There!", "Hello There!", "\n"   },
        d_p{"Hello There!",       "Hello There!", "\n"   },
        d_p{"Hello There!",       "There!",       " Helo"},
    };

    for (auto&& [in, out, trim_set] : data)
        CHECK(bm::util::left_trim(in, trim_set) == out);
}

TEST_CASE("right_trim", "[text]")
{
    using d_p = std::tuple<std::string_view, std::string_view, std::string_view>;

    // order: input, output, trim set
    std::array data = {
        d_p{"Hello There!\n",     "Hello There!", "\n"    },
        d_p{"Hello There!\n\n\n", "Hello There!", "\n"    },
        d_p{"Hello There!",       "Hello There!", "\n"    },
        d_p{"Hello There!",       "Hello",        " Ther!"},
    };

    for (auto&& [in, out, trim_set] : data)
        CHECK(bm::util::right_trim(in, trim_set) == out);
}

TEST_CASE("trim", "[text]")
{
    using d_p = std::tuple<std::string_view, std::string_view, std::string_view>;

    // order: input, output, trim set
    std::array data = {
        d_p{"Hello There!\n",           "Hello There!", "\n"       },
        d_p{"Hello There!\n\n\n",       "Hello There!", "\n"       },
        d_p{"\nHello There!",           "Hello There!", "\n"       },
        d_p{"\n\n\nHello There!",       "Hello There!", "\n"       },
        d_p{"\nHello There!\n",         "Hello There!", "\n"       },
        d_p{"\n\n\nHello There!\n\n\n", "Hello There!", "\n"       },
        d_p{"Hello There!",             "Hello There!", "\n"       },
        d_p{"Hello There!",             "Hello",        " Ther!"   },
        d_p{"Hello There!",             "There!",       " Helo"    },
        d_p{"Hello There!",             " ",            "HeloTher!"},
        d_p{"  Hello There!  ",         "Hello There!", " "        },
        d_p{"Hello There!",             "Hello There!", ""         },
    };

    for (auto&& [in, out, trim_set] : data)
        CHECK(bm::util::trim(in, trim_set) == out);
}

TEST_CASE("indice_substring", "[text]")
{
    using d_p = std::tuple<std::string_view, std::string_view, std::size_t, std::size_t>;

    // order: input, output, lower index, upper index
    std::array data = {
        d_p{"Hello There!", "Hello", 0, 5},
        d_p{"Hello There!", "There", 6, 11}
    };

    for (auto&& [in, out, lower, upper] : data)
        CHECK(bm::util::indice_substring(in, lower, upper) == out);
}