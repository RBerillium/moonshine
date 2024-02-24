#pragma once

#include "item.hpp"

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

#include "../unity/unity_list.hpp"

namespace memory = memory_system;

namespace stack_slot
{
	u32 get_max_count(u64 stack_slot)
	{
		return memory::read<u32>(stack_slot + offset::stack_slot::max_ammo);
	}

	uint32_t get_current_count(u64 stack_slot)
	{
		u32 ammo = 0;

		u64 ammo_type_list = memory::read<u64>(stack_slot + 0x10);

		if (!ammo_type_list) { return 0; }

		for (u64 item : unity_list::to_vec(ammo_type_list))
		{
			ammo += item::get_stack_count(item);
		}

		return ammo;
	}
}