#pragma once

#include "offset.hpp"


#include "../main.hpp"
#include "..//unity/external_mono.hpp"

#include "../main/memory_system.hpp"

#include "..//tarkov/player_eft_player.hpp"
#include "..//tarkov/player_movement.hpp"



#include "..\crypt\xor_string.hpp"

namespace memory = memory_system;

namespace game
{
	struct game_object_manager_t
	{
		u64 last_tagged_object;
		u64 tagged_objects;
		u64 last_main_camera_tagged;
		u64 main_camera_tagged;
		u64 last_active_object;
		u64 active_objects;
	};

	struct object_t
	{
		u64 previous_object_link;
		u64 next_object_link;
		u64 object;
	};

	u64 unity_player_module;

	u64 mono_module;

	u64 world;

	u64 camera;

	u64 application;

	u64 night_vision;

	u64 thermal_vision;

	u64 visor_effect;

	u64 eft_hard_settings; 

	u64 game_world_instance;

	vec2 local_player_angles;


	u64 get_object(u64 address, str name, vec<u64> chain)
	{
		char buffer[256];

		auto object = memory::read<std::array<u64, 2>>(address);

		if (!object.empty())
		{
			object_t active_object = memory::read<object_t>(object[1]);

			object_t last_object = memory::read<object_t>(object[0]);

			if (active_object.object)
			{
				while (active_object.object != 0 && active_object.object != last_object.object)
				{
					u64 class_name_pointer = memory::read<u64>(active_object.object + 0x60);

					memory::read_memory(class_name_pointer, &buffer, sizeof(buffer));

					if (strcmp(buffer, name.c_str()) == 0)
					{
						if (!chain.empty())
						{
							return memory::read<u64>(active_object.object, chain);
						}
						else
						{
							return active_object.object;
						}

					}
					active_object = memory::read<object_t>(active_object.next_object_link);
				}
			}

			if (last_object.object)
			{
				u64 class_name_pointer = memory::read<u64>(last_object.object + 0x60);

				memory::read_memory(class_name_pointer, &buffer, sizeof(buffer));

				if (strcmp(buffer, name.c_str()) == 0)
				{
					if (!chain.empty())
					{
						return memory::read<u64>(active_object.object, chain);
					}
					else
					{
						return last_object.object;
					}
				}
			}
		}

		return 0;
	}

	u64 get_component(u64 object, str component)
	{
		char name[256];

		u64 buffer = memory::read<u64>(object + 0x30);

		for (u32 i = 0x8; i < 0x1000; i += 0x10)
		{
			u64 fields = memory::read<u64>(memory::read<u64>(buffer + i) + 0x28);

			u64 name_chain = memory::read<u64>(fields, { 0x0, 0x0, 0x48});

			memory::read_memory(name_chain, &name, 256);

			if (strcmp(name, component.c_str()) == 0)
			{
				return fields;
			}
		}

		return 0;
	}

	void initialization(uint64_t mono, uint64_t unity)
	{
		game::mono_module = mono;
		game::unity_player_module = unity;
		game::world = 0;
		game::camera = 0;
		game::application = 0;
		game::visor_effect = 0;
		game::night_vision = 0;
		game::thermal_vision = 0;
	}

	void update()
	{
		u64 game_object_manager = memory::read<u64>(unity_player_module + offset::object_manager);

		if (game_object_manager)
		{
			world = (get_object(game_object_manager + offsetof(game_object_manager_t, last_active_object),
				_("GameWorld"), { 0x30, 0x18, 0x28 }));
			if (!world)
			{
				sprint_ptr = 0;
				camera = 0;
			}

			if (camera == 0)
			{

				camera = (get_object(game_object_manager + offsetof(game_object_manager_t, last_main_camera_tagged),
					_("FPS Camera"), { 0x30, 0x18 }));

				application = (get_object(game_object_manager + offsetof(game_object_manager_t, last_active_object),
					_("Application (Main Client)"), { 0x30, 0x18, 0x28 }));

				u64 camera_object = (get_object(game_object_manager + offsetof(game_object_manager_t, last_main_camera_tagged),
					_("FPS Camera"), { }));

				night_vision = (get_component(camera_object, _("NightVision")));

				thermal_vision = (get_component(camera_object, _("ThermalVision")));

				visor_effect = (get_component(camera_object, _("VisorEffect")));

				eft_hard_settings = memory::read<u64>(mono::find_class(_("Assembly-CSharp"), _("EFTHardSettings"))->get_vtable(mono::get_root_domain())->get_static_field_data());
				game_world_instance =  mono::find_class(_("Assembly-CSharp"), _("EFT.GameWorld"))->get_vtable(mono::get_root_domain())->get_static_field_data();
			}
			if (camera && var::esp::show_radar)
			{
				game::local_player_angles = movement::get_view_angles(eft_player::get_movement(local_player));
			}
		}
	}
}