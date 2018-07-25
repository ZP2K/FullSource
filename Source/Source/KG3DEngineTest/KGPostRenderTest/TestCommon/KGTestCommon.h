//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGTestCommon.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-6-2 10:00
//  Comment     :   �����Ĳ��Ը�������
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGTestCommon_H_
#define _KGTestCommon_H_

#include "stdafx.h"

#ifdef _DEBUG
const TCHAR ENGINE_DLL_NAME[] = _T("KG3DEngine_AutoTestD.dll");
#else
const TCHAR ENGINE_DLL_NAME[] = _T("KG3DEngine_AutoTest.dll");
#endif

const TCHAR LOG_NAME[] = _T("Logs");
extern TCHAR g_szApplicationPath[MAX_PATH];
extern TCHAR g_szLogPath[MAX_PATH];

namespace KGTestCommon
{
	std::string IntToString(const int nValue);
	//���Թ��̳�ʼ��
	int  TestingInit(const TCHAR cszProjectName[]);
	int  TestingUnInit(const TCHAR cszProjectName[]);
	//��ʽ��Ŀ¼
	int  FormatPath(TCHAR* pszPath, const size_t cuSize);
	//����ͳһ�����Ŀ¼
	int  MakeResultPath(const TCHAR cszProjectName[], TCHAR* pszResultPath, const size_t cuSize);
	//������־�ļ��������µ���־�ļ�
	int  FindLogFile(const TCHAR cszLogPath[], TCHAR* pszLogName, const size_t cuSize);
};
#endif