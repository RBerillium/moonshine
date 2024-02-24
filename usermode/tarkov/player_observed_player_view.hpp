#pragma once

#include "offset.hpp"

#include "../main.hpp"

#include "../main/memory_system.hpp"

#include "..//unity/unity_string.hpp"



namespace memory = memory_system;

namespace observer_player_view	
{

	u64 get_body(u64 player)
	{
		return memory::read<u64>(player + offset::observed_player_view::body);
	}

	u64 get_hands(u64 player)
	{
		return memory::read<u64>(player , offset::observed_player_view::hands);
	}

	u64 get_inventory(u64 player)
	{
		return memory::read<u64>(player, offset::observed_player_view::inventory);
	}

	u64 get_profile(u64 player)
	{
		return memory::read<u64>(player + offset::observed_player_view::profile);
	}

	u64 get_physical(u64 player)
	{
		return memory::read<u64>(player + offset::observed_player_view::physical);
	}
	//u64 get_pwa(u64 player)
	//{
	//	return memory::read<u64>(player + offset::observed_player_view::pwa);
	//}
	u64 get_movement(u64 player)
	{
		return memory::read<u64>(player, offset::observed_player_view::movement);
	}

	bool is_ai(u64 player)
	{
		return memory::read<bool>(player + offset::observed_player_view::is_ai);
	}
	
	str get_profile_id(u64 player)
	{
		return unity_string::to_str(memory::read<u64>(player + 0x40));
	}

	std::string get_name(u64 player)
	{
		return unity_string::to_str( memory::read<u64>(player + 0x48));
		
	}
	std::wstring get_ws_name(u64 player)
	{

		return unity_string::to_wide_str(memory::read<u64>(player + 0x48));

	}
	str get_account_id(u64 player)
	{
		return unity_string::to_str(memory::read<u64>(player + 0x50));
	}
	str get_player_side(u64 player)
	{
		u32 side = memory::read<u32>(player + 0xf0);
	
		switch (side)
		{
		case 1:
			return _("U");
		case 2:
			return _("B");
		case 4:
			return _("S");

		default: _("S");
		}
		
	}
	u64 get_player_controller(u64 player)
	{
		return memory::read<u64>(player + 0x80);
	}

	bool has_special_char(const char * string)
	{
		return string[strspn(string, _("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-"))] != 0;
	}
	std::wstring get_name_other(u64 player_controller)
	{
		u64 g_struct = memory::read<u64>(player_controller + 0xe8);
		return unity_string::to_wide_str(memory::read<u64>(g_struct + 0x18));
	}


	std::map <std::wstring, std::string> boss_names =
	{

		{_(L"Килла") , _("Killa")},
		{_(L"Тагилла") , _("Tagilla")},
		{_(L"Решала") , _("Reshala")},
		{_(L"Глухарь") , _("Glukhar")},
		{_(L"Штурман") , _("Shturman")},

		{_(L"Knight") , _("Knight")},
		{_(L"Big Pipe") , _("Big Pipe")},


		{_(L"Birdeye") , _("Birdeye")},



		{_(L"Зрячий") , _("Zryachiy")},
		{_(L"Санитар") , _("Sanitar")},
		{_(L"Жрец") , _("Priest")},
		{_(L"Кабан") , _("Kaban")},
		{_(L"Колонтай") , _("Kolontay")},

		//свита
		{_(L"Сектант") , _("Cultist")},
		{_(L"Питон") , _("Raider")},
		{_(L"Медведь") , _("Raider")},
		{_(L"Могильник") , _("Raider")},
		{_(L"Носорог") , _("Raider")},
		{_(L"Пиранья") , _("Raider")},
		{_(L"Кобра") , _("Raider")},
		{_(L"Зубр") , _("Raider")},
		{_(L"Балу") , _("Raider")},
		{_(L"Мангуст") , _("Raider")},
		{_(L"Ястреб") , _("Raider")},
		{_(L"Беркут") , _("Raider")},
		{_(L"Орлан") , _("Raider")},
		{_(L"Лис") , _("Raider")},
		{_(L"Калан") , _("Raider")},
		{_(L"Падальщик") , _("Raider")},
		{_(L"Шакал") , _("Raider")},
		{_(L"Акула") , _("Raider")},
		{_(L"Гадюка") , _("Raider")},
		{_(L"Вепрь") , _("Raider")},
		{_(L"Пума") , _("Raider")},
		{_(L"Пантера") , _("Raider")},
		{_(L"Пчел") , _("Raider")},
		{_(L"Гриф") , _("Raider")},
		{_(L"Тарантул") , _("Raider")},
		{_(L"Леопард") , _("Raider")},
		{_(L"Логгерхед") , _("Raider")},
		{_(L"Стервятник") , _("Raider")},
		{_(L"Красный") , _("Raider")},
		{_(L"Лев") , _("Raider")},
		{_(L"Кондор") , _("Raider")},
		{_(L"Тигр") , _("Raider")},
		{_(L"Рысь") , _("Raider")},
		{_(L"Мантис") , _("Raider")},
		{_(L"Варан") , _("Raider")},
		{_(L"Гюрза") , _("Raider")},
		{_(L"Кайман") , _("Raider")},
		{_(L"Скорпион") , _("Raider")},
		{_(L"Кречет") , _("Raider")},
		{_(L"Сапсан") , _("Raider")},
		{_(L"Волк") , _("Raider")},
		{_(L"Гриззли") , _("Raider")},
		{_(L"Секач") , _("Raider")},
		{_(L"Барракуда") , _("Raider")},
		{_(L"Ворон") , _("Raider")},
		{_(L"Барс") , _("Raider")},
		{_(L"Росомаха") , _("Raider")},
		{_(L"Крачун") , _("Raider")},
		{_(L"Ягуар") , _("Raider")},
		{_(L"Каракурт") , _("Raider")},
		{_(L"Тайпан") , _("Raider")},
		{_(L"Дикобраз") , _("Raider")},
		{_(L"Орел") , _("Raider")},
		{_(L"Гепард") , _("Raider")},
		{_(L"Ирбис") , _("Raider")},
		{_(L"Манул") , _("Raider")},
		{_(L"Aimbotkin") , _("Rogue")},
		{_(L"Andresto") , _("Rogue")},
		{_(L"Applejuice") , _("Rogue")},
		{_(L"Arizona") , _("Rogue")},
		{_(L"Afraid") , _("Rogue")},
		{_(L"Auron") , _("Rogue")},
		{_(L"Badboy") , _("Rogue")},
		{_(L"Baddie") , _("Rogue")},
		{_(L"Beard") , _("Rogue")},
		{_(L"Beverly") , _("Rogue")},
		{_(L"Bison") , _("Rogue")},
		{_(L"Blackbird") , _("Rogue")},
		{_(L"Blade") , _("Rogue")},
		{_(L"Blakemore") , _("Rogue")},
		{_(L"Boatswain") , _("Rogue")},
		{_(L"Boogerman") , _("Rogue")},
		{_(L"Brockley") , _("Rogue")},
		{_(L"Browski") , _("Rogue")},
		{_(L"Bullet") , _("Rogue")},
		{_(L"Bunny") , _("Rogue")},
		{_(L"Butcher") , _("Rogue")},
		{_(L"Chester") , _("Rogue")},
		{_(L"Churchill") , _("Rogue")},
		{_(L"Cliffhanger") , _("Rogue")},
		{_(L"Condor") , _("Rogue")},
		{_(L"Cook") , _("Rogue")},
		{_(L"Cougar") , _("Rogue")},
		{_(L"Coyote") , _("Rogue")},
		{_(L"Crooked") , _("Rogue")},
		{_(L"Cross") , _("Rogue")},
		{_(L"Dakota") , _("Rogue")},
		{_(L"Dawg") , _("Rogue")},
		{_(L"Deceit") , _("Rogue")},
		{_(L"Denver") , _("Rogue")},
		{_(L"Diggi") , _("Rogue")},
		{_(L"Donutop") , _("Rogue")},
		{_(L"Duke") , _("Rogue")},
		{_(L"Dustin") , _("Rogue")},
		{_(L"Enzo") , _("Rogue")},
		{_(L"Esquilo") , _("Rogue")},
		{_(L"Father") , _("Rogue")},
		{_(L"Firion") , _("Rogue")},
		{_(L"Floridaman") , _("Rogue")},
		{_(L"Frenzy") , _("Rogue")},
		{_(L"Foxy") , _("Rogue")},
		{_(L"Garandthumb") , _("Rogue")},
		{_(L"Goat") , _("Rogue")},
		{_(L"Golden") , _("Rogue")},
		{_(L"Grandpa") , _("Rogue")},
		{_(L"Greyzone") , _("Rogue")},
		{_(L"Grim") , _("Rogue")},
		{_(L"Grommet") , _("Rogue")},
		{_(L"Gunporn") , _("Rogue")},
		{_(L"Handsome") , _("Rogue")},
		{_(L"Haunted") , _("Rogue")},
		{_(L"Hellshrimp") , _("Rogue")},
		{_(L"Honorable") , _("Rogue")},
		{_(L"Hypno") , _("Rogue")},
		{_(L"Instructor") , _("Rogue")},
		{_(L"Ironfists") , _("Rogue")},
		{_(L"Iowa") , _("Rogue")},
		{_(L"James") , _("Rogue")},
		{_(L"Jeff") , _("Rogue")},
		{_(L"Jersey") , _("Rogue")},
		{_(L"John") , _("Rogue")},
		{_(L"Juggernaut") , _("Rogue")},
		{_(L"Justkilo") , _("Rogue")},
		{_(L"Kentucky") , _("Rogue")},
		{_(L"Kry") , _("Rogue")},
		{_(L"Lancaster") , _("Rogue")},
		{_(L"Lee") , _("Rogue")},
		{_(L"Legia") , _("Rogue")},
		{_(L"Litton") , _("Rogue")},
		{_(L"Lost") , _("Rogue")},
		{_(L"Lunar") , _("Rogue")},
		{_(L"Madknight") , _("Rogue")},
		{_(L"Mamba") , _("Rogue")},
		{_(L"Marooner") , _("Rogue")},
		{_(L"Marquesses") , _("Rogue")},
		{_(L"Meldon") , _("Rogue")},
		{_(L"Melo") , _("Rogue")},
		{_(L"Michigan") , _("Rogue")},
		{_(L"Mike") , _("Rogue")},
		{_(L"Momma") , _("Rogue")},
		{_(L"Mortal") , _("Rogue")},
		{_(L"Mother") , _("Rogue")},
		{_(L"Nevada") , _("Rogue")},
		{_(L"Nine-hole") , _("Rogue")},
		{_(L"Noisy") , _("Rogue")},
		{_(L"Nukem") , _("Rogue")},
		{_(L"Ocean") , _("Rogue")},
		{_(L"Oklahoma") , _("Rogue")},
		{_(L"OneEye") , _("Rogue")},
		{_(L"Oskar") , _("Rogue")},
		{_(L"Panther") , _("Rogue")},
		{_(L"Philbo") , _("Rogue")},
		{_(L"Quebec") , _("Rogue")},
		{_(L"Racoon") , _("Rogue")},
		{_(L"Rage") , _("Rogue")},
		{_(L"Rambo") , _("Rogue")},
		{_(L"Rassler") , _("Rogue")},
		{_(L"Receit") , _("Rogue")},
		{_(L"Rib-eye") , _("Rogue")},
		{_(L"Riot") , _("Rogue")},
		{_(L"Rock") , _("Rogue")},
		{_(L"Rocket") , _("Rogue")},
		{_(L"Ronflex") , _("Rogue")},
		{_(L"Ronny") , _("Rogue")},
		{_(L"Rossler") , _("Rogue")},
		{_(L"RoughDog") , _("Rogue")},
		{_(L"Scar") , _("Rogue")},
		{_(L"Scottsdale") , _("Rogue")},
		{_(L"Seafarer") , _("Rogue")},
		{_(L"Shadow") , _("Rogue")},
		{_(L"SharkBait") , _("Rogue")},
		{_(L"Sharkkiller") , _("Rogue")},
		{_(L"Sherman") , _("Rogue")},
		{_(L"Sherifu") , _("Rogue")},
		{_(L"Shifty") , _("Rogue")},
		{_(L"Slayer") , _("Rogue")},
		{_(L"Sly") , _("Rogue")},
		{_(L"Snake") , _("Rogue")},
		{_(L"Sneaky") , _("Rogue")},
		{_(L"Sniperlife") , _("Rogue")},
		{_(L"Solem") , _("Rogue")},
		{_(L"Solidus") , _("Rogue")},
		{_(L"Spectator-6") , _("Rogue")},
		{_(L"Spyke") , _("Rogue")},
		{_(L"Stamper") , _("Rogue")},
		{_(L"Striker") , _("Rogue")},
		{_(L"Texas") , _("Rogue")},
		{_(L"Three-Teeth") , _("Rogue")},
		{_(L"Trent") , _("Rogue")},
		{_(L"Trickster") , _("Rogue")},
		{_(L"Triggerhappy") , _("Rogue")},
		{_(L"Two-Finger") , _("Rogue")},
		{_(L"Vicious") , _("Rogue")},
		{_(L"Victor") , _("Rogue")},
		{_(L"Voodoo") , _("Rogue")},
		{_(L"Voss") , _("Rogue")},
		{_(L"Wadley") , _("Rogue")},
		{_(L"Weasel") , _("Rogue")},
		{_(L"Whale-Eye") , _("Rogue")},
		{_(L"Whisky") , _("Rogue")},
		{_(L"Whitemane") , _("Rogue")},
		{_(L"Woodrow") , _("Rogue")},
		{_(L"Wrath") , _("Rogue")},
		{_(L"Zed") , _("Rogue")},
		{_(L"Zero-Zero") , _("Rogue") }
	};

	std::string get_boss_name(std::wstring name)
	{
		return boss_names[name];
	}
}