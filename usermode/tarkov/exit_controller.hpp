#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace exit_controller
{
	u64 get_exit_array(u64 exit_controller)
	{
		return memory::read<u64>(exit_controller + offset::exit_controller::exits);
	}
}