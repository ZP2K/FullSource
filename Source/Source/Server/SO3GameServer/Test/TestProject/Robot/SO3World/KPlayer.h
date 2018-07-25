/************************************************************************/
/* ���			                                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.30	Create												*/
/************************************************************************/
#ifndef _KPLAYER_H_
#define	_KPLAYER_H_

#include <list>
#include "SO3ProtocolBasic.h"
#include "KCharacter.h"
#include "KCircleList.h"
//#include "KSkillList.h"
#include "KCDTimerList.h"
#include "KItemList.h"
#include "KRoleDBDataDef.h"
#include "KReputation.h"

class IKG_Buffer;

//�����Ϸ״̬��
enum GAME_STATUS
{
	gsInvalid = 0,			//��Ч״̬

	gsWaitForLogin,			//���ڵȴ��ͻ��˵�����
	gsWaitForPermit,		//��½ʱ���ڵȴ�GC��ȷ��
    gsWaitForRoleData,		//��¼ʱ���ڵȴ�Goddess�Ļذ�
	gsSyncData,				//������ͻ���ͬ����ɫ����

	gsPlaying,				//������Ϸ��

	gsSearchMap,			//����������ͼ,��һ����Ҫ���
	gsTransferData,			//���ʱ���ڴ����ɫ����

	gsLogouting,			//�ͻ��������������ʱ�ǳ�,������Ȼ��
	gsDeleting,				//�ͻ��˶����������ʱ�ǳ�
	gsWaitForDisconnect,	//���ʱԴ�������ȴ��ͻ�����������

	gsTotal
};

struct KWindowSelectList
{
	KTarget			Target;
	DWORD			dwScriptID;		//�ű�ID
	DWORD			dwIndex;		//��ˮ��
	int				nFuncCount;		//��������
	DWORD			dwMapID;
	DWORD			dwMapCopyIndex;
	int				nX;
	int				nY;
	int				nZ;
	char			szFuncName[MAX_WINDOW_SELECT_COUNT][MAX_SELECT_FUNCNAME_LEN];
};

enum KCHARACTER_REVIVE_STATE
{
    crsInvalid = 0,
    crsWaitingForRevive,     // ����״̬��,�ȴ�����ʱ�䵽�����Ǳ�����
    crsConfirmRevive,        // ��������,AҪ����B,��B��δ�ظ�
    crsTotal
};

struct KPLAYER_REVIVE_PARAM
{
    time_t                  nLastDieTime;              // �ϴ�����ʱ��
    time_t                  nLastSituReviveTime;       // �ϴ�ԭ�ظ���ʱ��
    DWORD                   dwSituReviveCounter;       // ԭ�ظ��������, ���ڼ�����������ͷ�ʱ��
    KCHARACTER_REVIVE_STATE eState;

    int                     nLife;
    int                     nMana;
    DWORD                   dwMapID;
    int                     nX;
    int                     nY;
    int                     nZ;
};

// ��ҽӵ�����֪ͨ���Ӧ����
enum KDEATH_RESPOND_CODE
{
    drcInvalid,
    drcReviveInSitu,        // ԭ�ظ���
    drcRevivedByPlayer,     // ����Ҹ���
    drcRevivedInAltar,      // ����㸴��      
    drcTotal
};

// ����ͬ����Ʒ��Ϣ
struct TALK_SYNC_ITEM
{
    int nType;
    unsigned uDataSize;
    BYTE byData[0];
};

class KPlayer : public KCharacter
{
public:
	// GUID��������У�����ӵ�GS�ϵ��û��ĺϷ���
	GUID				m_Guid;

	ROLE_TYPE			m_eRoleType;

	//�����Ϸ״̬
	GAME_STATUS			m_eGameStatus;
	//��ʱ��,һ��4���ط�ʹ��:����/�����ǳ�/���ʱ�ȴ��ͻ�����������/��½ʱ
	int					m_nTimer;
	
	//�����Ч�����б�
//	KSkillList			m_SkillList;
	
	//��ҵ�Cooldown��ʱ����
	KCDTimerList		m_TimerList;
	//�������
	KItemList			m_ItemList;			//��ҵĵ��߼��ϣ�װ���ġ��������ġ��������
	//�������
	KReputation			m_ReputeList;		//��������

//	KPlayerPK			m_PK;				//PK��صĿ���

	KWindowSelectList	m_SelectList;		//ѡ��ص������б�

	//���������Ӫ
	KCAMP				m_eCamp;

    //������������ID
    DWORD               m_dwPartyID;
    //�ӳ���־����ʾ���ǲ��Ƕӳ�
    BOOL                m_bPartyLeaderFlag;

	//ʣ���˫������ֵ
	int					m_nDoubleExp;

	WORD				m_wRepresentId[REPRESENT_COUNT];

    // ----------------- ������� ---------------------------->
    BOOL				m_bOnPracticeRoom;          // �Ƿ���������
    int					m_nQiToSkillExpPercentAdd;  // ���ĵ��������Ӽ��������ȵİٷֱȼӳ�

//�����ز����ӿ�
public:
	KPlayer();
	~KPlayer();

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL Activate(void);

    void SetDisplayData(
        char	szRoleName[_NAME_LEN],
        int     nRoleType,
        WORD	wRepresentId[REPRESENT_COUNT],
		int		nLevel
    );

	void AddExp(int nExpIncrement, BOOL bUseDoubleExp = FALSE);
	//��set��getΪǰ׺�ĺ�����������Lua�����ɵ����Է��ʺ�����������������������
	void setLevel(int nLevel);	

	virtual BOOL ChangeRegion(KRegion* pDestRegion);
	virtual BOOL ChangeCell(KCell* pDestCell);

	int GetPlayerRelation(KPlayer* pTarget);
	int GetNpcRelation(KNpc* pTarget);
    
    // �ú�������: ���ͻ�����ҵ�ǰû��Ŀ��, ����һ��Npc��Player��ͼ�����ͷ�һ���к�����ʱ,
    // �ͻ�����Ҿͻ�ѵ�ǰĿ���л�����ͼ��������Npc��Player
    BOOL SwitchSelectTarget(DWORD dwSkillSrcID);

	//����ʱ,���Player����Ϣ
	void Dump(void);
	//�������ڵ��������
	BOOL SaveCurrentPosition(void);
	//�ָ���ҵ�ԭʼλ��
	BOOL RestorePosition(void);

    // ----------------------- �� ------------------------------->
    int m_nCurrentQi;

    BOOL AddLogOutQi(time_t TotalLogOutTime);
    BOOL SetQi(int nQi);

    //<----------------------- ����� --------------------------------

    //------------------------ ����� ------------------------------->
    int	m_nCurrentStamina;
    int	m_nStaminaReplenish;
    int m_nStaminaReplenishNextFrame;
    
	//------------------------ ����ӿ� ------------------------------->
#ifdef _CLIENT
    BOOL CopyTalkData(unsigned uSize, BYTE* pbyData);
#endif
#ifdef _SERVER
    BOOL SendSystemMessage(const char cszText[]);
#endif
    BOOL TalkToOne( const char cszTalkTo[],       unsigned uTalkDataSize, BYTE* pbyTalkData);    //����
    BOOL TalkToMany(PLAYER_TALK_RANGE eTalkRange, unsigned uTalkDataSize, BYTE* pbyTalkData);    //Ⱥ�ģ�eg. С�ӡ���ᡢ�Զ���Ƶ����
	//<----------------------- ����ӿ� --------------------------------

private:
	//���ʱ���������,���ָ���
	struct KOldPosition
	{
		int				m_nX;
		int				m_nY;
		int				m_nZ;
		DWORD			m_dwMapID;
		DWORD			m_dwMapCopyIndex;
	}m_OldPosition;

    static BYTE     s_byTalkData[MAX_CHAT_TEXT_LEN];
    static unsigned s_uTalkDataSize;
	//����ɫ��ǰ���ڸ����Ƿ���Trap�ű�
	BOOL CheckTrap(KCell* pOldCell);
	DWORD m_dwLastScriptID;
	//����ѭ����ָ����������˵��ָ���б�
	virtual BOOL GetCommand(void);


	//ʹ����Ʒ֮ǰ������Ʒ���м���������ж�
	int ItemTryCastSkillXYZ(KItem* pItem, KTarget &Target);
	int ItemTryCastSkill(KItem* pItem, KTarget &Target);

public:
	BOOL SetDoubleExp(DWORD dwDoubleExp); 
    int GetCDValue(DWORD dwCooldownID);

    // �书װ����������: Client->LuaMountKungfu->server->MountKungfu->Client->MountKungfu .
    BOOL    MountKungfu(DWORD dwID, DWORD dwLevel);
    BOOL    UmountKungfu();

public:
    int CanRevive();
    KPLAYER_REVIVE_PARAM m_ReviveParam;
    
public:
	//�ű��ӿڶ���
	DECLARE_LUA_CLASS(KPlayer);

    DECLARE_LUA_ENUM(Camp);
	DECLARE_LUA_BOOL(PartyLeaderFlag);
	DECLARE_LUA_DWORD(PartyID);
	DECLARE_LUA_ENUM(RoleType);
	DECLARE_LUA_INTEGER(DoubleExp);

	int LuaGetItem(Lua_State* L);
	int LuaDelItem(Lua_State* L);

	int LuaGetItemByIndex(Lua_State* L);
	
	int LuaGetMoney(Lua_State *L);

	int LuaExchangeItem(Lua_State* L);

	int LuaDestroyItem(Lua_State* L);
	int LuaGetRepresentId(Lua_State* L);

	int LuaGetBoxSize(Lua_State* L);
	int LuaCanExchange(Lua_State* L);

	int LuaGetEquipPos(Lua_State* L);	//��ȡװ������װ���ĵط�������װ������nil
	int LuaGetFreeRoom(Lua_State* L);
	int LuaSatisfyRequire(Lua_State* L);	//�Ƿ�����װ������

	int LuaGetItemAmount(Lua_State* L);

    int LuaGetSkillList(Lua_State* L);
	
	int LuaCastProfessionSkill(Lua_State* L);

    int LuaWindowSelect(Lua_State* L);
    int LuaDoDeathRespond(Lua_State* L);

	int LuaOnCloseLootWindow(Lua_State* L);
    int LuaCastCommonSkill(Lua_State* L);
    int LuaStopCurrentAction(Lua_State* L);

    int LuaGetSkillCDProgress(Lua_State* L);
    int LuaGetCDInterval(Lua_State* L);
    int LuaGetCDLeft(Lua_State* L);
    int LuaGetSkillLevel(Lua_State* L);

    int LuaGetSkillLifeCost(Lua_State* L);
    int LuaGetSkillManaCost(Lua_State* L);
    int LuaGetSkillRageCost(Lua_State* L);
    int LuaGetSkillStaminaCost(Lua_State* L);

    int LuaGetSkillPrepare(Lua_State* L);
    int LuaGetSkillChannel(Lua_State* L);

    int LuaGetSkillMinRadius(Lua_State* L);
    int LuaGetSkillMaxRadius(Lua_State* L);

    int LuaGetSkillExp(Lua_State* L);

    int LuaMountKungfu(Lua_State* L);
    int LuaUmountKungfu(Lua_State* L);
    int LuaGetKungfuMount(Lua_State* L);

	int LuaUseItemOnSelf(Lua_State* L);
	int LuaUseItemOnTarget(Lua_State* L);
	int LuaUseItemOnCoordination(Lua_State* L);

	int	LuaGetOTActionState(Lua_State* L);

	//--- AI ----------------------------------
	int LuaFollowSelectTarget(Lua_State* L);
	int LuaStopFollow(Lua_State* L);

	int LuaSearchForAllies(Lua_State* L);
	int LuaSearchForEnemy(Lua_State* L);
	//-----------------------------------------

	int LuaUseItem(Lua_State* L);
	int LuaGetItemPos(Lua_State* L);

    int LuaSearchForPartyMember(Lua_State* L);

    int LuaTradingInviteRequest(Lua_State* L);
    int LuaTradingInviteRespond(Lua_State* L);
    int LuaTradingAddItem(Lua_State* L);
    int LuaTradingDeleteItem(Lua_State* L);
    int LuaTradingSetMoney(Lua_State* L);
    int LuaTradingConfirm(Lua_State* L);
    //--- Talk --------------------------------
    int LuaGetTalkData(Lua_State* L);
    int LuaGetTalkLinkItem(Lua_State* L);
    int LuaTalkToOne(Lua_State* L);
    int LuaTalkToMany(Lua_State* L);
    int LuaDoAnimation(Lua_State* L);
    //-----------------------------------------
    int LuaAddRelation(Lua_State* L);
    int LuaDelRelation(Lua_State* L);
};
#endif	//_KPLAYER_H_
