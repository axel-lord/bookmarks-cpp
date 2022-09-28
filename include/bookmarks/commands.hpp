#pragma once

#include "./command_context.hpp"
#include <functional>

namespace bm::commands
{
void show(command_context);
void regex(command_context);
void filter(command_context);
void fuzzy(command_context);
void count(command_context);
void reset(command_context);

/*! Generate a command that handles filesystem operations.
 * 
 * @return a command
*/
std::function<void(command_context)> make_fs_command();
} // namespace bm::commands
