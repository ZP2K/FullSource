/************************************************************************/
/* ���߼��ϻ���							                                */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.25	Create												*/
/************************************************************************/
#ifndef _KITEM_LIST_H_
#define _KITEM_LIST_H_

#include <vector>

#include "SO3Result.h"
#include "KItem.h"
#include "KInventory.h"

class KTarget;

#define TRADE_BOX_WIDTH		8
#define TRADE_BOX_HEIGHT	4

typedef struct 
{
	DWORD	dwBox;
	DWORD	dwX;
} TItemPos;

class KPlayer;

struct KUSE_ITEM
{
    DWORD dwProfessionID;
    DWORD dwProfessionBranchID;
    DWORD dwProfessionLevel;
};

#pragma	pack(1)
struct KITEM_DB_HEADER 
{
    BYTE byBox;
    BYTE byPos;
    BYTE byDataLen;
    BYTE byData[0];
};
#pragma	pack()

class KItemList
{
public:
	int		m_nMoney;			//  ��Ǯ
	int		m_nMoneyLimit;		//  ��Ǯ����

    BOOL    m_bBankOpened;      //  �����Ƿ��Ѵ�

#ifdef _SERVER
    BOOL    m_bSoldListOpened;  //  �ع��б��Ƿ��Ѵ�
#endif

public:
	BOOL	Init(KPlayer* pPlayer);
	void    UnInit();

#ifdef _SERVER
    BOOL    Activate();
#endif

    ADD_ITEM_RESULT_CODE    CanAddItem(KItem* pItem);
    ADD_ITEM_RESULT_CODE    CanAddItemToPos(KItem* pItem, DWORD dwDestBox, DWORD dwDestX);
	ADD_ITEM_RESULT_CODE	AddItem(KItem* pItem, BOOL bNotifyClient = true);
	ADD_ITEM_RESULT_CODE	AddItemToPos(KItem* pItem, DWORD dwBoxIndex, DWORD dwX);
    	
    KItem*	PickUpItem(DWORD dwBoxIndex, DWORD dwX, BOOL bSync = true);
	KItem*	GetItem(DWORD dwBoxIndex, DWORD dwX);
    
    DWORD   GetItemAmountInEquipAndPackage(DWORD dwTabType, DWORD dwIndex, int nBookRecipeID = -1);
    DWORD	GetItemTotalAmount(DWORD dwTabType, DWORD dwIndex, int nBookRecipeID = -1);
    DWORD   GetItemAmountInBox(int nBoxIndex, DWORD dwTabType, DWORD dwIndex, int nBookRecipeID = -1);
    
    BOOL	GetItemPos(int nVersion, DWORD dwTabType, DWORD dwIndex, DWORD& dwBox, DWORD& dwX);
	BOOL	GetItemPos(DWORD dwTabType, DWORD dwIndex, DWORD& dwBox, DWORD& dwX);
	BOOL	GetItemPos(DWORD dwID, DWORD& dwBox, DWORD& dwX);
    
    int		GetBoxType(DWORD dwBox);
	
#ifdef _SERVER
	BOOL	CostItemInEquipAndPackage(DWORD dwTabType, DWORD dwIndex, DWORD dwAmount, int nBookRecipeID = -1);
    BOOL    CostItemInAllBox(DWORD dwTabType, DWORD dwIndex, DWORD dwAmount, int nBookRecipeID = -1);
#endif
    //��ȡĳ��ָ����Ʒ������
    BOOL	CostSingleItem(DWORD dwBoxIndex, DWORD dwX, int nAmount);

    //����ָ����С�Ŀ��пռ�
	BOOL	FindFreeRoom(DWORD dwBoxIndex, DWORD& dwX);
	BOOL	FindStackRoom(DWORD dwBoxIndex, DWORD& dwX, KItem* pItem);
	BOOL	FindFreeSoldList(DWORD& dwX);
    BOOL	FindFreeRoomInPackage(
        DWORD&  dwBoxIndex, DWORD& dwX, 
        int     nItemGenerType = INVALID_CONTAIN_ITEM_TYPE, 
        int     nItemSubType = INVALID_CONTAIN_ITEM_TYPE
    );
	BOOL	FindStackRoomInPackage(DWORD& dwBoxIndex, DWORD& dwX, KItem* pItem);
	BOOL	FindStackRoomInBank(DWORD& dwBoxIndex, DWORD& dwX, KItem* pItem);
	
	int	    GetAllPackageFreeRoomSize(void);
	int     GetBoxFreeRoomSize(int nBoxIndex);

    //Ǯ��صķ���
	int		GetMoney() { return m_nMoney; };
	BOOL	SetMoney(int nMoney);
    BOOL	AddMoney(int nMoney, BOOL bShowMsg = true);
    
    //Ǯ������صķ���
	int		GetMoneyLimit() { return m_nMoneyLimit; };
	
	//����������Ʒ��λ��
    ITEM_RESULT_CODE    CanExchangeItemToPos(KItem* pItem, DWORD dwDestBox, DWORD dwDestX);
    ITEM_RESULT_CODE	CanExchangeItem(DWORD dwSrcBox, DWORD dwSrcX, DWORD dwDestBox, DWORD dwDestX);
    ITEM_RESULT_CODE	ExchangeItem(const TItemPos& Src, const TItemPos& Dest, DWORD dwAmount = 0);
    
    BOOL    GetEquipPos(int nRepresentIndex, DWORD *pdwEquipPos);
    EQUIPMENT_INVENTORY_TYPE GetEquipPosBySubType(EQUIPMENT_SUB_TYPE eSubType);

    ITEM_RESULT_CODE GetCanEquipPos(KItem* pItem, int* pnRetPos);

	ITEM_RESULT_CODE CanEquip(KItem* pItem, DWORD dwX);
    ITEM_RESULT_CODE CanUnEquip(DWORD dwX);

    BOOL	         Equip(KItem* pItem, DWORD dwX);
    BOOL	         UnEquip(DWORD dwX);

    //������ɾ��һ����Ʒ��������Ϸ����ɾ����
	ITEM_RESULT_CODE	DestroyItem(const TItemPos& Pos, BOOL bSync = true);
	ITEM_RESULT_CODE	DestroyItem(KItem* pItem, BOOL bSync = true);
    
    DWORD	GetBoxSize(DWORD dwBox);
    DWORD   GetBoxUsedSize(DWORD dwBox);
	BOOL    GetContainType(DWORD dwBox, int* pnContainItemGenerType, int* pnContainItemSubType);
    int	    GetMainHandType();

#ifdef _SERVER
    BOOL    Load(BYTE* pbyData, size_t uDataLen);
    BOOL    Save(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
	
    BOOL    TidyUpSoldList();    
    void    AbradeArmor();
    void    AbradeItemAtEquipBox(DWORD dwPos);

    //��װ������;öȵİٷֱ�, ĥ��һ�������е�����װ��(��Ҫ������Ҹ���ʱ,����ĳͷ�)
    BOOL    AbradeEquipment(int nBoxType, int nAbradePercent);
#endif

    BOOL	ApplyAttrib(KItem* pItem);
	BOOL	UnApplyAttrib(KItem* pItem);

    BOOL	IsPackageSlot(DWORD dwBox, DWORD dwX);

#ifdef _SERVER
    BOOL	Repair(DWORD dwBox, DWORD dwX);
    BOOL	EnableBankPackage();
#endif
	int		GetBankPackageCount();

#ifdef _CLIENT
	BOOL	SetEnablePackageCount(int nCount);
#endif

    BOOL	IsBankPackageEnabled(EQUIPMENT_INVENTORY_TYPE eInventoryType);
#ifdef _SERVER
	BOOL	ConsumeAmmo(DWORD dwAmount);
	void	ReloadAmmo(DWORD dwTabType, DWORD dwIndex);
#endif

	BOOL	AddEnchant(DWORD dwBoxIndex, DWORD dwX, int nEnchantID);
	BOOL	RemoveEnchant(DWORD dwBoxIndex, DWORD dwX, BOOL bPermanentEnchant);
    
    BOOL    SetEquipColor(DWORD dwBoxIndex, DWORD dwX, int nColorID);

    KInventory* GetAllInventory();

    void    OnChangeCamp();

private:
    KItem*	FindItem(DWORD dwTabType, DWORD dwIndex, DWORD& dwBox, DWORD& dwX); 

#ifdef _SERVER
    void    CallItemScript(KItem* pItem, const TItemPos& crItemPos, KTarget& rTarget, BOOL& rbContinueSkill, BOOL& rbDestroyItem);
    USE_ITEM_RESULT_CODE CastItemSkill(KItem* pItem, KTarget& rTarget, BOOL* pbCostItemAtOnce);
#endif
    
    void    SetRepresentID(int nRepresentIndex, int nRepresentID);
    
    BOOL    GetEquipRepresentIndex(DWORD dwEquipPos, int* pnIndex);
    BOOL    GetEquipColorIndex(DWORD dwEquipPos, int* pnIndex);

	// ���ȫ��װ���Ƿ񻹷�������
	BOOL	CheckEquipRequire(void);
	BOOL	PushItem(KItem* pItem, DWORD dwBox, BOOL bNotifyClient);
	ITEM_RESULT_CODE	ExchangePackageSlot(DWORD dwBox1, DWORD dwBox2);
	ITEM_RESULT_CODE	EnablePackage(DWORD dwBox, DWORD dwSize, int nContainGenerType, int nContainSubType);
	ITEM_RESULT_CODE	DisablePackage(DWORD dwBox);

	int		GetEquipedSetNum(DWORD dwSetID);
	BOOL	ApplySetAttrib(DWORD dwSetID, int nSetNum);
	BOOL	UnApplySetAttrib(DWORD dwSetID, int nSetNum);
    
    BOOL    CostItemInBox(int nBoxIndex, DWORD dwTabType, DWORD dwIndex, DWORD& rdwAmount, int nBookRecipeID);
    
public:
    USE_ITEM_RESULT_CODE CanUseItem(KItem* pItem, DWORD dwBox, KUSE_ITEM* pUseItemInfo);
    BOOL CheckUseItemTargetType(KItem* pItem);

#ifdef _SERVER
    USE_ITEM_RESULT_CODE UseItem(TItemPos ItemPos, KTarget& rTarget);
    USE_ITEM_RESULT_CODE UseItem(DWORD dwBox, DWORD dwX, KTarget& rTarget);
    
    void OnBreakUseItem(KItem* pItem);
#endif
    void FinishUseItem(KItem* pItem);

#ifdef _SERVER
private:
    USE_ITEM_RESULT_CODE UseItemOnlyScripts(KItem* pSrcItem, KTarget& rTarget);
    USE_ITEM_RESULT_CODE UseItemOnlySkill(KItem* pSrcItem, KTarget& rTarget);
    USE_ITEM_RESULT_CODE UseItemScriptsAndSkill(KItem* pSrcItem, KTarget& rTarget);
#endif
    
private:
	KInventory			m_Box[ibTotal];
	DWORD				m_dwBoxCount;									// ���ӵ��ܸ���
	int					m_nEnabledBankPackageCount;						// ��������б���������

	KPlayer*			m_pPlayer;
};

DWORD g_GetBoxIndexFromSlot(DWORD dwSlot);

#endif	//_KITEM_LIST_H_
