#pragma once
#include <cstdlib>
#include <cstring>
#include <span>
#include <algorithm>

namespace custom_realloc_no_span
{

template <typename T, typename S = std::size_t> struct vector
{
    using size_type = S;

    size_type capacity = 0;
    size_type size_val = 0;
    T*        arr      = nullptr;

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

        new (arr + size_val) T{std::forward<Args>(args)...};
        ++size_val;
        return arr + size_val - 1;
    }

    ~vector()
    {
        auto call_deleters = [](T& value)
        {
            value.~T();
        };

        std::for_each(arr, arr+size_val, call_deleters);

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

} // namespace custom_realloc_no_span
