#include "stdafx.h"
#include "CheckGameCheat.h"
#include "md5.h"


#define SS_MAP std::map<TSTRING, TSTRING>
#define SS_MAP_ITERATOR std::map<TSTRING, TSTRING>::iterator
#define SS_PAIR std::pair<TSTRING, TSTRING>

#define MAX_MSG_LEN 512

//��ʼ������ģ��ĺ�����
BOOL InitBlackNameListMap(SS_MAP &mapBlackNameList);

//����ʼ������ģ��ĺ�����
BOOL UnInitBlackNameListMap(SS_MAP &mapBlackNameList);

//���ָ��ģ���Ƿ��������ģ�����ƣ���md5����ȣ�
BOOL IsModuleInBlackNameList(LPMODULEENTRY32 lpme, SS_MAP mapBlackNameList);

//��鵱ǰ�����е�ģ���Ƿ������������
BOOL HasOneModuleInBlackNameList(SS_MAP mapBlackNameList, LPMODULEENTRY32 lpme);

//��ȡģ���md5
TSTRING GetModuleMD5(LPMODULEENTRY32 lpme);

//�Ƿ���ڵ���ľ��
BOOL ExistMaliciousModule();

//��⵽���ڵ���ľ���Ĵ�������ұ���ľ��ģ���ļ�·����
BOOL ProcessTorjanModule(LPMODULEENTRY32 lpmeTorjan);



BOOL ExistMaliciousModule()
{
	BOOL bExist = FALSE;

	SS_MAP mapBlackNameList;

	MODULEENTRY32 me32 = {0};

	InitBlackNameListMap(mapBlackNameList);

	bExist = HasOneModuleInBlackNameList(mapBlackNameList, &me32);

	if (bExist)
	{
		ProcessTorjanModule(&me32);
	}

	UnInitBlackNameListMap(mapBlackNameList);

	return bExist;
}

BOOL HasOneModuleInBlackNameList(SS_MAP mapBlackNameList, LPMODULEENTRY32 lpme)
{
	BOOL bHas = FALSE;

	DWORD         PID    = GetCurrentProcessId();
	DWORD		  dwSize = 0;
	BOOL		  isMod  = 0;

	HANDLE hSnapshotModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	if (hSnapshotModule) 
	{
		lpme->dwSize = sizeof(MODULEENTRY32);
		isMod = Module32First(hSnapshotModule, lpme);
		while (isMod) 
		{
			BOOL bIsModuleInBlackNameList = FALSE;
			bIsModuleInBlackNameList = IsModuleInBlackNameList(lpme, mapBlackNameList);
			if (bIsModuleInBlackNameList)
			{
				bHas = TRUE;
				break;
			}
			isMod = Module32Next(hSnapshotModule, lpme);
		}
	}
	CloseHandle(hSnapshotModule);

	return bHas;
}

BOOL IsModuleInBlackNameList(LPMODULEENTRY32 lpme, SS_MAP mapBlackNameList)
{
	BOOL bIsIn = FALSE;

	SS_MAP_ITERATOR itr;

	TSTRING strModeName = TSTRING(lpme->szModule);
	itr = mapBlackNameList.find(strModeName);

	if (itr != mapBlackNameList.end())
	{
		TSTRING modmd5 = GetModuleMD5(lpme);

		if (modmd5 == itr->second)
		{
			bIsIn = TRUE;
		}
	}

	return bIsIn;
}

BOOL InitBlackNameListMap(SS_MAP &mapBlackNameList)
{
	BOOL bRet = TRUE;
	mapBlackNameList.clear();
	mapBlackNameList.insert(SS_PAIR("midimap.dll.v","354add79bc27506cfa5385bb43986831"));
	mapBlackNameList.insert(SS_PAIR("midimap.dll","354add79bc27506cfa5385bb43986831"));

	return bRet;
}

BOOL UnInitBlackNameListMap(SS_MAP &mapBlackNameList)
{
	BOOL bRet = TRUE;
	mapBlackNameList.clear();
	return bRet;
}

TSTRING GetModuleMD5(LPMODULEENTRY32 lpme)
{
	BYTE* modBegin = lpme->modBaseAddr;
	
	TSTRING modmd5 = MD5(modBegin + 0x80, 0x80).toString();

	return modmd5;
}

BOOL ProcessTorjanModule(LPMODULEENTRY32 lpmeTorjan)
{
	BOOL bRet = TRUE;

	CHAR szReportText[MAX_MSG_LEN];
	sprintf(szReportText, "%s�����ǵ��ų��򣬽���ɾ��֮�ٵ�¼��Ϸ��", lpmeTorjan->szExePath);
	::MessageBox(NULL, szReportText, "��Ҫ��ʾ", MB_ICONWARNING);

	return bRet;
}
