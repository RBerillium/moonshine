#pragma once

#include "../main.hpp"
#define M_PI  3.14159265358979323846  
namespace overlay = render_system;



u32 p = var::font_size - 1.5f;
cache_system::cached_player_t cached_local_player{};

void render()
{	
	ImGui::PushFont(overlay::ammo_font);

	if (cache_system::local_player_structed.max_ammo >= 0 && !cache_system::local_player_structed.ammo.empty())
	{
		overlay::text((screen_x / 4) * 3, screen_y - 100, var::main_color, _("%d/%d %s"), cache_system::local_player_structed.current_ammo, cache_system::local_player_structed.max_ammo, cache_system::local_player_structed.ammo.c_str());
	}

	ImGui::PushFont(overlay::main_font);

	if (var::aim::fov)
	{
		overlay::circle(screen_x / 2, screen_y / 2, var::aim::fov_radius, var::main_color, 36);
	}

	std::vector<cache_system::cached_player_t> players = cache_system::get_players();
	
	if (true)
	{
		auto bone = [&](vec2 bone1, vec2 bone2, ImColor color) -> void
		{
			overlay::line(bone1.x, bone1.y, bone2.x, bone2.y, color);
		};

		auto skeleton = [&](std::vector<vec3>& bones, ImColor color) -> void
		{
			std::vector<vec2> s_b;

			if (bones.empty())
			{
				return;
			}

			for (vec3 bone : bones)
			{
				s_b.push_back(camera::w2s(bone));
			}

			for (vec2 screen_bone : s_b)
			{
				if (screen_bone.x <= 0 || screen_bone.y <= 0)
				{
					return;
				}
			}

			bone(s_b.at(0), s_b.at(1), color); bone(s_b.at(1), s_b.at(2), color); bone(s_b.at(4), s_b.at(3), color);
			bone(s_b.at(3), s_b.at(2), color); bone(s_b.at(2), s_b.at(5), color); bone(s_b.at(5), s_b.at(6), color);
			bone(s_b.at(5), s_b.at(7), color); bone(s_b.at(7), s_b.at(8), color); bone(s_b.at(8), s_b.at(9), color);
			bone(s_b.at(5), s_b.at(10), color); bone(s_b.at(10), s_b.at(11), color); bone(s_b.at(11), s_b.at(12), color);

		};

		auto box = [&](vec2 base, vec2 head, ImColor color) -> void
		{
			const float box_height = abs(head.y - base.y);
			const float box_width = box_height * 0.75f;
			overlay::rect(head.x - (box_width * 0.5f), head.y, box_width, box_height, color);
		};

		u32 i = 0;
		if (true)
		{
			cached_local_player = cache_system::local_player_structed;
			local_player_position = cache_system::local_player_structed.root_position;

		}
		if (var::esp::show_radar)
		{
			auto calc_angle = [&](vec3 source, vec3 destination) -> vec2
				{
					vec3 difference = source - destination;
					float length = difference.length();

					vec2 ret{};

					ret.y = asinf(difference.y / length);
					ret.x = -atan2f(difference.x, -difference.z);

					return ret;

				};
			ImGui::SetNextWindowSize(ImVec2(250, 250));	
			
			ImVec2 window_size = { 250, 250 };
			ImGui::SetNextWindowBgAlpha(0.4f);
			ImGui::SetWindowPos(ImVec2(screen_x - 250, 50));	
			ImGui::Begin(_("Radar"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
			
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
			
			ImVec2 window_pos = ImGui::GetWindowPos();
			ImVec2 local_pos = ImVec2(window_size.x / 2, window_size.y/2);
			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(window_pos.x + local_pos.x, window_pos.y + local_pos.y), 3.f, ImColor(238,210,2), 5);
			//ImGui::GetWindowDrawList()->AddLine(ImVec2(window_pos.x + local_pos.x, window_pos.y + local_pos.y), ImVec2(window_pos.x + local_pos.x, window_pos.y + local_pos.y - 20), ImColor(0, 170, 0), 0.2f);
			ImGui::GetWindowDrawList()->AddLine(ImVec2((window_pos.x + local_pos.x), (window_pos.y + window_size.y)), ImVec2((window_pos.x + local_pos.x), (window_pos.y)), ImColor(0, 0, 0),1.5f);
			ImGui::GetWindowDrawList()->AddLine(ImVec2((window_pos.x ), (window_pos.y + window_size.y/2)), ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y/2), ImColor(0, 0, 0), 1.5f);
			
			if (game::local_player_angles.x != 0 || game::local_player_angles.y != 0)
			{
				for (cache_system::cached_player_t player : players)
				{

					ImColor color{};

					if (player.role == cache_system::role_t::player)
						color = var::esp::alive::color_players;

					else if (player.role == cache_system::role_t::scav)
						color = var::esp::alive::color_scavs;

					else if (player.role == cache_system::role_t::player_scav)
						color = var::esp::alive::color_scav_players;

					else if (player.role == cache_system::role_t::boss)
						color = var::esp::alive::color_bosses;

					else
						color = ImColor(255, 255, 255);





					float angle_radians = calc_angle(local_player_position, player.root_position).x;
					angle_radians = ((game::local_player_angles.x * M_PI / 180.f) - angle_radians);


					float distance = local_player_position.distance(player.root_position);


					ImVec2 point_pos = {};

					point_pos.x = local_pos.x - distance * sin(angle_radians);
					point_pos.y = local_pos.y - distance * cos(angle_radians);




					ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(window_pos.x + point_pos.x, window_pos.y + point_pos.y), 3.f, color, 5);

				}
			}
			
			

			//style.WindowBorderSize = 0.0f;
			ImGui::End();


		}
		
		if (!players.empty())
		{
			for (cache_system::cached_player_t player : players)
			{
			
				//i++;

				//if (i == 1) // local player
				//{
				//	cached_local_player = player;

				//	

				//	continue;
				//}

				vec2 root_screen_position = camera::w2s(player.root_position);

				vec2 head_screen_position = camera::w2s(player.head_position);

				if (head_screen_position.x <= 0 || head_screen_position.x >= screen_x || head_screen_position.y <= 0 || head_screen_position.y >= screen_y)
				{
					continue;
				}

				float distance = player.root_position.distance(local_player_position);

				if (distance > var::esp::alive::distance)
				{
					continue;
				}
				cache_system::role_t role = player.role;

				if (player.role == cache_system::role_t::player)
				{
					if (var::esp::alive::players)
					{
						if (var::esp::alive::minimal)
						{
							if (var::esp::alive::type_global::box && var::esp::alive::type_players::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_players);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_players::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_players);
							}
						}
						else
						{
							overlay::text(head_screen_position.x, head_screen_position.y - p - 2.0f, var::esp::alive::color_players,
								_("%0.fm"), distance);

							uint32_t k = -1;

							if (var::esp::alive::type_global::box && var::esp::alive::type_players::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_players);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_players::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_players);
							}

							if (var::esp::alive::type_global::info && var::esp::alive::type_players::skeleton)
							{
								k++;

								char kd[32]; sprintf(kd, _("%0.1f"), player.kd);

								std::string info =
									std::string(_("[") /*+ std::to_string(player.level)*/ + player.side /*+kd*/ + _("]") +
										player.name);

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_players,
									_("%s"), info.c_str());
							}

							if (var::esp::alive::type_global::weapon && var::esp::alive::type_players::weapon)
							{
								k++;

								std::string weapon{};

								if (player.max_ammo > 0)
								{
									/*	weapon =
											std::string(_("[") + std::to_string(player.current_ammo) + _("-") + std::to_string(player.max_ammo) + _("|") + player.ammo + _("]") +
												player.weapon);*/
									weapon = player.weapon;
								}
								else
								{
									weapon = player.weapon;
								}

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_players,
									_("%s"), weapon.c_str());
							}

							if (var::esp::alive::type_global::price && var::esp::alive::type_players::price)
							{
								k++;

								/*	overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_players,
										_("%sk"), std::to_string(player.price / 1000).c_str());*/
							}
						}
					}
				}
				else if (player.role == cache_system::role_t::player_scav)
				{
					if (var::esp::alive::scav_players)
					{
						if (var::esp::alive::minimal)
						{
							if (var::esp::alive::type_global::box && var::esp::alive::type_scav_players::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_scav_players);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_scav_players::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_scav_players);
							}
						}
						else
						{
							uint32_t k = -1;

							overlay::text(head_screen_position.x, head_screen_position.y - p - 2.0f, var::esp::alive::color_scav_players,
								_("%0.fm"), distance);

							if (var::esp::alive::type_global::box && var::esp::alive::type_scav_players::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_scav_players);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_scav_players::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_scav_players);
							}

							if (var::esp::alive::type_global::info && var::esp::alive::type_scav_players::info)
							{
								k++;

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_scav_players,
									_("%s"), player.name.c_str());
							}

							if (var::esp::alive::type_global::weapon && var::esp::alive::type_scav_players::weapon)
							{
								k++;

								std::string weapon{};

								if (player.max_ammo > 0)
								{
									/*weapon =
										std::string(_("[") + std::to_string(player.current_ammo) + _("-") + std::to_string(player.max_ammo) + _("|") + player.ammo + _("]") +
											player.weapon);*/
									weapon = player.weapon;
								}
								else
								{
									weapon = player.weapon;
								}

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_scav_players,
									_("%s"), weapon.c_str());

							}

							if (var::esp::alive::type_global::price && var::esp::alive::type_scav_players::price)
							{
								k++;

								/*overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_scav_players,
									_("%sk"), std::to_string(player.price / 1000).c_str());*/
							}
						}
					}
				}
				else if (player.role == cache_system::role_t::scav)
				{
					if (var::esp::alive::scavs)
					{
						if (var::esp::alive::minimal)
						{
							if (var::esp::alive::type_global::box && var::esp::alive::type_scavs::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_scavs);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_scavs::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_scavs);
							}
						}
						else
						{
							uint32_t k = -1;

							overlay::text(head_screen_position.x, head_screen_position.y - p - 2.0f, var::esp::alive::color_scavs,
								_("%0.fm"), distance);

							if (var::esp::alive::type_global::box && var::esp::alive::type_scavs::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_scavs);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_scavs::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_scavs);
							}

							if (var::esp::alive::type_global::info && var::esp::alive::type_scavs::info)
							{
								k++;

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_scavs,
									_("%s"), player.name.c_str());
							}

							if (var::esp::alive::type_global::weapon && var::esp::alive::type_scavs::weapon)
							{
								k++;

								std::string weapon{};

								if (player.max_ammo > 0)
								{
									/*weapon =
										std::string(_("[") + std::to_string(player.current_ammo) + _("-") + std::to_string(player.max_ammo) + _("|") + player.ammo + _("]") +
											player.weapon);*/
									weapon = player.weapon;
								}
								else
								{
									weapon = player.weapon;
								}

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_scavs,
									_("%s"), weapon.c_str());

							}

							if (var::esp::alive::type_global::price && var::esp::alive::type_scavs::price)
							{
								k++;

								/*	overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_scavs,
										_("%sk"), std::to_string(player.price / 1000).c_str());*/
							}
						}
					}
				}
				else if (player.role == cache_system::role_t::boss)
				{
					if (var::esp::alive::bosses)
					{
						if (var::esp::alive::minimal)
						{
							if (var::esp::alive::type_global::box && var::esp::alive::type_bosses::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_bosses);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_bosses::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_bosses);
							}
						}
						else
						{
							uint32_t k = -1;

							overlay::text(head_screen_position.x, head_screen_position.y - p - 2.0f, var::esp::alive::color_bosses,
								_("%0.fm"), distance);

							if (var::esp::alive::type_global::box && var::esp::alive::type_bosses::box)
							{
								box(root_screen_position, head_screen_position, var::esp::alive::color_bosses);
							}

							if (var::esp::alive::type_global::skeleton && var::esp::alive::type_bosses::skeleton)
							{
								skeleton(player.bones, var::esp::alive::color_bosses);
							}

							if (var::esp::alive::type_global::info && var::esp::alive::type_bosses::info)
							{
								k++;

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_bosses,
									_(u8"%s"), player.name.c_str());
							}

							if (var::esp::alive::type_global::weapon && var::esp::alive::type_bosses::weapon)
							{
								k++;

								std::string weapon{};

								if (player.max_ammo > 0)
								{
									/*weapon =
										std::string(_("[") + std::to_string(player.current_ammo) + _("-") + std::to_string(player.max_ammo) + _("|") + player.ammo + _("]") +
											player.weapon);*/
									weapon = player.weapon;
								}
								else
								{
									weapon = player.weapon;
								}

								overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_bosses,
									_("%s"), weapon.c_str());

							}

							if (var::esp::alive::type_global::price && var::esp::alive::type_bosses::price)
							{
								k++;

								/*	overlay::text(root_screen_position.x, root_screen_position.y + p * k + 2.f, var::esp::alive::color_bosses,
										_("%sk"), std::to_string(player.price / 1000).c_str());*/
							}
						}
					}
				}

				if ((root_screen_position.distance(vec2(screen_x / 2, screen_y / 2)) <= var::aim::fov_radius) && GetAsyncKeyState(var::esp::alive::bind_show_inventory))
				{
					auto get_color = [&](uint32_t price)
						{
							if (price >= 0 && price <= 50000) return var::esp::item::color_50;
							else if (price > 50000 && price <= 100000) return var::esp::item::color_100;
							else if (price > 100000 && price <= 150000) return var::esp::item::color_150;
							else if (price > 150000 && price <= 200000) return var::esp::item::color_200;
							else if (price > 200000) return var::esp::item::color_1000;
						};

					auto center_text = [&](const char* format, ...)
						{
							va_list list; char text[1024] = { 0 };
							va_start(list, format); _vsnprintf_s(text, sizeof(text), format, list); va_end(list);

							ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2);
							ImGui::Text(text);
						};

					auto center_colored_text = [&](ImColor color, const char* format, ...)
						{
							va_list list; char text[1024] = { 0 };
							va_start(list, format); _vsnprintf_s(text, sizeof(text), format, list); va_end(list);

							ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2);
							ImGui::TextColored(color, text);
						};

					ImGui::SetNextWindowSize(ImVec2(350, 0));

					ImGui::Begin(_("inventory"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

					ImGui::Separator();

					center_colored_text(var::main_color, _("inventory of %s"), player.name.c_str());

					ImGui::Separator();

					for (cache_system::inventory_slot_t slot : player.slots)
					{
						if (slot.main_item.name != _(""))
						{
							str full_text = slot.slot_name + _(" - ") + slot.main_item.short_name + _(" | ") + std::to_string(slot.main_item.price / 1000) + _("k");

							ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(full_text.c_str()).x) / 2);

							ImGui::TextColored(var::main_color, _("%s - "), slot.slot_name.c_str());

							ImGui::SameLine();

							ImGui::TextColored(get_color(slot.main_item.price), _("%s | %dk"), slot.main_item.short_name.c_str(), slot.main_item.price / 1000);
						}
						else
						{
							center_colored_text(var::main_color, _("%s - none"), slot.slot_name.c_str());
						}

						ImGui::Separator();

						for (cache_system::inventory_item_t item : slot.items)
						{
							center_colored_text(get_color(item.price), _("%s | %dk"), item.short_name.c_str(), item.price / 1000);
						}

						if (!slot.items.empty())
						{
							ImGui::Separator();
						}
					}

					ImGui::Spacing();

					ImGui::End();
				}
			}
		}
		

		if (!cache_system::get_items().empty() && var::esp::item::enable)
		{
			auto get_color = [&](uint32_t price)
			{
				if (price >= 0 && price <= 50000) return var::esp::item::color_50;
				else if (price > 50000 && price <= 100000) return var::esp::item::color_100;
				else if (price > 100000 && price <= 150000) return var::esp::item::color_150;
				else if (price > 150000 && price <= 200000) return var::esp::item::color_200;
				else if (price > 200000) return var::esp::item::color_1000;
			};

			std::vector<cache_system::cached_item_t> items = cache_system::get_items();

			for (cache_system::cached_item_t item : items)
			{
				vec2 screen_position = camera::w2s(item.position);

				if (screen_position.x <= 0 || screen_position.x >= screen_x || screen_position.y <= 0 || screen_position.y >= screen_y)
				{
					continue;
				}

				float distance = item.position.distance(local_player_position);

				if (distance > var::esp::item::distance)
				{
					continue;
				}
				if (item.type == cache_system::loot_type_t::quest && var::esp::item::quest_items)
				{
					if (item.items.empty())
					{
						continue;
					}

					overlay::text(screen_position.x, screen_position.y, ImColor(0,200,0), _("%s[%0.fm]%s"),
						var::esp::item::name ? item.items.at(0).short_name.c_str() : _(""), distance,
						var::esp::item::price ? std::string(std::to_string(item.items.at(0).price / 1000) + _("k")).c_str() : _(""));

				}
				else if (item.type == cache_system::loot_type_t::item && var::esp::item::on_ground)
				{
					if (item.items.empty())
					{
						continue;
					}

					if (!item.items.at(0).list_id)
					{
						if (item.items.at(0).price >= var::esp::item::minimal_price * 1000)
						{
							overlay::text(screen_position.x, screen_position.y, get_color(item.items.at(0).price), _("%s[%0.fm]%s"),
								var::esp::item::name ? item.items.at(0).short_name.c_str() : _(""), distance,
								var::esp::item::price ? std::string(std::to_string(item.items.at(0).price / 1000) + _("k")).c_str() : _(""));
						}
					}
					else
					{
						
						if ((item.items.at(0).price > var::esp::item::minimal_price * 1000 && var::esp::item::use_price_filter) ||
							!var::esp::item::use_price_filter)
						{
							overlay::text(screen_position.x, screen_position.y, var::esp::item::custom_item_lists.at(item.items.at(0).list_id - 1).color, _("%s[%0.fm]%s"),
								var::esp::item::name ? item.items.at(0).short_name.c_str() : _(""), distance,
								var::esp::item::price ? std::string(std::to_string(item.items.at(0).price / 1000) + _("k")).c_str() : _(""));
						}
					}

				}
				else if (item.type == cache_system::loot_type_t::container && var::esp::item::in_container)
				{
					uint32_t i = -1;

					for (cache_system::inventory_item_t container_item : item.items)
					{
						if (!container_item.list_id)
						{
							if (container_item.price >= var::esp::item::minimal_price * 1000)
							{
								i++;

								overlay::text(screen_position.x, screen_position.y + i * p, get_color(container_item.price), _("%s[%0.fm]%s"),
									var::esp::item::name ? container_item.short_name.c_str() : _(""), distance,
									var::esp::item::price ? std::string(std::to_string(container_item.price / 1000) + _("k")).c_str() : _(""));
							}
						}
						else
						{
							if ((container_item.price > var::esp::item::minimal_price * 1000 && var::esp::item::use_price_filter) ||
								!var::esp::item::use_price_filter)
							{
								i++;

								overlay::text(screen_position.x, screen_position.y + i * p, var::esp::item::custom_item_lists.at(container_item.list_id - 1).color, _("%s[%0.fm]%s"),
									var::esp::item::name ? container_item.short_name.c_str() : _(""), distance,
									var::esp::item::price ? std::string(std::to_string(container_item.price / 1000) + _("k")).c_str() : _(""));
							}
						}
					}
				}
				else if (item.type == cache_system::loot_type_t::corpse && var::esp::item::in_corpse)
				{
					if (var::esp::corpse::enable)
					{
						overlay::text(screen_position.x, screen_position.y - p, var::esp::corpse::color, _("corpse[%0.fm]"), distance);
					}

					uint32_t i = -1;

					for (cache_system::inventory_item_t corpse_item : item.items)
					{
						if (!corpse_item.list_id)
						{
							if (corpse_item.price >= var::esp::item::minimal_price * 1000)
							{
								i++;

								overlay::text(screen_position.x, screen_position.y + i * p, get_color(corpse_item.price), _("%s[%0.fm]%s"),
									var::esp::item::name ? corpse_item.short_name.c_str() : _(""), distance,
									var::esp::item::price ? std::string(std::to_string(corpse_item.price / 1000) + _("k")).c_str() : _(""));
							}
						}
						else
						{
							if ((corpse_item.price > var::esp::item::minimal_price * 1000 && var::esp::item::use_price_filter) ||
								!var::esp::item::use_price_filter)
							{
								i++;

								overlay::text(screen_position.x, screen_position.y + i * p, var::esp::item::custom_item_lists.at(corpse_item.list_id - 1).color, _("%s[%0.fm]%s"),
									var::esp::item::name ? corpse_item.short_name.c_str() : _(""), distance,
									var::esp::item::price ? std::string(std::to_string(corpse_item.price / 1000) + _("k")).c_str() : _(""));
							}
						}
					}
				}
			}
		}

		if (!cache_system::get_grenades().empty() && var::esp::grenade::enable)
		{
			std::vector<cache_system::cached_grenade_t> grenades = cache_system::get_grenades();

			for (cache_system::cached_grenade_t grenade : grenades)
			{
				vec2 screen_position = camera::w2s(grenade.position);

				if (screen_position.x <= 0 || screen_position.x >= screen_x || screen_position.y <= 0 || screen_position.y >= screen_y)
				{
					continue;
				}

				overlay::circle(screen_position.x, screen_position.y, 5.0f, var::esp::grenade::color, 12);

				overlay::text(screen_position.x, screen_position.y + p, var::esp::grenade::color, _("%0.fm"), grenade.position.distance(local_player_position));
			}
		}

		if (!cache_system::get_exits().empty() && var::esp::exit::enable)
		{
			std::vector<cache_system::cached_exit_t> exits = cache_system::get_exits();

			for (cache_system::cached_exit_t exit : exits)
			{
				vec2 screen_position = camera::w2s(exit.position);

				if (screen_position.x <= 0 || screen_position.x >= screen_x || screen_position.y <= 0 || screen_position.y >= screen_y)
				{
					continue;
				}

				overlay::text(screen_position.x, screen_position.y, var::esp::exit::color, _("%s[%0.fm]%s"),	
					var::esp::exit::name ? exit.name.c_str() : _(""), exit.position.distance(local_player_position),
					var::esp::exit::status ? exit.status.c_str() : _(""));
			}
		}
	}
}