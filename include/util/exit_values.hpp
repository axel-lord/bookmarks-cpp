#pragma once

namespace bm::util::EXIT_VALUE
{
constexpr auto SUCCESS               = 0;
constexpr auto USAGE_FAILURE         = 1;
constexpr auto FILE_READ_FAILURE     = 2;
constexpr auto FILE_CONTENT_INVALID  = 3;
constexpr auto LOG_CREATION_FAILURE  = 4;
constexpr auto UNSUPPORTED_EXECUTION = 5;
}; // namespace bm::util::EXIT_VALUE