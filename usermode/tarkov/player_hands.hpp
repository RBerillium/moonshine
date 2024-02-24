#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace hands
{
	u64 get_item_in_hand(u64 hands)
	{
		return memory::read<u64>(hands + offset::hands::item_in_hands);
	}
}