#include "stdafx.h"
#include "SO3GlobalDef.h"
#include "Global.h"
#include "Luna.h"
#include "KTongDef.h"
#include "KMentorDef.h"

#define REGISTER_CONST(__VALUE__)	{#__VALUE__, __VALUE__},
#define REGISTER_CONST_LIST(__VALUE__)	{#__VALUE__, LUA_CONST_##__VALUE__},

KLuaConst LUA_TONG_OPERATION_INDEX[] =
{
    {"SPEAK",                       totSpeak},                      //	�ڰ��Ƶ������
    {"JOIN_MANAGER_CHANNEL",        totJoinManagerChannel},         //	�����ԱƵ���������Բ鿴Ƶ������
    {"SPEAK_IN_MANAGER_CHANNEL",    totSpeakInManagerChannel},      //	�ڹ�ԱƵ������
    {"CHECK_OPERATION_RECORD",      totCheckOperationRecord},       //	�鿴��������¼
    {"MODIFY_ANNOUNCEMENT",         totModifyAnnouncement},         //	�޸İ�ṫ��
    {"MODIFY_ONLINE_MESSAGE",       totModifyOnlineMessage},        //	�޸İ��������ʾ
    {"MODIFY_INTRODUCTION",         totModifyIntroduction},         //	�޸İ�����
    {"MODIFY_RULES",                totModifyRules},                //	�޸İ����
    {"MODIFY_MEMORABILIA",          totModifyMemorabilia},          //	�޸İ����¼�
    {"DEVELOP_TECHNOLOGY",          totDevelopTechnology},          //  Ͷ�ʿƼ���
    {"PRODUCT_ITEM",                totProductItem},	            //  ��������

    {"PUT_REPOSITORY_PAGE2",        totPutRepositoryPage2},         // 	���ֿ��ڵڶ�ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE3",        totPutRepositoryPage3},	        //  ���ֿ��ڵ���ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE4",        totPutRepositoryPage4},	        //  ���ֿ��ڵ���ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE5",        totPutRepositoryPage5},	        //  ���ֿ��ڵ���ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE6",        totPutRepositoryPage6},	        //  ���ֿ��ڵ���ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE7",        totPutRepositoryPage7},	        //  ���ֿ��ڵ���ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE8",        totPutRepositoryPage8},	        //  ���ֿ��ڵڰ�ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE9",        totPutRepositoryPage9},	        //  ���ֿ��ڵھ�ҳ�����Ʒ
    {"PUT_REPOSITORY_PAGE10",       totPutRepositoryPage10},        //  ���ֿ��ڵ�ʮҳ�����Ʒ

    {"GET_FROM_REPOSITORY_PAGE2",   totGetFromRepositoryPage2},     //	�Ӳֿ��ڵڶ�ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE3",   totGetFromRepositoryPage3},     //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE4",   totGetFromRepositoryPage4},     //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE5",   totGetFromRepositoryPage5},     //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE6",   totGetFromRepositoryPage6},     //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE7",   totGetFromRepositoryPage7},     //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE8",   totGetFromRepositoryPage8},     //	�Ӳֿ��ڵڰ�ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE9",   totGetFromRepositoryPage9},     //	�Ӳֿ��ڵھ�ҳȡ����Ʒ
    {"GET_FROM_REPOSITORY_PAGE10",  totGetFromRepositoryPage10},    //	�Ӳֿ��ڵ�ʮҳȡ����Ʒ

    {"MODIFY_CALENDAR",             totModifyCalendar},             //	����/�༭/ɾ�������ص�������
    {"MANAGE_TASK",                 totManageTask},                 //	����/ȡ���������
    {"MANAGE_VOTE",                 totManageVote},                 //	����/ȡ��ͶƱ
    {"ADD_TO_GROUP",                totAddToGroup},                 //	������ĳһ�û���
    {"MOVE_FROM_GROUP",             totMoveFromGroup},              //	���˴�ĳһ�û������Ƴ�
    {"MODIFY_WAGE",                 totModifyWage},                 //	�༭���˹���
    {"PAY_SALARY",                  totPaySalary},                  //	������
    {"MODIFY_MEMBER_REMARK",        totModifyMemberRemark},         //	�༭���˵ı�ע
    {"MODIFY_GROUP_NAME",           totModifyGroupName},            //  �޸�����

    {NULL,                          0}
};

KLuaConst LUA_TONG_HISTORY_TYPE[]  =
{
    {"MEMBERMANAGE",    ohtMemberManage},
    {"FUND",            ohtFund},
    {"TECH_TREE",       ohtTechTree},
    {"ACTIVITY",        ohtActivity},
    {"REPERTORY",       ohtRepertory},

    {NULL, 0}
};

KLuaConst LUA_MENTOR_RECORD_STATE[] = 
{
    {"INVALID",              emrsInvalid},
    {"NORMAL",               emrsNormal},
    {"MENTOR_BREAK",         emrsMBreak},
    {"APPRENTICE_BREAK",     emrsABreak},
    {"BROKEN",               emrsBroken},
    {"GRADUATING",           emrsGraduating},
    {"GRADUATED",            emrsGraduated},
    
    {NULL, 0}
};

KLuaConst LUA_MAP_TYPE[] =
{
    {"NORMAL_MAP",          emtNormalMap},
    {"DUNGEON",             emtDungeon},
    {"BATTLE_FIELD",        emtBattleField},
    {"BIRTH_MAP",           emtBirthMap},

    {NULL,                  0}
};

KLuaConst LUA_CAMP_TYPE[] =
{
    {"NEUTRAL",             cNeutral},
    {"GOOD",                cGood},
    {"EVIL",                cEvil},

    {NULL, 0}    
};

KLuaConstList g_LuaCenterConstList[] =
{
    {"TONG_OPERATION_INDEX",    LUA_TONG_OPERATION_INDEX},
    {"TONG_HISTORY_TYPE",       LUA_TONG_HISTORY_TYPE},
    {"MENTOR_RECORD_STATE",     LUA_MENTOR_RECORD_STATE},
    {"MAP_TYPE",                LUA_MAP_TYPE},
    {"CAMP_TYPE",               LUA_CAMP_TYPE},

    {NULL,                      0}
};


