////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KItemDef.h
//  Version     : 1.0
//  Creator     : Chen Jie, Xia Yong
//  Comment     : ����ϵͳ
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _KITEM_DEF_H_
#define _KITEM_DEF_H_

#define MAX_ITEM_DATA_SIZE sizeof(KCUSTOM_EQUI_DATA)

#pragma	pack(1)
enum ENCHANT_INDEX
{
    eiPermanentEnchant = 0,
    eiTemporaryEnchant,

    eiMount1,
    eiMount2,
    eiMount3,
    eiMount4,

    eiTotal
};

struct KCOMMON_ITEM_DATA 
{
    BYTE    byVersion;
    BYTE    byTabType;
    WORD    wTabIndex;
    BYTE    byBind;
    WORD    wDurability;
    time_t	nGenTime;
};

struct KCUSTOM_EQUI_DATA : KCOMMON_ITEM_DATA
{
    DWORD   dwRandSeed;
    WORD    wEnchant[eiTotal];
    WORD    wLeftEnchantTime;
    BYTE    byColorID;
};

struct KITEM_DB_HEADER 
{
    BYTE byBox;
    BYTE byPos;
    BYTE byDataLen;
    BYTE byData[0];
};
#pragma	pack()

#define CURRENT_ITEM_DATA_VERSION 0

// ���ߵ�����
enum ITEM_GENRE
{
    igEquipment = 0,	// װ��
    igPotion,			// ҩƷ
    igTaskItem,			// �������
    igMaterial,			// ����
    igBook,				// �鼮
    igDesignation,      // �ƺŵ���
    igMountItem,        // ��Ƕ����
    igEnchantItem,      // ��ħ����

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

// ��Ƕ���ߵ�����, ��װ�����ౣ��һ��
enum ENCHANT_SUB_TYPE
{
    nstMeleeWeapon = estMeleeWeapon,    // ��ս����
    nstRangeWeapon = estRangeWeapon,    // Զ������
    nstHorse       = estHorse           // ���
};

// ��ߵ�ϸ��
enum ENCHANT_DETAIL_TYPE
{
    edtHead,            // ͷ��
    edtChest,           // ����
    edtFoot,            // ����
    edtHangItem,        // �Ҽ�

    edtTotal
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
    ibtBindOnTimeLimitation,
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

#endif //_KITEM_DEF_H_
