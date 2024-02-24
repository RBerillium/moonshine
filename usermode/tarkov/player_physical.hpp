#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;
byte prev_state{};
namespace physical
{
	struct physical_struct { u64 a; u64 b; u64 c; };

	void set_ulimited_stamina(u64 physical)
	{


		physical_struct stamins = memory::read<physical_struct>(physical + offset::physical::stamina);
		if (stamins.a && stamins.b && stamins.c)
		{
			f32 value_a = memory::read<f32>(stamins.a + offset::stamina::current);
			f32 value_b = memory::read<f32>(stamins.b + offset::stamina::current);
			f32 value_c = memory::read<f32>(stamins.c + offset::stamina::current);
			if(value_a < 70.f)
			memory::write<f32>(stamins.a + offset::stamina::current, 95.f);
			if (value_b < 70.f)
			memory::write<f32>(stamins.b + offset::stamina::current, 85.f);
			if (value_c < 70.f)
			memory::write<f32>(stamins.c + offset::stamina::current, 90.f);
		}
		
		//memory::write<f32>(physical + 0xbc, 0.f);
	}
}