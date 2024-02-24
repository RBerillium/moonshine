#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace inventory
{
	u64 get_equipment(u64 inventory)
	{
		return memory::read<u64>(inventory, { offset::inventory::inventory, offset::inventory::inventory_equipment });
	}
}