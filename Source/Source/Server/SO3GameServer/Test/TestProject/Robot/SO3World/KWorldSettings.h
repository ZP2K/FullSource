/************************************************************************/
/* ��Ϸ���������ļ��б�					                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.04.25	Create												*/
/************************************************************************/
#ifndef _KSETTINGS_H_
#define _KSETTINGS_H_

#include "KMapListFile.h"
//#include "KDoodadTemplateList.h"
#include "KReturnPosList.h"
//#include "KDoodadClassList.h"
#include "KLevelUpList.h"
#include "KCoolDownList.h"
//#include "KWeaponTypeList.h"
#include "KRelationList.h"
#include "KFightFlagList.h"
//#include "KDoodadReliveList.h"
#include "KRevivePosList.h"
//#include "KCharacterActionList.h"
#include "KMonster.h"

#define MAX_DROP_LEVEL			16
#define MAX_DROP_CLASS			8
#define MAX_SPECIAL_DROP_LIST	2048

class KWorldSettings
{
public:
	KWorldSettings(void);
	~KWorldSettings(void);

	BOOL Init(void);
	BOOL UnInit(void);

	// ��Ϸ�������,Ŀǰȫ�����Ƕ���Ԥ����ĸ���
	struct KWORLD_PARAM
	{
		int nSceneCount;
		int nRegionCount;
		int nCellCount;
		int nPlayerCount;
		int nNpcCount;
		int nDoodadCount;
		int nItemCount;
		int	nLootListCount;
		int nAttributeCount;
		int nThreatNodeCount;
        int nBulletCount;
        int nTradingBoxCount;
		int nNpcTeamCount;
	} m_WorldParam;

    struct ROBOT_PARAM
    {
        char szBishopIP[_NAME_LEN];
        int  nBishopPort;

        char szAccountNamePrefix[_NAME_LEN];
        int  nStartIndex;
        char szPassword[_NAME_LEN];
        
        ARRANGEMENT  nArrangement;
        int nRange;
        int nRunRate;

        int  nStartInterval;

        int  nPreLoadMap;
        int  nReconnect;
    }m_RobotParam;

	//���ص��б�
	KReturnPosList		m_ReturnPosList;

	//��ͼ��ز���
	KMapListFile		m_MapListFile;

	//Doodadģ���
	//KDoodadTemplateList m_DoodadTemplateList;

	//Doodad�����
	//KDoodadClassList	m_DoodadClassList;

	//����������ݱ�
	KLevelUpList		m_LevelUpList;

    KCoolDownList       m_CoolDownList;

    //KWeaponTypeList     m_WeaponTypeList;

	//��Ӫ��ϵ��
	KRelationList		m_RelationCampList;

	//������ϵ��
	KRelationList		m_RelationForceList;

	//Ĭ������ս������������
	KFightFlagList		m_ForceDefaultList;

	//KCharacterActionList m_CharacterActionList;
private:
	BOOL LoadWorldParam();
    BOOL LoadRobotParam();
};

extern KWorldSettings	g_WorldSettings;

#endif	//_KSETTINGS_H_