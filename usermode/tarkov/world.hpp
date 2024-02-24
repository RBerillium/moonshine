#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace world
{
	u64 get_player_list(u64 world)
	{
		return memory::read<u64>(world + offset::world::players);
	}
	u64 get_player_list1(u64 world)
	{
		return memory::read<u64>(world + offset::world::players_test);
	}


	u64 get_item_list(u64 world)
	{
		return memory::read<u64>(world + offset::world::items);
	}

	u64 get_grenade_controller(u64 world)
	{
		return memory::read<u64>(world + offset::world::grenades);
	}

	u64 get_exit_controller(u64 world)
	{
		return memory::read<u64>(world + offset::world::exits);
	}
}