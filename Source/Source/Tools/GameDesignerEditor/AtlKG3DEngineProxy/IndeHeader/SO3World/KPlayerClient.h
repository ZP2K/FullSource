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
#include <list>
#include <vector>
#include <map>
#include "Common/KG_Socket.h"
#include "IClient.h"
#include "GS_Client_Protocol.h"
#include "KIDCache.h"

#define	PACKET_MAX_SIZE			1024
#define PACKET_MAX_COUNT		512

class KSO3World;
class KCharacter;
class KPlayer;
class KNpc;
class KTarget;

class KPlayerClient
{
public:
	KPlayerClient(void);
	~KPlayerClient(void);

	BOOL Init(void);
	void UnInit(void);

	BOOL Connect(const char* pszIPAddr, int nPort);
	BOOL Disconnect();

	int  Breathe(void);

    int GetServerLoop(){ return m_nGSGameLoop; }
    int GetPingValue() { return (int)m_dwPingEchoTime; }

private:
    BOOL ProcessPackage(BYTE* pbyData, size_t uDataLen);
	BOOL Send(void* pvData, size_t nSize);

private:
    IKG_SocketStream*   m_piSocketStream;

	typedef void (KPlayerClient::*PROCESS_PROTOCOL_FUNC)(BYTE* pbyData, size_t nSize, int nFrame);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[gs_client_connection_end];

	int m_nProtocolSize[gs_client_connection_end];

	// ����ͬ�����ķ����֡��
	int		m_nGSGameLoop;
	// �ۼ�֡����
	float	m_fDeltaLoop;
	// ���ݰ�����
	struct KBufferPacket
	{
		int							m_nGameLoop;
		size_t						m_uSize;
		EXTERNAL_PROTOCOL_HEADER*	m_pHeader;
	};
	// ���ݰ��������
    std::list<IKG_Buffer*>          m_BufferPacketList;

	// �����ʾ���ݵ�Cache
	KIDCache			m_PlayerDisplayDataCache;
	// Npc��ʾ���ݵ�Cache
	KIDCache			m_NpcDisplayDataCache;

    DWORD  m_dwNextPingTime;
    DWORD  m_dwPingEchoTime;

public:
    BOOL DoHandshakeRequest(DWORD dwRoleID, const GUID& crGuid);
	// �ͻ��˳����������,�����ȥ����
	BOOL DoApplyEnterScene();
	// ��ҵǳ�
	BOOL DoPlayerLogout();
    // ping
    BOOL DoPingSignal();
    
    BOOL DoMoveCtrl(KPlayer* pPlayer, BOOL bMove, BOOL bWalk, int nDirection, int nTurn);
    // ����ͬ��У��
    BOOL DoApplyMoveAdjust();
    // ��ɫվ��
	BOOL DoCharacterStand(KPlayer* pPlayer, int nCurrentState);
	// ��ɫ��Ծ
	BOOL DoCharacterJump(KPlayer* pPlayer, int nCurrentState, BOOL bStandJump, int nDirectionXY);
	// ��ɫ����
	BOOL DoCharacterSit(KPlayer* pPlayer, int nCurrentState);
	// ��ɫ�����븩��
	BOOL DoSyncDirection(KPlayer* pPlayer);

    BOOL DoStopCurrentAction();
	// ��ɫ�ż���
	BOOL DoCharacterSkill(DWORD dwSkillID, DWORD dwSkillLevel, int nFaceDirection, KTarget* pTarget);
	// ��ɫ��������
	BOOL DoCharacterAction(DWORD dwCharacterID, DWORD dwAction);

	// ���������ʾ����
	BOOL DoApplyPlayerDisplayData(KPlayer* pPlayer, DWORD dwCheckSum);

    BOOL DoMountKungfu(DWORD dwID, DWORD dwLevel);
    BOOL DoCancelBuff(int nIndex);

    BOOL DoSetSheathState(BOOL bSheathFlag);

    BOOL DoSetHideHatFlag(BOOL bHideHat);

    // �������Buff list���ݣ�ͨ������ͬ����Χ�ڵĶ���
    BOOL DoApplyPlayerBuffList(DWORD dwPlayerID);

    // -------------------- party about ------------------->

	// ����xxx���
    BOOL DoInviteJoinTeamRequest(const char cszInvitePlayer[]);
    // �������������Ӧ��
    BOOL DoInviteJoinTeamRespond(const char cszInviteSrc[], BOOL bAgreeFlag);
    // �������xxx�Ķ���
    BOOL DoApplyJoinTeamRequest(const char cszApplyDst[]);
    // �������������Ӧ��
    BOOL DoApplyJoinTeamRespond(const char cszApplySrc[], BOOL bAgreeFlag);
    // �����뿪����
    BOOL DoLeaveTeamRequest();
    // ��xxx�߳�����
    BOOL DoKickoutTeamMember(const char cszRoleName[]);
    // �����ƽ��ӳ�
    BOOL DoTeamChangeAuthorityRequest(KTEAM_AUTHORITY_TYPE eType, DWORD dwTargetID);
    
    BOOL DoTeamSetLootMode(int nLootMode, KPlayer* pPlayer);
	BOOL DoTeamSetRollQuality(int nRollQuality, KPlayer* pPlayer);
    BOOL DoTeamSetFormationLeader(DWORD dwNewFormationLeaderID, int nGroupIndex, KPlayer* pPlayer);

    // ����С��ͼ�ϵı�Ǹ�����
    BOOL DoTeamMemberNotifySignpost(int nX, int nY);

    BOOL DoTeamSetMarkRequest(int nMarkType, DWORD dwTargetID);

    BOOL DoTeamLevelUpRaidRequest();

    BOOL DoTeamChangeMemberGroupRequest(DWORD dwMemberID, int nGroupIndex);

	// ��������
	BOOL DoAcceptQuest(KTarget* pTarget, DWORD dwQuestID);
	// �������
	BOOL DoFinishQuest(DWORD dwQuestID, KTarget& rTarget, int nPresentChoice1, int nPresentChoice2);
	// ȡ������
	BOOL DoCancelQuest(int nQuestIndex);
	// ��������
	BOOL DoShareQuest(int nQuestIndex);
	// ���ܻ�������
	BOOL DoAcceptEscortQuest(DWORD dwStartPlayerID, DWORD dwQuestID);

	BOOL DoGmCommand(const char cszCommand[]);

    // --------------------- shop about --------------------->
    BOOL DoOpenShopRequest(DWORD dwShopID);
    BOOL DoQueryShopItems(DWORD dwShopID, DWORD dwPageIndex);
    BOOL DoShopBuyRequest(DWORD dwShopID, DWORD dwPageIndex, DWORD dwPosIndex, DWORD dwItemID, int nCount, int nCost);
    BOOL DoShopSellRequest(DWORD dwShopID, DWORD dwItemID, int nCost, DWORD dwBoxIndex, DWORD dwPosIndex);
    BOOL DoShopRepairRequest(DWORD dwShopID, DWORD dwItemID, int nCost, DWORD dwBoxIndex, DWORD dwPosIndex);
    // ���󹺻ػع��б����һ����Ʒ
    BOOL DoSoldListBuyRequest(DWORD dwShopID, DWORD dwX, DWORD dwItemID);

    // ---------------------- Trading ----------------------->
    BOOL DoTradingInviteRequest(DWORD dwInviteDst);
    BOOL DoTradingInviteRespond(BOOL bAccept);
    BOOL DoTradingAddItem(DWORD dwBoxIndex, DWORD dwPosIndex, DWORD dwGridIndex);
    BOOL DoTradingDeleteItem(DWORD dwGridIndex);
    BOOL DoTradingSetMoney(int nMoney);
    BOOL DoTradingConfirm(BOOL bConfirm, int nKey);

    BOOL DoRequestTarget(int nTargetType, DWORD dwTargetID);

	BOOL DoApplyExchangeItem(DWORD dwBox1, DWORD dwX1, DWORD dwBox2, DWORD dwX2, DWORD dwAmount);

	BOOL DoWindowSelect(DWORD dwIndex, BYTE bySelect);
	BOOL DoCastProfessionSkill(DWORD dwCraftID, DWORD dwRecipeID, DWORD dwRBookItemID, KTarget* pTarget);

    BOOL DoLootItem(DWORD dwDoodadID, DWORD dwItemID);
    BOOL DoRollItem(DWORD dwDoodadID, DWORD dwItemID, BOOL bCancel);

	BOOL DoApplyLootList(DWORD dwDoodadID);
	BOOL DoCharacterOpen(DWORD dwDoodadID);
    BOOL DoDeathRespond(int nRespondCode);
	BOOL DoApplyUseItem(DWORD dwBox, DWORD dwX, KTarget& rTarget);
	BOOL DoApplyFightFlag(DWORD dwForceID, BOOL bFight);
	BOOL DoFinishLoot(DWORD dwDoodadID);
	BOOL DoLootMoney(DWORD dwDoodadID);
	BOOL DoChangePkMode(int nMode);
	BOOL DoDistributeItem(DWORD dwDoodadID, DWORD dwItemID, DWORD dwDstPlayerID);

    BOOL DoTalkMessage(int nMsgType, const char cszReceiver[], size_t uDataLen, BYTE* pbyData);
    
    // --------------------- ������� --------------------------->
    BOOL DoGetFellowshipListRequest(int nType); // Type 1: Friend, 2: Foe, 3: BlackList
    BOOL DoAddFellowshipRequest(const char cszAlliedPlayerName[], int nType);
    BOOL DoDelFellowshipRequest(DWORD dwAlliedPlayerID, int nType);
    BOOL DoAddFellowshipGroupRequest(const char cszGroupName[]);
    BOOL DoDelFellowshipGroupRequest(DWORD dwGroupID);
    BOOL DoRenameFellowshipGroupRequest(DWORD dwGroupID, const char cszGroupName[]);
    BOOL DoSetFellowshipRemarkRequest(DWORD dwAlliedPlayerID, const char cszRemark[]);
    BOOL DoSetFellowshipGroupRequest(DWORD dwAlliedPlayerID, DWORD dwGroupID);

    // --------------------- �ʼ���� -------------------------->
    BOOL DoSendMailRequest(int nRequestID, char szDestName[], DWORD dwBox, DWORD dwX, int nMoney, char szTitle[], char szText[]);
    BOOL DoGetMailListRequest();
    BOOL DoGetMailContent(DWORD dwMailID);
    BOOL DoGetMailAttachment(DWORD dwMailID, BOOL bGetMoney, BOOL bGetText, BOOL bGetItem);
    BOOL DoSetMailRead(DWORD dwMailID);
    BOOL DoDeleteMail(DWORD dwMailID);
    BOOL DoReturnMail(DWORD dwMailID);

	// --------------------- PK --------------------------->
	BOOL DoApplyPKOperate(BYTE byPKOperate, DWORD dwParam1, DWORD dwParam2);

	BOOL DoSyncUserPreferences(int nOffset, int nLength, BYTE* pbyData);
    BOOL DoSetBankOpenedFlag(BOOL bBankOpenedFlag, DWORD dwNpcID);
	BOOL DoEnableBankPackage();

	BOOL DoSyncMidMapMark(int nMapID, int nX, int nY, int nType, const char* pszComment);

    // --------------------- ����ѧϰ ------------------------->
	BOOL DoPlayerLearnSkill(DWORD dwSkillID, DWORD dwNpcID);
	BOOL DoPlayerLearnRecipe(DWORD dwCraftID, DWORD dwRecipeID, DWORD dwNpcID);

    // --------------------- �ؼ����� ------------------------->
    BOOL DoPlayerActiveSkillRecipe(DWORD dwSkillRecipeKey, BOOL bActive);

    // --------------------------------------------------------->
    BOOL DoPeekOtherPlayerEquip(DWORD dwPlayerID);
    BOOL DoPeekOtherPlayerBook(DWORD dwPlayerID);
    BOOL DoPeekOtherPlayerQuest(DWORD dwPlayerID);

    // --------------------- ��ͨϵͳ -------------------------->
    BOOL DoRoadTrackStartOut(int nFromNode, int nTargetCity, int nCost);
    BOOL DoRoadTrackForceRequest();

    // --------------------- ������� -------------------------->
    BOOL DoOpenVenation(int nVenationID);

    BOOL DoSendGmMessage(const char cszMessage[]);

    BOOL DoJoinBattleFieldQueueRequest(DWORD dwNpcID, DWORD dwMapID, int nCopyIndex, DWORD dwGroupID, BOOL bTeamJoin);
    BOOL DoLeaveBattleFieldQueueRequest(DWORD dwMapID);

    BOOL DoAcceptJoinBattleField(DWORD dwMapID, int nCopyIndex);
    BOOL DoGetBattleFieldList(DWORD dwMapID);
    BOOL DoLeaveBattleField();

    // --------------------- ������ -------------------------->
    BOOL DoApplyTongRosterRequest();
    BOOL DoApplyTongInfoRequest();
    BOOL DoInvitePlayerJoinTongRequest(const char* cpszNewMemberName);
    BOOL DoInvitePlayerJoinTongRespond(DWORD dwInviterID, DWORD dwTongID, BOOL bAccept);
    BOOL DoApplyKickOutTongMember(DWORD dwMemberID);

    BOOL DoModifyTongInfoRequest(int nDataType, BYTE* pbyData, size_t uDataLen);
    BOOL DoModifyTongSchemaRequest(int nGroupIndex, int nModifyType, BYTE* byData, size_t uDataLen);

    BOOL DoApplyQuitTongRequest();

    BOOL DoChangeTongMemberGroupRequest(DWORD dwTargetMemberID, int nTargetGroup);
    BOOL DoChangeTongMasterRequest(DWORD dwNewMasterID);
    BOOL DoChangeMemberRemarkRequest(DWORD dwTargetMemberID, const char* cpszRemark);
    BOOL DoGetTongDescriptionRequest(DWORD dwTongID);
    // -----------------------------------------------------------
    BOOL DoApplyBattleFieldStatistics();
    
    BOOL DoSetHeroFlag(BOOL bHeroFlag);

    // --------------------- ��������� -------------------------->
    BOOL DoAuctionLookupRequest(
        DWORD dwNpcID, BYTE byRequestID, const char* pszSaleName, int nAucGenre, int nAucSub, int nLevelMin,
        int nLevelMax, int nQuality, const char* pszSellerName, DWORD dwBidderID, int nTopPrice, int nAuction,
        int nStartIndex, BYTE byOrderType, BOOL bDesc
    );
    BOOL DoAuctionBidRequest(DWORD dwNpcID, DWORD dwSaleID, DWORD dwCRC, int nPrice);
    BOOL DoAuctionSellRequest(DWORD dwNpcID, DWORD dwBox, DWORD dwX, int nStartPrice, int nBuyItNowPrice, int nLeftHour);
    BOOL DoAuctionCancelRequest(DWORD dwNpcID, DWORD dwSaleID);
    
    BOOL DoApplySetCampRequest(KCAMP eNewCamp);

    BOOL DoRemoteLuaCall(const char cszFunction[], BYTE* pbyData, size_t uDataLen);

    BOOL DoPlayerReviveRequest(int nReviveCode);

    // --------------------- �ɾ�ϵͳ��� -------------------------->
    BOOL DoApplyAchievementDataRequest(DWORD dwPlayerID);
    BOOL DoQueryProductIdentity(int nMagic);
	//AutoCode:-����Э�麯������-

private:
	// ֪ͨ��Ϣ���ӻ�
	void OnMessageNotify(BYTE* pbyData, size_t nSize, int nFrame);

	// ͬ���������
	void OnSyncPlayerBaseInfo(BYTE* pbyData, size_t nSize, int nFrame);
	
	void OnSyncQuestData(BYTE* pbyData, size_t nSize, int nFrame);

	// �ߺ�֪ͨ
	void OnAccountKickout(BYTE* pbyData, size_t nSize, int nFrame);
	// �л���������֪ͨ
	void OnSwitchGS(BYTE* pbyData, size_t nSize, int nFrame);
    // �л���ͼ
    void OnSwitchMap(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncNewPlayer(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncNewNpc(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncNewDoodad(BYTE* pbyData, size_t nSize, int nFrame);

	void OnRemoveCharacter(BYTE* pbyData, size_t nSize, int nFrame);
    void OnRemoveDoodad(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncMoveCtrl(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncMoveParam(BYTE* pbyData, size_t nSize, int nFrame);
    void OnAdjustPlayerMove(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncMoveState(BYTE* pbyData, size_t nSize, int nFrame);

	void OnCharacterDeath(BYTE* pbyData, size_t nSize, int nFrame);
	void OnMoveCharacter(BYTE* pbyData, size_t nSize, int nFrame);
    void OnPlayerRevivedNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ����ɫ����
	void OnCharacterAction(BYTE* pbyData, size_t nSize, int nFrame);

    void OnOverTimeAction(BYTE* pbyData, size_t nSize, int nFrame);

	// ��ɫ��ʾ����
	void OnPlayerDisplayData(BYTE* pbyData, size_t nSize, int nFrame);

	// ���Ӿ���ֵ
	void OnSyncExperience(BYTE* pbyData, size_t nSize, int nFrame);

    void OnCommonHealth(BYTE* pbyData, size_t nSize, int nFrame);

    void OnPlayerLevelUp(BYTE* pbyData, size_t nSize, int nFrame);

	void OnSyncDoodadState(BYTE* pbyData, size_t nSize, int nFrame);

	void OnFrameSignal(BYTE* pbyData, size_t nSize, int nFrame);

    void OnTerrainTest(BYTE* pbyData, size_t nSize, int nFrame);
    void OnPingSignal(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncSkillData(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncSkillRecipe(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSkillPrepare(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSkillCast(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncCommonSkillAutoCastState(BYTE* pbyData, size_t nSize, int nFrame);
    void OnResetCooldown(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSkillChainEffect(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSkillEffectResult(BYTE* pbyData, size_t nSize, int nFrame);   
    void OnSyncBehitRepresent(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncBuffList(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncBuffSingle(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncRepresentState(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncKungfuMount(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSkillBeatBack(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncSheathState(BYTE* pbyData, size_t nSize, int nFrame);

    // --------------------- ��ֵͬ�� --------------------------

    void OnSyncSelfMaxLMRS(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncSelfCurrentLMRS(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncSelfWeakInfo(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncSelectCharacterMaxLMR(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncSelectCharacterCurrentLMR(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncSelectCharacterWeakInfo(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTeamMemberMaxLMR(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTeamMemberCurrentLMR(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTeamMemberMisc(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTeamMemberPosition(BYTE* pbyData, size_t nSize, int nFrame);
	void OnBroadcastCharacterLife(BYTE* pbyData, size_t nSize, int nFrame);

    // --------------------- party about -----------------------

    // ͬ�����������Ϣ
    void OnSyncPlayerTeamBaseInfo(BYTE* pbyData, size_t nSize, int nFrame);
    // xxx���������
    void OnInvitePlayerJoinTeamRequest(BYTE* pbyData, size_t nSize, int nFrame);
    // xxx���������Ķ���
    void OnApplyJoinTeamRequest(BYTE* pbyData, size_t nSize, int nFrame);
    // �����¼Ӷ�Ա��֪ͨ
    void OnTeamAddMemberNotify(BYTE* pbyData, size_t nSize, int nFrame);
    // ��Ա�뿪�����֪ͨ
    void OnTeamDelMemberNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnTeamChangeAuthorityNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnTeamSetLootModeNotify(BYTE* pbyData, size_t nSize, int nFrame);
    
    void OnTeamSetFormationLeaderNotify(BYTE* pbyData, size_t nSize, int nFrame);

    // �����ɢ֪ͨ
    void OnTeamDisbandNotify(BYTE* pbyData, size_t nSize, int nFrame);
    // ��Ա������֪ͨ
    void OnSyncTeamMemberOnlineFlag(BYTE* pbyData, size_t nSize, int nFrame);
    // ͬ��С��ͼ��ע��
    void OnTeamMemberSignpostNotify(BYTE* pbyData, size_t nSize, int nFrame);
    // С�ӱ��
    void OnTeamSetMarkRespond(BYTE* pbyData, size_t nSize, int nFrame);
    // С����Ӫ
    void OnTeamCampChange(BYTE* pbyData, size_t nSize, int nFrame);
    // ����Ϊ�Ŷ�
    void OnTeamLevelUpRaidNotify(BYTE* pbyData, size_t nSize, int nFrame);
    // ��Ա����λ��
    void OnTeamChangeMemberGroupNotify(BYTE* pbyData, size_t nSize, int nFrame);
    // <--------------------- party about ---------------------


    // --------------------- shop about --------------------->
    void OnOpenShopNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnUpdateShopItem(BYTE* pbyData, size_t nSize, int nFrame);
    void OnShopBuyRespond(BYTE* pbyData, size_t nSize, int nFrame);
    // <--------------------- shop about ---------------------
    
    // ---------------------- Trading ----------------------->
    void OnTradingInviteRequest(BYTE* pbyData, size_t nSize, int nFrame);
    void OnOpenTradingBoxNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTradingConfirm(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTradingItemData(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTradingMoney(BYTE* pbyData, size_t nSize, int nFrame);
    void OnTradingComplete(BYTE* pbyData, size_t nSize, int nFrame);
    // <--------------------- Trading ------------------------

	// ͬ����Ǯ����
	void OnSyncMoney(BYTE* pbyData, size_t nSize, int nFrame);
    // ͬ������ֵ
    void OnSyncContribution(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ�����ߵ���ʱ����
	void OnSyncTempItemData(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ����������
	void OnSyncItemData(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ������ɾ��
	void OnSyncItemDestroy(BYTE* pbyData, size_t nSize, int nFrame);
	// ����װ��
	void OnExchangeItem(BYTE* pbyData, size_t nSize, int nFrame);
    // ��װ�㲥
    void OnSyncEquipRepresent(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ�����߻�õ���Ϣ
	void OnAddItemNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// ������Ʒ�������;�
	void OnUpdateItemAmount(BYTE* pbyData, size_t nSize, int nFrame);
	// ѧϰ�����
	void OnLearnProfessionNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// ѧϰ����ܷ�֧
	void OnLearnBranchNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// ��������
	void OnForgetProfessionNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// ��������ܾ���
	void OnAddProfessionProficiency(BYTE* pbyData, size_t nSize, int nFrame);
	// ������������ȼ�
	void OnSetMaxProfessionLevelNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ��������������
	void OnUpdatePlayerProfession(BYTE* pbyData, size_t nSize, int nFrame);
	// ѧϰ�䷽
	void OnLearnRecipeNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// �����䷽
	void OnForgetRecipeNotify(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ���䷽�б�
	void OnSyncRecipeState(BYTE* pbyData, size_t nSize, int nFrame);
	// ��һ���ͻ��˴���
	void OnOpenWindow(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ���䷽��ʹ��
	void OnStartUseRecipe(BYTE* pbyData, size_t nSize, int nFrame);

	void OnAcceptQuest(BYTE* pbyData, size_t nSize, int nFrame);
	void OnFinishQuest(BYTE* pbyData, size_t nSize, int nFrame);
	void OnCancelQuest(BYTE* pbyData, size_t nSize, int nFrame);
    void OnClearQuest(BYTE* pbyData, size_t nSize, int nFrame);
	void OnShareQuest(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ���������
	void OnSyncQuestValue(BYTE* pbyData, size_t nSize, int nFrame);
	void OnStartEscortQuest(BYTE* pbyData, size_t nSize, int nFrame);

	// ͬ��ʰȡ�б�
	void OnSyncLootList(BYTE* pbyData, size_t nSize, int nFrame);
	// ��������Ϣ
	void OnRollItemMessage(BYTE* pbyData, size_t nSize, int nFrame);
	// ��ɫʰȡ��Ʒ
	void OnCharacterPick(BYTE* pbyData, size_t nSize, int nFrame);
	// ��ʰȡ���
	void OnOpenLootList(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ��ս������
	void OnSyncFightFlag(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ������
	void OnSyncRepute(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ����������ս������
	void OnSyncFightflagList(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ�����е���������
	void OnSyncReputeList(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ�������޸ı��
	void OnShowReputation(BYTE* pbyData, size_t nSize, int nFrame);

	void OnCharacterOpen(BYTE* pbyData, size_t nSize, int nFrame);
    void OnDistributeItemMessage(BYTE* pbyData, size_t nSize, int nFrame);

	// ����ʰȡ
	void OnFinishLoot(BYTE* pbyData, size_t nSize, int nFrame);
	// start roll
	void OnBeginRollItem(BYTE* pbyData, size_t nSize, int nFrame);
	// ͬ�����Ƶ��û���Ϣ
	void OnCustomOTAction(BYTE* pbyData, size_t nSize, int nFrame);
	// ��ӳ��
	void OnModifyThreat(BYTE* pbyData, size_t nSize, int nFrame);
	// �޸ı����
	void OnModifyThreaten(BYTE* pbyData, size_t nSize, int nFrame);
	// ������
	void OnClearThreat(BYTE* pbyData, size_t nSize, int nFrame);
	// ��������
	void OnClearThreaten(BYTE* pbyData, size_t nSize, int nFrame);

    void OnTalkMessage(BYTE* pbyData, size_t nSize, int nFrame);

	void OnSyncSelfCurrentST(BYTE* pbyData, size_t nSize, int nFrame);

	void OnDoodadLootOver(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncFellowshipList(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncFellowshipGroupName(BYTE* pbyData, size_t nSize, int nFrame);

	void OnNpcTalkToPlayer(BYTE* pbyData, size_t nSize, int nFrame);
	void OnSetForce(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSendMailRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnGetMailListRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnNewMailNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnGetMailContentRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnMailGeneralRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnMailReleaseAttachment(BYTE* pbyData, size_t nSize, int nFrame);

	void OnSyncForceId(BYTE* pbyData, size_t nSize, int nFrame);
	void OnSyncUserPreferences(BYTE* pbyData, size_t nSize, int nFrame);
	void OnSyncAllBookState(BYTE* pbyData, size_t nSize, int nFrame);
	void OnSyncBookState(BYTE* pbyData, size_t nSize, int nFrame);
	void OnCharacterChangeModel(BYTE* pbyData, size_t nSize, int nFrame);
	void OnSyncNpcDialogFlag(BYTE* pbyData, size_t nSize, int nFrame);

	void OnSyncPKState(BYTE* pbyData, size_t nSize, int nFrame);
	void OnWinDuelBroadcast(BYTE* pbyData, size_t nSize, int nFrame);
	void OnCancelDuelRespond(BYTE* pbyData, size_t nSize, int nFrame);

	void OnSlayRespond(BYTE* pbyData, size_t nSize, int nFrame);
    
	void OnSyncRoleDataOver(BYTE* pbyData, size_t nSize, int nFrame);

	void OnSyncEnableBankPackage(BYTE* pbyData, size_t nSize, int nFrame);

	void OnUpdateMapMark(BYTE* pbyData, size_t nSize, int nFrame);
	void OnSyncMidMapMark(BYTE* pbyData, size_t nSize, int nFrame);

	void OnPlaySound(BYTE* pbyData, size_t nSize, int nFrame);
	void OnPlayMiniGame(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncHorseFlag(BYTE* pbyData, size_t nSize, int nFrame);

    // ��ͨϵͳ���
    void OnRoadTrackForceRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncRouteNodeOpenList(BYTE* pbyData, size_t nSize, int mFrame);

    // ͬ�����״̬����
    void OnSyncPlayerStateInfo(BYTE* pbyData, size_t nSize, int mFrame);

    // ͬ����ҵ���Ϊ
    void OnSyncPlayerTrain(BYTE* pbyData, size_t nSize, int mFrame);

    void OnBroadcastPlayerSchoolID(BYTE* pbyData, size_t nSize, int mFrame);

    void OnSyncPrinkForce(BYTE* pbyData, size_t nSize, int mFrame);
    
    void OnSyncVisitMapInfo(BYTE* pbyData, size_t nSize, int mFrame);

    void OnSyncAccumulateValue(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSetTarget(BYTE* pbyData, size_t nSize, int nFrame);
    
    void OnSyncKillPoint(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSetPlayerRedName(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSetPlayerDialogFlag(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSetOnlyReviveInSitu(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncFormationCoefficient(BYTE* pbyData, size_t nSize, int nFrame);

    //------------------------ BATTLE_FIELD ------------------------------------
    void OnJoinBattleFieldQueueRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnLeaveBattleFieldQueueRespond(BYTE* pbyData, size_t nSize, int nFrame);

    void OnBattleFieldNotify(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncBattleFieldList(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncBattleFieldSide(BYTE* pbyData, size_t nSize, int nFrame);

    void OnBattleFieldEnd(BYTE* pbyData, size_t nSize, int nFrame);
    //--------------------------------------------------------------------------

    void OnSyncCampInfo(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncCurrentPrestige(BYTE* pbyData, size_t nSize, int nFrame);

    //------------------------      ���    ------------------------------------
    void OnSyncTongMemberInfo(BYTE* pbyData, size_t nSize, int nFrame);
    void OnTongDeleteMemberNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnApplyTongInfoRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnUpdateTongClientDataVersion(BYTE* pbyData, size_t nSize, int nFrame);
    void OnInvitePlayerJoinTongRequest(BYTE* pbyData, size_t nSize, int nFrame);
    void OnChangePlayerTongNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnBroadcastTongMessage(BYTE* pbyData, size_t nSize, int nFrame);
    void OnGetTongDescriptionRespond(BYTE* pbyData, size_t nSize, int nFrame);
    void OnBroadcastPlayerChangeTong(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncTongOnlineMessage(BYTE* pbyData, size_t nSize, int nFrame);
    //--------------------------------------------------------------------------

    //------------------------���������------------------------------------
    void OnAuctionLookupRespond(BYTE* pbyData, size_t uSize, int nFrame);
    void OnAuctionBidRespond(BYTE* pbyData, size_t uSize, int nFrame);
    void OnAuctionSellRespond(BYTE* pbyData, size_t uSize, int nFrame);
    void OnAuctionCancelRespond(BYTE* pbyData, size_t uSize, int nFrame);
    void OnAuctionMessageNotify(BYTE* pbyData, size_t uSize, int nFrame);

    void OnSetCamp(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncBattleFieldStatistics(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncBattleFieldObjective(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncBattleFieldPQInfo(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncHeroFlag(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncNpcDropTarget(BYTE* pbyData, size_t nSize, int nFrame);
    void OnRemoteLuaCall(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncPlayerReviveCtrl(BYTE* pbyData, size_t nSize, int nFrame);

    //------------------------�ɾ�ϵͳ���----------------------------
    void OnSyncAchievementData(BYTE* pbyData, size_t nSize, int nFrame);
    void OnNewAchievementNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnAchievementAnnounceNotify(BYTE* pbyData, size_t nSize, int nFrame);
    void OnSyncAchievementPoint(BYTE* pbyData, size_t nSize, int nFrame);

    void OnSyncMaxLevel(BYTE* pbyData, size_t nSize, int nFrame);
    void OnUseItemPrepare(BYTE* pbyData, size_t nSize, int nFrame);
	//AutoCode:-����Э�麯������-
};

extern KPlayerClient g_PlayerClient;

#endif	//_CLIENT
#endif	//_KPLAYER_CLIENT_H_
