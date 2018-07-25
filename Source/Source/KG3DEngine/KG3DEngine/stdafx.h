////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : stdafx.h
//  Version     : 
//  Creator     : 
//  Create Date : 
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_STDAFX_H_
#define _INCLUDE_STDAFX_H_

////////////////////////////////////////////////////////////////////////////////
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//


#include "KGStdAfx.h"

//<defs>
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define STRSAFE_NO_DEPRECATE
#define NOMINMAX
#define  _KG3D_ENGINE_USE_DEBUG_NEW 1	//���Ҫ����Debug������ڴ�й¶�����ƣ���ô�������Ϊ0�����Ǻ���û��ʲô��Ҫ����
//</defs>

//<macros>
#ifndef SAFE_RENDER
#define SAFE_RENDER(p, u, w)    { if (p) { p->Render(u, w); } }
#endif // #ifndef SAFE_RENDER

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif // #ifndef SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif // #ifndef SAFE_DELETE_ARRAY

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif // #ifndef SAFE_RELEASE


#if 0
#ifdef SAFE_RELEASE
#undef SAFE_RELEASE
#endif // #ifndef SAFE_RELEASE

template<typename T>
inline ULONG safe_release(T& p, LPCSTR szFile, size_t nLine) 
{
	if (p) 
	{ 
		ULONG ul = p->Release(); 
		if (ul != 0)
			ATLTRACE("SAFE_RELEASE(%x) %u %s %d\n", p, ul, szFile, nLine);
		p = NULL;
		return ul;
	}
	else
	{
		return 0;
	}
}

#define SAFE_RELEASE(p) safe_release(p, __FILE__, __LINE__)
#endif

#ifdef VERIFY_RELEASE
#pragma message("VERIFY_RELEASE is already defined")
#undef VERIFY_RELEASE
#endif // VERIFY_RELEASE
#if defined(DEBUG) | defined(_DEBUG)
#define VERIFY_RELEASE(p) { if (p) { VERIFY((p)->Release() == 0); (p)=NULL; } }
#else
#define VERIFY_RELEASE SAFE_RELEASE
#endif
//</macros>

//<pragma warning>
#pragma warning(disable:4100)	//û��ʹ�õĲ���
//#pragma warning(disable:4101)	//û��ʹ�õľֲ�����
//#pragma warning(disable:4189)	//��ʼ����û��ʹ�õľֲ�����
#pragma warning(disable:4127)	//������ת�ǳ�������while(1){}
#pragma warning(disable:4201)	//�Ǳ�׼��չ������struct/unin����struct {int a,b,c};
#pragma warning(disable:4238)	//�Ǳ�׼��չ����ֵ����ֵ�á���C * pC = &C(); 
#pragma warning(disable:4290)	//�쳣���⻯ָ��
//#pragma warning(disable:4702)	//�޷������ָ��
#pragma warning(error:4701)		//ʹ�þֲ�������ȴû�г�ʼ��
#pragma warning(disable:4819)	//�ڱ�ϵͳ��ҳ�޷������ļ��е�ĳЩ�ַ�
//#pragma warning(disable:4505) //û���õ��ľֲ������ᱻɾ��
//</pragma warning>

//<base library include>
#include <windows.h>
#include "Ksdef.h"
#include "KGPublic.h"
#include "KGLog.h"
#include "Engine.h"
#include "Engine/KG_InterlockedVariable.h"
#include "Engine/KMutex.h"
#include "Engine/KMemory.h"
#include "Common/KG_Memory.h"
//<base libray include>

//<system libray include>
#include <mmsystem.h>
#include <shlwapi.h>

#include <tchar.h>
#include <strsafe.h>

#include <list>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <bitset>
#include <map>
#include <hash_map>
#include <algorithm>
#include <functional>
#include <limits>
#include <new>
#include <iostream>
#include <sstream>

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>

#include <atltrace.h>
#include <atlbase.h>
//</system libray include>

//<d3d include>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>
#include <d3dx9math.h>
#include <ddraw.h>
#include <d3d9caps.h>
//</d3d include>

//<third part include>
#include "NxPhysics.h"
//</third part include>

//<memory checking>
/*���淲����_KG3D_ENGINE_USE_DEBUG_NEW�Ķ����ڴ�й¶����������Ҫ�ģ�
��Ҫ��������֮���˳��Ӧ��_CRTDBG_MAP_ALLOC�ȶ��壬Ȼ�����
#include <crtdbg.h>
#include <dbghelp.h>
Ȼ����DEBUG_NEW*/
#if defined(DEBUG) | defined(_DEBUG)
#if _KG3D_ENGINE_USE_DEBUG_NEW == 1

#define _CRTDBG_MAP_ALLOC	//��΢���ڴ������е��ڴ������ include Microsoft memory leak detection procedures
//#define _INC_MALLOC	     //����鲻�� exclude standard memory alloc procedures
#endif

#include <crtdbg.h>
#include <dbghelp.h>

#endif

#define _ConvertBindToLower
#ifndef _NORMAL_BLOCK
#define _NORMAL_BLOCK 1
#endif

//����new������תΪDebugNew�İ汾

#ifndef DEBUG_NEW_3DENGINE
#define DEBUG_NEW_3DENGINE new
#endif

#if defined(DEBUG) | defined(_DEBUG)
#if _KG3D_ENGINE_USE_DEBUG_NEW == 1
    void* __cdecl operator new(size_t nSize, const char* lpszFileName, int nLine);
    void* __cdecl operator new[](size_t nSize, int nType, LPCSTR lpszFileName, int nLine);

    #ifdef DEBUG_NEW_3DENGINE
        #undef DEBUG_NEW_3DENGINE
        #define DEBUG_NEW_3DENGINE new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
    #endif
#endif
#endif

//</memory check>

//<engine helpers>
#include "KGTester.h"
#include "KGMathFunctions.h"
#include "KGCommonWidgets.h"
#include "KGHelperClasses.h"
#include "KGCommonBuffer.h"
#include "KGDesignPatternWidgets.h"
#include "KG3DGraphicHelpers.h"
#include "KGBitSet.h"
#include "KG3DLifeTimeDependedPointer.h"
#include "KGStringFunctions.h"
#include "mathtool.h"
#include "KOutputDebug.h"
#include "KGSTLWrappers.h"
//</engine helpers>

//<engine global includes>
#include "KG3DTypes.h"
#include "KG3DEngineTypes.h"
//</engine global includes>

#include "KG3DResourcePacker.h" // it redirect g_OpenFile(), must after engine/file.h

//<engine global variables>
extern LPDIRECT3DDEVICE9 g_pd3dDevice;
//</engine global variables>

#endif //_INCLUDE_STDAFX_H_
