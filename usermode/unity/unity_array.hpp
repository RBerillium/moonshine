#pragma once

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace unity_array
{
	vec<u64> to_vec(u64 array)
	{
		u32 size = memory::read<u32>(array + 0x18);

		if (size > 10000) { return { }; }

		std::vector<u64> buffer(size);

		memory::read_memory(array + 0x20, buffer.data(), size * sizeof(u64));

		return buffer;
	}
	vec<byte> to_vec_byte(u64 array)
	{
		u32 size = memory::read<u32>(array + 0x18);

		if (size > 10000) { return { }; }

		std::vector<byte> buffer(size);

		memory::read_memory(array + 0x20, buffer.data(), size * sizeof(byte));

		return buffer;
	}
}