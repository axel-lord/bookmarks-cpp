#pragma once
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <new>
#include <span>

namespace custom_realloc_starting_capacity
{

template <typename T, typename S = std::size_t, S starting_capacity = 2> struct vector
{
    using size_type = S;

    size_type capacity_val = starting_capacity;
    size_type size_val = 0;
    T*        arr      = nullptr;

    template <typename... Args> inline T* emplace_back(Args&&... args)
    {
        if (size_val == capacity_val)
        {
            auto const new_capacity = capacity_val * 2;

            auto new_arr = static_cast<T*>(std::realloc(arr, new_capacity * sizeof(T)));
            if (!new_arr)
                return nullptr;

            arr      = new_arr;
            capacity_val = new_capacity;
        }

        new (arr + size_val) T{std::forward<Args>(args)...};
        ++size_val;
        return arr + size_val - 1;
    }

    vector() : arr(static_cast<T*>(std::malloc(capacity_val * sizeof(T))))
    {
    }

    ~vector()
    {
        auto call_deleters = [](T& value)
        {
            value.~T();
        };

        std::for_each(arr, arr + size_val, call_deleters);

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
    return vec.arr;
}

template <typename T>
[[nodiscard]] inline auto
end(vector<T>& vec) noexcept
{
    return vec.arr + size(vec);
}

} // namespace custom_realloc_starting_capacity
