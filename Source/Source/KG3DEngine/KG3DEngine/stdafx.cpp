// stdafx.cpp : source file that includes just the standard includes
// S3TEngine.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> 

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

//(by dengzhihui 2006��9��18�� 17:44:41
//�ڴ�й©���԰���,��������_heap_alloc_dbg�����ϵ�
#if defined(DEBUG) | defined(_DEBUG)
#if _KG3D_ENGINE_USE_DEBUG_NEW == 1
void* __cdecl operator new(size_t nSize, const char* lpszFileName, int nLine)
{
	return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}

void* __cdecl operator new[](size_t nSize, int nType, LPCSTR lpszFileName, int nLine)
{
	return ::operator new(nSize, nType, lpszFileName, nLine);
}

#ifndef DEBUG_NEW_3DENGINE
#define DEBUG_NEW_3DENGINE new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif

#endif
#endif
//)