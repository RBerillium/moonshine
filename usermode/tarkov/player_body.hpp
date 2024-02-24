#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace body
{
	u64 get_skeleton(u64 body)
	{
		return memory::read<u64>(body + offset::body::skeleton);
	}
}