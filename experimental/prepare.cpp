#include "prepare.hpp"

#include "util/string_view_functions.hpp"

#include <random>

namespace prep
{

constexpr int input_size = 4096;

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
make_data_array() -> std::span<input_type>
{
    using input_array = std::array<input_type, input_size>;

    static auto a = std::make_unique<input_array>();

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

    return std::span<input_type, std::dynamic_extent>{*a};
}

const std::span<input_type> DATA_ARRAY = make_data_array();

} // namespace prep
