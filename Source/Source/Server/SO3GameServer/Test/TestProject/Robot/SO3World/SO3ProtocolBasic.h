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

#define INVALID_PROTOCOL_VERSION	-1
#define SWORD3_PROTOCOL_VERSION		 1

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

// �������͵�װ��ͬ�����������ṹ����
struct COMMON_ITEM_SYNC
{
	DWORD	dwItemId;
	BYTE	byTabFile;
	BYTE	byVersion;
	WORD	wTabIndex;
	WORD	wDurability;
};

struct CUSTOM_EQUIP_SYNC : COMMON_ITEM_SYNC
{
	DWORD	dwRandSeed;
	WORD	wExtendMagic;
	WORD	wTemporaryMagic;
};

struct RAND_EQUIP_SYNC : COMMON_ITEM_SYNC
{
	BYTE	byQuality;
	WORD	wMagic[MAX_ITEM_MAGIC_COUNT];
	WORD	wExtendMagic;
	WORD	wTemporaryMagic;
	DWORD	dwRandSeed;
};

struct TRAIN_EQUIP_SYNC: RAND_EQUIP_SYNC
{
	char	szMadeBy[_NAME_LEN];
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
	WORD	wSize;
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
//1.Э����������������
//	�����ӻ�������
//		"Server������"_"Client������"_connection
//		������ĸȫСд
//		e.g		bishop_relay_connection		��Bishop��Relay�����ӣ�����Bishop��Server�ˣ�Relay��Client�ˣ�
//	��ģ�黮������
//		["Server��������д""Client��������д"]_"ģ������"_module
//		������ĸȫ��Сд
//		e.g.	relay_package_module
//				rg_dynamic_map_module
//2.Э���ö��������������
//		Э���������ĺ�׺Connection���滻��Protocol
//		��ĸȫ��д
//		e.g.	BISHOP_RELAY_PROTOCOL		����Ӧ��Э����bishop_relay_connection��
//3.Э���������������
//		["���Ͷ�������д"2"���ն�������д"_]["����ģ��������д"_]"��������"
//		��ĸȫСд
//		����������Ӣ�ĵ���֮����_�ָ�
//		e.g.	b2r_player_login			��Bishop֪ͨRelay��ҵ�½��Э�飩
//				g2r_dml_apply_mapcopy		��GS��Relay�����ͼ������Э�飬���ڶ�̬��ͼ����ģ�飩
//4.Э��ṹ���Ƶ���������
//		��Э������Ʊ��ȫ��д����
//		e.g.	B2R_PLAYER_LOGIN
//5.���Э�鴦��������������
//		On"��������"
//		����������Ӣ�ĵ��ʵ�����ĸ��д
//		e.g		OnPlayerLogin

enum INTERNAL_PROTOCOL_FAMILY
{
	invalid_protocol_family = 0,
	bishop_relay_connection,
	relay_gs_connection,
	relayserver_relay_connection,
	relay_package_module,


};

//Э���������
#define MAX_PROTOCOL_ID_NUM			256
#define MAX_PROTOCOL_FAMILY_NUM		8

#pragma	pack(1)

typedef struct _INTERNAL_PROTOCOL_HEADER
{
	BYTE	byProtocolFamily;		//Э����
	BYTE	bReserved;
	WORD	wProtocolID;			//Э���
}INTERNAL_PROTOCOL_HEADER;

typedef struct _IDENTITY_HEADER : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPacketIdentity;		//���ݰ���Ψһ��ǣ�����֤����·��ʹ��
}IDENTITY_HEADER;

#pragma pack()

#define REGISTER_FUNC(_PROTOCOL_FAMILY_, _PROTOCOL_ID_, _FUNC_NAME_)	\
	m_ProcessProtocolFuns[(_PROTOCOL_FAMILY_ << 8) + _PROTOCOL_ID_] = \
	(PROCESS_PROTOCOL_FUNC)_FUNC_NAME_

#endif	//_SO3_PROTOCOL_BASIC_