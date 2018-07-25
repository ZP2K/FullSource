//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGTestMTLResource.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-6-2 10:00
//  Comment     :   ��֤��ȡ�ͱ�������ļ���.MTL����������ȷ�Ժ��¾ɰ汾�ļ��Ķ�ȡ������
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGTestMTLResource_H__
#define _KGTestMTLResource_H__

#include "KGBaseResourceTestCase.h"
#include "KGTestCommon.h"

const char TEST_PROJECT_NAME[] = "KG3DMaterialTest";

class KGTestMTLResource
{
public:
	KGTestMTLResource();
	~KGTestMTLResource();
	int  Init(const char cszClientPath[]);	
	int  RunTest(const char cszTestPath[]);
	void UnInit();

private:
	int  LoadNewEngineDLL(const char cszClientPath[]);
	void TestOneMtlFile(const char cszFileName[]);
	void RefreshConsole();

private:
	HMODULE m_hEngineDLL;
	IKG3DEngineManager*		 m_pEngineMgr;
	IKG3DAutoTestInterface*  m_pTestInterface;
	char m_szApplicationPath[MAX_PATH];
	char m_szResultPath[MAX_PATH];
	vector<KGBaseResourceTestCase*> m_vecTestCase;
	int m_nTotalFileNum;
	int m_nMtlFileNum;
};
#endif