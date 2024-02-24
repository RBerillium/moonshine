
#pragma once
#include <intrin.h>


void CopyList(IN PLIST_ENTRY Original,
	IN PLIST_ENTRY Copy,
	IN KPROCESSOR_MODE Mode)
{
	if (IsListEmpty(&Original[Mode]))
	{
		InitializeListHead(&Copy[Mode]);
	}
	else
	{
		Copy[Mode].Flink = Original[Mode].Flink;
		Copy[Mode].Blink = Original[Mode].Blink;
		Original[Mode].Flink->Blink = &Copy[Mode];
		Original[Mode].Blink->Flink = &Copy[Mode];
	}
}

void
MoveApcState(PKAPC_STATE OldState,
	PKAPC_STATE NewState)
{
	RtlCopyMemory(NewState, OldState, sizeof(KAPC_STATE));

	CopyList(OldState->ApcListHead, NewState->ApcListHead, KernelMode);
	CopyList(OldState->ApcListHead, NewState->ApcListHead, UserMode);
}

uintptr_t OldProcess;
void AttachProcess(PEPROCESS NewProcess)
{
	PKTHREAD Thread = KeGetCurrentThread();

	PKAPC_STATE ApcState = *(PKAPC_STATE*)(uintptr_t(Thread) + 0x98); // 0x98 = _KTHREAD::ApcState

	if (*(PEPROCESS*)(uintptr_t(ApcState) + 0x20) == NewProcess) // 0x20 = _KAPC_STATE::Process
		return;

	if ((*(UCHAR*)(uintptr_t(Thread) + 0x24a) != 0)) // 0x24a = _KTHREAD::ApcStateIndex
	{
		KeBugCheck(INVALID_PROCESS_ATTACH_ATTEMPT);
		return;
	}

	MoveApcState(ApcState, *(PKAPC_STATE*)(uintptr_t(Thread) + 0x258)); // 0x258 = _KTHREAD::SavedApcState

	InitializeListHead(&ApcState->ApcListHead[KernelMode]);
	InitializeListHead(&ApcState->ApcListHead[UserMode]);

	OldProcess = *(uintptr_t*)(uintptr_t(ApcState) + 0x20);

	*(PEPROCESS*)(uintptr_t(ApcState) + 0x20) = NewProcess; // 0x20 = _KAPC_STATE::Process
	*(UCHAR*)(uintptr_t(ApcState) + 0x28) = 0;				// 0x28 = _KAPC_STATE::InProgressFlags
	*(UCHAR*)(uintptr_t(ApcState) + 0x29) = 0;				// 0x29 = _KAPC_STATE::KernelApcPending
	*(UCHAR*)(uintptr_t(ApcState) + 0x2a) = 0;				// 0x2a = _KAPC_STATE::UserApcPendingAll

	*(UCHAR*)(uintptr_t(Thread) + 0x24a) = 1; // 0x24a = _KTHREAD::ApcStateIndex

	auto DirectoryTableBase = *(uint64_t*)(uint64_t(NewProcess) + 0x28);  // 0x28 = _EPROCESS::DirectoryTableBase
	__writecr3(DirectoryTableBase);
}

void DetachProcess()
{
	PKTHREAD Thread = KeGetCurrentThread();
	PKAPC_STATE ApcState = *(PKAPC_STATE*)(uintptr_t(Thread) + 0x98); // 0x98 = _KTHREAD->ApcState

	if ((*(UCHAR*)(uintptr_t(Thread) + 0x24a) == 0)) // 0x24a = KTHREAD->ApcStateIndex
		return;

	if ((*(UCHAR*)(uintptr_t(ApcState) + 0x28)) ||  // 0x28 = _KAPC_STATE->InProgressFlags
		!(IsListEmpty(&ApcState->ApcListHead[KernelMode])) ||
		!(IsListEmpty(&ApcState->ApcListHead[UserMode])))
	{
		KeBugCheck(INVALID_PROCESS_DETACH_ATTEMPT);
	}

	MoveApcState(*(PKAPC_STATE*)(uintptr_t(Thread) + 0x258), ApcState); // 0x258 = _KTHREAD::SavedApcState

	if (OldProcess)
		*(uintptr_t*)(uintptr_t(ApcState) + 0x20) = OldProcess; // 0x20 = _KAPC_STATE::Process

	*(PEPROCESS*)(*(uintptr_t*)(uintptr_t(Thread) + 0x258) + 0x20) = 0; // 0x258 = _KTHREAD::SavedApcState + 0x20 = _KAPC_STATE::Process

	*(UCHAR*)(uintptr_t(Thread) + 0x24a) = 0; // 0x24a = _KTHREAD::ApcStateIndex

	auto DirectoryTableBase = *(uint64_t*)(uint64_t(*(PEPROCESS*)(uintptr_t(ApcState) + 0x20)) + 0x28); // 0x20 = _KAPC_STATE::Process + 0x28 = _EPROCESS::DirectoryTableBase
	__writecr3(DirectoryTableBase);

	if (!(IsListEmpty(&ApcState->ApcListHead[KernelMode])))
	{
		*(UCHAR*)(uint64_t(ApcState) + 0x29) = 1; // 0x29 = _KAPC_STATE::KernelApcPending
	}

	RemoveEntryList(&ApcState->ApcListHead[KernelMode]);

	OldProcess = 0;
}

PHYSICAL_ADDRESS
SafeMmGetPhysicalAddress(PVOID BaseAddress)
{
	static BOOLEAN* KdEnteredDebugger = 0;
	if (!KdEnteredDebugger)
	{
		UNICODE_STRING UniCodeFunctionName = RTL_CONSTANT_STRING(L"KdEnteredDebugger");
		KdEnteredDebugger = reinterpret_cast<BOOLEAN*>(MmGetSystemRoutineAddress(&UniCodeFunctionName));
	}

	*KdEnteredDebugger = TRUE;
	PHYSICAL_ADDRESS PhysicalAddress = MmGetPhysicalAddress(BaseAddress);
	*KdEnteredDebugger = FALSE;

	return PhysicalAddress;
}

NTSTATUS ReadVirtualMemory(
	PEPROCESS Process,
	PVOID Destination,
	PVOID Source,
	SIZE_T Size)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	PHYSICAL_ADDRESS SourcePhysicalAddress;
	PVOID MappedIoSpace;
	BOOLEAN IsAttached;

	AttachProcess(Process);
	IsAttached = TRUE;

	if (!MmIsAddressValid(Source))
		goto _Exit;

	SourcePhysicalAddress = SafeMmGetPhysicalAddress(Source);

	DetachProcess();
	IsAttached = FALSE;

	if (!SourcePhysicalAddress.QuadPart)
		return ntStatus;

	MappedIoSpace = MmMapIoSpaceEx(SourcePhysicalAddress, Size, PAGE_READWRITE);
	if (!MappedIoSpace)
		goto _Exit;

	memcpy(Destination, MappedIoSpace, Size);

	MmUnmapIoSpace(MappedIoSpace, Size);

	ntStatus = STATUS_SUCCESS;

_Exit:

	if (IsAttached)
		DetachProcess();

	return ntStatus;
}

NTSTATUS WriteVirtualMemory(
	PEPROCESS Process,
	PVOID Destination,
	PVOID Source,
	SIZE_T Size)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	PHYSICAL_ADDRESS SourcePhysicalAddress;
	PVOID MappedIoSpace;
	BOOLEAN IsAttached;

	AttachProcess(Process);
	IsAttached = TRUE;

	if (!MmIsAddressValid(Source))
		goto _Exit;

	SourcePhysicalAddress = SafeMmGetPhysicalAddress(Source);

	DetachProcess();
	IsAttached = FALSE;

	if (!SourcePhysicalAddress.QuadPart)
		return ntStatus;

	MappedIoSpace = MmMapIoSpaceEx(SourcePhysicalAddress, Size, PAGE_READWRITE);
	if (!MappedIoSpace)
		goto _Exit;

	memcpy(MappedIoSpace, Destination, Size);

	MmUnmapIoSpace(MappedIoSpace, Size);

	ntStatus = STATUS_SUCCESS;

_Exit:

	if (IsAttached)
		DetachProcess();
}

NTSTATUS ReadProcessMemory(PEPROCESS Process, PVOID Address, PVOID Buffer, SIZE_T Size)
{
	
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	if (Process)
	{
		status = ReadVirtualMemory(Process, Buffer, Address, Size);
	}

	//ObDereferenceObject(Process);
	return status;
}

NTSTATUS WriteProcessMemory(HANDLE ProcessPid, PVOID Address, PVOID Buffer, SIZE_T Size)
{
	PEPROCESS Process = { 0 };
	auto ntStatus = PsLookupProcessByProcessId(ProcessPid, &Process);
	if (NT_SUCCESS(ntStatus) && Process)
	{
		ntStatus = WriteVirtualMemory(Process, Buffer, Address, Size);
	}

	ObDereferenceObject(Process);
	return ntStatus;
}

PVOID GetModuleBaseProcess(
	HANDLE ProcessId,
	LPCWSTR ModuleName
)
{
	PVOID mBase = 0;
	PEPROCESS Process = { 0 };

	UNICODE_STRING module_name = RTL_CONSTANT_STRING(ModuleName);
	if (ProcessId && NT_SUCCESS(PsLookupProcessByProcessId(HANDLE(ProcessId), &Process)) && Process)
	{
		PPEB pPeb = PsGetProcessPeb(Process);

		AttachProcess(Process);

		for (PLIST_ENTRY pListEntry = pPeb->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPeb->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
		{
			PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

			if (RtlEqualUnicodeString(&pEntry->BaseDllName, &module_name, TRUE) == 0) {
				mBase = pEntry->DllBase;
				break;
			}
		}

		DetachProcess();
	}

	return mBase;
}