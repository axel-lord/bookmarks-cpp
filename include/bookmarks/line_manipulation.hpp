#pragma once

#include <string_view>
#include <vector>

namespace bm
{
[[nodiscard]] inline auto
split_by_lines(std::string_view in)
{
    auto build_store = std::vector<decltype(in)>{};
    while (in.size())
    {
        while (in.size() && in[0] == '\n')
            in.remove_prefix(1);
        if (!in.size())
            break;

        auto lf_index = in.find('\n');
        if (lf_index == in.npos)
        {
            build_store.emplace_back(in);
            break;
        }

        build_store.emplace_back(in.substr(0, lf_index));
        in.remove_prefix(lf_index);
    }
    return build_store;
}
} // namespace bm