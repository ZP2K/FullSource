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
#include "KEnchantLib.h"

// ���ߵ�����
enum ITEM_GENRE
{
	igEquipment = 0,	// װ��
	igPotion,			// ҩƷ
	igTaskItem,			// �������
	igMaterial,			// ����
	igBook,				// �鼮
	igTotal
};

// װ���Ĵ��·���
enum EQUIPMENT_SUB_TYPE
{
	estMeleeWeapon = 0,		// ��ս����
	estRangeWeapon,			// Զ������
	estChest,				// ����
	estHelm,				// ͷ��
	estAmulet,				// ����
	estRing,				// ��ָ
	estWaist,				// ����
	estPendant,				// ��׺
	estPants,				// ����
	estBoots,				// Ь��
	estBangle,				// ����
	estWaistExtend,		    // ������չ��װ�����ƺ�֮���
	estPackage,				// ����
	estArrow,				// ����
    estBackExtend,          // ������չ
    estHorse,               // ����
	estTotal
};

// �����ľ������
enum WEAPON_DETAIL_TYPE
{
    wdtWand = 0,	    // ��
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

// ����Ʒ��
enum ITEM_QUALITY
{
	iqLow = 0,				// ��Ʒ
	iqNormal,				// ��ͨװ������ɫ
	iqGood,					// ����
	iqTotal
};

// ������
enum ITEM_BIND_TYPE
{
	ibtInvalid = 0,
	ibtNeverBind,
	ibtBindOnEquipped,
	ibtBindOnPicked,
	ibtTotal
};

enum ITEM_USE_TARGET_TYPE
{
    iuttInvalid = 0,

    iuttNoTarget,
    iuttSelf,
    iuttOtherPlayer,
    iuttNpc,
    iuttCoordinates,
    iuttDoodad,
    iuttItem,

    iuttTotal
};

struct COMMON_ATTRIBUTE
{
	char				szItemName[ITEM_NAME_LEN];	// ����
	int					nLevel;						// �ȼ�
	int					nPrice;						// �۸�
	int					nGenre;						// ����
	int					nSub;						// С��
	int					nDetail;					// ϸ��
	BOOL				bCanTrade;					// �Ƿ������
	BOOL				bCanDestroy;				// �Ƿ�ɴݻ�	TODO�� --spe
    int					nMaxExistAmount;			// ����������

	BOOL				bConsumeDurabiltity;		// �Ƿ������;�
	BOOL				bStack;						// �Ƿ��ܹ�����
	int					nBindType;					// ������
	int					nAbradeRate;				// ��ĥ�����
	DWORD				dwSetID;					// ��װ��
};

struct GENERATE_PARAM
{
	int					nVersion;
	DWORD				dwTabType;
	DWORD				dwIndex;
	time_t				nGenTime;
	DWORD				dwRandSeed;
	int					nQuality;					// ����Ʒ�� (��ͨ? �ƽ�? )
	int					nMagicIndex[MAX_ITEM_MAGIC_COUNT];
	DWORD               dwPermanentEnchantID;
	DWORD               dwTemporaryEnchantID;
};

class KItem : public KBaseObject
{
public:
	KItem();
	~KItem();

	BOOL Init();
	void UnInit();

	GENERATE_PARAM		m_GenParam;				// ������Ҫ�Ĳ���
	COMMON_ATTRIBUTE	m_Common;				// һ������
	DWORD				m_dwSkillID;			// ����ID
	DWORD				m_dwSkillLevel;			// ���ܵȼ�

	KAttribute*			m_pBaseAttr;			// ��������
	KAttribute*			m_pRequireAttr;			// ��������
	KAttribute*			m_pMagicAttr;			// ħ������

	KENCHANT*			m_pPermanentEnchant;	// ��ħ����
	KENCHANT*			m_pTemporaryEnchant;	// ��ʱ��ħ
	time_t				m_nEnchantTime;         // ��ʱ��ħ����ʱ��

	DWORD				m_dwScriptID;			// �ű�ID

	union
	{
		int				m_nCurrentDurability;	// ��ǰ�;ö�
		int				m_nStackNum;			// ��ǰ��������
	};
	union
	{
		int				m_nMaxDurability;		// ����;ö�(ĳЩװ������ֵ�ᶯ̬�仯)
		int				m_nMaxStackNum;			// ����������
		int				m_nPackageSize;			// ������������
	};

	BOOL				m_bBind;				// �Ƿ񱻰�
	int					m_nRepresentID;			// �����߼�ID
    int                 m_nColorID;             // ƫɫid
	BOOL				m_bApplyed;				// �Ƿ���Ӧ������
	BOOL				m_bEquiped;				// �Ƿ��Ѿ�װ��

#if defined(_CLIENT)
	int					m_nUiId;				// ����ID
#endif	//_CLIENT

public:
	BOOL				SetStackNum(int nStackNum);
	int					GetStackNum();
	int					GetMaxStackNum();
	int					GetMaxExistAmount();				// ��ȡ��Ʒ����������,0��������
	int					GetMaxExistTime();					// ���ߺ���Ʒ�Ĵ���ʱ��,0��������
	int					Stack(int nCount);
    BOOL                SetCurrentDurability(int nDurability);
    int                 GetCurrentDurability();
    int                 GetMaxDurability();
	int					GetGenre() { return m_Common.nGenre; };
	int					GetPrice() { return m_Common.nPrice; };
	BOOL				IsBind() { return m_bBind; };
	BOOL				IsStackable();
	BOOL				IsRepairable();
	BOOL				IsCanTrade() { return m_Common.bCanTrade; };
	BOOL				IsCanDestroy() { return m_Common.bCanDestroy; };
	BOOL				CanStack(KItem* pDest);
	BOOL				Repair();
	BOOL				Abrade();

    BOOL                GetBinaryData(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL                CheckCoolDown(KPlayer* pPlayer);
#ifdef _SERVER
    BOOL                ResetCoolDown(KPlayer* pPlayer);
#endif
    int                 GetRequireLevel();
    KItemInfo*          GetItemInfo();

private:
    BOOL                HaveDurability();
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
	DECLARE_LUA_STRUCT_BOOL(CanDestroy, m_Common.bCanDestroy);
	DECLARE_LUA_STRUCT_BOOL(CanStack, m_Common.bStack);
	DECLARE_LUA_STRUCT_BOOL(CanConsume, m_Common.bConsumeDurabiltity);
	DECLARE_LUA_STRUCT_DWORD(TabType, m_GenParam.dwTabType);
	DECLARE_LUA_STRUCT_DWORD(Index, m_GenParam.dwIndex);
	DECLARE_LUA_STRUCT_INTEGER(MaxExistAmount, m_Common.nMaxExistAmount);
	DECLARE_LUA_STRUCT_INTEGER(Quality, m_GenParam.nQuality)

	DECLARE_LUA_STRUCT_INTEGER(Version, m_GenParam.nVersion);

	DECLARE_LUA_INTEGER(CurrentDurability);
	DECLARE_LUA_INTEGER(StackNum);
	DECLARE_LUA_INTEGER(MaxDurability);
	DECLARE_LUA_INTEGER(MaxStackNum);

	DECLARE_LUA_BOOL(Bind);
	DECLARE_LUA_INTEGER(RepresentID);
    DECLARE_LUA_INTEGER(ColorID);

#ifdef _CLIENT
	DECLARE_LUA_INTEGER(UiId);

	int LuaGetBaseAttrib(Lua_State* L);
	int LuaGetRequireAttrib(Lua_State* L);
	int LuaGetMagicAttrib(Lua_State* L);
	int LuaGetPermanentEnchantAttrib(Lua_State* L);
	int LuaGetTemporaryEnchantAttrib(Lua_State* L);
	int	LuaGetSetAttrib(Lua_State* L);
	int LuaCanEquip(Lua_State* L);
#endif	//_CLIENT
    int LuaIsRepairable(Lua_State* L);

	int LuaGetUseSkill(Lua_State* L);
	int LuaHasScript(Lua_State* L);
    int LuaGetRequireLevel(Lua_State* L);
};

#pragma	pack(1)
struct KCOMMON_ITEM_DATA
{
    BYTE	byVersion : 7;
    BYTE    byBind    : 1;
    BYTE	byTabType;
	WORD	wTabIndex;
	WORD	wDurability;
    time_t	nGenTime;
};

struct KCUSTOM_EQUI_DATA : KCOMMON_ITEM_DATA
{
	DWORD	dwRandSeed;
	WORD	wPermanentEnchantID;
	WORD    wTemporaryEnchantID;
    WORD    wLeftEnchantTime;
    BYTE    byColorID;
};

struct KTRAIN_EQUIP_DATA : KCUSTOM_EQUI_DATA
{
    DWORD	dwMadeBy;
};
#pragma	pack()

#endif	//_KITEM_H_
