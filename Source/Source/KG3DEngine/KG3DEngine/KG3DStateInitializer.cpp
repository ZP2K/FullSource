////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DStateInitializer.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-3-6 11:01:20
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "KG3DStateInitializer.h"
#include "KG3DScene.h"


#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bClient;

VOID KG3DStateInitializer::InitScene( KG3DScene& Scene )
{
	//Ĭ�Ͼʹ�����SceneSelectionTool,�ƶ�����ת�ͷ�������
	KG3DCommonObjectNode& Node = Scene.GetCommonObjectNode();

	if (! g_bClient)
	{
		DWORD PreCreateNodeTypes[] =
		{
			KG3DTYPE_SCENE_SELECTION_TOOL,
			KG3DTYPE_TRANSLATION_TOOL,
			KG3DTYPE_ROTATION_TOOL,
			KG3DTYPE_SCALING_TOOL,
		};

		for (size_t i = 0; i < _countof(PreCreateNodeTypes); ++i)
		{
			Node.Request(KG3DCOBJ_HANDLE(PreCreateNodeTypes[i], 0), NULL
				, reinterpret_cast<DWORD_PTR>(&Scene), NULL, NULL);
		}		
	}
	else	//�ͻ��˲���Ҫ�ƶ��ȹ���
	{
		DWORD PreCreateNodeTypes[] =
		{
			KG3DTYPE_SCENE_SELECTION_TOOL,
		};

		for (size_t i = 0; i < _countof(PreCreateNodeTypes); ++i)
		{
			Node.Request(KG3DCOBJ_HANDLE(PreCreateNodeTypes[i], 0), NULL
				, reinterpret_cast<DWORD_PTR>(&Scene), NULL, NULL);
		}
	}
}
