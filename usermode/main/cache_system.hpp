#pragma once
#include <mutex>
#include <shared_mutex>
#include "../main.hpp"
#include "../tarkov/tarkov.hpp"
#include "../unity/vector2.hpp"
#include "../unity/vector3.hpp"
#include "../unity/vector4.hpp"
#include "../unity/unity_list.hpp"
#include "../unity/unity_array.hpp"
#include "../unity/unity_string.hpp"
#include "../unity/external_mono.hpp"
#include "../unity/unity_transform.hpp"
#include "../unity/unity_dictionary.hpp"

std::mutex player_mtx;
std::mutex item_mtx;
std::mutex grenade_mtx;
std::mutex exit_mtx;

//
//övoid change_mov_packet(u64 player)
//{
//	u64 pedometer = memory::read<u64>(player + 0x48); //EFT.Player -> pedometer
//	byte state = memory::read<byte>(pedometer + 0x3c); // Pedometer -> CurrentState
//	
//	{
//		u64 cstruct = memory::read<u64>(player + 0x910);    //?????
//		u64 movement_packet = memory::read<u64>(cstruct + 0x30);
//		memory::write<byte>(movement_packet + 0x11 /*EPlayerState*/, 0x4);
//	}
//
//}
void no_collisions(u64 player)
{
	u64 movement_context_backing_field = memory::read<u64>(player + 0x40); // EFT.Player -> <MovementContext>k_BackingField (typeof EFT.MovementContext)
	//memory::write<float>(movement_context_backing_field + 0x218, 0.04f); //fall time
	//memory::write<bool>(movement_context_backing_field + 0x2c8, true);
	//int a = memory::read<int>(movement_context_backing_field + 0x2d0);
	memory::write <int>(movement_context_backing_field + 0x2e0, 0); // backing_field -> _physicalCondition


}

void fix_stamina(u64 player)
{
	u64 movement_context_backing_field = memory::read<u64>(player + 0x40);  // EFT.Player -> <MovementContext>k_BackingField (typeof EFT.MovementContext)
	u64 current_state_k_backing_field = memory::read<u64>(movement_context_backing_field + 0xd0); //kBackingField -> <CurrentState> k_BackingField (typeof EFT.BaseMovementState)
	byte current_state = memory::read<byte>(current_state_k_backing_field + 0x21); // <CurrentState> k_BackingField -> Name (typeof EPlayerState)



	if (!game::world)
	{
		sprint_ptr = 0;
	}

	if (current_state == 0x5 && sprint_ptr == 0)
	{
		sprint_ptr = current_state_k_backing_field;
	}

	if (current_state_k_backing_field != 0 && sprint_ptr != 0 && current_state_k_backing_field == sprint_ptr)
	{
		memory::write<byte>(sprint_ptr + 0x21 /*Name*/, 0x1);
	}
	else if (sprint_ptr != 0)
	{
		memory::write<byte>(sprint_ptr + 0x21 /*Name*/, 0x5);
	}


}
namespace cache_system
{

	enum class role_t { player, scav, player_scav, boss };

	enum class loot_type_t { container, corpse, item, quest };

	struct inventory_item_t
	{

		str id;
		str name;
		str short_name;
		u32 price;
		u32 list_id;
	};

	struct inventory_slot_t
	{

		str slot_name;
		inventory_item_t main_item;
		vec<inventory_item_t> items;
	};

	struct cached_grenade_t
	{
		vec3 position;
	};

	struct cached_player_t
	{
		vec3 root_position;
		vec3 head_position;
		u32 current_ammo;
		u32 max_ammo;
		u32 level;
		u32 price;
		str weapon;
		str ammo;
		std::string name;
		str side;
		f32 kd;
		role_t role;

		vec<inventory_slot_t> slots;
		vec<vec3> bones;
	};

	struct cached_item_t
	{
		vec3 position;
		loot_type_t type;
		vec<inventory_item_t> items;
	};

	struct cached_exit_t
	{
		vec3 position;
		str name;
		str status;
	};

	vec<cached_grenade_t> grenades; u32 g_time;

	vec<cached_player_t> players; u32 p_time;

	vec<cached_item_t> items; u32 i_time;

	vec<cached_exit_t> exits; u32 e_time;

	vec<cached_player_t> players_buffer{};

	vec<cached_grenade_t> grenades_buffer{};

	vec<cached_item_t> items_buffer{};

	vec<cached_exit_t> exits_buffer{};

	vec<cached_grenade_t> get_grenades()
	{

		std::lock_guard<std::mutex> guard(grenade_mtx);
		return grenades;
	}

	vec<cached_player_t> get_players()
	{
		std::lock_guard<std::mutex> guard(player_mtx);

		return players;
	}

	vec<cached_item_t> get_items()
	{
		std::lock_guard<std::mutex> guard(item_mtx);
		return items;

	}

	vec<cached_exit_t> get_exits()
	{
		std::lock_guard<std::mutex> guard(exit_mtx);
		return exits;
	}

	u32 is_in_list(str id)
	{
		u32 i = 0;

		for (var::esp::item::custom_item_list list : var::esp::item::custom_item_lists)
		{
			i++;

			if (list.enable)
			{
				if (std::find(list.items.begin(), list.items.end(), id) != list.items.end())
				{
					return i;
				}
			}
		}

		return 0;
	}

	void iterate_weapon_slots(vec<u64>& weapon_slots, inventory_slot_t& buffer_slot)
	{
		for (u64 weapon_slot : weapon_slots)
		{
			u64 weapon_attachment = slot::get_item(weapon_slot);

			if (!weapon_attachment)
			{
				continue;
			}

			str item_id = unity_string::to_str(item_template::get_item_id(item::get_template(weapon_attachment)));

			api::item_info_t item_info = api::get_item_info(item_id);

			buffer_slot.items.push_back({ item_id, item_info.name, item_info.short_name, item_info.avg_price == 0 ? item_info.base_price : item_info.avg_price });

			vec<u64> new_weapon_slots = unity_array::to_vec(item::get_slots_array(weapon_attachment));

			if (!new_weapon_slots.empty())
			{
				iterate_weapon_slots(new_weapon_slots, buffer_slot);
			}
		}
	};


	cached_player_t local_player_structed{};

	void cache_players()
	{


		clock_t start = clock();



		u32 i = 0;


		vec<u64> player_list_online = unity_list::to_vec(world::get_player_list(game::world));

		if (player_list_online.empty())
		{
			return;
		}


		u64 local_player_temp = memory::read<u64>(game::world + 0x148); // MainPlayer

		local_player = local_player_temp;

		if (!local_player_temp)
			sprint_ptr = 0;
		if (local_player_temp) // ClientPlayer  // HideoutPlayer
		{




			//change_mov_packet(local_player_temp);
			if (game::camera)
			{
				fix_stamina(local_player_temp);
				no_collisions(local_player_temp);
			}



			//start caching
			cached_player_t cached_player;

			vec<u64> bones{};

			bones = unity_list::to_vec(skeleton::get_transforms(body::get_skeleton(eft_player::get_body(local_player_temp))));

			//root_position
			cached_player.root_position = unity_transform::to_vec3(bones.at(u32(skeleton::bone_index::root)));

			//head_position
			cached_player.head_position = unity_transform::to_vec3(bones.at(u32(skeleton::bone_index::head)));

			//bones
			for (skeleton::bone_index bone : skeleton::skeleton_bones)
			{
				cached_player.bones.push_back(unity_transform::to_vec3(bones.at(u32(bone))));
			}

			u32 items_price = 0;

			local_player_structed = cached_player;


		}
#ifdef OFFLINE_MODE
		if (player_list_online.size() > 1)
		{
			int i = 0;
			players_buffer.clear();
			for (u64 player : player_list_online)
			{



				u64 name_pointer = memory::read<u64>(player, offset::object::name);

				char class_name[256]; memory::read_memory(name_pointer, &class_name, sizeof(class_name));
				i++;

				/*	if (str(class_name) == _("ClientPlayer") || (str(class_name) == _("LocalPlayer") || (str(class_name) == _("HideoutPlayer"))))
						continue;*/
				if (i == 1)
				{
					local_player = player;
					continue;
				}




				//start caching
				cached_player_t cached_player;


				vec<u64> bones{};

				bones = unity_list::to_vec(skeleton::get_transforms(body::get_skeleton(eft_player::get_body(player))));

				//root_position
				cached_player.root_position = unity_transform::to_vec3(bones.at(u32(skeleton::bone_index::root)));

				//head_position
				cached_player.head_position = unity_transform::to_vec3(bones.at(u32(skeleton::bone_index::head)));

				//bones
				for (skeleton::bone_index bone : skeleton::skeleton_bones)
				{
					cached_player.bones.push_back(unity_transform::to_vec3(bones.at(u32(bone))));
				}
				//u64 profile = player::get_profile(player);
				//profile::information_struct info = profile::get_information(profile);

				cached_player.name = _("scav");

				cached_player.side = _("none");

				cached_player.role = role_t::scav;

				/*if (info.role == _("scav"))
				{

				}
				else
				{
					cached_player.role = role_t::boss;
				}*/
				//weapon
				//u64 weapon = hands::get_item_in_hand(player::get_hands(player));

				//str weapon_id = unity_string::to_str(item_template::get_item_id(item::get_template(weapon)));

				//api::item_info_t weapon_info = api::get_item_info(weapon_id);

				//size_t space = weapon_info.short_name.find(' ');

				//if (space != std::string::npos)
				//{
				//	cached_player.weapon = weapon_info.short_name.erase(space);
				//}
				//else
				//{
				//	cached_player.weapon = weapon_info.short_name;
				//}


				//u32 items_price = 0;
				if (var::esp::alive::show_inventory)
				{
					vec<u64> slots = unity_array::to_vec(item::get_slots_array(inventory::get_equipment(eft_player::get_inventory(player))));

					for (u64 slot : slots)
					{
						inventory_slot_t buffer_slot;

						u64 slot_item = slot::get_item(slot);

						buffer_slot.slot_name = unity_string::to_str(slot::get_name(slot));

						if (buffer_slot.slot_name == _("SecuredContainer")
							|| buffer_slot.slot_name == _("ArmBand")
							|| buffer_slot.slot_name == _("DogTag")
							|| buffer_slot.slot_name == _("Scabbard"))
						{
							continue;
						}

						str main_item_id = unity_string::to_str(item_template::get_item_id(item::get_template(slot_item)));

						api::item_info_t main_item_info = api::get_item_info(main_item_id);

						buffer_slot.main_item = { main_item_id, main_item_info.name, main_item_info.short_name,
							main_item_info.avg_price == 0 ? main_item_info.base_price : main_item_info.avg_price,
							is_in_list(main_item_id) };



						if (slot_item)
						{
							if (buffer_slot.slot_name == _("FirstPrimaryWeapon") || buffer_slot.slot_name == _("SecondPrimaryWeapon") || buffer_slot.slot_name == _("Holster"))
							{
								vec<u64> weapon_slots = unity_array::to_vec(item::get_slots_array(slot_item));

								iterate_weapon_slots(weapon_slots, buffer_slot);
							}

							vec<u64> grids = unity_array::to_vec(item::get_grids_array(slot_item));

							for (u64 grid : grids)
							{
								vec<u64> items = unity_dictionary::to_vec(grid::get_items_dictionary(grid));

								for (u64 item : items)
								{
									if (item)
									{
										str item_id = unity_string::to_str(item_template::get_item_id(item::get_template(item)));

										api::item_info_t item_info = api::get_item_info(item_id);

										buffer_slot.items.push_back({ item_id, item_info.name, item_info.short_name,
											item_info.avg_price == 0 ? item_info.base_price : item_info.avg_price,
											is_in_list(item_id) });

									}
								}
							}

							cached_player.slots.push_back(buffer_slot);
						}
					}
					u64 weapon = hands::get_item_in_hand(eft_player::get_hands(player));

					str weapon_id = unity_string::to_str(item_template::get_item_id(item::get_template(weapon)));

					api::item_info_t weapon_info = api::get_item_info(weapon_id);

					size_t space = weapon_info.short_name.find(' ');

					if (space != std::string::npos)
					{
						cached_player.weapon = weapon_info.short_name.erase(space);
					}
					else
					{
						cached_player.weapon = weapon_info.short_name;
					}

					u32 items_price = 0;
				}


				//ammo
				//cached_player.ammo = get_ammo_name(weapon);

				////current_ammo

				players_buffer.push_back(cached_player);


			}

			auto safe_cache_players = [&](std::vector<cache_system::cached_player_t>& buffer) -> void
				{
					std::lock_guard<std::mutex> guard(item_mtx);
					players = buffer;

				};
			safe_cache_players(players_buffer);

			clock_t end = clock();

			p_time = end - start;
		}
#endif
#ifdef ONLINE_MODE
		if (game::world)
		{
			for (u64 player : player_list_online)
			{



				u64 name_pointer = memory::read<u64>(player, offset::object::name);

				char class_name[256]; memory::read_memory(name_pointer, &class_name, sizeof(class_name));
				int a = 1;

				if (str(class_name) == _("ClientPlayer") || (str(class_name) == _("LocalPlayer") || (str(class_name) == _("HideoutPlayer"))))
					continue;




				cached_player_t cached_player;


				vec<u64> bones{};

				bones = unity_list::to_vec(skeleton::get_transforms(body::get_skeleton(observer_player_view::get_body(player))));

				//root_position
				cached_player.root_position = unity_transform::to_vec3(bones.at(u32(skeleton::bone_index::root)));

				//head_position
				cached_player.head_position = unity_transform::to_vec3(bones.at(u32(skeleton::bone_index::head)));

				//bones


				for (skeleton::bone_index bone : skeleton::skeleton_bones)
				{
					cached_player.bones.push_back(unity_transform::to_vec3(bones.at(u32(bone))));
				}


				str side = observer_player_view::get_player_side(player);

				std::string name = observer_player_view::get_name(player);


				str profile_id = observer_player_view::get_profile_id(player);
				str get_account_id = observer_player_view::get_account_id(player);

				bool is_ai = observer_player_view::is_ai(player);

				if (is_ai)
				{
					std::string boss_name = observer_player_view::get_boss_name(observer_player_view::get_ws_name(player));


					if (boss_name != _(""))
					{
						cached_player.name = boss_name;

						cached_player.side = _("none");

						cached_player.role = role_t::boss;
					}

					else
					{
						cached_player.name = _("scav");

						cached_player.side = _("none");

						cached_player.role = role_t::scav;
					}
				}

				else
				{
					if (side == _("S"))
					{
						cached_player.name = _("player scav");

						cached_player.side = _("none");

						cached_player.role = role_t::player_scav;
					}
					else
					{
						cached_player.name = name;

						cached_player.side = side;

						cached_player.role = role_t::player;
					}

				}

				u64 weapon = hands::get_item_in_hand(observer_player_view::get_hands(player));

				str weapon_id = unity_string::to_str(item_template::get_item_id(item::get_template(weapon)));

				api::item_info_t weapon_info = api::get_item_info(weapon_id);

				size_t space = weapon_info.short_name.find(' ');

				if (space != std::string::npos)
				{
					cached_player.weapon = weapon_info.short_name.erase(space);
				}
				else
				{
					cached_player.weapon = weapon_info.short_name;
				}


				u32 items_price = 0;
				if (var::esp::alive::show_inventory)
				{
					vec<u64> slots = unity_array::to_vec(item::get_slots_array(inventory::get_equipment(observer_player_view::get_inventory(player))));

					for (u64 slot : slots)
					{
						inventory_slot_t buffer_slot;

						u64 slot_item = slot::get_item(slot);

						buffer_slot.slot_name = unity_string::to_str(slot::get_name(slot));

						if (buffer_slot.slot_name == _("SecuredContainer")
							|| buffer_slot.slot_name == _("ArmBand")
							|| buffer_slot.slot_name == _("DogTag")
							|| buffer_slot.slot_name == _("Scabbard"))
						{
							continue;
						}




						str main_item_id = unity_string::to_str(item_template::get_item_id(item::get_template(slot_item)));

						api::item_info_t main_item_info = api::get_item_info(main_item_id);

						buffer_slot.main_item = { main_item_id, main_item_info.name, main_item_info.short_name,
							main_item_info.avg_price == 0 ? main_item_info.base_price : main_item_info.avg_price,
							is_in_list(main_item_id) };



						if (slot_item)
						{
							if (buffer_slot.slot_name == _("FirstPrimaryWeapon") || buffer_slot.slot_name == _("SecondPrimaryWeapon") || buffer_slot.slot_name == _("Holster"))
							{
								vec<u64> weapon_slots = unity_array::to_vec(item::get_slots_array(slot_item));

								iterate_weapon_slots(weapon_slots, buffer_slot);
							}

							vec<u64> grids = unity_array::to_vec(item::get_grids_array(slot_item));

							for (u64 grid : grids)
							{
								vec<u64> items = unity_dictionary::to_vec(grid::get_items_dictionary(grid));

								for (u64 item : items)
								{
									if (item)
									{
										str item_id = unity_string::to_str(item_template::get_item_id(item::get_template(item)));

										api::item_info_t item_info = api::get_item_info(item_id);

										buffer_slot.items.push_back({ item_id, item_info.name, item_info.short_name,
											item_info.avg_price == 0 ? item_info.base_price : item_info.avg_price,
											is_in_list(item_id) });

									}
								}
							}

							cached_player.slots.push_back(buffer_slot);
						}
					}
				}


				players_buffer.push_back(cached_player);
			}

			auto safe_cache_players = [&](std::vector<cache_system::cached_player_t> buffer) -> void
				{
					std::lock_guard<std::mutex> guard(player_mtx);
					players = buffer;
					players_buffer.clear();

				};

			safe_cache_players(players_buffer);

			clock_t end = clock();

			p_time = end - start;
		}
#endif
	}

	void cache_items()
	{
		clock_t start = clock();


		if (var::esp::item::enable)
		{
			for (u64 object : unity_list::to_vec(world::get_item_list(game::world)))
			{
				cached_item_t buffer_item;

				buffer_item.position = unity_transform::to_vec3(memory::read<u64>(object, offset::object::position));

				if (buffer_item.position.y < -100)
				{
					continue;
				}

				u64 name_pointer = memory::read<u64>(object, offset::object::name);

				char class_name[256]; memory::read_memory(name_pointer, &class_name, sizeof(class_name));

				if (str(class_name) == _("LootableContainer"))
				{
					if (!var::esp::item::in_container)
					{
						continue;
					}

					buffer_item.type = loot_type_t::container;

					u64 container = memory::read<u64>(object, { 0x10, 0x28, 0x120, 0xc0 });

					u64 lootable = memory::read<u64>(object, { 0x10, 0x28, 0xfe });

					//memory::write<bool>(lootable + 0xfd, true);

					vec<u64> grids = unity_array::to_vec(item::get_grids_array(container));

					for (u64 grid : grids)
					{
						vec<u64> items = unity_dictionary::to_vec(grid::get_items_dictionary(grid));

						for (u64 item : items)
						{
							if (item)
							{
								str item_id = unity_string::to_str(item_template::get_item_id(item::get_template(item)));

								api::item_info_t item_info = api::get_item_info(item_id);

								u32 price = item_info.avg_price == 0 ? item_info.base_price : item_info.avg_price;

								if (var::esp::item::price_per_slot && item_info.width != 0 && item_info.height != 0)
								{
									price = price / (item_info.width * item_info.height);
								}


								u32 in_list = is_in_list(item_id);

								if (in_list != 0 && var::esp::item::use_price_filter && price < var::esp::item::minimal_price * 1000)
								{
									Sleep(0);
								}
								else if (in_list == 0 && price < var::esp::item::minimal_price * 1000)
								{
									Sleep(0);
								}
								else
								{
									buffer_item.items.push_back({ item_id, item_info.name, item_info.short_name, price, in_list });
								}
							}
						}
					}

					if (buffer_item.items.empty())
					{
						continue;
					}
				}
				else if (str(class_name) == _("ObservedLootItem"))
				{
					if (!var::esp::item::on_ground)
					{
						continue;
					}



					u64 item = memory::read<u64>(object, { 0x10, 0x28, 0xb0 });

					if (item_template::is_quest_item(item::get_template(item)))
					{
						buffer_item.type = loot_type_t::quest;

						inventory_item_t item;

						item.id = _("");
						item.list_id = 0;
						item.name = _("quest item");
						item.short_name = _("quest item");
						item.price = 0;

						buffer_item.items.push_back(item);

					}
					else
					{
						buffer_item.type = loot_type_t::item;
						str item_id = unity_string::to_str(item_template::get_item_id(item::get_template(item)));

						if (item_id != _("55d7217a4bdc2d86028b456d")) // corpse_id
						{
							api::item_info_t item_info = api::get_item_info(item_id);

							u32 price = item_info.avg_price == 0 ? item_info.base_price : item_info.avg_price;

							if (var::esp::item::price_per_slot && item_info.width != 0 && item_info.height != 0)
							{
								price = price / (item_info.width * item_info.height);
							}

							u32 in_list = is_in_list(item_id);

							if (in_list != 0 && var::esp::item::use_price_filter && price < var::esp::item::minimal_price * 1000)
							{
								Sleep(0);
							}
							else if (in_list == 0 && price < var::esp::item::minimal_price * 1000)
							{
								Sleep(0);
							}
							else
							{
								buffer_item.items.push_back({ item_id, item_info.name, item_info.short_name, price, in_list });
							}
						}


						if (buffer_item.items.empty())
						{
							continue;
						}
					}

				}
				else if ((str(class_name) == _("Corpse") || str(class_name) == _("ObservedCorpse")))
				{




					//test
					u64 corpse_dict = memory::read<u64>(game::world + 0b0);
					vec<u64> corpse_vec = unity_dictionary::to_vec(corpse_dict);
					//test
					if (!var::esp::item::in_corpse)
					{
						continue;
					}

					buffer_item.type = loot_type_t::corpse;

					u64 item = memory::read<u64>(object, { 0x10, 0x28, 0xb0 });

					vec<u64> slots = unity_array::to_vec(item::get_slots_array(item));

					for (u64 slot : slots)
					{
						u64 name_pointer = memory::read<u64>(slot, offset::object::name);

						char class_name[256]; memory::read_memory(name_pointer, &class_name, sizeof(class_name));

						str slot_name = unity_string::to_str(slot::get_name(slot));


						if (slot_name == _("SecuredContainer")
							|| slot_name == _("ArmBand")
							|| slot_name == _("DogTag") || slot_name == _("Scabbard"))
						{
							continue;
						}



						u64 slot_item = slot::get_item(slot);
						u64 item_template = item::get_template(slot_item);


						str main_item_id = unity_string::to_str(item_template::get_item_id(item_template));

						api::item_info_t main_item_info = api::get_item_info(main_item_id);


						if (main_item_info.name != _(""))
						{
							u32 price = main_item_info.avg_price == 0 ? main_item_info.base_price : main_item_info.avg_price;

							if (var::esp::item::price_per_slot && main_item_info.width != 0 && main_item_info.height != 0)
							{
								price = price / (main_item_info.width * main_item_info.height);
							}

							u32 in_list = is_in_list(main_item_id);

							if (in_list != 0 && var::esp::item::use_price_filter && price < var::esp::item::minimal_price * 1000)
							{
								Sleep(0);
							}
							else if (in_list == 0 && price < var::esp::item::minimal_price * 1000)
							{
								Sleep(0);
							}
							else
							{
								buffer_item.items.push_back({ main_item_id, main_item_info.name, main_item_info.short_name, price, in_list });
							}
						}

						if (slot_item)
						{
							vec<u64> grids = unity_array::to_vec(item::get_grids_array(slot_item));

							for (u64 grid : grids)
							{
								vec<u64> items = unity_dictionary::to_vec(grid::get_items_dictionary(grid));

								for (u64 item : items)
								{


									if (item)
									{

										str item_id = unity_string::to_str(item_template::get_item_id(item::get_template(item)));



										api::item_info_t item_info = api::get_item_info(item_id);

										u32 price = item_info.avg_price == 0 ? item_info.base_price : item_info.avg_price;

										if (var::esp::item::price_per_slot && item_info.width != 0 && item_info.height != 0)
										{
											price = price / (item_info.width * item_info.height);
										}

										u32 in_list = is_in_list(item_id);

										if (in_list != 0 && var::esp::item::use_price_filter && price < var::esp::item::minimal_price * 1000)
										{

										}
										else if (in_list == 0 && price < var::esp::item::minimal_price * 1000)
										{

										}
										else
										{
											buffer_item.items.push_back({ item_id, item_info.name, item_info.short_name, price, in_list });
										}
									}
								}
							}
						}
					}


				}

				items_buffer.push_back(buffer_item);
			}

			auto safe_cache_items = [&](std::vector<cache_system::cached_item_t> buffer) -> void
				{
					std::lock_guard<std::mutex> guard(item_mtx);
					items = buffer;
					items_buffer.clear();

				};

			//items = buffer;
			safe_cache_items(items_buffer);
		}


		clock_t end = clock();

		i_time = end - start;
	}

	void cache_grenades()
	{
		clock_t start = clock();



		for (uint64_t object : unity_list::to_vec(grenade_controller::get_grenade_list(world::get_grenade_controller(game::world))))
		{
			cached_grenade_t buffer_grenade;

			buffer_grenade.position = unity_transform::to_vec3(memory::read<u64>(object, offset::object::position));

			grenades_buffer.push_back(buffer_grenade);
		}
		auto safe_cache_grenade = [&](std::vector<cache_system::cached_grenade_t> buffer) -> void
			{
				std::lock_guard<std::mutex> guard(grenade_mtx);
				grenades = buffer;
				grenades_buffer.clear();

			};
		safe_cache_grenade(grenades_buffer);

		clock_t end = clock();

		g_time = end - start;
	};

	void cache_exits()
	{
		clock_t start = clock();



		for (u64 exit : unity_array::to_vec(exit_controller::get_exit_array(world::get_exit_controller(game::world))))
		{
			cached_exit_t buffer_exit;

			buffer_exit.position = unity_transform::to_vec3(memory::read<u64>(exit, offset::object::position));

			buffer_exit.name = unity_string::to_str(exit_object::get_name(exit));

			buffer_exit.status = exit_object::get_status(exit);

			exits_buffer.push_back(buffer_exit);
		}
		auto safe_cache_exit = [&](std::vector<cache_system::cached_exit_t> buffer) -> void
			{
				std::lock_guard<std::mutex> guard(exit_mtx);
				exits = buffer;
				exits_buffer.clear();

			};
		safe_cache_exit(exits_buffer);


		clock_t end = clock();

		e_time = end - start;
	};
}