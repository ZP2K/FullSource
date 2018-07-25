#ifndef _KG_GLOBAL_DEF_H_
#define _KG_GLOBAL_DEF_H_

// ���漸�����漰Game server��GameCenter�ϵ������ŷ���,�Լ��߼������ID�������
// ���е�GameServer�Ϸ����ID���4λǰ׺��GameCenter����,���ǰ׺����ֵӦ�õ���Game server��
// GameCenter�ϵ�������,������ֵ��Ϊ[1, 14]
// ǰ׺Ϊ0x0��IDΪ��ҽ�ɫID,����GameCenter�����0x0�����GameServer��Ϊ������(Ҳ��IDǰ׺��ֵ)
// ǰ׺Ϊ0xf��IDΪ�ͻ���ռ��,����GameCenterҲ�����0xf�����GameServer��Ϊ������(Ҳ��IDǰ׺��ֵ)
// ȥ��0x0, 0xf, �������һ�����������14��GameServer
#define ID_PREFIX_BIT_NUM	4
#define MAX_GAME_SERVER		14
#define PLAYER_ID_PREFIX	0x0
#define CLIENT_ID_PREFIX	0xf



//��������Ϸ����Ŀռ仮�ִ�С����
//�������ݶ���2���ݴ�
#define MAX_REGION_WIDTH_BIT_NUM	6	//��ͼ��X����������Region����
#define MAX_REGION_HEIGHT_BIT_NUM	6	//��ͼ��Y����������Region����

#define REGION_GRID_WIDTH_BIT_NUM	6	//Region��X����������Cell����
#define REGION_GRID_HEIGHT_BIT_NUM	6	//Region��Y����������Cell����

#define CELL_LENGTH_BIT_NUM			5	//�����е����ص㾫��
#define ALTITUDE_BIT_NUM			6

#define MAX_Z_ALTITUDE_BIT_NUM		16	//Z������ֵλ��(ALTITUDE)
#define MAX_Z_POINT_BIT_NUM         (MAX_Z_ALTITUDE_BIT_NUM + ALTITUDE_BIT_NUM)

#define MOVE_DEST_RANGE_BIT_NUM		12
#define MOVE_DEST_RANGE				(1 << MOVE_DEST_RANGE_BIT_NUM)

#define	MAX_VELOCITY_XY_BIT_NUM		(CELL_LENGTH_BIT_NUM + 2)
#define MAX_VELOCITY_Z_BIT_NUM		(ALTITUDE_BIT_NUM + 4)
// ����ˮƽ�ٶ���������ʱ�����������,ע��,���ֵ��ò�ҪС����������ʱ�ķ�ĸ��ֵ
#define  VELOCITY_ZOOM_BIT_NUM      4
#define  VELOCITY_ZOOM_COEFFICIENT (1 << VELOCITY_ZOOM_BIT_NUM)

#define MAX_ZOOM_VELOCITY_BIT_NUM   (MAX_VELOCITY_XY_BIT_NUM + VELOCITY_ZOOM_BIT_NUM)
#define MAX_ZOOM_VELOCITY           ((1 << MAX_ZOOM_VELOCITY_BIT_NUM) - 1)

#define MAX_VELOCITY_XY				((1 << MAX_VELOCITY_XY_BIT_NUM) - 1)
// ע��,Z���ٶ����з���,����Ϊ��ֵ��,Ŀǰȡֵ����Ϊ[-512, 511], 10 bits
#define MAX_VELOCITY_Z				((1 << (MAX_VELOCITY_Z_BIT_NUM - 1)) - 1)
#define MIN_VELOCITY_Z				(-(1 << (MAX_VELOCITY_Z_BIT_NUM - 1)))

#define MAX_X_COORDINATE_BIT        (MAX_REGION_WIDTH_BIT_NUM  + REGION_GRID_WIDTH_BIT_NUM  + CELL_LENGTH_BIT_NUM)
#define MAX_Y_COORDINATE_BIT        (MAX_REGION_HEIGHT_BIT_NUM + REGION_GRID_HEIGHT_BIT_NUM + CELL_LENGTH_BIT_NUM)
#define MAX_X_COORDINATE            ((1 << MAX_X_COORDINATE_BIT) - 1)
#define MAX_Y_COORDINATE            ((1 << MAX_Y_COORDINATE_BIT) - 1)

#define MAX_SYNC_PER_REGION_BIT_NUM	7	//ͬ�������Ʋ���


//������ض���
#define MAX_REGION_WIDTH			(1 << MAX_REGION_WIDTH_BIT_NUM)
#define MAX_REGION_HEIGHT			(1 << MAX_REGION_HEIGHT_BIT_NUM)
#define MAX_Z_ALTITUDE				((1 << MAX_Z_ALTITUDE_BIT_NUM) - 1)
#define MAX_Z_POINT				    ((1 << MAX_Z_POINT_BIT_NUM) - 1)

#define REGION_GRID_WIDTH			(1 << REGION_GRID_WIDTH_BIT_NUM)
#define REGION_GRID_HEIGHT			(1 << REGION_GRID_HEIGHT_BIT_NUM)

#define CELL_LENGTH					(1 << CELL_LENGTH_BIT_NUM)
#define POINT_PER_ALTITUDE			(1 << ALTITUDE_BIT_NUM)
#define ALTITUDE_2_CELL				(POINT_PER_ALTITUDE * LOGICAL_CELL_CM_LENGTH/ ALTITUDE_UNIT / CELL_LENGTH)

#define MAX_GRAVITY_BIT_NUM          5
#define MAX_GRAVITY                 ((1 << MAX_GRAVITY_BIT_NUM) - 1)

#define LOGICAL_CELL_CM_LENGTH		50
#define _3D_CELL_CM_LENGTH			100

// �ر�߶ȼ�����λ(����)
#define ALTITUDE_UNIT				((LOGICAL_CELL_CM_LENGTH) / 4.0f)
#define TERRAIN_MIN_HEIGHT			(-65536.0f / 4.0f * ALTITUDE_UNIT)
#define TERRAIN_MAX_HEIGHT			(65536.0f / 4.0f * ALTITUDE_UNIT)

#define ALTITUDE_TO_XYPOINT(Altitude)   ((Altitude) * 8)
#define ZPOINT_TO_XYPOINT(Z)            ((Z) / 8)
#define XYPOINT_TO_ZPOINT(XY)           ((XY) * 8)
#define ZPOINT_TO_ALTITUDE(Z)           ((Z) / POINT_PER_ALTITUDE)

#define AI_PARAM_COUNT		8

#define DIALOG_RANGE		(6 * CELL_LENGTH)	 //���ԶԻ��ľ���
#define DIALOG_ACTION_ID	1					//�Ի��ı���ID

#define	MAX_TARGET_RANGE		(CELL_LENGTH * REGION_GRID_WIDTH * 2)
#define MAX_LOOT_RANGE			(CELL_LENGTH * REGION_GRID_WIDTH * 2)

// �����ϰ��߶Ȳ�
#define CLIMB_ABILITY 	    (XYPOINT_TO_ZPOINT(CELL_LENGTH))

// �����ƶ�ʱ��λǰ��Ŀ���ľ���,30��
#define MOVE_TARGET_RANGE   (30 * 100 * 32 / 50)

#define MAX_MOVE_STATE_BIT_NUM  4
#define MAX_MOVE_STATE_VALUE    ((1 << MAX_MOVE_STATE_BIT_NUM) - 1)

enum
{
	REPRESENT_EQUIP_MELEE_WEAPON,
	REPRESENT_EQUIP_HELM,
	REPRESENT_EQUIP_CHEST,
	REPRESENT_EQUIP_WAIST,
	REPRESENT_EQUIP_PANTS,
	REPRESENT_EQUIP_BANGLE,
	REPRESENT_EQUIP_EXTEND,

	REPRESENT_FACE,				//����װ�����������ͣ����ͣ���������װ

	REPRESENT_COUNT				//��Ҫ��ʾ��װ������
};



// AI�������ֻ����������
// ���Ҫ�м�����ɾ��,С����Щ����ָ���������
enum AI_TYPE
{
	aitInvalid = 0,

	aitBase,
	aitPlayer,
	aitWolf,

	aitTotal
};

// AI����Ϊ״̬
enum KAI_STATE
{
	aisInvalid = 0,

	aisIdle,        // ����,ͨ��ָ����ĳĿ���Ŀ���
	aisWander,		// �й�
	aisPatrol,      // Ѳ�ߵ�ĳ����Ĺ�����
	aisFollow,      // ����
	aisAlert,       // ����Ŀ��,��������aisPatrol�з���һ��Ŀ��,Ȼ������,Ҳ������Idle�Ƿ���Ŀ��,Ȼ������
	aisPursuit,     // ׷��
	aisKeepAway,    // ����
	aisEscape,      // ����
	aisReturn,      // Attack����Ժ󷵻�
};

enum AI_SKILL_TYPE
{
	aistInvalid = 0,

	aistDamage,
	aistHeal,
	aistDot,
	aistHot,
	aistBuff,
	aistDebuf,
	aistControl,
	aistStun,
	aistSpecial1,
	aistSpecial2,

	aistTotal
};

//��ɫ����
enum ROLE_TYPE
{
	rtInvalid = 0,

	rtStandardMale,     // ��׼��
	rtStandardFemale,   // ��׼Ů
    rtStrongMale,       // ������
    rtSexyFemale,       // �Ը�Ů
    rtLittleBoy,        // С�к�
    rtLittleGirl,       // С��Ů

	rtTotal
};

enum KBUFF_REPRESENT_POS
{
    brpInvalid,
    brpTrample,     // ���²��ŵ�
    brpBody,        // ���ϵ�
    brpTopHead,     // ����ͷ�ϵ�
    brpTotal
};


//��ɫ��������
enum CHARACTER_ACTION_TYPE
{
	catInvalid = 0,

	catDialogue,		//�Ի�
	catHowl,			//����

	catTotal
};

//������ض���
enum FORCE_RELATION_TYPE
{
	frtInvalid = 0,

	frtExalted,
	frtRevered,
	frtHonored,
	frtFriendly,

	frtNeutral,

	frtUnfriendly,
	frtHostile,
	frtHated,

	frtTotal
};

enum KCAMP
{
    cNeutral, // ������Ӫ
    cGood,    // ������Ӫ
    cEvil     // а����Ӫ
};

enum SCENE_OBJ_RELATION_TYPE
{
	sortInvalid = 0,

	sortNone	= 1,
	sortSelf	= 2,
	sortAlly	= 4,
	sortEnemy	= 8,
	sortDialog	= 16,
	sortParty	= 32,

	sortAll	= sortNone | sortAlly | sortEnemy | sortSelf | sortDialog | sortParty,

	sortTotal,
};

struct KForceRelation
{
	int						nRelation;
	FORCE_RELATION_TYPE		eRelationType;
};

// �������
#define MAX_ITEM_BASE_COUNT		6
#define	MAX_ITEM_REQUIRE_COUNT	6
#define	MAX_ITEM_MAGIC_COUNT	6


//�����ļ�·������׺
#define SETTING_DIR			"Settings"	//�����ļ�Ŀ¼

#define MAP_LIST_FILE_NAME	"MapList.tab"

//�ű��ص�������
#define SCRIPT_ON_LOAD		"OnLoad"

//��ͼ�ϰ��ͳ�������
#define MAP_DIR				"maps"			//Ŀ¼
#define MAP_FILE_EXT		"map"			//��׺
#define OBJ_FILE_EXT		"obj"			//��׺

//�ַ������ȶ���
#define TITLE_LEN			32
#define TASK_NAME_LEN		256
#define FUNC_NAME_LEN		32
#define MAX_CHAT_TEXT_LEN	768

//��չ��
#define MAX_EXT_POINT_COUNT   8

//������ض���
#define MAX_FORCE_COUNT			64
#define FORCE_NAME_LEN			32

//��ͼ����趨
#define MAX_MAP_INDEX				256			//����ͼ���

#define MAP_MAP_ID					"MapID"
#define MAP_MAP_NAME				"MapName"
#define MAP_DROP_NAME				"MapDrop"
#define MAP_MAP_TYPE				"MapType"
#define MAP_INIT_GROUP				"InitGroup"
#define MAP_MAP_PERFORM				"MapPerform"
#define MAP_RESOURCE_FILE_PATH		"ResourceFilePath"
#define MAP_NPC_RELIVE_TAB_NAME		"NpcReliveList"
#define MAP_DOODAD_RELIVE_TAB_NAME	"DoodadReliveList"
#define MAP_NPC_TEAM_TAB_NAME		"NpcTeamList"
#define MAP_PATROL_PATH_TAB_NAME	"PatrolPathList"
#define MAP_REVIVE_IN_SITU          "ReviveInSitu"

//����
#define MAX_EXTERNAL_PACKAGE_SIZE	    1000		//����������ݰ��Ĵ�С
#define MAX_EXTERNAL_PACKAGE_SIZE_ALIGN	1024		//����������ݰ��Ĵ�С�Ķ�������,�������

#define _PERFORMANCE_OPTIMIZATION
#ifdef _PERFORMANCE_OPTIMIZATION
    extern int g_nGameFps;
    #define GAME_FPS					g_nGameFps			//��Ϸ֡��
#else
    #define GAME_FPS                    16
#endif


// �����ض���
// PARTY: ��ӵĶ���
#define INVALID_PARTY_ID    ((DWORD)(0x7fff0000))
// ������Ϸ������������Ķ�����
#define MAX_PARTY_COUNT     4096
// һ֧�����������������Ŀ
#define MAX_PARTY_SIZE      8

#define INVALID_SKILL_ID    0

// �������Ʒ����ģʽ
enum PARTY_LOOT_MODE
{
    plmInvalid = 0,

	plmAbsoluteFree,	// ��������
	plmFreeForAll,		// ����ʰȡ
    plmRoundRobin,		// ����ʰȡ
	plmLeaderLoot,		// �ӳ�����
	plmGroupLoot,		// ����ʰȡ

	plmTotal           
};

enum INVENTORY_TYPE
{
	ivtEquipment = 0,
	ivtPackage,
	ivtBank,
	ivtSlot,
	ivtTrade,
	ivtSoldlist,
	ivtTotal,
};

enum EQUIPMENT_INVENTORY_TYPE
{
	eitMeleeWeapon = 0,		//��ս����
	eitRangeWeapon,			//Զ������
	eitChest,				//����
	eitHelm,				//ͷ��
	eitAmulet,				//����
	eitLeftRing,			//���ֽ�ָ
	eitRightRing,			//���ֽ�ָ
	eitWaist,				//����
	eitPendant,				//��׺
	eitPants,				//����
	eitBoots,				//Ь��
	eitBangle,				//����
	eitExtend,				//��չ��װ�����ƺ�֮���
	eitPackage1,			//��չ����1
	eitPackage2,			//��չ����2
	eitPackage3,			//��չ����3
	eitPackage4,			//��չ����4
	eitArrow,				//����
	eitTotal
};

// ----------------- TALK ---------------------------->
#define CLIENT_TALK_LINK_ITEM_BUFFER_SIZE 1024

#define TALK_ERROR_S_OK                 0
#define TALK_ERROR_E_ERROR              1
#define TALK_ERROR_E_OUT_OF_BUFFER      2
#define TALK_ERROR_E_NOT_IN_PARTY       3
#define TALK_ERROR_E_NOT_IN_SENCE       4
#define TALK_ERROR_E_PLAYER_NOT_FOUND   5

enum TALK_DATA_BLOCK_TYPE
{
    tdbInvalid = 0,
    tdbText,
    tdbLinkName,
    tdbLinkItem,

    tdbTotal
};

// Ⱥ�ķ�Χ
enum PLAYER_TALK_RANGE
{
    trInvalid = 0,
    trNearby,       // ����
    trParty,        // С������
    trSence,        // ͬ�������죨���ģ�
    trSystem,       // ϵͳ��Ϣ
    trWhisper,      // ����
    trWhisperReply,

    trCustomCh1 = 100,
    trCustomCh2,
    trCustomCh3,
    trCustomCh4,
    trCustomCh5,
    trCustomCh6,
    trCustomCh7,
    trCustomCh8,

    trTotal
};
// <---------------------------------------------------

#ifdef WIN32
#define STR_CASE_CMP(szA, szB) _mbsicmp((unsigned char*)(szA), (unsigned char*)(szB))
#else   // linux
#define STR_CASE_CMP(szA, szB) strcasecmp(szA, szB)
#endif

#endif	//_KG_GLOBAL_DEF_H_