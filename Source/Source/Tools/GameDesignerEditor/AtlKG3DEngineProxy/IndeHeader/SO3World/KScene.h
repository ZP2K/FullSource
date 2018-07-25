/************************************************************************/
/* ��ͼ��								                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.31	Create												*/
/* Comment	 :															*/
/*		��װ��һ����Ϸ�����е�ͼ�����в���������						*/
/************************************************************************/
#ifndef _KSCENE_H_
#define	_KSCENE_H_

#include <list>
#include <map>
#include <string>
#include <bitset>
#include "Engine/KList.h"
#include "Global.h"
#include "SO3GlobalDef.h"
#include "KBaseObject.h"
#include "KCell.h"
#include "Luna.h"
#include "KCustomData.h"
#include "INpcFile.h"
#include "IDoodadFile.h"
#include "KRegion.h"

#ifdef _SERVER
#include "KScriptTimerList.h"
#include "KNpcReviveManager.h"
#include "KDoodadReviveManager.h"
#include "KAIPatrolGroupManager.h"
#include "KThreatList.h"
#include "KEscortQuestManager.h"
#endif

class KTerrainScene;
struct KPartition;
class KRegion;
class KScene;
class KPlayer;
class KNpc;
class KDoodad;
class KNpcTeam;

struct KREVIVE_POSITION;

#define SCENE_CUSTOM_DATA_SIZE      1024
#define INT64_STRING_LEN 32

enum SCENE_STATE
{
	ssInvalid = 0,			// ��Ч״̬
    ssLoading,              // ���ڼ���
    ssCompleteLoading,      // �������
    ssFailedLoading,	    // ����ʧ��
	ssNormal,				// ����״̬

	ssTotal
};


enum KRESIZE_SCENE_ALIGN_TYPE
{
	eResizeSceneAlignInvalid = 0,

	eResizeSceneAlignLeftTop,
	eResizeSceneAlignLeftBottom,
	eResizeSceneAlignRightTop,
	eResizeSceneAlignRightBottom,

	eResizeSceneAlignTotal
};

typedef std::map<DWORD, KCustomData<64> > KOBJ_CUSTOM_DATA_TABLE;

typedef int(*FUNC_LOADPROGRESS)(LPCTSTR pName,float a); 

class KScene : public KBaseObject
{
public:
	KScene();
	~KScene();

	BOOL Init();
    void UnInit();

	int Activate();

public:
	// �趨�ر������Ϣ
	BOOL SetCellData(int nXCell, int nYCell, KCell::KBaseInfo* pBaseInfo, WORD wLowLayer, WORD wHighLayer);
	// ��ȡ�ر������Ϣ
	BOOL GetCellData(int nXCell, int nYCell, KCell::KBaseInfo& BaseInfo, WORD& wLowLayer, WORD& wHighLayer);

	BOOL GetObstacle(int nXCell, int nYCell, int nZ, KCell::KBaseInfo& BaseInfo, WORD& wLowLayer, WORD& wHighLayer);
	BOOL CheckObstacle(int nXCell, int nYCell, int nLayer);

    BOOL ClearAllScript();
    BOOL ClearScriptSpecial(DWORD dwScriptID);

	// �����ͼ��С
	BOOL Resize(int nRegionWidth, int nRegionHeight, KRESIZE_SCENE_ALIGN_TYPE eAlignType);

	// Ԥ�����ϰ�����
	typedef void (*PREPROCESS_PROGRESS_CALLBACK_FUNC)(int nYCell, int nCellWidth, int nCellHeight);
    BOOL PreprocessCalculateData();

	// ����Scene�е�Region
	template <class TFunc> BOOL TraverseRegion(TFunc& Func);
	// ����Scene�е�Cell
	template <class TFunc> BOOL TraverseCell(TFunc& Func);
	// ����Scene�е�Npc
	template <class TFunc> BOOL TraverseNpc(TFunc& Func);
	// ����Scene�е����
	template <class TFunc> BOOL TraversePlayer(TFunc& Func);
    // ������������Region��������㲥,�㲥��Χ�ɳ�������
	template <class TFunc> void Broadcast(KRegion* pCenter, TFunc& BroadcastFunc, int nRange = -1);

    // ������Χָ����Χ��Region,����ָ��"�뾶"
    template <class TFunc> BOOL TraverseRangePlayer(KRegion* pCenter, TFunc& Func, int nRange = -1);
    template <class TFunc> BOOL TraverseRangeNpc(KRegion* pCenter, TFunc& Func, int nRange = -1);
    template <class TFunc> BOOL TraverseRangeDoodad(KRegion* pCenter, TFunc& Func, int nRange = -1);

private:
    BOOL ReviseCalculateGradient();
	BOOL CalculateObstacleRange();

public:
    BOOL IsVisible(int nSrcX, int nSrcY, int nSrcZ, int nDstX, int nDstY, int nDstZ);

public:
	DWORD			m_dwMapID;						
	int			    m_nCopyIndex;
	SCENE_STATE		m_eSceneState;					
	char			m_szName[_NAME_LEN];
    int             m_nType;                        // ��������
    BOOL            m_bReviveInSitu;                // ����Ƿ�����ԭ�ظ���
	DWORD			m_dwScriptID;
    int             m_nBroadcastRegion;             // ͬ���㲥�İ뾶,��RegionΪ��λ
    DWORD           m_dwBanSkillMask;
    DWORD           m_dwBattleRelationMask;
    int             m_nReviveCycle;
    BOOL            m_bDoNotGoThroughRoof;

    int             m_nInFightPlayerCount;

#ifdef _SERVER
    DWORD           m_dwOwnerID;
    BOOL            m_bSaved;
    BOOL            m_bProgressChanged;
#endif

    BOOL            m_bCanPK;

// �������� --------------------------------------->
#ifdef _SERVER
public:
    KCustomData<SCENE_PROGRESS_DATA_LEN> m_ProgressData;
    
    int LuaSetProgress(Lua_State* L);
    int LuaGetProgress(Lua_State* L);
#endif
// <-------------------------------------------��������
public:

#ifdef _SERVER
	BOOL ServerLoad();
    BOOL CallInitScript();
#endif

#if defined(_CLIENT)
    BOOL EditorLoad(FUNC_LOADPROGRESS);
#endif

	char m_szFilePath[MAX_PATH];

	BOOL Save(const char cszName[], FUNC_LOADPROGRESS);

    BOOL SaveBaseInfo();
    BOOL LoadBaseInfo();

    BOOL LoadPartitionTable();

    // ʹָ�������Region��Ч: δ���صľͼ���,�����˵ľͼ���
    BOOL ValidateRegions(int nRegionX, int nRegionY);

#ifdef _SERVER
    KTerrainScene*                      m_pTerrainScene;

	KCustomData<SCENE_CUSTOM_DATA_SIZE> m_CustomData;

	// �������������,ͬһ����������ͬһ����ͬʱֻ����һ������
	KEscortQuestManager			        m_EscortQuestMgr;

	KScriptTimerList<KScene, 100>		m_ScriptTimerList;

    KOBJ_CUSTOM_DATA_TABLE              m_NpcCustomDataTable;
    KOBJ_CUSTOM_DATA_TABLE              m_DoodadCustomDataTable;
    
    KAIPatrolGroupManager               m_AIPatrolGroupManager;
    KThreatLink                         m_ThreatLink;

public:
    KNpcReviveManager       m_NpcReviveManager;
    KDoodadReviveManager    m_DoodadReviveManager;
#endif

    time_t                  m_nTimeStamp;

    BOOL        IsPointInSceneSpace(int nX, int nY, int nZ);
    
	BOOL        SetRegionWidth(int nWidth);
	BOOL        SetRegionHeight(int nHeight);
	int         GetRegionWidth(void);
	int         GetRegionHeight(void);

	BOOL        SetRegion(int nRegionX, int nRegionY, KRegion* pRegion);
	KRegion*    GetRegion(int nRegionX, int nRegionY);

	// �����ص�����õ�Regionָ��
	KRegion*    GetRegionByPoint(int nX, int nY);
	BOOL        DelRegion(int nRegionX, int nRegionY);
	BOOL        DeleteAllRegion(void);
	BOOL        DeleteAllObstacle(void);

	// NPC��غ���
	BOOL AddNpc(KNpc* pNpc);
	BOOL RemoveNpc(KNpc* pNpc);

	BOOL AddDoodad(KDoodad* pDoodad);
	BOOL RemoveDoodad(KDoodad* pDoodad);

	BOOL AddPlayer(KPlayer* pPlayer);
    void CallEnterSceneScript(KPlayer* pPlayer);
	BOOL RemovePlayer(KPlayer* pPlayer);

    void DeleteAllObjects();

    void DeleteAllNpc();
    void DeleteAllDoodad();

    template <typename TFunc, typename TObj>
    void RegisterObjectViewSync(TObj* pObj);

    // ���ж�̬�ϰ�����Ĭ��Ϊ"true",true��ʾ��Щ�ϰ���enable��
    BOOL SetDynamicObstacleState(int nGroupIndex, BOOL bState);
    BOOL GetDynamicObstacleState(int nGroupIndex); 

public:
	int							    m_nRegionWidth;
	int  							m_nRegionHeight;
	KRegion*						m_RegionList[MAX_REGION_WIDTH][MAX_REGION_HEIGHT];
    KPartition*                     m_pPartitionTable;

    typedef std::bitset<MAX_OBSTACLE_GROUP> KDYNAMIC_OBSTACLE_SWITCH_TABLE;
    KDYNAMIC_OBSTACLE_SWITCH_TABLE  m_DynamicObstacleSwitchTable;

	friend class KSO3World;
	friend class KCharacter;

#ifdef _SERVER
private:
    INpcFile*               m_piNpcFile;
    IDoodadFile*            m_piDoodadFile;

	// �����б�
	typedef std::map<std::string, DWORD> KNICK_NAME_LIST;
	KNICK_NAME_LIST         m_NpcNickNameList;
    KNICK_NAME_LIST         m_DoodadNickNameList;

public:
    typedef std::vector<std::pair<DWORD /* dwMapID */, BOOL /* bMapAutoJoin */> >  KPQID_FLAG_VECTOR;
    KPQID_FLAG_VECTOR       m_PQIDFlagVector;

public:
    BOOL DeleteNpcNickname(KNpc* pNpc);
    BOOL DeleteDoodadNickname(KDoodad* pDoodad);

	BOOL AddReviveNpc(KNpc *pNpc, int nReviveTime);
	BOOL AddReviveDoodad(KDoodad *pDoodad, BOOL bIgnoreReliveTime = false);

    BOOL CompleteLoading();
    BOOL ActivateAllRegions();
    BOOL CompleteNpcLoading();
    BOOL CompleteDoodadLoading();
#endif //_SERVER

public:
	// �ű��ӿں���
	DECLARE_LUA_CLASS(KScene);

	DECLARE_LUA_STRING(Name, _NAME_LEN);
	DECLARE_LUA_DWORD(MapID);
    DECLARE_LUA_INTEGER(CopyIndex);
	DECLARE_LUA_DWORD(ID);
    DECLARE_LUA_INTEGER(Type);
	DECLARE_LUA_ENUM(SceneState);
    DECLARE_LUA_BOOL(ReviveInSitu);
    DECLARE_LUA_INTEGER(InFightPlayerCount);

#if defined(_SERVER)
	int LuaCreateNpc(Lua_State* L);
	int LuaDestroyNpc(Lua_State* L);
	int LuaMoveNpc(Lua_State* L);

	int LuaCreateDoodad(Lua_State* L);
	int LuaDestroyDoodad(Lua_State* L);

	int LuaSetTimer(Lua_State* L);
	int LuaStopTimer(Lua_State* L);
	int LuaRestartTimer(Lua_State* L);
	int LuaRemoveTimer(Lua_State* L);

	int LuaGetNpcByNickName(Lua_State* L);
    int LuaIsNickNameNpcExist(Lua_State* L);

    int LuaGetDoodadByNickName(Lua_State* L);
    int LuaIsNickNameDoodadExist(Lua_State* L);

	// �û�����    
    int LuaSetCustomInteger4(Lua_State* L);
    int LuaGetCustomInteger4(Lua_State* L);
    int LuaSetCustomInteger2(Lua_State* L);
    int LuaGetCustomInteger2(Lua_State* L);
    int LuaSetCustomInteger1(Lua_State* L);
    int LuaGetCustomInteger1(Lua_State* L);

    int LuaSetCustomUnsigned4(Lua_State* L);
    int LuaGetCustomUnsigned4(Lua_State* L);
    int LuaSetCustomUnsigned2(Lua_State* L);
    int LuaGetCustomUnsigned2(Lua_State* L);
    int LuaSetCustomUnsigned1(Lua_State* L);
    int LuaGetCustomUnsigned1(Lua_State* L);
    
    int LuaSetCustomBoolean(Lua_State* L);
    int LuaGetCustomBoolean(Lua_State* L);

    // ƫ����, �ڼ���Bit(������, ��0��ʼ����), ��ֵΪtrue/false
    int LuaSetCustomBitValue(Lua_State* L);
    int LuaGetCustomBitValue(Lua_State* L);

    int LuaSetCustomString(Lua_State* L);
    int LuaGetCustomString(Lua_State* L);

	// ��������ӿ�
    int LuaSetEscortNpcByNickName(Lua_State* L);
	int LuaSetEscortQuestFailFrames(Lua_State* L);
    int LuaSetEscortQuestValue(Lua_State* L);
    int LuaEscortPlayerAddBuff(Lua_State* L);
    int LuaEscortPlayerDelBuff(Lua_State* L);
    int LuaCloseEscortQuest(Lua_State* L);
    int LuaGetEscortQuestPlayerList(Lua_State* L);

    int LuaSaveMap(Lua_State* L);
    int LuaSendMessage(Lua_State* L);
#endif //_SERVER
    
#ifdef _SERVER
    int LuaDebugRevive(Lua_State* L);
#endif

    int LuaIsReviveCycle(Lua_State* L);
};

//�õ�ȫ�����ص��������ڵ�Regionָ��
inline KRegion* KScene::GetRegionByPoint(int nX, int nY)
{
	KRegion*    pRegion     = NULL;
	int         nRegionX    = nX / REGION_GRID_WIDTH / CELL_LENGTH;
	int         nRegionY    = nY / REGION_GRID_HEIGHT / CELL_LENGTH;

	KG_PROCESS_ERROR(nX >= 0);
    KG_PROCESS_ERROR(nX < CELL_LENGTH * REGION_GRID_WIDTH * m_nRegionWidth);

	KG_PROCESS_ERROR(nY >= 0);
    KG_PROCESS_ERROR(nY < CELL_LENGTH * REGION_GRID_WIDTH * m_nRegionHeight);

	pRegion = m_RegionList[nRegionX][nRegionY];

Exit0:
	return pRegion;
}

template <class TFunc>
BOOL KScene::TraverseRegion(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	for (int nX = 0; nX < m_nRegionWidth; nX++)
	{
		for (int nY = 0; nY < m_nRegionHeight; nY++)
		{
			KRegion* pRegion = m_RegionList[nX][nY];
			if (pRegion == NULL)
				continue;

			bRetCode = Func(pRegion);
			KG_PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
Exit0:
	return bResult;
}

template <class TFunc>
BOOL KScene::TraverseCell(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	for (int i = 0; i < MAX_REGION_WIDTH; ++i)
	{
		for (int j = 0; j < MAX_REGION_HEIGHT; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];
			if (pRegion == NULL)
				continue;

			bRetCode = pRegion->TraverseCell(Func);
			KG_PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
Exit0:
	return bResult;
}

// ����Scene�е�Npc
template <class TFunc> 
BOOL KScene::TraverseNpc(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	for (int i = 0; i < MAX_REGION_WIDTH; ++i)
	{
		for (int j = 0; j < MAX_REGION_HEIGHT; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];
			if (pRegion == NULL)
				continue;

			bRetCode = pRegion->TraverseNpc(Func);
			KG_PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
Exit0:
	return bResult;
}

// ����Scene�е����
template <class TFunc> 
BOOL KScene::TraversePlayer(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	for (int i = 0; i < MAX_REGION_WIDTH; ++i)
	{
		for (int j = 0; j < MAX_REGION_HEIGHT; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];
			if (pRegion == NULL)
				continue;

			bRetCode = pRegion->TraversePlayer(Func);
			KG_PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
Exit0:
	return bResult;
}

template <class TFunc>
void KScene::Broadcast(KRegion* pCenter, TFunc& BroadcastFunc, int nRange)
{
    int nLeft       = 0;
    int nRight      = 0;
    int nTop        = 0;
    int nBottom     = 0;

    assert(pCenter);

    if (nRange < 0)
    {
        nRange = m_nBroadcastRegion;
    }

    nLeft   = pCenter->m_nRegionX - nRange;
    nRight  = pCenter->m_nRegionX + nRange;
    nBottom = pCenter->m_nRegionY - nRange;
    nTop    = pCenter->m_nRegionY + nRange;

    for (int nX = nLeft; nX <= nRight; nX++)
    {
        for (int nY = nBottom; nY <= nTop; nY++)
        {
            KRegion* pRegion = GetRegion(nX, nY);
            if (pRegion)
            {
                BroadcastFunc.m_nMaxSyncCount = MAX_SYNC_PER_REGION;

                pRegion->TraversePlayer(BroadcastFunc);
            }
        }
    }
}

template <class TFunc>
BOOL KScene::TraverseRangePlayer(KRegion* pCenter, TFunc& Func, int nRange)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    int     nLeft       = 0;
    int     nRight      = 0;
    int     nTop        = 0;
    int     nBottom     = 0;

    assert(pCenter);

    if (nRange < 0)
    {
        nRange = m_nBroadcastRegion;
    }

    nLeft   = pCenter->m_nRegionX - nRange;
    nRight  = pCenter->m_nRegionX + nRange;
    nBottom = pCenter->m_nRegionY - nRange;
    nTop    = pCenter->m_nRegionY + nRange;

    for (int nX = nLeft; nX <= nRight; nX++)
    {
        for (int nY = nBottom; nY <= nTop; nY++)
        {
            KRegion* pRegion = GetRegion(nX, nY);
            if (pRegion == NULL)
                continue;
            
            bRetCode = pRegion->TraversePlayer(Func);
            KG_PROCESS_ERROR(bRetCode);
        }
    }

    bResult = true;
Exit0:
    return bResult;
}

template <class TFunc>
BOOL KScene::TraverseRangeNpc(KRegion* pCenter, TFunc& Func, int nRange)
{
    BOOL    bResult     = false;
    BOOL    bRetCode   = false;
    int     nLeft       = 0;
    int     nRight      = 0;
    int     nTop        = 0;
    int     nBottom     = 0;

    assert(pCenter);

    if (nRange < 0)
    {
        nRange = m_nBroadcastRegion;
    }

    nLeft   = pCenter->m_nRegionX - nRange;
    nRight  = pCenter->m_nRegionX + nRange;
    nBottom = pCenter->m_nRegionY - nRange;
    nTop    = pCenter->m_nRegionY + nRange;

    for (int nX = nLeft; nX <= nRight; nX++)
    {
        for (int nY = nBottom; nY <= nTop; nY++)
        {
            KRegion* pRegion = GetRegion(nX, nY);
            if (pRegion == NULL)
                continue;
            
            bRetCode = pRegion->TraverseNpc(Func);
            KG_PROCESS_ERROR(bRetCode);
        }
    }

    bResult = true;
Exit0:
    return bResult;
}

template <class TFunc>
BOOL KScene::TraverseRangeDoodad(KRegion* pCenter, TFunc& Func, int nRange)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    int     nLeft       = 0;
    int     nRight      = 0;
    int     nTop        = 0;
    int     nBottom     = 0;

    assert(pCenter);

    if (nRange < 0)
    {
        nRange = m_nBroadcastRegion;
    }

    nLeft   = pCenter->m_nRegionX - nRange;
    nRight  = pCenter->m_nRegionX + nRange;
    nBottom = pCenter->m_nRegionY - nRange;
    nTop    = pCenter->m_nRegionY + nRange;

    for (int nX = nLeft; nX <= nRight; nX++)
    {
        for (int nY = nBottom; nY <= nTop; nY++)
        {
            KRegion* pRegion = GetRegion(nX, nY);
            if (pRegion == NULL)
                continue;
            
            bRetCode = pRegion->TraverseDoodad(Func);
            KG_PROCESS_ERROR(bRetCode);
        }
    }

    bResult = true;
Exit0:
    return bResult;
}

template <typename TFunc, typename TObj>
void KScene::RegisterObjectViewSync(TObj* pObj)
{
    int         nLeft       = 0;
    int         nRight      = 0;
    int         nBottom     = 0;
    int         nTop        = 0;
    KRegion*    pRegion     = pObj->m_pRegion;
    TFunc       Func(pObj);

    assert(pRegion);

    nLeft       = pRegion->m_nRegionX - m_nBroadcastRegion;
    nRight      = pRegion->m_nRegionX + m_nBroadcastRegion;
    nBottom     = pRegion->m_nRegionY - m_nBroadcastRegion;
    nTop        = pRegion->m_nRegionY + m_nBroadcastRegion;

   for (int nX = nLeft; nX <= nRight; nX++)
    {
        for (int nY = nBottom; nY <= nTop; nY++)
        {
            pRegion = GetRegion(nX, nY);
            if (pRegion)
            {
                pRegion->TraversePlayer(Func);
            }
        }
    }
}
#endif
