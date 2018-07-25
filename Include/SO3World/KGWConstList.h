#ifndef _KGW_CONST_LIST_
#define _KGW_CONST_LIST_

#include "Global.h"

#define PLAYER_AND_QUEST_DIFF_LEVEL_COUNT 4 
#define MAX_KILL_COUNT                    11

struct KGWConstList
{
public:
	// AI
	int nMaxThreatCount;
	int nNpcSkillCommonCD;
	int nNpcCommonShortCD;
    int nAISearchByLifeConversionLevel;

	// PK
	int nPKFlagDoodadTemplateID;
    int nSlayCDIndex;
    int nWaitSlayTime;
    int nCloseSlayTime;
    int nMinDuelLevel;
    int nMinRevengeLevel;
    int nMinSlayLevel;
    int nMinBeSlayedLevel;
    int nKillPointReduceCycle;
    int nReduceKillPoint;

    // Mail
    int nMailDelaySecondOnText;
    int nMailDelaySecondOnMoney;
    int nMailDelaySecondOnItem;
    int nMailSurvivalTime;

	// Item
	int nBankPackagePrice[MAX_BANK_PACKAGE_COUNT];

    // Fellowship
    int nTeamAttractiveInterval;       // ������Ӻøжȵļ��(Frame)
    int nTeamAttractiveRange;          // ������Ӻøжȵľ���
    int nTeamAttractiveUpLimit;        // ������Ӻøжȵ�����ֵ
    int nTeamAttractiveAdd;

    int nTeamAttractiveIntervalOnlyGroup;       // ������Ӻøжȵļ��(Frame)
    int nTeamAttractiveRangeOnlyGroup;          // ������Ӻøжȵľ���
    int nTeamAttractiveUpLimitOnlyGroup;        // ������Ӻøжȵ�����ֵ
    int nTeamAttractiveAddOnlyGroup;

    int nDuelAttractiveDailyCount;      // �д����Ӻøжȵ�ÿ������
    int nDuelAttractiveUpLimit;         // �д����Ӻøжȵ�����ֵ
    int nDuelAttractiveAdd;

    int nAssistAttractiveUpLimit;       // Э���������Ӻøжȵ�����ֵ
    int nAssistPresentContribution;     // һ�������˻���Э�����ӹ��׶�

    int nWhisperAttractiveDailyCount;   // �������Ӻøжȵ�ÿ������
    int nWhisperAttractiveUpLimit;      // �������Ӻøжȵ�����ֵ
    int nWhisperAttractiveAdd;

    int nAttractiveLevel[MAX_FELLOWSHIP_ATTRACTIVE_LEVEL];          // �øжȵȼ��Ļ���
    int nAttractiveAchievement[MAX_FELLOWSHIP_ATTRACTIVE_LEVEL];    // �øжȵȼ��ɾ�
    int nCountAchievement[MAX_FELLOWSHIP_COUNT_ACHIEVEMENT];        // ���������ɾ�

    int nDifferentCampApplyJoinTeam;                                // ��ͬ��Ӫ�����Ҫ�ĺøжȵȼ�

    // ��ˮ������ʱ��,��λ: ��
    int nDrowningTime;
    int nSaveInterval;
    
    // NPC������,����ʬ����ӳ�ʱ��
    int nCorpseGenerationFrame;

    // ��ҿ����������ȼ�
    int nPlayerMaxLevel;

    // Skill
    int nRiseFrame;
    int nSkidFrame;
    int nNoAddSkillExp;

    // Talk
    int nSceneChannelCoolDownID;
    int nWorldChannelCoolDownID;
    int nForceChannelCoolDownID;
    int nCampChannelCoolDownID;
    int nMentorChannelCoolDownID;
    int nFriendsChannelCoolDownID;

    // ��޲���ӿ�
    int nApplyThreatRankListCD;

    int nPlayerAndQuestDiffLevel[PLAYER_AND_QUEST_DIFF_LEVEL_COUNT];

    // ��Ӫ������صĽ�����ͷ�
    int nReducePrestigeOnDeath;
    int nSoloKill;
    int nLastHit;
    int nCommonKill;
    int nKillerTeamMember;
    int nSameCampKill;
    int nSameCampAssistKill;
    int nKilledCountPercent[MAX_KILL_COUNT];
    int nResetKilledCountCycle;
    // ��Ӫ��������
    int nMaxPrestige; 
    
    // �Ƿ�ʹ���µ�ʹ�õ��߻��ƣ�������
    BOOL bNewUseItemStyle;

    // �շ����
    int nFreeMaxLevel;      // ��ѽ׶����ȼ�
    int nFreeDurationTime;  // ����ʱ��
    int nEndOfDayTimeDealy; // ��ֹʱ�������ʱ

    // ȫ����������ɾ�
    int nQuestCountAchievementID;
    int nAssistQuestCountAchievementID;
    int nFinishQuestAssistByOtherAchievementID;

    int nNpcReviveTimeReduceThreshold;
    int nNpcReviveTimeIncreaseThreshold;
    int nNpcReviveTimeReduceRate;
    int nNpcReviveTimeIncreaseRate;

    int nMaxDesignationBynameIndex;

    // ����Ǯ
    int     nAntiFarmerCheckOnlineTimeRange;
    float   fAntiFarmerOnlineRate;
    float   fAntiFarmerKillNpcRate;
    int     nAntiFarmerKillNpcTypeNum;
    int     nAntiFarmerRecordKillNpcNum;
    int     nAntiFarmerPunishTime;
    float   fAntiFarmerPunishRate;

    BOOL bFreeForAll;
    BOOL bDistribute;
    BOOL bGroupLoot;

    int  nLootItemQualityToLog;

	BOOL Init();
	void UnInit();
private:
	BOOL LoadData(IIniFile* piIniFile);
};

#endif //_KGW_CONST_LIST_
