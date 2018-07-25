/************************************************************************/
/* ���ܹ�����															*/
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.10.18	Create												*/
/* Comment	 :															*/
/*		�����ܵ�ȫ�����ݣ��洢�����м������еȼ��Ĺ�������			*/
/*		�������ݷ�������δ洢��										*/
/*			1 ����ĳ�����ܣ��ͼ��ܵȼ��޹ص�����						*/
/*			2 �ͼ��ܵȼ���أ����Ǻ;�������޹ص�����					*/
/*			3 �;��������صļ�������									*/
/*		���У�1��Ӧ��KSkillInfo��2��Ӧ��KSkill��1��2�����ڼ��ܹ���������*/
/*		��3��Ӧ��KPlayerSkill�����������Ч�����б����(KSkillList)		*/
/*		�ͻ��˺ͷ���˵ļ�������ά����ʽ��ͬ							*/
/*		��������ڴ�����������������ݣ���100M����						*/
/*		�ͻ���ֻ����ȫ����1�����ݣ�2��������Cache����ռ��2M���µ��ڴ�	*/
/************************************************************************/
#ifndef _KSKILL_MANAGER_H_
#define _KSKILL_MANAGER_H_

#include <map>
#include "KLRUCacheMap.h"
#include "../../Source/Common/SO3World/Src/KSkill.h"

class   KCharacter;

#define MAX_ROW                     203
#define MAX_COL                     5

#define MAX_SKILLS_FOR_EACH_MASTER  256

// Ϊ�˲߻����㣬��Tab�ļ�������Щ��ֵ�߻�������ַ�����������Ҫ����������Ӧ����ֵ
// ���磬���ܵ��˺����͵ȵȣ��ýṹ�Ǵӱ��ж�ȡ�ģ�δ��ת������ֵ.
struct SKILL_TABLE_LINE 
{
    char  szSkillName[_NAME_LEN];
    DWORD dwSkillID;
    int   nSkillMark;
    DWORD dwMaxLevel;
    char  szKindType[_NAME_LEN];
    int   nRecipeType;
    int   nUIType;
    DWORD dwBelongKungfu;               // �����书
    DWORD dwBelongSchool;               // ��������
    char  szEffectType[_NAME_LEN];                  // ����Ч������
    char  szCastMode[_NAME_LEN];
    DWORD dwWeaponRequest;
    BOOL  bCostAmmo;                    // �Ƿ����ĵ�ҩ������ĵ�ҩ
    char  szCostItemType[_NAME_LEN];    // ��������ָ����Ʒ���ڵ����ñ�
    int   nCostItemIndex;               // ��������ָ����Ʒ�������ñ��������ֵ(Index)  
    DWORD dwMountRequestType;
    DWORD dwMountRequestDetail;
    DWORD dwMountRequestDetailLevel;
    DWORD dwMountType;
    BOOL  bIsMountable;
    BOOL  bIsPassiveSkill;
    BOOL  bIsChannelSkill;
    BOOL  bIsExpSkill;
    BOOL  bIsExactHit;                  // �Ƿ����(��������������)
	BOOL  bIsInstinct;                  // �Ƿ���(������ѧ�����õļ���)
    BOOL  bIsAutoTurn;                  // �Ƿ���Ҫ�Զ�����
    BOOL  bCheckBindBuff;               // �Ƿ��ڼ��ܼ���ʱ����BindBuff
    BOOL  bCauseAbradeEquipment;        // �������к�,�Ƿ�Լ����ͷ��ߵ������ͼ���Ŀ��װ�����ĥ��
    BOOL  bCauseBeatBreak;              // �����Ƿ���԰ѱ��˵�ͨ�������
    BOOL  bCauseBeatBack;               // �����Ƿ���԰ѱ��˵�����/ͨ������
    BOOL  bHasCriticalStrike;           // �����Ƿ����漴�ػ�
    DWORD dwSkillEventMask1;
    DWORD dwSkillEventMask2;
    DWORD dwSkillCastFlag;
    DWORD dwCastMask;
    BOOL  bUseCastScript;
    BOOL  bUse3DObstacle;
    BOOL  bIgnorePositiveShield;        // ��������Ч������״̬
    BOOL  bIgnoreNegativeShield;        // ���Ը���Ч������״̬
    
    BOOL  bNeedOutOfFight;
    int   nSelfMoveStateMask;
    BOOL  bSelfOnFear;

    BOOL  bBindCombatTag;               // �Ƿ��ðѪ�Ķ�����ǩ�����߼��޹ء����ڱ���
	
    char  szSelfHorseStateRequest[_NAME_LEN];

    BOOL  bTargetTypePlayer;
    BOOL  bTargetTypeNpc;
    BOOL  bTargetRelationNone;
    BOOL  bTargetRelationSelf;
    BOOL  bTargetRelationAlly;
    BOOL  bTargetRelationEnemy;
    BOOL  bTargetRelationNeutrality;
    BOOL  bTargetRelationParty;

    int   nTargetMoveStateMask;
    BOOL  bTargetOnFear;

    char  szTargetHorseStateRequest[_NAME_LEN];

    char  szCommonSkillActiveMode[_NAME_LEN];
    BOOL  bAutoSelectTarget;
    int   nNPCWeaponType;
    DWORD dwMapBanMask;
    char  szScriptFile[MAX_PATH];
};


struct KKUNG_FU_EXP
{
    int nExpAdd;
    int nCostQiPoint;
};

struct KSKILL_MASTER_KEY 
{
    DWORD   dwMasterID;
    DWORD   dwSkillID;
    DWORD   dwSkillLevel;
};

struct KSKILL_ID_LEVEL 
{
    DWORD   dwSkillID;
    DWORD   dwSkillLevel;
};

inline bool operator < (const KSKILL_MASTER_KEY& crLv, const KSKILL_MASTER_KEY& crRv)
{
    if (crLv.dwMasterID != crRv.dwMasterID)
        return crLv.dwMasterID < crRv.dwMasterID;

    if (crLv.dwSkillID != crRv.dwSkillID)
        return crLv.dwSkillID < crRv.dwSkillID;

    return crLv.dwSkillLevel < crRv.dwSkillLevel;
}

struct KSKILL_LEARNING_INFO 
{
    DWORD   dwRequireExp;
    int     nRequirePlayerLevel;
    DWORD   dwReputationID;
    int     nReputationLevel;
    int     nPrice;
    int     nCostTrain;
};



class KSkillManager
{
public:
	KSkillManager(void);

	BOOL    Init(void);
	void    UnInit(void);

    KSkill* GetSkill_RAW(DWORD dwSkillID, DWORD dwSkillLevel);      // �����Ǽ��ܻ����书���ɻ��
    KSkill* GetSkill_RAW_S(DWORD dwSkillID, DWORD dwSkillLevel);    // ֻ�ܻ�ü���
    KSkill* GetSkill_K(DWORD dwSkillID, DWORD dwSkillLevel);        // ֻ�ܻ���书

    BOOL    GetSkillRecipeKey(KSKILL_RECIPE_KEY* pSkillRecipeKey, DWORD dwSkillID, DWORD dwSkillLevel, KCharacter* pCharacter);
    KSkill* GetSkill(const KSKILL_RECIPE_KEY& crSkillRecipeKey);    // ֻ�ܻ�ü���

    KSKILL_RECIPE_BASE_INFO* GetSkillRecipeBaseInfo(DWORD dwRecipeKey);

    KSKILL_EVENT*       GetEventInfo(int nID);

    int     GetDefenceCoefficient(int nRow, int nProbability);

    BOOL    GetMasterSkillList(DWORD dwMasterID, size_t uMaxArrayCount, size_t* puRetArrayCount, KSKILL_ID_LEVEL* pRetSkillListArray);
    KSKILL_LEARNING_INFO*   GetSkillLearningInfo(DWORD dwMasterID, DWORD dwSkillID, DWORD dwSkillLevel);

    BOOL    IsInstinct(DWORD dwSkillID, DWORD dwSkillLevel);
    BOOL    IsAutoTurn(DWORD dwSkillID, DWORD dwSkillLevel);
    BOOL    IsAccumulate(DWORD dwSkillID, DWORD dwSkillLevel);

    BOOL    IsCommonSkill(DWORD dwSkillID);                            // ����Ƿ�Ϊ���ּ���
    BOOL    WeaponMapCommonSkill(int nWeaponType, DWORD& rdwSkillID);  // ֻ�����ּ�����Ч
    int     CommonSkillMapWeapon(DWORD dwSkillID);                     // ֻ�����ּ�����Ч
    BOOL    WeaponMapBaseKungfu(int nWeaponType, DWORD& rdwKungfuID);

    BOOL    GetKungFuExp(DWORD dwKungFuID, DWORD dwKungFuLevel, KKUNG_FU_EXP** ppKungFuExp);

    #ifdef _SERVER
    BOOL    OpenVenation(KPlayer* pPlayer, int nVenationID);
    #endif

public:
#ifdef _SERVER
    DWORD m_dwBulletIDIndex;                                    // �ӵ���ID��1��ʼ, 0������ЧID
#endif

    DWORD m_dwSkillSNGenerator;

    typedef std::map<DWORD, KSKILL_RECIPE_BASE_INFO>        KSKILL_RECIPE_BASE_INFO_MAP;
    KSKILL_RECIPE_BASE_INFO_MAP     m_SkillRecipeBaseInfoMap;

private:

    BOOL LoadSkillTableLine(ITabFile* pSkillFile, int nRow, SKILL_TABLE_LINE *pTableLineInfo, const SKILL_TABLE_LINE &crDefaultValue);


    BOOL TableStringToNumber(const char cszString[], int *pnRetCode);  
    BOOL TranslateTableInfo(KSKILL_BASE_INFO *pSkillBaseInfo,  const SKILL_TABLE_LINE &crTableLineInfo);

    BOOL LoadSkillDataFull(DWORD dwSkillID);

    void ClearnUp();

private:
    // - Skill ---------------------------------------------------------------------------------------------

    typedef std::map<DWORD, KSKILL_BASE_INFO>	            SKILL_BASE_INFO_LIST;
    SKILL_BASE_INFO_LIST            m_SkillBaseInfoList;

	typedef std::map<DWORD, KSkill*>	                    SKILL_LIST;
	SKILL_LIST	                    m_SkillList;

    typedef KLRUCahceMap<KSKILL_RECIPE_KEY, KSkill, KDefaultUnInitEraseFunc<KSKILL_RECIPE_KEY, KSkill> > KSKILL_RECIPE_CACHE;
    KSKILL_RECIPE_CACHE             m_SkillRecipeCache;

    // - Skill Event ---------------------------------------------------------------------------------------

    typedef std::map<int, KSKILL_EVENT> KSKILL_EVENT_MAP;
    KSKILL_EVENT_MAP                m_SkillEventList;

    typedef std::map<KSKILL_MASTER_KEY, KSKILL_LEARNING_INFO> KSKILL_LEARNING_INFO_MAP;
    KSKILL_LEARNING_INFO_MAP        m_SkillLearningInfoMap;

    struct KSKILL 
    {
        DWORD dwCommonSkillID;
        DWORD dwBaseKungfuID;
    };

    typedef std::map<int, KSKILL> KWEAPON_MAP_SKILL;
    KWEAPON_MAP_SKILL m_WeaponMapSkill;

    typedef std::map<uint64_t, KKUNG_FU_EXP> KKUNG_FU_EXP_LIST;
    KKUNG_FU_EXP_LIST m_KungFuExpList;

    int m_nDefenceCoefficient[MAX_ROW][MAX_COL];

    BOOL LoadAttribute(ITabFile *piTabFile, int nRow, int nIndex, int &rnKey, int &rnValueA, int &rnValueB);

    BOOL LoadSkillEvent();
    BOOL LoadEventLine(ITabFile* piTabFile, int nLine);
    
    BOOL LoadWeaponMapSkill();
    BOOL LoadWeaponMapSkillLine(ITabFile* piTabFile, int nLine);    

    BOOL LoadDefenceCoefficientTable(const char* szTabFile);

    BOOL LoadKungFuExpTable(const char* szTabFile);
    BOOL LoadSkillRealizationTable(const char* szTabFile);
    BOOL LoadSubSkillLearningTable(DWORD dwMasterID, const char* szTabFile);
    BOOL LoadSkillLearningTable(const char* szTabFile);

    BOOL LoadSkillRecipeBaseLine(ITabFile* piTabFile, int nIndex, DWORD* pdwRecipeKey, KSKILL_RECIPE_BASE_INFO* pValue, const KSKILL_RECIPE_BASE_INFO& crDefault);
    BOOL LoadSkillRecipeBaseInfo(const char* szTabFile);

    // --------------------------- ������ -------------------------
    BOOL CheckError();
    BOOL CheckSkillParam(KSkill* pSkill);
    BOOL CheckKungfuParam(KSkill* pSkill);
    BOOL CheckMountRequest(KSkill* pSkill);
    BOOL CheckEvent();

private:
    int m_nCacheHitRatio;
    int m_nCacheMissRatio;
    int m_nCacheHitCount;
    int m_nCacheMissCount;
};
#endif	//_KSKILL_MANAGER_H_

