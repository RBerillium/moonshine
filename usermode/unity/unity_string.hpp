#pragma once

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace unity_string
{
	str to_str(u64 string)
	{
		auto get_size = [&](u64 address) -> u32
		{
			u32 size = memory::read<u32>(address + 0x10);

			return size > 1000 || size < 0 ? 0 : size;
		};

		str result; char buffer_a[0x1000]; chars buffer_b = buffer_a;

		u32 string_size = get_size(string) * 2;

		memory::read_memory(string + 0x14, &buffer_a, 256);

		for (u32 i = 0; i < string_size; i += 2)
		{
			char16_t current_char = *((chars)buffer_b + (i));

			current_char += (*((chars)buffer_b + (i + 1)) << 8);

			result.push_back((char)current_char);
		}

		return result;
	}
	std::wstring to_wide_str(u64 string)
	{
		auto get_size = [&](u64 address) -> u32
		{
			u32 size = memory::read<u32>(address + 0x10);

			return size > 1000 || size < 0 ? 0 : size;
		};

		std::wstring result; wchar_t buffer_a[0x1000]; wchars buffer_b = buffer_a;

		u32 string_size = get_size(string) * 2;

		memory::read_memory(string + 0x14, &buffer_a, 256);

		for (u32 i = 0; i < string_size; i += 2)
		{
			char16_t current_char = *((chars)buffer_b + (i));

			current_char += (*((chars)buffer_b + (i + 1)) << 8);

			result.push_back((wchar_t)current_char);
		}

		return result;
	}
}