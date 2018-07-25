// LuaRuntime.cpp : ���� DLL �ĳ�ʼ�����̡�
//
#include "stdafx.h"
#include "LuaRuntime.h"
#include "LuaPlus.h"
using namespace LuaPlus;

LuaStateOwner lua(true); 
BOOL __stdcall LuaInit(const char * szLua, char * * szOutput)
{
	try
	{
		if (lua->DoString(szLua) != 0)
		{
			* szOutput = (char *)lua->Stack(-1).GetString();
			return FALSE;
		}
	}
	catch (LuaException* e)
	{
		* szOutput = (char *)e->GetErrorMessage();
		return FALSE;
	}
	return TRUE;
}

BOOL __stdcall LuaRun(char * szFuncName, char * szCode, int nPos, int nIsDelete, char * szSetting, char * * szOutput)
{
	LuaObject luaObj = lua->GetGlobal(szFuncName);
	if (!luaObj.IsFunction())
	{
		* szOutput = "�Ҳ���ָ������";
		return FALSE;
	}
	try
	{
		lua->DoString("byrunrime_szCode='' byruntime_nPos=0 byruntime_nIsDelete=0 byruntime_szSetting=''");
		lua->GetGlobals().Set("byrunrime_szCode", szCode);
		lua->GetGlobals().Set("byruntime_nPos", nPos);
		lua->GetGlobals().Set("byruntime_nIsDelete", nIsDelete);
		lua->GetGlobals().Set("byruntime_szSetting", szSetting);
		char acbuffer[256] = {0};
		char * pcbuffer = acbuffer;
		strcpy(acbuffer, "byruntime_szRet =");
		pcbuffer += strlen(pcbuffer);
		strcpy(pcbuffer, szFuncName);
		pcbuffer += strlen(pcbuffer);
		strcpy(pcbuffer, "(byrunrime_szCode, byruntime_nPos, byruntime_nIsDelete, byruntime_szSetting)");
		if(lua->DoString(acbuffer) != 0)
		{
			* szOutput = (char *)lua->Stack(-1).GetString();
			return FALSE;
		}
		LuaObject loRet = lua->GetGlobal("byruntime_szRet");
		if(!loRet.IsString())
		{
			* szOutput = "��������ֵ����Ϊ�ַ���";
			return FALSE;
		}
		* szOutput = (char *)loRet.GetString();
	}
	catch (LuaException* e)
	{
		* szOutput = (char *)e->GetErrorMessage();
		return FALSE;
	}
	return TRUE;
}


// CLuaRuntimeApp

BEGIN_MESSAGE_MAP(CLuaRuntimeApp, CWinApp)
END_MESSAGE_MAP()


// CLuaRuntimeApp ����

CLuaRuntimeApp::CLuaRuntimeApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLuaRuntimeApp ����

CLuaRuntimeApp theApp;


// CLuaRuntimeApp ��ʼ��

BOOL CLuaRuntimeApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
