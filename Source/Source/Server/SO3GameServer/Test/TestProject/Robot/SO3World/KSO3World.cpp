#include "stdafx.h"
#include <time.h>
#include "KSO3World.h"
#include "KMath.h"
#include "KAttrModifier.h"
//#include "KSkillManager.h"
#include "KPlayerClient.h"
#include "KObjectManager.h"
#include "KWorldSettings.h"
#include "KScriptCenter.h"
#include "KItemLib.h"
#include "KMapFile.h"
//#include "KShopCenter.h"
#include "KRegion.h"
#include "KScene.h"
#include "KDoodad.h"

#define CIRCLE_LIST_SIZE				256
#define CHECK_FACE_DIRECTION_INTERVAL	(GAME_FPS)	//����ͬ�������

KSO3World g_SO3World;

KSO3World::KSO3World(void)
{
}

KSO3World::~KSO3World(void)
{
}

BOOL KSO3World::Init(int nWorldIndex)
{
	BOOL bRetCode               = FALSE;

    m_nPreLoadScene = FALSE;

	//��ʼ����ѧ��
	printf("\tInitialize math library................");
	bRetCode = g_InitMath();
	KGLOG_PROCESS_ERROR(bRetCode);
	printf("[OK]\n");

	//��ȡ�����ļ�
	printf("\tLoad setting files");
	bRetCode = g_WorldSettings.Init();
	KGLOG_PROCESS_ERROR(bRetCode);

	//��ʼ���ű�����
	printf("\tInitialize script center\n");
	bRetCode = g_ScriptCenter.Init();
	KGLOG_PROCESS_ERROR(bRetCode);

	//��ʼ�����������
	bRetCode = g_ObjectManager.Init(nWorldIndex);
	KGLOG_PROCESS_ERROR(bRetCode);

	//��ʼ�����Դ�����
	bRetCode = InitAttributeFunctions();
	KGLOG_PROCESS_ERROR(bRetCode);

//	bRetCode = g_SkillManager.Init();
//	KGLOG_CHECK_ERROR(bRetCode);

	bRetCode = g_ItemLib.Init();
	KGLOG_CHECK_ERROR(bRetCode);

//    bRetCode = g_ShopCenter.Init();
//    KGLOG_CHECK_ERROR(bRetCode);

    if (g_WorldSettings.m_RobotParam.nPreLoadMap)
    {
        bRetCode = g_SO3World.PreLoadScene(1,1);
        KGLOG_PROCESS_ERROR(bRetCode);
    }

    return TRUE;

Exit0:
	printf("[FAILED]\n");

	return FALSE;
}

BOOL KSO3World::UnInit(void)
{
	BOOL bRetCode = FALSE;

//    g_ShopCenter.UnInit();

	KDeleteSceneFunc DeleteSceneFunc;

	bRetCode = g_ObjectManager.m_SceneSet.Traverse(DeleteSceneFunc);
	KGLOG_CHECK_ERROR(bRetCode);

//    g_SkillManager.UnInit();

	bRetCode = g_WorldSettings.UnInit();
	KGLOG_CHECK_ERROR(bRetCode);

	return TRUE;
}

BOOL KSO3World::KDeleteSceneFunc::operator()(DWORD dwID, KScene* pScene)
{
    BOOL bResult  = FALSE;
	BOOL bRetCode = FALSE;

	bRetCode = pScene->DeleteAllRegion();
	KGLOG_PROCESS_ERROR(bRetCode);

	bRetCode = g_ObjectManager.m_SceneSet.Delete(pScene);
	KGLOG_PROCESS_ERROR(bRetCode);

	bResult =  TRUE;
Exit0:
	return bResult;
}

BOOL KSO3World::KGetSceneFunc::operator()(DWORD dwID, KScene* pScene)
{
    if (pScene->m_dwMapID == m_dwMapID && 
        pScene->m_dwMapCopyIndex == m_dwMapCopyIndex)
    {
        m_pScene = pScene;
        return FALSE;
    }
    return TRUE;
}

BOOL KSO3World::PreLoadScene(DWORD dwMapID, DWORD dwMapCopyIndex)
{
    int nResult = FALSE;
    BOOL bRetCode = FALSE;
    KScene* pScene = NULL;

    m_Lock.Lock();

    m_nPreLoadScene = TRUE;

    KGetSceneFunc GetSceneFunc;
    GetSceneFunc.m_dwMapID = dwMapID;
    GetSceneFunc.m_dwMapCopyIndex = dwMapCopyIndex;

    bRetCode = g_ObjectManager.m_SceneSet.Traverse(GetSceneFunc);
    KGLOG_PROCESS_ERROR(bRetCode);

    pScene = GetSceneFunc.m_pScene;
    if (!pScene)
    {
        pScene = NewClientScene(dwMapID, dwMapCopyIndex);
        KG_PROCESS_ERROR(pScene);

        bRetCode = pScene->PreLoadScene();
        KGLOG_PROCESS_ERROR(bRetCode);
    }    

    nResult = TRUE;
Exit0:
    if (!nResult)
    {
        m_nPreLoadScene = FALSE;
    }
    m_Lock.Unlock();
    return nResult;
}

KScene* KSO3World::GetScene(DWORD dwMapID, DWORD dwMapCopyIndex)
{
    BOOL bRetCode = FALSE;
    KScene* pScene = NULL;

    if (!m_nPreLoadScene)
        m_Lock.Lock();

    KGetSceneFunc GetSceneFunc;
    GetSceneFunc.m_dwMapID = dwMapID;
    GetSceneFunc.m_dwMapCopyIndex = dwMapCopyIndex;

    bRetCode = g_ObjectManager.m_SceneSet.Traverse(GetSceneFunc);
    KGLOG_PROCESS_ERROR(bRetCode);

    pScene = GetSceneFunc.m_pScene;
    if (!pScene && !m_nPreLoadScene)
    {
        pScene = NewClientScene(dwMapID, dwMapCopyIndex);
        KG_PROCESS_ERROR(pScene);
    }

Exit0:
    if (!m_nPreLoadScene)
        m_Lock.Unlock();
    return pScene;
}

BOOL KSO3World::DeleteScene(KScene* pScene, BOOL bDirectDel)
{
    BOOL bRetCode = FALSE;
    BOOL bResult  = FALSE;

    m_Lock.Lock();
    bRetCode = pScene->DeleteAllRegion();
    KGLOG_PROCESS_ERROR(bRetCode);

    pScene->UnInit();

    bRetCode = g_ObjectManager.m_SceneSet.Delete(pScene);
    KGLOG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
Exit0:
    m_Lock.Unlock();
    return bResult;
}

KScene* KSO3World::NewClientScene(DWORD dwMapID, DWORD dwMapCopyIndex)
{
    BOOL bRetCode = FALSE;
    KScene* pScene = NULL;

    //��ȡ������ز�����Region����NPC������Doodad����
    KMapParams* pMapParams = g_WorldSettings.m_MapListFile.GetData(dwMapID);
    KGLOG_PROCESS_ERROR(pMapParams);

    DWORD dwRegionWidth = pMapParams->nRegionWidth;
    KGLOG_PROCESS_ERROR(dwRegionWidth != 0);

    DWORD dwRegionHeight = pMapParams->nRegionHeight;
    KGLOG_PROCESS_ERROR(dwRegionHeight != 0);

    DWORD dwNpcTotal = pMapParams->dwServerNpcTotal;
    DWORD dwDoodadTotal = pMapParams->dwServerDoodadTotal;

    //��ȡScene
    pScene = g_ObjectManager.m_SceneSet.New();
    KGLOG_PROCESS_ERROR(pScene);

    bRetCode = pScene->Init();
    KGLOG_PROCESS_ERROR(bRetCode);

    pScene->m_dwMapID = dwMapID;
    pScene->m_dwMapCopyIndex = dwMapCopyIndex;
    pScene->m_eSceneState = ssNormal;

    bRetCode = pScene->SetRegionWidth(dwRegionWidth);
    KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = pScene->SetRegionHeight(dwRegionHeight);
    KGLOG_PROCESS_ERROR(bRetCode);

    char* pszMapName = pMapParams->szMapName;
    strncpy(pScene->m_szName, pszMapName, _NAME_LEN);
    pScene->m_szName[_NAME_LEN - 1] = 0;

    //��ȡRegion
    //for (DWORD dwHeightIndex = 0; dwHeightIndex < dwRegionHeight; dwHeightIndex++)
    //{
    //    for (DWORD dwWidthIndex = 0; dwWidthIndex < dwRegionWidth; dwWidthIndex++)
    //    {
    //        KRegion* pRegion = g_ObjectManager.m_RegionSet.New();
    //        KGLOG_PROCESS_ERROR(pRegion);

    //        bRetCode = pRegion->Init();
    //        KGLOG_PROCESS_ERROR(bRetCode);

    //        bRetCode = pScene->SetRegion(dwWidthIndex, dwHeightIndex, pRegion);
    //        KGLOG_PROCESS_ERROR(bRetCode);            
    //    }        
    //}

    return pScene;

Exit0:
    if (pScene)
    {
        DeleteScene(pScene, TRUE);
    }
    return NULL;
}

//����Ϸ���������Npc����Npc�����ݱ����Ѿ��������
BOOL KSO3World::AddNpc(KNpc* pNpc, KScene* pScene, int nX, int nY, int nZ/* = 0*/)
{
	BOOL bRetCode = FALSE;

	//�������
	KGLOG_PROCESS_ERROR(pNpc);
	KGLOG_PROCESS_ERROR(pScene);
	KGLOG_PROCESS_ERROR(nX >= 0);
	KGLOG_PROCESS_ERROR(nY >= 0);
	KGLOG_PROCESS_ERROR(nZ >= 0);

	//�趨����
	pNpc->m_nX = nX;
	pNpc->m_nY = nY;
	pNpc->m_nZ = nZ;
	pNpc->m_nXCell = nX / CELL_LENGTH % REGION_GRID_WIDTH;
	pNpc->m_nYCell = nY / CELL_LENGTH % REGION_GRID_HEIGHT;


	//�򳡾������
	bRetCode = pScene->AddNpc(pNpc);
	KGLOG_PROCESS_ERROR(bRetCode);

	return TRUE;

Exit0:
	if (pNpc && pNpc->m_pScene)
	{
		pNpc->m_pScene->RemoveNpc(pNpc);
	}

	return FALSE;
}

//����Ϸ�������Ƴ�һ��NPC
BOOL KSO3World::RemoveNpc(KNpc* pNpc)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(pNpc);
	KGLOG_PROCESS_ERROR(pNpc->m_pScene);

	bRetCode = pNpc->m_pScene->RemoveNpc(pNpc);
	KGLOG_PROCESS_ERROR(bRetCode);

	return TRUE;

Exit0:

	return FALSE;
}


//����Ϸ���������һ���³���װ����
BOOL KSO3World::AddDoodad(KDoodad* pDoodad, KScene* pScene, int nX, int nY, int nZ)
{
	BOOL bRetCode = FALSE;

	//�������
	KGLOG_PROCESS_ERROR(pDoodad);
	KGLOG_PROCESS_ERROR(pScene);
	KGLOG_PROCESS_ERROR(nX >= 0);
	KGLOG_PROCESS_ERROR(nY >= 0);
	KGLOG_PROCESS_ERROR(nZ >= 0);

	//�趨����
	pDoodad->m_nX = nX;
	pDoodad->m_nY = nY;
	pDoodad->m_nZ = nZ;
	pDoodad->m_nXCell = nX / CELL_LENGTH % REGION_GRID_WIDTH;
	pDoodad->m_nYCell = nY / CELL_LENGTH % REGION_GRID_HEIGHT;

	//�򳡾������
	bRetCode = pScene->AddDoodad(pDoodad);
	KGLOG_PROCESS_ERROR(bRetCode);

	return TRUE;

Exit0:
	if (pDoodad && pDoodad->m_pScene)
	{
		pDoodad->m_pScene->RemoveDoodad(pDoodad);
	}

	return FALSE;
}

//����Ϸ�������Ƴ�һ������װ����
BOOL KSO3World::RemoveDoodad(KDoodad* pDoodad)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(pDoodad);
	KGLOG_PROCESS_ERROR(pDoodad->m_pScene);

	bRetCode = pDoodad->m_pScene->RemoveDoodad(pDoodad);
	KGLOG_PROCESS_ERROR(bRetCode);

	return TRUE;

Exit0:

	return FALSE;
}

//����Ϸ���������һ�������
BOOL KSO3World::AddPlayer(KPlayer* pPlayer, KScene* pScene, int nX, int nY, int nZ)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(pPlayer);
	KGLOG_PROCESS_ERROR(pScene);
	KGLOG_PROCESS_ERROR(nX >= 0);
	KGLOG_PROCESS_ERROR(nY >= 0);
	KGLOG_PROCESS_ERROR(nZ >= 0);

	//�趨����
	pPlayer->m_nX = nX;
	pPlayer->m_nY = nY;
	pPlayer->m_nZ = nZ;
	pPlayer->m_nXCell = nX / CELL_LENGTH % REGION_GRID_WIDTH;
	pPlayer->m_nYCell = nY / CELL_LENGTH % REGION_GRID_HEIGHT;

	//�򳡾������
	bRetCode = pScene->AddPlayer(pPlayer);
	KGLOG_PROCESS_ERROR(bRetCode);

	return TRUE;

Exit0:

	if (pPlayer && pPlayer->m_pScene)
	{
		pPlayer->m_pScene->RemovePlayer(pPlayer);
	}

	return FALSE;
}

//����Ϸ�������Ƴ�һ�����
BOOL KSO3World::RemovePlayer(KPlayer* pPlayer)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(pPlayer);
	KGLOG_PROCESS_ERROR(pPlayer->m_pScene);

	bRetCode = pPlayer->m_pScene->RemovePlayer(pPlayer);
	KGLOG_PROCESS_ERROR(bRetCode);

	return TRUE;
Exit0:
	return FALSE;
}
