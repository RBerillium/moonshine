#pragma once

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace unity_list
{
	vec<u64> to_vec(u64 list)
	{
		u64 base = memory::read<u64>(list + 0x10);

		u32 size = memory::read<u32>(list + 0x18);

		if (size > 10000) { return { }; }

		std::vector<u64> buffer(size);

		memory::read_memory(base + 0x20, buffer.data(), size * sizeof(u64));

		return buffer;
	}

	vec<vec3> to_vec3(u64 list)
	{
		u64 base = memory::read<u64>(list + 0x10);

		u32 size = memory::read<u32>(list + 0x18);

		if (size > 10000) { return { }; }

		std::vector<vec3> buffer(size);

		memory::read_memory(base + 0x20, buffer.data(), size * sizeof(vec3));

		return buffer;
	}
}