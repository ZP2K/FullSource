/************************************************************************/
/* ����			                                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.31	Create												*/
/************************************************************************/
#ifndef _KITEM_H_
#define _KITEM_H_

#include "Global.h"
#include "KBaseObject.h"
#include "KAttribute.h"
#include "KItemInfoList.h"
#include "Luna.h"

//���ߵ�����
enum ITEM_GENRE
{
	igEquipment = 0,	//װ��
	igPotion,			//ҩƷ
	igTaskItem,			//�������
	igMaterial,			//����
	igTotal
};

//װ���Ĵ��·���
enum EQUIPMENT_SUB_TYPE
{
	estMeleeWeapon = 0,		//��ս����
	estRangeWeapon,			//Զ������
	estChest,				//����
	estHelm,				//ͷ��
	estAmulet,				//����
	estRing,				//��ָ
	estWaist,				//����
	estPendant,				//��׺
	estPants,				//����
	estBoots,				//Ь��
	estBangle,				//����
	estExtend,				//��չ��װ�����ƺ�֮���
	estPackage,				//����
	estArrow,				//����
	estTotal
};

//�����ľ������
enum WEAPON_DETAIL_TYPE
{
    wdtWand = 0,        // ��
    wdtSpear,           // ǹ
    wdtSword,           // ��
    wdtFist,            // ȭ��
    wdtDoubleWeapon,    // ˫������,����: ˫��,˫ɡ
    wdtPen,             // ��
    wdtSlingShot,       // ����
    wdtDart,            // ����
    wdtMachDart,        // ���ذ���
	wdtTotal
};

//����Ʒ��
enum ITEM_QUALITY
{
	iqLow = 0,				//��Ʒ
	iqNormal,				//��ͨװ������ɫ
	iqGood,					//����
	iqTotal
};

//������
enum ITEM_BIND_TYPE
{
	ibtInvalid = 0,
	ibtNeverBind,
	ibtBindOnEquipped,
	ibtBindOnPicked,
	ibtTotal
};

enum USE_ITEM_SKILL_TYPE
{
	uistInvalid = 0,

	uistCannotUse,
	uistUseInPackage,
	uistUseWhenEquipped,

	uistTotal
};

typedef struct _COMMON_ATTRIBUTE
{
	char				szItemName[ITEM_NAME_LEN];	// ����
	int					nLevel;						// �ȼ�
	int					nPrice;						// �۸�
	int					nGenre;						// ����
	int					nSub;						// С��
	int					nDetail;					// ϸ��
	int					nBindType;					// ������
	BOOL				bCanTrade;					// �Ƿ������
	BOOL				bCanDestory;				// �Ƿ�ɴݻ�	TODO�� --spe
	BOOL				bConsumeDurabiltity;		// �Ƿ������;�
	BOOL				bStack;						// �Ƿ��ܹ�����
	int					nAbradeRate;				// ��ĥ�����
} COMMON_ATTRIBUTE;

typedef struct _GENERATE_PARAM
{
	int					nVersion;
	DWORD				dwTabType;
	DWORD				dwIndex;
	time_t				nGenTime;
	DWORD				dwRandSeed;
	int					nQuality;					// ����Ʒ�� (��ͨ? �ƽ�? )
	int					nMagicIndex[MAX_ITEM_MAGIC_COUNT];
} GENERATE_PARAM;

class KItem : public KBaseObject
{
//��ֵ����
public:
	KItem(void);
	~KItem(void);

	BOOL Init(void);
	BOOL UnInit(void);

	GENERATE_PARAM		m_GenParam;				//������Ҫ�Ĳ���
	COMMON_ATTRIBUTE	m_Common;				//һ������

	USE_ITEM_SKILL_TYPE	m_eUseType;				//ʹ����Ʒ���ܵ�����
	DWORD				m_dwSkillID;			//����ID
	DWORD				m_dwSkillLevel;			//���ܵȼ�

	KAttribute*			m_pBaseAttr;			//��������
	KAttribute*			m_pRequireAttr;			//��������
	KAttribute*			m_pMagicAttr;			//ħ������
	KAttribute*			m_pEnchant;				//��ħ����
	KAttribute*			m_pTemporaryEnchant;	//��ʱ��ħ
	int					m_nTemporaryEnchantTime;//��ʱ��ħ����ʱ��

	DWORD				m_dwScriptID;			//�ű�ID

	union
	{
		int				m_nCurrentDurability;	//��ǰ�;ö�
		int				m_nStackNum;			//��ǰ��������
	};
	union
	{
		int				m_nMaxDurability;		//����;ö�(ĳЩװ������ֵ�ᶯ̬�仯)
		int				m_nMaxStackNum;			//����������
		int				m_nPackageSize;			//������������
	};

	BOOL				m_bBound;				//�Ƿ񱻰�
	int					m_nRepresentId;			//�����߼�ID
	BOOL				m_bApplyed;				//�Ƿ���Ӧ������
	BOOL				m_bEquiped;				//�Ƿ��Ѿ�װ��

#if defined(_CLIENT)
	int					m_nUiId;				//����ID
#endif	//_CLIENT

public:
	BOOL				SetStackNum(int nStackNum);
	int					GetStackNum();
	int					GetMaxStackNum();
	int					GetMaxExistAmount();				//��ȡ��Ʒ����������,0��������
	int					GetMaxExistTime();					//���ߺ���Ʒ�Ĵ���ʱ��,0��������
	int					Stack(int nCount);
    BOOL                SetCurrentDurability(int nDurability);
    int                 GetCurrentDurability();
    int                 GetMaxDurability();
	int					GetGenre() { return m_Common.nGenre; };
	int					GetPrice() { return m_Common.nPrice; };
	BOOL				IsBound() { return m_bBound; };
	BOOL				IsStackable();
	BOOL				IsRepairable();
	BOOL				IsCanTrade() { return m_Common.bCanTrade; };
	BOOL				IsCanDestory() { return m_Common.bCanDestory; };
	BOOL				CanStack(KItem* pDest);
	int					Repair();
	BOOL				Abrade();

public:
	//Lua�ű��ӿ�
	DECLARE_LUA_CLASS(KItem);

	DECLARE_LUA_STRUCT_STRING(Name, ITEM_NAME_LEN, m_Common.szItemName);
	DECLARE_LUA_STRUCT_INTEGER(Level, m_Common.nLevel);
	DECLARE_LUA_STRUCT_INTEGER(Price, m_Common.nPrice);
	DECLARE_LUA_STRUCT_INTEGER(Genre, m_Common.nGenre);
	DECLARE_LUA_STRUCT_INTEGER(Sub, m_Common.nSub);
	DECLARE_LUA_STRUCT_INTEGER(Detail, m_Common.nDetail);
	DECLARE_LUA_STRUCT_INTEGER(BindType, m_Common.nBindType);
	DECLARE_LUA_STRUCT_BOOL(CanTrade, m_Common.bCanTrade);
	DECLARE_LUA_STRUCT_BOOL(CanDestory, m_Common.bCanDestory);
	DECLARE_LUA_STRUCT_BOOL(CanStack, m_Common.bStack);
	DECLARE_LUA_STRUCT_BOOL(CanConsume, m_Common.bConsumeDurabiltity);
	DECLARE_LUA_STRUCT_DWORD(TabType, m_GenParam.dwTabType);
	DECLARE_LUA_STRUCT_DWORD(Index, m_GenParam.dwIndex);
	DECLARE_LUA_STRUCT_INTEGER(Quality, m_GenParam.nQuality)

	DECLARE_LUA_STRUCT_INTEGER(Version, m_GenParam.nVersion);

	DECLARE_LUA_INTEGER(CurrentDurability);
	DECLARE_LUA_INTEGER(StackNum);
	DECLARE_LUA_INTEGER(MaxDurability);
	DECLARE_LUA_INTEGER(MaxStackNum);

	DECLARE_LUA_BOOL(Bound);
	DECLARE_LUA_INTEGER(RepresentId);
#if defined(_CLIENT)
	DECLARE_LUA_INTEGER(UiId);
#endif	//_CLIENT
	int LuaGetBaseAttrib(Lua_State* L);
	int LuaGetRequireAttrib(Lua_State* L);
	int LuaGetMagicAttrib(Lua_State* L);
	int LuaGetUseSkill(Lua_State* L);
	int LuaHasScript(Lua_State* L);
};
#endif	//_KITEM_H_