/************************************************************************/
/* Relay��GS֮���Э�鶨��					                            */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.09	Create												*/
/************************************************************************/
#ifndef _RELAY_GS_PROTOCOL_H_
#define _RELAY_GS_PROTOCOL_H_

#include "ProtocolBasic.h"
#include "SO3ProtocolBasic.h"

enum RELAY_GS_PROTOCOL
{
	relay_gs_connection_begin = 0,

	//GS������Relay��ĳ�ʼ������
	g2r_initialize_data,
	r2g_initialize_data,

	//��ͼ�������Э��
	r2g_dml_create_mapcopy,
	g2r_dml_reply_create_mapcopy,
	r2g_dml_delete_mapcopy,
	g2r_dml_reply_delete_mapcoy,

	//���ͼ���Э��
	g2r_dml_search_mapcopy,
	r2g_dml_reply_search_mapcopy,
	g2r_dml_send_player_data,
	r2g_dml_send_player_data,
	g2r_dml_inform_player_guid,
	r2g_dml_inform_player_guid,
	r2g_dml_kick_player_out,

	//��ҵ�½���ǳ�
	r2g_login_request,
	g2r_reply_login_request,

	g2r_inform_player_login,
	r2g_reply_player_login,

	g2r_player_leave_gs,

	r2g_kick_player,

	//�������ݸ���Э��
	g2r_update_relation,
	r2g_update_relation,

	//�������ݸ���Э��

    // ------------- ������Э�� --------------------------------

	//�ͻ��˷��������GS���������ת����Relay����(�������һ���Ǳ�GS��)
	g2r_invite_player_join_party_request,
	//Relay�������������ӵ����󣬽�������ת��������������
	r2g_invite_player_join_party_request,
	//��������ҽ��յ�����������Ӧ��ͨ�����Э��ת����Relay
	g2r_invite_player_join_party_respond,
	//Relay���������Ļ�Ӧ,ע�����һ��˳���Ļ����ᷢ���Э�飬����ֱ���յ�������ӳ�Ա��Э��
	r2g_invite_player_join_party_respond,
    //GS���յ���ҵ��������󣬽�����ת����Relay����
    g2r_apply_join_party_request,
    //Relay�������������Ӧ��(ֻ��������ʧ�ܵ�����²Ż᷵�������Ϣ)
    r2g_apply_join_party_respond,
    //Relay����ҵ���������ԼӴ����ת����Ŀ���������GS
    r2g_apply_join_party_request,
    //�ӳ��������������Ӧ���ͨ�����Э��ת����Relay
    g2r_apply_join_party_respond,

    r2g_party_create_notify,
    r2g_party_add_member_notify,
    g2r_party_del_member_request,
    r2g_party_del_member_notify,
    g2r_party_change_leader_request,
    r2g_party_change_leader_notify,
    g2r_party_set_loot_mode_request,
	g2r_party_set_roll_quality_request,
    r2g_party_set_loot_mode_notify,
    g2r_party_disband_request,
    r2g_party_disband_notify,
    r2g_sync_party_member_online_flag,

    g2r_sync_party_member_max_lmr,
    g2r_sync_party_member_current_lmr,
    g2r_sync_party_member_misc,

    r2g_sync_party_member_max_lmr,
    r2g_sync_party_member_current_lmr,
    r2g_sync_party_member_misc,

    // ---------------- ������� --------------------------------------
    r2g_player_talk_error,

    g2r_player_talk_to_one,
    r2g_player_talk_to_one,
    g2r_player_talk_to_many,
    r2g_player_talk_to_many,

    // ----------------------------------------------------------------

    // ---------------- ������� --------------------------------------
    g2r_player_add_relation,
    r2g_player_add_relation,

    g2r_player_del_relation,
    r2g_player_del_relation,

    g2r_player_update_relation,
    r2g_player_update_relation,
    // ----------------------------------------------------------------

	relay_gs_connection_end
};

#pragma	pack(1)
//GS������Relay��ĳ�ʼ������
struct G2R_INITIALIZE_DATA : INTERNAL_PROTOCOL_HEADER
{
};

struct R2G_INITIALIZE_DATA : INTERNAL_PROTOCOL_HEADER
{
	int		nGSIndex;
};

//��ͼ�������Э��ṹ
struct R2G_DML_CREATE_MAPCOPY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;							//��ͼID
	DWORD		dwMapCopyIndex;					//��ͼ�������
	DWORD		dwPlayerID;						//���봴����ͼ����ҵ�ID
};

struct G2R_DML_REPLY_CREATE_MAPCOPY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;							//��ͼID
	DWORD		dwMapCopyIndex;					//��ͼ�������
	DWORD		dwPlayerID;						//���봴����ͼ����ҵ�ID
	BOOL		bResult;							//���ؽ����1 �ɹ�
};

struct R2G_DML_DELETE_MAPCOPY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;							//��ͼID
	DWORD		dwMapCopyIndex;					//��ͼ�������
	BOOL		bDirectDelete;					//�Ƿ�ֱ��ɾ��
};

struct G2R_DML_REPLY_DELETE_MAPCOY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwMapID;							//��ͼID
	DWORD		dwMapCopyIndex;					//��ͼ�������
	BOOL		bResult;							//ɾ�������1 �ɹ�
};

//���ͼ���Э��ṹ
struct G2R_DML_SEARCH_MAPCOPY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	DWORD		dwMapID;
	DWORD		dwMapCopyIndex;
};

struct R2G_DML_REPLY_SEARCH_MAPCOPY : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	BOOL		bResult;
	int			nDestConnIndex;
	DWORD		dwDestMapID;
	DWORD		dwDestMapCopyIndex;
};

struct G2R_DML_SEND_PLAYER_DATA : INTERNAL_PROTOCOL_HEADER
{
	DWORD			dwPlayerID;
	DWORD			dwDestMapID;
	DWORD			dwDestMapCopyIndex;

	char            szAccountName[_NAME_LEN];
	char            szRoleName[_NAME_LEN];  // ��ʵ��ɫ���ں�����������浱Ȼ��,�������︱�ϱ���д��־
	tagExtPointInfo	ExtPointInfo;			// ���õĸ��͵�
	tagExtPointInfo	ChangePointInfo;		// �仯�ĸ��͵�
	unsigned		ulLeftTime;				// ʣ��ʱ��,����Ϊ��λ,���������Ͱ�ʱ(���죬����)
	unsigned        uRoleDataLength;        // ��ɫ��Ϣ���ݵĳ���
	char			RoleInfo[0];			// ��ɫ����
};

struct R2G_DML_SEND_PLAYER_DATA : INTERNAL_PROTOCOL_HEADER
{
	DWORD			dwPlayerID;
	DWORD			dwDestMapID;
	DWORD			dwDestMapCopyIndex;

	char            szAccountName[_NAME_LEN];
	char            szRoleName[_NAME_LEN];  // ��ʵ��ɫ���ں�����������浱Ȼ��,�������︱�ϱ���д��־
	tagExtPointInfo	ExtPointInfo;			// ���õĸ��͵�
	tagExtPointInfo	ChangePointInfo;		// �仯�ĸ��͵�
	unsigned long	ulLeftTime;				// ʣ��ʱ��,����Ϊ��λ,���������Ͱ�ʱ(���죬����)
	unsigned        uRoleDataLength;        // ��ɫ��Ϣ���ݵĳ���
	char			RoleInfo[0];			// ��ɫ����
};

struct G2R_DML_INFORM_PLAYER_GUID : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	GUID		Guid;
	int			nGSAddr;
	int			nGSPort;
	BOOL		bResult;
};

struct R2G_DML_INFORM_PLAYER_GUID : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	GUID		Guid;
	int			nGSAddr;
	int			nGSPort;
	BOOL		bResult;
};

struct R2G_DML_KICK_PLAYER_OUT : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
};


//��ҵ�½���ǳ�
struct R2G_LOGIN_REQUEST : IDENTITY_HEADER
{
	char            szAccountName[_NAME_LEN];
	char            szRoleName[_NAME_LEN];  // ��ʵ��ɫ���ں�����������浱Ȼ��,�������︱�ϱ���д��־
    DWORD           dwPartyID;
	tagExtPointInfo	ExtPointInfo;			// ���õĸ��͵�
	tagExtPointInfo	ChangePointInfo;		// �仯�ĸ��͵�
	unsigned long   ulLeftTime;				// ʣ��ʱ��,����Ϊ��λ,���������Ͱ�ʱ(���죬����)
	unsigned        uRoleDataLength;        // ��ɫ��Ϣ���ݵĳ���
	char			RoleInfo[0];			// ��ɫ����
};

struct G2R_REPLY_LOGIN_REQUEST : IDENTITY_HEADER
{
	DWORD		dwPlayerID;
	BOOL		bResult;
	GUID		Guid;
	int			nGSAddr;
	int			nGSPort;
};

struct G2R_INFORM_PLAYER_LOGIN : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
};

struct R2G_REPLY_PLAYER_LOGIN : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPlayerID;
	BYTE		byResult;
};

struct G2R_PLAYER_LEAVE_GS : INTERNAL_PROTOCOL_HEADER
{
    DWORD       dwPlayerID;
};

struct R2G_KICK_PLAYER : INTERNAL_PROTOCOL_HEADER
{
	DWORD       dwPlayerID;
};

struct G2R_UPDATE_RELATION : INTERNAL_PROTOCOL_HEADER
{
	__int64			nPlayerIndexA;	//�����ˮ��
	__int64			nPlayerIndexB;	//�����ˮ��
	int				nRelationType;	//���ö�� RELATION_TYPE
	int				nIntimacy;		//�øж� 
};

struct R2G_UPDATE_RELATION : INTERNAL_PROTOCOL_HEADER
{
	__int64			nPlayerIndexA;	//�����ˮ��
	__int64			nPlayerIndexB;	//�����ˮ��
	int				nRelationType;	//���ö�� RELATION_TYPE
	int				nIntimacy;		//�øж� 
};

// ------------------ ������Э��ṹ���� ---------------------------

// ��GSת������������������
struct G2R_INVITE_PLAYER_JOIN_PARTY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwInviteSrc;
	char  szInviteRole[_NAME_LEN];
};

// Relay���������ת���������������ڵķ�����
struct R2G_INVITE_PLAYER_JOIN_PARTY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	char  szInviteSrc[_NAME_LEN];
	DWORD dwInviteDst;
    GUID  guid;
};

// Relay�������뷢���ߵ�Ӧ����Ϣ
struct R2G_INVITE_PLAYER_JOIN_PARTY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwInviteSrc;
    char  szInvitedPlayer[_NAME_LEN];
    int   nRespondCode;
};

// ��������ҵ�Ӧ����ϢGS==>Relay
struct G2R_INVITE_PLAYER_JOIN_PARTY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    char  szInviteSrc[_NAME_LEN];
    DWORD dwInviteDst;
    BOOL  bAgreeFlag;
    GUID  guid;
};

// ת����ҵ�������뵽Relay
struct G2R_APPLY_JOIN_PARTY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwApplySrc; // ���������������
    char  szApplyDst[_NAME_LEN]; // ������������ҵĶ���
};

// Relay����������ԼӴ����ת����Ŀ��������ڵķ�����
struct R2G_APPLY_JOIN_PARTY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwApplyDst;
    char  szApplySrc[_NAME_LEN];
    GUID  guid;
};

// ���ӳ�����������Ӧ��ת����Relay
struct G2R_APPLY_JOIN_PARTY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    char  szApplySrc[_NAME_LEN];
    DWORD dwApplyDst;
    BOOL  bAgreeFlag;
    GUID  guid;
};

struct R2G_APPLY_JOIN_PARTY_RESPOND : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwApplySrc;
    char  szLeader[_NAME_LEN];
    int   nRespondCode;
};

// �����ɢ����
struct G2R_PARTY_DISBAND_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwLeaderID;
    DWORD dwPartyID;
};

// �����ɢ��Ϣ�㲥
struct R2G_PARTY_DISBAND_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
};

// ��Ա������֪ͨ
struct R2G_SYNC_PARTY_MEMBER_ONLINE_FLAG : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwMemberID;
    BOOL  bOnlineFlag;
};


struct R2G_PARTY_CREATE_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwLeaderID;
    int   nLootMode;
	int	  nRollQuality;
    char  szLeaderName[_NAME_LEN];
    int   nLeaderPortraitID; // �ӳ�ͷ��ID
    BOOL  bLeaderOnlineFlag;
};

// ������ӳ�Ա��Ϣ�㲥
struct R2G_PARTY_ADD_MEMBER_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwMemberID;
    char  szMemberName[_NAME_LEN];
    int   nMemberPortraitID; // ͷ��ID
    BOOL  bMemberOnlineFlag;
};

// ����ɾ����Ա����
struct G2R_PARTY_DEL_MEMBER_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwRequestSrc; // �������ߵ�ID
    DWORD dwMemberID;
};

// ����ɾ����Ա��Ϣ�㲥
struct R2G_PARTY_DEL_MEMBER_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwMemberID; 
};

// ���ö���ӳ�����
struct G2R_PARTY_CHANGE_LEADER_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwRequestSrc; // �������ߵ�ID
    DWORD dwNewLeaderID;
};

// ����ӳ����֪ͨ
struct R2G_PARTY_CHANGE_LEADER_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwNewLeaderID;
};

// ���ö�����Ʒ���䷽ʽ
struct G2R_PARTY_SET_LOOT_MODE_REQUEST : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwRequestSrc;
    int   nLootMode;
};

struct G2R_PARTY_SET_ROLL_QUALITY_REQUEST : INTERNAL_PROTOCOL_HEADER
{
	DWORD dwPartyID;
	DWORD dwRequestSrc;
	int   nRollQuality;
};

// ��Ʒ����ģʽ���֪ͨ
struct R2G_PARTY_SET_LOOT_MODE_NOTIFY : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    int   nLootMode;
	int   nRollQuality;
};


struct G2R_SYNC_PARTY_MEMBER_MAX_LMR : INTERNAL_PROTOCOL_HEADER
{
    DWORD 	dwPartyID;
    DWORD 	dwMemberID;
    int   	nMaxLife; 
    int   	nMaxMana; 
    int   	nMaxRage;
};

struct G2R_SYNC_PARTY_MEMBER_CURRENT_LMR : INTERNAL_PROTOCOL_HEADER
{
    DWORD 	dwPartyID;
    DWORD 	dwMemberID;
    BYTE   	byLifePercent; 
    BYTE   	byManaPercent; 
    BYTE   	byRagePercent; 
};

struct G2R_SYNC_PARTY_MEMBER_MISC : INTERNAL_PROTOCOL_HEADER
{
    DWORD 	dwPartyID;
    DWORD 	dwMemberID;
    DWORD   dwPortraitID;
    DWORD   dwMapID;
    DWORD   dwForceID;
    BYTE    byLevel;
};

struct R2G_SYNC_PARTY_MEMBER_MAX_LMR : INTERNAL_PROTOCOL_HEADER
{
    DWORD 	dwPartyID;
    DWORD 	dwMemberID;
    int   	nMaxLife; 
    int   	nMaxMana; 
    int   	nMaxRage;
};

struct R2G_SYNC_PARTY_MEMBER_CURRENT_LMR : INTERNAL_PROTOCOL_HEADER
{
    DWORD 	dwPartyID;
    DWORD 	dwMemberID;
    BYTE   	byLifePercent; 
    BYTE   	byManaPercent; 
    BYTE   	byRagePercent; 
};

struct R2G_SYNC_PARTY_MEMBER_MISC : INTERNAL_PROTOCOL_HEADER
{
    DWORD 	dwPartyID;
    DWORD 	dwMemberID;
    DWORD   dwPortraitID;
    DWORD   dwMapID;
    DWORD   dwForceID;
    BYTE    byLevel;
};


// -------------- �������Э��ṹ���� ------------------------
struct R2G_PLAYER_TALK_ERROR : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwReceiverID;
    int   nErrorCode;
};

struct G2R_PLAYER_TALK_TO_ONE : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwTalkerID;
    char  szTalkTo[_NAME_LEN];
    BYTE  byTalkData[0];        // �Ի���������
};

struct R2G_PLAYER_TALK_TO_ONE : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwReceiverID;
    BYTE  byReplyFlag;          // �����Ƿ��Ƕ�˵���ߵĻ�Ӧ
    char  szName[_NAME_LEN];    // bReplyFlag = 1, �����ߵ�����; bReplyFlag = 0, ˵���˵�����;
    BYTE  byTalkData[0];        // �Ի���������
};

struct G2R_PLAYER_TALK_TO_MANY : INTERNAL_PROTOCOL_HEADER
{
    BYTE  byTalkRange;
    DWORD dwTalkerID;
    BYTE  byTalkData[0];        // �Ի���������
};

struct R2G_PLAYER_TALK_TO_MANY : INTERNAL_PROTOCOL_HEADER
{
    BYTE  byTalkRange;
    DWORD dwReceiverID;
    char  szTalker[_NAME_LEN];
    BYTE  byTalkData[0];        // �Ի���������
};
// -------------------------------------------------------------

// -------------- �������Э��ṹ���� -------------------------
struct G2R_PLAYER_ADD_RELATION : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    char  szAlliedPlayerName[_NAME_LEN];
};

struct R2G_PLAYER_ADD_RELATION : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    char  szAlliedPlayerName[_NAME_LEN];
    BYTE  byResult;
    DWORD dwAlliedPlayerID;
};

struct G2R_PLAYER_DEL_RELATION : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    DWORD dwAlliedPlayerID;
};

struct R2G_PLAYER_DEL_RELATION : INTERNAL_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    DWORD dwAlliedPlayerID;
    BYTE  byResult;
};
// -------------------------------------------------------------

#pragma pack()

#endif	//_RELAY_GS_PROTOCOL_H