#pragma once

#include "bookmarks/bookmark.hpp"

#include <span>

namespace prep
{
using input_type = bm::bookmark;
extern const std::span<bm::bookmark> DATA_ARRAY;
} // namespace prep
