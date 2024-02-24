#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace skeleton
{
	enum class bone_index
	{
		root = 0,

		pelvis = 14,

		l_calf = 17,
		l_foot = 18,

		r_calf = 22,
		r_foot = 23,

		spine = 37,

		l_upperarm = 90,
		l_forearm = 91,
		l_palm = 94,

		r_upperarm = 111,
		r_forearm = 112,
		r_palm = 115,

		head = 133
	};

	bone_index skeleton_bones[13] =
	{
		bone_index::r_foot,
		bone_index::r_calf,
		bone_index::pelvis,
		bone_index::l_calf,
		bone_index::l_foot,
		bone_index::spine,
		bone_index::head,
		bone_index::l_upperarm,
		bone_index::l_forearm,
		bone_index::l_palm,
		bone_index::r_upperarm,
		bone_index::r_forearm,
		bone_index::r_palm
	};

	u64 get_transforms(u64 skeleton)
	{
		return memory::read<u64>(skeleton + offset::skeleton::values);
	}
}