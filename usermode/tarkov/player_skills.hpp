#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace skills
{
	void enable_super_jump(u64 skills)
	{
		if (skills)
		{
			uint64_t strength_buff_jump_height_inc = memory::read<u64>(skills + offset::skills::strength_buff_jump_height_inc);
			if (strength_buff_jump_height_inc)
				memory::write<f32>(strength_buff_jump_height_inc + offset::skills::value, 0.6f);

		}
	}

	void disable_super_jump(u64 skills)
	{
		if (skills)
		{
			uint64_t strength_buff_jump_height_inc = memory::read<u64>(skills + offset::skills::strength_buff_jump_height_inc);
			if(strength_buff_jump_height_inc)
				memory::write<f32>(strength_buff_jump_height_inc + offset::skills::value, 0.0f);

		}
	
	}
	//[180] MagDrillsLoadSpeed : -.SkillManager.GClass1641
	//[188] MagDrillsUnloadSpeed : -.SkillManager.GClass1641
	//[190] MagDrillsInventoryCheckSpeed : -.SkillManager.GClass1641
	//[198] MagDrillsInventoryCheckAccuracy : -.SkillManager.GClass1641
	void enable_fast_loot(u64 skills)
	{
		uint64_t fast_loot = memory::read<uint64_t>(skills + 0x4B8);
		memory::write<f32>(fast_loot + offset::skills::value, 5100.f);

		uint64_t MagDrillsLoadSpeed = memory::read<uint64_t>(skills + 0x180);
		memory::write<f32>(MagDrillsLoadSpeed + offset::skills::value, 85.f);//

		uint64_t MagDrillsUnloadSpeed = memory::read<uint64_t>(skills + 0x188);
		memory::write<f32>(MagDrillsUnloadSpeed + offset::skills::value, 60.f); //

		//uint64_t MagDrillsInventoryCheckSpeed = memory::read<uint64_t>(skills + 0x190);
		//memory::write<f32>(MagDrillsInventoryCheckSpeed + offset::skills::value, 5100.f);


		//uint64_t MagDrillsInventoryCheckAccuracy = memory::read<uint64_t>(skills + 0x198);
		//memory::write<f32>(MagDrillsInventoryCheckAccuracy + offset::skills::value, 5100.f);

		//uint64_t double_search = memory::read<uint64_t>(skills + 0x4C0);
		//memory::write<f32>(double_search + offset::skills::value, 100.f);

		//uint64_t mag_drills = memory::read<uint64_t>(skills + 0x430);
		//memory::write<f32>(mag_drills + offset::skills::value, 5100.f);

	
		uint64_t fast_search = memory::read<uint64_t>(skills + 0x160);
		memory::write<f32>(fast_search + offset::skills::value, 5100.f);
		
		uint64_t fast_study = memory::read<uint64_t>(skills + 0x168);
		memory::write<f32>(fast_study + offset::skills::value, 5100.f);

		//uint64_t lucky_search = memory::read<uint64_t>(skills + 0x170);
		//memory::write<f32>(fast_study + offset::skills::value, 5100.f);
	}
}