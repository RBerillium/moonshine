#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace slot
{
	u64 get_name(u64 slot)
	{
		return memory::read<u64>(slot + offset::slot::name);
	}

	u64 get_item(u64 slot)
	{
		return memory::read<u64>(slot + offset::slot::item);
	}
}