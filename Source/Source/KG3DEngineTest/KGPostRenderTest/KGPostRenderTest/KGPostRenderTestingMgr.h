//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGPostRenderTestingMgr.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-7-28 10:00
//  Comment     :   1.�������������Ļ��Ч�������������������ļ��ء���ʼ����ִ�С����ٵĲ���
//					2.Ϊÿ�������ṩ��ͬ�Ĳ��Ի���
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGPostRenderTestingMgr_H_
#define _KGPostRenderTestingMgr_H_

#include "KGTestEffect.h"

const TCHAR TEST_PROJECT_NAME[] = "KGPostRenderTesting";

class KGPostRenderTestingMgr
{
public:
	KGPostRenderTestingMgr(void);
	~KGPostRenderTestingMgr(void);

public:
	int  Init(const TCHAR cszClientPath[], const HWND chWindow);
	void UnInit();
	int	 Run();	

private:
	int LoadEngineDLL();
	int LoadTestCaseFromFile();
	int MakeupEffectType(const TCHAR cszTypeName[], DWORD* dwTypeValue);

private:
	TCHAR m_szClientPath[MAX_PATH];
	TCHAR m_szResultPath[MAX_PATH];
	HMODULE m_hEngineDLL;
	IKG3DEngineManager* m_pEngineMgr;
	HWND m_hRenderWindow;
	KG3DEngineOption m_EngineOption;

	vector<int> m_vecEffectType;
	vector<int> m_vecTestFrame;
	vector<KGTestEffect*> m_vecTestCases;
	int m_nFrame;
	bool m_bKMemoryInited;
};

#endif