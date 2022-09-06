#pragma once
#include <algorithm>
#include <cstring>
#include <memory>
#include <span>

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

        std::for_each(begin(span), end(span), call_deleters);

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
