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

#ifndef _KGTestCommon_H__
#define _KGTestCommon_H__

#include "stdafx.h"

#ifndef EPSILON
#define EPSILON 0.00001f  // �����жϸ������Ƿ����0
#endif

#ifndef INV_SIGN_BIT
#define INV_SIGN_BIT 0x7FFFFFFF  // ��λ
#endif

#ifdef _DEBUG
const char ENGINE_DLL_NAME[] = "KG3DEngine_AutoTestD.dll";
#else
const char ENGINE_DLL_NAME[] = "KG3DEngine_AutoTest.dll";
#endif

const char LOG_NAME[] = "Logs";
const int  MAX_LOG_LINE = MAX_PATH * 3;

extern char g_szApplicationPath[MAX_PATH];
extern char g_szLogPath[MAX_PATH];

namespace KGTestCommon
{
	string IntToString(const int nValue);
	float FastFabs(const float& pfIn);
	BOOL FloatEqual(float fNum1, float fNum2);
	BOOL D3DCOLORVALUEqual(D3DCOLORVALUE pCo1, D3DCOLORVALUE pCo2);
	BOOL D3DMATERIAL9Equal(D3DMATERIAL9 pMat1, D3DMATERIAL9 pMat2);

	//���Թ��̳�ʼ��
	int  TestingInit(const char cszProjectName[]);
	int  TestingUnit(const char cszProjectName[]);

	//��ʽ��Ŀ¼
	int  FormatPath(char* pszPath, const size_t cuSize);

	//����ͳһ�����Ŀ¼
	int  MakeResultPath(const char cszProjectName[], char* pszResultPath, const size_t cuSize);

	//������־�ļ��������µ���־�ļ�
	int  FindLogFile(const char cszLogPath[], char* pszLogName, const size_t cuSize);

	//��ȡ�Ӵ�
	//����GetSubStr("abcdefg", "a", "g", pszSub, cuSize); -> pszSub = "bcdef"
	int  GetSubStr(const char cszSource[],
				   const char cszStart[],
				   const char cszEnd[],
				   char* pszSub,
				   const size_t cuSize);

	//�������ʱ����Ϣ������̨����־
	int  OutInfo(const char cszInfo[]);

	//���Ŀ¼
	int  CleanPath(const char cszPath[]);

	//��ȡ��Χ�ڵ������
	int  GetRandom(int nMin, int nMax);
};
#endif