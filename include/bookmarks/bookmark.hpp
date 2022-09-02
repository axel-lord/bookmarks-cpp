#pragma once

#include <string_view>

namespace bm
{
	struct bookmark
	{
        std::string_view url;
        std::string_view description;
        std::string_view tags;
	};
}