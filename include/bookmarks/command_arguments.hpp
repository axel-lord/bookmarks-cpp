#pragma once
#include "util/string_view_functions.hpp"

#include <algorithm>
#include <string_view>
#include <utility>

namespace bm::commands
{
constexpr auto
parse_arguments(std::string_view const input_line)
{
    using namespace std::literals::string_view_literals;

    auto const whitespace_trim = [](auto s)
    {
        return bm::util::trim(s, " \n\t"sv);
    };

    auto const line = whitespace_trim(input_line);

    if (auto space_index = line.find(' '); space_index != line.npos)
    {
        return std::pair{
            whitespace_trim(line.substr(0, space_index)),
            whitespace_trim(line.substr(std::min(space_index + 1, size(line))))};
    }

    return std::pair{line, ""sv};
}
} // namespace bm::commands