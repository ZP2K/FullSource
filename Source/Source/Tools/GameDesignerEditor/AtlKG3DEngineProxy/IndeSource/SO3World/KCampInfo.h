#ifndef _KCAMPINFO_H_
#define _KCAMPINFO_H_

#include "SO3GlobalDef.h"
#include "Luna.h"

class KCampInfo
{
public:
    KCampInfo();
    ~KCampInfo(){};

    BOOL Init();
    void UnInit(){};
    
    void SetCampScore(int nCampScore){m_nCampScore = nCampScore;}
    int  GetCampScore(){return m_nCampScore;};

    void SetCampLevel(int nCampLevel){m_nCampLevel = nCampLevel;}
    int  GetCampLevel(){return m_nCampLevel;};
    
    void SetNextResetTime(time_t nNextResetTime){m_nNextResetTime = nNextResetTime;}
    time_t GetNextResetTime(){return m_nNextResetTime;}

    int GetMoneyPercent(KCAMP eCamp, int nLevel);
    int GetReputePercent(KCAMP eCamp, int nLevel);
    int GetPrestigePercent(KCAMP eCamp, int nLevel);
    int GetReducePrestigeOnDeath(KCAMP eCamp, int nLevel);
    int GetDamagePercent(KCAMP eCamp, int nLevel);
    int GetShieldPercent(KCAMP eCamp, int nLevel);
    int GetVersion(){return m_nVersion;}
    void SetVersion(int nNewVersion){m_nVersion = nNewVersion;}

private:
    BOOL LoadCampLevelParam();

private:
    int m_nCampScore;
    int m_nCampLevel;  // ��Χ��0��CAMP_LEVEL_COUNT-1
    time_t m_nNextResetTime;
    int m_nVersion;
    
    int m_nLevelScore[CAMP_LEVEL_COUNT];

    struct KCampLevelParam
    {
        int nMoneyPercent;          // ��Ǯ����ٷֱ�
        int nReputePercent;         // �����ٷֱ�
        int nPrestigePercent;       // �����ٷֱ�
        int nReducePrestigeOnDeath; // ����������
        int nDamagePercent;         // �˺��仯
        int nShieldPercent;         // �����仯
    };

    KCampLevelParam m_GoodCampLevelParam[CAMP_LEVEL_COUNT];
    KCampLevelParam m_EvilCampLevelParam[CAMP_LEVEL_COUNT];

public:
    int LuaGetLevelScore(Lua_State* L);

    int LuaGetMoneyPercent(Lua_State* L);
    int LuaGetReputePercent(Lua_State* L);
    int LuaGetPrestigePercent(Lua_State* L);
    int LuaGetReducePrestigeOnDeath(Lua_State* L);
    int LuaGetDamagePercent(Lua_State* L);
    int LuaGetShieldPercent(Lua_State* L);

    int LuaGetNextResetTime(Lua_State* L);

public:
    DECLARE_LUA_CLASS(KCampInfo)
    DECLARE_LUA_INTEGER(CampScore);
    DECLARE_LUA_INTEGER(CampLevel);
};

#endif
