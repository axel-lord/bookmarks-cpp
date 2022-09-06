#include "bookmarks/bookmark.hpp"
#include "util/string_view_functions.hpp"
#include "custom_capacity.hpp"
#include "custom_malloc.hpp"
#include "custom_memcpy.hpp"
#include "custom_normal.hpp"
#include "custom_realloc.hpp"
#include "custom_realloc_no_span.hpp"
#include "custom_realloc_no_span_starting_capacity.hpp"
#include "custom_realloc_no_span_simple.hpp"
#include "prepare.hpp"

#include <array>
#include <benchmark/benchmark.h>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <new>
#include <random>
#include <span>
#include <vector>
#include <algorithm>


namespace
{

template <typename vector_like>
inline auto
compare_vector(auto& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        auto vec = vector_like{};
        for (auto&& value : prep::DATA_ARRAY)
            vec.emplace_back(value);

		benchmark::DoNotOptimize(vec);
    }
}

auto
compare_vector_std(benchmark::State& state)
{
    compare_vector<std::vector<prep::input_type>>(state);
}

auto
compare_vector_custom(benchmark::State& state)
{
    compare_vector<custom_normal::vector<prep::input_type>>(state);
}

auto
compare_vector_custom_capacity(benchmark::State& state)
{
    compare_vector<custom_capacity::vector<prep::input_type>>(state);
}

auto
compare_vector_custom_memcpy(benchmark::State& state)
{
    compare_vector<custom_memcpy::vector<prep::input_type>>(state);
}

auto
compare_vector_custom_malloc(benchmark::State& state)
{
    compare_vector<custom_malloc::vector<prep::input_type>>(state);
}

auto
compare_vector_custom_realloc(benchmark::State& state)
{
    compare_vector<custom_realloc::vector<prep::input_type>>(state);
}

auto
compare_vector_custom_realloc_int_size(benchmark::State& state)
{
    compare_vector<custom_realloc::vector<prep::input_type, unsigned int>>(state);
}

auto
compare_vector_custom_realloc_no_span(benchmark::State& state)
{
    compare_vector<custom_realloc_no_span::vector<prep::input_type>>(state);
}

auto
compare_vector_custom_realloc_starting_capacity_4096(benchmark::State& state)
{
    compare_vector<custom_realloc_starting_capacity::vector<prep::input_type, std::size_t, 4096>>(state);
}

auto
compare_vector_custom_realloc_no_span_simple(benchmark::State& state)
{
    compare_vector<custom_realloc_no_span_simple::vector<prep::input_type>>(state);
}
} // namespace

BENCHMARK(compare_vector_std);
BENCHMARK(compare_vector_custom);
BENCHMARK(compare_vector_custom_capacity);
BENCHMARK(compare_vector_custom_memcpy);
BENCHMARK(compare_vector_custom_malloc);
BENCHMARK(compare_vector_custom_realloc);
BENCHMARK(compare_vector_custom_realloc_int_size);
BENCHMARK(compare_vector_custom_realloc_no_span);
BENCHMARK(compare_vector_custom_realloc_starting_capacity_4096);
BENCHMARK(compare_vector_custom_realloc_no_span_simple);
