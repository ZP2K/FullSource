#ifndef _KLOOT_LIST_H_
#define _KLOOT_LIST_H_

#include "Global.h"
#include "SO3Result.h"

//ʰȡ����ʱ�䣬ʱ�䵽���κ��˶�����ʰȡ
#define FREE_LOOT_TIME		(GAME_FPS * 300)
//������ʱ�䣬ʱ�䵽�󣬵�ǰ������ֵ��˻��ʰȡȨ��
#define LOOT_ROLL_TIME		(GAME_FPS * 120)
#define ROLL_ITEM_NUMBER	100

class KItem;
class KPlayer;
class KNpc;
class KDoodad;

struct KLootItem
{
	KItem*		pItem;
	int			nRollCount;
	KPlayer*	pWinner;
	int			nRandom;
	DWORD		dwFlag[MAX_PARTY_SIZE];			
	BOOL		bCanOTFreeLoot;					//FALSE��ʾ��ʹ������ʱ�䣬����ʰȡ��ʽΪ����ʰȡ��Ҳֻ����winner��ʰȡ(��������ĳЩ������Ʒ)
	BOOL		bDisplay;
};

class KLootList
{
public:
	KLootList(void);
	~KLootList(void);

	BOOL Init(void);
	BOOL UnInit(void);

#ifdef _SERVER
	//ÿ����Ϸѭ������飬������ʱ���Ƿ��ѵ�
	BOOL CheckGameLoop(void);
#endif //_SERVER

	//��ʰȡ���м�����Ʒ��������ţ����㿪ʼ����
	int AddItem(KItem* pItem);
	BOOL RemoveItem(KItem* pItem);
	//���money
	BOOL AddMoney(int nMoney);
	BOOL SetMoney(int nMoney);
	inline int GetMoney();
	
	// ���һ��������ʾ�Ƿ�õ������ڳ�ʱ��������ʰȡ
	BOOL SetWinner(int nItemIndex, KPlayer *pWinner, BOOL bCanOTFreeLoot);

	//��ȡ������Ʒ����
	int GetItemCount(void);
	//loot���ʣ����Ʒ��
	inline int GetSize(void);
	KLootItem* GetLootItem(DWORD dwLootIndex);
	BOOL CanLoot(DWORD dwLootIndex, KPlayer* pPlayer);
	BOOL CanRoll(DWORD dwRollIndex, KPlayer* pPlayer, BOOL bCheckRollFlag);

	BOOL IsVisible(KPlayer* pPlayer);

	inline BOOL SetLootMode(int nLootMode);
	inline PARTY_LOOT_MODE GetLootMode();

	inline BOOL SetRollQuality(int nRollQuality);
	inline int GetRollQuality();

	void OnOpen();
	void OnClose();

#if defined(_SERVER)
	// ����m_pLooterList
	BOOL PreProcess(KPlayer *pAttacker, KNpc *pNpc);

	BOOL SetOwnerInParty();
	BOOL IsEmpty();

	//ʰȡ���е���Ʒ
	SKILL_RESULT_CODE LootOrRollItem(KPlayer* pLooter, DWORD dwDoodadID, DWORD dwItemID, BOOL bCancel);
	SKILL_RESULT_CODE LootMoney(KPlayer *pPlayer, KDoodad *pDoodad);
	SKILL_RESULT_CODE LeaderDistributeItem(KPlayer* pLeader, KPlayer* pDstPlayer, KDoodad* pDoodad, DWORD dwItemID);
#endif	//_SERVER

	BOOL IsInLooterList(KPlayer* pPlayer);

	KPlayer*	m_pLooterList[MAX_PARTY_SIZE]; //���в����ߣ��ӳ�ģʽ���б�����Ȩ������ʱ�������������ʰȡȨ
	int			m_nLooterCount; //m_pLooterList�е��������
	KDoodad*	m_pDoodad;
private:
	KLootItem	m_LootList[MAX_LOOT_SIZE];
	int			m_nItemCount;
	int			m_nLootedCount;
	int			m_nGameLoop;
	int			m_nMoney;
	int			m_nLootMode;
	int			m_nRollQuality;
	BOOL		m_bIsOpened;

	BOOL		m_bOverLootRollTime;
	BOOL		m_bOverFreeLootTime;
};

inline int KLootList::GetSize()
{
	return m_nItemCount - m_nLootedCount;
}

inline int KLootList::GetMoney()
{
	return m_nMoney;
}

inline BOOL KLootList::SetLootMode(int nLootMode)
{
	KGLOG_PROCESS_ERROR(nLootMode > plmInvalid && nLootMode < plmTotal);

	m_nLootMode = nLootMode;
	
	return TRUE;
Exit0:
	return FALSE;
}

inline PARTY_LOOT_MODE KLootList::GetLootMode()
{
	return (PARTY_LOOT_MODE)m_nLootMode;
}

inline BOOL KLootList::SetRollQuality(int nRollQuality)
{
	KGLOG_PROCESS_ERROR(nRollQuality >= 0);

	m_nRollQuality = nRollQuality;

	return TRUE;
Exit0:
	return FALSE;
}

inline int KLootList::GetRollQuality()
{
	return m_nRollQuality;
}

#endif	//_KLOOT_LIST_H_