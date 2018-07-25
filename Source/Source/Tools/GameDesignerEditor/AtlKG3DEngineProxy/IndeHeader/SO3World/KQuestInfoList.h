/************************************************************************/
/* �������ñ�															*/
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.11.01	Create												*/
/************************************************************************/
#ifndef _KQUEST_INFO_LIST_H_
#define _KQUEST_INFO_LIST_H_

#include <map>
#include <vector>
#include "Global.h"
#include "SO3ProtocolBasic.h"
#include "Luna.h"

#define QUEST_NAME_LEN					32
#define QUEST_OBJECTIVE_LEN				1026
#define QUEST_DESCRIPTION_LEN			1024
#define QUEST_UNFINISHED_DIALOGUE_LEN	1024
#define QUEST_FINISHED_DIALOGUE_LEN		1024
#define QUEST_VALUE_STR_LEN				256
#define QUEST_GOSSIP_LEN		        1024

#define MAX_QUEST_COUNT			        8192
#define QUEST_END_ITEM_COUNT            8

class ITabFile;

struct KQuestInfo
{
	DWORD			dwQuestID;
	char			szName[QUEST_NAME_LEN];
	char			szQuestClass[QUEST_NAME_LEN];				// �����������
	BOOL			bShare;										// �Ƿ�ɹ���
	BOOL			bRepeat;									// �Ƿ���ظ�
	BOOL			bAssist;									// �Ƿ��Э��
	BOOL			bAccept;									// �Ƿ���Ҫ���˲��ܽ�
	BOOL			bEscortQuest;                               // �Ƿ�������
	int				nLevel;										// ����ȼ�

	char			szQuestDiff[QUEST_NAME_LEN];				// �����Ѷ�����		

	//�����������Ϣ
	DWORD			dwStartMapID;								// ������Npc���ڵ�ͼ��ID, ��ʱ����
	DWORD			dwStartNpcTemplateID;						// ������Npc��ģ��ID
	DWORD			dwStartDoodadTemplateID;					// ������Doodad��ģ��ID
	DWORD			dwStartItemType;							// ������Item����
	DWORD			dwStartItemIndex;							// ������Item����

#ifdef _CLIENT
	char			szDescription[QUEST_DESCRIPTION_LEN];		// ����������
	char			szObjective[QUEST_OBJECTIVE_LEN];			// ������Ŀ��
	int				nStartGossipType;							// ��������̸����
	char			szStartGossip[QUEST_PARAM_COUNT][QUEST_GOSSIP_LEN];
#endif
																// ��������̸����

	int				nMinLevel;									// ��������С�ȼ�����
	int				nMaxLevel;									// ���������ȼ�����
	int				nRequireGender;								// ��������Ա�����
	int				nRequireRoleType;							// ��������������������
	DWORD			dwCoolDownID;								// CoolDown��ʱ��ID
	BOOL			bPrequestLogic;								// true:and;false:or
	DWORD			dwPrequestID[QUEST_PARAM_COUNT];			// ǰ������ID
	DWORD			dwSubsequenceID;							// ֱ�Ӻ������ID
	int				nStartTime;									// ��ʼʱ��
	int				nEndTime;									// ����ʱ��

	DWORD			dwRequireSkillID;							// ������
	DWORD			dwMinSkillLevel;							// ��������С�ȼ�

	DWORD			dwRequireLifeSkillID;						// ������
	DWORD			dwMinLifeSkillLevel;						// ��������С�ȼ�
 
	DWORD			dwStartRequireItemType[QUEST_PARAM_COUNT];	// ������ʱ������ߵ�����
	DWORD			dwStartRequireItemIndex[QUEST_PARAM_COUNT];	// ������ʱ������ߵ�����
	DWORD			dwStartRequireItemAmount[QUEST_PARAM_COUNT];// ������ʱ������ߵ�����

	DWORD			dwOfferItemType[QUEST_PARAM_COUNT];			// ������ʱ�ṩ���ߵ�����
	DWORD			dwOfferItemIndex[QUEST_PARAM_COUNT];		// ������ʱ�ṩ���ߵ�����
	DWORD			dwOfferItemAmount[QUEST_PARAM_COUNT];		// ������ʱ�ṩ���ߵ�����

    DWORD           dwRequireSchoolMask;                        // ������ʱ����������
    BOOL            bRequireReputeAll;                          // ������ʱ�������Ƶ��߼���ϵ(true: AND, false: OR)
    DWORD           dwRequireForceID[QUEST_PARAM_COUNT];
    int             nReputeLevelMin[QUEST_PARAM_COUNT];
    int             nReputeLevelMax[QUEST_PARAM_COUNT];

	//�����������Ϣ
	DWORD			dwEndMapID;									//������Npc���ڵĵ�ͼID, ��ʱ����
	DWORD			dwEndNpcTemplateID;							//������Npc��ģ��ID
	DWORD			dwEndDoodadTemplateID;						//������Doodad��ģ��ID
	DWORD			dwEndItemType;								//������Item����
	DWORD			dwEndItemIndex;								//������Item����

#ifdef _CLIENT
	char			szDunningDialogue[QUEST_UNFINISHED_DIALOGUE_LEN];
	char			szUnfinishedDialogue[QUEST_UNFINISHED_DIALOGUE_LEN];
    char			szFinishedDialogue[QUEST_FINISHED_DIALOGUE_LEN];
	int				nEndGossipType;								//��������̸����
	char			szEndGossip[QUEST_PARAM_COUNT][QUEST_GOSSIP_LEN];
                                                                //��������̸����
#endif

	int				nFinishTime;								//���ʱ������

	DWORD			dwEndRequireItemType[QUEST_END_ITEM_COUNT];	//������ʱ������ߵ�����
	DWORD			dwEndRequireItemIndex[QUEST_END_ITEM_COUNT];	//������ʱ������ߵ�����
	DWORD			dwEndRequireItemAmount[QUEST_END_ITEM_COUNT];	//������ʱ������ߵ�����
	BOOL			bIsDeleteEndRequireItem[QUEST_END_ITEM_COUNT]; //���������ɾ�������ʱ���Ƿ�ɾ���������
	DWORD			dwDropItemNpcTemplateID[QUEST_END_ITEM_COUNT];		//���佻������ߵ�Npcģ��ID
	DWORD			dwDropItemDoodadTemplateID[QUEST_END_ITEM_COUNT];	//���佻������ߵ�doodadģ��ID
	DWORD			dwDropItemRate[QUEST_END_ITEM_COUNT];			//����������ߵļ���
	BOOL			bDropForEach[QUEST_END_ITEM_COUNT];			//�Ƿ�ÿ�˶����һ��

	int				nCostMoney;									//����������Ľ�Ǯ

	DWORD			dwKillNpcTemplateID[QUEST_PARAM_COUNT];		//������ʱ����ɱ����Npcģ��ID
	DWORD			dwKillNpcAmount[QUEST_PARAM_COUNT];			//������ʱ����ɱ����Npc����

	int				nQuestValue[QUEST_PARAM_COUNT * 2];			//�������
#ifdef _CLIENT
	char			szQuestValueStr[QUEST_PARAM_COUNT * 2][QUEST_VALUE_STR_LEN]; //��������ַ���
#endif

	DWORD			dwRelationDoodadID[QUEST_PARAM_COUNT * 2];  // �����������DoodadID

	BOOL			bHidePresent;								// �Ƿ����ؽ���
	BOOL			bPresentAll[2];								// ��һ��(1-4)�����Ƿ�ȫ������ֻ��һ��
	DWORD			dwPresentItemType[QUEST_PARAM_COUNT * 2];	// ������ʱ�������ߵ�����
	DWORD			dwPresentItemIndex[QUEST_PARAM_COUNT * 2];	// ������ʱ�������ߵ�����
	DWORD			dwPresentItemAmount[QUEST_PARAM_COUNT * 2];	// ������ʱ�������ߵ�����

	int				nPresentExp;								// ������ʱ�����ľ���
	int				nPresentMoney;								// ������ʱ�����Ľ�Ǯ����
	int				nPresentSkill;								// �����ļ���ID

    int             nPresentAssistThew;                         // Э������������
    int             nPresentAssistStamina;                      // Э������������
    int             nPresentAssistFriendship;                   // Э���������øж�

	DWORD			dwAffectForceID[QUEST_PARAM_COUNT];			// ������Ӱ�������
	int				nAffectForceValue[QUEST_PARAM_COUNT];		// ������Ӱ��������Ѻö�

	int				nRepeatCutPercent;							// �ظ�������ʱ�����������ٷֱ�

	char			szScriptName[MAX_PATH];						// �ű��ļ�·��

    int             nRequireCampMask;                           // ��Ӫ����; ��0,1,2λ�ֱ��Ӧ���������塢а��
    int             nPresentPrestige;                           // ����������
    int             nPresentContribution;                       // ����������ֵ
    int             nPresentTrain;                              // ��������Ϊ
#ifdef _SERVER
    int             nAchievementID;                             // ����ɾ�
#endif

//Lua�ű��ӿ�
	DECLARE_LUA_CLASS(KQuestInfo);

	DECLARE_LUA_STRUCT_STRING(Name, QUEST_NAME_LEN, szName);
	DECLARE_LUA_STRUCT_STRING(QuestClass, QUEST_NAME_LEN, szQuestClass);
	DECLARE_LUA_STRUCT_BOOL(Share, bShare);					    // �Ƿ�ɹ���
	DECLARE_LUA_STRUCT_BOOL(Repeat, bRepeat);					// �Ƿ���ظ�
	DECLARE_LUA_STRUCT_BOOL(Assist, bAssist);					// �Ƿ��Э��
	DECLARE_LUA_STRUCT_BOOL(Accept, bAccept);					// �Ƿ��ȽӲ��ܽ�
	DECLARE_LUA_STRUCT_INTEGER(Level, nLevel);					// ����ȼ�

	DECLARE_LUA_STRUCT_STRING(QuestDiff, QUEST_NAME_LEN, szQuestDiff);

	DECLARE_LUA_STRUCT_DWORD(StartMapID, dwStartMapID);							//������Npc���ڵ�ͼ��ID
	DECLARE_LUA_STRUCT_DWORD(StartNpcTemplateID, dwStartNpcTemplateID);			//������Npc��ID
	DECLARE_LUA_STRUCT_DWORD(StartItemType, dwStartItemType);					//������Item����
	DECLARE_LUA_STRUCT_DWORD(StartItemIndex, dwStartItemIndex);					//������Item����

#ifdef _CLIENT
	DECLARE_LUA_STRUCT_STRING(Objective, QUEST_OBJECTIVE_LEN, szObjective);		//������ʱ��Npc�Ի�����
    DECLARE_LUA_STRUCT_STRING(Description, QUEST_DESCRIPTION_LEN, szDescription);
	DECLARE_LUA_STRUCT_INTEGER(StartGossipType, nStartGossipType);						//��������̸����
	DECLARE_LUA_STRUCT_STRING(StartGossip1, QUEST_GOSSIP_LEN, szStartGossip[0]);		//��������̸����
	DECLARE_LUA_STRUCT_STRING(StartGossip2, QUEST_GOSSIP_LEN, szStartGossip[1]);		//��������̸����
	DECLARE_LUA_STRUCT_STRING(StartGossip3, QUEST_GOSSIP_LEN, szStartGossip[2]);		//��������̸����
	DECLARE_LUA_STRUCT_STRING(StartGossip4, QUEST_GOSSIP_LEN, szStartGossip[3]);		//��������̸����
#endif

    DECLARE_LUA_STRUCT_DWORD(RequireSchoolMask, dwRequireSchoolMask);

    DECLARE_LUA_STRUCT_BOOL(RequireReputeAll, bRequireReputeAll);
    DECLARE_LUA_STRUCT_DWORD(RequireForceID1, dwRequireForceID[0]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMin1, nReputeLevelMin[0]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMax1, nReputeLevelMax[0]);
    DECLARE_LUA_STRUCT_DWORD(RequireForceID2, dwRequireForceID[1]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMin2, nReputeLevelMin[1]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMax2, nReputeLevelMax[1]);
    DECLARE_LUA_STRUCT_DWORD(RequireForceID3, dwRequireForceID[2]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMin3, nReputeLevelMin[2]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMax3, nReputeLevelMax[2]);
    DECLARE_LUA_STRUCT_DWORD(RequireForceID4, dwRequireForceID[3]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMin4, nReputeLevelMin[3]);
    DECLARE_LUA_STRUCT_INTEGER(ReputeLevelMax4, nReputeLevelMax[3]);

    DECLARE_LUA_STRUCT_INTEGER(MinLevel, nMinLevel);			//��������С�ȼ�����
	DECLARE_LUA_STRUCT_INTEGER(MaxLevel, nMaxLevel);			//���������ȼ�����
	DECLARE_LUA_STRUCT_INTEGER(RequireGender, nRequireGender);	//��������Ա�����
	DECLARE_LUA_STRUCT_INTEGER(RequireRoleType, nRequireRoleType); //���������������
	DECLARE_LUA_STRUCT_DWORD(CoolDownID, dwCoolDownID);		//CoolDown��ʱ��ID
	DECLARE_LUA_STRUCT_BOOL(PrequestLogic, bPrequestLogic);
	DECLARE_LUA_STRUCT_DWORD(PrequestID1, dwPrequestID[0]);			//ǰ������ID
	DECLARE_LUA_STRUCT_DWORD(PrequestID2, dwPrequestID[1]);			//ǰ������ID
	DECLARE_LUA_STRUCT_DWORD(PrequestID3, dwPrequestID[2]);			//ǰ������ID
	DECLARE_LUA_STRUCT_DWORD(PrequestID4, dwPrequestID[3]);			//ǰ������ID
	DECLARE_LUA_STRUCT_INTEGER(StartTime, nStartTime);			//��ʼʱ��
	DECLARE_LUA_STRUCT_INTEGER(EndTime, nEndTime);				//����ʱ��
	DECLARE_LUA_STRUCT_DWORD(SubsequenceID, dwSubsequenceID);	//ֱ�Ӻ������ID   

	DECLARE_LUA_STRUCT_DWORD(RequireSkillID, dwRequireSkillID);	//������
	DECLARE_LUA_STRUCT_DWORD(MinSkillLevel, dwMinSkillLevel);	//��������С�ȼ�

	DECLARE_LUA_STRUCT_DWORD(RequireLifeSkillID, dwRequireLifeSkillID);		//������
	DECLARE_LUA_STRUCT_DWORD(MinLifeSkillLevel, dwMinLifeSkillLevel);		//��������С�ȼ�

	DECLARE_LUA_STRUCT_DWORD(StartRequireItemType1, dwStartRequireItemType[0]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemIndex1, dwStartRequireItemIndex[0]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemAmount1, dwStartRequireItemAmount[0]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemType2, dwStartRequireItemType[1]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemIndex2, dwStartRequireItemIndex[1]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemAmount2, dwStartRequireItemAmount[1]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemType3, dwStartRequireItemType[2]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemIndex3, dwStartRequireItemIndex[2]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemAmount3, dwStartRequireItemAmount[2]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemType4, dwStartRequireItemType[3]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemIndex4, dwStartRequireItemIndex[3]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(StartRequireItemAmount4, dwStartRequireItemAmount[3]);	//������ʱ������ߵ�����

	DECLARE_LUA_STRUCT_DWORD(OfferItemType1, dwOfferItemType[0]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemIndex1, dwOfferItemIndex[0]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemAmount1, dwOfferItemAmount[0]);		//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemType2, dwOfferItemType[1]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemIndex2, dwOfferItemIndex[1]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemAmount2, dwOfferItemAmount[1]);		//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemType3, dwOfferItemType[2]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemIndex3, dwOfferItemIndex[2]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemAmount3, dwOfferItemAmount[2]);		//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemType4, dwOfferItemType[3]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemIndex4, dwOfferItemIndex[3]);			//��������ṩ���ߵ�����
	DECLARE_LUA_STRUCT_DWORD(OfferItemAmount4, dwOfferItemAmount[3]);		//��������ṩ���ߵ�����

	DECLARE_LUA_STRUCT_DWORD(EndMapID, dwEndMapID);							//������Npc���ڵĵ�ͼID
	DECLARE_LUA_STRUCT_DWORD(EndNpcTemplateID, dwEndNpcTemplateID);			//������Npc��ID
    DECLARE_LUA_STRUCT_DWORD(EndDoodadTemplateID, dwEndDoodadTemplateID);   //������Doodad��ID  
	DECLARE_LUA_STRUCT_DWORD(EndItemType, dwEndItemType);					//������Item����
	DECLARE_LUA_STRUCT_DWORD(EndItemIndex, dwEndItemIndex);					//������Item����

#ifdef _CLIENT
	DECLARE_LUA_STRUCT_STRING(DunningDialogue, QUEST_UNFINISHED_DIALOGUE_LEN, szDunningDialogue);
	DECLARE_LUA_STRUCT_STRING(UnfinishedDialogue, QUEST_UNFINISHED_DIALOGUE_LEN, szUnfinishedDialogue);
	DECLARE_LUA_STRUCT_STRING(FinishedDialogue, QUEST_FINISHED_DIALOGUE_LEN, szFinishedDialogue);
	DECLARE_LUA_STRUCT_INTEGER(EndGossipType, nEndGossipType);					//��������̸����
	DECLARE_LUA_STRUCT_STRING(EndGossip1, QUEST_GOSSIP_LEN, szEndGossip[0]);	//��������̸����
	DECLARE_LUA_STRUCT_STRING(EndGossip2, QUEST_GOSSIP_LEN, szEndGossip[1]);	//��������̸����
	DECLARE_LUA_STRUCT_STRING(EndGossip3, QUEST_GOSSIP_LEN, szEndGossip[2]);	//��������̸����
	DECLARE_LUA_STRUCT_STRING(EndGossip4, QUEST_GOSSIP_LEN, szEndGossip[3]);	//��������̸����
#endif

	DECLARE_LUA_STRUCT_INTEGER(FinishTime, nFinishTime);						//���ʱ������

	//---------------------------------------------------------------------------------->
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemType1, dwEndRequireItemType[0]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex1, dwEndRequireItemIndex[0]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount1, dwEndRequireItemAmount[0]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem1, bIsDeleteEndRequireItem[0]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID1, dwDropItemNpcTemplateID[0]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID1, dwDropItemDoodadTemplateID[0]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate1, dwDropItemRate[0]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach1, bDropForEach[0]);						//�Ƿ�ÿ�˵���һ��

	DECLARE_LUA_STRUCT_DWORD(EndRequireItemType2, dwEndRequireItemType[1]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex2, dwEndRequireItemIndex[1]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount2, dwEndRequireItemAmount[1]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem2, bIsDeleteEndRequireItem[1]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID2, dwDropItemNpcTemplateID[1]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID2, dwDropItemDoodadTemplateID[1]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate2, dwDropItemRate[1]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach2, bDropForEach[1]);						//�Ƿ�ÿ�˵���һ��

	DECLARE_LUA_STRUCT_DWORD(EndRequireItemType3, dwEndRequireItemType[2]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex3, dwEndRequireItemIndex[2]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount3, dwEndRequireItemAmount[2]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem3, bIsDeleteEndRequireItem[2]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID3, dwDropItemNpcTemplateID[2]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID3, dwDropItemDoodadTemplateID[2]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate3, dwDropItemRate[2]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach3, bDropForEach[2]);						//�Ƿ�ÿ�˵���һ��

	DECLARE_LUA_STRUCT_DWORD(EndRequireItemType4, dwEndRequireItemType[3]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex4, dwEndRequireItemIndex[3]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount4, dwEndRequireItemAmount[3]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem4, bIsDeleteEndRequireItem[3]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID4, dwDropItemNpcTemplateID[3]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID4, dwDropItemDoodadTemplateID[3]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate4, dwDropItemRate[3]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach4, bDropForEach[3]);						//�Ƿ�ÿ�˵���һ��

    DECLARE_LUA_STRUCT_DWORD(EndRequireItemType5, dwEndRequireItemType[4]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex5, dwEndRequireItemIndex[4]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount5, dwEndRequireItemAmount[4]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem5, bIsDeleteEndRequireItem[4]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID5, dwDropItemNpcTemplateID[4]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID5, dwDropItemDoodadTemplateID[4]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate5, dwDropItemRate[4]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach5, bDropForEach[4]);						//�Ƿ�ÿ�˵���һ��

    DECLARE_LUA_STRUCT_DWORD(EndRequireItemType6, dwEndRequireItemType[5]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex6, dwEndRequireItemIndex[5]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount6, dwEndRequireItemAmount[5]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem6, bIsDeleteEndRequireItem[5]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID6, dwDropItemNpcTemplateID[5]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID6, dwDropItemDoodadTemplateID[5]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate6, dwDropItemRate[5]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach6, bDropForEach[5]);						//�Ƿ�ÿ�˵���һ��

    DECLARE_LUA_STRUCT_DWORD(EndRequireItemType7, dwEndRequireItemType[6]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex7, dwEndRequireItemIndex[6]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount7, dwEndRequireItemAmount[6]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem7, bIsDeleteEndRequireItem[6]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID7, dwDropItemNpcTemplateID[6]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID7, dwDropItemDoodadTemplateID[6]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate7, dwDropItemRate[6]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach7, bDropForEach[6]);						//�Ƿ�ÿ�˵���һ��

    DECLARE_LUA_STRUCT_DWORD(EndRequireItemType8, dwEndRequireItemType[7]);			//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemIndex8, dwEndRequireItemIndex[7]);		//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(EndRequireItemAmount8, dwEndRequireItemAmount[7]);	//������ʱ������ߵ�����
	DECLARE_LUA_STRUCT_BOOL(IsDeleteEndRequireItem8, bIsDeleteEndRequireItem[7]);  //�Ƿ�ɾ������
	DECLARE_LUA_STRUCT_DWORD(DropItemNpcTemplateID8, dwDropItemNpcTemplateID[7]);		//���佻������ߵ�Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemDoodadTemplateID8, dwDropItemDoodadTemplateID[7]);	//���佻������ߵ�doodadģ��ID
	DECLARE_LUA_STRUCT_DWORD(DropItemRate8, dwDropItemRate[7]);					//����������ߵļ���
	DECLARE_LUA_STRUCT_BOOL(DropForEach8, bDropForEach[7]);						//�Ƿ�ÿ�˵���һ��
	//<------------------------------------------------------------------------------------

	DECLARE_LUA_STRUCT_INTEGER(CostMoney, nCostMoney);							//���ĵĽ�Ǯ

	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID1, dwKillNpcTemplateID[0]);		//������ʱ����ɱ����Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(KillNpcAmount1, dwKillNpcAmount[0]);				//������ʱ����ɱ����Npc����
	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID2, dwKillNpcTemplateID[1]);		//������ʱ����ɱ����Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(KillNpcAmount2, dwKillNpcAmount[1]);				//������ʱ����ɱ����Npc����
	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID3, dwKillNpcTemplateID[2]);		//������ʱ����ɱ����Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(KillNpcAmount3, dwKillNpcAmount[2]);				//������ʱ����ɱ����Npc����
	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID4, dwKillNpcTemplateID[3]);		//������ʱ����ɱ����Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(KillNpcAmount4, dwKillNpcAmount[3]);				//������ʱ����ɱ����Npc����

	DECLARE_LUA_STRUCT_INTEGER(QuestValue1, nQuestValue[0]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue2, nQuestValue[1]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue3, nQuestValue[2]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue4, nQuestValue[3]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue5, nQuestValue[4]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue6, nQuestValue[5]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue7, nQuestValue[6]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue8, nQuestValue[7]);						//�������

#ifdef _CLIENT
	DECLARE_LUA_STRUCT_STRING(QuestValueStr1,QUEST_VALUE_STR_LEN ,szQuestValueStr[0]);
	DECLARE_LUA_STRUCT_STRING(QuestValueStr2,QUEST_VALUE_STR_LEN ,szQuestValueStr[1]);
	DECLARE_LUA_STRUCT_STRING(QuestValueStr3,QUEST_VALUE_STR_LEN ,szQuestValueStr[2]);
	DECLARE_LUA_STRUCT_STRING(QuestValueStr4,QUEST_VALUE_STR_LEN ,szQuestValueStr[3]);
	DECLARE_LUA_STRUCT_STRING(QuestValueStr5,QUEST_VALUE_STR_LEN ,szQuestValueStr[4]);
	DECLARE_LUA_STRUCT_STRING(QuestValueStr6,QUEST_VALUE_STR_LEN ,szQuestValueStr[5]);
	DECLARE_LUA_STRUCT_STRING(QuestValueStr7,QUEST_VALUE_STR_LEN ,szQuestValueStr[6]);
	DECLARE_LUA_STRUCT_STRING(QuestValueStr8,QUEST_VALUE_STR_LEN ,szQuestValueStr[7]);
#endif

	DECLARE_LUA_STRUCT_INTEGER(HidePresent, bHidePresent);			//�Ƿ����ؽ���

	DECLARE_LUA_STRUCT_INTEGER(PresentAll1, bPresentAll[0]);		//��һ��(1-4)�����Ƿ�ȫ������ֻ��һ��
	DECLARE_LUA_STRUCT_INTEGER(PresentAll2, bPresentAll[1]);		//�ڶ���(5-8)�����Ƿ�ȫ������ֻ��һ��
	
	DECLARE_LUA_STRUCT_DWORD(PresentItemType1, dwPresentItemType[0]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex1, dwPresentItemIndex[0]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount1, dwPresentItemAmount[0]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemType2, dwPresentItemType[1]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex2, dwPresentItemIndex[1]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount2, dwPresentItemAmount[1]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemType3, dwPresentItemType[2]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex3, dwPresentItemIndex[2]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount3, dwPresentItemAmount[2]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemType4, dwPresentItemType[3]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex4, dwPresentItemIndex[3]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount4, dwPresentItemAmount[3]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemType5, dwPresentItemType[4]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex5, dwPresentItemIndex[4]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount5, dwPresentItemAmount[4]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemType6, dwPresentItemType[5]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex6, dwPresentItemIndex[5]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount6, dwPresentItemAmount[5]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemType7, dwPresentItemType[6]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex7, dwPresentItemIndex[6]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount7, dwPresentItemAmount[6]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemType8, dwPresentItemType[7]);		//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemIndex8, dwPresentItemIndex[7]);	//������ʱ�������ߵ�����
	DECLARE_LUA_STRUCT_DWORD(PresentItemAmount8, dwPresentItemAmount[7]);	//������ʱ�������ߵ�����

	DECLARE_LUA_STRUCT_INTEGER(PresentExp, nPresentExp);					//������ʱ�����ľ���
	DECLARE_LUA_STRUCT_INTEGER(PresentMoney, nPresentMoney);				//������ʱ�����Ľ�Ǯ����
	DECLARE_LUA_STRUCT_INTEGER(PresentSkill, nPresentSkill);				//������ʱ�ļ��ܽ���

    DECLARE_LUA_STRUCT_INTEGER(PresentAssistThew, nPresentAssistThew);
    DECLARE_LUA_STRUCT_INTEGER(PresentAssistStamina, nPresentAssistStamina);
    DECLARE_LUA_STRUCT_INTEGER(PresentAssistFriendship, nPresentAssistFriendship);

	DECLARE_LUA_STRUCT_DWORD(AffectForceID1, dwAffectForceID[0]);			//������Ӱ�������
	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue1, nAffectForceValue[0]);	//������Ӱ��������Ѻö�
	DECLARE_LUA_STRUCT_DWORD(AffectForceID2, dwAffectForceID[1]);			//������Ӱ�������
	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue2, nAffectForceValue[1]);	//������Ӱ��������Ѻö�
	DECLARE_LUA_STRUCT_DWORD(AffectForceID3, dwAffectForceID[2]);			//������Ӱ�������
	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue3, nAffectForceValue[2]);	//������Ӱ��������Ѻö�
	DECLARE_LUA_STRUCT_DWORD(AffectForceID4, dwAffectForceID[3]);			//������Ӱ�������
	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue4, nAffectForceValue[3]);	//������Ӱ��������Ѻö�

	DECLARE_LUA_STRUCT_INTEGER(RepeatCutPercent, nRepeatCutPercent);		//�ظ�������ʱ�����������ٷֱ�

    DECLARE_LUA_STRUCT_INTEGER(RequireCampMask, nRequireCampMask);

    DECLARE_LUA_STRUCT_INTEGER(PresentPrestige, nPresentPrestige);
    DECLARE_LUA_STRUCT_INTEGER(PresentContribution, nPresentContribution);
    DECLARE_LUA_STRUCT_INTEGER(PresentTrain, nPresentTrain);

	int LuaGetHortation(Lua_State* L);
};

class KQuestInfoList
{
public:
	BOOL Init();
	void UnInit();

	KQuestInfo* GetQuestInfo(DWORD dwQuestID);

	int GetNpcQuestString(DWORD dwMapID, DWORD dwNpcTemplateID, char* pszBuffer, size_t uBufferSize);
	BOOL GetNpcQuest(DWORD dwNpcTemplateID, std::vector<DWORD>* pvQuestID);
    BOOL IsNpcQuestExist(DWORD dwNpcTemplateID);

private:
	typedef std::map<DWORD, KQuestInfo> MAP_ID_2_QUEST_INFO;
	MAP_ID_2_QUEST_INFO					m_mapID2QuestInfo;

#ifdef _SERVER
	typedef std::multimap<DWORD, DWORD>	MAP_NPC_ID_2_QUEST_ID;
	MAP_NPC_ID_2_QUEST_ID				m_mapNpcID2QuestID;						// �������������ߵ�NPC
#endif

	typedef std::multimap<int64_t, DWORD>	MAP_NPC_2_QUEST_INFO;
	MAP_NPC_2_QUEST_INFO				m_mapNpc2QuestInfo;						//������ʼ����ɵ�NPC

	typedef std::multimap<DWORD, DWORD> MAP_DOODAD_ID_2_QUEST_ID;
	MAP_DOODAD_ID_2_QUEST_ID			m_mapDoodadID2QuestID;					//�������������ߵ�Doodad

	typedef std::multimap<DWORD, DWORD> MAP_RDOODAD_ID_2_QUEST_ID;
	MAP_RDOODAD_ID_2_QUEST_ID			m_mapRDoodadID2QuestID;					//�������Doodad

	KQuestInfo	m_DefaultQuestInfo;		//Ĭ���趨

    // �������ļ��ж�ȡdwQuestID�ŵ�������Ϣ
    BOOL LoadQuestInfoByID(ITabFile* pTabFile, DWORD dwQuestID, KQuestInfo* pQuestInfo);
    // �������ļ��ж�ȡ��nIndex�е�������Ϣ
	BOOL LoadQuestInfo(ITabFile* piTabFile, int nIndex, KQuestInfo* pQuestInfo);
    // ����Quest��Ϣ��дMap��Ϣ
    BOOL MakeQuestMap(KQuestInfo* cpQuestInfo);

	friend class KDropCenter;
    friend class KDoodad;
};

#endif	//_KQUEST_INFO_LIST_H_
