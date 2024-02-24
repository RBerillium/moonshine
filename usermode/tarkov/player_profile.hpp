#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

#include "..\crypt\xor_string.hpp"

namespace memory = memory_system;

namespace profile
{
	vec<str> sides =
	{
		_("*"),
		_("U"),
		_("B")
	};

	vec<str> roles =
	{
		_("sniper_scav"),
		_("scav"),
		_("reshala"),
		_("reshala"),
		_("follower_reshala_assault"),
		_("follower_reshala"),
		_("killa"),
		_("shturman"),
		_("shturman_assault"),
		_("raider"),
		_("cultist"),
		_("gluhar"),
		_("follower_gluhar_assault"),
		_("follower_gluhar_security"),
		_("follower_gluhar_scout"),
		_("follower_gluhar_snipe"),
		_("follower_sanitar"),
		_("sanitar"),
		_("raider"),
		_("assault_group"),
		_("sectant_warrior"),
		_("sectant_priest"),
		_("tagilla"),
		_("follower_tagilla"),
		_("ex_pmc"),
		_("santa"),
		_("knight"),
		_("follower_bigpipe"),
		_("follower_birdeye"),
		_("zryachiy"),
		_("follower_zryachiy"),
		_("arena_fighter"),
	};

	u32 experience_table[79] =
	{
		0, 1000, 4017, 8432, 14256, 21477, 30023, 39936, 51204, 63723,
		77563, 92713, 111881, 134674, 161139, 191417, 225194, 262366, 302484, 345751,
		391649, 440444, 492366, 547896, 609066, 675913, 748474, 826786, 910885, 1000809,
		1096593, 1198275, 1309251, 1429580, 1559321, 1698532, 1847272, 2005600, 2173575, 2351255,
		2538699, 2735966, 2946585, 3170637, 3408202, 3659361, 3924195, 4202784, 4495210, 4801553,
		5121894, 5456314, 5809667, 6182063, 6573613, 6984426, 7414613, 7864284, 8333549, 8831052,
		9360623, 9928578, 10541848, 11206300, 11946977, 12789143, 13820522, 15229487, 17206065, 19706065,
		22706065, 26206065, 30206065, 34706065, 39706065, 45206065, 51206065, 58206065, 68206065
	};

	struct memory_struct
	{
		byte pad01[0x10];
		u64 nickname;					// 0x10
		byte pad02[0x38];
		u64 settings_address;			// 0x50
		byte pad03[0x18];
		u32 side;						// 0x70
		u32 registration_date;			// 0x74
		byte pad04[0x18];
		u32 experience;					// 0x90
	};

	struct information_struct
	{
		u64 nickname;
		u32 registration_date;
		u32 level;
		str side;
		str role;
	};

	u32 get_level(u32 exp)
	{
		u32 level = 0;

		for (u32 i = 0; i < ARRAYSIZE(experience_table); i++)
		{
			if (exp >= experience_table[i]) { level = i + 1; }
			else { return level; }
		}

		return 0;
	}

	information_struct get_information(u64 profile)
	{
		memory_struct info = memory::read<memory_struct>(profile, { offset::profile::info, 0x0 });

		information_struct buffer;

		u32 role = memory::read<u32>(info.settings_address + offset::settings::role);

		if (role >= 0 && role <= 31) { buffer.role = roles.at(role); }

		if (info.side == 1 || info.side == 2) { buffer.side = sides.at(info.side); };

		buffer.nickname = info.nickname;

		buffer.registration_date = info.registration_date;

		buffer.level = get_level(info.experience);

		return buffer;
	}

	f32 get_kd(u64 profile)
	{
		u64 counters = memory::read<u64>(profile, { offset::profile::stats, 
			offset::profile::stats_overall_counters, offset::profile::stats_overall_counters_counters, 0x18 });

		u32 kills = memory::read<u32>(counters + offset::counters::kills);
		u32 deaths = memory::read<u32>(counters + offset::counters::deaths);

		return (deaths == 0) ? f32(kills) : f32(kills) / f32(deaths);
	}

	u64 get_skills(u64 profile)
	{
		return memory::read<u64>(profile + offset::profile::skills);
	}
}