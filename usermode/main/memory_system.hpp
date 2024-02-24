#pragma once

#include "../main.hpp"
#include <mutex>
//#include "..//tarkov/game.hpp"


//#define OFFLINE_MODE
#define ONLINE_MODE

uint64_t mono_copy;
std::mutex r_w_mutex;
std::mutex r_w_mutex1;










namespace memory_system
{
	enum class _Type
	{
		Init,
		Read,
		Write,
		Base,
		Complete,
		Unmap
	};

	struct _Packet
	{
		_Type Type;
		union
		{
			struct { uint32_t Process; }												Init;
			struct { uint64_t Address; uint64_t Responce; uint32_t Size; }				Read;
			struct { uint64_t Address; uint64_t Value; uint32_t Size; }					Write;
			struct { uint64_t mono_address; uint64_t unity_player_address; }			Module;
		} Data;
	};

	volatile _Packet Packet;
	volatile _Packet Packet_1;
	


	void create_config()
	{
		remove(xor ("C:\\Windows\\Packet_0"));
		remove(xor ("C:\\Windows\\Packet_1"));
		remove(xor ("C:\\Windows\\Usermode"));

		std::ofstream Packetfile(xor ("C:\\Windows\\Packet_0"), std::ios::out);
		Packetfile << std::hex << (void*)&Packet; Packetfile.close();

		std::ofstream Packetfile_1(xor ("C:\\Windows\\Packet_1"), std::ios::out);
		Packetfile_1 << std::hex << (void*)&Packet_1; Packetfile_1.close();
		

		std::ofstream Usermodefile(xor ("C:\\Windows\\Usermode"), std::ios::out);
		Usermodefile << std::dec << GetCurrentProcessId(); Usermodefile.close();
	}

	void delete_config()
	{
		remove(xor ("C:\\Windows\\Packet_0"));
		remove(xor ("C:\\Windows\\Packet_1"));
		remove(xor ("C:\\Windows\\Usermode"));
	}

	int get_process_by_name(PCSTR Name)
	{
		DWORD Id = 0;
		PROCESSENTRY32 Process;
		HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		ZeroMemory(&Process, sizeof(Process)); Process.dwSize = sizeof(Process);

		if (Process32First(Snapshot, &Process))
		{
			do
			{
				if (std::string(Process.szExeFile) == std::string(Name))
				{
					Id = Process.th32ProcessID;
					break;
				}
			} while (Process32Next(Snapshot, &Process));
		}
		CloseHandle(Snapshot);

		if (Id != 0) { return Id; };
		return NULL;
	}

	void wait_complete()
	{
		while (Packet.Type != _Type::Complete)
			Sleep(0);

	}

	void wait_complete1()
	{
		while (Packet_1.Type != _Type::Complete)
			Sleep(0);

	}
	void init(uint32_t Beef)
	{
		Packet.Data.Init.Process = Beef;
		Packet.Type = _Type::Init;
		//Packet_1.Data.Init.Process = Beef;
		//Packet_1.Type = _Type::Init;

		wait_complete();
		//wait_complete1();
	}

	void unmap()
	{
		Packet.Type = _Type::Unmap;

		wait_complete();
	}

	void copy_memory(uint64_t from, uint64_t to, size_t size, _Type type)
	{

		std::lock_guard<std::mutex> lock_control(r_w_mutex);
		if (type == _Type::Read)
		{
			Packet.Data.Read.Address = from;
			Packet.Data.Read.Responce = (uint64_t)to;
			Packet.Data.Read.Size = size;
			Packet.Type = _Type::Read;
			wait_complete();
		}
		else
		{
			Packet.Data.Write.Address = to;
			Packet.Data.Write.Value = from;
			Packet.Data.Write.Size = size;
			Packet.Type = _Type::Write;
			wait_complete();

		}


	}

	//void copy_memory1(uint64_t from, uint64_t to, size_t size, _Type type)
	//{

	//	std::lock_guard<std::mutex> lock_control(r_w_mutex1);
	//	if (type == _Type::Read)
	//	{
	//		Packet_1.Data.Read.Address = from;
	//		Packet_1.Data.Read.Responce = (uint64_t)to;
	//		Packet_1.Data.Read.Size = size;
	//		Packet_1.Type = _Type::Read;
	//		wait_complete1();
	//	}
	//	else
	//	{
	//		Packet_1.Data.Write.Address = to;
	//		Packet_1.Data.Write.Value = from;
	//		Packet_1.Data.Write.Size = size;
	//		Packet_1.Type = _Type::Write;
	//		wait_complete1();

	//	}


	//}

	void read_memory(uint64_t from, void* to, size_t size)
	{
		copy_memory(from, (uint64_t)to, size, _Type::Read);
	}
	template<typename T>
	T read(uint64_t Address)
	{



		T Buffer;
		ZeroMemory(&Buffer, sizeof(T));
		//if(Packet.Type == _Type::Complete)
			copy_memory(Address, (uint64_t)&Buffer, sizeof(T), _Type::Read);
		//else
			//copy_memory1(Address, (uint64_t)&Buffer, sizeof(T), _Type::Read);

		return Buffer;




	}
	template<typename type>
	type read(u64 address, vec<u64> chain)
	{
		u64 current = address;

		for (u32 i = 0; i < chain.size() - 1; i++) { current = read<u64>(current + chain[i]); }

		return read<type>(current + chain[chain.size() - 1]);
	}
	template<typename T>
	void write(uint64_t Address, T Value)
	{


		T Buffer = Value;
		//if (Packet.Type == _Type::Complete)
			copy_memory((uint64_t)&Buffer, Address, sizeof(T), _Type::Write);
		//else
			//copy_memory1((uint64_t)&Buffer, Address, sizeof(T), _Type::Write);
		//Packet.Data.Write.Address = Address;
		//Packet.Data.Write.Value = (uint64_t)&Buffer;
		//Packet.Data.Write.Size = sizeof(T);
		//Packet.Type = _Type::Write;

		//WaitComplete();
	}

	void get_base_values(uint64_t* mono_module, uint64_t * unity_player_module)
	{
		uint64_t mono = 0;
		uint64_t unity = 0;
	

		while (mono == 0 || unity == 0)
		{
			Packet.Type = _Type::Base;
			unity = Packet.Data.Module.unity_player_address;
			mono = Packet.Data.Module.mono_address;
			wait_complete();
		}
		*mono_module = mono;
		*unity_player_module = unity;

		

	}

}