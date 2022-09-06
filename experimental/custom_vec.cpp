#include "bookmarks/bookmark.hpp"
#include "custom_capacity.hpp"
#include "custom_final.hpp"
#include "custom_malloc.hpp"
#include "custom_memcpy.hpp"
#include "custom_normal.hpp"
#include "custom_realloc.hpp"
#include "custom_realloc_no_span.hpp"
#include "custom_realloc_no_span_simple.hpp"
#include "custom_realloc_no_span_starting_capacity.hpp"
#include "prepare.hpp"
#include "util/string_view_functions.hpp"

#include <algorithm>
#include <array>
#include <benchmark/benchmark.h>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <new>
#include <random>
#include <span>
#include <vector>

namespace
{

template <typename vector_like>
inline auto
compare_vector_with_return(auto& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        auto vec = vector_like{};
        benchmark::DoNotOptimize(vec.data());

        for (auto&& value : prep::DATA_ARRAY)
            [[maybe_unused]] auto ret = vec.emplace_back(value);

        benchmark::ClobberMemory();
        benchmark::DoNotOptimize(vec);
    }
}

template <typename vector_like>
inline auto
compare_vector_with_reserve(auto& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        auto vec = vector_like{};
        benchmark::DoNotOptimize(vec.data());

        vec.reserve(4096);
        for (auto&& value : prep::DATA_ARRAY)
            [[maybe_unused]] auto ret = vec.emplace_back(value);
        
        benchmark::ClobberMemory();
        benchmark::DoNotOptimize(vec);
    }
}

template <typename vector_like>
inline auto
compare_vector_with_reserve_return(auto& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        auto vec = vector_like{};
        benchmark::DoNotOptimize(vec.data());

        [[maybe_unused]] auto unused_val = vec.reserve(4096);
        for (auto&& value : prep::DATA_ARRAY)
            [[maybe_unused]] auto ret = vec.emplace_back(value);

        benchmark::ClobberMemory();
        benchmark::DoNotOptimize(vec);
    }
}

auto
compare_vector_std(benchmark::State& state)
{
    compare_vector_with_return<std::vector<prep::input_type>>(state);
}

auto
compare_vector_std_capacity_4096(benchmark::State& state)
{
    compare_vector_with_reserve<std::vector<prep::input_type>>(state);
}

auto
compare_vector_custom_final(benchmark::State& state)
{
    compare_vector_with_return<custom_final::vector<prep::input_type>>(state);
}
//auto
//compare_vector_custom_final_capacity_4096(benchmark::State& state)
//{
//    compare_vector_with_return<custom_final::vector<prep::input_type, std::size_t, 4096>>(state);
//}

auto
compare_vector_custom_final_capacity_4096(benchmark::State& state)
{
    compare_vector_with_reserve_return<custom_final::vector<prep::input_type>>(state);
}
} // namespace

BENCHMARK(compare_vector_std);
BENCHMARK(compare_vector_std_capacity_4096);
BENCHMARK(compare_vector_custom_final);
BENCHMARK(compare_vector_custom_final_capacity_4096);
