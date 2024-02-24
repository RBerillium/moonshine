#include <ntifs.h>
#include "stdint.h"
#include "import.hpp"
#include "struct.h"
#include "handler.h"
#include "physmem.hpp"
#include "crypt.hpp"






DWORD64 CharToInt(CHAR* Text, INT System)
{
	DWORD64 Buffer = 0;
	CHAR* CharNumbers = skCrypt("0123456789ABCDEF");
	INT IntNumbers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
	DWORD64 K = 1;

	for (INT A = strlen(Text); A >= 0; A--)
	{
		for (INT B = 0; B < strlen(CharNumbers); B++)
		{
			if (Text[A] == CharNumbers[B])
			{
				Buffer += IntNumbers[B] * K;
				K *= System;
			}
		}
	}
	return Buffer;
}

DWORD64 ReadVarFromFile(PCWSTR Path, INT System)
{
	CONST INT          Size = 17;
	CHAR               Buffer[Size];
	UNICODE_STRING     Name;
	OBJECT_ATTRIBUTES  Object;
	DWORD64            Result = 0;

	RtlInitUnicodeString(&Name, Path);
	InitializeObjectAttributes(&Object, &Name, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	HANDLE          Handle;
	NTSTATUS        Status;
	IO_STATUS_BLOCK IoStatusBlock;
	LARGE_INTEGER   Bytes;

	Status = ZwCreateFile(&Handle, GENERIC_READ, &Object, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (NT_SUCCESS(Status))
	{
		Bytes.LowPart = Bytes.HighPart = 0;
		Status = ZwReadFile(Handle, NULL, NULL, NULL, &IoStatusBlock, Buffer, Size, &Bytes, NULL);

		if (NT_SUCCESS(Status))
		{
			Buffer[Size - 1] = '\0';
			Result = CharToInt(Buffer, System);
		}
	}
	ZwClose(Handle);
	return Result;
}





typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation = 0x0B
} SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;
PVOID get_system_module_base(const char* module_name)
{
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, NULL, bytes, &bytes);

	if (!bytes)
		return NULL;

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x4e554c4c);

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
		return NULL;

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	PVOID module_base = 0, module_size = 0;

	for (ULONG i = 0; i < modules->NumberOfModules; i++) {
		if (strcmp((char*)module[i].FullPathName, module_name) == NULL) {
			module_base = module[i].ImageBase;
			module_size = (PVOID)module[i].ImageSize;
			break;
		}
	}

	if (0 == 0) {
		PEPROCESS process1;
	}

	if (modules)
		ExFreePoolWithTag(modules, NULL);

	if (module_base <= NULL)
		return NULL;

	return module_base;
}

template< typename T >
void SpoofAddress(PVOID address, PVOID* save, PVOID target = 0) {
	*save = *(PVOID*)(address); //save current stuff
	//print("\n[+] saved orig data");
	*(PVOID*)(address) = target; //overwrite
	T spoof = *(T*)((ULONG64)address);
	//print("\n[+] spoofed! -> 0x%llX (%d)", spoof, spoof);
}
#define InitialStack_UP			0x28
#define KernelStack_UP			0x58
#define VCreateTime_UP			0x430
#define StartAddress_UP			0x450
#define CID_UP					0x478
#define Win32StartAddress_UP	0x4d0
#define ExitStatus_UP			0x548
//#define KernelStackReference = 0x55c
//void HideThread() {
//
//    //this is not needed lol, i just use this to get a random address ill set the start address to
//    PVOID ntoskrnlbase = (PVOID)((ULONG64)get_kernel_module(("\\SystemRoot\\system32\\ntoskrnl.exe")) + 0x23810a);
//    //PVOID ntoskrnlbase = (PVOID)((ULONG64)get_system_module_base("\\SystemRoot\\system32\\ntoskrnl.exe"));
//    //print("\n[+] Base: 0x%llX", ntoskrnlbase);
//    PVOID Kthread = reinterpret_cast<PVOID>(KeGetCurrentThread());
//    //print("\n[+] Kthread: 0x%llX", Kthread);
//
//    PVOID InitialStack = (PVOID)((ULONG64)Kthread + GInitialStack);
//    PVOID VCreateTime = (PVOID)((ULONG64)Kthread + GVCreateTime);
//    PVOID StartAddress = (PVOID)((ULONG64)Kthread + GStartAddress);
//    PVOID Win32StartAddress = (PVOID)((ULONG64)Kthread + GWin32StartAddress);
//    PVOID KernelStack = (PVOID)((ULONG64)Kthread + GKernelStack);
//    PVOID CID = (PVOID)((ULONG64)Kthread + GCID);
//    PVOID ExitStatus = (PVOID)((ULONG64)Kthread + GExitStatus);
//
//    //print("\n[+] CreateTime: 0x%llX", VCreateTime);
//    SpoofAddress<LARGE_INTEGER>(VCreateTime, &_VCreateTime, (PVOID)2147483247);
//
//    //print("\n[+] StartAddress: 0x%llX", StartAddress);
//    SpoofAddress<void*>(StartAddress, &_StartAddress, ntoskrnlbase);
//
//    //print("\n[+] Win32StartAddress: 0x%llX", Win32StartAddress);
//    SpoofAddress<void*>(Win32StartAddress, &_Win32StartAddress, ntoskrnlbase);
//
//    //print("\n[+] KernelStack: 0x%llX", KernelStack);
//    SpoofAddress<LARGE_INTEGER>(KernelStack, &_KernelStack);
//
//    SpoofAddress<CLIENT_ID>(CID, &_CID);
//
//
//    SpoofAddress<LONG>(ExitStatus, &_ExitStatus);
//
//}


void CreateThread(PWORKER_THREAD_ROUTINE Thread)
{
	KeEnterGuardedRegion();
	PWORK_QUEUE_ITEM WorkItem = (PWORK_QUEUE_ITEM)ExAllocatePool(NonPagedPool, sizeof(WORK_QUEUE_ITEM));
	ExInitializeWorkItem(WorkItem, (PWORKER_THREAD_ROUTINE)Thread, WorkItem);
	ExQueueWorkItem(WorkItem, CriticalWorkQueue);

	KeLeaveGuardedRegion();

}
VOID wait(LONG usec)
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


//typedef struct _HANDLE_TABLE
//{
//    ULONG NextHandleNeedingPool;                                            //0x0
//    LONG ExtraInfoPages;                                                    //0x4
//    volatile ULONGLONG TableCode;                                           //0x8
//    struct _EPROCESS* QuotaProcess;                                         //0x10
//    struct _LIST_ENTRY HandleTableList;                                     //0x18
//    ULONG UniqueProcessId;                                                  //0x28
//    union
//    {
//        ULONG Flags;                                                        //0x2c
//        struct
//        {
//            UCHAR StrictFIFO : 1;                                             //0x2c
//            UCHAR EnableHandleExceptions : 1;                                 //0x2c
//            UCHAR Rundown : 1;                                                //0x2c
//            UCHAR Duplicated : 1;                                             //0x2c
//            UCHAR RaiseUMExceptionOnInvalidHandleClose : 1;                   //0x2c
//        };
//    };
//    struct _EX_PUSH_LOCK HandleContentionEvent;                             //0x30
//    struct _EX_PUSH_LOCK HandleTableLock;                                   //0x38
//    union
//    {
//        struct _HANDLE_TABLE_FREE_LIST FreeLists[1];                        //0x40
//        struct
//        {
//            UCHAR ActualEntry[32];                                          //0x40
//            struct _HANDLE_TRACE_DEBUG_INFO* DebugInfo;                     //0x60
//        };
//    };
//} HANDLE_TABLE, * PHANDLE_TABLE;
//
//
//typedef BOOLEAN(*func)(const PHANDLE_TABLE, const HANDLE, const PHANDLE_TABLE_ENTRY);
//func ExDestroyHandle;

//typedef struct _LIST_ENTRY
//{
//    struct _LIST_ENTRY* Flink;                                              //0x0
//    struct _LIST_ENTRY* Blink;                                              //0x8
//}LIST_ENTRY, *PLIST_ENTRY;

typedef BOOLEAN(*func)(const PHANDLE_TABLE, const HANDLE, const PHANDLE_TABLE_ENTRY);
func ExDestroyHandle;
ULONG64* resolve(const ULONG64 addressInstruction, const int opcodeBytes, int addressBytes)
{
	addressBytes += opcodeBytes;
	const ULONG32 RelativeOffset = *reinterpret_cast<ULONG32*>(addressInstruction + opcodeBytes);
	ULONG64* FinalAddress = reinterpret_cast<ULONG64*>(addressInstruction + RelativeOffset + addressBytes);
	return FinalAddress;
}
PHANDLE_TABLE_ENTRY ExpLookupHandleTableEntry(const ULONG64* pHandleTable, const LONGLONG Handle)
{
	ULONGLONG v2; // rdx
	LONGLONG v3; // r8

	v2 = Handle & 0xFFFFFFFFFFFFFFFC;
	if (v2 >= *pHandleTable)
		return 0;
	v3 = *(pHandleTable + 1);
	if ((v3 & 3) == 1)
		return reinterpret_cast<PHANDLE_TABLE_ENTRY>(*reinterpret_cast<ULONG_PTR*>(v3 + 8 * (v2 >> 10) - 1) + 4 * (v2 & 0x3FF));
	if ((v3 & 3) != 0)
		return reinterpret_cast<PHANDLE_TABLE_ENTRY>(*reinterpret_cast<ULONG_PTR*>(*reinterpret_cast<ULONG_PTR*>(v3 + 8 * (v2 >> 19) - 2) + 8 * ((v2 >> 10) & 0x1FF)) + 4 * (v2 & 0x3FF));
	return reinterpret_cast<PHANDLE_TABLE_ENTRY>(v3 + 4 * v2);
}
void DestroyPspCidTableEntry(const ULONG64* pPspCidTable, const HANDLE threadId)
{
	ULONG64* pHandleTable = reinterpret_cast<ULONG64*>(*pPspCidTable); //deref for pointer to handle table
	PHANDLE_TABLE_ENTRY pCidEntry = ExpLookupHandleTableEntry(pHandleTable, reinterpret_cast<LONGLONG>(threadId));


	if (pCidEntry != NULL)
	{
		ExDestroyHandle(reinterpret_cast<PHANDLE_TABLE>(pHandleTable), threadId, pCidEntry);
	}
}
void spoof_thread()
{


	PKTHREAD mythread = KeGetCurrentThread();
	KeSetBasePriorityThread(mythread, 32);
	//get ntos for making clean
	auto module_name = skCrypt("ntoskrnl.exe");
	uint64_t ntos_base = get_kernel_module(module_name);
	if (!ntos_base)
		return;
	module_name.clear();
	auto psp_cid_table_pattern = skCrypt("\x48\x8B\x05\xCC\xCC\xCC\xCC\x0F\xB6\xEA");
	auto psp_mask = skCrypt("xxx????xxx");
	auto ex_destroy_handle_pattern = skCrypt("\xE8\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x48\x8B\x8B\x18\x06\x00\x00");
	auto ex_destroy_mask = skCrypt("x????x????xxxxxxx");
	ULONG64* pPspCidTable = resolve(pattern_scan(ntos_base, psp_cid_table_pattern, psp_mask), 3, 4);
	ULONG64* pExDestroyHandle = resolve(pattern_scan(ntos_base, ex_destroy_handle_pattern, ex_destroy_mask), 1, 4);

	psp_cid_table_pattern.clear();
	psp_mask.clear();
	ex_destroy_handle_pattern.clear();
	ex_destroy_mask.clear();

	ExDestroyHandle = reinterpret_cast<func>(pExDestroyHandle);
	// DestroyPspCidTableEntry(pPspCidTable, PsGetCurrentThreadId());
	LIST_ENTRY plist_ethread = *reinterpret_cast<LIST_ENTRY*>((ULONG64)mythread + 0x4e8);
	PVOID Kthread = reinterpret_cast<PVOID>(KeGetCurrentThread());
	wait(100000);


	PKPRCB_META prcb = KeGetCurrentPrcb();
	PKTHREAD current_thread = prcb->CurrentThread;
	LIST_ENTRY* ethread_list_entry = (LIST_ENTRY*)make_ptr(current_thread, 0x4e8); //ethread KPRCB
	LIST_ENTRY* kthread_list_entry = (LIST_ENTRY*)make_ptr(current_thread, 0x2f8); //kthread KPRCB
	





	RemoveEntryListUnsafe(&mythread->ThreadListEntry);
	RemoveEntryListUnsafe(&plist_ethread);
	RemoveEntryListUnsafe(ethread_list_entry);
	RemoveEntryListUnsafe(kthread_list_entry); // TEST!!!





	PETHREAD1 ethread = reinterpret_cast<PETHREAD1>(mythread);
	ethread->KernelStackReference = 0;


	PVOID InitialStack = (PVOID)((ULONG64)Kthread + InitialStack_UP);
	PVOID VCreateTime = (PVOID)((ULONG64)Kthread + VCreateTime_UP);
	PVOID StartAddress = (PVOID)((ULONG64)Kthread + StartAddress_UP);
	PVOID Win32StartAddress = (PVOID)((ULONG64)Kthread + Win32StartAddress_UP);
	PVOID KernelStack = (PVOID)((ULONG64)Kthread + KernelStack_UP);
	PVOID CID = (PVOID)((ULONG64)Kthread + CID_UP);
	PVOID ExitStatus = (PVOID)((ULONG64)Kthread + ExitStatus_UP);
	PVOID _VCreateTime;
	PVOID _StartAddress;
	PVOID _Win32StartAddress;
	PVOID _KernelStack;
	PVOID _ExitStatus;
	PVOID _CID;



	PVOID mi_initialize_large_page_colour_sizes = (PVOID)(ntos_base + 0x3AC940);// (PVOID)0xFFFFFFFFFFFFFFFF;

	SpoofAddress<LARGE_INTEGER>(VCreateTime, &_VCreateTime, (PVOID)2147483247);

	SpoofAddress<void*>(StartAddress, &_StartAddress, mi_initialize_large_page_colour_sizes);

	SpoofAddress<void*>(Win32StartAddress, &_Win32StartAddress, mi_initialize_large_page_colour_sizes);

	SpoofAddress<LARGE_INTEGER>(KernelStack, &_KernelStack);

	SpoofAddress<CLIENT_ID>(CID, &_CID);

	SpoofAddress<LONG>(ExitStatus, &_ExitStatus);//fine






	//set apc for our thread

    mythread->MiscFlags &= ~(1UL << 10); // is system 
    uint32_t* miscFlags = (uint32_t*)((char*)mythread + 0x74);
    *miscFlags &= 0xffffbfff;

}


 //void spoof_thread_1()
 //{


	// PKTHREAD mythread = KeGetCurrentThread();
	// KeSetBasePriorityThread(mythread, 32);
	// //get ntos for making clean
	// auto module_name = skCrypt("ntoskrnl.exe");
	// uint64_t ntos_base = get_kernel_module(module_name);
	// if (!ntos_base)
	//	 return;
	// module_name.clear();
	// auto psp_cid_table_pattern = skCrypt("\x48\x8B\x05\xCC\xCC\xCC\xCC\x0F\xB6\xEA");
	// auto psp_mask = skCrypt("xxx????xxx");
	// auto ex_destroy_handle_pattern = skCrypt("\xE8\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x48\x8B\x8B\x18\x06\x00\x00");
	// auto ex_destroy_mask = skCrypt("x????x????xxxxxxx");
	// ULONG64* pPspCidTable = resolve(pattern_scan(ntos_base, psp_cid_table_pattern, psp_mask), 3, 4);
	// ULONG64* pExDestroyHandle = resolve(pattern_scan(ntos_base, ex_destroy_handle_pattern, ex_destroy_mask), 1, 4);

	// ex_destroy_handle_pattern.clear();
	// ex_destroy_mask.clear();

	// ExDestroyHandle = reinterpret_cast<func>(pExDestroyHandle);
	// DestroyPspCidTableEntry(pPspCidTable, PsGetCurrentThreadId());
	// LIST_ENTRY plist_ethread = *reinterpret_cast<LIST_ENTRY*>((ULONG64)mythread + 0x4e8);
	// PVOID Kthread = reinterpret_cast<PVOID>(KeGetCurrentThread());
	// wait(100000);





	// mythread->ThreadListEntry.Flink->Blink = mythread->ThreadListEntry.Blink;
	// mythread->ThreadListEntry.Blink->Flink = mythread->ThreadListEntry.Flink;
	// plist_ethread.Blink->Flink = plist_ethread.Flink;
	// plist_ethread.Flink->Blink = plist_ethread.Blink;



	// PETHREAD1 ethread = reinterpret_cast<PETHREAD1>(mythread);
	// ethread->KernelStackReference = 0;
	// //static ULONG stackReferenceOffset = 0;
	// //if (!stackReferenceOffset)
	//	 //stackReferenceOffset = *(ULONG*)((ULONG)PsDereferenceKernelStack + 0x10);

	// //*(PVOID*)(mythread + 0x28) = 0;
	// //*(ULONG*)(mythread + stackReferenceOffset) = 0;



	// //BOOLEAN is_system = PsIsSystemThread(mythread);

	// //DbgPrintEx(0, 0, "bubabiba is %d\n", is_system);
	// //PsIsThreadTerminating()

	// PVOID InitialStack = (PVOID)((ULONG64)Kthread + InitialStack_UP);
	// PVOID VCreateTime = (PVOID)((ULONG64)Kthread + VCreateTime_UP);
	// PVOID StartAddress = (PVOID)((ULONG64)Kthread + StartAddress_UP);
	// PVOID Win32StartAddress = (PVOID)((ULONG64)Kthread + Win32StartAddress_UP);
	// PVOID KernelStack = (PVOID)((ULONG64)Kthread + KernelStack_UP);
	// PVOID CID = (PVOID)((ULONG64)Kthread + CID_UP);
	// PVOID ExitStatus = (PVOID)((ULONG64)Kthread + ExitStatus_UP);
	// PVOID _VCreateTime;
	// PVOID _StartAddress;
	// PVOID _Win32StartAddress;
	// PVOID _KernelStack;
	// PVOID _ExitStatus;
	// PVOID _CID;



	// PVOID mi_initialize_large_page_colour_sizes = (PVOID)(ntos_base + 0x3AC940);// (PVOID)0xFFFFFFFFFFFFFFFF;

	// SpoofAddress<LARGE_INTEGER>(VCreateTime, &_VCreateTime, (PVOID)2147483247);

	// SpoofAddress<void*>(StartAddress, &_StartAddress, mi_initialize_large_page_colour_sizes);

	// SpoofAddress<void*>(Win32StartAddress, &_Win32StartAddress, mi_initialize_large_page_colour_sizes);

	// SpoofAddress<LARGE_INTEGER>(KernelStack, &_KernelStack);

	// SpoofAddress<CLIENT_ID>(CID, &_CID);

	// SpoofAddress<LONG>(ExitStatus, &_ExitStatus);//fine






	// //set apc for our thread

	// mythread->MiscFlags &= ~(1UL << 10); // is system 
	// uint32_t* miscFlags = (uint32_t*)((char*)mythread + 0x74);
	// *miscFlags &= 0xffffbfff;

 //}
 //void thread_1()
 //{

	// spoof_thread_1();
	// auto umid = skCrypt(L"\\SystemRoot\\Usermode");
	// INT UMID = ReadVarFromFile(umid, 10);
	// PsLookupProcessByProcessId((HANDLE)UMID, &SETTINGS::PeUM);
	// auto packet_string = skCrypt(L"\\SystemRoot\\Packet_1");
	// SETTINGS::PacketAddress1 = ReadVarFromFile(packet_string, 16);
	// umid.clear();
	// packet_string.clear();
	// KAPC_STATE State;
	// KeStackAttachProcess(SETTINGS::PeUM, &State);
	// {
	//	 auto PacketPhysicalAddress = MmGetPhysicalAddress((PVOID)SETTINGS::PacketAddress1);
	//	 if (PacketPhysicalAddress.QuadPart)
	//	 {
	//		 Packet_1 = (_Packet*)MmMapIoSpace(PacketPhysicalAddress, sizeof(_Packet), MmNonCached);
	//	 }
	// }
	// KeUnstackDetachProcess(&State);






	// //wait(5000);
	// //DbgPrintEx(0,0,"Packet is %llx\n", Packet);
	// if (Packet_1)
	// {
	//	 Packet_1->Type = _Type::Complete;

	//	 while (true)
	//	 {
	//		 if (Packet_1->Type != _Type::Complete)
	//		 {
	//			 switch (Packet_1->Type)
	//			 {
	//			 case _Type::Init:
	//				 thr_1::InitHandler();
	//				 break;
	//			 case _Type::Base:
	//				 thr_1::BaseHandler();
	//				 break;
	//			 case _Type::Read:
	//				 thr_1::ReadHandler();
	//				 break;
	//			 case _Type::Write:
	//				 thr_1::WriteHandler();
	//				 break;
	//			 case _Type::Unmap:
	//				 goto Exit;
	//				 break;
	//			 }
	//			 Packet_1->Type = _Type::Complete;
	//		 }
	//		 //do_nothing();
	//		 wait(1);
	//	 }
	// }
 //Exit:
	// UnmapHandler();
	// while (true)
	//	 wait(1000000);
 //}
void thread_0()
{


	spoof_thread();
	auto umid = skCrypt(L"\\SystemRoot\\Usermode");
	INT UMID = ReadVarFromFile(umid, 10);
	PsLookupProcessByProcessId((HANDLE)UMID, &SETTINGS::PeUM);
	auto packet_string = skCrypt(L"\\SystemRoot\\Packet_0");
	SETTINGS::PacketAddress = ReadVarFromFile(packet_string, 16);
	umid.clear();
	packet_string.clear();
	KAPC_STATE State;
	KeStackAttachProcess(SETTINGS::PeUM, &State);
	{
		auto PacketPhysicalAddress = MmGetPhysicalAddress((PVOID)SETTINGS::PacketAddress);
		if (PacketPhysicalAddress.QuadPart)
		{
			Packet = (_Packet*)MmMapIoSpace(PacketPhysicalAddress, sizeof(_Packet), MmNonCached);
		}
	}
	KeUnstackDetachProcess(&State);

		

	

	//wait(5000);
	//DbgPrintEx(0,0,"Packet is %llx\n", Packet);
	if (Packet)
	{
		Packet->Type = _Type::Complete;

		while (true)
		{
			if (Packet->Type != _Type::Complete)
			{
				switch (Packet->Type)
				{
				case _Type::Init:
					InitHandler();
					break;
				case _Type::Base:
					BaseHandler();
					break;
				case _Type::Read:
					ReadHandler();
					break;
				case _Type::Write:
					WriteHandler();
					break;
				case _Type::Unmap:
					goto Exit;
					break;
				}
				Packet->Type = _Type::Complete;
			}
			//do_nothing();
			wait(1);
			if (!initialized)
				wait(100000);
		}
	}
Exit:
	UnmapHandler();
	while (true)
		wait(10000000);
}


NTSTATUS DriverEntry(PDRIVER_OBJECT Object, PUNICODE_STRING Path)
{

	UNREFERENCED_PARAMETER(Object);
	UNREFERENCED_PARAMETER(Path);



	HANDLE thread_handle = nullptr;
	OBJECT_ATTRIBUTES object_attribues{ };
	InitializeObjectAttributes(&object_attribues, nullptr, OBJ_KERNEL_HANDLE, nullptr, nullptr);
	NTSTATUS status = PsCreateSystemThread(&thread_handle, 0, &object_attribues, nullptr, nullptr, reinterpret_cast<PKSTART_ROUTINE>(&thread_0), nullptr);



	return STATUS_SUCCESS;
}