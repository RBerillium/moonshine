#pragma once

#include "../main.hpp"

void binds()
{
	if (GetAsyncKeyState(VK_END) && 0x8000)
	{
		var::end_of_life = true;
	}

	//menu
	if (GetAsyncKeyState(var::bind_show_menu) && 0x8000)
	{
		var::menu = !var::menu; 
		
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	//players
	if (GetAsyncKeyState(var::esp::alive::bind_players) && 0x8000)
	{
		var::esp::alive::players = !var::esp::alive::players;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::esp::alive::bind_scavs) && 0x8000)
	{
		var::esp::alive::scavs = !var::esp::alive::scavs;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::esp::alive::bind_scav_players) && 0x8000)
	{
		var::esp::alive::scav_players = !var::esp::alive::scav_players;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::esp::alive::bind_bosses) && 0x8000)
	{
		var::esp::alive::bosses = !var::esp::alive::bosses;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::esp::alive::bind_minimal) && 0x8000)
	{
		var::esp::alive::minimal = !var::esp::alive::minimal;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	//items
	if (GetAsyncKeyState(var::esp::item::bind_item) && 0x8000)
	{
		var::esp::item::enable = !var::esp::item::enable;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	for (var::esp::item::custom_item_list &list : var::esp::item::custom_item_lists)
	{
		if (GetAsyncKeyState(list.bind) && 0x8000)
		{
			list.enable = !list.enable;

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}

	//misc
	if (GetAsyncKeyState(var::misc::bind_inertia) && 0x8000)
	{
		var::misc::inertia = !var::misc::inertia;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_recoil) && 0x8000)
	{
		var::misc::recoil = !var::misc::recoil;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_stamina) && 0x8000)
	{
		var::misc::stamina = !var::misc::stamina;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_visor) && 0x8000)
	{
		var::misc::visor = !var::misc::visor;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_thermal) && 0x8000)
	{
		var::misc::thermal = !var::misc::thermal;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_night) && 0x8000)
	{
		var::misc::night = !var::misc::night;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_speed) && 0x8000)
	{
		var::misc::speed = !var::misc::speed;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_jump) && 0x8000)
	{
		var::misc::jump = !var::misc::jump;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	if (GetAsyncKeyState(var::misc::bind_loot_walls) && 0x8000)
	{
		var::misc::loot_walls = !var::misc::loot_walls;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

}