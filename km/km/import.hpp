#pragma once
#include "kli.hpp"

extern "C"
{
	PPEB NTAPI PsGetProcessPeb(PEPROCESS);

	PVOID NTAPI RtlFindExportedRoutineByName(PVOID, PCCH);

	NTSTATUS NTAPI ZwQuerySystemInformation(INT, PVOID, ULONG, PULONG);

	NTSTATUS NTAPI MmCopyVirtualMemory(PEPROCESS, PVOID, PEPROCESS, PVOID, SIZE_T, KPROCESSOR_MODE, PSIZE_T);

	NTSTATUS NTAPI ZwQueryInformationProcess(
		_In_      HANDLE           ProcessHandle,
		_In_      PROCESSINFOCLASS ProcessInformationClass,
		_Out_     PVOID            ProcessInformation,
		_In_      ULONG            ProcessInformationLength,
		_Out_opt_ PULONG           ReturnLength
	);
}


KLI_CACHED_DEF(PsCreateSystemThread);


void init_import()
{

	KLI_CACHED_SET(PsCreateSystemThread);

}