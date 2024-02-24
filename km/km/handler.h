#include "physmem.hpp"
#include "crypt.hpp"

#pragma once

namespace SETTINGS
{
    uint64_t PacketAddress = NULL;
	uint64_t PacketAddress1 = NULL;
    PEPROCESS PeBeef = NULL;
    PEPROCESS PeUM = NULL;
}



VOID Wait(LONG usec)
{
	ULONG ulReturn = 0;
	if (usec < 50000)
	{
		/* KeDelayExecutionThread yields the process */
		/* Waiting times lower than 50ms are not accurate */
		/* Because of this we need to busy wait here */
		KeStallExecutionProcessor(usec);
	}
	else
	{
		LARGE_INTEGER waittime;
		/* specifies the wait time in steps of 100ns */
		/* Negative values specify a relative time offset */
		waittime.QuadPart = -1 * usec * 10;
		if (STATUS_SUCCESS == KeDelayExecutionThread(KernelMode, FALSE, &waittime))
			ulReturn = 0;
		else
			ulReturn = 1;
	}
}
BOOLEAN initialized = false;
void InitHandler()
{
    //process_dirbase = 0;
    PsLookupProcessByProcessId((HANDLE)Packet->Data.Init.Process, &SETTINGS::PeBeef);
	initialized = true;

}
UINT64 GetModuleBase(PEPROCESS Process, UNICODE_STRING module_name)
{
	PPEB peb = PsGetProcessPeb(Process);

	if (!peb)
	{
		return 0;
	}
	KAPC_STATE state;
	KeStackAttachProcess(Process, &state);
	PPEB_LDR_DATA ldr = peb->Ldr;

	if (!ldr)
	{
		KeUnstackDetachProcess(&state);
		return 0;
	}

	for (PLIST_ENTRY listEntry = (PLIST_ENTRY)ldr->InLoadOrderModuleList.Flink; listEntry != &ldr->InLoadOrderModuleList; listEntry = (PLIST_ENTRY)listEntry->Flink)
	{
		PLDR_DATA_TABLE_ENTRY ldrEntry = CONTAINING_RECORD(listEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (RtlCompareUnicodeString(&ldrEntry->BaseDllName, &module_name, TRUE) == 0)
		{
			ULONG64 baseAddr = (ULONG64)ldrEntry->DllBase;
			KeUnstackDetachProcess(&state);
			return baseAddr;
		}
	}
	KeUnstackDetachProcess(&state);
	return 0;
}
void BaseHandler()
{


	//Wait(5000000);
	UNICODE_STRING unity_player_name;
	UNICODE_STRING mono_dll_name;

	RtlInitUnicodeString(&unity_player_name, skCrypt(L"UnityPlayer.dll"));
	RtlInitUnicodeString(&mono_dll_name, skCrypt(L"mono-2.0-bdwgc.dll"));

	uint64_t unity_player_address = GetModuleBase(SETTINGS::PeBeef, unity_player_name);
	uint64_t mono_module_address = GetModuleBase(SETTINGS::PeBeef, mono_dll_name);

	Packet->Data.Module.mono_address = mono_module_address;
	Packet->Data.Module.unity_player_address = unity_player_address;

	//ObfDereferenceObject(process);
	//uint64_t Address = (uint64_t)PsGetProcessSectionBaseAddress(SETTINGS::PeBeef);
   // Packet->Data.Module.Address = Address;
}

void ReadHandler()
{
    SIZE_T Bytes;
    MmCopyVirtualMemory(SETTINGS::PeBeef, (PVOID)Packet->Data.Read.Address, SETTINGS::PeUM, (PVOID)Packet->Data.Read.Responce, Packet->Data.Read.Size, UserMode, &Bytes);
    //ReadProcessMemory(SETTINGS::PeBeef, (PVOID)Packet->Data.Read.Address, (PVOID)Packet->Data.Read.Responce, Packet->Data.Read.Size);//&Bytes
} 

void WriteHandler()
{
    SIZE_T Bytes;
    MmCopyVirtualMemory(SETTINGS::PeUM, (PVOID)Packet->Data.Write.Value, SETTINGS::PeBeef, (PVOID)Packet->Data.Write.Address, Packet->Data.Write.Size, UserMode, &Bytes);
     // WriteProcessMemory(SETTINGS::PeBeef, (PVOID)Packet->Data.Write.Address, (PVOID)Packet->Data.Write.Value, Packet->Data.Write.Size, &Bytes);
   
}

void UnmapHandler()
{
    if (Packet)
    {
        Packet->Type = _Type::Complete;
        MmUnmapIoSpace(Packet, sizeof(_Packet));
    }
}

//namespace thr_1
//{
//	void InitHandler()
//	{
//		//process_dirbase = 0;
//		PsLookupProcessByProcessId((HANDLE)Packet_1->Data.Init.Process, &SETTINGS::PeBeef);
//	}
//	UINT64 GetModuleBase(PEPROCESS Process, UNICODE_STRING module_name)
//	{
//		PPEB peb = PsGetProcessPeb(Process);
//
//		if (!peb)
//		{
//			return 0;
//		}
//		KAPC_STATE state;
//		KeStackAttachProcess(Process, &state);
//		PPEB_LDR_DATA ldr = peb->Ldr;
//
//		if (!ldr)
//		{
//			KeUnstackDetachProcess(&state);
//			return 0;
//		}
//
//		for (PLIST_ENTRY listEntry = (PLIST_ENTRY)ldr->InLoadOrderModuleList.Flink; listEntry != &ldr->InLoadOrderModuleList; listEntry = (PLIST_ENTRY)listEntry->Flink)
//		{
//			PLDR_DATA_TABLE_ENTRY ldrEntry = CONTAINING_RECORD(listEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
//			if (RtlCompareUnicodeString(&ldrEntry->BaseDllName, &module_name, TRUE) == 0)
//			{
//				ULONG64 baseAddr = (ULONG64)ldrEntry->DllBase;
//				KeUnstackDetachProcess(&state);
//				return baseAddr;
//			}
//		}
//		KeUnstackDetachProcess(&state);
//		return 0;
//	}
//	void BaseHandler()
//	{
//
//		//UNICODE_STRING DLLName;
//		//RtlInitUnicodeString(&DLLName,skCrypt( L"GameAssembly.dll"));
//		uint64_t Address = (uint64_t)PsGetProcessSectionBaseAddress(SETTINGS::PeBeef); //GetModuleBase(SETTINGS::PeBeef, DLLName);
//		Packet_1->Data.Module.Address = Address;
//
//		//ObfDereferenceObject(process);
//		//uint64_t Address = (uint64_t)PsGetProcessSectionBaseAddress(SETTINGS::PeBeef);
//	   // Packet->Data.Module.Address = Address;
//	}
//
//	void ReadHandler()
//	{
//		SIZE_T Bytes;
//		MmCopyVirtualMemory(SETTINGS::PeBeef, (PVOID)Packet_1->Data.Read.Address, SETTINGS::PeUM, (PVOID)Packet_1->Data.Read.Responce, Packet_1->Data.Read.Size, UserMode, &Bytes);
//		//ReadProcessMemory(SETTINGS::PeBeef, (PVOID)Packet->Data.Read.Address, (PVOID)Packet->Data.Read.Responce, Packet->Data.Read.Size);//&Bytes
//	}
//
//	void WriteHandler()
//	{
//		SIZE_T Bytes;
//		MmCopyVirtualMemory(SETTINGS::PeUM, (PVOID)Packet_1->Data.Write.Value, SETTINGS::PeBeef, (PVOID)Packet_1->Data.Write.Address, Packet_1->Data.Write.Size, UserMode, &Bytes);
//		// WriteProcessMemory(SETTINGS::PeBeef, (PVOID)Packet->Data.Write.Address, (PVOID)Packet->Data.Write.Value, Packet->Data.Write.Size, &Bytes);
//
//	}
//
//	void UnmapHandler()
//	{
//		if (Packet_1)
//		{
//			Packet_1->Type = _Type::Complete;
//			MmUnmapIoSpace(Packet_1, sizeof(_Packet));
//		}
//	}
//}
