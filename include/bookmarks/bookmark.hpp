#pragma once

#include <string_view>

namespace bm
{

//! A struct for representing bookmarks.
struct bookmark
{
    std::string_view url;         //!< URL the bookmark points to.
    std::string_view description; //!< Description/Displayname of the bookmark.
    std::string_view tags;        //!< list of tags for the bookmark separated by "<,>".

    friend auto operator<=>(const bookmark&, const bookmark&) = default;
};

} // namespace bm

