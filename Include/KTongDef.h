#ifndef _KTONGDEF_H_
#define _KTONGDEF_H_

#define MAX_TONG_GROUP                          16                  // ����Ա������
#define MAX_TONG_MEMORABILIA_SIZE               10                  // �����¼���Ŀ����
#define TONG_INIT_MAX_MEMBER_COUNT              50                  // ����ʼ��������
#define TONG_OPERATION_NUM                      8 
#define MAX_TONG_TECHTREE_NODE_NUM              64                  // �Ƽ������������
#define MAX_TONG_MEMBER_COUNT                   255
#define MIN_TONG_MEMBER_COUNT                   10
#define TONG_TRIAL_TIME                         (3 * 24 * 60 * 60)  // ���ﱸ����      3 days
#define TONG_DISBAND_TIME                       (3 * 24 * 60 * 60)  // �����̭����      3 days
#define TONG_INVITE_TIME                        (2  * 60)           // ���������Чʱ��  2 min
#define INVALID_TONG_OPERATION                  -1
#define MAX_TONG_DEVELOPMENT_POINT              100000              //  ��ᷢչ��������
#define MAX_TONG_MEMORABILIA_COUNT              16                  //  �����¼��������
#define TONG_MEMORABILIA_RECORD_LEN             64                  //  �����¼��ַ�������
#define TONG_MEMBER_REMARK_LEN                  32
#define TONG_ANNOUNCEMENT_LEN                   128
#define TONG_ONLINE_MESSAGE_LEN                 128
#define TONG_INTRODUCTION_LEN                   128
#define TONG_RULES_LEN                          1024
#define TONG_MAX_WAGE_RATE_LIMIT                25                  // ��Ṥ�ʷ��ű���Ӳ����25%


#define TONG_REPERTORY_PAGE_NUM                 10                  //  ���ֿ�ҳ��
#define TONG_REPERTORY_PAGE_CAPACITY            98                  //  ���ֿⵥҳ��������
// #define MAX_TONG_REPERTORY_ITEM_DATA_SIZE       30
// #define TONG_REPERTORY_TOTAL_CAPACITY        (TONG_REPERTORY_PAGE_NUM * TONG_REPERTORY_PAGE_CAPACITY)

#define MAX_SYNC_TONG_REPERTORY_PAGE_PAK_SIZE   (1024 * 4)          // ���ֿ����ͬ��������

#define MAX_TONG_FUND_BY_SILVER                 INT_MAX             // ����ʽ�����(����Ϊ��λ)
#define MONEY_CONVERSION_SILVER_RATE            100                 // ��Ǯ�����Ӳ�ҵĽ���
#define TONG_EVENT_NOTIFY_FUNCTION              "OnSendTongEvent"   // �����Ϣ֪ͨ(Server��Զ�̵��ú�����)

// ��������ʷ��¼
#define MAX_MEMBER_MANAGE_RECORD_CONUT      120
#define MAX_FUND_RECORD_COUNT               100
#define MAX_TECH_TREE_RECORD_COUNT          80
#define MAX_MISSIOM_RECORD_COUNT            80
#define MAX_REPERTORY_RECORD_COUNT          120
#define TOTAL_RECORD_COUNT \
(MAX_MEMBER_MANAGE_RECORD_CONUT + MAX_FUND_RECORD_COUNT + MAX_TECH_TREE_RECORD_COUNT + MAX_MISSIOM_RECORD_COUNT + MAX_REPERTORY_RECORD_COUNT)

#define TONG_OPERATION_HISTORY_PAGE_SIZE    10

#pragma pack(1)
// �����֯�ṹ��������,���ڰ��༭�����̸�ʽ�Լ�������ݿ���̸�ʽ
struct KTongGroup
{
    char szName[_NAME_LEN];
    bool bEnable;
    int  nWage;                                                 // ���ʶ��(����)

    BYTE byBaseOperationMask;                                   // ��������Ȩ��
    BYTE byAdvanceOperationMask[MAX_TONG_GROUP];                // �߼�����,���뱻������������Group��صĲ���Ȩ��

    BYTE byBaseGrantMask[MAX_TONG_GROUP];                       // ������Ȩ
    BYTE byAdvanceGrantMask[MAX_TONG_GROUP][MAX_TONG_GROUP];    // �߼���Ȩ,���뱻������������Group��صĲ�����Ȩ
};

struct KTongSchemaData
{
    int        nMasterGroup;
    int        nDefaultGroup;
    KTongGroup Data[MAX_TONG_GROUP];
};

// ����Ա��Ϣ���紫��ṹ, �ͻ��˱�ʾ
struct KTongMemberInfo
{
    DWORD   dwID;
    int     nGroupID;                           // ������Ա��
	time_t  nJoinTime;                          // ����ʱ��
    char    szRemark[TONG_MEMBER_REMARK_LEN];   // ��ע
    time_t  nLastOfflineTime;                   // �������ʱ��
    char    szName[_NAME_LEN];
    BYTE    byLevel;
    BYTE    byForceID;
    DWORD   dwMapID;
    bool    bIsOnline;
};

// �����¼�
struct KTongIncidentRecordData
{
    time_t      nTime;
    char        szDescription[TONG_MEMORABILIA_RECORD_LEN];
};
#pragma pack()

// ����˰���Ա��Ϣ
struct KTongMember
{
    DWORD   dwID;
    int     nGroupID;                           // ������Ա��
	time_t  nJoinTime;                          // ����ʱ��
    char    szRemark[TONG_MEMBER_REMARK_LEN];   // ��ע
    time_t  nLastOfflineTime;                   // �������ʱ��
    int     nSalary;                            // ���ʴ���(����)
//---------- ���±��������Բ�ѯ���� ----------
    int     nLevel;
    DWORD   dwForceID;
    DWORD   dwMapID;
    BOOL    bIsOnline;
//--------------------------------------------
    int     nLastUpdateFrame;
};

struct KTongIncidentRecord
{
    DWORD   dwID;
    time_t  nTime;
    char    szDescription[TONG_MEMORABILIA_RECORD_LEN];
};

enum KTONG_OPERATION_TYPE
{
    totInvalid = 0,
    totSpeak,                   //	�ڰ��Ƶ������
    totJoinManagerChannel,      //	�����ԱƵ���������Բ鿴Ƶ������
    totSpeakInManagerChannel,   //	�ڹ�ԱƵ������
    totCheckOperationRecord,    //	�鿴��������¼
    totModifyAnnouncement,      //	�޸İ�ṫ��
    totModifyOnlineMessage,     //	�޸İ��������ʾ
    totModifyIntroduction,      //	�޸İ�����
    totModifyRules,             //	�޸İ����
    totModifyMemorabilia,       //	�޸İ����¼�
    totDevelopTechnology,       // 	Ͷ�ʿƼ���
    totProductItem,	            //  ��������

    totPutRepositoryPage2,      // 	���ֿ��ڵڶ�ҳ�����Ʒ
    totPutRepositoryPage3,	    //  ���ֿ��ڵ���ҳ�����Ʒ
    totPutRepositoryPage4,	    //  ���ֿ��ڵ���ҳ�����Ʒ
    totPutRepositoryPage5,	    //  ���ֿ��ڵ���ҳ�����Ʒ
    totPutRepositoryPage6,	    //  ���ֿ��ڵ���ҳ�����Ʒ
    totPutRepositoryPage7,	    //  ���ֿ��ڵ���ҳ�����Ʒ
    totPutRepositoryPage8,	    //  ���ֿ��ڵڰ�ҳ�����Ʒ
    totPutRepositoryPage9,	    //  ���ֿ��ڵھ�ҳ�����Ʒ
    totPutRepositoryPage10,     //  ���ֿ��ڵ�ʮҳ�����Ʒ

    totGetFromRepositoryPage2,  //	�Ӳֿ��ڵڶ�ҳȡ����Ʒ
    totGetFromRepositoryPage3,  //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    totGetFromRepositoryPage4,  //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    totGetFromRepositoryPage5,  //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    totGetFromRepositoryPage6,  //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    totGetFromRepositoryPage7,  //	�Ӳֿ��ڵ���ҳȡ����Ʒ
    totGetFromRepositoryPage8,  //	�Ӳֿ��ڵڰ�ҳȡ����Ʒ
    totGetFromRepositoryPage9,  //	�Ӳֿ��ڵھ�ҳȡ����Ʒ
    totGetFromRepositoryPage10, //	�Ӳֿ��ڵ�ʮҳȡ����Ʒ

    totModifyCalendar,          //	����/�༭/ɾ�������ص�������
    totManageTask,              //	����/ȡ���������
    totManageVote,              //	����/ȡ��ͶƱ
    totAddToGroup,              //	������ĳһ�û���
    totMoveFromGroup,           //	���˴�ĳһ�û������Ƴ�
    totModifyWage,              //	�༭���˹���
    totPaySalary,               //	������
    totModifyMemberRemark,      //	�༭���˵ı�ע
    totModifyGroupName,         //  �޸�����

    totTotal
};

// ���״̬
enum KTONG_STATE
{
    tsInvalid,
    tsTrialTong,            // ���ڳﱸ��
    tsNormalTong,           // ����״̬
    tsDisbandTong,          // ������̭
    tsTotal
};
// -----------------------    ��������Ϣͬ��    -------------------------
enum KTONG_SYNC_DATA_TYPE
{
    tdtBaseInfo,
    tdtDevelopmentInfo,
    tdtAnnouncement,
    tdtOnlineMessage,
    tdtIntroduction,
    tdtRules,
    tdtSchemaInfo,
    tdtMemorabilia,
    tdtTotal
};

enum KTONG_MODIFY_REQUEST_TYPE
{
    tmrBegin,
    tmrTongName,
    tmrAnnouncement,
    tmrOnlineMessage,
    tmrIntroduction,
    tmrRules,
    tmrMemorabilia,
    tmrEnd
};

enum KTONG_MODIFY_SCHEMA_TYPE
{
    tmsBegin,
    tmsGroupName,
    tmsBaseOperation,
    tmsAdvanceOperation,
    tmsGroupWage,
    tmsEnd
};

enum KTONG_UPDATE_FLAG
{
    tufInfo,
    tufRoster
};

enum KCHANGE_TONG_REASON
{
    ctrCreate,
    ctrDisband,
    ctrJoin,
    ctrQuit,
    ctrFired
};

#pragma pack(1)

struct KTONG_BASE_INFO
{
    DWORD       dwMasterID;
    BYTE        byLevel;
    int         nMaxMemberCount;
    BYTE        byState;
    time_t      nStateTimerOffset;
    char        szTongName[_NAME_LEN];
    BYTE        byCamp;
};

struct KTONG_DEVELOPMENT_INFO
{
    int     nFound;
    int     nMaxWageRate;
    int     nDevelopmentPoint;
    int     nUsedDevelopmentPoint;
    BYTE    m_byTechTree[MAX_TONG_TECHTREE_NODE_NUM];
};

struct KTONG_TEXT_INFO
{
    size_t uDataLen;
    BYTE   byContent[0];
};

struct KTONG_SCHEMA_DATA_INFO
{
    KTongSchemaData SchemaData;
};

struct KTONG_MEMORABILIA_INFO
{
    BYTE                byCount;
    KTongIncidentRecord Memorabilia[0];
};

struct KTONG_MODIFY_GROUP_NAME
{
    char szGroupName[_NAME_LEN];
};

struct KTONG_MODIFY_BASE_OPERATION_MASK
{
    BYTE byOffset;
    bool bValue;
};

struct KTONG_MODIFY_ADVANCE_OPERATION_MASK
{
    BYTE byIndex;
    BYTE byOffset;
    bool bValue;
};

enum KTONG_MODIFY_MEMORABILIA_TYPE
{
    mmtAdd,
    mmtModify,
    mmtDelete
};
// �޸İ����¼�
struct KTONG_MODIFY_MEMORABILIA_INFO
{
    BYTE byOperationType;
};

struct KTONG_ADD_MEMORABILIA : KTONG_MODIFY_MEMORABILIA_INFO
{
    time_t  nTime;
    char    szDescription[TONG_MEMORABILIA_RECORD_LEN];
};

struct KTONG_MODIFY_MEMORABILIA : KTONG_MODIFY_MEMORABILIA_INFO
{
    DWORD   dwID;
    time_t  nTime;
    char    szDescription[TONG_MEMORABILIA_RECORD_LEN];
};

struct KTONG_DELETE_MEMORABILIA : KTONG_MODIFY_MEMORABILIA_INFO
{
    DWORD dwID;
};

struct KTONG_ITEM_SYNC_DATA
{
    BYTE    byPos;
    BYTE    bySize;
    BYTE    byData[0];
};
#pragma pack()

// -----------------------    ���㲥    -------------------------

enum TONG_BROADCAST_MESSAGE_TYPE
{
    tbmInvalid,
    tbmTongStateChange,
    tbmGroupRightChange,
    tbmGroupNameChange,
    tbmGroupWageChange,
    tbmMemberJoin,
    tbmMemberQuit,
    tbmMemberFired,
    tbmMemberChangeGroup,
    tbmMasterChange,
    tbmCampChange,
    tbmMemberOnline,
    tbmMemberOffline,
    tbmMaxMemberCountChange,
    tbmGroupEnabled,
    tbmTotal
};

#pragma pack(1)

struct KTONG_CHANGE_GROUP_RIGHT_BROADCAST
{
    char szGroupName[_NAME_LEN];
};

struct KTONG_CHANGE_GROUP_NAME_BROADCAST
{
    char szOldGroupName[_NAME_LEN];
    char szNewGroupName[_NAME_LEN];
};

struct KTONG_CHANGE_GROUP_WAGE_BROADCAST
{
    char szGroupName[_NAME_LEN];
    int  nWage;
};

struct KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST
{
    char szMemberName[_NAME_LEN];
};

struct KTONG_MEMBER_CHANGE_GROUP_BROADCAST
{
    char szMemberName[_NAME_LEN];
    char szOldGroupName[_NAME_LEN];
    char szNewGroupName[_NAME_LEN];
};

struct KTONG_CHANGE_MASTER_BROADCAST
{
    char szOldMasterName[_NAME_LEN];
    char szNewMasterName[_NAME_LEN];
};

struct KTONG_MEMBER_ONLINE_STATE_CHANGE_BROADCAST
{
    char szMemberName[_NAME_LEN];
};

struct KTONG_GROUP_ENABLED_BROADCAST
{
    char szGroupName[_NAME_LEN];
};

enum INVITE_TONG_RESULT_CODE
{
    itrInvalid,

    itrSuccess,
    itrRefuse,
    itrChargeLimit,

    itrTotal
};

#pragma pack()

// ---------------------    �����ʷ��¼    ---------------------
enum KTONG_OPERATION_HISTORY_TYPE
{
    ohtMemberManage = 0,
    ohtFund,
    ohtTechTree,
    ohtActivity,
    ohtRepertory,
    ohtTotal
};

enum KTONG_HISTORY_RECORD_TYPE
{
    hrtInvalied,

    hrtMemberManageBegin,

    hrtChangeGroupBaseRight,
    hrtChangeGroupAdvanceRight,
    hrtChangeMemberGroup,
    hrtJoin,
    hrtQuit,
    hrtKickOut,

    hrtMemberManageEnd,
// ------------------------
    hrtFundBegin,

    hrtChangeGroupWage,
    hrtPaySalary,
    hrtSaveFund,

    hrtFundEnd,
// ------------------------
    hrtTechTreeBegin,

    hrtActivateTechtree,
    hrtProductItem,

    hrtTechTreeEnd,
// ------------------------
    hrtActivityBegin,

    hrtMissionStart,
    hrtMissionComplete,
    hrtMissionFailed,
    hrtWarStart,
    hrtWarWin,
    hrtWarLose,
    hrtAchievement,

    hrtActivityEnd,
// ------------------------
    hrtRepertoryBegin,

    hrtRepertoryTakeItem,
    hrtRepertoryPutItem,

    hrtRepertoryEnd,

    hrtTotal
};

#pragma pack(1)
struct KTONG_SYNC_HISTORY
{
    BYTE    byRecordType;
    time_t  nTime;
};

// ------------ ���±䶯 ----------------

struct KTONG_SYNC_HISTORY_CHANGE_GROUP_BASE_RIGHT : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byTargetGroup;
    BYTE    byRightOffset;
    BYTE    byIsOpen;
};

struct KTONG_SYNC_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    bySrcGroup;
    BYTE    byDstGroup;
    BYTE    byRightOffset;
    BYTE    bIsOpen;
};

struct KTONG_SYNC_HISTORY_CHANGE_MEMBER_GROUP : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    char    szTargetMember[_NAME_LEN];
    BYTE    bySrcGroup;
    BYTE    byDstGroup;
};

struct KTONG_SYNC_HISTORY_JOIN_OR_QUIT : KTONG_SYNC_HISTORY
{
    char szMember[_NAME_LEN];
};

struct KTONG_SYNC_HISTORY_KICK_OUT_MEMBER : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    char  szTargetMember[_NAME_LEN];
};

// ------------ �ʽ�䶯 ----------------
struct KTONG_SYNC_HISTORY_CHANGE_GROUP_WAGE : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byTargetGroup;
    DWORD   dwOldWage;
    DWORD   dwNewWage;
};

struct KTONG_SYNC_HISTORY_PAY_SALARY : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    DWORD dwTotalSalary;
};

struct KTONG_SYNC_HISTORY_SAVE_FUND : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    DWORD dwSaveSilver;
};

// ------------ �Ƽ����͵��߳��� ------------

struct KTONG_SYNC_HISTORY_ACTIVATE_TECHTREE : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byNode;
    BYTE    byLevel;
    DWORD   dwConsumeDevelopmentPoint;
    DWORD   dwConsumeFund;
};

struct KTONG_SYNC_HISTORY_PRODUCT_ITEM : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    DWORD dwRecipeID;
    DWORD dwCount;
    DWORD dwConsumeDevelopmentPoint;
    DWORD dwConsumeFund;
};

// ------------ ��� ----------------

struct KTONG_SYNC_HISTORY_MISSION_START : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    DWORD   dwMission;
    time_t  nMissionStartTime;
};

struct KTONG_SYNC_HISTORY_MISSION_END : KTONG_SYNC_HISTORY
{
    DWORD   dwMission;
};

struct KTONG_SYNC_HISTORY_WAR_START : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    char    szEnemyTong[_NAME_LEN];
};

struct KTONG_SYNC_HISTORY_WAR_END : KTONG_SYNC_HISTORY
{
    char   szEnemyTong[_NAME_LEN];
};

struct KTONG_SYNC_HISTORY_ACHIEVEMENT : KTONG_SYNC_HISTORY
{
    DWORD   dwAchievement;
};

// ------------ ���ֿ� ----------------

struct KTONG_SYNC_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byTabType;
    WORD    wTabIndex;
    WORD    wStackNum;
};

#pragma pack()

#endif
