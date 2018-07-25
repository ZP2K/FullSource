///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName :   KTongData.h
// Creator  :   liuzhibiao
// Date     :   2009-7-27
// Comment  :   v1.0        �����ݿ��в�ѯ�����Ϣ
///////////////////////////////////////////////////////////////

#ifndef KTONGDATA_H
#define KTONGDATA_H

// linux����ʱ��Ҫ
#define KGLOG_ERR 3
#define KGLOG_PROCESS_ERROR(Condition) \
    do  \
    {   \
        if (!(Condition))   \
            goto Exit0;     \
    } while (false)
// linux����ʱ��Ҫend

#include "ProtocolBasic.h"
#include "SO3GlobalDef.h"
#include "KGPublic.h"
#include "KTongDef.h"
#include "KTongDB.h"
#include "Engine/EngineBase.h"
#include "Engine/FileType.h"
#include "KGPublic.h"
#include "KGD_Interface.h"
#include "KRoleDBDataDef.h"
#include "Luna.h"
#include "KItem.h"
#include "KTongRepertory.h"
#include <vector>

using namespace std;

#define MAX_TONG_TECHTREE_NODE_NUM_AnanlyserCompatible             64                  // �Ƽ������������
#define TONG_MEMBER_REMARK_LEN_AnanlyserCompatible                 32

#pragma pack(1)

struct KTONG_DB_DEVELOPMENT_DATA_NOW_USE
{
    int             nFund;              // �ʽ�(����)
    int             nMaxWageRate;
    int             nDevelopmentPoint;  // ��չ��
    int             nUsedDevelopmentPoint;
    unsigned char   ucTechTree[MAX_TONG_TECHTREE_NODE_NUM_AnanlyserCompatible];
};

struct KTONG_DB_MEMBER_DATA_NOW_USE
{
    DWORD   dwID;
    int     nGroupID;                                               // ������Ա��
    time_t  nJoinTime;                                              // ����ʱ��
    char    szRemark[TONG_MEMBER_REMARK_LEN_AnanlyserCompatible];   // ��ע
    time_t  nLastOfflineTime;                                       // �������ʱ��
    int     nSalary;                                                // ���ʴ���(����)
};

struct KTONG_DB_MEMBER_DATA_VERSION1_AnanlyserCompatible
{
    DWORD   dwID;
    int     nGroupID;                                               // ������Ա��
    time_t  nJoinTime;                                              // ����ʱ��
    char    szRemark[TONG_MEMBER_REMARK_LEN_AnanlyserCompatible];   // ��ע
    time_t  nLastOfflineTime;                                       // �������ʱ��
};

struct KTONG_DB_BASEINFO_VERSION1_ANALYSER
{
    char            szName[_NAME_LEN];
    unsigned long   dwMaster;           // ����
    int             nFund;              // �ʽ�
    int             nLevel;
    int             nDevelopmentPoint;  // ��չ��
    unsigned short  wMaxMemberCount;    // ��Ա����
    unsigned char   byTongState;
    time_t          nStateTimer;
    unsigned char   byCamp;             // ��Ӫ
};

struct KTONG_DB_BASEINFO_ANALYSER
{
    char    szName[_NAME_LEN];
    DWORD   dwMaster;           // ����
    int     nLevel;
    WORD    wMaxMemberCount;    // ��Ա����
    BYTE    byTongState;
    time_t  nStateTimer;
    BYTE    byCamp;             // ��Ӫ

};

struct KJX3TONG_DB_SECTION
{
    int             nType;
    size_t          uDataLen;
    unsigned char   ucData[0];
};

struct KJX3TONG_DB_SCHEMA
{
    KTongSchemaData Schema;
};

struct KJX3TONG_DB_ROSTER
{
    int              nMemberCount;
    unsigned char    byData[0];
};

struct TONG_REPERTORY_ITEM_ANALYSER
{
    bool          bIsExist;
    unsigned char ucPos;
    unsigned char ucSize;
    int           nIsEqui;
    union
    {
        KCOMMON_ITEM_DATA commonEquipData;
        KCUSTOM_EQUI_DATA customEquipData;
    };
};

struct TONG_REPERTORY_PAGE_ANALYSER
{
    bool          bIsEnable;
    unsigned char ucPageIndex;
    unsigned char ucCount;
    TONG_REPERTORY_ITEM_ANALYSER trItem[TONG_REPERTORY_PAGE_CAPACITY];
};
#pragma pack()

#define TONG_MEMBER_REMARK_LEN_NOW_USE 32

struct KTongMember_NowUse
{
    DWORD   dwID;
    int     nGroupID;                                   // ������Ա��
    time_t  nJoinTime;                                  // ����ʱ��
    char    szRemark[TONG_MEMBER_REMARK_LEN_NOW_USE];   // ��ע
    time_t  nLastOfflineTime;                           // �������ʱ��
    int     nSalary;                                    // ���ʴ���(����)
    //---------- ���±��������Բ�ѯ���� ----------
    int     nLevel;
    DWORD   dwForceID;
    DWORD   dwMapID;
    BOOL    bIsOnline;
    //--------------------------------------------
    int     nLastUpdateFrame;
};

class KTongData
{
public:
    KTongData(void);
    ~KTongData(void);
public:
    int PaserTongDataInfo(unsigned char* pbyData, size_t uDataLen);

private:
    int LoadBaseInfoVersion1(unsigned char* pbyData, size_t uDataLen);
    int LoadBaseInfo(unsigned char* pbyData, size_t uDataLen);
    int LoadSchema(unsigned char* pbyData, size_t uDataLen);
    int LoadRoster(unsigned char* pbyData, size_t uDataLen);
    int LoadRosterVersion1(unsigned char* pbyData, size_t uDataLen);
    int LoadMemorabilia(unsigned char* pbyData, size_t uDataLen);
    int LoadRepertory(unsigned char* pbyData, size_t uDataLen);
    int LoadDevelopmentInfo(unsigned char* pbyData, size_t uDataLen);

public:
    KTONG_DB_BASEINFO_VERSION1_ANALYSER     m_TongBaseInfo;
    KTongSchemaData                         m_SchemaData;
    char                                    m_szAnnouncement[128];              // ����
    char                                    m_szOnlineMessage[128];             // ������ʾ
    char                                    m_szIntroduction[128];              // ����
    char                                    m_szRules[1024];                    // ���
    vector<KTongMember_NowUse*>             m_Memberlist;                       // ��Ա�б�
    int                                     m_nMaxWageRate;                     // ���ʷ��ű�������
    int                                     m_nUsedDevelopmentPoint;            // Ͷ�뷢չ��
    unsigned char                           m_byTechTree[MAX_TONG_TECHTREE_NODE_NUM];

    typedef std::vector<KTongIncidentRecord> KINCIDENT_TABLE;

    KINCIDENT_TABLE                         m_Memorabilia;                      // �����¼�

    TONG_REPERTORY_PAGE_ANALYSER    m_TongRepertoryPage[TONG_REPERTORY_PAGE_NUM];   // analyser���ֿ�
};
#endif //KTONGDATA_H