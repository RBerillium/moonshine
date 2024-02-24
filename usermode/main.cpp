#include "main.hpp"

#include "tarkov/tarkov.hpp"
#include "main/render_system.hpp"
#include "main/memory_system.hpp"
#include "main/cache_system.hpp"
#include "main/file_system.hpp"

#include "visual/bind.hpp"
#include "visual/menu.hpp"
#include "visual/visual.hpp"
#include "visual/image.hpp"
#include <math.h>


bool initialized = false;



namespace overlay = render_system;

namespace memory = memory_system;




#define RAD_TO_DEG 180.f / 3.141592654f

void main()
{
	memory::create_config();
	memory::wait_complete();
	//memory::wait_complete1();
	memory::delete_config();

	LoadLibraryA(_("user32.dll")); HMODULE module_win32u = LoadLibraryA(_("win32u.dll"));

	if (module_win32u)
	{

	}
	
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	u32 process_identifier = 0;


	var::config::custom_config default_settings;

	default_settings.name = _("default(automatic)");

	//file_system::save_settings(default_settings);

	file_system::load_custom_list(var::path_item_list);

	file_system::load_config_list(var::path_config_list);

	file_system::load_item_library(_("C:\\Windows\\output.json"), var::language_t::en);





	if (!overlay::initialization())
	{
		return;
		exit(-3);
	}
	
	

	D3DX11CreateShaderResourceViewFromMemory(overlay::device, aim_skeleton, sizeof(aim_skeleton), &aim_skeleton_info, aim_skeleton_thread_pump, &aim_skeleton_shader, 0);


	// mouse and keyboard ~~
	std::thread(
		[&]()
		{
			while (true)
			{
				ImGuiIO& io = ImGui::GetIO();

				for (int i = 5; i < 512; i++)
				{
					if (GetAsyncKeyState(i) && 0x8000)
					{
						io.KeysDown[i] = true;

						io.AddInputCharacterUTF16(tolower(i));

						std::this_thread::sleep_for(std::chrono::milliseconds(200));
					}
				}

				POINT mouse_position; GetCursorPos(&mouse_position); ScreenToClient(overlay::window, &mouse_position);

				io.MousePos.x = mouse_position.x; io.MousePos.y = mouse_position.y;

				if (GetAsyncKeyState(VK_LBUTTON) && 0x8000)
				{
					io.MouseDown[0] = true;
				}
				else
				{
					io.MouseDown[0] = false;
				};

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	).detach();



	//cache players and grenades 1
	std::thread(
		[&]()
		{
			

			while (true)
			{
				if (initialized)
				{
					if (var::end_of_life)
					{
						
						break;
					}

					try
					{
						game::update();

					}
					catch (const std::exception& ex)
					{
						continue;
					}
					
					if (game::world)
					{
						try
						{
							
							cache_system::cache_players();

							cache_system::cache_grenades();

							if (game::camera && game::world)
							{


								if (local_player)
								{
									try
									{

										if (var::misc::inertia && local_player) //
										{
											app::disable_inertia(game::application);
										}
										else if (local_player)
										{
											app::enable_inertia(game::application);
										}

										if (var::misc::loot_walls && local_player) //
										{
											//u64 eft_hard_settings = memory::read<u64>(mono::find_class(_("Assembly-CSharp"), _("EFTHardSettings"))->get_vtable(mono::get_root_domain())->get_static_field_data());

											//u64 game_world_instance = mono::find_class(_("Assembly-CSharp"), _("EFT.GameWorld"))->get_vtable(mono::get_root_domain())->get_static_field_data();



											memory::write<f32>(game::game_world_instance + offset::raycast_loot::instance, 0);

											memory::write<f32>(game::eft_hard_settings + offset::raycast_loot::distance, 5.0f);

										}
										else if (local_player)
										{
											//u64 eft_hard_settings = memory::read<u64>(mono::find_class(_("Assembly-CSharp"), _("EFTHardSettings"))->get_vtable(mono::get_root_domain())->get_static_field_data());
											//u64 game_world_instance = mono::find_class(_("Assembly-CSharp"), _("EFT.GameWorld"))->get_vtable(mono::get_root_domain())->get_static_field_data();

											memory::write<f32>(game::game_world_instance + offset::raycast_loot::instance, 0);

											memory::write<f32>(game::eft_hard_settings + offset::raycast_loot::distance, 1.f);
										}

										//if (var::misc::speed_x50 && !var::misc::speed && local_player) //
										//{

										//	u64 time = (offset::time_scale + (7 * 8));
										//	u64 time_scale = memory::read<u64>(game::unity_player_module + time);
										//	memory::write<f32>(time_scale + 0xFC, 10.f);
										//	if (local_player != 0)
										//	{
										//		u64 pedometer = memory::read<u64>(local_player + 0x48);
										//		byte state = memory::read<byte>(pedometer + 0x3c);
										//		if (true)//state == 0x5
										//		{
										//			//memory::write<bool>(game::world + 0x1f0, false);
										//			u64 character_controller = eft_player::get_character_controller(local_player);

										//			memory::write<f32>(character_controller + 0x74, (float)var::misc::speed_multiply); // _speedLimit
										//			memory::write<f32>(character_controller + 0x78, 0.31200000643730164f); //sqrSpeedLimit
										//		}
										//		//else
										//		//memory::write<bool>(game::world + 0x1f0, true);

										//	}
										//}

										if (var::misc::speed && local_player) //
										{
											u64 time = (offset::time_scale + (7 * 8));
											u64 time_scale = memory::read<u64>(game::unity_player_module + time);
											memory::write<f32>(time_scale + 0xFC, 1.8f);
										}
										else if (local_player) //
										{
											u64 time = (offset::time_scale + (7 * 8));
											u64 time_scale = memory::read<u64>(game::unity_player_module + time);
											memory::write<f32>(time_scale + 0xFC, 1.0f);
										}

										if (var::misc::night && local_player) //
										{
											night::enable(game::night_vision);
										}
										else if (local_player)
										{
											night::disable(game::night_vision);
										}

										if (var::misc::thermal && local_player)
										{
											thermal::enable(game::thermal_vision);
										}
										else if (local_player)
										{
											thermal::disable(game::thermal_vision);
										}

										if (var::misc::visor && local_player)
										{
											visor::disable(game::visor_effect);
										}
										else if (local_player)
										{
											visor::enable(game::visor_effect);
										}

										if (var::misc::recoil && local_player) //!!
										{
											u64 pwa = eft_player::get_pwa(local_player);

											firearm::set_weapon_length(pwa::get_firearm(pwa), 0.0001f);
											if (pwa)
												pwa::recoil_disable(pwa);
										

										}
										else if (local_player)
										{
											u64 pwa = eft_player::get_pwa(local_player);
											if (pwa)
												pwa::recoil_enable(pwa);

										}

										if (var::misc::stamina && local_player)
										{
											u64 physical = eft_player::get_physical(local_player);
											if (physical)
												physical::set_ulimited_stamina(physical);
										}

											if (var::misc::jump && local_player)
											{
												//skills::enable_fast_loot(profile::get_skills(eft_player::get_profile(local_player)));
												//skills::enable_super_jump(profile::get_skills(eft_player::get_profile(local_player)));
											}
											else if(local_player)
											{
													//skills::disable_super_jump(profile::get_skills(eft_player::get_profile(local_player)));
											}
									}
									catch (const std::exception& ex)
									{

									}
								}


							}
						}
						catch (const std::exception& ex)
						{
							continue;
						}
					}

				
					if (game::camera && local_player && var::aim::enable)
					{
						try
						{

							auto calc_angle = [&](vec3 source, vec3 destination) -> vec2
								{
									vec3 difference = source - destination;
									float length = difference.length();

									vec2 ret{};

									ret.y = asinf(difference.y / length);
									ret.x = -atan2f(difference.x, -difference.z);

									return { ret.x * 57.29578f, ret.y * 57.29578f };

								};


							float best_center_distance = FLT_MAX;

							float best_player_distance = FLT_MAX;

							vec3 target = { 0, 0, 0 };

							u64 target_player = 0;

							for (u64 player : unity_list::to_vec(world::get_player_list(game::world)))
							{
								if (player == local_player)
								{
									continue;
								}

								vec3 aim_bone = unity_transform::to_vec3(unity_list::to_vec(
									skeleton::get_transforms(body::get_skeleton(observer_player_view::get_body(player)))).at(var::aim::current_bone_id)); //// EFT.Player for offline

								if (aim_bone == vec3(0, 0, 0))
								{
									continue;
								}

								float buffer_player_distance = unity_transform::to_vec3(firearm::get_fireport(pwa::get_firearm(eft_player::get_pwa(local_player)))).distance(aim_bone);

								float buffer_center_distance = camera::w2s(aim_bone).distance(vec2(screen_x / 2, screen_y / 2));

								if (var::aim::priority == var::aim::priority_type::distance)
								{
									if (buffer_center_distance < var::aim::fov_radius && buffer_player_distance < best_player_distance && buffer_player_distance <= var::aim::distance)
									{
										best_player_distance = buffer_player_distance;
										target = aim_bone;
										target_player = player;
									}
								}
								else if (var::aim::priority == var::aim::priority_type::center)
								{
									if (buffer_center_distance < var::aim::fov_radius && buffer_center_distance < best_center_distance && buffer_player_distance <= var::aim::distance)
									{
										best_center_distance = buffer_center_distance;
										target = aim_bone;
										target_player = player;
									}
								}
							}


							auto calc_angle1 = [&](vec3 source, vec3 destination) -> vec2
								{
									vec3 difference = source - destination;
									float length = difference.length();

									vec2 ret{};

									ret.y = asinf(difference.y / length);
									ret.x = -atan2f(difference.x, -difference.z);

									return ret;

								};







							if (target != vec3(0, 0, 0))
							{
								vec2 screen_target = camera::w2s(target);

								if (var::aim::aim_line)
								{
									overlay::line(screen_x / 2, screen_y / 2, screen_target.x, screen_target.y, var::main_color);
								}
								///TEST PART;




								if (!var::aim::silent_aim)
								{
									if (GetAsyncKeyState(var::aim::bind_aim) && 0x8000)
									{
										vec3 fireport = unity_transform::to_vec3(firearm::get_fireport(pwa::get_firearm(eft_player::get_pwa(local_player))));





										f32 distance = fireport.distance(target);

										f32 bullet_speed = memory::read<float>(local_player, { offset::eft_player::hands, offset::hands::item,
												offset::item::item_template, offset::item_template::ammo_template, offset::ammo_template::speed });

										/*	if (var::aim::fast_bullets)
											{
												u64 bullet_speed_factor = memory::read<u64>(local_player, { offset::eft_player::hands, offset::hands::item,
													offset::item::item_template, offset::item_template::ammo_template });
												memory::write<float>(bullet_speed_factor + offset::ammo_template::speed, 2300.f);
											}*/

										if (var::aim::prediction && distance > var::aim::predict_distance && bullet_speed > 0)
										{
											vec3 target_speed = memory::read<vec3>(target_player, offset::observed_player_view::velocity);

											f32 travel_time = distance / bullet_speed;

											target_speed = target_speed * travel_time;

											target = target + target_speed;

										}

										if (var::aim::prediction && distance > var::aim::predict_distance && bullet_speed > 0)
										{
											f32 travel_time = distance / bullet_speed;

											f32 bullet_drop = 0.5f * 9.81f * travel_time * travel_time;

											target.y += bullet_drop;
										}

										movement::set_view_angles(eft_player::get_movement(local_player), calc_angle(fireport, target));

									}
								}

								else
								{
									vec2 screen_target = camera::w2s(target);

									if (var::aim::aim_line)
									{
										overlay::line(screen_x / 2, screen_y / 2, screen_target.x, screen_target.y, var::main_color);
									}



									vec3 fireport = unity_transform::to_vec3(firearm::get_fireport(pwa::get_firearm(eft_player::get_pwa(local_player))));

									u64 fireport_ptr = firearm::get_fireport(pwa::get_firearm(eft_player::get_pwa(local_player)));


									f32 distance = fireport.distance(target);

									f32 bullet_speed = memory::read<float>(local_player, { offset::eft_player::hands, offset::hands::item,
											offset::item::item_template, offset::item_template::ammo_template, offset::ammo_template::speed });



									if (var::aim::prediction && distance > var::aim::predict_distance && bullet_speed > 0)
									{
										vec3 target_speed = memory::read<vec3>(target_player, offset::observed_player_view::velocity);

										f32 travel_time = distance / bullet_speed;

										target_speed = target_speed * travel_time;

										target = target + target_speed;

									}

									if (var::aim::prediction && distance > var::aim::predict_distance && bullet_speed > 0)
									{
										f32 travel_time = distance / bullet_speed;

										f32 bullet_drop = 0.5f * 9.81f * travel_time * travel_time;

										target.y += bullet_drop;
									}

									u64 pwa = eft_player::get_pwa(local_player);
									u64 hands_container = memory::read<u64>(pwa + 0x18);
									u64 recoil = memory::read<u64>(hands_container + 0x38);
									memory::write<float>(recoil + 0x38, 0.f); // ReturnSpeed
									memory::write<float>(recoil + 0x3c, 0.f); //Damping



									vec2 angle = calc_angle(fireport, target);

									vec2 view_angles = movement::get_view_angles(eft_player::get_movement(local_player));
									vec2 res = { angle.x - view_angles.x, angle.y - view_angles.y };


									memory::write<float>(recoil + 0x80, res.y);
									memory::write <float>(recoil + 0x88, res.x);

									//vec3 _shot_direction = memory::read<vec3>(eft_player::get_pwa(local_player) + 0x218);
									//vec2 angle = calc_angle1(fireport, target);
									//vec2 local_player_angles = movement::get_view_angles(eft_player::get_movement(local_player));
									//vec2 angle_radians = { (float)(local_player_angles.x * M_PI / 180.f) - angle.x ,(float)(local_player_angles.y * M_PI / 180.f) - angle.y };
									//

									//float a =  -sin(angle_radians.x); // - 0.35f * - sin(angle_radians.x)
									//float z = sin(angle_radians.y); //  - 0.35f * sin(angle_radians.y)

									//vec3 shot_direction_result = { a  ,-1, z  };

									//memory::write<vec3>(eft_player::get_pwa(local_player) + 0x218, shot_direction_result);

								}
							}
							else
							{
								u64 pwa = eft_player::get_pwa(local_player);
								u64 hands_container = memory::read<u64>(pwa + 0x18);
								u64 recoil = memory::read<u64>(hands_container + 0x38);

								memory::write<float>(recoil + 0x38, 0.1f);
								memory::write <float>(recoil + 0x3c, 0.5f);

							}



						}
						catch (const std::exception& ex)
						{
							continue;
						}
					}
	
				}
				else
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
			
			}
		}
	).detach();
	

	//aim thread test

	//std::thread(
	//	[&]()
	//	{
	//	
	//		while (true)
	//		{
	//			if (initialized)
	//			{
	//			
	//			}
	//			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	//		}
	//		
	//		
	//	}).detach();
	// cache items and exits and update world 2
	std::thread(
		[&]()
		{
	

			while (true)
			{
				if (initialized)
				{
					if (var::end_of_life)
					{
					

						break;
					}

					// update
				

					// cahing
					if (game::camera)
					{
						try
						{
							cache_system::cache_items();

							cache_system::cache_exits();
						}
						catch (const std::exception& ex)
						{

						}
					}

					// miscs
				

					std::this_thread::sleep_for(std::chrono::milliseconds(3000));
				}
				else
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				
			}
		}
	).detach();



	while (!process_identifier)
	{
		process_identifier = memory::get_process_by_name(_("EscapeFromTarkov.exe"));
	}

	Sleep(5000);
    memory::init(process_identifier);
	uint64_t mono_module = 0;
	uint64_t unity_player_module = 0;
	memory::get_base_values(&mono_module, &unity_player_module);
	game::initialization(mono_module, unity_player_module);
	mono_copy = mono_module;


	
	//game::initialization(unity_player_module, mono_module);
	initialized = true;
	// render and aim and update matrix

	while (true)
	{
		if (initialized)
		{
			
			if (var::end_of_life)
			{
				

				Sleep(10000);

				//system(_("TASKKILL /IM \"MedalEncoder.exe\" /F"));

				break;
			}

			// overlay
			overlay::start();
			{
				binds();

				menu();

				// render
				//printf(_("camera is %llx\n"), game::camera);
				//printf(_("world is %llx\n"), game::world);

				if (!var::esp::item::edit_window && game::camera)
				{
					try
					{

						camera::cache_matrix();
						render();
					}
					catch (const std::exception& ex)
					{
						
					}
				}

				// aim
	
			}
			f32 fps = ImGui::GetIO().Framerate;

			overlay::end();

			//f32 ms = 1000.f / fps;

			//if (ms < 10.f)
			//{
			//	std::this_thread::sleep_for(std::chrono::milliseconds(10 - (u32)ms));
			//}
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	}
	
	memory::unmap();
	return;
}

extern "C" __declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinstdll, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH: CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), 0, 0, 0); break;
	}
	return TRUE;
}