// GameConfig.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// KGameConfigApp:
// �йش����ʵ�֣������ GameConfig.cpp
//

class KGameConfigApp : public CWinApp
{
public:
	KGameConfigApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
private:
	int CanRun();

	int _CheckSystemMemory();
	int _GetCurrentVersionPath(TCHAR szVersionPath[], TCHAR* szRetVersinPath, unsigned uBuffSize);
	void _StringTrim(TCHAR szString[]);

	DECLARE_MESSAGE_MAP()
};

extern KGameConfigApp theApp;