#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace grenade_controller
{
	u64 get_grenade_list(u64 grenade_controller)
	{
		return memory::read<u64>(grenade_controller + offset::grenade_controller::grenades);
	}
}