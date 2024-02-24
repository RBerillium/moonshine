#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../unity/vector2.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace movement
{
	void set_view_angles(u64 movement, vec2 angles)
	{
		memory::write<vec2>(movement + offset::movement::view_angles, angles);
	}
	vec2 get_view_angles(u64 movement)
	{
		return memory::read<vec2>(movement + offset::movement::view_angles);
	}
}