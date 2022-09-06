#pragma once
#include <memory>
#include <span>

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
