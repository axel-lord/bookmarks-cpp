#include <array>
#include <catch2/catch_test_macros.hpp>
#include <utility>
#include <climits>

#include "util/string_view_functions.hpp"

TEST_CASE("to_upper/to_lower", "[text]")
{
	using d_p = std::pair<char, char>;

	// order: lower, upper
	std::array const data = {
		d_p{'a', 'A'},
		d_p{'b', 'B'},
		d_p{'c', 'C'},
		d_p{'d', 'D'},
		d_p{'e', 'E'},
		d_p{'f', 'F'},
		d_p{'g', 'G'},
		d_p{'h', 'H'},
		d_p{'i', 'I'},
		d_p{'j', 'J'},
		d_p{'k', 'K'},
		d_p{'l', 'L'},
		d_p{'m', 'M'},
		d_p{'n', 'N'},
		d_p{'o', 'O'},
		d_p{'p', 'P'},
		d_p{'q', 'Q'},
		d_p{'r', 'R'},
		d_p{'s', 'S'},
		d_p{'t', 'T'},
		d_p{'u', 'U'},
		d_p{'v', 'V'},
		d_p{'w', 'W'},
		d_p{'x', 'X'},
		d_p{'y', 'Y'},
		d_p{'z', 'Z'},
		d_p{'(', '('},
		d_p{')', ')'},
		d_p{'[', '['},
		d_p{']', ']'},
		d_p{'}', '}'},
		d_p{'{', '{'},
		d_p{';', ';'},
		d_p{':', ':'},
		d_p{'/', '/'},
		d_p{'+', '+'},
		d_p{'-', '-'},
		d_p{'*', '*'},
		d_p{'=', '='},
		d_p{'&', '&'},
	};

	SECTION("to_upper")
	{
		for (auto const& [lower, upper] : data)
			CHECK(bm::util::to_upper(lower) == upper);
	}

	SECTION("to_lower")
	{
		for (auto const& [lower, upper] : data)
			CHECK(bm::util::to_lower(upper) == lower);
	}

	SECTION("automatic data")
	{
		auto const build_data = [] 
		{
			auto data_build = std::vector<d_p>{};

			for (char i = 0; i + 'a' <= 'z'; ++i)
				data_build.emplace_back(i + 'a', i + 'A');

			auto const not_alphabetical = [] (char c)
			{
				if (c >= 'a' && c <= 'z')
					return false;
				if (c >= 'A' && c <= 'Z')
					return false;
				return true;
			};
			
			for (int i = std::max(CHAR_MIN, 0); i <= CHAR_MAX;  ++i)
			{
				char c = static_cast<char>(i);

				if (not_alphabetical(c))
					data_build.emplace_back(c, c);
			}

			return data_build;
		};

		auto const auto_data = build_data();
		
		for (auto const& [lower, upper] : auto_data)
		{
			CHECK(bm::util::to_upper(lower) == upper);
			CHECK(bm::util::to_lower(upper) == lower);
		}
	}
}
