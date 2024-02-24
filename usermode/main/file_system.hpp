#pragma once

#include "../main.hpp"

#include "../json/json.hpp"

#include "../crypt/xor_string.hpp"

#include "../json/json.hpp"

using namespace nlohmann;

namespace file_system
{
	json load_as_json(str file_name)
	{
        json object; std::ifstream file(file_name);

        if (file.is_open())
        {
            file >> object;
            file.close();
        }

        return object;
	}

    bool load_item_library(str path, var::language_t lang)
    {
        if (lang == var::language_t::en)
        {
            system(_(R"(curl -X POST -H "Content-Type: application/json" --data ^" {""query"": ""{ items { id name shortName avg24hPrice basePrice width height} }""} ^" https://api.tarkov.dev/graphql > C:\windows\output.json")"));
        }
        else if (lang == var::language_t::ru)
        {
            system(_(R"("curl -X POST -H "Content-Type: application/json" --data ^" {""query"": ""{ items(lang: ru) { id name shortName avg24hPrice basePrice } }""} ^" https://api.tarkov.dev/graphql > C:\windows\output.json")"));
        }

        json items;

        try
        {
            items = load_as_json(path);
        }
        catch (const std::exception& ex)
        {
            return false;
        }

        for (auto& item : items[_("data")][_("items")].items())
        {
            str name;
            str short_name;
            u32 avg_price;
            u32 base_price;
            u32 width;
            u32 height;
            str id;

            item.value()[_("id")].get_to(id);
            item.value()[_("name")].get_to(name);
            item.value()[_("shortName")].get_to(short_name);
            item.value()[_("avg24hPrice")].get_to(avg_price);
            item.value()[_("width")].get_to(width);
            item.value()[_("height")].get_to(height);
            item.value()[_("basePrice")].get_to(base_price);

            if (name.empty() || short_name.empty())
            {
                continue;
            }

            api::items_library[id] = {name, short_name, avg_price, base_price, width, height};
        }

        if (api::items_library.empty())
        {
            return false;
        }

        return true;
    }

    bool load_config_list(str path)
    {
		for (auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.path().filename().string().find(_(".config")) != std::string::npos)
			{
				json file;

				try
				{
					file = load_as_json(entry.path().string());
				}
				catch (const std::exception& ex)
				{
					return false;
				}

                var::config::custom_config conf;

                conf.name = entry.path().filename().string().erase(entry.path().filename().string().find(_(".config")));

                var::config::custom_configs.push_back(conf);
			}
		}

		return true;
    }

    bool load_custom_list(str path)
    {
		for (auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.path().filename().string().find(_(".items")) != std::string::npos)
			{
				json file;

				try
				{
					file = load_as_json(entry.path().string());
				}
				catch (const std::exception& ex)
				{
					return false;
				}

				var::esp::item::custom_item_list list;

				list.name = entry.path().filename().string().erase(entry.path().filename().string().find(_(".items")));
				list.bind = file[_("enable")];
				list.bind = file[_("bind")];
				list.color = { file[_("color")][_("r")], file[_("color")][_("g")], file[_("color")][_("b")], 255 };

				for (auto& item : file[_("data")].items())
				{
					list.items.push_back(item.value());
				}

				var::esp::item::custom_item_lists.push_back(list);
			}
		}

		return true;
    }

    void save_settings(var::config::custom_config& conf)
    {
        json config_json;

        //all
        config_json[_("mc1")] = int(var::main_color.Value.x * 255);
        config_json[_("mc2")] = int(var::main_color.Value.y * 255);
        config_json[_("mc3")] = int(var::main_color.Value.z * 255);
        config_json[_("dd")] = var::debug;

        //players
        config_json[_("p1")] = var::esp::alive::players;
        config_json[_("p2")] = var::esp::alive::bind_players;
        config_json[_("p3")] = int(var::esp::alive::color_players.Value.x * 255);
        config_json[_("p4")] = int(var::esp::alive::color_players.Value.y * 255);
        config_json[_("p5")] = int(var::esp::alive::color_players.Value.z * 255);

        //scavs
        config_json[_("s1")] = var::esp::alive::scavs;
        config_json[_("s2")] = var::esp::alive::bind_scavs;
        config_json[_("s3")] = int(var::esp::alive::color_scavs.Value.x * 255);
        config_json[_("s4")] = int(var::esp::alive::color_scavs.Value.y * 255);
        config_json[_("s5")] = int(var::esp::alive::color_scavs.Value.z * 255);

        //scav_players
        config_json[_("sp1")] = var::esp::alive::scav_players;
        config_json[_("sp2")] = var::esp::alive::bind_scav_players;
        config_json[_("sp3")] = int(var::esp::alive::color_scav_players.Value.x * 255);
        config_json[_("sp4")] = int(var::esp::alive::color_scav_players.Value.y * 255);
        config_json[_("sp5")] = int(var::esp::alive::color_scav_players.Value.z * 255);

        //bosses
        config_json[_("b1")] = var::esp::alive::bosses;
        config_json[_("b2")] = var::esp::alive::bind_bosses;
        config_json[_("b3")] = int(var::esp::alive::color_bosses.Value.x * 255);
        config_json[_("b4")] = int(var::esp::alive::color_bosses.Value.y * 255);
        config_json[_("b5")] = int(var::esp::alive::color_bosses.Value.z * 255);

        //show inv
        config_json[_("si1")] = var::esp::alive::show_inventory;
        config_json[_("si2")] = var::esp::alive::bind_show_inventory;

        //minimal
        config_json[_("mi1")] = var::esp::alive::minimal;
        config_json[_("mi2")] = var::esp::alive::bind_minimal;

        //chams
        config_json[_("c1")] = var::esp::alive::chams;

        //distance
        config_json[_("d1")] = var::esp::alive::distance;

        //type_global
        config_json[_("tg1")] = var::esp::alive::type_global::box;
        config_json[_("tg2")] = var::esp::alive::type_global::info;
        config_json[_("tg3")] = var::esp::alive::type_global::weapon;
        config_json[_("tg4")] = var::esp::alive::type_global::skeleton;
        config_json[_("tg5")] = var::esp::alive::type_global::price;

        //type_players
        config_json[_("tp1")] = var::esp::alive::type_players::box;
        config_json[_("tp2")] = var::esp::alive::type_players::info;
        config_json[_("tp3")] = var::esp::alive::type_players::weapon;
        config_json[_("tp4")] = var::esp::alive::type_players::skeleton;
        config_json[_("tp5")] = var::esp::alive::type_players::price;

        //type_scav_players
        config_json[_("tsp1")] = var::esp::alive::type_scav_players::box;
        config_json[_("tsp2")] = var::esp::alive::type_scav_players::info;
        config_json[_("tsp3")] = var::esp::alive::type_scav_players::weapon;
        config_json[_("tsp4")] = var::esp::alive::type_scav_players::skeleton;
        config_json[_("tsp5")] = var::esp::alive::type_scav_players::price;

        //type_bosses
        config_json[_("tb1")] = var::esp::alive::type_bosses::box;
        config_json[_("tb2")] = var::esp::alive::type_bosses::info;
        config_json[_("tb3")] = var::esp::alive::type_bosses::weapon;
        config_json[_("tb4")] = var::esp::alive::type_bosses::skeleton;
        config_json[_("tb5")] = var::esp::alive::type_bosses::price;

        //type_scavs
        config_json[_("ts1")] = var::esp::alive::type_scavs::box;
        config_json[_("ts2")] = var::esp::alive::type_scavs::info;
        config_json[_("ts3")] = var::esp::alive::type_scavs::weapon;
        config_json[_("ts4")] = var::esp::alive::type_scavs::skeleton;
        config_json[_("ts5")] = var::esp::alive::type_scavs::price;

        //item
        config_json[_("i1")] = var::esp::item::enable;
        config_json[_("i2")] = var::esp::item::bind_item;
        config_json[_("i3")] = var::esp::item::on_ground;
        config_json[_("i4")] = var::esp::item::in_container;
        config_json[_("i5")] = var::esp::item::in_corpse;
        config_json[_("i6")] = var::esp::item::price;
        config_json[_("i7")] = var::esp::item::name;
        config_json[_("i8")] = var::esp::item::minimal_price;
        config_json[_("i9")] = var::esp::item::distance;

        config_json[_("ic501")] = int(var::esp::item::color_50.Value.x * 255);
        config_json[_("ic502")] = int(var::esp::item::color_50.Value.y * 255);
        config_json[_("ic503")] = int(var::esp::item::color_50.Value.z * 255);

        config_json[_("ic1001")] = int(var::esp::item::color_100.Value.x * 255);
        config_json[_("ic1002")] = int(var::esp::item::color_100.Value.y * 255);
        config_json[_("ic1003")] = int(var::esp::item::color_100.Value.z * 255);

        config_json[_("ic1501")] = int(var::esp::item::color_150.Value.x * 255);
        config_json[_("ic1502")] = int(var::esp::item::color_150.Value.y * 255);
        config_json[_("ic1503")] = int(var::esp::item::color_150.Value.z * 255);

        config_json[_("ic2001")] = int(var::esp::item::color_200.Value.x * 255);
        config_json[_("ic2002")] = int(var::esp::item::color_200.Value.y * 255);
        config_json[_("ic2003")] = int(var::esp::item::color_200.Value.z * 255);

        config_json[_("ic10001")] = int(var::esp::item::color_1000.Value.x * 255);
        config_json[_("ic10002")] = int(var::esp::item::color_1000.Value.y * 255);
        config_json[_("ic10003")] = int(var::esp::item::color_1000.Value.z * 255);

        config_json[_("i10")] = var::esp::item::use_price_filter;

        //exit
        config_json[_("e1")] = var::esp::exit::enable;
        config_json[_("e2")] = var::esp::exit::window;

        config_json[_("e3")] = int(var::esp::exit::color.Value.x * 255);
        config_json[_("e4")] = int(var::esp::exit::color.Value.y * 255);
        config_json[_("e5")] = int(var::esp::exit::color.Value.z * 255);

        //grenade
        config_json[_("g1")] = var::esp::grenade::enable;

        config_json[_("g2")] = int(var::esp::grenade::color.Value.x * 255);
        config_json[_("g3")] = int(var::esp::grenade::color.Value.y * 255);
        config_json[_("g4")] = int(var::esp::grenade::color.Value.z * 255);

        //corpse
        config_json[_("c1")] = var::esp::corpse::enable;

        config_json[_("c2")] = int(var::esp::corpse::color.Value.x * 255);
        config_json[_("c3")] = int(var::esp::corpse::color.Value.y * 255);
        config_json[_("c4")] = int(var::esp::corpse::color.Value.z * 255);

        //aim
        config_json[_("a1")] = var::aim::enable;
        config_json[_("a2")] = var::aim::fov;
        config_json[_("a3")] = var::aim::aim_line;
        config_json[_("a4")] = var::aim::prediction;
        config_json[_("a5")] = var::aim::prediction;
        config_json[_("a6")] = var::aim::fov_radius;
        config_json[_("a7")] = var::aim::predict_distance;
        config_json[_("a8")] = var::aim::current_bone_id;
        config_json[_("a9")] = var::aim::distance;
        config_json[_("a10")] = var::aim::bind_aim;
        config_json[_("a11")] = var::aim::priority;

        //misc
        config_json[_("m1")] = var::misc::inertia;
        config_json[_("m2")] = var::misc::bind_inertia;

        config_json[_("m3")] = var::misc::recoil;
        config_json[_("m4")] = var::misc::bind_recoil;

        config_json[_("m5")] = var::misc::stamina;
        config_json[_("m6")] = var::misc::bind_stamina;

        config_json[_("m7")] = var::misc::visor;
        config_json[_("m8")] = var::misc::bind_visor;

        config_json[_("m9")] = var::misc::thermal;
        config_json[_("m10")] = var::misc::bind_thermal;

        config_json[_("m11")] = var::misc::night;
        config_json[_("m12")] = var::misc::bind_night;

        config_json[_("m13")] = var::misc::speed;
        config_json[_("m14")] = var::misc::bind_speed;

        config_json[_("m15")] = var::misc::jump;
        config_json[_("m16")] = var::misc::bind_jump;

        config_json[_("m17")] = var::misc::loot_walls;
        config_json[_("m18")] = var::misc::bind_loot_walls;

        config_json[_("m19")] = var::esp::show_radar; 
        config_json[_("m20")] = var::misc::speed_multiply;


        //work with file
        std::ofstream file;

        file.open(std::string(var::path_config_list + conf.name + _(".config")).c_str());

        if (file.is_open())
        {
            file << config_json;
        }

        file.close();
    }

    void load_settings(var::config::custom_config& conf)
    {
        json config_json;

        try
        {
            config_json = load_as_json(var::path_config_list + conf.name + _(".config"));
        }
        catch (const std::exception& ex)
        {
            return;
        }

        if (config_json.empty())
        {
            return;
        }

        //all
        var::main_color = { config_json[_("mc1")], config_json[_("mc2")], config_json[_("mc3")], 255 };

        var::debug = config_json[_("dd")];

        //players
        var::esp::alive::players = config_json[_("p1")];
        var::esp::alive::bind_players = config_json[_("p2")];
        var::esp::alive::color_players = { config_json[_("p3")], config_json[_("p4")], config_json[_("p5")], 255 };

        //scavs
        var::esp::alive::scavs = config_json[_("s1")];
        var::esp::alive::bind_scavs = config_json[_("s2")];
        var::esp::alive::color_scavs = { config_json[_("s3")], config_json[_("s4")], config_json[_("s5")], 255};

        //scav_players
        var::esp::alive::scav_players = config_json[_("sp1")];
        var::esp::alive::bind_scav_players = config_json[_("sp2")];
        var::esp::alive::color_scav_players = { config_json[_("sp3")], config_json[_("sp4")], config_json[_("sp5")], 255};

        //bosses
        var::esp::alive::bosses = config_json[_("b1")];
        var::esp::alive::bind_bosses = config_json[_("b2")];
        var::esp::alive::color_bosses = { config_json[_("b3")], config_json[_("b4")], config_json[_("b5")], 255};

        //show inv
        var::esp::alive::show_inventory = config_json[_("si1")];
        var::esp::alive::bind_show_inventory = config_json[_("si2")];

        //minimal
        var::esp::alive::minimal = config_json[_("mi1")];
        var::esp::alive::bind_minimal = config_json[_("mi2")];

        //chams
        var::esp::alive::chams = config_json[_("c1")];

        //distance
        var::esp::alive::distance = config_json[_("d1")];

        //type_global
        var::esp::alive::type_global::box = config_json[_("tg1")];
        var::esp::alive::type_global::info = config_json[_("tg2")];
        var::esp::alive::type_global::weapon = config_json[_("tg3")];
        var::esp::alive::type_global::skeleton = config_json[_("tg4")];
        var::esp::alive::type_global::price = config_json[_("tg5")];

        //type_players
        var::esp::alive::type_players::box = config_json[_("tp1")];
        var::esp::alive::type_players::info = config_json[_("tp2")];
        var::esp::alive::type_players::weapon = config_json[_("tp3")];
        var::esp::alive::type_players::skeleton = config_json[_("tp4")];
        var::esp::alive::type_players::price = config_json[_("tp5")];

        //type_scav_players
        var::esp::alive::type_scav_players::box = config_json[_("tsp1")];
        var::esp::alive::type_scav_players::info = config_json[_("tsp2")];
        var::esp::alive::type_scav_players::weapon = config_json[_("tsp3")];
        var::esp::alive::type_scav_players::skeleton = config_json[_("tsp4")];
        var::esp::alive::type_scav_players::price = config_json[_("tsp5")];

        //type_bosses
        var::esp::alive::type_bosses::box = config_json[_("tb1")];
        var::esp::alive::type_bosses::info = config_json[_("tb2")];
        var::esp::alive::type_bosses::weapon = config_json[_("tb3")];
        var::esp::alive::type_bosses::skeleton = config_json[_("tb4")];
        var::esp::alive::type_bosses::price = config_json[_("tb5")];

        //type_scavs
        var::esp::alive::type_scavs::box = config_json[_("ts1")];
        var::esp::alive::type_scavs::info = config_json[_("ts2")];
        var::esp::alive::type_scavs::weapon = config_json[_("ts3")];
        var::esp::alive::type_scavs::skeleton = config_json[_("ts4")];
        var::esp::alive::type_scavs::price = config_json[_("ts5")];

        //item
        var::esp::item::enable = config_json[_("i1")];
        var::esp::item::bind_item = config_json[_("i2")];
        var::esp::item::on_ground = config_json[_("i3")];
        var::esp::item::in_container = config_json[_("i4")];
        var::esp::item::in_corpse = config_json[_("i5")];
        var::esp::item::price = config_json[_("i6")];
        var::esp::item::name = config_json[_("i7")];
        var::esp::item::minimal_price = config_json[_("i8")];
        var::esp::item::distance = config_json[_("i9")];

        var::esp::item::color_50 = { config_json[_("ic501")], config_json[_("ic502")], config_json[_("ic503")], 255 };

        var::esp::item::color_100 = { config_json[_("ic1001")], config_json[_("ic1002")], config_json[_("ic1003")], 255 };

        var::esp::item::color_150 = { config_json[_("ic1501")], config_json[_("ic1502")], config_json[_("ic1503")], 255 };

        var::esp::item::color_200 = { config_json[_("ic2001")], config_json[_("ic2002")], config_json[_("ic2003")], 255 };

        var::esp::item::color_1000 = { config_json[_("ic10001")], config_json[_("ic10002")], config_json[_("ic10003")], 255};

        var::esp::item::use_price_filter = config_json[_("i10")];

        //exit
        var::esp::exit::enable = config_json[_("e1")];
        var::esp::exit::window = config_json[_("e2")];

        var::esp::exit::color = { config_json[_("e3")], config_json[_("e4")], config_json[_("e5")], 255 };

        //grenade
        var::esp::grenade::enable = config_json[_("g1")];

        var::esp::grenade::color = { config_json[_("g2")], config_json[_("g3")], config_json[_("g4")], 255 };

        //corpse
        var::esp::corpse::enable = config_json[_("c1")];

        var::esp::corpse::color = { config_json[_("c2")], config_json[_("c3")], config_json[_("c4")], 255};

        //aim
        var::aim::enable = config_json[_("a1")];
        var::aim::fov = config_json[_("a2")];
        var::aim::aim_line = config_json[_("a3")];
        var::aim::prediction = config_json[_("a4")];
        var::aim::prediction = config_json[_("a5")];
        var::aim::fov_radius = config_json[_("a6")];
        var::aim::predict_distance = config_json[_("a7")];
        var::aim::current_bone_id = config_json[_("a8")];
        var::aim::distance = config_json[_("a9")];
        var::aim::bind_aim = config_json[_("a10")];
        var::aim::priority = config_json[_("a11")];

        //misc
        var::misc::inertia = config_json[_("m1")];
        var::misc::bind_inertia = config_json[_("m2")];

        var::misc::recoil = config_json[_("m3")];
        var::misc::bind_recoil = config_json[_("m4")];

        var::misc::stamina = config_json[_("m5")];
        var::misc::bind_stamina = config_json[_("m6")];

        var::misc::visor = config_json[_("m7")];
        var::misc::bind_visor = config_json[_("m8")];

        var::misc::thermal = config_json[_("m9")];
        var::misc::bind_thermal = config_json[_("m10")];

        var::misc::night = config_json[_("m11")];
        var::misc::bind_night = config_json[_("m12")];

        var::misc::speed = config_json[_("m13")];
        var::misc::bind_speed = config_json[_("m14")];

        var::misc::jump = config_json[_("m15")];
        var::misc::bind_jump = config_json[_("m16")];

        var::misc::loot_walls = config_json[_("m17")];
        var::misc::bind_loot_walls = config_json[_("m18")];
        var::esp::show_radar = config_json[_("m19")];
        var::misc::speed_multiply = config_json[_("m20")];
    }
}