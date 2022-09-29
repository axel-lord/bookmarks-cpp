#pragma once
#include <filesystem>

// only used by sources, not exposed beyon library

namespace bm::commands::file
{
inline auto
ensure_directory(std::filesystem::path const& p)
{
    if (is_directory(p))
        return std::optional<std::filesystem::path>{p};
    else
        return std::optional<std::filesystem::path>{std::nullopt};
}

inline auto
build_location_path(std::filesystem::path const& cwd, std::filesystem::path const& input)
{
    if (input.is_absolute())
        return input;
    else
        return absolute(cwd / input);
}
} // namespace bm::commands::file
