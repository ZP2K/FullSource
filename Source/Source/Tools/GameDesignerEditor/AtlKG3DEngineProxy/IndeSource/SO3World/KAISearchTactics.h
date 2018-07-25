#ifndef _KAISEARCH_TACTICS_H_
#define _KAISEARCH_TACTICS_H_

#include <list>
#include <vector>

#include "KSceneObject.h"
#include "KScene.h"
#include "KCharacter.h"
#include "KRegion.h"

struct KVirtualMove 
{
    int m_nSrcX;
    int m_nSrcY;
    int m_nSrcZ;

    int m_nDstX;
    int m_nDstY;
    int m_nDstZ;

    int m_nDistance;

    int m_State;
    // ... ...


}; 

struct KSearchNode
{
	KCharacter* m_pCharacter;
	int m_nDistance2;
};

bool    g_CompByDistance(const KSearchNode& left, const KSearchNode& right);
double  g_GetRangeByLevelDiff(int nLevelDiff);

template <typename T>
void AISearchPlayer(T& rTactic)
{
    BOOL          bRetCode   = false;
    KSceneObject* pCenterObj = (KSceneObject*)rTactic.m_pSelf;
    KScene*       pScene     = NULL;

    assert(pCenterObj);

    pScene = pCenterObj->m_pScene;
    KGLOG_PROCESS_ERROR(pScene);

    pScene->TraverseRangePlayer(pCenterObj->m_pRegion, rTactic, pScene->m_nBroadcastRegion);

Exit0:
    return;
}

template <typename T>
void AISearchNpc(T& rTactic)
{
    BOOL          bRetCode   = false;
    KSceneObject* pCenterObj = (KSceneObject*)rTactic.m_pSelf;
    KScene*       pScene     = NULL;

    assert(pCenterObj);

    pScene = pCenterObj->m_pScene;
    KGLOG_PROCESS_ERROR(pScene);

    pScene->TraverseRangeNpc(pCenterObj->m_pRegion, rTactic, pScene->m_nBroadcastRegion);

Exit0:
    return;
}

template <typename T>
void AISearchCharacter(T& rTactic)
{
	BOOL          bRetCode   = false;
	KSceneObject* pCenterObj = (KSceneObject*)rTactic.m_pSelf;
    KScene*       pScene     = NULL;

    assert(pCenterObj);

    pScene = pCenterObj->m_pScene;
    KGLOG_PROCESS_ERROR(pScene);

    bRetCode = pScene->TraverseRangePlayer(pCenterObj->m_pRegion, rTactic, pScene->m_nBroadcastRegion);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = pScene->TraverseRangeNpc(pCenterObj->m_pRegion, rTactic, pScene->m_nBroadcastRegion);
    KG_PROCESS_ERROR(bRetCode);

Exit0:
	return;
}

template <typename T>
void AISearchDoodad(T& rTactic)
{
    BOOL          bRetCode   = false;
    KSceneObject* pCenterObj = (KSceneObject*)rTactic.m_pSelf;
    KScene*       pScene     = NULL;

    assert(pCenterObj);

    pScene = pCenterObj->m_pScene;
    KGLOG_PROCESS_ERROR(pScene);

    pScene->TraverseRangeDoodad(pCenterObj->m_pRegion, rTactic, pScene->m_nBroadcastRegion);

Exit0:
    return;
}

template<typename TSelfType>
struct KSearchTactic
{
    KSearchTactic() : m_pSelf(NULL){}
    TSelfType* m_pSelf;
};

struct KSearchForDoodad : KSearchTactic<KPlayer>
{
	KSearchForDoodad() : m_nDistance(INT_MAX){ m_Result.clear(); }
	int m_nDistance;
	DWORD m_dwDoodadTemplateID;
	std::vector<KDoodad*> m_Result;
	BOOL operator()(KDoodad* pDoodad);
};

struct KSearchForAnyDoodad : KSearchTactic<KPlayer>
{
	KSearchForAnyDoodad() : m_pResult(NULL), m_nDistance(INT_MAX){}
	int m_nDistance;
	DWORD m_dwDoodadTemplateID;
	KDoodad* m_pResult;
	BOOL operator()(KDoodad* pDoodad);
};


struct KSearchForPartyMember : KSearchTactic<KPlayer>
{
    int m_nDistance;
    std::vector<KPlayer*> m_Result;
    BOOL operator()(KPlayer* pPlayer);
};

struct KSearchForAllies : KSearchTactic<KCharacter>
{
	KSearchForAllies() : m_nDistance2(INT_MAX){ m_Result.clear(); }
	int m_nDistance2;
	std::vector<KCharacter*> m_Result;
	BOOL operator()(KCharacter* pCharacter);
};

struct KSearchForAnyVisiblePlayer : KSearchTactic<KCharacter>
{
    KSearchForAnyVisiblePlayer() : m_pResult(NULL){}
    KPlayer* m_pResult;
    BOOL operator()(KPlayer* pPlayer);
};


struct KSearchVisiblePlayerClosest : KSearchTactic<KCharacter>
{
    KSearchVisiblePlayerClosest() : m_pResult(NULL), m_nDistance2(INT_MAX){}
    KPlayer* m_pResult;
    int      m_nDistance2;
    BOOL operator()(KPlayer* pPlayer);
};

struct KSearchForAnyVisibleNpc : KSearchTactic<KCharacter>
{
    KSearchForAnyVisibleNpc() : m_pResult(NULL){}
    KNpc* m_pResult;
    BOOL operator()(KNpc* pPlayer);
};

//��ʱ���룬TAB�ã�����Ƕ����ƣ����ؾ�����Ϣ
//TODO:�����Ƿ��ж��ϰ���ѡ��
struct KSearchForCharacter : KSearchTactic<KCharacter>
{
	KSearchForCharacter() : m_nDistance2(INT_MAX)
        , m_nAngle(DIRECTION_COUNT)
        , m_nRelation(0)
        , m_nLifeConversionLevel(0)
        , m_bAdjustByVisible(true)
        , m_bAdjustByAttribute(false)                        
        , m_bAdjustRangeByLevelDiff(false)
        , m_bAdjustRangeByTargetLife(false){ m_Result.clear(); }
	int m_nDistance2;
	int m_nAngle;
	int m_nRelation;
    int m_nLifeConversionLevel;
    BOOL m_bAdjustByVisible;            //3D�ϰ�
	BOOL m_bAdjustByAttribute;		    //�������Ե������ȼ�
	BOOL m_bAdjustRangeByLevelDiff;     //���ݵȼ����������ҷ�Χ
    BOOL m_bAdjustRangeByTargetLife;    //����Ŀ��Ѫ���������ҷ�Χ
	std::vector<KSearchNode> m_Result;
	BOOL operator()(KCharacter* pCharacter);
};

//�����ֻ��һ���ͷ��صİ汾
struct KSearchForAnyCharacter : KSearchTactic<KCharacter>
{
	KSearchForAnyCharacter() : m_nDistance2(INT_MAX)
        , m_nAngle(DIRECTION_COUNT)
        , m_nRelation(0)
        , m_nLifeConversionLevel(0)
        , m_pResult(NULL)
        , m_bAdjustByVisible(true)
        , m_bAdjustByAttribute(false)
        , m_bAdjustRangeByLevelDiff(false)
        , m_bAdjustRangeByTargetLife(false){}
	int m_nDistance2;
	int m_nAngle;
	int m_nRelation;	
    int m_nLifeConversionLevel;
    BOOL m_bAdjustByVisible;            //3D�ϰ�
    BOOL m_bAdjustRangeByTargetLife;    //����Ŀ��Ѫ���������ҷ�Χ
    BOOL m_bAdjustRangeByLevelDiff;     //���ݵȼ����������ҷ�Χ
    BOOL m_bAdjustByAttribute;		    //�������Ե������ȼ�
	KCharacter* m_pResult;
	BOOL operator()(KCharacter* pCharacter);
};

struct KSearchForMultiCharacterAddThreatList : KSearchTactic<KCharacter>
{
    KSearchForMultiCharacterAddThreatList() : m_nDistance2(INT_MAX)
        , m_nAngle(DIRECTION_COUNT)
        , m_nRelation(0)
        , m_pResult(NULL)
        , m_nLifeConversionLevel(0)
        , m_bAdjustByVisible(true)
        , m_bAdjustByAttribute(false)
        , m_bAdjustRangeByLevelDiff(false)
        , m_bAdjustRangeByTargetLife(false)
        , m_nCharacterCounter(1){}
    int m_nDistance2;
    int m_nAngle;
    int m_nRelation;
    int m_nCharacterCounter;
    int m_nLifeConversionLevel;
    BOOL m_bAdjustByVisible;            //3D�ϰ�
    BOOL m_bAdjustRangeByLevelDiff;     //���ݵȼ����������ҷ�Χ
    BOOL m_bAdjustRangeByTargetLife;    //����Ŀ��Ѫ���������ҷ�Χ
    BOOL m_bAdjustByAttribute;		    //�������Ե������ȼ�
    KCharacter* m_pResult;
    BOOL operator()(KCharacter* pCharacter);
};

#endif //_KAISEARCH_TACTICS_H_
