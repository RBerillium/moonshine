#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace eft_player
{

	u64 get_body(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::body);
	}

	u64 get_hands(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::hands);
	}

	u64 get_inventory(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::inventory);
	}

	u64 get_profile(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::profile);
	}

	u64 get_physical(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::physical);
	}
	u64 get_pwa(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::pwa);
	}
	u64 get_movement(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::movement);
	}
	u64 get_character_controller(u64 player)
	{
		return memory::read<u64>(player + offset::eft_player::character);
	}

}