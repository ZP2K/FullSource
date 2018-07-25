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

	// Mail
	int nMailDelaySecondOnText;
	int nMailDelaySecondOnMoney;
	int nMailDelaySecondOnItem;
	int nMailSurvivalTime;

	// Item
	int nBankPackagePrice[MAX_BANK_PACKAGE_COUNT];

	// Fellowship
	int nPartyAttractiveInterval;       // ������Ӻøжȵļ��(Frame)
	int nPartyAttractiveRange;          // ������Ӻøжȵľ���
	int nPartyAttractiveUpLimit;        // ������Ӻøжȵ�����ֵ
	int nPartyAttractiveAdd;

	int nPartyAttractiveIntervalOnlyGroup;       // ������Ӻøжȵļ��(Frame)
	int nPartyAttractiveRangeOnlyGroup;          // ������Ӻøжȵľ���
	int nPartyAttractiveUpLimitOnlyGroup;        // ������Ӻøжȵ�����ֵ
	int nPartyAttractiveAddOnlyGroup;

	int nDuelAttractiveDailyCount;      // �д����Ӻøжȵ�ÿ������
	int nDuelAttractiveUpLimit;         // �д����Ӻøжȵ�����ֵ
	int nDuelAttractiveAdd;

	int nAssistAttractiveUpLimit;       // Э���������Ӻøжȵ�����ֵ

	int nWhisperAttractiveDailyCount;   // �������Ӻøжȵ�ÿ������
	int nWhisperAttractiveUpLimit;      // �������Ӻøжȵ�����ֵ
	int nWhisperAttractiveAdd;

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

	int nSceneChannelCoolDownID;
	int nWorldChannelCoolDownID;

	int nForceChannelCoolDownID;
	int nForceChannelDailyLimit;

	int nCampChannelCoolDownID;
	int nCampChannelDailyLimit;

	int nPlayerAndQuestDiffLevel[PLAYER_AND_QUEST_DIFF_LEVEL_COUNT];

	// ��Ӫ������صĽ�����ͷ�
	int nReducePrestigeOnDeath;
	int nSoloKill;
	int nLastHit;
	int nCommonKill;
	int nKillerPartyMember;
	int nSameCampKill;
	int nSameCampAssistKill;
	int nKilledCountPercent[MAX_KILL_COUNT];
	int nResetKilledCountCycle;

	// �Ƿ�ʹ���µ�ʹ�õ��߻��ƣ�������
	BOOL bNewUseItemStyle;

	BOOL Init();
	void UnInit();
private:
	BOOL LoadData(IIniFile* piIniFile);
};

#endif //_KGW_CONST_LIST_
