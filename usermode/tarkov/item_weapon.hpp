#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace weapon
{
	u64 get_mag_slot(u64 weapon)
	{
		return memory::read<u64>(weapon + offset::weapon::mag_slot);
	}

	u64 get_chambers(u64 weapon)
	{
		return memory::read<u64>(weapon + offset::weapon::chambers);
	}

	u64 get_shells_in_chambers(u64 weapon)
	{
		return memory::read<u64>(weapon + offset::weapon::shells);
	}
}