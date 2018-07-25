#ifndef _KTONGDB_H_
#define _KTONGDB_H_

// ������ݿ�������ݽṹ����

#define TONG_DB_VERSION     2
#define MAX_TONG_DATA_LEN   (1024 * 1024)

enum KTongDataSection
{
    eTongDataBegin,
    eTongBaseInfo,
    eTongSchema,
    eTongRoster,
    eTongAnnouncement,
    eTongOnlineMessage,
    eTongIntroduction,
    eTongRules,
    eTongMemorabilia,
    eTongRepertory,
    eTongDevelopmentInfo,
    eTongHistory,
    eTongDataEnd
};

#pragma pack(1)
struct KTONG_DB_HEADER
{
    int nVersion;
};

struct KTONG_DB_SECTION
{
    int     nType;
    size_t  uDataLen;
    BYTE    byData[0];
};

struct KTONG_DB_BASEINFO
{
    char    szName[_NAME_LEN];
    DWORD   dwMaster;           // ����
    int     nLevel;
    WORD    wMaxMemberCount;    // ��Ա����
    BYTE    byTongState;
    time_t  nStateTimer;
    BYTE    byCamp;             // ��Ӫ
    
};

// --- KTONG_DB_BASEINFO �汾���� ---
struct KTONG_DB_BASEINFO_VERSION1
{
    char    szName[_NAME_LEN];
    DWORD   dwMaster;           // ����
    int     nFund;              // �ʽ�(����)
    int     nLevel;
    int     nDevelopmentPoint;  // ��չ��
    WORD    wMaxMemberCount;    // ��Ա����
    BYTE    byTongState;
    time_t  nStateTimer;
    BYTE    byCamp;             // ��Ӫ
};
// ---------------------------------

struct KTONG_DB_SCHEMA
{
    KTongSchemaData Schema;
};

struct KTONG_DB_ROSTER
{
    int     nMemberCount;
    BYTE    byData[0];
};

struct KTONG_DB_MEMORABILIA
{
    time_t  nTime;
    BYTE    byDataLen;
    BYTE    byData[0];
};

struct KTONG_DB_MEMBER_DATA
{
    DWORD   dwID;
    int     nGroupID;                           // ������Ա��
	time_t  nJoinTime;                          // ����ʱ��
    char    szRemark[TONG_MEMBER_REMARK_LEN];   // ��ע
    time_t  nLastOfflineTime;                   // �������ʱ��
    int     nSalary;                            // ���ʴ���(����)
};

// --- KTONG_DB_MEMBER_DATA �汾���� ---
struct KTONG_DB_MEMBER_DATA_VERSION1
{
    DWORD   dwID;
    int     nGroupID;                           // ������Ա��
	time_t  nJoinTime;                          // ����ʱ��
    char    szRemark[TONG_MEMBER_REMARK_LEN];   // ��ע
    time_t  nLastOfflineTime;                   // �������ʱ��
};
// -------------------------------------

struct KTONG_DB_DEVELOPMENT_DATA
{
    int     nFund;              // �ʽ�(����)
    int     nMaxWageRate;
    int     nDevelopmentPoint;  // ��չ��
    int     nUsedDevelopmentPoint;
    BYTE    byTechTree[MAX_TONG_TECHTREE_NODE_NUM];
};

struct KTONG_REPERTORY_DB_PAGE
{
    BYTE byPageIndex;
    BYTE byCount;
};

struct KTONG_REPERTORY_DB_ITEM
{
    BYTE byPos;
    BYTE bySize;
    BYTE byData[0];
};

struct KTONG_DB_HISTORY_CATEGORY
{
    BYTE byType;
    WORD wRecordCount;
};
#pragma pack()

#endif

