#pragma once

#include "../main.hpp"

#include "../imgui/imgui_hotkey.h"
#include "../imgui/imgui_toggle.h"
#include "../imgui/imgui_stdlib.h"

#include "../crypt/xor_string.hpp"

#include "image.hpp"

namespace overlay = render_system;

void menu()
{
	auto center_text = [&](const char* format, ...)
	{
		va_list list; char text[1024] = { 0 };
		va_start(list, format); _vsnprintf_s(text, sizeof(text), format, list); va_end(list);

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2);
		ImGui::Text(text);
	};

	ImGui::PushFont(overlay::menu_font);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);	  

	overlay::text(screen_x / 2, 5, var::main_color, _("Overdose [%.0f]"), ImGui::GetIO().Framerate);

	if (var::debug)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 104));

		ImGui::Begin(_("Debug"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		ImGui::Columns(3, nullptr, true);

		ImGui::Text(_("Players")); ImGui::NextColumn(); ImGui::Text(_("%dms"), cache_system::p_time); ImGui::NextColumn(); ImGui::Text(_("%d"), cache_system::players.size()); ImGui::NextColumn();
		ImGui::Text(_("Items")); ImGui::NextColumn(); ImGui::Text(_("%dms"), cache_system::i_time); ImGui::NextColumn(); ImGui::Text(_("%d"), cache_system::items.size()); ImGui::NextColumn();
		ImGui::Text(_("Exits")); ImGui::NextColumn(); ImGui::Text(_("%dms"), cache_system::e_time); ImGui::NextColumn(); ImGui::Text(_("%d"), cache_system::exits.size()); ImGui::NextColumn();
		ImGui::Text(_("Grenades")); ImGui::NextColumn(); ImGui::Text(_("%dms"), cache_system::g_time); ImGui::NextColumn(); ImGui::Text(_("%d"), cache_system::grenades.size()); ImGui::NextColumn();

		
		ImGui::EndColumns();

		ImGui::End();
	}

	if (var::esp::exit::window)
	{
		ImGui::SetNextWindowSize(ImVec2(400, 0));

		ImGui::Begin(_("Exit List"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		ImGui::Columns(3, nullptr, true);

		for (cache_system::cached_exit_t exit : cache_system::get_exits())
		{
			ImGui::Text(_("%0.fm"), exit.position.distance(local_player_position));

			ImGui::NextColumn();

			ImGui::Text(_("%s"), exit.name.c_str());

			ImGui::NextColumn();

			ImGui::Text(_("%s"), exit.status.c_str());

			ImGui::NextColumn();
		}

		ImGui::EndColumns();

		ImGui::End();
	}

	if (var::menu)
	{
		auto set_name = [&](const char* name, std::string tag) -> std::string
		{
			return std::string(_("##") + std::string(name) + _("_") + std::string(tag));
		};

		auto radio_with_toggle_and_bind_and_color = [&](const char* name, ImVec2 size, ImColor& color, bool& value, int& bind, int& selected, int& iterator, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child") + std::to_string(iterator)).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				if (ImGui::RadioButton(set_name(name, _("radio") + std::to_string(iterator)).c_str(), selected == iterator))
				{
					selected = iterator;
				}

				ImGui::SameLine();

				ImGui::ColorEdit3(set_name(name, _("color") + std::to_string(iterator)).c_str(), (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel); ImGui::SameLine();

				ImGui::Text(name); ImGui::SameLine();

				ImGui::SetCursorPosX(size.x - 140.f - spacing); // 140.f = Toggle + Spacing + hotkey width

				ImGui::Toggle(set_name(name, _("toggle") + std::to_string(iterator)).c_str(), &value, ImGuiToggleFlags_Animated); ImGui::SameLine();

				ImGui::Hotkey(set_name(name, _("hotkey") + std::to_string(iterator)).c_str(), &bind, ImVec2(100, 0));
			}
			ImGui::EndChild();
		};

		auto radio_with_name = [&](const char* name, ImVec2 size, int& selected, int& iterator, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child") + std::to_string(iterator)).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				if (ImGui::RadioButton(set_name(name, _("radio") + std::to_string(iterator)).c_str(), selected == iterator))
				{
					selected = iterator;
				}

				ImGui::SameLine();

				ImGui::Text(name);
			}
			ImGui::EndChild();
		};

		auto toggle_with_bind_and_color = [&](const char* name, ImVec2 size, ImColor& color, bool& value, int& bind, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child")).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				ImGui::ColorEdit3(set_name(name, _("color")).c_str(), (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel); ImGui::SameLine();

				ImGui::Text(name); ImGui::SameLine();

				ImGui::SetCursorPosX(size.x - 140.f - spacing); // 140.f = Toggle + Spacing + hotkey width

				ImGui::Toggle(set_name(name, _("toggle")).c_str(), &value, ImGuiToggleFlags_Animated); ImGui::SameLine();

				ImGui::Hotkey(set_name(name, _("hotkey")).c_str(), &bind, ImVec2(100, 0));
			}
			ImGui::EndChild();
		};

		auto toggle_with_4checkbox = [&](const char* name, ImVec2 size, bool& main_value, bool& value1, bool& value2, bool& value3, bool& value4, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child")).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				ImGui::Text(name); ImGui::SameLine();

				ImGui::SetCursorPos(ImVec2(size.x - 343.f - spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1)); // 343.f = Toggle + Spacing + 4 checkbox width

				ImGui::Toggle(set_name(name, _("toggle")).c_str(), &main_value, ImGuiToggleFlags_Animated); ImGui::SameLine();

				ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1);

				ImGui::Checkbox(_("Player  "), &value1); ImGui::SameLine();

				ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1);

				ImGui::Checkbox(_("Player Scav  "), &value2); ImGui::SameLine();

				ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1);

				ImGui::Checkbox(_("Scav  "), &value3); ImGui::SameLine();

				ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1);

				ImGui::Checkbox(_("Boss  "), &value4); ImGui::SameLine();
			}
			ImGui::EndChild();
		};

		auto toggle_with_bind = [&](const char* name, ImVec2 size, bool& value, int& bind, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child")).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				ImGui::Text(name); ImGui::SameLine();

				ImGui::SetCursorPos(ImVec2(size.x - 140.f - spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1));

				ImGui::Toggle(set_name(name, _("toggle")).c_str(), &value, ImGuiToggleFlags_Animated); ImGui::SameLine();

				ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1);

				ImGui::Hotkey(set_name(name, _("hotkey")).c_str(), &bind, ImVec2(100, 0));
			}
			ImGui::EndChild();
		};

		auto toggle_with_color = [&](const char* name, ImVec2 size, ImColor& color, bool& value, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child")).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				ImGui::ColorEdit3(set_name(name, _("color")).c_str(), (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel); ImGui::SameLine();

				ImGui::Text(name); ImGui::SameLine();

				ImGui::SetCursorPos(ImVec2(size.x - 35.f - spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1));

				ImGui::Toggle(set_name(name, _("toggle")).c_str(), &value, ImGuiToggleFlags_Animated);
			}
			ImGui::EndChild();
		};

		auto just_toggle = [&](const char* name, ImVec2 size, bool& value, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child")).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				ImGui::Text(name); ImGui::SameLine();

				ImGui::SetCursorPos(ImVec2(size.x - 35.f - spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1));

				ImGui::Toggle(set_name(name, _("toggle")).c_str(), &value, ImGuiToggleFlags_Animated);
			}
			ImGui::EndChild();
		};

		auto just_color = [&](const char* name, ImVec2 size, ImColor& color, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child")).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				ImGui::ColorEdit3(set_name(name, _("color")).c_str(), (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel); ImGui::SameLine();

				ImGui::Text(name); ImGui::SameLine();
			}
			ImGui::EndChild();
		};

		auto just_slider = [&](const char* name, ImVec2 size, int& value, int min, int max, float spacing = 15.f) -> void
		{
			ImGui::BeginChild(set_name(name, _("child")).c_str(), size);
			{
				ImGui::SetCursorPos(ImVec2(spacing, (ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				ImGui::Text(name); ImGui::SameLine();

				ImGui::SetNextItemWidth(140.f);

				ImGui::SetCursorPos(ImVec2(size.x - 140.f - spacing, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 2) / 2.f + 1));

				ImGui::SliderInt(set_name(name, _("distance")).c_str(), &value, min, max, (_("%d")));
			}
			ImGui::EndChild();
		};

		ImGui::SetNextWindowSize(ImVec2(600, 505));

		ImGui::Begin(_("Overdose"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		float tab_size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 5) / 6;

		str tabs[] = { _("Players"), _("Items"), _("Other"), _("Misc"), _("Aim"), _("Settings") };

		for (u32 i = 0; i < IM_ARRAYSIZE(tabs); i++)
		{
			ImVec4 default_color = ImGui::GetStyleColorVec4(ImGuiCol_Button);

			if (var::current_tab == i)
				ImGui::GetStyle().Colors[ImGuiCol_Button] = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

			if (ImGui::Button(tabs[i].c_str(), ImVec2(tab_size, 40)))
				var::current_tab = i;

			ImGui::GetStyle().Colors[ImGuiCol_Button] = default_color;

			if (i < IM_ARRAYSIZE(tabs) - 1)
				ImGui::SameLine();
		}

		ImGui::Separator();

		if (var::current_tab == 0)
		{
			float size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

			ImGui::Columns(2, nullptr, false);

			toggle_with_bind_and_color(_("Player"), ImVec2(size, 40), var::esp::alive::color_players, var::esp::alive::players, var::esp::alive::bind_players);
			toggle_with_bind_and_color(_("Player Scav"), ImVec2(size, 40), var::esp::alive::color_scav_players, var::esp::alive::scav_players, var::esp::alive::bind_scav_players);
			toggle_with_bind_and_color(_("Scav"), ImVec2(size, 40), var::esp::alive::color_scavs, var::esp::alive::scavs, var::esp::alive::bind_scavs);
			toggle_with_bind_and_color(_("Boss"), ImVec2(size, 40), var::esp::alive::color_bosses, var::esp::alive::bosses, var::esp::alive::bind_bosses);

			ImGui::NextColumn();

			toggle_with_bind(_("Show Inventory"), ImVec2(size, 40), var::esp::alive::show_inventory, var::esp::alive::bind_show_inventory);
			toggle_with_bind(_("Minimalism"), ImVec2(size, 40), var::esp::alive::minimal, var::esp::alive::bind_minimal);
			just_toggle(_("Chams"), ImVec2(size, 40), var::esp::alive::chams);
			just_slider(_("Distance"), ImVec2(size, 40), var::esp::alive::distance, 0, 1500);

			ImGui::EndColumns();

			size = (ImGui::GetContentRegionAvail().x);

			toggle_with_4checkbox(_("Box"), ImVec2(size, 40), var::esp::alive::type_global::box,
				var::esp::alive::type_players::box,
				var::esp::alive::type_scav_players::box,
				var::esp::alive::type_scavs::box,
				var::esp::alive::type_bosses::box);

			toggle_with_4checkbox(_("Skeleton"), ImVec2(size, 40), var::esp::alive::type_global::skeleton,
				var::esp::alive::type_players::skeleton,
				var::esp::alive::type_scav_players::skeleton,
				var::esp::alive::type_scavs::skeleton,
				var::esp::alive::type_bosses::skeleton);

			toggle_with_4checkbox(_("Weapon"), ImVec2(size, 40), var::esp::alive::type_global::weapon,
				var::esp::alive::type_players::weapon,
				var::esp::alive::type_scav_players::weapon,
				var::esp::alive::type_scavs::weapon,
				var::esp::alive::type_bosses::weapon);

			toggle_with_4checkbox(_("Info"), ImVec2(size, 40), var::esp::alive::type_global::info,
				var::esp::alive::type_players::info,
				var::esp::alive::type_scav_players::info,
				var::esp::alive::type_scavs::info,
				var::esp::alive::type_bosses::info);

			toggle_with_4checkbox(_("Price"), ImVec2(size, 40), var::esp::alive::type_global::price,
				var::esp::alive::type_players::price,
				var::esp::alive::type_scav_players::price,
				var::esp::alive::type_scavs::price,
				var::esp::alive::type_bosses::price);
		}
		else if (var::current_tab == 1)
		{
			float size = (ImGui::GetContentRegionAvail().x);

			toggle_with_bind(_("Items"), ImVec2(size, 40), var::esp::item::enable, var::esp::item::bind_item);

			size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 4.f;

			ImGui::Columns(4, nullptr, false);

			just_toggle(_("On Ground"), ImVec2(size, 40), var::esp::item::on_ground);

			ImGui::NextColumn();

			just_toggle(_("In Container"), ImVec2(size, 40), var::esp::item::in_container);

			ImGui::NextColumn();

			just_toggle(_("In Corpse"), ImVec2(size, 40), var::esp::item::in_corpse);

			ImGui::NextColumn();

			just_toggle(_("Quest Items"), ImVec2(size, 40), var::esp::item::quest_items);

			ImGui::EndColumns();

			size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

			ImGui::Columns(2, nullptr, false);

			just_toggle(_("Name"), ImVec2(size, 40), var::esp::item::name);

			just_toggle(_("Price per slot"), ImVec2(size, 40), var::esp::item::price_per_slot);

			ImGui::NextColumn();

			just_slider(_("Min Price (k)"), ImVec2(size, 40), var::esp::item::minimal_price, 0, 500);

			just_slider(_("Distance"), ImVec2(size, 40), var::esp::item::distance, 0, 1500);

			ImGui::EndColumns();

			size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 4) / 5.f;

			ImGui::Columns(5, nullptr, false);

			just_color(_("0 - 50"), ImVec2(size, 40), var::esp::item::color_50);

			ImGui::NextColumn();

			just_color(_("50 - 100"), ImVec2(size, 40), var::esp::item::color_100);

			ImGui::NextColumn();

			just_color(_("100 - 150"), ImVec2(size, 40), var::esp::item::color_150);

			ImGui::NextColumn();

			just_color(_("150 - 200"), ImVec2(size, 40), var::esp::item::color_200);

			ImGui::NextColumn();

			just_color(_("> 200"), ImVec2(size, 40), var::esp::item::color_1000);

			ImGui::EndColumns();

			size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 3) / 4.f;

			ImGui::BeginChild(_("##custom_lists"), ImVec2(size * 3 + ImGui::GetStyle().ItemSpacing.x * 2, ImGui::GetContentRegionAvail().y - 25.0f));
			{
				int i = 0;

				for (var::esp::item::custom_item_list& list : var::esp::item::custom_item_lists)
				{
					i++;

					radio_with_toggle_and_bind_and_color(list.name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 40),
						list.color, list.enable, list.bind, var::esp::item::select_list, i);
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild(_("##custom_lists_handler"), ImVec2(size, ImGui::GetContentRegionAvail().y - 25.0f));
			{
				just_toggle(_("Price Limit"), ImVec2(size, 40), var::esp::item::use_price_filter);

				ImGui::SetCursorPosX(5);

				float size_y = (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 3) / 3.f;

				if (ImGui::Button(_("Add"), ImVec2(size - 10, size_y)))
				{
					var::esp::item::add_window = true;
				}

				ImGui::SetCursorPosX(5);

				if (ImGui::Button(_("Del"), ImVec2(size - 10, size_y)))
				{
					int index = var::esp::item::select_list;

					if (index > 0 && index <= var::esp::item::custom_item_lists.size())
					{
						remove(std::string(var::path_item_list + var::esp::item::custom_item_lists.at(index - 1).name + _(".items")).c_str());

						var::esp::item::custom_item_lists.erase(var::esp::item::custom_item_lists.begin() + index - 1);
					}
				}

				ImGui::SetCursorPosX(5);

				if (ImGui::Button(_("Edit"), ImVec2(size - 10, size_y)))
				{
					int index = var::esp::item::select_list;

					if (index > 0 && index <= var::esp::item::custom_item_lists.size())
					{
						var::esp::item::edit_window = true;
					}
				}
			}
			ImGui::EndChild();
		}
		else if (var::current_tab == 2)
		{
			float size = (ImGui::GetContentRegionAvail().x);

			just_toggle(_("Exits Window"), ImVec2(size, 40), var::esp::exit::window);
			just_toggle(_("Show Radar"), ImVec2(size, 40), var::esp::show_radar);
			//just_toggle(_("Speedhack x40 (RISK!!!)"), ImVec2(size, 40), var::misc::speed_x50);
			//just_slider(_("Speedhack Multiplier"), ImVec2(size, 40), var::misc::speed_multiply, 1, 40);

			toggle_with_color(_("Exits"), ImVec2(size, 40), var::esp::exit::color, var::esp::exit::enable);

			
		
			//size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

			//just_toggle(_("Name"), ImVec2(size, 40), var::esp::exit::name);

			//ImGui::SameLine();

			//just_toggle(_("Status"), ImVec2(size, 40), var::esp::exit::status);

			//ImGui::Separator();

			//size = (ImGui::GetContentRegionAvail().x);

			toggle_with_color(_("Grenades"), ImVec2(size, 40), var::esp::grenade::color, var::esp::grenade::enable);

			toggle_with_color(_("Corpse Title (working only with items)"), ImVec2(size, 40), var::esp::corpse::color, var::esp::corpse::enable);
			

			//center_text(_("EXPERIMENTAL"));

			//just_toggle(_("Fast bullets"), ImVec2(size, 40), var::aim::fast_bullets);
		}
		else if (var::current_tab == 3)
		{
			float size = (ImGui::GetContentRegionAvail().x);

			toggle_with_bind(_("No Inertia"), ImVec2(size, 40), var::misc::inertia, var::misc::bind_inertia);
			toggle_with_bind(_("No Recoil + Weapon Length Fix"), ImVec2(size, 40), var::misc::recoil, var::misc::bind_recoil);
			toggle_with_bind(_("No Stamina + No Oxygen"), ImVec2(size, 40), var::misc::stamina, var::misc::bind_stamina);
			toggle_with_bind(_("No Visor"), ImVec2(size, 40), var::misc::visor, var::misc::bind_visor);
			toggle_with_bind(_("Thermal Vision"), ImVec2(size, 40), var::misc::thermal, var::misc::bind_thermal);
			toggle_with_bind(_("Night Vision"), ImVec2(size, 40), var::misc::night, var::misc::bind_night);
			toggle_with_bind(_("Speed Hack x1.7"), ImVec2(size, 40), var::misc::speed, var::misc::bind_speed);
			toggle_with_bind(_("Super Jump"), ImVec2(size, 40), var::misc::jump, var::misc::bind_jump);
			toggle_with_bind(_("Loot Through Walls"), ImVec2(size, 40), var::misc::loot_walls, var::misc::bind_loot_walls);

		}
		else if (var::current_tab == 4)
		{
			float size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

			ImGui::Columns(2, nullptr, false);

			toggle_with_bind(_("Aim"), ImVec2(size, 40), var::aim::enable, var::aim::bind_aim);

			just_toggle(_("Ghetto Aim"), ImVec2(size, 40), var::aim::silent_aim);

			ImGui::BeginChild(_("aim_priority_child"), ImVec2(size, 40));
			{
				ImGui::SetCursorPos(ImVec2(15.f, (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				if (ImGui::RadioButton(_("Center"), var::aim::priority == var::aim::priority_type::center))
				{
					var::aim::priority = var::aim::priority_type::center;
				}

				ImGui::SameLine(); ImGui::SetCursorPosX(size / 2);

				if (ImGui::RadioButton(_("Distance"), var::aim::priority == var::aim::priority_type::distance))
				{
					var::aim::priority = var::aim::priority_type::distance;
				}
			}
			ImGui::EndChild();

			just_toggle(_("Aim Line"), ImVec2(size, 40), var::aim::aim_line);

			just_toggle(_("Fov"), ImVec2(size, 40), var::aim::fov);

			just_slider(_("Fov Radius"), ImVec2(size, 40), var::aim::fov_radius, 0, 480);

			just_toggle(_("Prediction"), ImVec2(size, 40), var::aim::prediction);

			

			

			just_slider(_("Dist. for Predict"), ImVec2(size, 40), var::aim::predict_distance, 0, 500);

			just_slider(_("Distance"), ImVec2(size, 40), var::aim::distance, 0, 500);

			ImGui::NextColumn();

			ImGui::BeginChild(_("aim_skeleton"), ImVec2(size, ImGui::GetContentRegionAvail().y - 25));

			ImGui::Image(aim_skeleton_shader, ImVec2(size, ImGui::GetContentRegionAvail().y));

			ImGui::SetCursorPos(ImVec2(15, 15));
			
			ImGui::Text(_("bone_id: %d"), var::aim::current_bone_id);

			ImGui::SetCursorPos(ImVec2(139, 25));

			if (ImGui::RadioButton(_("##head"), var::aim::current_bone_id == 133))
			{
				var::aim::current_bone_id = 133;
			}

			ImGui::SetCursorPos(ImVec2(139, 50));

			if (ImGui::RadioButton(_("##neck"), var::aim::current_bone_id == 132))
			{
				var::aim::current_bone_id = 132;
			}

			ImGui::SetCursorPos(ImVec2(139, 80));

			if (ImGui::RadioButton(_("##chest"), var::aim::current_bone_id == 37))
			{
				var::aim::current_bone_id = 37;
			}

			ImGui::SetCursorPos(ImVec2(139, 130));

			if (ImGui::RadioButton(_("##spine1"), var::aim::current_bone_id == 36))
			{
				var::aim::current_bone_id = 36;
			}

			ImGui::SetCursorPos(ImVec2(74, 130));

			if (ImGui::RadioButton(_("##left_hand"), var::aim::current_bone_id == 112))
			{
				var::aim::current_bone_id = 112;
			}

			ImGui::SetCursorPos(ImVec2(204, 130));

			if (ImGui::RadioButton(_("##right_hand"), var::aim::current_bone_id == 91))
			{
				var::aim::current_bone_id = 91;
			}

			ImGui::SetCursorPos(ImVec2(139, 165));

			if (ImGui::RadioButton(_("##spine2"), var::aim::current_bone_id == 29))
			{
				var::aim::current_bone_id = 29;
			}

			ImGui::SetCursorPos(ImVec2(139, 200));

			if (ImGui::RadioButton(_("##pelvis"), var::aim::current_bone_id == 14))
			{
				var::aim::current_bone_id = 14;
			}

			ImGui::SetCursorPos(ImVec2(112, 275));

			if (ImGui::RadioButton(_("##left_foot"), var::aim::current_bone_id == 22))
			{
				var::aim::current_bone_id = 22;
			}

			ImGui::SetCursorPos(ImVec2(166, 275));

			if (ImGui::RadioButton(_("##right_foot"), var::aim::current_bone_id == 17))
			{
				var::aim::current_bone_id = 17;
			}

			ImGui::EndChild();

			ImGui::EndColumns();
		}
		else if (var::current_tab == 5)
		{
			float size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

			just_toggle(_("Debug Window"), ImVec2(size, 40), var::debug);

			ImGui::SameLine();

			just_color(_("Main Color"), ImVec2(size, 40), var::main_color);

			ImGui::SetCursorPosY(255.f);

			ImGui::BeginChild(_("##config_title"), ImVec2(0, 40));
			{
				ImGui::SetCursorPos(ImVec2(15.f, (ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(_("Text")).y) / 2.f));

				center_text(_("Configs"));
			}
			ImGui::EndChild();

			//////////////////////////////////////////////////////////////////////////////////////////////

			ImGui::SetCursorPosY(300.f);

			size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 3) / 4.f;

			ImGui::BeginChild(_("##custom_configs"), ImVec2(size * 3 + ImGui::GetStyle().ItemSpacing.x * 2, ImGui::GetContentRegionAvail().y - 25.0f));
			{
				int i = 0;

				for (var::config::custom_config& conf : var::config::custom_configs)
				{
					i++;

					radio_with_name(conf.name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 40),
						var::config::select_config, i);
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild(_("##custom_config_handler"), ImVec2(size, ImGui::GetContentRegionAvail().y - 25.0f));
			{		
				float size_y = (ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ItemSpacing.y * 5) / 4.f;

				ImGui::SetCursorPos(ImVec2(5, 5));

				if (ImGui::Button(_("Add##Config"), ImVec2(size - 10, size_y)))
				{
					var::config::add_window = true;
				}

				ImGui::SetCursorPosX(5);

				if (ImGui::Button(_("Load##Config"), ImVec2(size - 10, size_y)))
				{
					try
					{
						file_system::load_settings(var::config::custom_configs.at(var::config::select_config - 1));
					}
					catch (const std::exception& ex)
					{
						return;
					}
					
				}

				ImGui::SetCursorPosX(5);

				if (ImGui::Button(_("Save##Config"), ImVec2(size - 10, size_y)))
				{
					try
					{
						file_system::save_settings(var::config::custom_configs.at(var::config::select_config - 1));
					}
					catch (const std::exception& ex)
					{
						return;
					}
					
				}

				ImGui::SetCursorPosX(5);

				if (ImGui::Button(_("Del##Config"), ImVec2(size - 10, size_y)))
				{
					int index = var::config::select_config;

					if (index > 0 && index <= var::config::custom_configs.size())
					{
						remove(std::string(var::path_config_list + var::config::custom_configs.at(index - 1).name + _(".config")).c_str());

						var::config::custom_configs.erase(var::config::custom_configs.begin() + index - 1);
					}
				}
			}
			ImGui::EndChild();

		}

		ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 25.f);

		ImGui::Separator();

		center_text(_("V1.2 Release 20.02.2024 (NV)"));

		ImGui::End();
	}

	if (var::esp::item::add_window)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 95));

		ImGui::Begin(_("Add Custom List"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		ImGui::InputText(_("##name_list"), &var::input::list_name);

		ImGui::SameLine();

		if (ImGui::Button(_("<"), ImVec2(40, 20)))
		{
			if (!var::input::list_name.empty())
			{
				var::input::list_name.pop_back();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button(_("clear"), ImVec2(40, 20)))
		{
			var::input::list_name.clear();
		}

		float size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

		if (ImGui::Button(_("Add"), ImVec2(size, 40)))
		{
			bool repeat_name = false;

			for (var::esp::item::custom_item_list list : var::esp::item::custom_item_lists)
			{
				if (list.name == var::input::list_name)
				{
					repeat_name = true;
				}
			}

			if (!var::input::list_name.empty() && !repeat_name)
			{
				var::esp::item::custom_item_list list;

				list.enable = false;
				list.bind = 0;
				list.color = { 70, 70, 70, 255 };
				list.name = var::input::list_name;

				var::esp::item::custom_item_lists.push_back(list);

				var::esp::item::add_window = false;
			}
		}

		ImGui::SameLine();

		if (ImGui::Button(_("Close"), ImVec2(size, 40)))
		{
			var::esp::item::add_window = false;
		}

		ImGui::End();
	}

	if (var::esp::item::edit_window)
	{
		ImGui::SetNextWindowSize(ImVec2(1000, 600));

		ImGui::Begin(_("Edit Custom List"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		ImGui::SetCursorPosX(5);

		float size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

		ImGui::SetNextItemWidth(size - 40.f * 2 - ImGui::GetStyle().ItemSpacing.x * 2);

		var::input::search_filter.Draw(_("##search_filter"));

		ImGui::SameLine();

		if (ImGui::Button(_("<"), ImVec2(40, 20)))
		{
			var::input::search_filter.Backspace();
		}
		ImGui::SameLine();

		if (ImGui::Button(_("clear")))
		{
			var::input::search_filter.Clear();
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x -
			ImGui::CalcTextSize(var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).name.c_str()).x) / 2 + size);

		ImGui::Text(_("%s"), var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).name.c_str());

		ImGui::Columns(2, nullptr, false);

		ImGui::BeginChild(_("##search"), ImVec2(size, ImGui::GetContentRegionAvail().y - 45.f));
		{
			int i = 0;

			ImGui::SetCursorPosY(5);

			for (std::pair<std::string, api::item_info_t> item : api::items_library)
			{
				i++;

				std::string text = item.second.short_name + item.second.name;

				if (var::input::search_filter.PassFilter(text.c_str()))
				{
					ImGui::BulletText(_("%s (%s)"), item.second.short_name.c_str(), item.second.name.c_str());

					ImGui::SameLine(); ImGui::SetCursorPosX(size - 60.f);

					if (ImGui::Button(std::string(_("add##") + std::to_string(i)).c_str(), ImVec2(40, 0)))
					{
						var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).items.push_back(item.first);
					}
				}
			}
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild(_("##list_items"), ImVec2(size, ImGui::GetContentRegionAvail().y - 45.f));
		{
			int i = 0;

			ImGui::SetCursorPosY(5);

			for (std::string& item : var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).items)
			{
				i++;

				ImGui::BulletText(_("%s"), api::get_item_info(item).short_name.c_str());

				ImGui::SameLine(); ImGui::SetCursorPosX(size - 60.f);

				if (ImGui::Button(std::string(_("del##") + std::to_string(i)).c_str(), ImVec2(40, 0)))
				{
					var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).items.erase(
						var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).items.begin() + i - 1
					);
				}
			}
		}
		ImGui::EndChild();

		ImGui::EndColumns();

		if (ImGui::Button(_("Save"), ImVec2(size, 40)))
		{
			nlohmann::json json;

			json[_("enable")] = var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).enable;
			json[_("bind")] = var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).bind;

			json[_("color")][_("r")] = int(var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).color.Value.x * 255);
			json[_("color")][_("g")] = int(var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).color.Value.y * 255);
			json[_("color")][_("b")] = int(var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).color.Value.z * 255);

			for (std::string& item : var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).items)
			{
				json[_("data")].push_back(item.c_str());
			}

			std::ofstream file;

			file.open(std::string(var::path_item_list + var::esp::item::custom_item_lists.at(var::esp::item::select_list - 1).name + _(".items")).c_str());

			if (file.is_open())
			{
				file << json;
			}

			file.close();

			var::esp::item::edit_window = false;
		}

		ImGui::SameLine();

		if (ImGui::Button(_("Close"), ImVec2(size, 40)))
		{
			var::esp::item::edit_window = false;
		}

		ImGui::End();
	}

	if (var::config::add_window)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 95));

		ImGui::Begin(_("Add Config"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		ImGui::InputText(_("##name_config"), &var::input::config_name);

		ImGui::SameLine();

		if (ImGui::Button(_("<##config"), ImVec2(40, 20)))
		{
			if (!var::input::config_name.empty())
			{
				var::input::config_name.pop_back();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button(_("clear##clear_config"), ImVec2(40, 20)))
		{
			var::input::config_name.clear();
		}

		float size = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 1) / 2.f;

		if (ImGui::Button(_("Add##add_config"), ImVec2(size, 40)))
		{
			bool repeat_name = false;

			for (var::config::custom_config conf : var::config::custom_configs)
			{
				if (conf.name == var::input::config_name)
				{
					repeat_name = true;
				}
			}

			if (!var::input::config_name.empty() && !repeat_name)
			{
				var::config::custom_config conf;

				conf.name = var::input::config_name;

				var::config::custom_configs.push_back(conf);

				var::config::add_window = false;
			}
		}

		ImGui::SameLine();

		if (ImGui::Button(_("Close##add_config"), ImVec2(size, 40)))
		{
			var::config::add_window = false;
		}

		ImGui::End();
	}
}