#pragma once
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <span>

namespace custom_final
{

template <typename T, typename S = std::size_t, S starting_capacity = 1> class vector
{
    static_assert(starting_capacity > 0, "Starting capacity has to be 1 or higher.");
    static_assert(
        std::is_nothrow_destructible_v<T>, "Stored object has to be noexcept destructible."
    );

    using size_type = S;

    size_type capacity_val = 0;
    size_type size_val     = 0;
    T*        arr          = nullptr;

public:
    template <typename... Args> [[nodiscard]] inline T* emplace_back(Args&&... args) noexcept
    {
        static_assert(
            std::is_nothrow_constructible_v<T, Args...>,
            "Stored object has to be nothrow constructible with given arguments."
        );
        if (size_val == capacity_val) [[unlikely]]
        {
            auto const new_capacity = (capacity_val == 0) * starting_capacity + capacity_val * 2;

            auto new_arr = static_cast<T*>(std::realloc(arr, new_capacity * sizeof(T)));
            if (!new_arr) [[unlikely]]
                return nullptr;

            arr          = new_arr;
            capacity_val = new_capacity;
        }

        new (arr + size_val) T{std::forward<Args>(args)...};
        ++size_val;
        return arr + size_val - 1;
    }

    ~vector() noexcept
    {
        auto call_deleters = [](T& value)
        {
            value.~T();
        };

        std::for_each(arr, arr + size_val, call_deleters);

        if (arr) [[likely]]
            std::free(arr);
    }

    [[nodiscard]] inline T* operator[](size_type index) noexcept
    {
        if (index < size_val) [[likely]]
            return arr + index;
        return nullptr;
    }

    [[nodiscard]] inline T* const& operator[](size_type index) const noexcept
    {
        if (index < size_val) [[likely]]
            return arr + index;
        return nullptr;
    }

    [[nodiscard]] inline T* front() noexcept
    {
        return arr ? arr : nullptr;
    }

    [[nodiscard]] inline T* back() noexcept
    {
        return arr ? arr + size_val - 1 : nullptr;
    }

    [[nodiscard]] inline auto data() const noexcept
    {
        return arr;
    }

    [[nodiscard]] inline auto size() const noexcept
    {
        return size_val;
    }

    [[nodiscard]] inline bool reserve(size_type new_capacity) noexcept
    {
        if (new_capacity <= capacity_val) [[unlikely]]
            return true;

        auto new_arr = static_cast<T*>(std::realloc(arr, new_capacity * sizeof(T)));
        if (!new_arr) [[unlikely]]
            return false;

        arr          = new_arr;
        capacity_val = new_capacity;

        return true;
    }

    [[nodiscard]] inline auto capacity() const noexcept
    {
        return capacity_val;
    }

    [[nodiscard]] inline auto begin() noexcept
    {
        return arr;
    }
    [[nodiscard]] inline auto end() noexcept
    {
        return arr + size_val;
    }
    [[nodiscard]] inline T const* cbegin() const noexcept
    {
        return arr;
    }
    [[nodiscard]] inline T const* cend() const noexcept
    {
        return arr + size_val;
    }
};

} // namespace custom_final
