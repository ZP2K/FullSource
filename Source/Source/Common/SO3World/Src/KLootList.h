#ifndef _KLOOT_LIST_H_
#define _KLOOT_LIST_H_

#include "Global.h"
#include "SO3Result.h"

#define ROLL_ITEM_NUMBER	100

class KItem;
class KPlayer;
class KNpc;
class KSceneObject;

enum LOOT_ITEM_TYPE // ���庬����ο�����ĵ�
{
    litInvalid = 0,

    litOwnerLoot,
    litOverTimeLooterFree,
    litAbsoluteFree,
    litLooterFree,
    litNeedRoll,
    litNeedDistribute
};

struct KLootItem
{
	KItem*		    pItem;
    LOOT_ITEM_TYPE  eLootItemType;
#ifdef _SERVER
    BYTE            byRollCount;
    DWORD           dwOwnerIDSet;   // ǰMAX_PARTY_SIZE��bit��ʾʰȡ�б��ж�Ӧ����ܷ�ʰȡ�õ���
	int			    nMaxRollPoint;                  
	bool            bRollFlag[MAX_PARTY_SIZE];		// false ��ʾ�����и������Ȩ��roll��rollһ��֮������Ϊtrue
	DWORD		    dwQuestID;						
	DWORD		    dwQuestState;					
#else
    bool            bDisplay;
#endif
};

BOOL g_IsInLootRange(KSceneObject* pObjectA, KSceneObject* pObjectB, int nRegionRange);

class KLootList
{
public:
	~KLootList(void);

	BOOL            Init(int nOverLootFrame);
	void            UnInit(void);

#ifdef _SERVER
    void            Activate(KDoodad* pDoodad);
    BOOL            IsEmpty();
    BOOL            CanLoot(int nItemIndex, KPlayer* pPlayer);
	BOOL            CanRoll(int nItemIndex, KPlayer* pPlayer);
    BOOL            CanDisplayInClient(int nItemIndex, KPlayer* pPlayer);
    BOOL            SetItemLooter(int nItemIndex, DWORD dwLooterID);
    DWORD           GetItemLooter(int nItemIndex);
    BOOL            IsItemOwner(int nItemIndex, DWORD dwLooterID);
    void            OnOpen(KDoodad *pDoodad);
	BOOL            GenerateLooterList(KPlayer* pAttacker, KDoodad* pDoodad);
	void            ProcessNpcDropItems();
    int             GetLooterPos(DWORD dwPlayerID);

    LOOT_ITEM_RESULT_CODE   LootItem(KPlayer* pLooter, KDoodad* pDoodad, DWORD dwItemID);
    LOOT_ITEM_RESULT_CODE   CancelRollItem(KPlayer* pRoller, KDoodad* pDoodad, DWORD dwItemID);
    LOOT_ITEM_RESULT_CODE   RollItem(KPlayer* pRoller, KDoodad* pDoodad, DWORD dwItemID);
	LOOT_ITEM_RESULT_CODE   LootMoney(KPlayer *pPlayer, KDoodad *pDoodad);
    LOOT_ITEM_RESULT_CODE   DistributeItem(KPlayer* pDistributer, KPlayer* pDstPlayer, KDoodad* pDoodad, DWORD dwItemID);
	void                    GetLooterList(std::vector<DWORD>& vecLooterList);
    void                    ProcessBindOnTimeLimitationItem(KDoodad* pDoodad, DWORD dwItemID, DWORD dwLooterID, LOOT_ITEM_TYPE eType);
#endif
    
    KLootItem*      AddItem(KItem* pItem, DWORD dwQuestID = 0, DWORD dwQuestState = 0);
    BOOL            AddMoney(int nMoney);
	BOOL            SetMoney(int nMoney);
    int             GetMoney(){ return m_nMoney; };
    int             GetItemCount(void){ return m_nItemCount; };
	KLootItem*      GetLootItem(DWORD dwLootIndex);

#ifdef _CLIENT
	BOOL            IsVisible(KPlayer* pPlayer, KDoodad* pDoodad);
#endif
    
	BOOL            IsInLooterList(DWORD dwPlayerID);

#ifdef _SERVER
private:
    void            ProcessRollerOffline(KDoodad* pDoodad);
    void            ProcessOverRollTime(KDoodad* pDoodad);
    void            ProcessOverLootTime(KDoodad* pDoodad);
    BOOL            EndRollItem(int nItemIndex, KDoodad* pDoodad);
    BOOL            CanLootAbsoluteFreeQuestItem(int nItemIndex, KPlayer* pPlayer);
#endif

public:
    DWORD           m_LooterList[MAX_PARTY_SIZE];
    int             m_nLooterCount;
#ifdef _SERVER
    BOOL            m_bLootListChanged;
#endif

private:
	KLootItem	    m_ItemList[MAX_LOOT_SIZE];
	int			    m_nItemCount;
	int			    m_nMoney;
	
#ifdef _SERVER
    int			    m_nLootedCount;
	int             m_nEndRollFrame; // roll����ʱ��֡��
    int             m_nOverLootFrame; // ����ʱ�����ʱ��֡��
    BOOL		    m_bIsOpened;
    BYTE            m_OfflineFlag[MAX_PARTY_SIZE];
#endif
};
#endif
