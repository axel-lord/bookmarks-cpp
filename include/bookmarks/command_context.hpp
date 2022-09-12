#pragma once

#include "./bookmark.hpp"

#include <functional>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace bm::commands
{

struct command_context;

using command_map = std::unordered_map<std::string_view, std::function<void(command_context)>>;

struct command_context
{
    std::string_view           arguments;
    std::vector<bm::bookmark>& bookmarks;
    std::vector<bm::bookmark>& bookmark_buffer;
    std::span<bm::bookmark>&   current;
    command_map const&         cmap;
};
} // namespace bm::commands
