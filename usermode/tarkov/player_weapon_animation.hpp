#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"
template <typename T = std::uint64_t>
bool valid_ptr(T ptr) {
	return (ptr && ptr > (T)0xFFFFFF && ptr < (T)0x7FFFFFFFFFFF);
}
namespace memory = memory_system;

namespace pwa
{
	struct pwa_struct { u64 a; u64 b; u64 c; u64 d; };

	u64 get_firearm(u64 pwa)
	{
		return memory::read<u64>(pwa + offset::procedural_weapon_animation::fire_arm);
	}

	void set_shot_direction(u64 pwa, vec3 direction)
	{
		memory::write<vec3>(pwa + 0x200, direction);
	}

	void recoil_enable(u64 pwa)
	{
		//if (pwa)
		//{
		//	//memory::write<u32>(pwa + offset::procedural_weapon_animation::mask, 125);
		//	u64 shot_effector = memory::read<u64>(pwa + 0x48);
		//	u64 new_shot_recoil = memory::read<u64>(shot_effector + 0x18);
		//	memory::write(new_shot_recoil + 0x41, true);
		//}
		memory::write<u32>(pwa + offset::procedural_weapon_animation::mask, 125);
	}
	
	void recoil_disable(u64 pwa)
	{
	
	
			

		pwa_struct effectors = memory::read<pwa_struct>(pwa + offset::procedural_weapon_animation::breath_effector);
		if (effectors.a && effectors.b && effectors.c &&effectors.d)
		{
			memory::write<u32>(pwa + offset::procedural_weapon_animation::mask, 1);
			memory::write<f32>(effectors.a + offset::breath_effector::intensity, 0.f);
			memory::write<f32>(effectors.b + offset::walk_effector::intensity, 0.f);
			memory::write<f32>(effectors.c + offset::motion_effector::intensity, 0.f);
			memory::write<f32>(effectors.d + offset::force_effector::intensity, 0.f);
		}


		//uint64_t shot_effector = memory::read<uint64_t>(pwa + offset::procedural_weapon_animation::shot_effector);
		//uint64_t config = memory::read<u64>(shot_effector + 0x40);
		//memory::write<f32>(config + 0x38, 0.f);
		//u64 new_shot_recoil = memory::read<u64>(shot_effector + 0x18);
		//memory::write(new_shot_recoil + 0x41, false);
		//uintptr_t config = hv.read<uintptr_t>(bsg::shot_effector + 0x40);// [40] _aimingConfig : -.GClass11DE.AimingConfiguration
		//hv.write<float>(config + 0x38, 0.f);//[38] RecoilScaling : Single
		//uintptr_t new_shot_recoil = hv.read<uintptr_t>(bsg::shot_effector + 0x18);// EFT.Animations.NewRecoil.NewRecoilShotEffect
		//hv.write<bool>(new_shot_recoil + 0x41, false);// [41] RecoilEffectOn : Boolean

		

	}
}