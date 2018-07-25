/************************************************************************/
/* ������ɫ���ݽṹ����			                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.10.21	Create												*/
/* Comment	 :															*/
/*		���ļ�ֻ�����ɫ���ݵ�����ṹ��Base�еĻ������ݽṹ			*/
/*		�������ݿ��ڲ��Ľṹ�ڸ��ԵĶ�д�����ж��弴��					*/
/************************************************************************/
/* �������ݽṹ����:													*/
/*		KRoleDBData														*/
/*			KRoleDataHeader				�����������ݵ�ͷ�ṹ			*/
/*			WORD wBlockCount			���ݿ����						*/
/*			KRoleDataBlock BlockList[0]	���ݿ�������б�				*/
/*				byBlockType				���ݿ�����						*/
/*				wDataLen;				���ݿ鳤��						*/
/*				byReserved;				����							*/
/*			�ֲ����������ݿ�											*/
/************************************************************************/
#ifndef _KROLE_DB_DATA_DEF_H_
#define _KROLE_DB_DATA_DEF_H_

#include "SO3ProtocolBasic.h"

#define ROLE_DATA_VERSION			1
#define MAX_PLAYER_DB_DATA_SIZE		60000		//��ɫ�����������

#pragma	pack(1)

struct KRoleDataHeader
{
	//(һ��)��ɫ�浵���ݵ��ܴ�С��ֵ��СС��PLAYER_SAVE_DATA_SIZE_LIMIT
	//��TRoleData�ṹ��ʼ���Ϳ�ʼ������
	DWORD		dwDataLen;
	//(��)��ɫ�浵���ݵ�CRCУ����ֵ�������������ֵ�Ļ�������ΧΪ���Ӹ�dwCRC��ֵ֮��TRoleData::wVersion��ʼ
	//����ΪTRoleData->dwDataLen - sizeof(TRoleData->dwDataLen) - sizeof(TRoleData->dwCRC)
	DWORD		dwCRC;
	//�浵���ݰ汾
	WORD		wVersion;
	//----����dwDataLen��dwCRC��wVersionλ�ù̶���
	//��Ҫ��������Ҫ�ı��������ݹ����Ĺ����Լ�ROLE_SAVE_DATA_CRC_BEGIN_OFFSET���塣----
};

// ���Ҫ�������ݿ飬����ϣ��������ǰ���ݵĻ��������ں���ӣ������м���롣
enum ROLEDATA_BLOCK_TYPE
{
	rbtInvalid = 0,

	rbtBaseData,
	rbtSkillList,
	rbtItemList,
	rbtQuestList,
	rbtProfessionList,
	rbtRecipeList,
    rbtAttribData,
    rbtBuffList,
	rbtRepute,
    rbtDeathInfo,

	rbtTotal
};

struct KRoleDataBlock
{
	BYTE	byBlockType;
	WORD	wDataLen;
	BYTE	byReserved;
};

struct KRoleDBData : KRoleDataHeader
{
	WORD				wBlockCount;		//���ݿ�����
	KRoleDataBlock		BlockList[0];		//��ɫ���ݿ��б���һ��ض���BaseData
};

//������ɫ���ݣ�Bishop��GameCenter���õ�������
struct KBaseData
{
	char	szAccount[_NAME_LEN];
	char	szRoleName[_NAME_LEN];
	DWORD	dwPlayerID;
	char    cRoleType;  // see ROLE_TYPE;
	DWORD	dwMapID;
	DWORD	dwMapCopyIndex;
	int		nX;
	int		nY;
	int		nZ;
    BYTE    byFaceDirection;
	int		nReturnPosIndex;
	BYTE	byLevel;
    bool    bAlive;
    int     nExperience;
    WORD    wRepresentID[REPRESENT_COUNT];
	int		nDoubleExp;
	time_t	lLastSaveTime;
};

//��ɫ��������
struct KRoleAttribData
{
	int		nCurrentLife;				
	int		nCurrentMana;				
    int     nCurrentStamina;
    int     nCurrentRage;
};

struct KROLE_GENERATOR_PARAM
{
	char	szAccountName[_NAME_LEN];
	char	szRoleName[_NAME_LEN];
	char    cRoleType;
    DWORD   dwMapID;
    DWORD   dwMapCopyIndex;
    WORD    wRepresentID[REPRESENT_COUNT];
};

struct KROLE_REVIVE_INFO
{
    time_t  nLastDieTime;
    time_t  nLastSituReviveTime;
    WORD    wSituReviveCounter;
    DWORD   dwKillerID;
    int     nRevivePosIndex;
};

struct KItemData
{

};

struct KSkillData
{

};

#pragma pack()

#endif	//_KROLE_DB_DATA_DEF_H_
