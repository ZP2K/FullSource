/************************************************************************/
/* ��תЭ�鶨��								                            */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* Comment	 :															*/
/*		��תЭ�鴦�����												*/
/*		1. �����ӽڵ�													*/
/*			������յ���IP��ַ�Ǳ�����IP�Ͷ˿ڣ�����֮				*/
/*			���򣬸���Ѱַ���ݲ��������Ļ����У��Ƿ��з��ϵģ�������	*/
/*			�����򸸽ڵ㷢��											*/
/*		2. ���Ը��ڵ�													*/
/*			������յ���IP��ַ�Ǳ�����IP�Ͷ˿ڣ�����֮				*/
/*			���򣬸���Ѱַ���ݲ��������Ļ����У��Ƿ��з��ϵģ�������	*/
/*			���򣬷���Loseway�������ݰ�									*/
/* History	 :															*/
/*		2004.12.13	Create												*/
/************************************************************************/
#ifndef _RELAY_PACKAGE_PROTOCOL_H_
#define _RELAY_PACKAGE_PROTOCOL_H_

enum RELAY_PACKAGE_PROTOCOL
{
	relay_package_module_begin = 0,

	rp_seek_ipport,
	rp_seek_map,
	rp_seek_player,
	rp_seek_account,
	rp_seek_gm,

	rp_loseway,
	
	relay_package_module_end
};

#pragma	pack(1)
struct RP_SEEK_IPPORT : INTERNAL_PROTOCOL_HEADER
{
	IP_ADDR_PORT	DestAddr;
	IP_ADDR_PORT	SrcAddr;
	size_t			nDataSize;
	char			Data[1];
};

struct RP_SEEK_MAP : INTERNAL_PROTOCOL_HEADER
{
	int				nMapID;
	int				nMapCopyIndex;
	IP_ADDR_PORT	DestAddr;
	IP_ADDR_PORT	SrcAddr;
	size_t			nDataSize;
	char			Data[1];
};

struct RP_SEEK_PLAYER : INTERNAL_PROTOCOL_HEADER
{
	__int64			nPlayerIndex;
	IP_ADDR_PORT	DestAddr;
	IP_ADDR_PORT	SrcAddr;
	size_t			nDataSize;
	char			Data[1];
};

struct RP_SEEK_ACCOUNT : INTERNAL_PROTOCOL_HEADER
{
	char			szAccountName[32];
	IP_ADDR_PORT	DestAddr;
	IP_ADDR_PORT	SrcAddr;
	size_t			nDataSize;
	char			Data[1];
};

struct RP_SEEK_GM : INTERNAL_PROTOCOL_HEADER
{
	IP_ADDR_PORT	DestAddr;
	IP_ADDR_PORT	SrcAddr;
	size_t			nDataSize;
	char			Data[1];
};

struct RP_LOSEWAY : INTERNAL_PROTOCOL_HEADER
{
	IP_ADDR_PORT	DestAddr;
	IP_ADDR_PORT	SrcAddr;
	size_t			nDataSize;
	char			Data[1];
};

#pragma pack()

#endif	//_RELAY_PACKAGE_PROTOCOL_H_