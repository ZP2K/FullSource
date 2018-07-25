#include "stdafx.h"
#include "KQuestInfoList.h"

int KQuestInfo::LuaGetHortation(Lua_State* L)
{
	int nHave = false;
	int nTopIndex = Lua_GetTopIndex(L);

	Lua_NewTable(L);
	int nTable = Lua_GetTopIndex(L);

	if (nPresentMoney)
	{
		Lua_PushString(L, "money");
		Lua_PushNumber(L, nPresentMoney);
		Lua_SetTable(L, nTable);
		nHave = true;
	}

    if (nPresentSkill)
    {
        Lua_PushString(L, "skill");
        Lua_PushNumber(L, nPresentSkill);
        Lua_SetTable(L, nTable);
        nHave = true;
    }

	if (nPresentPrestige)
	{
		Lua_PushString(L, "prestige");
		Lua_PushNumber(L, nPresentPrestige);
		Lua_SetTable(L, nTable);
		nHave = true;
	}

	if (nPresentContribution)
	{
		Lua_PushString(L, "contribution");
		Lua_PushNumber(L, nPresentContribution);
		Lua_SetTable(L, nTable);
		nHave = true;
	}

	int nNotSet = true;
	int nRelIndex = 1;
	for (int i = 0; i < QUEST_PARAM_COUNT; ++i)
	{
		if (dwAffectForceID[i] != 0 && nAffectForceValue[i] != 0)
		{
			if (nNotSet)
			{
				Lua_PushString(L, "reputation");
				Lua_NewTable(L);
				nHave = true;
				nNotSet = false;
			}

			Lua_PushNumber(L, nRelIndex);
			Lua_NewTable(L);
			++nRelIndex;

			Lua_PushString(L, "force");
			Lua_PushNumber(L, dwAffectForceID[i]);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "value");
			Lua_PushNumber(L, nAffectForceValue[i]);
			Lua_SetTable(L, -3);

			Lua_SetTable(L, -3);
		}
	}
	if (!nNotSet)
	{
		Lua_SetTable(L, nTable);
	}

	nNotSet = true;
	nRelIndex = 1;

    if (bHidePresent) 
    {   // ������ʾ,Ϊ�ƶ��ĵ���ͼ����ʱ��ΪType5,Index2931,Count1
        if (nNotSet)
        {
            Lua_PushString(L, "itemgroup1");
            Lua_NewTable(L);

            Lua_PushString(L, "all");
            Lua_PushBoolean(L, bPresentAll[0]);
            Lua_SetTable(L, -3);
            nHave = true;
            nNotSet = false;
        }

        Lua_PushNumber(L, nRelIndex);
        Lua_NewTable(L);
        ++nRelIndex;

        Lua_PushString(L, "selectindex");
        Lua_PushNumber(L, 0);
        Lua_SetTable(L, -3);

        Lua_PushString(L, "count");
        Lua_PushNumber(L, 1);
        Lua_SetTable(L, -3);

        Lua_PushString(L, "version");
        Lua_PushNumber(L, CURRENT_ITEM_VERSION);
        Lua_SetTable(L, -3);

        Lua_PushString(L, "type");
        Lua_PushNumber(L, 5);
        Lua_SetTable(L, -3);

        Lua_PushString(L, "index");
        Lua_PushNumber(L, 2931);
        Lua_SetTable(L, -3);

        Lua_SetTable(L, -3);
    }
    else
    {
        for (int i = 0; i < QUEST_PARAM_COUNT; ++i)
        {
            if (dwPresentItemAmount[i])
            {
                if (nNotSet)
                {
                    Lua_PushString(L, "itemgroup1");
                    Lua_NewTable(L);

                    Lua_PushString(L, "all");
                    Lua_PushBoolean(L, bPresentAll[0]);
                    Lua_SetTable(L, -3);
                    nHave = true;
                    nNotSet = false;
                }

                Lua_PushNumber(L, nRelIndex);
                Lua_NewTable(L);
                ++nRelIndex;

                Lua_PushString(L, "selectindex");
                Lua_PushNumber(L, i);
                Lua_SetTable(L, -3);

                Lua_PushString(L, "count");
                Lua_PushNumber(L, dwPresentItemAmount[i]);
                Lua_SetTable(L, -3);

                Lua_PushString(L, "version");
                Lua_PushNumber(L, CURRENT_ITEM_VERSION);
                Lua_SetTable(L, -3);

                Lua_PushString(L, "type");
                Lua_PushNumber(L, dwPresentItemType[i]);
                Lua_SetTable(L, -3);

                Lua_PushString(L, "index");
                Lua_PushNumber(L, dwPresentItemIndex[i]);
                Lua_SetTable(L, -3);

                Lua_SetTable(L, -3);
            }
        }
    }
	if (!nNotSet)
	{
		Lua_SetTable(L, nTable);
	}

	nNotSet = true;
	nRelIndex = 1;
	for (int i = QUEST_PARAM_COUNT; i < QUEST_PARAM_COUNT * 2; ++i)
	{
		if (dwPresentItemAmount[i])
		{
			if (nNotSet)
			{
				Lua_PushString(L, "itemgroup2");
				Lua_NewTable(L);

				Lua_PushString(L, "all");
				Lua_PushBoolean(L, bPresentAll[1]);
				Lua_SetTable(L, -3);
				nHave = true;
				nNotSet = false;
			}

			Lua_PushNumber(L, nRelIndex);
			Lua_NewTable(L);
			++nRelIndex;

			Lua_PushString(L, "selectindex");
			Lua_PushNumber(L, i);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "count");
			Lua_PushNumber(L, dwPresentItemAmount[i]);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "version");
			Lua_PushNumber(L, CURRENT_ITEM_VERSION);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "type");
			Lua_PushNumber(L, dwPresentItemType[i]);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "index");
			Lua_PushNumber(L, dwPresentItemIndex[i]);
			Lua_SetTable(L, -3);

			Lua_SetTable(L, -3);
		}
	}
	if (!nNotSet)
	{
		Lua_SetTable(L, nTable);
	}

	if (nHave)
	{
		assert(nTopIndex + 1 == Lua_GetTopIndex(L));
		return 1;
	}
	else
	{
		Lua_SetTopIndex(L, nTopIndex);
		return 0;
	}
}

DEFINE_LUA_CLASS_BEGIN(KQuestInfo)
	REGISTER_LUA_STRING(KQuestInfo, Name)
	REGISTER_LUA_STRING(KQuestInfo, QuestClass)
	REGISTER_LUA_BOOL(KQuestInfo, Share)					//�Ƿ�ɹ���
	REGISTER_LUA_BOOL(KQuestInfo, Repeat)					//�Ƿ���ظ�
	REGISTER_LUA_BOOL(KQuestInfo, Assist)					//�Ƿ��Э��
	REGISTER_LUA_BOOL(KQuestInfo, Accept)					//�Ƿ��ȽӲ��ܽ�
	REGISTER_LUA_INTEGER(KQuestInfo, Level)					//����ȼ�

	REGISTER_LUA_STRING(KQuestInfo, QuestDiff)				//�����Ѷ�����

	REGISTER_LUA_DWORD(KQuestInfo, StartMapID)				//������Npc���ڵ�ͼ��ID
	REGISTER_LUA_DWORD(KQuestInfo, StartNpcTemplateID)		//������Npc��ID
	REGISTER_LUA_DWORD(KQuestInfo, StartItemType)			//������Item����
	REGISTER_LUA_DWORD(KQuestInfo, StartItemIndex)			//������Item����

#ifdef _CLIENT
	REGISTER_LUA_STRING(KQuestInfo, Objective)			
	REGISTER_LUA_STRING(KQuestInfo, Description)
	REGISTER_LUA_INTEGER(KQuestInfo, StartGossipType)		//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, StartGossip1)			//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, StartGossip2)			//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, StartGossip3)			//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, StartGossip4)			//��������̸����
#endif

    REGISTER_LUA_DWORD(KQuestInfo, RequireSchoolMask)

    REGISTER_LUA_BOOL(KQuestInfo, RequireReputeAll)
    REGISTER_LUA_DWORD(KQuestInfo, RequireForceID1)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMin1)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMax1)
    REGISTER_LUA_DWORD(KQuestInfo, RequireForceID2)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMin2)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMax2)
    REGISTER_LUA_DWORD(KQuestInfo, RequireForceID3)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMin3)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMax3)
    REGISTER_LUA_DWORD(KQuestInfo, RequireForceID4)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMin4)
    REGISTER_LUA_INTEGER(KQuestInfo, ReputeLevelMax4)

    REGISTER_LUA_INTEGER(KQuestInfo, MinLevel)			//��������С�ȼ�����
	REGISTER_LUA_INTEGER(KQuestInfo, MaxLevel)			//���������ȼ�����
	REGISTER_LUA_INTEGER(KQuestInfo, RequireGender)		//��������Ա�����
	REGISTER_LUA_INTEGER(KQuestInfo, RequireRoleType)	//���������������
	REGISTER_LUA_DWORD(KQuestInfo, CoolDownID)			//CoolDown��ʱ��ID
	REGISTER_LUA_BOOL(KQuestInfo, PrequestLogic)
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID1)			//ǰ������ID
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID2)			//ǰ������ID
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID3)			//ǰ������ID
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID4)			//ǰ������ID
	REGISTER_LUA_INTEGER(KQuestInfo, StartTime)			//��ʼʱ��
	REGISTER_LUA_INTEGER(KQuestInfo, EndTime)			//����ʱ��
	REGISTER_LUA_DWORD(KQuestInfo, SubsequenceID)		//ֱ�Ӻ������ID   

	REGISTER_LUA_DWORD(KQuestInfo, RequireSkillID)		//������
	REGISTER_LUA_DWORD(KQuestInfo, MinSkillLevel)		//��������С�ȼ�

	REGISTER_LUA_DWORD(KQuestInfo, RequireLifeSkillID)		//������
	REGISTER_LUA_DWORD(KQuestInfo, MinLifeSkillLevel)		//��������С�ȼ�

	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemType1)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemIndex1)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemAmount1)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemType2)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemIndex2)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemAmount2)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemType3)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemIndex3)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemAmount3)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemType4)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemIndex4)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, StartRequireItemAmount4)	//������ʱ������ߵ�����

	REGISTER_LUA_DWORD(KQuestInfo, OfferItemType1)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemIndex1)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemAmount1)		//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemType2)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemIndex2)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemAmount2)		//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemType3)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemIndex3)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemAmount3)		//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemType4)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemIndex4)			//������ʱ�ṩ���ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, OfferItemAmount4)		//������ʱ�ṩ���ߵ�����

	REGISTER_LUA_DWORD(KQuestInfo, EndMapID)				//������Npc���ڵĵ�ͼID
	REGISTER_LUA_DWORD(KQuestInfo, EndNpcTemplateID)		//������Npc��ID
    REGISTER_LUA_DWORD(KQuestInfo, EndDoodadTemplateID)     //������Doodad��ID
	REGISTER_LUA_INTEGER(KQuestInfo, EndItemType)			//������Item����
	REGISTER_LUA_INTEGER(KQuestInfo, EndItemIndex)			//������Item����

#ifdef _CLIENT
	REGISTER_LUA_STRING(KQuestInfo, DunningDialogue)
	REGISTER_LUA_STRING(KQuestInfo, UnfinishedDialogue)
	REGISTER_LUA_STRING(KQuestInfo, FinishedDialogue)
	REGISTER_LUA_INTEGER(KQuestInfo, EndGossipType)			//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, EndGossip1)				//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, EndGossip2)				//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, EndGossip3)				//��������̸����
	REGISTER_LUA_STRING(KQuestInfo, EndGossip4)				//��������̸����
#endif

	REGISTER_LUA_INTEGER(KQuestInfo, FinishTime)			//���ʱ������

	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType1)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex1)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount1)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem1)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID1)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID1)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate1)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach1)				//�Ƿ�ÿ���˵�һ��

	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType2)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex2)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount2)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem2)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID2)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID2)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate2)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach2)				//�Ƿ�ÿ���˵�һ��

	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType3)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex3)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount3)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem3)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID3)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID3)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate3)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach3)				//�Ƿ�ÿ���˵�һ��

	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType4)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex4)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount4)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem4)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID4)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID4)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate4)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach4)				//�Ƿ�ÿ���˵�һ��

    REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType5)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex5)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount5)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem5)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID5)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID5)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate5)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach5)				//�Ƿ�ÿ���˵�һ��

    REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType6)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex6)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount6)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem6)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID6)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID6)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate6)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach6)				//�Ƿ�ÿ���˵�һ��

    REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType7)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex7)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount7)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem7)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID7)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID7)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate7)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach7)				//�Ƿ�ÿ���˵�һ��

    REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemType8)		//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemIndex8)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, EndRequireItemAmount8)	//������ʱ������ߵ�����
	REGISTER_LUA_BOOL(KQuestInfo, IsDeleteEndRequireItem8)  //�Ƿ�ɾ������
	REGISTER_LUA_DWORD(KQuestInfo, DropItemNpcTemplateID8)		//���佻������ߵ�Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemDoodadTemplateID8)	//���佻������ߵ�doodadģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, DropItemRate8)			//����������ߵļ���
	REGISTER_LUA_BOOL(KQuestInfo, DropForEach8)				//�Ƿ�ÿ���˵�һ��

	REGISTER_LUA_INTEGER(KQuestInfo, CostMoney)				//���ĵĽ�Ǯ

	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID1)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcAmount1)		//������ʱ����ɱ����Npc����
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID2)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcAmount2)		//������ʱ����ɱ����Npc����
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID3)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcAmount3)		//������ʱ����ɱ����Npc����
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID4)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcAmount4)		//������ʱ����ɱ����Npc����

	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue1)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue2)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue3)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue4)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue5)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue6)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue7)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue8)			//�������

#ifdef _CLIENT
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr1)
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr2)
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr3)
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr4)
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr5)
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr6)
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr7)
	REGISTER_LUA_STRING(KQuestInfo, QuestValueStr8)
#endif

	REGISTER_LUA_INTEGER(KQuestInfo, HidePresent)

	REGISTER_LUA_INTEGER(KQuestInfo, PresentAll1)		//��һ��(KQuestInfo, 1-4)�����Ƿ�ȫ������ֻ��һ��
	REGISTER_LUA_INTEGER(KQuestInfo, PresentAll2)		//�ڶ���(KQuestInfo, 5-8)�����Ƿ�ȫ������ֻ��һ��

	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType1)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex1)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount1)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType2)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex2)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount2)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType3)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex3)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount3)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType4)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex4)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount4)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType5)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex5)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount5)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType6)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex6)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount6)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType7)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex7)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount7)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemType8)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemIndex8)	//������ʱ�������ߵ�����
	REGISTER_LUA_DWORD(KQuestInfo, PresentItemAmount8)	//������ʱ�������ߵ�����

	REGISTER_LUA_INTEGER(KQuestInfo, PresentExp)			//������ʱ�����ľ���
	REGISTER_LUA_INTEGER(KQuestInfo, PresentMoney)			//������ʱ�����Ľ�Ǯ����
    REGISTER_LUA_INTEGER(KQuestInfo, PresentSkill)

    REGISTER_LUA_INTEGER(KQuestInfo, PresentAssistThew)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentAssistStamina)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentAssistFriendship)

	REGISTER_LUA_DWORD(KQuestInfo, AffectForceID1)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue1)		//������Ӱ��������Ѻö�
	REGISTER_LUA_DWORD(KQuestInfo, AffectForceID2)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue2)		//������Ӱ��������Ѻö�
	REGISTER_LUA_DWORD(KQuestInfo, AffectForceID3)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue3)		//������Ӱ��������Ѻö�
	REGISTER_LUA_DWORD(KQuestInfo, AffectForceID4)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue4)		//������Ӱ��������Ѻö�

	REGISTER_LUA_INTEGER(KQuestInfo, RepeatCutPercent)		//�ظ�������ʱ�����������ٷֱ�

    REGISTER_LUA_INTEGER(KQuestInfo, RequireCampMask)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentPrestige)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentContribution)
	
    REGISTER_LUA_FUNC(KQuestInfo, GetHortation)

DEFINE_LUA_CLASS_END(KQuestInfo)
