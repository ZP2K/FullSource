////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEngineInterfaceGetter.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-11-18 10:30:07
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DEngineInterfaceGetter.h"

#include "KG3DTypes.h"
#include "KG3DRegionInfoManager.h"
#include "KG3DScene.h"
#include "KG3DSceneCheckers.h"
#include "KG3DCollocator.h"
#include "KG3DEngineManager.h"
#include "KG3DTime.h"
////////////////////////////////////////////////////////////////////////////////


HRESULT g_GetEngineIEInterface( KG3DEngineManager& EM, DWORD dwType1, DWORD dwType2, DWORD_PTR wParam, DWORD_PTR lParam, LPVOID* ppInterface )
{
	///�ڴ���ָ��֮ǰ���ǵ���һ����̬������ת�ͣ�ת��IE�ӿڣ�
	///������Ϊû�б�֤IE�ӿ�һ������ĵ�һ��public�����Կ���ǿת�����Ĳ�����ȷ��
	LPVOID lpInterface = NULL;
	KG_PROCESS_ERROR(ppInterface);
	{
		if (dwType1 == KG3DTYPE_RESOURCE_TABLE_TYPE)
		{
			if (dwType2 == KG3DTYPE_REGION_INFO_MANAGER)
			{
				IEKG3DRegionInfoManagerTable* pIETable = &g_cRegionInfoManagerTable;///��Ҫ�����ӣ���Ȼ���
				lpInterface = (LPVOID)pIETable;
				goto Exit1;
			}
		}
		IEKG3DScene* pScene = g_cGraphicsTool.GetScene();///��������Ժ�ǿ����SceneEditor�Ĺ��ܴ󲿷��ǻ�������
		KG_PROCESS_ERROR(pScene);

		switch(dwType2)
		{
		case KG3DTYPE_COLLOCATOR:
			{
				lpInterface = static_cast<LPVOID>(KG3DCollocator::GetAnotherInterface());
			}
			break;
		case KG3DTYPE_SCENE:
			{
				lpInterface = (LPVOID)pScene;
			}
			break;
		case KG3DTYPE_SCENE_CHECKERS:
			{
				lpInterface = g_GetSceneCheckers().ToAnotherInterface(NULL);
			}
			break;
		case KG3DTYPE_SCENE_SCENE_EDITOR:
			{
				IEKG3DSceneSceneEditor* pSE = NULL;
				KG_COM_PROCESS_ERROR(pScene->GetSceneSceneEditor(&pSE));
				_ASSERTE(pSE);
				lpInterface = (LPVOID)pSE;
			}
			break;
		default:
			{
				if (dwType1 == 0)
				{
					return pScene->GetEngineIEInterface(dwType2, wParam, lParam, ppInterface);
				}
				else
				{
					return pScene->GetEngineIEAbstructInterface(dwType1, dwType2, wParam, lParam, ppInterface);
				}
			}
		    break;
		}		
Exit1:
		_ASSERTE(NULL != lpInterface);
		*ppInterface = lpInterface;
		return S_OK;
	}
Exit0:
	return E_FAIL;	
}

HRESULT g_GetEngineInterface( KG3DEngineManager& Em, DWORD dwType, DWORD_PTR p1, DWORD_PTR p2, LPVOID* ppInterface )
{
	LPVOID lpInterface = NULL;
	KG_PROCESS_ERROR(NULL != ppInterface);

	switch(dwType)
	{
	case KG3DTYPE_TIME_CLOCK:
		{
			lpInterface = Em.GetTimeClock().ToAnotherInterface(KG3DTYPE_I_INTERFACE);
		}		
		break;
	default:
	    break;
	}
	KG_PROCESS_ERROR(NULL != lpInterface);
//Exit1:
	*ppInterface = lpInterface;
	return S_OK;
Exit0:
	return E_FAIL;
}