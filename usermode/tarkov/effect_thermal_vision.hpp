#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace thermal
{
	struct thermal_struct { bool a; bool b; bool c; bool d; bool e; bool f; };

	void enable(u64 thermal)
	{
		memory::write<thermal_struct>(thermal + offset::thermal::enable, { true, false, false, false, false, false });
	}

	void disable(u64 thermal)
	{
		memory::write<thermal_struct>(thermal + offset::thermal::enable, { false, false, false, false, false, false });
	}
}