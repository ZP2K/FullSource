/************************************************************************/
/* ȫ��������							                                */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* Comment	 : �˴���������SO3World��ʹ�õĳ�����һЩ������Ҫ���õĳ��� */
/*			   ����������KG_GlobalDef.h�У�����Project�������		*/
/* History	 :															*/
/*		2005.01.12	Create												*/
/************************************************************************/
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "SO3GlobalDef.h"
#include "ProtocolBasic.h"

#define DIRECTION_BIT_NUM       8
#define DIRECTION_COUNT			(1 << DIRECTION_BIT_NUM)

#define	CURRENT_ITEM_VERSION	0
#define	ITEM_SETTING_DIR	SETTING_DIR"/item"
#define	NORMAL_WEAPON_FILE	"Weapon.tab"
#define	NORMAL_ARMOR_FILE	"Armor.tab"
#define NORMAL_TRINKET_FILE	"Trinket.tab"
#define CUST_WEAPON_FILE	"Custom_Weapon.tab"
#define	CUST_ARMOR_FILE		"Custom_Armor.tab"
#define	CUST_TRINKET_FILE	"Custom_Trinket.tab"
#define	ATTRIB_FILE			"Attrib.tab"
#define	COMMON_ITEM_FILE	"Other.tab"

//���ߵ��������
#define DROP_LIST_DIR		"DropList"
#define DOODAD_DROP_LIST_DIR "DoodadDropList"
#define MONEY_DROP_FILE		"ClassDrop\\MoneyDrop.tab"

#define MAX_DROP_PER_NPC	4
#define MAX_DROP_PER_DOODAD	4
#define MAX_LOOT_SIZE		32

#define CUSTOM_OTACTION_NAME_LEN 16

#define SCRIPT_DIR		"Scripts"

//Doodad�ϰ���Ϣ���ñ�
#define OBSTACLE_DIR	"Obstacle"

//����INT64
#define MAKE_INT64(__A__, __B__)	(((unsigned __int64)(__A__) << 32) + (__int64)__B__)

#define ERROR_ID		0


//�ű��ص�������
#define SCRIPT_ON_ENTER_TRAP	"OnEnterTrap"		//Param1:player;Param2:cell
#define SCRIPT_ON_LEAVE_TRAP	"OnLeaveTrap"		//Param1:player;Param2:old cell

#define SCRIPT_ON_ENTER_SCENE	"OnEnterScene"		//Param1:player;Param2:DestScene
#define SCRIPT_ON_LEAVE_SCENE	"OnLeaveScene"		//Param1:player;Param2:SrcScene

#define SCRIPT_ON_USE_ITEM		"OnUse"				//Param1:player;Param2:item

#define SCRIPT_ON_OPEN_DOODAD	"OnOpen"			//Param1:doodad;Param2:player
#define SCRIPT_ON_BREAK_OPEN	"OnBreak"			//Param1:doodad;Param2:player
#define SCRIPT_ON_PICK_DOODAD	"OnPick"			//Param1:doodad;Param2:player

#define SCRIPT_ON_DIALOGUE		"OnDialogue"		//Param1:npc;	Param2:player
#define SCRIPT_ON_NPC_DEATH		"OnDeath"			//Param1:npc;	Param3:killer

#define SCRIPT_ON_ACCEPT_QUEST	"OnAcceptQuest"		//Param1:player		
#define SCRIPT_ON_FINISH_QUEST	"OnFinishQuest"		//Param1:player
#define SCRIPT_ON_CANCEL_QUEST	"OnCancelQuest"		//Param1:player

#define SCRIPT_ON_CUSTOM_EVENT	"OnCustomEvent"		//Param1:player

//AI�ýű��ص�������
#define SCRIPT_ON_PATROL		"OnPatrol"			//Param1:npc

//������ʱ���Ļص�������
#define SCRIPT_ON_TIMER			"OnTimer"			//Param1:int Param2:int


#define MAX_WINDOW_SELECT_COUNT	32					//�ͻ��˴���ѡ�����
#define MAX_SELECT_FUNCNAME_LEN	32					//ѡ��ص�����������

//�������������
#define DICE_MAX				100
#define DICE_MIN				1

//�������
//#define MAX_SKILL_COUNT			1024

//�������
#define	MAX_ITEM_COUNT			1024
#define	MAX_ATTRIB_COUNT		4096

//AI���
#define MAX_AI_EVENT 6
#define INIT_AI_EVENT_FUNC_NAME "InitAIEvent"

#define AI_RETURN_OVERTIME_FRAME 160

//�ַ������ȶ���
#define ITEM_NAME_LEN			64
#define QUEST_NAME_LEN			32
#define QUEST_LINK_NAME_LEN		32

//Region���
#define CONNECTED_REGION_TOTAL	8

#define MAX_PLAYER_LEVEL		100

//NPC AIӵ�еļ�������
#define MAX_NPC_AI_SKILL		4

#define MAX_OPEN_WINDOW_TEXT_LEN 1000

enum DOODAD_KIND
{
	dkInvalid = 0,

	dkNormal,					//��ͨ��Doodad,�����˶��ܴ�
	dkCorpse,					//ʬ��
	dkQuest,					//������ص�Doodad
	dkTreasure,					//����
	dkOrnament,					//װ����,���ܲ���
	dkCraftTarget,				//����ܵĲɼ���
	dkClientOnly,

	dkTotal
};

//��ɫ���ͣ����ڸ�ö��ֵ���̣������Ͻ�����˳��ֻ��׷�ӣ�������
enum CHARACTER_KIND
{
	ckInvalid = 0,

	ckPlayer,
	ckFightNpc,
	ckDialogNpc,
	ckCannotOperateNpc,	//���ܱ�������������Npc
	ckClientNpc,

	ckTotal
};

enum PATROL_REPEAT_STYLE
{
	prsInvalid = 0,

	prsNoRepeat,
	prsCircle,
	prsLine,

	prsTotal
};

//��ɫ���Ա�
enum CHARACTER_GENDER
{
	cgInvalid = 0,

	cgMale,
	cgFemale,
	cgGay,
	cgLibido,

	cgTotal
};

//��ɫ����
enum CHARACTER_RACE
{
	crInvalid = 0,

	crNone,				//������
	crHuman,			//����
	crAnimal,			//����
	crRobot,			//��е
	crMonster,			//����
	crHolyBeast,		//����	ע��HolyBeast�ɺ���ԭ��

	crTotal
};

//����״̬���
enum QUEST_MARK
{
	qmInvalid = -1,

	qmNoMark = 0,
	qmHide = 1,
	qmWhiteQuestion,
	qmWhiteExclamation,
	qmBlueQuestion,
	qmBlueExclamation,
	qmYellowQuestion,
	qmYellowExclamation,

	qmTotal
};

enum QUEST_EVENT_TYPE
{
	qetInvalid = -1,

	qetUpdateAll = 0,
	qetKillNpc,
	qetGetItem,
	qetDeleteItem,

	qetTotal
};

//��������
enum ALL_CRAFT_TYPE
{
	actInvalid = 0,

	actCollection,						// �ռ�����,����ɻ�,�ɽ�
	actProduce,							// ���켼��,��������,ҽ��,���
	actRead,								// �Ķ�����,�Ե���ʹ��
	// ���и�ħ�ȼ��ܿ����Ժ����
	actTotal
};

enum ITEM_BOX
{
	ibInvalid = 0x000000ff,		// ��ǿת��bye��ͬ��ʱ������ֻ�ܶ���0xffΪ�Ƿ���
	ibEquip = 0,				// ���뱣֤����ǵ�һ����Ч�ģ�����˳���Ӱ��������ʱ��װ������
	ibPackage,
	ibPackage1,
	ibPackage2,
	ibPackage3,
	ibPackage4,
	ibBank,
	ibSoldList,
//	ibLootList,
	ibTotal
};

//Npc�Ի��õ�Ƶ��,����Npc����ҹ㲥����Npc˵����ʱ��
enum NPC_TALK_TYPE
{
	nttInvalid = 0,

	nttNearby,
	nttSence,
	nttWhisper,

	nttTotal
};

//Npc����Ի���ʱ��
enum NPC_TALK_ACTION_TYPE
{
	ntatTurnToFight = 0,
	ntatDeath,
	ntatTeammateDeath,
	ntatIdle,
	ntatCustom,
	ntatTurnToIdle,
	ntatKillEnemy,
	ntatEscape,
	
	ntatTotal
};

enum DROP_TYPE
{
	dtInvalid = -1,

	dtNpcDrop,
	dtDoodadDrop,

	dtTotal
};

enum TARGET_TYPE
{
    ttInvalid,

    ttNoTarget,
    ttCoordination,
    ttNpc,
    ttPlayer,
    ttDoodad,
    ttItem,

    ttTotal
};

class KPlayer;
class KNpc;
class KDoodad;

struct KCaster
{
	TARGET_TYPE	eTargetType;

	union
	{
		KPlayer*	pPlayer;
		KNpc*		pNpc;
		KDoodad*	pDoodad;
	};
};

#pragma	pack(1)


struct KNPC_DATA
{
	// ע��,IDֻ�Ƕ�Client only��NPC������,������������ʱ��������ID, DoodadҲ��һ����
	DWORD	dwID;
	char	szName[_NAME_LEN];
	DWORD	dwTemplateID;
	BYTE	byLevel;
	int		nX;
	int		nY;
	int		nZ;
	int		nKind;							// Npc���ͣ���CHARACTER_KIND
	DWORD	dwScriptID;
	BYTE	byFaceDirection;				// ����
	BYTE	byAIType;						// AI���ͣ���AI_TYPE
	DWORD	dwAIParams[AI_PARAM_COUNT];		// AI����
	DWORD	dwNpcReliveID;
	DWORD	dwNpcTeamID;
	int		nNpcTeamIndex;
	int		nPatrolPathID;
	BYTE	byAIMainState;					// NPCĬ�ϵ�AI��Ϊ,1Ϊ����,2Ϊ�й�,3ΪѲ��
	DWORD	dwRepresentID;
	BYTE	byReserved[11];
};

struct KDOODAD_DATA
{
	DWORD	dwID;
	char	szName[_NAME_LEN];
	DWORD	dwTemplateID;
	BYTE	byLevel;
	int		nX;
	int		nY;
	int		nZ;
	DWORD	dwScriptID;
	BYTE	byFaceDirection;	
	DWORD	dwDoodadReliveID;
	BYTE	byReserved[28];
};

#pragma pack()


//8����ĺ�
//�������еĽǶ������ֵ
//�������ҷ�Ϊ�㣬��ʱ�����ӣ��͵ѿ�������ϵ��ͬ
//�м��мǣ�����
#define RIGHT_DIRECTION			0
#define UP_RIGHT_DIRECTION		1
#define UP_DIRECTION			2
#define UP_LEFT_DIRECTION		3
#define LEFT_DIRECTION			4
#define DOWN_LEFT_DIRECTION		5
#define DOWN_DIRECTION			6
#define DOWN_RIGHT_DIRECTION	7
#define CENTER_DIRECTION		8
#define ALL_DIRECTION			8


//����
#define TEN_THOUSAND_NUM        10000
#define KILO_NUM                1024        //�˺����㣬���Լ���ı�������������1/1024ȡ��1/100
#define MILLION_NUM				1048576		//��Ʒ�����ʰٷֱȾ��Ȼ���
#define MAX_SYNC_PER_REGION		(1 << MAX_SYNC_PER_REGION_BIT_NUM)	//ͬ�������Ʋ���
#define SYNC_CHARACTER_DELAY	GAME_FPS * 2	//ͬ����ʱ
#define STATE_UPDATE_TIME		8			//ħ��״̬ˢ�¼��
#define DEFAULT_MASS			128			//��λ��g
#define DEFAULT_GRAVITY			(8 * 512 / GAME_FPS / GAME_FPS)	//��λ����/֡^2
#define LOGOUT_WAITING_TIME		(GAME_FPS * 20)	//������ߵȴ�20��
#define DISCONNECT_TIME			(GAME_FPS * 4)	//������ߵȴ�ʱ��
#define LOGIN_TIME_OUT			(GAME_FPS * 200)	//��½��ʱʱ��

//�¾ɷ�λת����
static int g_DirectionChangeList[(ALL_DIRECTION + 1) * 2][ALL_DIRECTION + 1] =
{
	{6, UP_DIRECTION, CENTER_DIRECTION, DOWN_DIRECTION, UP_RIGHT_DIRECTION, RIGHT_DIRECTION, DOWN_RIGHT_DIRECTION, 0, 0},
	{6, UP_LEFT_DIRECTION, LEFT_DIRECTION, DOWN_LEFT_DIRECTION, UP_DIRECTION, CENTER_DIRECTION, DOWN_DIRECTION, 0, 0},

	{4, CENTER_DIRECTION, UP_DIRECTION, RIGHT_DIRECTION, UP_RIGHT_DIRECTION, 0, 0, 0, 0},
	{4, DOWN_LEFT_DIRECTION, LEFT_DIRECTION, DOWN_DIRECTION, CENTER_DIRECTION, 0, 0, 0, 0},

	{6, LEFT_DIRECTION, CENTER_DIRECTION, RIGHT_DIRECTION, UP_LEFT_DIRECTION, UP_DIRECTION, UP_RIGHT_DIRECTION, 0, 0},
	{6, DOWN_LEFT_DIRECTION, DOWN_DIRECTION, DOWN_RIGHT_DIRECTION, LEFT_DIRECTION, CENTER_DIRECTION, RIGHT_DIRECTION, 0, 0},

	{4, CENTER_DIRECTION, LEFT_DIRECTION, UP_DIRECTION, UP_LEFT_DIRECTION, 0, 0, 0, 0},
	{4, DOWN_RIGHT_DIRECTION, DOWN_DIRECTION, RIGHT_DIRECTION, CENTER_DIRECTION, 0, 0, 0, 0},

	{6, UP_DIRECTION, CENTER_DIRECTION, DOWN_DIRECTION, UP_LEFT_DIRECTION, LEFT_DIRECTION, DOWN_LEFT_DIRECTION, 0, 0},
	{6, UP_RIGHT_DIRECTION, RIGHT_DIRECTION, DOWN_RIGHT_DIRECTION, UP_DIRECTION, CENTER_DIRECTION, DOWN_DIRECTION, 0, 0},

	{4, CENTER_DIRECTION, LEFT_DIRECTION, DOWN_DIRECTION, DOWN_LEFT_DIRECTION, 0, 0, 0, 0},
	{4, UP_RIGHT_DIRECTION, UP_DIRECTION, RIGHT_DIRECTION, CENTER_DIRECTION, 0, 0, 0, 0},

	{6, LEFT_DIRECTION, CENTER_DIRECTION, RIGHT_DIRECTION, DOWN_LEFT_DIRECTION, DOWN_DIRECTION, DOWN_RIGHT_DIRECTION, 0, 0},
	{6, UP_LEFT_DIRECTION, UP_DIRECTION, UP_RIGHT_DIRECTION, LEFT_DIRECTION, CENTER_DIRECTION, RIGHT_DIRECTION, 0, 0},

	{4, CENTER_DIRECTION, DOWN_DIRECTION, RIGHT_DIRECTION, DOWN_RIGHT_DIRECTION, 0, 0, 0, 0},
	{4, UP_LEFT_DIRECTION, LEFT_DIRECTION, UP_DIRECTION, CENTER_DIRECTION, 0, 0, 0, 0},

	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static int g_SyncRegionList[ALL_DIRECTION + 1][ALL_DIRECTION + 1] =
{
	{3, RIGHT_DIRECTION, UP_RIGHT_DIRECTION, DOWN_RIGHT_DIRECTION, 0, 0, 0, 0, 0},
	{5, UP_RIGHT_DIRECTION, UP_DIRECTION, RIGHT_DIRECTION, UP_LEFT_DIRECTION, DOWN_RIGHT_DIRECTION, 0, 0, 0},
	{3, UP_DIRECTION, UP_LEFT_DIRECTION, UP_RIGHT_DIRECTION, 0, 0, 0, 0, 0},
	{5, UP_LEFT_DIRECTION, LEFT_DIRECTION, UP_DIRECTION, DOWN_LEFT_DIRECTION, UP_RIGHT_DIRECTION, 0, 0, 0},
	{3, LEFT_DIRECTION, UP_LEFT_DIRECTION, DOWN_LEFT_DIRECTION, 0, 0, 0, 0, 0},
	{5, DOWN_LEFT_DIRECTION, LEFT_DIRECTION, DOWN_DIRECTION, UP_LEFT_DIRECTION, DOWN_RIGHT_DIRECTION, 0, 0, 0},
	{3, DOWN_DIRECTION, DOWN_LEFT_DIRECTION, DOWN_RIGHT_DIRECTION, 0, 0, 0, 0, 0},
	{5, DOWN_RIGHT_DIRECTION, DOWN_DIRECTION, RIGHT_DIRECTION, DOWN_LEFT_DIRECTION, UP_RIGHT_DIRECTION, 0, 0, 0},
	{8, RIGHT_DIRECTION, UP_RIGHT_DIRECTION, UP_DIRECTION, UP_LEFT_DIRECTION, LEFT_DIRECTION, DOWN_LEFT_DIRECTION, DOWN_DIRECTION, DOWN_RIGHT_DIRECTION},
};

static int g_Direction2CoordinationList[ALL_DIRECTION][2] =
{
	{ 1, 0},
	{ 1, 1},
	{ 0, 1},
	{-1, 1},
	{-1, 0},
	{-1,-1},
	{ 0,-1},
	{ 1,-1}
};

// ԭ�ظ�����, �������ԭ�ظ����ڴ˼����,�ͻ�����ԭ�ظ���ĵȴ�ʱ��
#define SITU_REVIVE_INTERVAL (20 * 60)

#define MAX_DEATH_PUNISH_TIME_SIZE 10
// �����ͷ�ʱ��,��λ: ��
static int gs_nDeathPunishSecond[MAX_DEATH_PUNISH_TIME_SIZE] = {15, 60, 90, 120, 150, 180, 210, 240, 270, 300};

// �̵�ÿҳҪ��ʾ�������Ʒ����
#define MAX_SHOP_PAGE_ITEM_COUNT 40
// �̵����ҳ��
#define MAX_SHOP_PAGES           8

#define MAX_OPEN_SHOP_DISTANCE   (6 * 6)

#define MAX_TRADING_BOX_SIZE    10

#define MAX_TRADING_DISTANCE    (6 * 6)
#endif	//_GLOBAL_H_
