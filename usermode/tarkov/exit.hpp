#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

#include "../crypt/xor_string.hpp"

namespace memory = memory_system;

namespace exit_object
{
	vec<str> statuses =
	{
		_("none"),
		_("not_open"),
		_("incomplete_requirement"),
		_("countdown"),
		_("open"),
		_("pending"),
		_("await_activation")
	};

	str get_status(u64 exit)
	{
		u32 status = memory::read<u32>(exit + offset::exit::status);

		if (status >= 1 && status <= 6) { return statuses.at(status); }
	};

	u64 get_name(u64 exit)
	{
		return memory::read<u64>(exit, { offset::exit::settings, offset::exit::settings_name });
	}
}