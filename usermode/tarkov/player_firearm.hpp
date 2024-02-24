#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace firearm
{
	u64 get_fireport(u64 firearm)
	{
		return memory::read<u64>(firearm, { offset::fire_arm::fireport, 0x10 });
	}


	void set_weapon_length(u64 firearm, f32 length)
	{
		memory::write<f32>(firearm + offset::fire_arm::weapon_ln, length);
	}

	u64 get_shots(u64 firearm)
	{
		return memory::read<u64>(firearm + 0x3A0);
	}
}