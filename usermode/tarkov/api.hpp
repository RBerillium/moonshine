#pragma once

#include "../main.hpp"

namespace api
{
    struct item_info_t
    {
        str name;
        str short_name;
        u32 avg_price;
        u32 base_price;
        u32 width;
        u32 height;
    };

    map<str, item_info_t> items_library;

    item_info_t get_item_info(str id)
    {
        return items_library[id];
    }
}