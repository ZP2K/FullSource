// LuaRuntime.h : LuaRuntime DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif


#include "resource.h"		// ������


// CLuaRuntimeApp
// �йش���ʵ�ֵ���Ϣ������� LuaRuntime.cpp
//

class CLuaRuntimeApp : public CWinApp
{
public:
	CLuaRuntimeApp();


// ��д
public:

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
