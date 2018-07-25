/************************************************************************/
/* ����Э������ṹ����						                            */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.11.30	Create												*/
/************************************************************************/
#ifndef _SO3_PROTOCOL_BASIC_
#define _SO3_PROTOCOL_BASIC_

#include "ProtocolBasic.h"
#include "SO3GlobalDef.h"

#define UNUSED_PROTOCOL_ID			-1
#define UNDEFINED_PROTOCOL_SIZE		-1

#pragma	pack(1)

/************************************************************************/
/* ����Э���г��õĹ��ýṹ												*/

#define QUEST_PARAM_COUNT		4		//�����������

// ���߽ṹ
enum ITEM_SYNC_STRUCT_TYPE
{
	isst_Common = 0,
	isst_CustomEquip,
	isst_Equip,
	isst_Train,
	isst_Total,
};

// ս����Ϣ����

enum BATTLE_FIELD_NOTIFY_TYPE
{
    bfntInvalid = 0,
    bfntQueueInfo,
    bfntJoinBattleField,
    bfntLeaveBattleField,
    bfntInBlackList,
    bfntLeaveBlackList,
    bfntTotle
};

/************************************************************************/
/* ��������Э��ͷ����						                            */

struct EXTERNAL_PROTOCOL_HEADER
{
	BYTE	byProtocolID;			//Э���
};

struct UPWARDS_PROTOCOL_HEADER : EXTERNAL_PROTOCOL_HEADER
{
	int		nFrame;				//��ǰ֡��	
};

struct DOWNWARDS_PROTOCOL_HEADER : EXTERNAL_PROTOCOL_HEADER
{

};

struct UNDEFINED_SIZE_UPWARDS_HEADER : UPWARDS_PROTOCOL_HEADER
{
};

struct UNDEFINED_SIZE_DOWNWARDS_HEADER : DOWNWARDS_PROTOCOL_HEADER
{
	WORD	wSize;
};

#pragma pack()

#define REGISTER_EXTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
	m_nProtocolSize[ProtocolID] = ProtocolSize;}						

/************************************************************************/
/* ��������Э��ͷ����						                            */
//
//1.Э���������������
//		["���Ͷ�������д"2"���ն�������д"_]["����ģ��������д"_]"��������"
//		��ĸȫСд
//		����������Ӣ�ĵ���֮����_�ָ�
//		e.g.	b2r_player_login			��Bishop֪ͨRelay��ҵ�½��Э�飩
//				g2r_dml_apply_mapcopy		��GS��Relay�����ͼ������Э�飬���ڶ�̬��ͼ����ģ�飩
//2.Э��ṹ���Ƶ���������
//		��Э������Ʊ��ȫ��д����
//		e.g.	B2R_PLAYER_LOGIN
//3.���Э�鴦��������������
//		On"��������"
//		����������Ӣ�ĵ��ʵ�����ĸ��д
//		e.g		OnPlayerLogin


#pragma	pack(1)

typedef struct _INTERNAL_PROTOCOL_HEADER
{
	WORD	wProtocolID;			//Э���
    BYTE	bReserved;
}INTERNAL_PROTOCOL_HEADER;

typedef struct _IDENTITY_HEADER : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPacketIdentity;		//���ݰ���Ψһ��ǣ�����֤����·��ʹ��
}IDENTITY_HEADER;

#pragma pack()

#define REGISTER_INTERNAL_FUNC(ProtocolID, FuncName, ProtocolSize)	\
{m_ProcessProtocolFuns[ProtocolID] = FuncName;					\
    m_uProtocolSize[ProtocolID] = ProtocolSize;}

#endif	//_SO3_PROTOCOL_BASIC_
