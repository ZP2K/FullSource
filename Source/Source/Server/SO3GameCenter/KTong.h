#ifndef _KTONG_H_
#define _KTONG_H_

#include <map>
#include <list>
#include "ProtocolBasic.h"
#include "KTongDef.h"
#include "KTongDB.h"
#include "Luna.h"
#include "KTongRepertory.h"
#include "KTongHistory.h"

class KRole;

class KTong
{
public:
    KTong();

    BOOL Init();
    void UnInit();

    void Activate();

    BOOL Load(DWORD dwTongID, BYTE* pbyData, size_t uDataLen);
    BOOL Save(size_t* puUseSize, BYTE* pbyBuffer, size_t uBufferSize);

    KTongMember* GetMember(DWORD dwID)
    {
        KROSTER::iterator Iter = m_Roster.find(dwID);
        
        if (Iter == m_Roster.end())
            return NULL;

        return &Iter->second;
    }

    BOOL AddNewMember(KRole* pRole, int nGroupID);
    BOOL RemoveMember(KRole* pRole);

    void Disband();

    // nGroup �ܲ����� nOperation �Ĳ���
    BOOL CanBaseOperate(int nGroup, int nOperation);

    // nGroup �ܲ��ܶ� nTargetGroup �� nOperation �Ĳ���
    BOOL CanAdvanceOperate(int nGroup, int nTargetGroup, int nOperation);

    // nGroup �Ƿ���Ծ� [nTargetGroup��nOperation����] ������Ȩ
    BOOL CanBaseGrant(int nGroup, int nTargetGroup, int nMaskOffset);

    // nGroup �Ƿ���Ծ� [nTargetGroupA��nTargetGroupB��nOperation����] ������Ȩ
    BOOL CanAdvanceGrant(int nGroup, int nTargetGroupA, int nTargetGroupB, int nMaskOffset);

    BOOL CanSpeak(DWORD dwMemberID);

    BOOL ChangeCamp(KCAMP eCamp);

    BOOL CheckCampCompatible(KCAMP eCamp)
    {
        return eCamp == m_eCamp || eCamp == cNeutral;
    }

    BOOL PaySalary(DWORD dwMemberID, int nTotalSalary);
    BOOL AddFund(int nSilver);

    template <class TFunc>
    void TraverseMember(TFunc& Func)
    {
        for (KROSTER::iterator i = m_Roster.begin(); i != m_Roster.end(); ++i)
        {
            Func(&(i->second));
        }
    }

public:
    KTongSchemaData m_Schema;

    DWORD           m_dwID;
    char            m_szName[_NAME_LEN];                        // �������
    DWORD           m_dwMaster;                                 // ����
    int             m_nFund;                                    // �ʽ�(����)
    int             m_nMaxWageRate;                             // ���ʷ��ű�������

    int             m_nLevel;
    int             m_nDevelopmentPoint;                        // ��չ��
    int             m_nMaxMemberCount;                          // ��Ա����

    char            m_szAnnouncement[TONG_ANNOUNCEMENT_LEN];    // ����
    char            m_szOnlineMessage[TONG_ONLINE_MESSAGE_LEN]; // ������ʾ
    char            m_szIntroduction[TONG_INTRODUCTION_LEN];    // ����
    char            m_szRules[TONG_RULES_LEN];                  // ���
    KCAMP           m_eCamp;                                    // �����Ӫ

    typedef KMemory::KAllocator<std::pair<DWORD, KTongMember> >KROSTER_ALLOCTOR;
    typedef std::map<DWORD, KTongMember, less<DWORD>, KROSTER_ALLOCTOR> KROSTER;
    KROSTER         m_Roster;                           // ��Ա������

    typedef KMemory::KAllocator<KTongIncidentRecord>KINCIDENT_ALLOCTOR;
    typedef std::vector<KTongIncidentRecord, KINCIDENT_ALLOCTOR> KINCIDENT_TABLE;
    KINCIDENT_TABLE m_Memorabilia;                      // �����¼�

    int             m_nUsedDevelopmentPoint;            // Ͷ�뷢չ��
    BYTE            m_byTechTree[MAX_TONG_TECHTREE_NODE_NUM];

    KTongRepertory  m_Repertory;                // �ֿ�
    KTongHistory    m_History;                  // ��ʷ��¼

public:
    BOOL            m_bUpdateFlag;                      // ���ݱ䶯��ǣ� ���ڶ��ڱ������ݿ��ʶ
    KTONG_STATE     m_eState;
    time_t          m_nStateTimer;
    int             m_nLastUpdateFrameTable[tdtTotal];
    DWORD           m_dwMemorabiliaCount;

private:
    BOOL LoadBaseInfo(BYTE* pbyData, size_t uDataLen);
    BOOL LoadBaseInfoVersion1(BYTE* pbyData, size_t uDataLen);
    BOOL SaveBaseInfo(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize);

    BOOL LoadSchema(BYTE* pbyData, size_t uDataLen);
    BOOL SaveSchema(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize);

    BOOL LoadRoster(BYTE* pbyData, size_t uDataLen);
    BOOL LoadRosterVersion1(BYTE* pbyData, size_t uDataLen); // �汾����

    BOOL SaveRoster(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize);

    BOOL LoadMemorabilia(BYTE* pbyData, size_t uDataLen);
    BOOL SaveMemorabilia(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize);

    BOOL LoadDevelopmentInfo(BYTE* pbyData, size_t uDataLen);
    BOOL SaveDevelopmentInfo(BYTE* pbyBuffer, size_t uBufferSize, size_t* puUsedSize);

private:
    int LuaSetLevel(Lua_State* L);
    int LuaAddFund(Lua_State* L);
    int LuaAddDevelopmentPoint(Lua_State* L);
    int LuaConsumeDevelopmentPoint(Lua_State* L);
    int LuaGetTechNodeLevel(Lua_State* L);
    int LuaSetTechNodeLevel(Lua_State* L);
    int LuaIsGroupEnabled(Lua_State* L);
    int LuaEnableGroup(Lua_State* L);
    int LuaSetMaxMemberCount(Lua_State* L);
    int LuaSetCamp(Lua_State* L);
    int LuaSetMaxWageRate(Lua_State* L);

    int LuaGetGroupID(Lua_State* L);
    int LuaGetMemberCount(Lua_State* L);
    int LuaCanBaseOperate(Lua_State* L);
    int LuaCanAdvanceOperate(Lua_State* L);

    int LuaGetGroupWage(Lua_State* L);
    int LuaActiveRepertoryPage(Lua_State* L);
    int LuaGetMemberSalaryAmount(Lua_State* L);

    int LuaGetHistoryRange(Lua_State* L);
    int LuaSyncTongHistoryRecord(Lua_State* L);
public:
    DECLARE_LUA_CLASS(KTong);
    
    DECLARE_LUA_DWORD(ID);
    DECLARE_LUA_STRING(Name, sizeof(m_szName));
    DECLARE_LUA_DWORD(Master);
    DECLARE_LUA_ENUM(Camp);
    DECLARE_LUA_INTEGER(Fund);
    DECLARE_LUA_INTEGER(Level);
    DECLARE_LUA_INTEGER(DevelopmentPoint);
    DECLARE_LUA_INTEGER(UsedDevelopmentPoint);
    DECLARE_LUA_INTEGER(MaxMemberCount);
    DECLARE_LUA_INTEGER(MaxWageRate);
};

#endif // _KTONG_H_
