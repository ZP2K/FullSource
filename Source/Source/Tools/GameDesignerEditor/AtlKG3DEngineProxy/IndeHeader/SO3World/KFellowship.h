////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KFellowship.h
//  Version     : 1.0
//  Creator     : Chen Jie
//  Create Date : 2007-01-26 15:04:58
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _KFELLOWSHIP_H_
#define _KFELLOWSHIP_H_

#include <map>
#include <set>
#include <deque>
#include <algorithm>

#include "SO3GlobalDef.h"

#define KG_FELLOWSHIP_DB_CURRENT_DATA_VERSION   1

//////////////////////////////////////////////////////////////////////////
//
// ����ģ����ÿ�� GameServer �Լ�ά���ģ�GameCenter ֻ��һ�����ݿ�
// �� Cache�����һ����ҵĺ�������ͬʱ���������� GameServer ���̣�
// ���п��ܷ������ݸ��ǵĴ��󡣲������ǵ���Ϸ��֤��һ����Ҳ���
// ͬʱ������ GameServer ���̳��֡������ڿ���������˳���Ϸ��ʱ
// �����ҪС�Ĵ���
//
//////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct KG_FELLOWSHIP_BIT_FLAG
{
    uint8_t dwGroupID           :   8;
    uint8_t bMarried            :   1;
    uint8_t bBrother            :   1;
    uint8_t bReserve            :   6;
    int8_t  nWhisperDailyCount  :   4;
    int8_t  nDuelDailyCount     :   4;
};

struct KG_FELLOWSHIP_ALL_GS
{
    DWORD   dwAlliedPlayerID;
    int     nAttraction;
    int     nLevel;
    DWORD   dwMapID;
    DWORD   dwForceID;
    BYTE    byFlags[sizeof(KG_FELLOWSHIP_BIT_FLAG)];
    char    szName[_NAME_LEN];
    char    szRemark[_NAME_LEN];
};

struct KG_FOE_ALL_GS
{
    DWORD   dwAlliedPlayerID;
    int     nLevel;
    DWORD   dwForceID;
    char    szName[_NAME_LEN];
};

struct KG_BLACK_LIST_ALL_GS
{
    DWORD   dwAlliedPlayerID;
    char    szName[_NAME_LEN];
};

struct KG_FELLOWSHIP_DB 
{
    BYTE    nVersion;
};

struct KG_FELLOWSHIP_ENTRY_V1 
{
    DWORD                   dwAlliedPlayerID;
    int                     nAttraction;
    KG_FELLOWSHIP_BIT_FLAG  Flags;
    char                    szRemark[_NAME_LEN];
};

struct KG_FELLOWSHIP_FOE_ENTRY_V1 
{
    DWORD   dwAlliedPlayerID;
};

struct KG_FELLOWSHIP_BLACKLIST_ENTRY_V1 
{
    DWORD   dwAlliedPlayerID;
};

struct KG_FELLOWSHIP_DB_V1 : KG_FELLOWSHIP_DB
{
    char    szGroupName[KG_FELLOWSHIP_MAX_CUSTEM_GROUP][_NAME_LEN];
    int     nFriendCount;
    int     nFoeCount;
    int     nBlackListCount;
    BYTE    byData[0];
};
#pragma pack()

//////////////////////////////////////////////////////////////////////////

struct KGFellowship
{
    int                     m_nAttraction;
    int                     m_nPlayerLevel;   // ���ѵĵȼ�
    DWORD                   m_dwMapID;
    DWORD                   m_dwForceID;
    KG_FELLOWSHIP_BIT_FLAG  m_BitFlag;
    char                    m_szName[_NAME_LEN];
    char                    m_szRemark[_NAME_LEN];

    KGFellowship()
    {
        m_nAttraction   = 0;
        m_nPlayerLevel  = 0;
        m_dwMapID       = 0;
        m_dwForceID     = 0;
        memset(&m_BitFlag, 0, sizeof(m_BitFlag));
        m_szName[0]     = '\0';
        m_szRemark[0]   = '\0';
    };
};

struct KGFoe 
{
    int                     m_nPlayerLevel;   // ���˵ĵȼ�
    DWORD                   m_dwForceID;
    char                    m_szName[_NAME_LEN];

    KGFoe()
    {
        m_nPlayerLevel  = 0;
        m_dwForceID     = 0;
        m_szName[0]     = '\0';
    }
};

struct KGBlackNode  
{
    char                    m_szName[_NAME_LEN];

    KGBlackNode()
    {
        m_szName[0]     = '\0';
    }
};

struct KGFellowshipGroupList 
{
    int     nGroupCount;
    char    szGroupName[KG_FELLOWSHIP_MAX_CUSTEM_GROUP][_NAME_LEN];
};

typedef std::pair<DWORD, DWORD>                                     KG_ID_PAIR;

typedef std::map<KG_ID_PAIR, KGFellowship, less<KG_ID_PAIR> >       KG_FELLOWSHIP_MAP;
typedef KG_FELLOWSHIP_MAP::iterator                                 KG_FELLOWSHIP_MAP_IT;

typedef std::map<KG_ID_PAIR, KGFoe, less<KG_ID_PAIR> >              KG_FOE_MAP;
typedef KG_FOE_MAP::iterator                                        KG_FOE_MAP_IT;

typedef std::map<KG_ID_PAIR, KGBlackNode, less<KG_ID_PAIR> >        KG_BLACK_LIST_MAP;
typedef KG_BLACK_LIST_MAP::iterator                                 KG_BLACK_LIST_MAP_IT;

typedef std::set<KG_ID_PAIR, less<KG_ID_PAIR> >                     KG_ID_PAIR_SET;
typedef KG_ID_PAIR_SET::iterator                                    KG_ID_PAIR_SET_IT;

typedef std::map<DWORD, KGFellowshipGroupList>                      KG_FELLOWSHIP_GROUPNAMES_MAP;
typedef std::set<DWORD>                                             KG_FELLOWSHIP_ONLINE_ID_SET;

class KGFellowshipMgr
{
public:
    KGFellowshipMgr() {};
    ~KGFellowshipMgr() {};

    BOOL            Init();
    void            UnInit();

    void            Reset();

#ifdef _SERVER
    BOOL            LoadFellowshipData(DWORD dwPlayerID);   // �� GameCenter ���غ������ݡ�
    BOOL            OnLoadFellowshipData(DWORD dwPlayerID, size_t uDataSize, BYTE byData[]);
    BOOL            OnLoadFellowshipDataV1(DWORD dwPlayerID, size_t uDataSize, BYTE byData[]);

    BOOL            SaveFellowshipData(DWORD dwPlayerID);   // ���������ݽ��� GameCenter ���档

    BOOL            AttractionFalloff(DWORD dwPlayerID, int nDays); // �øж�˥����
    BOOL            RefreshDailyCount(DWORD dwPlayerID); // �øж�˥����

    BOOL            AddFellowshipAttraction(DWORD dwPlayerID, DWORD dwAlliedPlayerID, int nAttraction, BOOL bBidirectional = true);
#endif

    BOOL            UnloadPlayerFellowship(DWORD dwPlayerID);

    KGFellowship*   GetFellowship(DWORD dwPlayerID, DWORD dwAlliedPlayerID);

    KGFellowship*   AddFellowship(DWORD dwPlayerID, DWORD dwAlliedPlayerID, const char cszAlliedPlayerName[], BOOL bNotifyClient);
    BOOL            DelFellowship(DWORD dwPlayerID, DWORD dwAlliedPlayerID);
    int             CountFellowship(DWORD dwPlayerID);
    BOOL            ClearFellowship(DWORD dwPlayerID);

    KGFoe*          GetFoe(DWORD dwPlayerID, DWORD dwAlliedPlayerID);

    KGFoe*          AddFoe(DWORD dwPlayerID, DWORD dwAlliedPlayerID, const char cszAlliedPlayerName[], BOOL bNotifyClient);
    BOOL            DelFoe(DWORD dwPlayerID, DWORD dwAlliedPlayerID);
    int             CountFoe(DWORD dwPlayerID);
    BOOL            ClearFoe(DWORD dwPlayerID);

    KGBlackNode*    GetBlackListNode(DWORD dwPlayerID, DWORD dwAlliedPlayerID);

    KGBlackNode*    AddBlackList(DWORD dwPlayerID, DWORD dwAlliedPlayerID, const char cszAlliedPlayerName[], BOOL bNotifyClient);
    BOOL            DelBlackList(DWORD dwPlayerID, DWORD dwAlliedPlayerID);
    int             CountBlackList(DWORD dwPlayerID);
    BOOL            ClearBlackList(DWORD dwPlayerID);

    BOOL            AddFellowshipGroup(DWORD dwPlayerID, const char cszGroupName[]);
    BOOL            DelFellowshipGroup(DWORD dwPlayerID, DWORD dwGroupID);
    BOOL            RenameFellowshipGroup(DWORD dwPlayerID, DWORD dwGroupID, const char cszGroupName[]);

    static int      AttractionValue2Level(int nAttraction, int nCoefficient = 1);

    template <class TFunc>
    BOOL            TraverseFellowshipID(DWORD dwPlayerID, TFunc& Func);

    template <class TFunc>
    BOOL            TraverseFoeID(DWORD dwPlayerID, TFunc& Func);

    template <class TFunc>
    BOOL            TraverseBlackListID(DWORD dwPlayerID, TFunc& Func);

    template <class TFunc>
    BOOL            TraverseReverseFellowshipID(DWORD dwPlayerID, TFunc& Func);

    template <class TFunc>
    BOOL            TraverseReverseFoeID(DWORD dwPlayerID, TFunc& Func);

    template <class TFunc>
    BOOL            TraverseReverseBlackListID(DWORD dwPlayerID, TFunc& Func);

public:
    KG_FELLOWSHIP_GROUPNAMES_MAP    m_GroupNamesMap;
#ifdef _CLIENT
    BOOL                            m_bWaitInitSync;
#endif // _CLIENT

private:
    KG_FELLOWSHIP_MAP               m_FellowshipMap;        // ������ҵĺ����б�(��Ҫ����)
    KG_ID_PAIR_SET                  m_ReverseFellowshipSet; // ����������(�����̣�����������ҵĺ�����Ϣ��̬����)\

    KG_FOE_MAP                      m_FoeMap;               // �����б�
    KG_ID_PAIR_SET                  m_ReverseFoeSet;

    KG_BLACK_LIST_MAP               m_BlackListMap;         // ������
    KG_ID_PAIR_SET                  m_ReverseBlackListSet;

    KG_FELLOWSHIP_ONLINE_ID_SET     m_OnlineIDSet;
};

struct GetFellowshipIDListFunc
{
    GetFellowshipIDListFunc() : m_nIDListCount(0) {};

    BOOL operator()(DWORD dwPlayerID, DWORD dwAlliedPlayerID);

    int    m_nIDListCount;
    DWORD  m_dwIDList[KG_FELLOWSHIP_MAX_RECORD];
};

template <class TFunc>
BOOL KGFellowshipMgr::TraverseFellowshipID(DWORD dwPlayerID, TFunc& Func)
{
    BOOL                    bResult  = false;
    BOOL                    bRetCode = false;
    KG_FELLOWSHIP_MAP_IT    it;
    KG_FELLOWSHIP_MAP_IT    itEnd;

    assert(dwPlayerID != ERROR_ID);

    it = m_FellowshipMap.lower_bound(KG_ID_PAIR(dwPlayerID, 0));
    itEnd = m_FellowshipMap.lower_bound(KG_ID_PAIR(dwPlayerID + 1, 0));

    for (NULL; it != itEnd; ++it)
    {
        KG_ID_PAIR& rIDPair = (KG_ID_PAIR&)it->first;

        bRetCode = Func(rIDPair.first, rIDPair.second);
        if (!bRetCode)
            break;
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

template <class TFunc>
BOOL KGFellowshipMgr::TraverseFoeID(DWORD dwPlayerID, TFunc& Func)
{
    BOOL            bResult  = false;
    BOOL            bRetCode = false;
    KG_FOE_MAP_IT   it;
    KG_FOE_MAP_IT   itEnd;

    assert(dwPlayerID != ERROR_ID);

    it = m_FoeMap.lower_bound(KG_ID_PAIR(dwPlayerID, 0));
    itEnd = m_FoeMap.lower_bound(KG_ID_PAIR(dwPlayerID + 1, 0));

    for (NULL; it != itEnd; ++it)
    {
        KG_ID_PAIR& rIDPair = (KG_ID_PAIR&)it->first;

        bRetCode = Func(rIDPair.first, rIDPair.second);
        if (!bRetCode)
            break;
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

template <class TFunc>
BOOL KGFellowshipMgr::TraverseBlackListID(DWORD dwPlayerID, TFunc& Func)
{
    BOOL                    bResult  = false;
    BOOL                    bRetCode = false;
    KG_BLACK_LIST_MAP_IT    it;
    KG_BLACK_LIST_MAP_IT    itEnd;

    assert(dwPlayerID != ERROR_ID);

    it = m_BlackListMap.lower_bound(KG_ID_PAIR(dwPlayerID, 0));
    itEnd = m_BlackListMap.lower_bound(KG_ID_PAIR(dwPlayerID + 1, 0));

    for (NULL; it != itEnd; ++it)
    {
        KG_ID_PAIR& rIDPair = (KG_ID_PAIR&)it->first;

        bRetCode = Func(rIDPair.first, rIDPair.second);
        if (!bRetCode)
            break;
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

template <class TFunc>
BOOL KGFellowshipMgr::TraverseReverseFellowshipID(DWORD dwPlayerID, TFunc& Func)
{
    BOOL                bResult  = false;
    BOOL                bRetCode = false;
    KG_ID_PAIR_SET_IT   it;
    KG_ID_PAIR_SET_IT   itEnd;

    assert(dwPlayerID != ERROR_ID);

    it = m_ReverseFellowshipSet.lower_bound(KG_ID_PAIR(dwPlayerID, 0));
    itEnd = m_ReverseFellowshipSet.lower_bound(KG_ID_PAIR(dwPlayerID + 1, 0));

    for (NULL; it != itEnd; ++it)
    {
        KG_ID_PAIR& rIDPair = (KG_ID_PAIR&)*it;

        bRetCode = Func(rIDPair.first, rIDPair.second);
        if (!bRetCode)
            break;
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

template <class TFunc>
BOOL KGFellowshipMgr::TraverseReverseFoeID(DWORD dwPlayerID, TFunc& Func)
{
    BOOL                bResult  = false;
    BOOL                bRetCode = false;
    KG_ID_PAIR_SET_IT   it;
    KG_ID_PAIR_SET_IT   itEnd;

    assert(dwPlayerID != ERROR_ID);

    it = m_ReverseFoeSet.lower_bound(KG_ID_PAIR(dwPlayerID, 0));
    itEnd = m_ReverseFoeSet.lower_bound(KG_ID_PAIR(dwPlayerID + 1, 0));

    for (NULL; it != itEnd; ++it)
    {
        KG_ID_PAIR& rIDPair = (KG_ID_PAIR&)*it;

        bRetCode = Func(rIDPair.first, rIDPair.second);
        if (!bRetCode)
            break;
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

template <class TFunc>
BOOL KGFellowshipMgr::TraverseReverseBlackListID(DWORD dwPlayerID, TFunc& Func)
{
    BOOL                bResult  = false;
    BOOL                bRetCode = false;
    KG_ID_PAIR_SET_IT   it;
    KG_ID_PAIR_SET_IT   itEnd;

    assert(dwPlayerID != ERROR_ID);

    it = m_ReverseBlackListSet.lower_bound(KG_ID_PAIR(dwPlayerID, 0));
    itEnd = m_ReverseBlackListSet.lower_bound(KG_ID_PAIR(dwPlayerID + 1, 0));

    for (NULL; it != itEnd; ++it)
    {
        KG_ID_PAIR& rIDPair = (KG_ID_PAIR&)*it;

        bRetCode = Func(rIDPair.first, rIDPair.second);
        if (!bRetCode)
            break;
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

#endif // _KFELLOWSHIP_H_
