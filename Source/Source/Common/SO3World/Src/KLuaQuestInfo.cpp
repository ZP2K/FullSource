#include "stdafx.h"
#include "KQuestInfoList.h"
#include "KSO3World.h"

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

    if (wPresentSkill)
    {
        Lua_PushString(L, "skill");
        Lua_PushNumber(L, wPresentSkill);
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

    if (nPresentTrain)
    {
        Lua_PushString(L, "presenttrain");
        Lua_PushNumber(L, nPresentTrain);
        Lua_SetTable(L, nTable);
        nHave = true;
    }

	int nNotSet = true;
	int nRelIndex = 1;
	for (int i = 0; i < QUEST_PARAM_COUNT; ++i)
	{
		if (byAffectForceID[i] != 0 && nAffectForceValue[i] != 0)
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
			Lua_PushNumber(L, byAffectForceID[i]);
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
        Lua_PushNumber(L, 0);
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
            if (wPresentItemAmount[i])
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
                Lua_PushNumber(L, wPresentItemAmount[i]);
                Lua_SetTable(L, -3);

                Lua_PushString(L, "version");
                Lua_PushNumber(L, 0);
                Lua_SetTable(L, -3);

                Lua_PushString(L, "type");
                Lua_PushNumber(L, byPresentItemType[i]);
                Lua_SetTable(L, -3);

                Lua_PushString(L, "index");
                Lua_PushNumber(L, wPresentItemIndex[i]);
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
		if (wPresentItemAmount[i])
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
			Lua_PushNumber(L, wPresentItemAmount[i]);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "version");
			Lua_PushNumber(L, 0);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "type");
			Lua_PushNumber(L, byPresentItemType[i]);
			Lua_SetTable(L, -3);

			Lua_PushString(L, "index");
			Lua_PushNumber(L, wPresentItemIndex[i]);
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
    REGISTER_LUA_DWORD(KQuestInfo, QuestClassID)

	REGISTER_LUA_BOOL(KQuestInfo, Share)					//�Ƿ�ɹ���
	REGISTER_LUA_BOOL(KQuestInfo, Repeat)					//�Ƿ���ظ�
	REGISTER_LUA_BOOL(KQuestInfo, Assist)					//�Ƿ��Э��
	REGISTER_LUA_BOOL(KQuestInfo, Accept)					//�Ƿ��ȽӲ��ܽ�
	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, Level)					//����ȼ�

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartMapID)		//������Npc���ڵ�ͼ��ID
	REGISTER_LUA_DWORD(KQuestInfo, StartNpcTemplateID)		//������Npc��ID
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartItemType)			//������Item����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartItemIndex)			//������Item����

    REGISTER_LUA_DWORD(KQuestInfo, RequireSchoolMask)

    REGISTER_LUA_BOOL(KQuestInfo, RequireReputeAll)
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, RequireForceID1)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMin1)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMax1)
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, RequireForceID2)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMin2)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMax2)
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, RequireForceID3)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMin3)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMax3)
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, RequireForceID4)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMin4)
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, ReputeLevelMax4)

    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, MinLevel)			//��������С�ȼ�����
	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, MaxLevel)			//���������ȼ�����
	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, RequireGender)		//��������Ա�����
	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, RequireRoleType)	//���������������
	REGISTER_LUA_DWORD(KQuestInfo, CoolDownID)			//CoolDown��ʱ��ID
	REGISTER_LUA_BOOL(KQuestInfo, PrequestLogic)
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID1)			//ǰ������ID
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID2)			//ǰ������ID
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID3)			//ǰ������ID
	REGISTER_LUA_DWORD(KQuestInfo, PrequestID4)			//ǰ������ID
	REGISTER_LUA_INTEGER(KQuestInfo, StartTime)			//��ʼʱ��
	REGISTER_LUA_INTEGER(KQuestInfo, EndTime)			//����ʱ��
	REGISTER_LUA_DWORD(KQuestInfo, SubsequenceID)		//ֱ�Ӻ������ID   

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, RequireSkillID)		//������
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, MinSkillLevel)		//��������С�ȼ�

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, RequireLifeSkillID)		//������
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, MinLifeSkillLevel)		//��������С�ȼ�

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemType1)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemIndex1)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemAmount1)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemType2)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemIndex2)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemAmount2)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemType3)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemIndex3)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemAmount3)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemType4)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemIndex4)	//������ʱ������ߵ�����
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, StartRequireItemAmount4)	//������ʱ������ߵ�����

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndMapID)		//������Npc���ڵĵ�ͼID
	REGISTER_LUA_DWORD(KQuestInfo, EndNpcTemplateID)		//������Npc��ID
    REGISTER_LUA_DWORD(KQuestInfo, EndDoodadTemplateID)     //������Doodad��ID
	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, EndItemType)			//������Item����
	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, EndItemIndex)			//������Item����

	REGISTER_LUA_INTEGER(KQuestInfo, FinishTime)			//���ʱ������

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType1)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex1)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount1)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType2)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex2)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount2)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType3)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex3)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount3)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType4)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex4)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount4)	
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType5)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex5)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount5)	
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType6)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex6)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount6)	
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType7)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex7)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount7)	
    REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemType8)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemIndex8)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, EndRequireItemAmount8)	

	REGISTER_LUA_INTEGER(KQuestInfo, CostMoney)				//���ĵĽ�Ǯ

	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID1)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, KillNpcAmount1)		//������ʱ����ɱ����Npc����
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID2)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, KillNpcAmount2)		//������ʱ����ɱ����Npc����
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID3)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, KillNpcAmount3)		//������ʱ����ɱ����Npc����
	REGISTER_LUA_DWORD(KQuestInfo, KillNpcTemplateID4)		//������ʱ����ɱ����Npcģ��ID
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, KillNpcAmount4)		//������ʱ����ɱ����Npc����

	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue1)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue2)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue3)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue4)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue5)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue6)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue7)			//�������
	REGISTER_LUA_INTEGER(KQuestInfo, QuestValue8)			//�������

	REGISTER_LUA_BOOL(KQuestInfo, HidePresent)

	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, PresentAll1)		//��һ��(KQuestInfo, 1-4)�����Ƿ�ȫ������ֻ��һ��
	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, PresentAll2)		//�ڶ���(KQuestInfo, 5-8)�����Ƿ�ȫ������ֻ��һ��

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType1)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex1)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount1)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType2)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex2)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount2)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType3)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex3)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount3)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType4)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex4)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount4)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType5)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex5)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount5)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType6)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex6)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount6)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType7)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex7)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount7)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemType8)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemIndex8)	
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, PresentItemAmount8)	

	REGISTER_LUA_INTEGER(KQuestInfo, PresentExp)			//������ʱ�����ľ���
	REGISTER_LUA_INTEGER(KQuestInfo, PresentMoney)			//������ʱ�����Ľ�Ǯ����
    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, PresentSkill)

    REGISTER_LUA_INTEGER(KQuestInfo, PresentAssistThew)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentAssistStamina)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentAssistFriendship)

	REGISTER_LUA_DWORD_READONLY(KQuestInfo, AffectForceID1)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue1)		//������Ӱ��������Ѻö�
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, AffectForceID2)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue2)		//������Ӱ��������Ѻö�
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, AffectForceID3)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue3)		//������Ӱ��������Ѻö�
	REGISTER_LUA_DWORD_READONLY(KQuestInfo, AffectForceID4)			//������Ӱ�������
	REGISTER_LUA_INTEGER(KQuestInfo, AffectForceValue4)		//������Ӱ��������Ѻö�

	REGISTER_LUA_INTEGER_READONLY(KQuestInfo, RepeatCutPercent)		//�ظ�������ʱ�����������ٷֱ�

    REGISTER_LUA_INTEGER_READONLY(KQuestInfo, RequireCampMask)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentPrestige)
    REGISTER_LUA_INTEGER(KQuestInfo, PresentContribution)
	
    REGISTER_LUA_FUNC(KQuestInfo, GetHortation)

DEFINE_LUA_CLASS_END(KQuestInfo)
