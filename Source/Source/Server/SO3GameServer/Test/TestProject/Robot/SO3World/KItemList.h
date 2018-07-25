/************************************************************************/
/* ���߼��ϻ���							                                */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.25	Create												*/
/************************************************************************/
#ifndef _KITEM_LIST_H_
#define _KITEM_LIST_H_

#include "SO3Result.h"
#include "KItem.h"
#include "KInventory.h"

#define TRADE_BOX_WIDTH		8
#define TRADE_BOX_HEIGHT	4

#define MONEY_LIMIT 2000000000

enum ITEM_LIST_STATE
{
	ilsInvalid = 0,

	ilsLocked,			//����״̬
	ilsOpened,			//����״̬

	ilsTotal
};

typedef struct 
{
	DWORD	dwBox;
	DWORD	dwX;
} TItemPos;

class KPlayer;
class KEquipInfo;
class KCustomEquipInfo;

class KItemList
{
public:
	int		m_nMoney;			//��Ǯ
	int		m_nMoneyLimit;		//��Ǯ����
public:
	KItemList(void);
	~KItemList(void);

	BOOL	Init(KPlayer* pPlayer);
	BOOL	UnInit();

	//���һ����Ʒ����Ҫ�Ƿ����ʹ�ã��Զ���������ҵ����õ�λ�á�
	BOOL	AddItem(KItem* pItem);

	//���һ����Ʒ����Ҫ�ǿͻ�ʹ�ã�ͬ��������װ��ֱ�ӷŵ�ָ��λ�á�
	BOOL	AddItem(KItem* pItem, DWORD dwBoxIndex, DWORD dwX);

	//����������Ʒ��λ��
	ITEM_RESULT_CODE	ExchangeItem(const TItemPos& Src, const TItemPos& Dest, DWORD dwAmount = 0);
	//������ɾ��һ����Ʒ��������Ϸ����ɾ����
	KItem*	DelItem(DWORD dwBoxIndex, DWORD dwX);
	//������Ʒ���ȴ�����ɾ����Ҳ����Ϸ����ɾ��
	ITEM_RESULT_CODE	DestoryItem(const TItemPos& Pos, BOOL bSync = TRUE);
	ITEM_RESULT_CODE	DestoryItem(KItem* pItem, BOOL bSync = TRUE);
	//���������е���������Ʒ
	KItem*	GetItem(DWORD dwBoxIndex, DWORD dwX);
	//����ָ����С�Ŀ��пռ�
	BOOL	FindFreeRoom(DWORD dwBoxIndex, DWORD& dwX);
	BOOL	FindFreeRoomInPackage(DWORD& dwBoxIndex, DWORD& dwX);
	BOOL	FindFreeSoldList(DWORD& dwX);
	DWORD	GetFreeRoomSize(void);

	DWORD	GetBoxSize(DWORD dwBox);
	//Ǯ��صķ���
	int		GetMoney() { return m_nMoney; };
	BOOL	SetMoney(int nMoney);
	BOOL	AddMoney(int nMoney);
	//Ǯ������صķ���
	int		GetMoneyLimit() { return m_nMoneyLimit; };
	BOOL	SetMoneyLimit(int nLimit);
	BOOL	AddMoneyLimit(int nLimit);
	//����
	BOOL	UnLock(DWORD dwPassword);
	//����
	BOOL	Lock(void);
	//��ȡ��Ʒ����
	DWORD	GetItemAmount(DWORD dwTabType, DWORD dwIndex);
	KItem*	FindItem(DWORD dwTabType, DWORD dwIndex, DWORD& dwBox, DWORD& dwX);
	//�ҵ�������Ʒ�ĵ�һ��λ�ã����û�У�����ʧ��
	BOOL	GetItemPos(int nVersion, DWORD dwTabType, DWORD dwIndex, DWORD &dwBox, DWORD &dwX);
	BOOL	GetItemPos(DWORD dwID, DWORD &dwBox, DWORD &dwX);
	int		GetBoxType(DWORD dwBox);
	//����ĳ����Ʒ�������ĳ�����ͬ������Ʒ��ֱ�������㹻Ϊֹ����������װ������Ʒ
	BOOL	CostItem(DWORD dwTabType, DWORD dwIndex, DWORD dwAmount);
	//��ȡĳ��ָ����Ʒ������
	BOOL	CostSingleItem(DWORD dwBoxIndex, DWORD dwX, DWORD dwAmount);
	//��ҩ���ģ����������Զ��
	BOOL	ConsumeAmmo(DWORD dwAmount);
	//��ָ�������
	BOOL	ReloadAmmo(DWORD dwTabType, DWORD dwIndex);

	//ʹ�õ���
	ITEM_RESULT_CODE	UseItem(TItemPos ItemPos, KTarget& rTarget);
	//�����Ʒ�Ƿ����װ��
	ITEM_RESULT_CODE CanEquip(KItem* pItem, DWORD dwX);
	ITEM_RESULT_CODE CanEquip(KEquipInfo* pItem, DWORD dwX);
	ITEM_RESULT_CODE CanEquip(KCustomEquipInfo* pItem, DWORD dwX);

	BOOL	CanPut(KItem* pItem, DWORD OrgBox, DWORD dwBox, DWORD dwX, BOOL bSrc);

	BOOL	AbradeArmor();
	BOOL	AbradeMeleeWeapon();
	BOOL	AbradeRangeWeapon();      
    //��װ������;öȵİٷֱ�, ĥ��һ�������е�����װ��(��Ҫ������Ҹ���ʱ,����ĳͷ�)
    BOOL    AbradeEquipment(int nBoxType, int nAbradePercent);
	BOOL	Repair(DWORD dwBox, DWORD dwX);

    BOOL    TidyUpSoldList();

    BOOL	OpenBox(DWORD dwBox);
	BOOL	CloseBox(DWORD dwBox);

	BOOL	ApplyAttrib(KItem* pItem);
	BOOL	UnApplyAttrib(KItem* pItem);

	char*	Load(char* pData);
	char*	Save(char* pData);
private:
	BOOL	UnEquip(KItem* pItem, DWORD dwX);
	BOOL	Equip(KItem* pItem, DWORD dwX);
	//���ȫ��װ���Ƿ񻹷�������
	BOOL	CheckEquipRequire(void);
	BOOL	PushItem(KItem* pItem, DWORD dwBox);
	BOOL	IsPackageSlot(DWORD dwBox, DWORD dwX);
	ITEM_RESULT_CODE	ExchangePackageSlot(DWORD dwBox1, DWORD dwBox2);
	ITEM_RESULT_CODE	EnablePackage(DWORD dwSize, int nContainType, DWORD dwBox);
	ITEM_RESULT_CODE	DisablePackage(DWORD dwBox);

private:
	KInventory			m_Box[ibTotal];
	DWORD				m_dwBoxCount;									//���ӵ��ܸ���

	ITEM_LIST_STATE		m_State;										//���ӵ�״̬
	DWORD				m_dwPassword;									//����
	KPlayer*			m_pPlayer;

private:
	//���̽ṹ����
	// ���߽ṹ
	enum ITEM_DB_STRUCT_TYPE
	{
		idst_Common = 0,
		idst_CustomEquip,
		idst_Equip,
		idst_Train,
		idst_Total,
	};

#pragma	pack(1)
	// �������͵�װ��ͬ�����������ṹ����
	struct KCommonItemDBData
	{
		BYTE	byVersion;
		BYTE	byBoxIndex;
		BYTE	byX;
		BYTE	byReserved1;
		BYTE	byReserved2;
		BYTE	byTabType;
		WORD	wTabIndex;
		time_t	nGenTime;
		WORD	wDurability;
	};

	struct KCustomEquipDBData : KCommonItemDBData
	{
		DWORD	dwRandSeed;
		WORD	wExtendMagic;
		WORD	wTemporaryMagic;
	};

	struct KRandEquipDBData : KCommonItemDBData
	{
		WORD	wMagic[MAX_ITEM_MAGIC_COUNT];
		WORD	wExtendMagic;
		WORD	wTemporaryMagic;
		DWORD	dwRandSeed;
		BYTE	byQuality;
	};

#pragma	pack()
};
#endif	//_KITEM_LIST_H_