// ProfileGraph.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CProfileGraphApp:
// �йش����ʵ�֣������ ProfileGraph.cpp
//

class CProfileGraphApp : public CWinApp
{
public:
	CProfileGraphApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProfileGraphApp theApp;
