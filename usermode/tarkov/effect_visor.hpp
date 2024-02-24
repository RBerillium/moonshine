#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace visor
{
	void enable(u64 visor)
	{
		memory::write<f32>(visor + offset::visor::intensity, 1.f);
	}

	void disable(u64 visor)
	{
		memory::write<f32>(visor + offset::visor::intensity, 0.f);
	}
}