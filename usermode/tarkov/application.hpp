#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../unity/vector2.hpp"

#include "../main/memory_system.hpp"

namespace memory = memory_system;

namespace app
{
	void disable_inertia(u64 app)
	{
		u64 inertia = memory::read<u64>(app, offset::application::inertia);
	
	
		memory::write<vec2>(inertia + 0xf4, { 0.f, 0.f });		// MoveTimeRange
		memory::write<f32>(inertia + 0x20, 99999.f);			// FallThreshold 
		memory::write<f32>(inertia + 0x4c, 0.f);				// BaseJumpPenaltyDuration
		memory::write<f32>(inertia + 0x50, 0.f);				// DurationPower
		memory::write<f32>(inertia + 0x54, 0.f);				// BaseJumpPenalty
		memory::write<f32>(inertia + 0x58, 0.f);				// PenaltyPower	
		memory::write<f32>(inertia + 0xf0, 0.f);				// MinDirectionBlendTime
	}

	void enable_inertia(u64 app)
	{
		u64 inertia = memory::read<u64>(app, offset::application::inertia);

		memory::write<vec2>(inertia + 0xf4, { 0.15f, 0.5f });	// MoveTimeRange
		memory::write<f32>(inertia + 0x20, 0.15f);				// FallThreshold 
		memory::write<f32>(inertia + 0x4c, 0.3f);				// BaseJumpPenaltyDuration
		memory::write<f32>(inertia + 0x50, 1.58f);				// DurationPower
		memory::write<f32>(inertia + 0x54, 0.15f);				// BaseJumpPenalty
		memory::write<f32>(inertia + 0x58, 1.12f);				// PenaltyPower	
		memory::write<f32>(inertia + 0xf0, 0.1f);				// MinDirectionBlendTime
	}
}