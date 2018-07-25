/************************************************************************/
/* �ͻ�������Ϸ������������												*/
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* Modify	 : Spe														*/
/* Remark	 : ��Ҫ�����޸ķָ������ע�ͣ����Զ����ɴ���ĺ�����Ҫʹ�� */
/* History	 :															*/
/*		2005.01.26	Create												*/
/************************************************************************/
#ifndef _KPLAYER_CLIENT_H_
#define _KPLAYER_CLIENT_H_

#if defined(_CLIENT)

#include <vector>
#include <map>
#include "Common/KG_Socket.h"
#include "IClient.h"
#include "GS_Client_Protocol.h"
#include "KIDCache.h"
#include "KCircleList.h"

#define	PACKET_MAX_SIZE			1024
#define PACKET_MAX_COUNT		512

class KSO3World;
class KCharacter;
class KPlayer;
class KNpc;
class KTarget;
class KMonster;

enum GAMSERVER_STATE
{
    gmsInvalid,
    gmsOnline,
    gmsConnectFail,
    gmsSyncDataFail,
    gmsDisconnect,
    gmsTotal
};

class KPlayerClient
{
public:
	KPlayerClient(void);
	~KPlayerClient(void);

	BOOL Init(KMonster *pOwner);
	BOOL UnInit(void);

	BOOL Connect(const char* pszIPAddr, int nPort);
	BOOL Disconnect();

	int Breathe(void);

    KMonster *m_pOwner;

    GAMSERVER_STATE GetOnlineState(){return m_nState;}
    const char *GetStateDecription(){return ms_szStateDecription[m_nState];}

private:
    GAMSERVER_STATE m_nState;
    static char ms_szStateDecription[gmsTotal][265];

public:
	//�����������
	BOOL DoApplyPlayerData(char* pszRoleName, GUID Guid);
	//ȷ��ͬ������������
	BOOL DoAckPlayerData(void);
	//��ҵǳ�
	BOOL DoPlayerLogout(void);
    
    // ��ɫվ��
	BOOL DoCharacterStand(KCharacter* pCharacter);
	// ��ɫ�߶�
	BOOL DoCharacterWalk(KCharacter* pCharacter, int nDstX, int nDstY);
	//��ɫ�ܶ�
	BOOL DoCharacterRun(KCharacter* pCharacter, int nDstX, int nDstY);
    // ��ɫ��Ӿ
    BOOL DoCharacterSwim(KCharacter* pCharacter, int nDstX, int nDstY, int nDstZ);
	// ��ɫ��Ծ
	BOOL DoCharacterJump(KCharacter* pCharacter, BOOL bStandJump, int nDirectionXY);
	// ��ɫ����
	BOOL DoCharacterSit(KCharacter* pCharacter);

    BOOL DoStopCurrentAction();
	// ��ɫ�ż���
	BOOL DoCharacterSkill(DWORD dwSkillID, DWORD dwSkillLevel, KTarget* pTarget);
	// ��ɫ��������
	BOOL DoCharacterAction(DWORD dwCharacterID, BYTE byAction);
	// ��ɫ˵��
	BOOL DoCharacterSay(char* pszText);
	// ��ɫ����
	BOOL DoCharacterTurn(int nFaceDirection);

	//�����½�ɫ������
	BOOL DoApplyNewCharacterData(DWORD dwCharacterID);
	//���������ʾ����
	BOOL DoApplyPlayerDisplayData(KPlayer* pPlayer, DWORD dwCheckSum);
	//����Npc��ʾ����
	BOOL DoApplyNpcDisplayData(KNpc* pNpc, DWORD dwCheckSum);    

    BOOL DoMountKungfu(DWORD dwID, DWORD dwLevel);
    BOOL DoCancelBuff(int nStyle, int nIndex);

    BOOL DoSetSheathState(BOOL bSheathFlag);

    // -------------------- party about ------------------->

	//����xxx���
    BOOL DoInviteJoinPartyRequest(const char cszInvitePlayer[]);
    //�������������Ӧ��
    BOOL DoInviteJoinPartyRespond(const char cszInviteSrc[], const GUID &crGuid, BOOL bAgreeFlag);
    //�������xxx�Ķ���
    BOOL DoApplyJoinPartyRequest(const char cszApplyDst[]);
    //�������������Ӧ��
    BOOL DoApplyJoinPartyRespond(const char cszApplySrc[], const GUID &crGuid, BOOL bAgreeFlag);
    //�����뿪����
    BOOL DoLeavePartyRequest();
    //��xxx�߳�����
    BOOL DoKickoutPartyMember(const char cszRoleName[]);
    //�����ƽ��ӳ�
    BOOL DoPartyChangeLeaderRequest(const char cszNewLeader[]);
    //���ö�����Ʒ����ģʽ
    BOOL DoPartySetLootMode(int nLootMode, KPlayer* pPlayer);
	//���ö�����Ʒ��ҪROLL��Ʒ��
	BOOL DoPartySetRollQuality(int nRollQuality, KPlayer* pPlayer);
    // ����С��ͼ�ϵı�Ǹ�����
    BOOL DoPartyMemberNotifySignpost(int nX, int nY);
    // <-------------------- party about -------------------

	// ��������
	BOOL DoAcceptQuest(KTarget* pTarget, DWORD dwQuestID);
	// �������
	BOOL DoFinishQuest(DWORD dwQuestID, KTarget& rTarget, int nPresentChoice1, int nPresentChoice2);
	// ȡ������
	BOOL DoCancelQuest(int nQuestIndex);
	// ��������
	BOOL DoShareQuest(int nQuestIndex);

	//GMָ��
	BOOL DoGmCommand(const char* pszGmCommand);

    // --------------------- shop about --------------------->
    BOOL DoOpenShopRequest(DWORD dwShopID);
    BOOL DoQueryShopItems(DWORD dwShopID, DWORD dwPageIndex);
    BOOL DoShopBuyRequest(DWORD dwShopID, DWORD dwPageIndex, DWORD dwPosIndex, DWORD dwItemID, int nCount, int nCost);
    BOOL DoShopSellRequest(DWORD dwShopID, DWORD dwItemID, int nCost, DWORD dwBoxIndex, DWORD dwPosIndex);
    BOOL DoShopRepairRequest(DWORD dwShopID, DWORD dwItemID, int nCost, DWORD dwBoxIndex, DWORD dwPosIndex);
    // ���󹺻ػع��б����һ����Ʒ
    BOOL DoSoldListBuyRequest(DWORD dwShopID, DWORD dwX, DWORD dwItemID);
    // <--------------------- shop about ---------------------

    // ---------------------- Trading ----------------------->
    BOOL DoTradingInviteRequest(DWORD dwInviteDst);
    BOOL DoTradingInviteRespond(BOOL bAccept);
    BOOL DoTradingAddItem(DWORD dwBoxIndex, DWORD dwPosIndex, DWORD dwGridIndex);
    BOOL DoTradingDeleteItem(DWORD dwGridIndex);
    BOOL DoTradingSetMoney(int nMoney);
    BOOL DoTradingConfirm(BOOL bConfirm, int nKey);
    // <---------------------- Trading ----------------------

    BOOL DoSetTarget(int nTargetType, DWORD dwTargetID);

	// ���뽻��װ��
	BOOL DoApplyExchangeItem(DWORD dwBox1, DWORD dwX1, DWORD dwBox2, DWORD dwX2, DWORD dwAmount);

	// ѡ�񴰿�ѡ��
	BOOL DoWindowSelect(DWORD dwIndex, BYTE bySelect);
	// �䷽ʹ��Э��
	BOOL DoCastProfessionSkill(DWORD dwCraftID, DWORD dwRecipeID, KTarget* pTarget);
	// ʰȡ��Ʒ����������
	BOOL DoLootOrRollItem(DWORD dwDoodadID, DWORD dwItemID, BOOL bCancel);
	// ����ʰȡ�б�
	BOOL DoApplyLootList(DWORD dwDoodadID);
	// ��
	BOOL DoCharacterOpen(DWORD dwDoodadID);
    // ����ڽӵ�����֪ͨ�����Ӧ��
    BOOL DoDeathRespond(int nRespondCode);
	// ����ʹ�õ���
	BOOL DoApplyUseItem(DWORD dwBox, DWORD dwX, KTarget& rTarget);
	// �ı�ս������
	BOOL DoApplyFightFlag(DWORD dwForceID, BOOL bFight);
	// ����ʰȡ
	BOOL DoFinishLoot(DWORD dwDoodadID);
	// ʰȡ��Ǯ
	BOOL DoLootMoney(DWORD dwDoodadID);
	// �ı�PK״̬
	BOOL DoChangePkMode(int nMode);
	// �ӳ�������Ʒ
	BOOL DoLeaderDistributeItem(DWORD dwDoodadID, DWORD dwItemID, DWORD dwDstPlayerID);

    // ---------------------- talk about ---------------------->
    // ������Ϣ
    BOOL DoPlayerTalkToOne( const char cszTalkTo[], unsigned uTalkDataSize, BYTE* pbyTalkData);
    // Ⱥ����Ϣ
    BOOL DoPlayerTalkToMany(BYTE byTalkRange,       unsigned uTalkDataSize, BYTE* pbyTalkData);
    // <--------------------- talk about -----------------------

    BOOL DoPlayerAddRelationRequest(const char cszAlliedPlayerName[]);
    BOOL DoPlayerDelRelationRequest(DWORD dwAlliedPlayerID);

    //AutoCode:-����Э�麯������-

private:
	//����Э�麯��
	BOOL Send(void* pvData, size_t nSize);

public:
    int GetServerLoop(){ return m_nGSGameLoop; }
private:
    IKG_SocketStream   *m_piSocketStream;

	//Э�鴦����ָ������
	typedef void (KPlayerClient::*PROCESS_PROTOCOL_FUNC)(char* pData, size_t nSize, int nFrame);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[gs_client_connection_end];

	//Э���С����
	int m_nProtocolSize[gs_client_connection_end];

	friend void __stdcall ClientEventNotify(LPVOID lpParam, const unsigned long &ulnEventType);

	//����ͬ�����ķ����֡��
	int		m_nGSGameLoop;
	//�ۼ�֡����
	float	m_fDeltaLoop;
	//���ݰ�����
	struct KBufferPacket
	{
		int							m_nGameLoop;
		size_t						m_nSize;
		EXTERNAL_PROTOCOL_HEADER*	m_pHeader;
	};
	//���ݰ��������
	KCircleList			m_BufferPacketList;

	//�����ʾ���ݵ�Cache
	KIDCache			m_PlayerDisplayDataCache;
	//Npc��ʾ���ݵ�Cache
	KIDCache			m_NpcDisplayDataCache;

	//�����ɫ���ݵĶ���
	typedef std::map<DWORD, DWORD>		APPLY_NEW_CHARACTER_LIST;
	APPLY_NEW_CHARACTER_LIST			m_ApplyNewCharacterList;

//Э�鴦����
private:
	//������Ϣ
	void OnResponseMessage(char* pData, size_t nSize, int nFrame);

	//ͬ���������
	void OnSyncPlayerBaseInfo(char* pData, size_t nSize, int nFrame);
	//ͬ������״̬
	void OnSyncQuestState(char* pData, size_t nSize, int nFrame);
	//ͬ�������б�
	void OnSyncQuestList(char* pData, size_t nSize, int nFrame);

	//�ߺ�֪ͨ
	void OnKickSelfOut(char* pData, size_t nSize, int nFrame);
	//�л���������֪ͨ
	void OnChangeGameServer(char* pData, size_t nSize, int nFrame);

    void OnSyncRegionPlayer(char* pData, size_t nSize, int nFrame);
    void OnSyncRegionNpc(char* pData, size_t nSize, int nFrame);

    void OnSyncNewPlayer(char* pData, size_t nSize, int nFrame);
    void OnSyncNewNpc(char* pData, size_t nSize, int nFrame);

	void OnRemoveCharacter(char* pData, size_t nSize, int nFrame);

    void OnReviseMoveState(char* pData, size_t nSize, int nFrame);
    void OnSyncMoveState(char* pData, size_t nSize, int nFrame);

    void OnCharacterStand(char* pData, size_t nSize, int nFrame);
	//��ɫ�߶�ͬ������
	void OnCharacterWalk(char* pData, size_t nSize, int nFrame);
	void OnCharacterRun(char* pData, size_t nSize, int nFrame);
    void OnCharacterSwim(char* pData, size_t nSize, int nFrame);
	void OnCharacterJump(char* pData, size_t nSize, int nFrame);
    void OnCharacterSit(char* pData, size_t nSize, int nFrame);
    void OnCharacterMeditate(char* pData, size_t nSize, int nFrame);

	void OnCharacterDash(char* pData, size_t nSize, int nFrame);
	void OnCharacterKnockedDown(char* pData, size_t nSize, int nFrame);
	void OnCharacterKnockedBack(char* pData, size_t nSize, int nFrame);
	void OnCharacterKnockedOff(char* pData, size_t nSize, int nFrame);
    void OnCharacterStun(char* pData, size_t nSize, int nFrame);
    void OnCharacterHalt(char* pData, size_t nSize, int nFrame);
	//��ɫ����
	void OnCharacterDeath(char* pData, size_t nSize, int nFrame);
	void OnMoveCharacter(char* pData, size_t nSize, int nFrame);
    //��ұ�����֪ͨ
    void OnPlayerRevivedNotify(char* pData, size_t nSize, int nFrame);
	// ͬ����ɫת��
	void OnCharacterTurn(char* pData, size_t nSize, int nFrame);
	// ͬ����ɫ����
	void OnCharacterAction(char* pData, size_t nSize, int nFrame);
	// ͬ����ɫ˵��
	void OnCharacterSay(char* pData, size_t nSize, int nFrame);

    void OnOverTimeAction(char* pData, size_t nSize, int nFrame);

	// ��ɫ��ʾ����
	void OnPlayerDisplayData(char* pData, size_t nSize, int nFrame);
	// Npc��ʾ����
	void OnNpcDisplayData(char* pData, size_t nSize, int nFrame);

	// ���Ӿ���ֵ
	void OnAddExperience(char* pData, size_t nSize, int nFrame);

    void OnCommonHealth(char* pData, size_t nSize, int nFrame);

    //Doodad״̬ͬ������
	void OnSyncDoodadState(char* pData, size_t nSize, int nFrame);

	//֡����ź�
	void OnFrameSignal(char* pData, size_t nSize, int nFrame);

    void OnTerrainTest(char* pData, size_t nSize, int nFrame);

    void OnSyncSkillData(char* pData, size_t nSize, int nFrame);    

    void OnSkillPrepare(char* pData, size_t nSize, int nFrame);
    void OnSkillCast(char* pData, size_t nSize, int nFrame);
    void OnSyncCommonSkillAutoCastState(char* pData, size_t nSize, int nFrame);
    void OnResetSkillCooldown(char* pData, size_t nSize, int nFrame);

    void OnSkillEffectResult(char* pData, size_t nSize, int nFrame);   
    void OnSyncBehitRepresent(char* pData, size_t nSize, int nFrame);
    void OnSyncFightState(char* pData, size_t nSize, int nFrame);   	
	void OnSyncBuffList(char* pData, size_t nSize, int nFrame);
    void OnSyncBuffSingle(char* pData, size_t nSize, int nFrame);
    void OnSyncRepresentState(char* pData, size_t nSize, int nFrame);
    void OnSyncKungfuMount(char* pData, size_t nSize, int nFrame);
    void OnSkillBeatBack(char* pData, size_t nSize, int nFrame);
    void OnSyncSheathState(char* pData, size_t nSize, int nFrame);

    // --------------------- ��ֵͬ�� --------------------------

    void OnSyncSelfMaxLMRS(char* pData, size_t nSize, int nFrame);
    void OnSyncSelfCurrentLMRS(char* pData, size_t nSize, int nFrame);
    void OnSyncTargetMaxLMR(char* pData, size_t nSize, int nFrame);
    void OnSyncTargetCurrentLMR(char* pData, size_t nSize, int nFrame);
    void OnSyncPartyMemberMaxLMR(char* pData, size_t nSize, int nFrame);
    void OnSyncPartyMemberCurrentLMR(char* pData, size_t nSize, int nFrame);
    void OnSyncPartyMemberMisc(char* pData, size_t nSize, int nFrame);
    void OnSyncPartyMemberCoordinate(char* pData, size_t nSize, int nFrame);
	void OnBroadcastCharacterLife(char* pData, size_t nSize, int nFrame);

    // --------------------- party about -----------------------

    //ͬ�����������Ϣ
    void OnSyncPlayerPartyBaseInfo(char* pData, size_t nSize, int nFrame);
    //ͬ�������Ա��Ϣ
    void OnSyncPlayerPartyMemberInfo(char* pData, size_t nSize, int nFrame);
    //xxx���������
    void OnInvitePlayerJoinPartyRequest(char* pData, size_t nSize, int nFrame);
    //��������������Ӧ(��ʵֻ��ʧ�ܵ�ʱ��Ż��������Ӧ)
    void OnInvitePlayerJoinPartyRespond(char* pData, size_t nSize, int nFrame);
    //xxx���������Ķ���
    void OnApplyJoinPartyRequest(char* pData, size_t nSize, int nFrame);
    //��������������Ӧ(��ʵֻ��ʧ�ܵ�ʱ��Ż��������Ӧ)
    void OnApplyJoinPartyRespond(char* pData, size_t nSize, int nFrame);
    //��������
    void OnPartyCreateNotify(char* pData, size_t nSize, int nFrame);
    //֪ͨ"�������xxx�Ķ���"
    void OnPlayerJoinPartyNotify(char* pData, size_t nSize, int nFrame);
    //�����¼Ӷ�Ա��֪ͨ
    void OnPartyAddMemberNotify(char* pData, size_t nSize, int nFrame);
    //��Ա�뿪�����֪ͨ
    void OnPartyDelMemberNotify(char* pData, size_t nSize, int nFrame);
    //������Ҷӳ���־λ�ı��֪ͨ(Player���ϵ�������ĵ�֪ͨ)
    void OnSyncPartyLeaderFlag(char* pData, size_t nSize, int nFrame);
    // �ӳ��ƽ�֪ͨ
    void OnPartyChangeLeaderNotify(char* pData, size_t nSize, int nFrame);
    // ��Ʒ���䷽ʽ���֪ͨ
    void OnPartySetLootModeNotify(char* pData, size_t nSize, int nFrame);
    // �����ɢ֪ͨ
    void OnPartyDisbandNotify(char* pData, size_t nSize, int nFrame);
    // ��Ա������֪ͨ
    void OnSyncPartyMemberOnlineFlag(char* pData, size_t nSize, int nFrame);
    // ͬ��С��ͼ��ע��
    void OnPartyMemberSignpostNotify(char* pData, size_t nSize, int nFrame);
    // <--------------------- party about ---------------------


    // --------------------- shop about --------------------->
    void OnOpenShopNotify(char* pData, size_t nSize, int nFrame);
    void OnUpdateShopItem(char* pData, size_t nSize, int nFrame);
    void OnShopBuyRespond(char* pData, size_t nSize, int nFrame);
    // <--------------------- shop about ---------------------
    
    // ---------------------- Trading ----------------------->
    void OnTradingInviteRequest(char* pData, size_t nSize, int nFrame);
    void OnOpenTradingBoxNotify(char* pData, size_t nSize, int nFrame);
    void OnSyncTradingConfirm(char* pData, size_t nSize, int nFrame);
    void OnSyncTradingItemData(char* pData, size_t nSize, int nFrame);
    void OnSyncTradingMoney(char* pData, size_t nSize, int nFrame);
    void OnTradingComplete(char* pData, size_t nSize, int nFrame);
    // <--------------------- Trading ------------------------

	// ͬ����Ǯ����
	void OnSyncMoney(char* pData, size_t nSize, int nFrame);
	// ͬ�����ߵ���ʱ����
	void OnSyncTempItemData(char* pData, size_t nSize, int nFrame);
	// ͬ����������
	void OnSyncItemData(char* pData, size_t nSize, int nFrame);
	// ͬ������ɾ��
	void OnSyncItemRemove(char* pData, size_t nSize, int nFrame);
	// ����װ��
	void OnExchangeItem(char* pData, size_t nSize, int nFrame);
    // ��װ�㲥
    void OnSyncEquipRepresent(char* pData, size_t nSize, int nFrame);
	// ������Ʒ�������;�
	void OnUpdateItemAmount(char* pData, size_t nSize, int nFrame);


	// ѧϰ�����
	void OnLearnProfessionNotify(char* pData, size_t nSize, int nFrame);
	// ѧϰ�䷽
	void OnLearnRecipeNotify(char* pData, size_t nSize, int nFrame);
	// ��������
	void OnForgetProfessionNotify(char* pData, size_t nSize, int nFrame);
	// �����䷽
	void OnForgetRecipeNotify(char* pData, size_t nSize, int nFrame);
	// ��������ܾ���
	void OnAddProfessionProficiency(char* pData, size_t nSize, int nFrame);
	// ������������ȼ�
	void OnSetMaxProfessionLevelNotify(char* pData, size_t nSize, int nFrame);
	// ͬ��������������
	void OnUpdatePlayerProfession(char* pData, size_t nSize, int nFrame);
	// ͬ����������䷽�б�
	void OnUpdatePlayerRecipe(char* pData, size_t nSize, int nFrame);
    // ʩ�������
    void OnCastRecipe(char* pData, size_t nSize, int nFrame);
	// ��һ���ͻ��˴���
	void OnOpenWindow(char* pData, size_t nSize, int nFrame);
	// ͬ���䷽��ʹ��
	void OnStartUseRecipe(char* pData, size_t nSize, int nFrame);
	// ���������ͷ�
	void OnEndCast(char* pData, size_t nSize, int nFrame);

	void OnAcceptQuest(char* pData, size_t nSize, int nFrame);
	void OnFinishQuest(char* pData, size_t nSize, int nFrame);
	void OnCancelQuest(char* pData, size_t nSize, int nFrame);
	void OnShareQuest(char* pData, size_t nSize, int nFrame);
	// ͬ���������
	void OnSyncQuestValue(char* pData, size_t nSize, int nFrame);

	// ͬ��ʰȡ�б�
	void OnSyncLootList(char* pData, size_t nSize, int nFrame);
	// ��������Ϣ
	void OnRollItemMessage(char* pData, size_t nSize, int nFrame);
	// ��ɫʰȡ��Ʒ
	void OnCharacterPick(char* pData, size_t nSize, int nFrame);
	// ��ʰȡ���
	void OnOpenLootList(char* pData, size_t nSize, int nFrame);
	// ͬ��ս������
	void OnSyncFightFlag(char* pData, size_t nSize, int nFrame);
	// ͬ������
	void OnSyncRepute(char* pData, size_t nSize, int nFrame);
	// ͬ����������ս������
	void OnSyncFightflagList(char* pData, size_t nSize, int nFrame);
	// ͬ�����е���������
	void OnSyncReputeList(char* pData, size_t nSize, int nFrame);
	// 
	void OnCharacterOpen(char* pData, size_t nSize, int nFrame);
	// ��ӹ㲥��Ǯ��ʰȡ
	void OnRollMoneyMessage(char* pData, size_t nSize, int nFrame);
	// ����ʰȡ
	void OnFinishLoot(char* pData, size_t nSize, int nFrame);
	// start roll
	void OnBeginRollItem(char* pData, size_t nSize, int nFrame);
	// ͬ�����Ƶ��û���Ϣ
	void OnCustomOTAction(char* pData, size_t nSize, int nFrame);
	// ͬ��������Ľ��
	void OnShareQuestResult(char* pData, size_t nSize, int nFrame);
	// ��ӳ��
	void OnModifyThreat(char* pData, size_t nSize, int nFrame);
	// �޸ı����
	void OnModifyThreaten(char* pData, size_t nSize, int nFrame);
	// ������
	void OnClearThreat(char* pData, size_t nSize, int nFrame);
	// ��������
	void OnClearThreaten(char* pData, size_t nSize, int nFrame);

    // ---------------------- talk about ---------------------->
    // ������Ϣ
    void OnPlayerTalkToOne(char* pData, size_t nSize, int nFrame);
    // Ⱥ����Ϣ
    void OnPlayerTalkToMany(char* pData, size_t nSize, int nFrame);

	// ͬ��˫������
	void OnUpdateDoubleExp(char* pData, size_t nSize, int nFrame);
	// 
	void OnUpdatePlayerStamina(char* pData, size_t nSize, int nFrame);
    // '��'��ͬ��
    void OnUpdatePlayerQi(char* pData, size_t nSize, int nFrame);
	// 
	void OnDoodadLootOver(char* pData, size_t nSize, int nFrame);

    void OnPlayerAddRelationRespond(char* pData, size_t nSize, int nFrame);
    void OnPlayerDelRelationRespond(char* pData, size_t nSize, int nFrame);

	//AutoCode:-����Э�麯������-
};

//extern KPlayerClient g_PlayerClient;

#endif	//_CLIENT
#endif	//_KPLAYER_CLIENT_H_