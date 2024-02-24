#pragma once

#include "../main.hpp"

namespace offset
{
	u64 object_manager = 0x17ffd28;
	u64 time_scale = 0x17ffae0;

	namespace object
	{
		vec<u64> position{ 0x10, 0x30, 0x30, 0x8, 0x28 };
		vec<u64> name{ 0x0, 0x0, 0x48 };
	}

	//Application(Main Client)->ComponentArray(0x30)[1](0x10 * index + 0x8 = 0x18)(Tarkov application component)->
	//->ScriptingObject(0x28)->Interface(0x28)->Class(0x48)->BackEndConfig(0xa8)->Config(0x10)->InertiaConfig(0xd8)
	namespace application
	{
		//Application(Main Client)->ComponentArray(0x30)[1](0x10 * index + 0x8 = 0x18)(Tarkov application component)->
		//->ScriptingObject(0x28)->Interface(0x28)->Class(0x48)->BackEndConfig(0xa8)->Config(0x10)->InertiaConfig(0xd0)

		vec<u64> inertia{ 0x28, 0x58, 0xc0, 0x10, 0xD8 };
	}

	namespace raycast_loot
	{
		u64 instance = 0x14;
		u64 distance = 0x210;//0x204; //
	}
	namespace camera
	{
		u64 matrix = 0xdc;
		u64 scope_matrix = 0x5c;
	}

	namespace world
	{
		u64 players = 0xf0;//0xc8;
		u64 players_test = 0xc8;

		u64 items = 0xc8; //0xa0;   
		u64 grenades = 0x1a0;//0x178; //
		u64 exits = 0x18;
	}

	namespace grenade_controller
	{
		u64 grenades = 0x18;
	}

	namespace exit_controller
	{
		u64 exits = 0x20;
	}

	namespace exit
	{
		u64 status = 0xa8;
		u64 settings = 0x58;
		u64 settings_name = 0x10;
	}

	namespace inventory
	{
		u64 inventory = 0x130;
		u64 inventory_equipment = 0x10;
	}

	namespace eft_player
	{
		u64 character = 0x28;
		u64 movement = 0x40;
		u64 body = 0xa8;
		u64 physical = 0x598;//0x540;
		u64 hands = 0x5e8;// 0x588;
		u64 profile = 0x588;//0x530;
		u64 inventory = 0x5e0; // 0x580;
		u64 pwa = 0x1a0;

	}
	namespace observed_player_view
	{
		u64 character = 0x28;

		vec<u64> movement{ 0x80, 0xE8, 0x10 };//{ 0x80, 0xC0, 0x10 }; 
		vec<u64> hands = { 0x80, 0xF8 }; //0xD0//!!
		u64 body = 0x60;
		u64 physical = 0x530;
		//u64 hands = 0x578;
		u64 is_ai = 0x109; //0xf2;
		u64 profile = 0x520;
		vec<u64> inventory = { 0x80, 0x138 }; //0x110 //!!!
		vec<u64> velocity = { 0x80, 0xE8, 0x10, 0xD8 }; //ObservedPlayerController.StateContext.Velocity PlayerController -> 0x10 shift -> StateContext.Velocity //{ 0x80, 0xC0, 0x10, 0xD8 }
		//u64 pwa = 0x1a0;


	}
	//vec<u64> pwa = { 0xD0, 0xB0 };
	namespace character
	{
		u64 target_speed = 0x48; // 0x48 - online | 0xE4 - offline
	}

	namespace movement
	{
		u64 view_angles = 0x25c; // 78;//0x234;
	}

	namespace physical
	{
		u64 stamina = 0x38;
		u64 hands_stamina = 0x40;
		u64 oxygen = 0x48;
	}

	namespace stamina
	{
		u64 current = 0x48;
	}

	namespace skeleton
	{
		u64 values = 0x28;
	}

	namespace profile
	{
		u64 info = 0x28;
		u64 skills = 0x60;
		u64 stats = 0xf0;
		u64 stats_overall_counters = 0x18;
		u64 stats_overall_counters_counters = 0x10;
	}

	namespace skills
	{
		u64 strength_buff_jump_height_inc = 0x60;
		u64 value = 0x30;
	}

	namespace counters
	{
		u64 kills = 0x108;
		u64 deaths = 0x120;
	}

	namespace settings
	{
		u64 role = 0x10;
	}

	namespace hands
	{
		u64 item = 0x60;
		u64 item_in_hands = 0x58;
	}

	namespace body
	{
		//u64 skeleton_online = 
		u64 skeleton = 0x28;
	}

	namespace item
	{
		u64 item_template = 0x40;
		u64 stack_count = 0x64;
		u64 grids = 0x70;
		u64 slots = 0x78;
		u64 stack_slot = 0x90;
	}

	namespace weapon
	{
		u64 chambers = 0xa8;
		u64 mag_slot = 0xc8;
		u64 shells = 0xe0;
	}

	namespace stack_slot
	{
		u64 max_ammo = 0x38;
	}

	namespace grid
	{
		u64 collection = 0x40;
		u64 collection_dict = 0x10;
	}

	namespace slot
	{
		u64 name = 0x18;
		u64 item = 0x40;
	}

	namespace loot_item
	{
		u64 item = 0xb0;
	}

	namespace item_template
	{
		u64 is_quest = 0x9c;
		u64 is_lootable = 0x104;

		u64 id = 0x50;
		u64 ammo_template = 0x170; // if weapon
	}

	namespace ammo_template
	{
		u64 speed = 0x18c;
	}

	namespace thermal
	{
		u64 enable = 0xe0;
		//u64 is_noisy = 0xd9;
		//u64 is_fps_stuck = 0xda;
		//u64 motion_blur = 0xdb;
		//u64 is_glitched = 0xdc;
		//u64 Ppixellated = 0xdd;
	}

	namespace night
	{
		u64 on = 0xec;
		u64 intensity = 0xc8;
		u64 mask = 0xcc;
	}

	namespace visor
	{
		u64 intensity = 0xc0;
	}

	namespace procedural_weapon_animation
	{
		u64 fire_arm = 0xa8;// 0x98;
		u64 mask = 0x138;
		u64 breath_effector = 0x28;
		u64 walk_effector = 0x30;
		u64 motion_effector = 0x38;
		u64 force_effector = 0x40;
		u64 shot_effector = 0x48;

	}

	namespace fire_arm
	{
		u64 weapon_ln = 0x174;
		u64 fireport = 0xd0;
	}

	namespace breath_effector
	{
		u64 intensity = 0xa4;
	}

	namespace walk_effector
	{
		u64 intensity = 0x44;
	}

	namespace motion_effector
	{
		u64 intensity = 0xd0;
	}

	namespace force_effector
	{
		u64 intensity = 0x30;
	}
}