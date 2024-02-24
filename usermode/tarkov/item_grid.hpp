#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace grid
{
	u64 get_items_dictionary(u64 grid)
	{
		return memory::read<u64>(grid, { offset::grid::collection, offset::grid::collection_dict });
	}
}