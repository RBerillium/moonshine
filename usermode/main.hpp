#pragma once

#include <windows.h>
#include <unordered_map>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <filesystem>
#include <tlhelp32.h>
#include <algorithm>
#include <iostream>
#include <dwmapi.h>
#include <d3dx11.h>
#include <d3d11.h>
#include <fstream>
#include <codecvt>
#include <thread>
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <ctime>
#include <map>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

#pragma warning(disable: 4996)

#include "imgui/imgui.h"
#include "json/json.hpp"
#include "crypt/xor_string.hpp"


#include "unity/vector2.hpp"
#include "unity/vector3.hpp"
#include "unity/vector4.hpp"

using namespace nlohmann;

typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef const char* chars;
typedef const wchar_t* wchars;

typedef std::string str;
template<class t> using vec = std::vector<t>;
template<class t1, class t2> using map = std::map<t1, t2>;



f32 screen_x = GetSystemMetrics(SM_CXSCREEN);

f32 screen_y = GetSystemMetrics(SM_CYSCREEN);

u64 local_player;

vec3 local_player_position;


u64 sprint_ptr = 0;

namespace var
{
	enum class version_t { lite, pro };

	version_t version = version_t::pro;

	enum class language_t { ru, en };

	language_t language = language_t::en;

	//

	ImColor main_color = { 127, 199, 255, 255 };

	bool end_of_life = false;

	bool menu = false;

	bool debug = false;

	i32 font_size = 12;

	i32 bind_show_menu = VK_INSERT;

	i32 current_tab = 0;

	str path_item_list = _("C:\\Windows\\AppSave\\Lists\\");

	str path_config_list = _("C:\\Windows\\AppSave\\Configs\\");

	namespace input
	{
		ImGuiTextFilter search_filter;

		str list_name = _("");

		str config_name = _("");
	}

	namespace esp
	{
		bool show_radar = false;
		namespace alive
		{
			bool players = true;
			i32 bind_players = 0;
			ImColor color_players = { 228, 113, 122, 255 };

			bool scavs = true;
			i32 bind_scavs = 0;
			ImColor color_scavs = { 204, 204, 255, 255 };

			bool scav_players = true;
			i32 bind_scav_players = 0;
			ImColor color_scav_players = { 153, 255, 153, 255 };

			bool bosses = true;
			i32 bind_bosses = 0;
			ImColor color_bosses = { 62, 180, 137, 255 };

			bool show_inventory = true;
			i32 bind_show_inventory = VK_LMENU;

			bool minimal = false;
			i32 bind_minimal = 0;

			bool chams = true;

			i32 distance = 1000;

			namespace type_global
			{
				bool box = true;
				bool info = true;
				bool weapon = true;
				bool skeleton = true;
				bool price = true;
			}

			namespace type_players
			{
				bool box = true;
				bool info = true;
				bool weapon = true;
				bool skeleton = true;
				bool price = true;
			}

			namespace type_scav_players
			{
				bool box = true;
				bool info = true;
				bool weapon = true;
				bool skeleton = true;
				bool price = true;
			}

			namespace type_scavs
			{
				bool box = true;
				bool info = true;
				bool weapon = true;
				bool skeleton = true;
				bool price = true;
			}

			namespace type_bosses
			{
				bool box = true;
				bool info = true;
				bool weapon = true;
				bool skeleton = true;
				bool price = true;
			}
		}

		namespace item
		{
			bool enable = true;
			i32 bind_item = 0;

			bool on_ground = true;
			bool in_container = true;
			bool in_corpse = true;
			bool quest_items = false;

			bool price = true;
			bool name = true;
			bool price_per_slot = false;

			i32 minimal_price = 50;
			i32 distance = 1000;

			ImColor color_50 = { 254, 229, 172, 255 };
			ImColor color_100 = { 255, 189, 136, 255 };
			ImColor color_150 = { 253, 219, 109, 255 };
			ImColor color_200 = { 255, 188, 173, 255 };
			ImColor color_1000 = { 225, 69, 217, 255 };

			bool use_price_filter = true;
			bool add_window = false;
			bool edit_window = false;

			struct custom_item_list
			{
				bool enable;
				ImColor color;
				str name;
				i32 bind;

				vec<str> items;
			};

			i32 select_list = 0;

			vec<custom_item_list> custom_item_lists;
		}

		namespace exit
		{
			bool enable = false;
			bool name = true;	// not use
			bool status = true;	// not use
			bool window = false;

			ImColor color = { 105, 215, 100, 255 };
		}

		namespace grenade
		{
			bool enable = false;
			ImColor color = { 255, 0, 0, 255 };
		}

		namespace corpse
		{
			bool enable = false;
			ImColor color = { 125, 12, 12, 255 };
		}
	}

	namespace aim
	{
		bool enable = false; ////
		bool fov = true;
		bool aim_line = false;
		bool prediction = false;
		bool silent_aim = false;
		bool fast_bullets = false;

		i32 fov_radius = 150;
		i32 predict_distance = 50.f;
		i32 current_bone_id = 133;
		i32 distance = 500;
		i32 bind_aim = 0x56;

		enum class priority_type { distance, center };
		priority_type priority = priority_type::center;
	}

	namespace misc
	{
		bool inertia = false;
		i32 bind_inertia = 0;

		bool recoil = false;
		i32 bind_recoil = 0;

		bool stamina = false;
		i32 bind_stamina = 0;

		bool visor = false;
		i32 bind_visor = 0;

		bool thermal = false;
		i32 bind_thermal = 0;

		bool night = false;
		i32 bind_night = 0;

		
		bool speed = false;
		i32 bind_speed = 0;
		

		bool speed_x50 = false;
		i32 bind_speed_x50 = 0;
		i32 speed_multiply = 1.f;

		bool jump = false;
		i32 bind_jump = 0;

		bool loot_walls = false;
		i32 bind_loot_walls = 0;
	}

	namespace config
	{
		bool add_window = false;

		struct custom_config
		{
			str name;
		};

		i32 select_config = 0;

		vec<custom_config> custom_configs;
	}
}

