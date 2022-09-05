#include "bookmarks/bookmark.hpp"
#include "util/string_view_functions.hpp"

#include <array>
#include <benchmark/benchmark.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <iostream>
#include <memory>
#include <new>
#include <random>
#include <ranges>
#include <span>
#include <vector>

namespace custom_normal
{

template <typename T, typename S = std::size_t> struct vector
{
    using size_type = S;

    size_type            capacity = 0;
    size_type            size_val = 0;
    std::unique_ptr<T[]> arr;
    std::span<T>         span;

    template <typename... Args> inline auto emplace_back(Args&&... args)
    {
        if (size_val == capacity)
        {
            auto const new_capacity = static_cast<size_type>(capacity == 0) + capacity * 2;

            auto new_arr = std::make_unique<T[]>(new_capacity);

            for (size_type i = 0; i < size_val; ++i)
            {
                new_arr[i] = std::move(arr[i]);
            }

            arr      = std::move(new_arr);
            capacity = new_capacity;
        }

        T& val = arr[size_val];
        val.~T();
        new (&arr[size_val]) T{std::forward<Args>(args)...};
        ++size_val;
        span = std::span<T>{arr.get(), size_val};
    }

    [[nodiscard]] inline T& operator[](size_type index) noexcept
    {
        return arr.get()[index];
    }

    [[nodiscard]] inline T const& operator[](size_type index) const noexcept
    {
        return arr.get()[index];
    }

    [[nodiscard]] inline auto size() const
    {
        return size_val;
    }
};

template <typename T>
[[nodiscard]] inline auto
begin(vector<T>& vec) noexcept
{
    return begin(vec.span);
}

template <typename T>
[[nodiscard]] inline auto
end(vector<T>& vec) noexcept
{
    return end(vec.span);
}

} // namespace custom_normal

namespace custom_capacity
{

template <typename T, typename S = std::size_t> struct vector
{
    using size_type = S;

    size_type            capacity = 0;
    size_type            size_val = 0;
    std::unique_ptr<T[]> arr;
    std::span<T>         span;

    template <typename... Args> inline auto emplace_back(Args&&... args)
    {

        if (size_val == capacity)
        {
            auto const new_capacity = static_cast<size_type>(std::max<float>(2, capacity * 1.5f));

            auto new_arr = std::make_unique<T[]>(new_capacity);

            for (size_type i = 0; i < size_val; ++i)
            {
                new_arr[i] = std::move(arr[i]);
            }

            arr      = std::move(new_arr);
            capacity = new_capacity;
        }

        T& val = arr[size_val];
        val.~T();
        new (&arr[size_val]) T{std::forward<Args>(args)...};
        ++size_val;
        span = std::span<T>{arr.get(), size_val};
    }

    [[nodiscard]] inline T& operator[](size_type index) noexcept
    {
        return arr.get()[index];
    }

    [[nodiscard]] inline T const& operator[](size_type index) const noexcept
    {
        return arr.get()[index];
    }

    [[nodiscard]] inline auto size() const
    {
        return size_val;
    }
};

template <typename T>
[[nodiscard]] inline auto
begin(vector<T>& vec) noexcept
{
    return begin(vec.span);
}

template <typename T>
[[nodiscard]] inline auto
end(vector<T>& vec) noexcept
{
    return end(vec.span);
}

} // namespace custom_capacity

namespace custom_memcpy
{

template <typename T, typename S = std::size_t> struct vector
{
    using size_type = S;

    size_type    capacity = 0;
    size_type    size_val = 0;
    T*           arr      = nullptr;
    std::span<T> span;

    template <typename... Args> inline auto emplace_back(Args&&... args)
    {
        if (size_val == capacity)
        {
            auto const new_capacity = static_cast<size_type>(capacity == 0) + capacity * 2;

            auto new_arr = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

            std::memcpy(new_arr, arr, size_val * sizeof(T));

            operator delete[](arr);

            arr      = new_arr;
            capacity = new_capacity;
        }

        new (&arr[size_val]) T{std::forward<Args>(args)...};
        ++size_val;
        span = std::span<T>{arr, size_val};
    }

    ~vector()
    {
        auto call_deleters = [](T& value)
        {
            value.~T();
        };

        std::ranges::for_each(span, call_deleters);

        if (arr)
            operator delete[](arr);
    }

    [[nodiscard]] inline T& operator[](size_type index) noexcept
    {
        return arr[index];
    }

    [[nodiscard]] inline T const& operator[](size_type index) const noexcept
    {
        return arr[index];
    }

    [[nodiscard]] inline auto size() const
    {
        return size_val;
    }
};

template <typename T>
[[nodiscard]] inline auto
begin(vector<T>& vec) noexcept
{
    return begin(vec.span);
}

template <typename T>
[[nodiscard]] inline auto
end(vector<T>& vec) noexcept
{
    return end(vec.span);
}

} // namespace custom_memcpy

namespace custom_malloc
{

template <typename T, typename S = std::size_t> struct vector
{
    using size_type = S;

    size_type    capacity = 0;
    size_type    size_val = 0;
    T*           arr      = nullptr;
    std::span<T> span;

    template <typename... Args> inline T* emplace_back(Args&&... args)
    {
        if (size_val == capacity)
        {
            auto const new_capacity = static_cast<size_type>(capacity == 0) + capacity * 2;

            auto new_arr = static_cast<T*>(std::malloc(new_capacity * sizeof(T)));
            if (!new_arr)
                return nullptr;

            std::memcpy(new_arr, arr, size_val * sizeof(T));

            std::free(arr);

            arr      = new_arr;
            capacity = new_capacity;
        }

        new (&arr[size_val]) T{std::forward<Args>(args)...};
        ++size_val;
        span = std::span<T>{arr, size_val};
        return &span.back();
    }

    ~vector()
    {
        auto call_deleters = [](T& value)
        {
            value.~T();
        };

        std::ranges::for_each(span, call_deleters);

        if (arr)
            std::free(arr);
    }

    [[nodiscard]] inline T& operator[](size_type index) noexcept
    {
        return arr[index];
    }

    [[nodiscard]] inline T const& operator[](size_type index) const noexcept
    {
        return arr[index];
    }

    [[nodiscard]] inline auto size() const
    {
        return size_val;
    }
};

template <typename T>
[[nodiscard]] inline auto
begin(vector<T>& vec) noexcept
{
    return begin(vec.span);
}

template <typename T>
[[nodiscard]] inline auto
end(vector<T>& vec) noexcept
{
    return end(vec.span);
}

} // namespace custom_malloc

namespace custom_realloc
{

template <typename T, typename S = std::size_t> struct vector
{
    using size_type = S;

    size_type    capacity = 0;
    size_type    size_val = 0;
    T*           arr      = nullptr;
    std::span<T> span;

    template <typename... Args> inline T* emplace_back(Args&&... args)
    {
        if (size_val == capacity)
        {
            auto const new_capacity = static_cast<size_type>(capacity == 0) + capacity * 2;

            auto new_arr = static_cast<T*>(std::realloc(arr, new_capacity * sizeof(T)));
            if (!new_arr)
                return nullptr;

            arr      = new_arr;
            capacity = new_capacity;
        }

        new (&arr[size_val]) T{std::forward<Args>(args)...};
        ++size_val;
        span = std::span<T>{arr, size_val};
        return &span.back();
    }

    ~vector()
    {
        auto call_deleters = [](T& value)
        {
            value.~T();
        };

        std::ranges::for_each(span, call_deleters);

        if (arr)
            std::free(arr);
    }

    [[nodiscard]] inline T& operator[](size_type index) noexcept
    {
        return arr[index];
    }

    [[nodiscard]] inline T const& operator[](size_type index) const noexcept
    {
        return arr[index];
    }

    [[nodiscard]] inline auto size() const
    {
        return size_val;
    }
};

template <typename T>
[[nodiscard]] inline auto
begin(vector<T>& vec) noexcept
{
    return begin(vec.span);
}

template <typename T>
[[nodiscard]] inline auto
end(vector<T>& vec) noexcept
{
    return end(vec.span);
}

} // namespace custom_realloc

namespace
{
using clock_type = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clock_type>;
using duration   = decltype(std::declval<time_point>() - std::declval<time_point>());

class time_actions
{
    time_point start_time;
    duration*  output;

public:
    time_actions(duration& output) : start_time(clock_type::now()), output(&output)
    {
    }

    ~time_actions()
    {
        time_point end_time = clock_type::now();
        *output             = end_time - start_time;
    }
};

constexpr int input_size = 128;

using input_type  = bm::bookmark;
using input_array = std::array<input_type, input_size>;

auto
make_data_string(std::size_t length)
{
    static auto s = std::make_unique<char[]>(length);

    auto r = std::mt19937_64{std::random_device{}()};
    auto d = std::uniform_int_distribution<int>{'a', 'z'};

    for (std::size_t i = 0; i < length; ++i)
    {
        s[i] = static_cast<char>(d(r));
    }

    return std::string_view{s.get(), length};
}

auto
make_data_array()
{
    auto a = std::make_unique<input_array>();
    auto s = make_data_string(512);

    auto r = std::mt19937_64{std::random_device{}()};
    auto d = std::uniform_int_distribution<std::size_t>{0, std::size(s) - 1};

    auto get_string_view = [&]
    {
        auto index_1 = d(r);
        auto index_2 = d(r);

        return bm::util::indice_substring(
            s, std::min(index_1, index_2), std::max(index_1, index_2)
        );
    };

    for (auto&& item : *a)
    {
        item = bm::bookmark{get_string_view(), get_string_view(), get_string_view()};
    }

    return a;
}

auto data_array = make_data_array();

} // namespace

namespace
{

template <typename vector_like>
inline auto
compare_vector(auto& state)
{
    for (auto _ : state)
    {
        auto vec = vector_like{};
        for (auto&& value : *data_array)
        {
            vec.emplace_back(value);
        }

        for (decltype(std::size(vec)) i = 0; i < std::size(vec); ++i)
        {
            if (vec[i] != (*data_array)[i])
            {
                fmt::print("vector emplace failed!");
            }
        }
    }
}

auto
compare_vector_std(benchmark::State& state)
{
    compare_vector<std::vector<input_type>>(state);
}

auto
compare_vector_custom(benchmark::State& state)
{
    compare_vector<custom_normal::vector<input_type>>(state);
}

auto
compare_vector_custom_capacity(benchmark::State& state)
{
    compare_vector<custom_capacity::vector<input_type>>(state);
}

auto
compare_vector_custom_memcpy(benchmark::State& state)
{
    compare_vector<custom_memcpy::vector<input_type>>(state);
}

auto
compare_vector_custom_malloc(benchmark::State& state)
{
    compare_vector<custom_malloc::vector<input_type>>(state);
}

auto
compare_vector_custom_realloc(benchmark::State& state)
{
    compare_vector<custom_realloc::vector<input_type>>(state);
}
} // namespace

BENCHMARK(compare_vector_std);
BENCHMARK(compare_vector_custom);
BENCHMARK(compare_vector_custom_capacity);
BENCHMARK(compare_vector_custom_memcpy);
BENCHMARK(compare_vector_custom_malloc);
BENCHMARK(compare_vector_custom_realloc);