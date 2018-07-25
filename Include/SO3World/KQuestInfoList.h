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
#include "KLRUCacheMap.h"

#define QUEST_NAME_LEN					32
#define QUEST_OBJECTIVE_LEN				1026
#define QUEST_DESCRIPTION_LEN			1024
#define QUEST_UNFINISHED_DIALOGUE_LEN	1024
#define QUEST_FINISHED_DIALOGUE_LEN		1024
#define QUEST_VALUE_STR_LEN				256
#define QUEST_GOSSIP_LEN		        1024

#define MAX_QUEST_COUNT			        8192
#define QUEST_END_ITEM_COUNT            8

struct KQuestInfo
{
	DWORD			dwQuestID;
    DWORD           dwClassID;
#ifdef _SERVER
    char            szQuestName[QUEST_NAME_LEN];
#endif

	bool            bShare;										// �Ƿ�ɹ���
	bool            bRepeat;									// �Ƿ���ظ�
	bool            bAssist;									// �Ƿ��Э��
	bool            bAccept;									// �Ƿ���Ҫ���˲��ܽ�

    bool			bEscortQuest;                               // �Ƿ�������
    bool			bPrequestLogic;								// true:and;false:or
    bool            bRequireReputeAll;                          // ������ʱ�������Ƶ��߼���ϵ(true: AND, false: OR)
    bool			bHidePresent;								// �Ƿ����ؽ���

    bool			bPresentAll[2];								// ��һ��(1-4)�����Ƿ�ȫ������ֻ��һ��
    BYTE            byStartMapID;								// ������Npc���ڵ�ͼ��ID, ��ʱ����
    BYTE            byEndMapID;									//������Npc���ڵĵ�ͼID, ��ʱ����
    
    BYTE            byLevel;                                    // ����ȼ�
    BYTE            byStartItemType;							// ������Item����
    WORD			wStartItemIndex;							// ������Item����			

    BYTE            byMinLevel;									// ��������С�ȼ�����
    BYTE            byMaxLevel;									// ���������ȼ�����
    BYTE            byRequireGender;								// ��������Ա�����
    BYTE            byRequireRoleType;							// ��������������������

    WORD            wRequireSkillID;							// ������
    BYTE            byMinSkillLevel;							// ��������С�ȼ�
    BYTE			byRequireLifeSkillID;						// ���������

    BYTE			byMinLifeSkillLevel;						// ��������С�ȼ�
    bool            bRequireApprentice;                         // ������ͽ��

    BYTE			byStartRequireItemType[QUEST_PARAM_COUNT];	
    WORD			wStartRequireItemIndex[QUEST_PARAM_COUNT];	
    WORD            wStartRequireItemAmount[QUEST_PARAM_COUNT];

#ifdef _SERVER
    BYTE            byOfferItemType[QUEST_PARAM_COUNT];			// ������ʱ�ṩ���ߵ�����
    WORD            wOfferItemIndex[QUEST_PARAM_COUNT];		// ������ʱ�ṩ���ߵ�����
    WORD            wOfferItemAmount[QUEST_PARAM_COUNT];		// ������ʱ�ṩ���ߵ�����
#endif

    BYTE            byRequireForceID[QUEST_PARAM_COUNT];
    BYTE            byReputeLevelMin[QUEST_PARAM_COUNT];
    BYTE            byReputeLevelMax[QUEST_PARAM_COUNT];

    BYTE			byEndItemType;								//������Item����
    WORD            wEndItemIndex;								//������Item����
    BYTE            byRepeatCutPercent;							// �ظ�������ʱ�����������ٷֱ�

    BYTE            byRequireCampMask;                           // ��Ӫ����; ��0,1,2λ�ֱ��Ӧ���������塢а��
    BYTE            byTeamRequireMode;                           // �Ŷ�ģʽ�����������
    WORD            wPresentSkill;								// �����ļ���ID

    BYTE            byEndRequireItemType[QUEST_END_ITEM_COUNT];	//������ʱ������ߵ�����
    WORD			wEndRequireItemIndex[QUEST_END_ITEM_COUNT];	//������ʱ������ߵ�����
    WORD            wEndRequireItemAmount[QUEST_END_ITEM_COUNT];	//������ʱ������ߵ�����
    DWORD			dwDropItemDoodadTemplateID[QUEST_END_ITEM_COUNT];	//���佻������ߵ�doodadģ��ID
#ifdef _SERVER
    DWORD			dwDropItemNpcTemplateID[QUEST_END_ITEM_COUNT];		//���佻������ߵ�Npcģ��ID
    DWORD			dwDropItemRate[QUEST_END_ITEM_COUNT];			//����������ߵļ���
    bool			bDropForEach[QUEST_END_ITEM_COUNT];			    //�Ƿ�ÿ�˶����һ��
    bool			bIsDeleteEndRequireItem[QUEST_END_ITEM_COUNT]; //���������ɾ�������ʱ���Ƿ�ɾ���������
#endif

    DWORD			dwRelationDoodadID[QUEST_PARAM_COUNT * 2];  // �����������DoodadID

    BYTE			byPresentItemType[QUEST_PARAM_COUNT * 2];	// ������ʱ�������ߵ�����
    WORD			wPresentItemIndex[QUEST_PARAM_COUNT * 2];	// ������ʱ�������ߵ�����
    WORD            wPresentItemAmount[QUEST_PARAM_COUNT * 2];	// ������ʱ�������ߵ�����

    BYTE            byAffectForceID[QUEST_PARAM_COUNT];			// ������Ӱ�������
    int             nAffectForceValue[QUEST_PARAM_COUNT];		// ������Ӱ��������Ѻö�
	
	DWORD			dwStartNpcTemplateID;						// ������Npc��ģ��ID
	DWORD			dwStartDoodadTemplateID;					// ������Doodad��ģ��ID
    
	DWORD			dwCoolDownID;								// CoolDown��ʱ��ID
	
	DWORD			dwPrequestID[QUEST_PARAM_COUNT];			// ǰ������ID
	DWORD			dwSubsequenceID;							// ֱ�Ӻ������ID
	int				nStartTime;									// ��ʼʱ��
	int				nEndTime;									// ����ʱ��

    DWORD           dwRequireSchoolMask;                        // ������ʱ����������
    
	DWORD			dwEndNpcTemplateID;							//������Npc��ģ��ID
	DWORD			dwEndDoodadTemplateID;						//������Doodad��ģ��ID
    
	int				nFinishTime;								//���ʱ������

	int				nCostMoney;									//����������Ľ�Ǯ

	DWORD			dwKillNpcTemplateID[QUEST_PARAM_COUNT];		//������ʱ����ɱ����Npcģ��ID
	WORD			wKillNpcAmount[QUEST_PARAM_COUNT];			//������ʱ����ɱ����Npc����

	int				nQuestValue[QUEST_PARAM_COUNT * 2];			//�������

	int				nPresentExp;								// ������ʱ�����ľ���
	int				nPresentMoney;								// ������ʱ�����Ľ�Ǯ����

    int             nPresentAssistThew;                         // Э������������
    int             nPresentAssistStamina;                      // Э������������
    int             nPresentAssistFriendship;                   // Э���������øж�

    DWORD           dwScriptID;
    
    int             nPresentPrestige;                           // ����������
    int             nPresentContribution;                       // ����������ֵ
    int             nPresentTrain;                              // ��������Ϊ
#ifdef _SERVER
    int             nAchievementID;                             // ����ɾ�
    int             nAssistMentorValue;                         // Э����������ʦ��Э���ǣ���ʦ���ӵ�ʦͽֵ
#endif

//Lua�ű��ӿ�
    DWORD getQuestClass(){return dwClassID;}
    DWORD getStartMapID() {return byStartMapID;}
    DWORD getEndMapID() {return byEndMapID;}
    int   getPresentAll1() {return bPresentAll[0];}
    int   getPresentAll2() {return bPresentAll[1];}
    int   getLevel() {return byLevel;}
    DWORD getStartItemType() {return byStartItemType;}
    DWORD getStartItemIndex() {return wStartItemIndex;}
    int   getMinLevel() {return byMinLevel;}
    int   getMaxLevel() {return byMaxLevel;}
    int   getRequireGender() {return byRequireGender;}
    int   getRequireRoleType() {return byRequireRoleType;}
    DWORD getRequireSkillID() {return wRequireSkillID;}
    DWORD getMinSkillLevel() {return byMinSkillLevel;}
    DWORD getRequireLifeSkillID() {return byRequireLifeSkillID;}
    DWORD getMinLifeSkillLevel() {return byMinLifeSkillLevel;}

    DWORD getStartRequireItemType1() {return byStartRequireItemType[0];}
    DWORD getStartRequireItemIndex1() {return wStartRequireItemIndex[0];};
    DWORD getStartRequireItemAmount1() {return wStartRequireItemAmount[0];}
    
    DWORD getStartRequireItemType2() {return byStartRequireItemType[1];}
    DWORD getStartRequireItemIndex2() {return wStartRequireItemIndex[1];}
    DWORD getStartRequireItemAmount2() {return wStartRequireItemAmount[1];}

    DWORD getStartRequireItemType3() {return byStartRequireItemType[2];}
    DWORD getStartRequireItemIndex3() {return wStartRequireItemIndex[2];}
    DWORD getStartRequireItemAmount3() {return wStartRequireItemAmount[2];}

    DWORD getStartRequireItemType4() {return byStartRequireItemType[3];}
    DWORD getStartRequireItemIndex4() {return wStartRequireItemIndex[3];}
    DWORD getStartRequireItemAmount4() {return wStartRequireItemAmount[3];}
    
    DWORD getRequireForceID1() {return byRequireForceID[0];}
    int   getReputeLevelMin1() {return byReputeLevelMin[0];}
    int   getReputeLevelMax1() {return byReputeLevelMax[0];}
    
    DWORD getRequireForceID2() {return byRequireForceID[1];}
    int   getReputeLevelMin2() {return byReputeLevelMin[1];}
    int   getReputeLevelMax2() {return byReputeLevelMax[1];}

    DWORD getRequireForceID3() {return byRequireForceID[2];}
    int   getReputeLevelMin3() {return byReputeLevelMin[2];}
    int   getReputeLevelMax3() {return byReputeLevelMax[2];}

    DWORD getRequireForceID4() {return byRequireForceID[3];}
    int   getReputeLevelMin4() {return byReputeLevelMin[3];}
    int   getReputeLevelMax4() {return byReputeLevelMax[3];}

    DWORD getEndItemType() {return byEndItemType;}
    DWORD getEndItemIndex() {return wEndItemIndex;}
    int   getRepeatCutPercent() {return byRepeatCutPercent;}
    int   getRequireCampMask() {return byRequireCampMask;}
    int   getTeamRequireMode() {return byTeamRequireMode;}
    int   getPresentSkill() {return wPresentSkill;}

    DWORD getEndRequireItemType1() {return byEndRequireItemType[0];}
    DWORD getEndRequireItemIndex1() {return wEndRequireItemIndex[0];}
    DWORD getEndRequireItemAmount1() {return wEndRequireItemAmount[0];}

    DWORD getEndRequireItemType2() {return byEndRequireItemType[1];}
    DWORD getEndRequireItemIndex2() {return wEndRequireItemIndex[1];}
    DWORD getEndRequireItemAmount2() {return wEndRequireItemAmount[1];}

    DWORD getEndRequireItemType3() {return byEndRequireItemType[2];}
    DWORD getEndRequireItemIndex3() {return wEndRequireItemIndex[2];}
    DWORD getEndRequireItemAmount3() {return wEndRequireItemAmount[2];}

    DWORD getEndRequireItemType4() {return byEndRequireItemType[3];}
    DWORD getEndRequireItemIndex4() {return wEndRequireItemIndex[3];}
    DWORD getEndRequireItemAmount4() {return wEndRequireItemAmount[3];}

    DWORD getEndRequireItemType5() {return byEndRequireItemType[4];}
    DWORD getEndRequireItemIndex5() {return wEndRequireItemIndex[4];}
    DWORD getEndRequireItemAmount5() {return wEndRequireItemAmount[4];}

    DWORD getEndRequireItemType6() {return byEndRequireItemType[5];}
    DWORD getEndRequireItemIndex6() {return wEndRequireItemIndex[5];}
    DWORD getEndRequireItemAmount6() {return wEndRequireItemAmount[5];}

    DWORD getEndRequireItemType7() {return byEndRequireItemType[6];}
    DWORD getEndRequireItemIndex7() {return wEndRequireItemIndex[6];}
    DWORD getEndRequireItemAmount7() {return wEndRequireItemAmount[6];}

    DWORD getEndRequireItemType8() {return byEndRequireItemType[7];}
    DWORD getEndRequireItemIndex8() {return wEndRequireItemIndex[7];}
    DWORD getEndRequireItemAmount8() {return wEndRequireItemAmount[7];}

    DWORD getPresentItemType1() {return byPresentItemType[0];}
    DWORD getPresentItemIndex1() {return wPresentItemIndex[0];}
    DWORD getPresentItemAmount1() {return wPresentItemAmount[0];}
    
    DWORD getPresentItemType2() {return byPresentItemType[1];}
    DWORD getPresentItemIndex2() {return wPresentItemIndex[1];}
    DWORD getPresentItemAmount2() {return wPresentItemAmount[1];}

    DWORD getPresentItemType3() {return byPresentItemType[2];}
    DWORD getPresentItemIndex3() {return wPresentItemIndex[2];}
    DWORD getPresentItemAmount3() {return wPresentItemAmount[2];}

    DWORD getPresentItemType4() {return byPresentItemType[3];}
    DWORD getPresentItemIndex4() {return wPresentItemIndex[3];}
    DWORD getPresentItemAmount4() {return wPresentItemAmount[3];}

    DWORD getPresentItemType5() {return byPresentItemType[4];}
    DWORD getPresentItemIndex5() {return wPresentItemIndex[4];}
    DWORD getPresentItemAmount5() {return wPresentItemAmount[4];}

    DWORD getPresentItemType6() {return byPresentItemType[5];}
    DWORD getPresentItemIndex6() {return wPresentItemIndex[5];}
    DWORD getPresentItemAmount6() {return wPresentItemAmount[5];}

    DWORD getPresentItemType7() {return byPresentItemType[6];}
    DWORD getPresentItemIndex7() {return wPresentItemIndex[6];}
    DWORD getPresentItemAmount7() {return wPresentItemAmount[6];}

    DWORD getPresentItemType8() {return byPresentItemType[7];}
    DWORD getPresentItemIndex8() {return wPresentItemIndex[7];}
    DWORD getPresentItemAmount8() {return wPresentItemAmount[7];}
    
    DWORD getAffectForceID1() {return byAffectForceID[0];}
	DWORD getAffectForceID2() {return byAffectForceID[1];}
    DWORD getAffectForceID3() {return byAffectForceID[2];}
    DWORD getAffectForceID4() {return byAffectForceID[3];}

    DWORD getKillNpcAmount1() {return wKillNpcAmount[0];}
    DWORD getKillNpcAmount2() {return wKillNpcAmount[1];}
    DWORD getKillNpcAmount3() {return wKillNpcAmount[2];}
    DWORD getKillNpcAmount4() {return wKillNpcAmount[3];}

    DECLARE_LUA_CLASS(KQuestInfo);

    DECLARE_LUA_STRUCT_DWORD(QuestClassID, dwClassID);	

	DECLARE_LUA_STRUCT_BOOL(Share, bShare);					    // �Ƿ�ɹ���
	DECLARE_LUA_STRUCT_BOOL(Repeat, bRepeat);					// �Ƿ���ظ�
	DECLARE_LUA_STRUCT_BOOL(Assist, bAssist);					// �Ƿ��Э��
	DECLARE_LUA_STRUCT_BOOL(Accept, bAccept);					// �Ƿ��ȽӲ��ܽ�

	DECLARE_LUA_STRUCT_DWORD(StartNpcTemplateID, dwStartNpcTemplateID);			//������Npc��ID
    DECLARE_LUA_STRUCT_DWORD(RequireSchoolMask, dwRequireSchoolMask);

    DECLARE_LUA_STRUCT_BOOL(RequireReputeAll, bRequireReputeAll);

	DECLARE_LUA_STRUCT_DWORD(CoolDownID, dwCoolDownID);		//CoolDown��ʱ��ID
	DECLARE_LUA_STRUCT_BOOL(PrequestLogic, bPrequestLogic);
	DECLARE_LUA_STRUCT_DWORD(PrequestID1, dwPrequestID[0]);			//ǰ������ID
	DECLARE_LUA_STRUCT_DWORD(PrequestID2, dwPrequestID[1]);			//ǰ������ID
	DECLARE_LUA_STRUCT_DWORD(PrequestID3, dwPrequestID[2]);			//ǰ������ID
	DECLARE_LUA_STRUCT_DWORD(PrequestID4, dwPrequestID[3]);			//ǰ������ID
	DECLARE_LUA_STRUCT_INTEGER(StartTime, nStartTime);			//��ʼʱ��
	DECLARE_LUA_STRUCT_INTEGER(EndTime, nEndTime);				//����ʱ��
	DECLARE_LUA_STRUCT_DWORD(SubsequenceID, dwSubsequenceID);	//ֱ�Ӻ������ID   

	DECLARE_LUA_STRUCT_DWORD(EndNpcTemplateID, dwEndNpcTemplateID);			//������Npc��ID
    DECLARE_LUA_STRUCT_DWORD(EndDoodadTemplateID, dwEndDoodadTemplateID);   //������Doodad��ID  

	DECLARE_LUA_STRUCT_INTEGER(FinishTime, nFinishTime);						//���ʱ������

	DECLARE_LUA_STRUCT_INTEGER(CostMoney, nCostMoney);							//���ĵĽ�Ǯ

	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID1, dwKillNpcTemplateID[0]);		//������ʱ����ɱ����Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID2, dwKillNpcTemplateID[1]);		//������ʱ����ɱ����Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID3, dwKillNpcTemplateID[2]);		//������ʱ����ɱ����Npcģ��ID
	DECLARE_LUA_STRUCT_DWORD(KillNpcTemplateID4, dwKillNpcTemplateID[3]);		//������ʱ����ɱ����Npcģ��ID

	DECLARE_LUA_STRUCT_INTEGER(QuestValue1, nQuestValue[0]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue2, nQuestValue[1]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue3, nQuestValue[2]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue4, nQuestValue[3]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue5, nQuestValue[4]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue6, nQuestValue[5]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue7, nQuestValue[6]);						//�������
	DECLARE_LUA_STRUCT_INTEGER(QuestValue8, nQuestValue[7]);						//�������

	DECLARE_LUA_STRUCT_BOOL(HidePresent, bHidePresent);			//�Ƿ����ؽ���

	DECLARE_LUA_STRUCT_INTEGER(PresentExp, nPresentExp);					//������ʱ�����ľ���
	DECLARE_LUA_STRUCT_INTEGER(PresentMoney, nPresentMoney);				//������ʱ�����Ľ�Ǯ����

    DECLARE_LUA_STRUCT_INTEGER(PresentAssistThew, nPresentAssistThew);
    DECLARE_LUA_STRUCT_INTEGER(PresentAssistStamina, nPresentAssistStamina);
    DECLARE_LUA_STRUCT_INTEGER(PresentAssistFriendship, nPresentAssistFriendship);

	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue1, nAffectForceValue[0]);	//������Ӱ��������Ѻö�
	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue2, nAffectForceValue[1]);	//������Ӱ��������Ѻö�
	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue3, nAffectForceValue[2]);	//������Ӱ��������Ѻö�
	DECLARE_LUA_STRUCT_INTEGER(AffectForceValue4, nAffectForceValue[3]);	//������Ӱ��������Ѻö�

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
	
    BOOL LoadQuestInfo(ITabFile* piTabFile, int nIndex, KQuestInfo* pQuestInfo);
    BOOL MakeQuestMap(KQuestInfo* cpQuestInfo);

	friend class KDropCenter;
    friend class KDoodad;
};

#endif	//_KQUEST_INFO_LIST_H_
