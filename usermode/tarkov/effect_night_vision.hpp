#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace night
{
	struct night_struct { float a; float b; };

	void enable(u64 night)
	{
		memory::write<bool>(night + offset::night::on, true);
		memory::write<night_struct>(night + offset::night::intensity, { 0.f, 0.f });
	}

	void disable(u64 night)
	{
		memory::write<bool>(night + offset::night::on, false);
		memory::write<night_struct>(night + offset::night::intensity, { 0.f, 0.f });
	}
}