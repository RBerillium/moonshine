#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace item_template
{

	bool is_unlootable(u64 item_template)
	{
		return memory::read<bool>(item_template + offset::item_template::is_lootable);
	}
	bool is_quest_item(u64 item_template)
	{
		return memory::read<bool>(item_template + offset::item_template::is_quest);
	}
	u64 get_item_id(u64 item_template)
	{
		return memory::read<u64>(item_template + offset::item_template::id);
	}

	u64 get_ammo_template(u64 item_template)
	{
		return memory::read<u64>(item_template + offset::item_template::id);
	}
}