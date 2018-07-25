/************************************************************************/
/* GS�Ϳͻ���֮��Э�鶨��					                            */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* Modify	 : Spe														*/
/* Remark	 : ��Ҫ�����޸ķָ������ע�ͣ����Զ����ɴ���ĺ�����Ҫʹ�� */
/* History	 :															*/
/*		2005.01.27	Create												*/
/************************************************************************/
#ifndef _GS_CLIENT_PROTOCOL_H_
#define _GS_CLIENT_PROTOCOL_H_

#include "Global.h"
#include "SO3ProtocolBasic.h"


//Э��Ŷ���
//GS 2 Client Protocol Begin
enum GS_CLIENT_PROTOCOL
{
    gs_client_connection_begin = 0,

    g2c_response_message,			// Ӧ����Ϣ
    g2c_frame_signal,				// ֡���
    g2c_terrain_test,               // ����Э��,���ڼ�� �����<==>�ͻ��� �ϰ���Ϣ��һ����

    // ��½��������ߺŵ����Э��
    g2c_sync_player_base_info,	    // ��¼ʱͬ���������
    g2c_sync_quest_state,			// ��¼ʱͬ������״̬
    g2c_sync_quest_list,			// ��½ʱͬ�������б�

    g2c_kick_self_out,				// �ߺ�֪ͨ
    g2c_change_game_server,			// ���֪ͨ

    // ͬ�����Э��
    g2c_sync_region_player,	        // ��һ��Region�е��������ͬ�����ͻ���
    g2c_sync_region_npc,	        // ��һ��Region�е��������ͬ�����ͻ���
    g2c_sync_new_player,	        // ֪ͨ�ͻ���: һ����ҽ���Region��
    g2c_sync_new_npc,	            // ֪ͨ�ͻ���: һ��npc����Region��

    g2c_move_character,				// �ƶ���ɫ��λ��
    g2c_remove_character,			// �ӳ������Ƴ���ɫ

    g2c_sync_doodad_state,			// Doodad״̬ͬ��

    g2c_player_display_data,		// �����ʾ����
    g2c_npc_display_data,			// Npc��ʾ����

    g2c_add_experience,				// ���Ӿ���ֵ
    g2c_common_health,              // ��ͨ�˺���������,����ˤ��֮���

    // ��ɫ�ƶ�ͬ��
    g2c_revise_move_state,          // �����ͻ�����ҵ��ƶ�״̬
    g2c_sync_move_state,            // �ͻ��˷����ƶ�ָ���ʱ��,�������˷������겻��,��֪ͨ�ͻ�������

    g2c_character_stand,
    g2c_character_walk,				// ��ɫ�߶���ָ��ͬ�����ݰ�
    g2c_character_run,				// ��ɫ�ܶ���ָ��ͬ�����ݰ�
    g2c_character_swim,				// ��ɫ��Ӿ��ָ��ͬ�����ݰ�
    g2c_character_jump,				// ��ɫ��Ծ��ָ��ͬ�����ݰ�
    g2c_character_sit,
    g2c_character_meditate,         // ����ͬ��

    g2c_character_action,			// ��ɫ����������ͬ�����ݰ�
    g2c_character_dash,
    g2c_character_knocked_down,
    g2c_character_knocked_back,
    g2c_character_knocked_off,
    g2c_character_stun,
    g2c_character_halt,
    g2c_character_death,			// ��ɫ������ͬ�����ݰ�
    g2c_player_revived_notify,      // ��ұ������ͬ�����ݰ�
    g2c_character_say,				// ��ɫ˵����ͬ�����ݰ�
    g2c_character_turn,				// ͬ����ɫת��
    g2c_over_time_action,

    // ------------- skill about protocol -------------------
    g2c_sync_skill_data,			//��¼����ѧϰ�¼���ʱͬ����������
    g2c_skill_prepare,			    
    g2c_skill_cast,
    g2c_sync_common_skill_auto_cast_state, // ���ּ����Զ��ͷ�״̬
    g2c_reset_skill_cooldown,
    g2c_skill_effect_result,
    g2c_sync_behit_represent,
    g2c_sync_fight_state,
    g2c_sync_buff_list,		
    g2c_sync_buff_single,		
    g2c_sync_represent_state,
    g2c_sync_kungfu_mount,
    g2c_skill_beat_back,
    g2c_sync_sheath_state,

    // ------------- ��ֵͬ�� -----------------------------

    g2c_sync_self_max_lmrs,
    g2c_sync_self_current_lmrs,
    g2c_sync_target_max_lmr,
    g2c_sync_target_current_lmr,
    g2c_sync_party_member_max_lmr,
    g2c_sync_party_member_current_lmr,
    g2c_sync_party_member_misc,
    g2c_sync_party_member_coordinate,
    g2c_broadcast_character_life,

    // ------------- party about protocol -----------------
    g2c_sync_player_party_base_info,    // ��Ҹոյ�½��Ϸ��ʱ��ͬ�������Ļ�����Ϣ: ID,�ӳ���Ϣ,���䷽ʽ ... ...
    g2c_sync_player_party_member_info,  // ��Ҹոյ�½��Ϸ��ʱ��ͬ�������ĳ�Ա�б�,һ��ֻ����һ����Ա����Ϣ
    g2c_invite_join_party_request,     
    g2c_invite_join_party_respond,
    g2c_apply_join_party_request,
    g2c_apply_join_party_respond,
    g2c_party_create_notify,            // ֪ͨ�ӳ�"�㽨����һ֧����",ԭ����������Э����һ��,���Ǻ��������������һЩ����д�úܸ���:)
    g2c_player_join_party_notify,       // ֪ͨ��ң������Լ�������ĳ����
    g2c_party_add_member_notify,        // ֪ͨ�������г�Ա"ĳ�˼��������ǵĶ���"
    g2c_party_del_member_notify,
    g2c_sync_party_leader_flag,         // ֪ͨ������ҵĶӳ����
    g2c_party_change_leader_notify,
    g2c_party_set_distribute_mode_notify,
    g2c_party_disband_notify,
    g2c_sync_party_member_online_flag,
    g2c_party_member_signpost_notify,   // ֪ͨ����С��ͼ�ϵı��
    // <------------- party about protocol -------------

    // ------------- shop about protocol ------------->
    g2c_open_shop_notify,
    g2c_update_shop_item,
    g2c_shop_buy_respond,
    // <------------- shop about protocol -------------
    
    // -------------- Trading about ------------------>
    g2c_trading_invite_request,
    g2c_open_trading_box_notify,
    g2c_sync_trading_confirm,
    g2c_sync_trading_item_data,    
    g2c_sync_trading_money,
    g2c_trading_complete,
    // <------------- Trading about ------------------

    g2c_sync_money,					//ͬ����Ǯ����
	g2c_sync_temp_item_data,		//ͬ����ʱ��������
    g2c_sync_item_data,				//ͬ����������
    g2c_sync_item_remove,			//ͬ������ɾ��
    g2c_exchange_item,				//����װ��
    g2c_update_item_amount,			//������Ʒ�������;�
    g2c_sync_equip_represent,       //��װ�㲥

    g2c_learn_profession_notify,	//ѧϰ�����
    g2c_learn_recipe_notify,		//ѧϰ�䷽
    g2c_forget_profession_notify,		//��������
    g2c_forget_recipe_notify,		//�����䷽

    g2c_add_profession_proficiency,		//��������ܾ���
    g2c_set_max_profession_level_notify,//������������ȼ�
    g2c_update_player_profession,		//ͬ��������������
    g2c_update_player_recipe,			//ͬ����������䷽�б�
    g2c_cast_recipe,

    g2c_open_window,				//��һ���ͻ��˴���

    g2c_start_use_recipe,			//ͬ���䷽��ʹ��
    g2c_end_cast,					//���������ͷ�

    g2c_accept_quest,				//֪ͨ�ͻ��˽�������
    g2c_finish_quest,				//֪ͨ�ͻ����������
    g2c_cancel_quest,				//֪ͨ�ͻ���ȡ������
    g2c_share_quest,				//��������
    g2c_sync_quest_value,			//ͬ���������
	g2c_share_quest_result,			//ͬ��������Ľ��

    g2c_sync_loot_list,				//ͬ��ʰȡ�б�
    g2c_roll_item_message,			//��������Ϣ
    g2c_character_pick,				//��ɫʰȡ��Ʒ
    g2c_open_loot_list,				//��ʰȡ���
	g2c_sync_fight_flag,			//ͬ��ս������
	g2c_sync_repute,				//ͬ������
	g2c_sync_fightflag_list,		//ͬ����������ս������
	g2c_sync_repute_list,			//ͬ�����е���������
	g2c_character_open,				//
	g2c_roll_money_message,			//��ӹ㲥��Ǯ��ʰȡ
	g2c_finish_loot,				//����ʰȡ
	g2c_begin_roll_item,			//start roll
	g2c_custom_OTAction,			//ͬ�����Ƶ��û���Ϣ
	

	g2c_modify_threat,				//��ӳ��
	g2c_modify_threaten,			//�޸ı����
	g2c_clear_threat,				//������
	g2c_clear_threaten,				//��������
    g2c_player_talk_to_one,         //������Ϣ
    g2c_player_talk_to_many,        //Ⱥ����Ϣ
	g2c_update_double_exp,			//ͬ��˫������
	g2c_update_player_stamina,		//
    g2c_update_player_qi,           //ͬ��'��''��''��'�е�'��'
	g2c_doodad_loot_over,			//

    g2c_player_add_relation,
    g2c_player_del_relation,

	gs_client_connection_end
};
//GS 2 Client Protocol End

// Client 2 GS Protocol Begin
enum CLIENT_GS_PROTOCOL
{
    client_gs_connection_begin = 0,

    // ��½��������ߺ����Э��
    c2g_apply_player_data,			// ��¼ʱ�����������
    c2g_ack_player_data,			// ȷ��ͬ������������
    c2g_player_logout,				// ��ҵǳ�

    c2g_character_stand,            // ��ɫվ�����ϴ�ָ��
    c2g_character_walk,				// ��ɫ�߶����ϴ�ָ��
    c2g_character_run,				// ��ɫ�ܶ����ϴ�ָ��
    c2g_character_swim,				// ��ɫ��Ӿ���ϴ�ָ��
    c2g_character_jump,				// ��ɫ��Ծ���ϴ�ָ��
    c2g_character_sit,				// ��ɫ���µ��ϴ�ָ��

    c2g_character_action,			// ��ɫ�����������ϴ�ָ��
    c2g_character_say,				// ��ɫ˵�����ϴ�ָ��
    c2g_character_turn,				// ��ɫ������ϴ�ָ��

    // ͬ�����Э��
    c2g_apply_new_character_data,	// �����½�ɫ������
    c2g_apply_player_display_data,	// �����ʾ����
    c2g_apply_npc_display_data,		// Npc��ʾ����

    c2g_stop_current_action,        // �ж���ҵ�ǰ���ڽ��е���Ϊ

    // ------------ skill about protocol ------------->
    c2g_character_skill,			//��ɫ�������ϴ�ָ��
    c2g_mount_kungfu,
    c2g_cancel_buff,
    c2g_set_sheath_state,
    // <------------ skill about protocol -------------

    // ------------- party about protocol ------------->
    c2g_invite_join_party_request, 
    c2g_invite_join_party_respond, 
    c2g_apply_join_party_request,
    c2g_apply_join_party_respond,
    c2g_party_del_member_request,
    c2g_party_change_master_request,
    c2g_party_set_distribute_mode_request,
	c2g_party_set_roll_Quality_request,
    c2g_party_member_notify_signpost_request,   // ����ѷ���С��ͼ�ϵı��
    // <------------- party about protocol -------------

    //�������Э��
    c2g_accept_quest,				//��������
    c2g_cancel_quest,				//ȡ������
    c2g_finish_quest,				//�������
    c2g_share_quest,				//��������

    //GMָ���ϴ�Э��
    c2g_gm_command,

    // ------------- shop about protocol ------------->
    c2g_open_shop_request,
    c2g_query_shop_items,
    c2g_shop_buy_request,
    c2g_shop_sell_request,
    c2g_shop_repair_request,
    c2g_sold_list_buy_request,
    // <------------- shop about protocol -------------

    // ------------- Trading about ------------------->
    c2g_trading_invite_request,
    c2g_trading_invite_respond,
    c2g_trading_add_item,
    c2g_trading_delete_item,
    c2g_trading_set_money,
    c2g_trading_confirm,
    // <-------------Trading about --------------------

    c2g_set_target,
    c2g_apply_exchange_item,		//���뽻��װ��
    c2g_apply_use_item,				//����ʹ�õ���

    c2g_window_select,				//ѡ�񴰿�ѡ��

    c2g_cast_profession_skill,			//�䷽ʹ��Э��

    c2g_loot_or_roll_item,			//ʰȡ��Ʒ����������
    c2g_apply_loot_list,			//����ʰȡ�б�
    c2g_character_open,				//��
    c2g_death_respond,

	c2g_apply_fight_flag,			//�ı�ս������

	c2g_finish_loot,				//����ʰȡ
	c2g_loot_money,					//ʰȡ��Ǯ
	c2g_change_pk_mode,				//�ı�PK״̬
	c2g_leader_distribute_item,		//�ӳ�������Ʒ

    c2g_player_talk_to_one,         //������Ϣ
    c2g_player_talk_to_many,        //Ⱥ����Ϣ

    c2g_player_add_relation,
    c2g_player_del_relation,

    client_gs_connection_end
};
// Client 2 GS Protocol End

//Э��ṹ����
#pragma	pack(1)

//GS 2 Client Struct Begin

// Ӧ����Ϣ
struct G2C_RESPONSE_MESSAGE : DOWNWARDS_PROTOCOL_HEADER
{
    BYTE	byErrorCodeType;
    BYTE	byErrorCode;
};

// ��¼ʱͬ����һ�����Ϣ
struct G2C_SYNC_PLAYER_BASE_INFO : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwPlayerID;
    char	szRoleName[_NAME_LEN];
    BYTE    byRoleType;
    bool    bAlive;
    DWORD	dwMapID;
    DWORD	dwMapCopyIndex;
    int		nX;
    int		nY;
    int		nZ;
    BYTE    byFaceDirection;
    BYTE    byLevel;
    int     nExp;
	int		nDoubleExp;
    int		nGameLoop;
    WORD    wRepresentID[REPRESENT_COUNT];
};


// ͬ������״̬
struct G2C_SYNC_QUEST_STATE : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD	dwPlayerID;
    char	szData[0];
};

//ͬ�������б�
struct G2C_SYNC_QUEST_LIST : UNDEFINED_SIZE_DOWNWARDS_HEADER 
{
    DWORD	dwPlayerID;
    char	szData[0];
};

//�ߺ�֪ͨ
struct G2C_KICK_SELF_OUT : DOWNWARDS_PROTOCOL_HEADER
{

};

//���֪ͨ
struct G2C_CHANGE_GAME_SERVER : DOWNWARDS_PROTOCOL_HEADER
{
    char            szRoleName[_NAME_LEN];
    GUID            Guid;
    UINT			nIPAddr;
    unsigned short	wPort;	
};

// ͬ��Region�е�������ҵ��ͻ���
struct G2C_SYNC_REGION_PLAYER : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    BYTE	byRegionX; // Region�ڵ�ͼ�е�X����
    BYTE	byRegionY; // Region�ڵ�ͼ�е�Y����
    struct	KPLAYER_INFO
    {
        DWORD	dwID;
        WORD	wRepresentState;
        BYTE    byFaceDirection;
        // uX, uY: ��ɫ��������Region�е�ƫ��, 11 bits
        UINT64  uPartyLeaderFlag    : 1;                            // �Ƿ�ӳ�
        UINT64	uX			        : (REGION_GRID_WIDTH_BIT_NUM  + CELL_LENGTH_BIT_NUM);	// 11 bits
        UINT64  uY			        : (REGION_GRID_HEIGHT_BIT_NUM + CELL_LENGTH_BIT_NUM);   // 11 bits
        UINT64	uZ				    : MAX_Z_POINT_BIT_NUM;	        // 22 bits
        UINT64  uVelocityXY         : MAX_ZOOM_VELOCITY_BIT_NUM;      // 11  bits
        UINT64  uDirectionXY        : DIRECTION_BIT_NUM;            // 8 bits
        // <-- 64 bits ---
        INT64   nVelocityZ          : MAX_VELOCITY_Z_BIT_NUM;       // 10 bits
        UINT64  uConvergenceSpeed   : MAX_VELOCITY_XY_BIT_NUM;      // ��̬�ٶ� 7 bits
        UINT64  uDestX              : MAX_X_COORDINATE_BIT;	        // 17 bits
        UINT64  uDestY              : MAX_Y_COORDINATE_BIT;	        // 17 bits
        UINT64  uCurrentGravity     : MAX_GRAVITY_BIT_NUM;          // 5 bits
        UINT64  uSwimAbility        : 1;                            // ��Ӿ����
        UINT64  uWaterFlyAbility    : 1;                            // ˮ��Ư����
        UINT64  uFightState         : 1;                            // ս��״̬
        UINT64  uSheathFlag         : 1;                            // �ν�״̬
        UINT64  uMoveState          : MAX_MOVE_STATE_BIT_NUM;       // 4 bits
        // <-- 64 bits ---
    } PlayerData[0];
};

// ͬ��Region�е�����NPC���ͻ���
struct G2C_SYNC_REGION_NPC : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    BYTE	byRegionX; // Region�ڵ�ͼ�е�X����
    BYTE	byRegionY; // Region�ڵ�ͼ�е�Y����
    struct	KNPC_INFO
    {
        DWORD	dwID;
        WORD	wRepresentState;
        BYTE    byFaceDirection;
        // uX, uY: ��ɫ��������Region�е�ƫ��, 11 bits
        UINT64	uX			        : (REGION_GRID_WIDTH_BIT_NUM  + CELL_LENGTH_BIT_NUM);	// 11 bits
        UINT64  uY			        : (REGION_GRID_HEIGHT_BIT_NUM + CELL_LENGTH_BIT_NUM);   // 11 bits
        UINT64	uZ				    : MAX_Z_POINT_BIT_NUM;	        // 22 bits
        UINT64  uVelocityXY         : MAX_ZOOM_VELOCITY_BIT_NUM;    // 11 bits
        UINT64  uDirectionXY        : DIRECTION_BIT_NUM;            // 8 bits
        UINT64  uSheathFlag         : 1;                            // �ν�״̬
        // <-- 64 bits ---
        INT64   nVelocityZ          : MAX_VELOCITY_Z_BIT_NUM;       // 10 bits
        UINT64  uConvergenceSpeed   : MAX_VELOCITY_XY_BIT_NUM;      // ��̬�ٶ� 7 bits
        UINT64  uDestX              : MAX_X_COORDINATE_BIT;	        // 17 bits
        UINT64  uDestY              : MAX_Y_COORDINATE_BIT;	        // 17 bits
        UINT64  uCurrentGravity     : MAX_GRAVITY_BIT_NUM;          // 5 bits
        UINT64  uSwimAbility        : 1;                            // ��Ӿ����
        UINT64  uWaterFlyAbility    : 1;                            // ˮ��Ư����
        UINT64  uFightState         : 1;                            // ս��״̬
        UINT64  uMoveState          : MAX_MOVE_STATE_BIT_NUM;       // 4 bits
        // <-- 63 bits ---
    } NpcData[0];
};

// ͬ���½������ҵ��ͻ���
struct G2C_SYNC_NEW_PLAYER : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwID;
    WORD	wRepresentState;
    BYTE    byFaceDirection;
    UINT64  uMoveState          : MAX_MOVE_STATE_BIT_NUM;       // 4 bits
    UINT64  uPosX               : MAX_X_COORDINATE_BIT;	        // 17 bits
    UINT64  uPosY               : MAX_Y_COORDINATE_BIT;         // 17 bits
    UINT64  uPosZ               : MAX_Z_POINT_BIT_NUM;          // 22 bits
    UINT64  uSwimAbility        : 1;                            // ��Ӿ����
    UINT64  uWaterFlyAbility    : 1;                            // ˮ��Ư����
    UINT64  uFightState         : 1;                            // ս��״̬
    UINT64  uSheathFlag         : 1;                            // �ν�״̬
    // <-- 64 bits ---
    UINT64  uConvergenceSpeed   : MAX_VELOCITY_XY_BIT_NUM;      // ��̬�ٶ� 7 bits
    UINT64  uDirectionXY        : DIRECTION_BIT_NUM;            // 8 bits
    INT64   nVelocityZ          : MAX_VELOCITY_Z_BIT_NUM;       // 10 bits
    UINT64  uDestX              : MAX_X_COORDINATE_BIT;	        // 17 bits
    UINT64  uDestY              : MAX_Y_COORDINATE_BIT;	        // 17 bits
    UINT64  uCurrentGravity     : MAX_GRAVITY_BIT_NUM;          // 5 bits
    // <-- 64 bits ---
    UINT16  uVelocityXY         : MAX_ZOOM_VELOCITY_BIT_NUM;    // 11  bits
    UINT16  uPartyLeaderFlag    : 1;                            // �Ƿ�ӳ�
    // <-- 12 bits ---
};

// ͬ���½����NPC���ͻ���
struct G2C_SYNC_NEW_NPC : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwID;
    WORD	wRepresentState;
    BYTE    byFaceDirection;
    UINT64  uMoveState          : MAX_MOVE_STATE_BIT_NUM;       // 4 bits
    UINT64  uPosX               : MAX_X_COORDINATE_BIT;	        // 17 bits
    UINT64  uPosY               : MAX_Y_COORDINATE_BIT;         // 17 bits
    UINT64  uPosZ               : MAX_Z_POINT_BIT_NUM;          // 22 bits
    UINT64  uSwimAbility        : 1;                            // ��Ӿ����
    UINT64  uWaterFlyAbility    : 1;                            // ˮ��Ư����
    UINT64  uFightState         : 1;                            // ս��״̬
    UINT64  uSheathFlag         : 1;                            // �ν�״̬
    // <-- 64 bits ---
    UINT64  uConvergenceSpeed   : MAX_VELOCITY_XY_BIT_NUM;      // ��̬�ٶ� 7 bits
    UINT64  uDirectionXY        : DIRECTION_BIT_NUM;            // 8 bits
    INT64   nVelocityZ          : MAX_VELOCITY_Z_BIT_NUM;       // 10 bits
    UINT64  uDestX              : MAX_X_COORDINATE_BIT;	        // 17 bits
    UINT64  uDestY              : MAX_Y_COORDINATE_BIT;	        // 17 bits
    UINT64  uCurrentGravity     : MAX_GRAVITY_BIT_NUM;          // 5 bits
    // <-- 64 bits ---
    UINT16  uVelocityXY         : MAX_ZOOM_VELOCITY_BIT_NUM;    // 11 bits
};



// �ӳ������Ƴ���ɫ
struct G2C_REMOVE_CHARACTER : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD		dwCharacterID;
};

// Doodad״̬ͬ��
struct G2C_SYNC_DOODAD_STATE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD		dwDoodadID;
    DWORD		dwTemplateID;
    int			nLifePercent;
    int			nDoodadInfo;
    int			nKind;
	int			nState;
	DWORD		dwNpcTemplateID;
	DWORD		dwRepresentID;
    unsigned    nX : MAX_X_COORDINATE_BIT;
    unsigned	nY : MAX_Y_COORDINATE_BIT;
    unsigned	nZ : MAX_Z_ALTITUDE_BIT_NUM;			//Z����
	int			nFaceDirection;
	BOOL		bLooted;
};


// �ͻ�����ҵ�˲ʱ�ƶ�״̬ͬ��
struct G2C_REVISE_MOVE_STATE :  DOWNWARDS_PROTOCOL_HEADER
{
    int     nFrame;
    char    cMoveState;
    int     nX;
    int     nY;
    int     nZ;
    BYTE    byFaceDirection;
    int     nDestX;
    int     nDestY;
    int     nVelocityXY;
    BYTE    byDirectionXY; 
    int     nVelocityZ;
    int     nConvergenceSpeed;
    BYTE    byCurrentGravity;
    char    cJumpCount;
};

// ��ɫ���ƶ������仯(�����ܵ�����Ч��)ʱ,���¹㲥�Լ����ƶ�����
struct G2C_SYNC_MOVE_STATE : DOWNWARDS_PROTOCOL_HEADER 
{
    DWORD	    dwCharacterID;
    UINT64      uPosX                   : MAX_X_COORDINATE_BIT;	     // 17 bits
    UINT64      uPosY                   : MAX_Y_COORDINATE_BIT;      // 17 bits
    UINT64      uPosZ                   : MAX_Z_POINT_BIT_NUM;       // 22 bits
    UINT64      uFaceDirection          : DIRECTION_BIT_NUM;         // 8  bits
    // <-- 64 bit --
    UINT64      uMoveState              : MAX_MOVE_STATE_BIT_NUM;    // 4 bits
    UINT64      uVelocityXY             : MAX_ZOOM_VELOCITY_BIT_NUM; // 11 bits
    UINT64      uDirectionXY            : DIRECTION_BIT_NUM;         // 8 bits
    INT64       nVelocityZ              : MAX_VELOCITY_Z_BIT_NUM;    // 10 bits
    UINT64      uConvergenceSpeed       : MAX_VELOCITY_XY_BIT_NUM;   // 8 bits, ��̬�����ٶ�
    UINT64      uCurrentGravity         : MAX_GRAVITY_BIT_NUM;       // 5 bits
    UINT64      uSwimAbility            : 1;      
    UINT64      uWaterFlyAbility        : 1;  
    // <-- 48 bits --
};

struct G2C_CHARACTER_STAND : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX                  : MAX_X_COORDINATE_BIT;	      // 17 bits
    UINT64      uSrcY                  : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ                  : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection         : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT32      uVelocityXY            : MAX_ZOOM_VELOCITY_BIT_NUM;  // 11 bits
    UINT32      uDirectionXY           : DIRECTION_BIT_NUM;          // 8 bits
    INT32       nVelocityZ             : MAX_VELOCITY_Z_BIT_NUM;     // 10 bits    
    // <-- 29 bits
};

struct G2C_CHARACTER_WALK : DOWNWARDS_PROTOCOL_HEADER				
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX                   : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uSrcY                   : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ                   : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection          : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT64      uVelocityXY             : MAX_ZOOM_VELOCITY_BIT_NUM;   // 11 bits
    UINT64      uDirectionXY            : DIRECTION_BIT_NUM;           // 8 bits
    INT64       nVelocityZ              : MAX_VELOCITY_Z_BIT_NUM;      // 10 bits
    UINT64      uDestX                  : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uDestY                  : MAX_Y_COORDINATE_BIT;	       // 17 bits
    // <-- 63 bits
    UINT16      uMoveState              : MAX_MOVE_STATE_BIT_NUM;      // 4 bits
    UINT16      uConvergenceSpeed       : MAX_VELOCITY_XY_BIT_NUM;     // 8 bits, ��̬�����ٶ�
};


struct G2C_CHARACTER_RUN : DOWNWARDS_PROTOCOL_HEADER				
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX                   : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uSrcY                   : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ                   : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection          : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT64      uVelocityXY             : MAX_ZOOM_VELOCITY_BIT_NUM;   // 11 bits
    UINT64      uDirectionXY            : DIRECTION_BIT_NUM;           // 8 bits
    INT64       nVelocityZ              : MAX_VELOCITY_Z_BIT_NUM;      // 10 bits
    UINT64      uDestX                  : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uDestY                  : MAX_Y_COORDINATE_BIT;	       // 17 bits
    // <-- 63 bits
    UINT16      uMoveState              : MAX_MOVE_STATE_BIT_NUM;      // 4 bits
    UINT16      uConvergenceSpeed       : MAX_VELOCITY_XY_BIT_NUM;     // 8 bits, ��̬�����ٶ�
};

struct G2C_CHARACTER_SWIM : DOWNWARDS_PROTOCOL_HEADER				
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX                   : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uSrcY                   : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ                   : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection          : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT64      uVelocityXY             : MAX_ZOOM_VELOCITY_BIT_NUM;   // 11 bits
    UINT64      uDirectionXY            : DIRECTION_BIT_NUM;           // 8 bits
    INT64       nVelocityZ              : MAX_VELOCITY_Z_BIT_NUM;      // 10 bits
    UINT64      uDestX                  : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uDestY                  : MAX_Y_COORDINATE_BIT;	       // 17 bits
    // <-- 63 bits
    UINT32      uMoveState              : MAX_MOVE_STATE_BIT_NUM;      // 4 bits
    UINT32      uConvergenceSpeed       : MAX_VELOCITY_XY_BIT_NUM;     // 8 bits, ��̬�����ٶ�
    UINT32      uPitchDirection         : DIRECTION_BIT_NUM;           // 8 bits ������
};

struct G2C_CHARACTER_JUMP : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX                   : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uSrcY                   : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ                   : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection          : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT64      uVelocityXY             : MAX_ZOOM_VELOCITY_BIT_NUM;   // 11 bits
    UINT64      uDirectionXY            : DIRECTION_BIT_NUM;           // 8 bits
    INT64       nVelocityZ              : MAX_VELOCITY_Z_BIT_NUM;      // 10 bits
    UINT64      uDestX                  : MAX_X_COORDINATE_BIT;	       // 17 bits
    UINT64      uDestY                  : MAX_Y_COORDINATE_BIT;	       // 17 bits
    // <-- 63 bits
    UINT16      uMoveState              : MAX_MOVE_STATE_BIT_NUM;      // 4 bits
    UINT16      uConvergenceSpeed       : MAX_VELOCITY_XY_BIT_NUM;     // 8 bits, ��̬�����ٶ�
};

struct G2C_CHARACTER_SIT : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	  // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;    // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;     // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;       // 8  bits
    // <-- 64 bit
};

struct G2C_CHARACTER_MEDITATE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	  // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;    // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;     // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;       // 8  bits
    // <-- 64 bit
};

struct G2C_CHARACTER_DASH : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	      // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT64      uVelocityXY        : MAX_ZOOM_VELOCITY_BIT_NUM;   // 11 bits
    UINT64      uDirectionXY       : DIRECTION_BIT_NUM;           // 8 bits
    INT64       nVelocityZ         : MAX_VELOCITY_Z_BIT_NUM;      // 10 bits
    UINT64      uDestX             : MAX_X_COORDINATE_BIT;	      // 17 bits
    UINT64      uDestY             : MAX_Y_COORDINATE_BIT;	      // 17 bits
    // <-- 63 bits
};

struct G2C_CHARACTER_KNOCKED_DOWN : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    WORD        wFrame;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	  // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;    // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;     // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;       // 8  bits
    // <-- 64 bit
};

struct G2C_CHARACTER_KNOCKED_BACK : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	      // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT64      uVelocityXY        : MAX_ZOOM_VELOCITY_BIT_NUM;   // 11 bits
    UINT64      uDirectionXY       : DIRECTION_BIT_NUM;           // 8 bits
    INT64       nVelocityZ         : MAX_VELOCITY_Z_BIT_NUM;      // 10 bits
    UINT64      uDestX             : MAX_X_COORDINATE_BIT;	      // 17 bits
    UINT64      uDestY             : MAX_Y_COORDINATE_BIT;	      // 17 bits
    // <-- 63 bits
};

struct G2C_CHARACTER_KNOCKED_OFF : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	      // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;        // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;         // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;           // 8  bits
    // <-- 64 bit
    UINT64      uVelocityXY        : MAX_ZOOM_VELOCITY_BIT_NUM;   // 11 bits
    UINT64      uDirectionXY       : DIRECTION_BIT_NUM;           // 8 bits
    INT64       nVelocityZ         : MAX_VELOCITY_Z_BIT_NUM;      // 10 bits
    UINT64      uDestX             : MAX_X_COORDINATE_BIT;	      // 17 bits
    UINT64      uDestY             : MAX_Y_COORDINATE_BIT;	      // 17 bits
    // <-- 63 bits
};

struct G2C_CHARACTER_STUN : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	  // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;    // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;     // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;       // 8  bits
    // <-- 64 bit
};

struct G2C_CHARACTER_HALT : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	    dwCharacterID;
    UINT64      uSrcX              : MAX_X_COORDINATE_BIT;	  // 17 bits
    UINT64      uSrcY              : MAX_Y_COORDINATE_BIT;    // 17 bits
    UINT64      uSrcZ              : MAX_Z_POINT_BIT_NUM;     // 22 bits
    UINT64      uFaceDirection     : DIRECTION_BIT_NUM;       // 8  bits
    // <-- 64 bit
};

// ��ɫ������ͬ�����ݰ�
struct G2C_CHARACTER_DEATH : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwCharacterID;
    DWORD	dwKillerID;
    DWORD   dwPunishTime;  // �����ͷ�ʱ��
    UINT64  uSrcX              : MAX_X_COORDINATE_BIT;	  // 17 bits
    UINT64  uSrcY              : MAX_Y_COORDINATE_BIT;    // 17 bits
    UINT64  uSrcZ              : MAX_Z_POINT_BIT_NUM;     // 22 bits
    UINT64  uFaceDirection     : DIRECTION_BIT_NUM;       // 8  bits
    // <-- 64 bit
};

// �ƶ���ɫλ��
struct G2C_MOVE_CHARACTER : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwID;
    DWORD   dwMapID;
    DWORD   dwMapCopyIndex;
    BYTE    byFaceDirection;
    UINT64  uPosX               : MAX_X_COORDINATE_BIT;	        // 17 bits
    UINT64  uPosY               : MAX_Y_COORDINATE_BIT;         // 17 bits
    UINT64  uPosZ               : MAX_Z_POINT_BIT_NUM;          // 22 bits
    UINT64  uDirectionXY        : DIRECTION_BIT_NUM;            // 8 bits
    // <-- 64 bits ---
    UINT32  uVelocityXY         : MAX_ZOOM_VELOCITY_BIT_NUM;    // 11  bits
    INT32   nVelocityZ          : MAX_VELOCITY_Z_BIT_NUM;       // 10 bits
    UINT32  uConvergenceSpeed   : MAX_VELOCITY_XY_BIT_NUM;      // ��̬�ٶ� 7 bits
    UINT32  uMoveState          : MAX_MOVE_STATE_BIT_NUM;       // 4 bits
    // <-- 32 bits ---
};


// ��ɫ����������ͬ�����ݰ�
struct G2C_CHARACTER_ACTION : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwSrcCharacterID;
    DWORD	dwDestCharacterID;
    BYTE	byAction;
};


struct G2C_PLAYER_REVIVED_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;    // A����B, A��ID
};

//��ɫ˵����ͬ�����ݰ�
struct G2C_CHARACTER_SAY : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD	dwCharacterID;
    char	szText[0];		//�Ի������ַ���
};

// ͬ����ɫת��
struct G2C_CHARACTER_TURN : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwCharacterID;
    BYTE	byDirection;
};

struct G2C_OVER_TIME_ACTION : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
    char  cActionType;
    WORD  wActionID;
    BYTE  byDirection;
    WORD  wDurationFrame;
};

// �����ʾ����
struct G2C_PLAYER_DISPLAY_DATA : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwPlayerID;
    char	szRoleName[_NAME_LEN];
    BYTE	byRoleType;
    BYTE    byLevel;
    WORD	wRepresentId[REPRESENT_COUNT];
};

// Npc��ʾ����
struct G2C_NPC_DISPLAY_DATA : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwNpcID;
    DWORD	dwTemplateID;
	int		nLevel;
	DWORD	dwRepresentID;
};

// ��ȡ����ֵ
struct G2C_ADD_EXPERIENCE : DOWNWARDS_PROTOCOL_HEADER
{
    int		nExpIncrement;
	BOOL	bUseDoubleExp;
};

struct G2C_COMMON_HEALTH :  DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
    int   nDeltaLife;
};

//֡����ź�
struct G2C_FRAME_SIGNAL : DOWNWARDS_PROTOCOL_HEADER
{
    int	nGameLoop;
};


struct G2C_TERRAIN_TEST : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    int  nX;
    int  nY;
    BYTE byCellCount;
    struct KCELL_INFO 
    {
        bool bBlockCharacter;
        BYTE byCellType;
        BYTE bySlipDirection;
        BYTE byGradient;
        BYTE byBarrierDirection;
	    WORD wLowLayer;	        // �²�߶�(�Ժ�ƽ��Ϊ��׼)
	    WORD wHighLayer;        // �ϲ�߶�(�Ժ�ƽ��Ϊ��׼)
    } CellInfo[0];
};


// ------------------ skill about protocol ----------------->

//ͬ����������,bySkillLevelΪ0��ζ���������û����(Forget)
struct G2C_SYNC_SKILL_DATA : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwSkillID;
    BYTE	bySkillLevel;
    DWORD   dwProficiency;
};

struct G2C_SKILL_PREPARE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwCharacterID;
    DWORD	dwSkillID;
    BYTE	bySkillLevel;
    int     nPrepareFrame;
    BYTE    byTargetType;		    //TARGET_TYPE����
    int	    nParam1;			    //���TargetType�������꣬�������ʾ��ɫID
    int	    nParam2;
    int	    nParam3;
};

struct G2C_SKILL_CAST : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
    DWORD dwSkillID;
    BYTE  bySkillLevel;
    DWORD dwBulletID;           //�ü������������ӵ�ID    
    bool  bSubCast;
    BYTE  byTargetType;		    //TARGET_TYPE����
    int	  nParam1;			    //���TargetType�������꣬�������ʾ��ɫID
    int	  nParam2;
    int	  nParam3;
};

struct G2C_SYNC_COMMON_SKILL_AUTO_CAST_STATE : DOWNWARDS_PROTOCOL_HEADER
{
    bool bStart;
    bool bMelee;
};

struct G2C_RESET_SKILL_COOLDOWN :  DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwSkillID;
    BYTE  bySkillLevel;
};

// ���ܴ��,��ֹ,������������ֹ,ͨ��������ֹ
struct G2C_SKILL_BREAK :  DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
};

// ��������
struct  G2C_PREPARE_BACK : DOWNWARDS_PROTOCOL_HEADER
{
};

struct G2C_SKILL_EFFECT_RESULT : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD dwSkillSrc;   // ���ܷ����ߵ�ID
    DWORD dwSkillDst;
    struct
    {
        BYTE    byType;
        DWORD   dwID;
        BYTE    byLevel;  
    } EffectSrc;
  
    BYTE  bShield : 1;
    BYTE  bPhysicsBlock : 1;
    BYTE  bSolarMagicBlock : 1;
    BYTE  bNeutralMagicBlock : 1;
    BYTE  bLunarMagicBlock : 1;
    BYTE  bPoisonBlock : 1;
    BYTE  bReact : 1;
    BYTE  bHitTarget : 1;
    BYTE  bSkillDodge : 1;
    BYTE  bCriticalStrike : 1;
    BYTE  bDebuffDodge : 1;
    BYTE  bDebuffImmunity : 1;
    BYTE  bDotDodge : 1;
    DWORD dwDebuffID;
    BYTE  byDebuffLevel;
    DWORD dwDotID;
    BYTE  byDotLevel;

    char  cResultCount;
    struct KSKILL_RESULT 
    {
        char cEffectResultType;
        int  nValue;
    } SkillResult[0];
};

struct G2C_SYNC_BEHIT_REPRESENT: UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD dwSkillSrc;
    DWORD dwSkillDst;
    DWORD dwBulletID;
    
    struct
    {
        BYTE    byType;
        DWORD   dwID;
        BYTE    byLevel;  
    } EffectSrc;

    int bReact: 1;
    int bHitTarget : 1;
    int bSkillDodge : 1;
    int bCriticalStrike : 1;    

    int bPhysicsBlock : 1;
    int bSolarMagicBlock : 1;
    int bNeutralMagicBlock : 1;
    int bLunarMagicBlock : 1;
    int bPoisonBlock : 1;

    int bPhysicsDamage : 1;
    int bSolarMagicDamage : 1;
    int bNeutralMagicDamage : 1;
    int bLunarMagicDamage : 1;
    int bPoisonDamage : 1;
    int bReflectiedDamage : 1;  

    int bTherapy : 1;          
    int bStealLife : 1;         
    int bAbsorbDamage : 1;     
    int bShieldDamage : 1;
};

struct G2C_SYNC_FIGHT_STATE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
    bool  bFightState;
};

struct G2C_SYNC_BUFF_LIST : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD dwCharacterID;
    WORD  wDataSize;
    BYTE  byData[0];
};

struct G2C_SYNC_BUFF_SINGLE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD   dwCharacterID;
    int     nIndex;
    char    cStyle;
    DWORD	dwBuffID;
    char    cLevel;
    char    cStackNum;
    int     nEndFrame;
};

struct G2C_SYNC_REPRESENT_STATE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
    WORD  wRepresentState;
};

struct G2C_SYNC_KUNGFU_MOUNT :  DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwID;
    DWORD dwLevel;
};

struct G2C_SKILL_BEAT_BACK :  DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
    int   nBeatBackFrame;
	BOOL  bBreak;
};

struct G2C_SYNC_SHEATH_STATE :  DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharacterID;
    bool  bSheathFlag;
};

// ------------------  ������ֵͬ�� -----------------------------

struct G2C_SYNC_SELF_MAX_LMRS : DOWNWARDS_PROTOCOL_HEADER
{
    int nMaxLife;
    int nMaxMana;
    int nMaxRage;
};

struct G2C_SYNC_SELF_CURRENT_LMRS : DOWNWARDS_PROTOCOL_HEADER
{
    int nCurrentLife;
    int nCurrentMana;
    int nCurrentRage;
};

struct G2C_SYNC_TARGET_MAX_LMR : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD   dwCharacterID;
    int     nMaxLife;
    int     nMaxMana;
    int     nMaxRage;
};

struct G2C_SYNC_TARGET_CURRENT_LMR : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD   dwCharacterID;
    BYTE    byLifePercent;
    BYTE    byManaPercent;
    BYTE    byRagePercent;
};

struct G2C_SYNC_PARTY_MEMBER_MAX_LMR : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD   dwMemberID;
    int     nMaxLife;
    int     nMaxMana;
    int     nMaxRage;
};

struct G2C_SYNC_PARTY_MEMBER_CURRENT_LMR : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD   dwMemberID;
    BYTE    byLifePercent;
    BYTE    byManaPercent;
    BYTE    byRagePercent;
};

struct G2C_SYNC_PARTY_MEMBER_MISC : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwMemberID;
    DWORD dwPortraitID;
    DWORD dwMapID;
    DWORD dwForceID;
    BYTE  byLevel;    
};

struct G2C_SYNC_PARTY_MEMBER_COORDINATE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwMemberID;
    int   nX;
    int   nY;
};

struct G2C_BROADCAST_CHARACTER_LIFE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD		dwCharacterID;
    BYTE	    byLifePercent;
};

// ------------------ party about protocol ------------------>

// ��ҵ�½������ʱͬ�����������Ϣ
struct G2C_SYNC_PLAYER_PARTY_BASE_INFO : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwLeaderID;
    int   nLootMode;
	int   nRollQuality;
};

// ��ҵ�½������ʱͬ�������Ա��Ϣ,ÿ��ֻͬ��һ����Ա
struct G2C_SYNC_PLAYER_PARTY_MEMBER_INFO : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwMemberID;
    char  szMemberName[_NAME_LEN];
    DWORD dwPortraitID; // ͷ��ID
    DWORD dwMapID;
    DWORD dwForceID;
    BYTE  byLevel;
    bool  bOnlineFlag;
    int   nMaxLife; 
    int   nMaxMana; 
    int   nMaxRage;
    BYTE  byLifePercent; 
    BYTE  byManaPercent; 
    BYTE  byRagePercent;
    int   nPosX;
    int   nPosY;
};

// xxx���������
struct G2C_INVITE_JOIN_PARTY_REQUEST : DOWNWARDS_PROTOCOL_HEADER
{
    char  szInviteSrc[_NAME_LEN]; // ���������������
    GUID  guid;
};

// ����������Ӧ��: Ŀǰֻ�жԷ��ܾ����ʱ�ŷ����Э��
struct G2C_INVITE_JOIN_PARTY_RESPOND : DOWNWARDS_PROTOCOL_HEADER
{
    char  szInvitedRole[_NAME_LEN];
    int   nRespondCode;
};

// xxx�������xxx�Ķ���
struct G2C_APPLY_JOIN_PARTY_REQUEST : DOWNWARDS_PROTOCOL_HEADER
{
    char  szApplySrc[_NAME_LEN]; // ���������������
    GUID  guid;
};

// ����������Ӧ(ֻ��ʧ�ܵ�ʱ��Ż��������Ӧ)
struct G2C_APPLY_JOIN_PARTY_RESPOND : DOWNWARDS_PROTOCOL_HEADER
{
    char  szApplyDst[_NAME_LEN];
    int   nRespondCode;
};

// ֪ͨ���"�㽨����һ֧����"
struct G2C_PARTY_CREATE_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
};

// ֪ͨ���"�����xxx�Ķ���"
struct G2C_PLAYER_JOIN_PARTY_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    char  szLeaderName[_NAME_LEN];
};

// ���A��Ҹոռ���ĳ����,֪ͨ������������г�Ա"A�ļ����˶���"
struct G2C_PARTY_ADD_MEMBER_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwMemberID;
    char  szMemberName[_NAME_LEN];
    DWORD dwPortraitID;
    bool  bOnlineFlag;
};

// ֪ͨ�ͻ���xxx�뿪xxx�Ķ���
struct G2C_PARTY_DEL_MEMBER_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwMemberID;
};

struct G2C_SYNC_PARTY_LEADER_FLAG : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    bool  bLeaderFlag; //�Ǳ�Ϊ�ӳ������ǴӶӳ���ɷǶӳ�
};

struct G2C_PARTY_CHANGE_LEADER_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwNewLeaderID;
};

struct G2C_PARTY_SET_LOOT_MODE_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwPartyID;
	int   nLootMode;
	int   nRollQuality;
};

struct G2C_PARTY_DISBAND_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
};

struct G2C_SYNC_PARTY_MEMBER_ONLINE_FLAG : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPartyID;
    DWORD dwMemberID;
    bool  bOnlineFlag;
};

struct  G2C_PARTY_MEMBER_SIGNPOST_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    int   nX;
    int   nY;
};

// <------------------ party about protocol ------------------


// ------------------ shop about protocol ------------------>

struct G2C_OPEN_SHOP_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD         dwShopID;
    char          cShopType;
    BYTE          byValidPage;
    bool          bCanRepair;
    DWORD         dwNpcID;
};


// ͬ���̵������Ϣ
struct G2C_UPDATE_SHOP_ITEM : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD   dwShopID;
    BYTE    byPageIndex;
    BYTE    byPosIndex;
    DWORD   dwItemID;
    int     nPrice;
    int     nCount;
    int     nLimit;
    BYTE    byReputeLevel;
    char    cSyncType;
    WORD    wItemDataLength;
    BYTE    byItemData[0];

};

struct G2C_SHOP_BUY_RESPOND : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD   dwShopID;
    BYTE    byPageIndex;
    BYTE    byPosIndex;
    char    cRespondCode;
};

// ע�⣬������Ʒʧ�ܵ�ʱ��Żᷢ�����Э��
// Ŀǰֻ����NPC�̵�����������ʧ�ܵ����,��������̵������������
struct G2C_SHOP_SELL_RESPOND : DOWNWARDS_PROTOCOL_HEADER
{
};


// <------------------ shop about protocol ------------------

struct G2C_TRADING_INVITE_REQUEST : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwTradingInviteSrc;
};

struct G2C_OPEN_TRADING_BOX_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwOtherPlayer;
};

struct G2C_SYNC_TRADING_CONFIRM : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    bool  bConfirm;
};

struct G2C_SYNC_TRADING_ITEM_DATA : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    BYTE    byItemType;
    DWORD   dwItemOwnerID;
    BYTE    byBoxIndex;
    BYTE    byPosIndex;
    BYTE    byGridIndex;
    BYTE    byStackNum;     // ��Ʒ�����������;ö�
    int     nKey;
    char    szBuff[0];
};

struct G2C_SYNC_TRADING_MONEY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    int   nMoney;
    int   nKey;
};

struct G2C_TRADING_COMPLETE : DOWNWARDS_PROTOCOL_HEADER
{
    bool bComplete;
};

// ͬ����Ǯ����
struct G2C_SYNC_MONEY : DOWNWARDS_PROTOCOL_HEADER
{
    int						nMoney;
};

// ͬ����ʱ��������
struct G2C_SYNC_TEMP_ITEM_DATA : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
	BYTE					nItemType;
	char					szBuff[0];
};
 

// ͬ����������
struct G2C_SYNC_ITEM_DATA : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    BYTE					nItemType;
    BYTE					byBox;
    BYTE					byX;
    char					szBuff[0];
};

// ͬ������ɾ��
struct G2C_SYNC_ITEM_REMOVE : DOWNWARDS_PROTOCOL_HEADER
{
    BYTE					byBoX;
    BYTE					byX;
};


// ����װ��
struct G2C_EXCHANGE_ITEM : DOWNWARDS_PROTOCOL_HEADER
{
    BYTE					byBox1;
    BYTE					byX1;
    BYTE					byBox2;
    BYTE					byX2;
};

// ��װ�㲥
struct G2C_SYNC_EQUIP_REPRESENT : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    char  cInventoryType;
    WORD  wRepresentId;
};


// ������Ʒ�������;�
struct G2C_UPDATE_ITEM_AMOUNT : DOWNWARDS_PROTOCOL_HEADER
{
    BYTE					byBox;
    BYTE					byX;
    WORD					wAmount;
};


// ѧϰ�����
struct G2C_LEARN_PROFESSION_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwProfessionID;
};


// ѧϰ�䷽
struct G2C_LEARN_RECIPE_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwCraftID;
    DWORD dwRecipeID;
};


// ��������
struct G2C_FORGET_PROFESSION_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwProfessionID;
};


// �����䷽
struct G2C_FORGET_RECIPE_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwCraftID;
    DWORD dwRecipeID;
};


// ��������ܾ���
struct G2C_ADD_PROFESSION_PROFICIENCY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwProfessionID;
    DWORD dwExp;
};


// ��������ܵȼ�
struct G2C_SET_MAX_PROFESSION_LEVEL_NOTIFY : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwProfessionID;
    DWORD dwLevel;
};


// ͬ��������������
struct G2C_UPDATE_PLAYER_PROFESSION : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwProfessionID;
    DWORD dwMaxLevel;
    DWORD dwCurrentLevel; 
    DWORD dwCurrentProficiency;
};


// ͬ����������䷽�б�
struct G2C_UPDATE_PLAYER_RECIPE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwRecipeID;
    BOOL  bIsUsed;
    DWORD dwEndCoolDownFrame;
};

struct G2C_CAST_RECIPE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
};

// ��һ���ͻ��˴���
struct G2C_OPEN_WINDOW : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD	dwIndex;			//��ˮ��
	DWORD	dwTargetType;		//Ŀ������ Doodad? Npc?
	DWORD	dwTargetID;			//Ŀ��ID
    char	szText[0];
};


// ͬ���䷽��ʹ��
struct G2C_START_USE_RECIPE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwPlayerID;
    DWORD	dwRecipeID;
    BYTE	byTargetType;
    int		nParam1;
    int		nParam2;
};

// ���������ͷ�
struct G2C_END_CAST : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwCastEndType;
};


// ֪ͨ�ͻ��˽�������
struct G2C_ACCEPT_QUEST : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwQuestID;
};


// ֪ͨ�ͻ����������
struct G2C_FINISH_QUEST : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwQuestID;
};


// ֪ͨ�ͻ���ȡ������
struct G2C_CANCEL_QUEST : DOWNWARDS_PROTOCOL_HEADER
{
    int		nQuestIndex;
};

// ��������
struct G2C_SHARE_QUEST : DOWNWARDS_PROTOCOL_HEADER
{
    BYTE	byTargetType;
    DWORD	dwTargetID;
    DWORD	dwQuestID;
};

// ͬ���������
struct G2C_SYNC_QUEST_VALUE : DOWNWARDS_PROTOCOL_HEADER
{
    BYTE	byQuestIndex;
    DWORD	dwKillNpcCount[QUEST_PARAM_COUNT];
    int		nQuestValue[QUEST_PARAM_COUNT * 2];
};


// ͬ��ʰȡ�б�
struct G2C_SYNC_LOOT_LIST : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    DWORD	dwDoodadID;
	int		nMoney; 
	int		nLootMode;
	int		nRollQuality;
	int		dwLooterIDList[MAX_PARTY_SIZE];
    char	szData[0];
};


// ��������Ϣ
struct G2C_ROLL_ITEM_MESSAGE : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD	dwPlayerID;
    DWORD	dwDoodadID;
    DWORD	dwItemID;
    BOOL	bLooted;
    int		nRandom;
};


// ��ɫʰȡ��Ʒ
struct G2C_CHARACTER_PICK : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwCharactorID;
};


// ��ʰȡ���
struct G2C_OPEN_LOOT_LIST : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwPlayerID;
    DWORD dwDoodadID;
};


// ͬ��ս������
struct G2C_SYNC_FIGHT_FLAG : DOWNWARDS_PROTOCOL_HEADER
{
	BYTE byForceID;
	BYTE byOpen;
};

// ͬ������
struct G2C_SYNC_REPUTE : DOWNWARDS_PROTOCOL_HEADER
{
	BYTE byForceID;
	short shRepute;
};


// ͬ����������ս������
struct G2C_SYNC_FIGHTFLAG_LIST : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
	BYTE	byFightFlagCount;
	BYTE	byFightFlag[0];
};


// ͬ�����е���������
struct G2C_SYNC_REPUTE_LIST : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
	BYTE	byForceCount;
	short	shRepute[0];	
};

// 
struct G2C_CHARACTER_OPEN : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwCharacterID;
	DWORD dwDoodadID;
};


// ��ӹ㲥��Ǯ��ʰȡ
struct G2C_ROLL_MONEY_MESSAGE : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwDoodadID;
};


// ����ʰȡ
struct G2C_FINISH_LOOT : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	DWORD dwDoodadID;
};


// start roll
struct G2C_BEGIN_ROLL_ITEM : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwItemID;
	DWORD dwDoodadID;
};


// ͬ�����Ƶ��û���Ϣ
struct G2C_CUSTOM_OTACTION : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwPlayerID;
	int nType;
	BOOL bIsBreakable;
	int nFrame;
	int nTargetType;
	int nTargetID;
	DWORD dwScriptID;
	char szActionName[CUSTOM_OTACTION_NAME_LEN];
};


// ͬ��������Ľ��
struct G2C_SHARE_QUEST_RESULT : DOWNWARDS_PROTOCOL_HEADER
{
	int nResult;
	DWORD dwDstPlayerID;
	BOOL bAccept;
};


// ��ӳ��
struct G2C_MODIFY_THREAT : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwCharacterID;
};


// �޸ı����
struct G2C_MODIFY_THREATEN : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwCharacterID;
};


// ������
struct G2C_CLEAR_THREAT : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwCharacterID;
};


// ��������
struct G2C_CLEAR_THREATEN : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwCharacterID;
};


// ������Ϣ
struct G2C_PLAYER_TALK_TO_ONE : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    BYTE byReplyFlag;           // �����Ƿ��Ƕ�˵���ߵĻ�Ӧ
    char szName[_NAME_LEN];     // bReplyFlag = 1, �����ߵ�����; bReplyFlag = 0, ˵���˵�����;
    BYTE byTalkData[0];         // �Ի���������
};

// Ⱥ����Ϣ
struct G2C_PLAYER_TALK_TO_MANY : UNDEFINED_SIZE_DOWNWARDS_HEADER
{
    BYTE byTalkRange;
    char szTalker[_NAME_LEN];   // ˵���˵�����
    BYTE byTalkData[0];         // �Ի���������
};


// ͬ��˫������
struct G2C_UPDATE_DOUBLE_EXP : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwDoubleExp;
};


// 
struct G2C_UPDATE_PLAYER_STAMINA : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwStamina;
};

struct G2C_UPDATE_PLAYER_QI : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwQi;
};

// 
struct G2C_DOODAD_LOOT_OVER : DOWNWARDS_PROTOCOL_HEADER
{
	DWORD dwDoodadID;
};

struct G2C_PLAYER_ADD_RELATION : DOWNWARDS_PROTOCOL_HEADER
{
    char  szAlliedPlayerName[_NAME_LEN];
    BYTE  byResult;
    DWORD dwAlliedPlayerID;
};

struct G2C_PLAYER_DEL_RELATION : DOWNWARDS_PROTOCOL_HEADER
{
    DWORD dwAlliedPlayerID;
    BYTE  byResult;
};

//AutoCode:GS 2 Client Struct End

/************************************************************************/
//Client 2 GS Struct Begin

//��¼ʱ�����������
struct C2G_APPLY_PLAYER_DATA : UPWARDS_PROTOCOL_HEADER
{
    char	szName[_NAME_LEN];
    GUID	Guid;
};

//��¼ʱȷ��ͬ������������
struct C2G_ACK_PLAYER_DATA : UPWARDS_PROTOCOL_HEADER
{
};

//��ҵǳ�
struct C2G_PLAYER_LOGOUT : UPWARDS_PROTOCOL_HEADER
{

};

struct C2G_CHARACTER_STAND : UPWARDS_PROTOCOL_HEADER
{
    int     nSrcX;
    int     nSrcY;
    int     nSrcZ;
    BYTE	byFaceDirection;
};


struct C2G_CHARACTER_WALK : UPWARDS_PROTOCOL_HEADER
{
    int     nSrcX;
    int     nSrcY;
    int     nSrcZ;
    int		nDstX;
    int		nDstY;
    BYTE	byFaceDirection;
};

struct C2G_CHARACTER_RUN : UPWARDS_PROTOCOL_HEADER
{
    int     nSrcX;
    int     nSrcY;
    int     nSrcZ;
    int		nDstX;
    int		nDstY;
    BYTE	byFaceDirection;
};

struct C2G_CHARACTER_SWIM : UPWARDS_PROTOCOL_HEADER
{
    int     nSrcX;
    int     nSrcY;
    int     nSrcZ;
    int		nDstX;
    int		nDstY;
    int     nDstZ;
    BYTE	byFaceDirection;
    BYTE    byPitchDirection;
};


struct C2G_CHARACTER_JUMP : UPWARDS_PROTOCOL_HEADER
{
    int     nSrcX;
    int     nSrcY;
    int     nSrcZ;
    bool    bStandJump;
    BYTE    byJumpDirection;
    BYTE	byFaceDirection;
};

struct C2G_CHARACTER_SIT :  UPWARDS_PROTOCOL_HEADER
{
    int     nSrcX;
    int     nSrcY;
    int     nSrcZ;
    BYTE	byFaceDirection;
};

// ��ҵĽ��������ϴ�ָ��
struct C2G_CHARACTER_ACTION : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwCharacterID;
    BYTE	byAction;
};

//���˵���ϴ�ָ��
struct C2G_CHARACTER_SAY : UNDEFINED_SIZE_UPWARDS_HEADER
{
    char	szText[0];			//˵������
};

// ��ɫ�����ϴ�ָ��
struct C2G_CHARACTER_TURN : UPWARDS_PROTOCOL_HEADER
{
    BYTE	byDirection;
};

// �����½�ɫ������
struct C2G_APPLY_NEW_CHARACTER_DATA : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwCharacterID;
};

// �����ʾ����
struct C2G_APPLY_PLAYER_DISPLAY_DATA : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwPlayerID;
    DWORD	dwCheckSum;
};

// Npc��ʾ����
struct C2G_APPLY_NPC_DISPLAY_DATA : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwNpcID;
    DWORD	dwCheckSum;
};

struct C2G_STOP_CURRENT_ACTION : UPWARDS_PROTOCOL_HEADER
{

};

// ------------------ skill about protocol ----------------->

//��ɫ�����ϴ�ָ��
struct C2G_CHARACTER_SKILL : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwSkillID;			//����ID
    BYTE	bySkillLevel;		//���ܵȼ�
    BYTE	byTargetType;		
    int		nParam1;	        //��TARGET_TYPE����������ʱ,����ֵΪ(nParam1, nParam2, nParam3)
    int		nParam2;            //������ΪxxIDʱ,nParam1ΪIDֵ,nParam2û����
    int     nParam3;
};

struct C2G_MOUNT_KUNGFU :  UPWARDS_PROTOCOL_HEADER
{
    DWORD dwID;
    DWORD dwLevel;
};

struct C2G_CANCEL_BUFF : UPWARDS_PROTOCOL_HEADER
{
    char cStyle;
    int  nIndex;
};

struct C2G_SET_SHEATH_STATE :  UPWARDS_PROTOCOL_HEADER
{
    bool bSheathFlag;
};

// <------------------ skill about protocol -----------------


// ----------------- party about protocol ----------------->

// xxx����xxx���
struct C2G_INVITE_JOIN_PARTY_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    char szInvitedPlayer[_NAME_LEN]; // �����������
};

// xxx�������xxx�Ķ���
struct C2G_APPLY_JOIN_PARTY_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    char  szApplyDst[_NAME_LEN]; // ������������ҵĶ���
};

// �ͻ����յ����������Ӧ��
struct C2G_INVITE_JOIN_PARTY_RESPOND : UPWARDS_PROTOCOL_HEADER
{
    char  szInviteSrc[_NAME_LEN]; // ���������������    
    GUID  guid;
    BOOL  bAgreeFlag;
};

// �ͻ���(�ӳ�)�յ����������Ӧ��
struct C2G_APPLY_JOIN_PARTY_RESPOND : UPWARDS_PROTOCOL_HEADER
{
    char  szApplySrc[_NAME_LEN]; // ���������������
    GUID  guid;
    BOOL  bAgreeFlag;
};

struct C2G_PARTY_DEL_MEMBER_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwMemberID;
};

struct C2G_PARTY_CHANGE_LEADER_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwNewLeaderID;
};

struct C2G_PARTY_SET_LOOT_MODE_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    int nLootMode;
};

struct C2G_PARTY_SET_ROLL_QUALITY_REQUEST : UPWARDS_PROTOCOL_HEADER
{
	int nRollQuality;
};

struct  C2G_PARTY_MEMBER_NOTIFY_SIGNPOST_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    int   nX;
    int   nY;
};

// <----------------- party about protocol -----------------

// ��������
struct C2G_ACCEPT_QUEST : UPWARDS_PROTOCOL_HEADER
{
    BYTE	byTargetType;
    DWORD	dwTargetID;
    DWORD	dwQuestID;
};

//�������
struct C2G_FINISH_QUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwQuestID;
	DWORD	dwTargetType;
	DWORD	dwTargetID;
    int		nPresentChoice1;
    int		nPresentChoice2;
};

//ȡ������
struct C2G_CANCEL_QUEST : UPWARDS_PROTOCOL_HEADER
{
    int		nQuestIndex;
};

// ��������
struct C2G_SHARE_QUEST : UPWARDS_PROTOCOL_HEADER
{
    int		nQuestIndex;
};

//GMָ��
struct C2G_GM_COMMAND : UNDEFINED_SIZE_UPWARDS_HEADER
{
    char	szGmCommand[0];
};

// ----------------- shop about protocol ----------------->


// ����ͬ���̵���Ʒ�б�ע�⣬������˷����̵������Ϣ��ʱ�򣬿ͻ��˾Ϳ��Դ��̵�����ˣ�����ͬ����Ʒ��
// һ��һ����ͬ���ģ���������һ����־˵����Ʒͬ����ϡ�

struct C2G_OPEN_SHOP_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwShopID;
};

struct C2G_QUERY_SHOP_ITEMS : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwShopID;    
    BYTE  byPageIndex;
    struct SHOP_SYNC_ITEM
    {
        BYTE  byPosIndex;
        DWORD dwItemID;
        int   nCount;
        int   nPrice;
    } ShopPageItems[MAX_SHOP_PAGE_ITEM_COUNT];
};

struct C2G_SHOP_BUY_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwShopID;
    DWORD dwItemID;
    int   nCount;
    int   nCost;
    BYTE  byPageIndex;
    BYTE  byPosIndex; 
};

struct C2G_SHOP_SELL_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwShopID;
    DWORD dwItemID;
    int   nCost;
    DWORD dwBoxIndex;
    DWORD dwPosIndex;
};

struct C2G_SHOP_REPAIR_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwShopID;
    DWORD dwItemID;     // ����ֵΪERROR_ID(0)ʱ,�����������ϺͰ��������е�װ��
    int   nCost;
    DWORD dwBoxIndex;
    DWORD dwPosIndex;
};

struct C2G_SOLD_LIST_BUY_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwShopID;
    DWORD dwItemID;
    DWORD dwX;
};

// <----------------- shop about protocol -----------------

// ------------- Trading about ------------------->
struct C2G_TRADING_INVITE_REQUEST : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwInviteDst;
};

struct C2G_TRADING_INVITE_RESPOND : UPWARDS_PROTOCOL_HEADER
{
    bool bAccept;
};

struct C2G_TRADING_ADD_ITEM : UPWARDS_PROTOCOL_HEADER
{
    BYTE byBoxIndex;
    BYTE byPosIndex;
    BYTE byGridIndex;
};

struct C2G_TRADING_DELETE_ITEM : UPWARDS_PROTOCOL_HEADER
{
    BYTE byGridIndex;
};

struct C2G_TRADING_SET_MONEY : UPWARDS_PROTOCOL_HEADER
{
    int nMoney;
};

struct C2G_TRADING_CONFIRM : UPWARDS_PROTOCOL_HEADER
{
    bool bConfirm;
    int  nKey;
};
// <-------------Trading about --------------------

struct C2G_SET_TARGET : UPWARDS_PROTOCOL_HEADER
{
    BYTE    byTargetType;
    DWORD   dwTargetID;   // Targetֻ����Player, NPC, Doodad.
};


// ���뽻��װ��
struct C2G_APPLY_EXCHANGE_ITEM : UPWARDS_PROTOCOL_HEADER
{
    BYTE	byBox1;
    BYTE	byX1;
    BYTE	byBox2;
    BYTE	byX2;
    WORD	wAmount;
};


// ѡ�񴰿�ѡ��
struct C2G_WINDOW_SELECT : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwIndex;
    BYTE	bySelect;
};


// �䷽ʹ��Э��
struct C2G_CAST_PROFESSION_SKILL : UPWARDS_PROTOCOL_HEADER
{
	DWORD	dwCraftID;
    DWORD	dwRecipeID;
    BYTE	byTargetType;
    int		nParam1;
    int		nParam2;
};


// ʰȡ��Ʒ����������
struct C2G_LOOT_OR_ROLL_ITEM : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwDoodadID;
    DWORD	dwItemID;
    BOOL	bCancel;
};


// ����ʰȡ�б�
struct C2G_APPLY_LOOT_LIST : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwDoodadID;
};


// ��
struct C2G_CHARACTER_OPEN : UPWARDS_PROTOCOL_HEADER
{
    DWORD	dwDoodadID;
};


struct C2G_DEATH_RESPOND : UPWARDS_PROTOCOL_HEADER
{
    int nRespondCode;
};

// ����ʹ�õ���
struct C2G_APPLY_USE_ITEM : UPWARDS_PROTOCOL_HEADER
{
    BYTE	byBox;
    BYTE	byX;
    BYTE	byTargetType;
    int	    nParam1;			    //���TargetType�������꣬�������ʾID
    int	    nParam2;
    int		nParam3;
};


// �ı�ս������
struct C2G_APPLY_FIGHT_FLAG : UPWARDS_PROTOCOL_HEADER
{
	BYTE	byForceID;
	BYTE	byOpen;
};


// ����ʰȡ
struct C2G_FINISH_LOOT : UPWARDS_PROTOCOL_HEADER
{
	DWORD dwDoodadID;
};


// ʰȡ��Ǯ
struct C2G_LOOT_MONEY : UPWARDS_PROTOCOL_HEADER
{
	DWORD dwDoodadID;
};

// �ı�PK״̬
struct C2G_CHANGE_PK_MODE : UPWARDS_PROTOCOL_HEADER
{
	BYTE byPKMode;
};

// �ӳ�������Ʒ
struct C2G_LEADER_DISTRIBUTE_ITEM : UPWARDS_PROTOCOL_HEADER
{
	DWORD dwDoodadID;
	DWORD dwItemID;
	DWORD dwDstPlayerID;
};

// ������Ϣ
struct C2G_PLAYER_TALK_TO_ONE : UNDEFINED_SIZE_UPWARDS_HEADER
{
    char szTalkTo[_NAME_LEN];   //�Է�������
    BYTE byTalkData[0];         //�Ի���������
};

// Ⱥ����Ϣ
struct C2G_PLAYER_TALK_TO_MANY : UNDEFINED_SIZE_UPWARDS_HEADER
{
    BYTE byTalkRange;           //Ⱥ�ķ�Χ
    BYTE byTalkData[0];         //�Ի���������
};

struct C2G_PLAYER_ADD_RELATION : UPWARDS_PROTOCOL_HEADER
{
    char szAlliedPlayerName[_NAME_LEN];
};

struct C2G_PLAYER_DEL_RELATION : UPWARDS_PROTOCOL_HEADER
{
    DWORD dwAlliedPlayerID;
};

//AutoCode:Client 2 GS Struct End
#pragma pack()

#endif	//_GS_CLIENT_PROTOCOL_H_
