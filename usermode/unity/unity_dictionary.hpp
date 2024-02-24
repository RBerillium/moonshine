#pragma once

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace unity_dictionary
{
	vec<u64> to_vec(u64 dictionary, u32 offset = 0x28)
	{
		u64 base = memory::read<u64>(dictionary + 0x18);

		u32 size = memory::read<u32>(dictionary + 0x40);

		struct dictionary_entry { u64 entry; byte pad[0x10]; };

		if (size > 10000){ return {}; }

		std::vector<dictionary_entry> buffer(size);

		std::vector<u64> result(size);

		memory::read_memory(base + offset, buffer.data(), size * (sizeof(u64) + 0x10));

		for (dictionary_entry entry : buffer) { result.push_back(entry.entry); }

		return result;
	}
}