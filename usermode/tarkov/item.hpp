#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace item
{
	u64 get_template(u64 item)
	{
		return memory::read<u64>(item + offset::item::item_template);
	}

	u64 get_grids_array(u64 item)
	{
		return memory::read<u64>(item + offset::item::grids);
	}

	u64 get_slots_array(u64 item)
	{
		return memory::read<u64>(item + offset::item::slots);
	}

	u64 get_stack_slot(u64 item)
	{
		return memory::read<u64>(item + offset::item::stack_slot);
	}

	u32 get_stack_count(u64 item)
	{
		return memory::read<u32>(item + offset::item::stack_count);
	}
}