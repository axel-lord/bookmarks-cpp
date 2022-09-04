#pragma once

#include <cstddef>

namespace bm::util::literals
{
[[nodiscard]] consteval ::std::size_t operator""_z(unsigned long long v)
{
    return v;
}
} // namespace bm::util::literals