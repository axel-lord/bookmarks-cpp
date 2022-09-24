#pragma once

#include "./command_context.hpp"

namespace bm::commands
{
void show(command_context);
void regex(command_context);
void filter(command_context);
void fuzzy(command_context);
void count(command_context);
void reset(command_context);
void fs(command_context);
} // namespace bm::commands
