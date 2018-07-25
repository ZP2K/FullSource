#ifndef	_KAI_BASE_
#define _KAI_BASE_

#include "Global.h"
#include "SO3Result.h"
#include "KPathFinder.h"

class KSkill;
class KCharacter;
class KNpc;
class KPlayer;
class KTarget;
class KNpcTeam;
class KPatrolPath;
class KAIParamTemplate;

#define AI_IMMEDIATELY_CAST_EXP		(INT_MAX)   //���ܵ�����ֵ�ﵽʱ�����ͷ�

//�������
#define AI_FOLLOW_RANGE				2048		//������ֹͣ����
#define AI_FOLLOW_RUN				512			//������ʹ�ñ��ܣ�������·
#define AI_FOLLOW_KEEPAWAY			128			//����ʱ���ֵľ���

#define DEFAULT_IDLE_FRAME          (1024 * 1024)
#define DEFAULT_ESCAPE_FRAME        -1
#define DEFAULT_ESCAPE_RANGE        -1

#define CALL_HEAL_LIMIT             0.7
#define CALL_HEAL_MAX_RATE          1024
#define CALL_HEAL_RATE              1000
#define CALL_HEAL_RANGE             2048
#define CALL_HEAL_INTERVAL          (5 * GAME_FPS)

#define MAX_SKILL_RATE              1024

#define MAX_SELECT_TARGET_COUNT     8   //����ѡ���ͷŵ�ʱ��ῼ��8����λ

//#define ADD_RATE_PER_TARGET_IN_AOE 128 //ÿ�����Ŀ�����ӵ��ͷ�AOE�ļ���

#define AI_TRACE(__STRING__)	


class KAIBase
{
public:
	KAIBase(void);
	~KAIBase(void);

	virtual BOOL Init(KCharacter* pSelf);
	virtual BOOL UnInit(void);

    KNpcTeam* GetNpcTeam();

private:
	BOOL SetAIState(KAI_STATE eNewState);
	AI_TYPE GetAIType(void);
	void SetOriginPoint(int nX, int nY, int nZ, int nDir);
	BOOL GetOriginPoint(int* pnRetX, int* pnRetY, int* pnRetZ, int* pnRetDir);
	BOOL SetAIInterval(int nAIInterval);
	BOOL SetAIInterval(double fRate);
	KAIParamTemplate* GetAIParam();
	BOOL LoadParamFromTemplate();

#if defined(_SERVER)
	BOOL SetNpcTeam(KNpcTeam* pNpcTeam);
	BOOL SetPatrolPath(int nPatrolPathID, int nNextPointIndex = 0);
#endif //_SERVER

protected:
	virtual void Activate(void);

	// -------------- AI ״̬���� ------------------------------
	virtual void OnIdle();
	virtual void OnWander();
	virtual void OnPartol();
	virtual void OnAlert();
	virtual void OnFollow();
	virtual void OnPursuit();
	virtual void OnKeepAway();
	virtual void OnEscape();
	virtual void OnReturn();
	virtual void OnWait();

public:
	// -------------- AI �¼����� ------------------------------
	virtual BOOL OnTryMove(int nRelativeZ, BOOL bIsWater);
	virtual void OnAttacked(KCharacter* pAttacker, int nDamage);
	virtual void OnClearThreat(KCharacter* pCharater);
	virtual void OnPathResult(BOOL bSucceed);

public:
	// ------------- ״̬�л����� -----------------------------
	void DoIdle(int nIdleFrame = DEFAULT_IDLE_FRAME);
	void DoWander();
	void DoPatrol();
	void DoAlert();
	void DoFollow();
	void DoPursuit();
	void DoKeepAway();
	void DoEscape();
	void DoReturn();
	void DoWait();

#ifdef _SERVER
	// ------------- ����ѡ���������� -----------------------------
protected:
	virtual void InitSkillCD();
	virtual void CheckAttackTarget();

	virtual int GetSkillExp_PeriodTarget(DWORD dwSkillIndex, KTarget* pTarget);
	virtual int GetSkillExp_PeriodSelf(DWORD dwSkillIndex, KTarget* pTarget);
	virtual int GetSkillExp_PeriodFriend(DWORD dwSkillIndex, KTarget* pTarget);
	virtual int GetSkillExp_PassiveRespond(DWORD dwSkillIndex, KTarget* pTarget);
	virtual int GetSkillExp_ChannelingBreak(DWORD dwSkillIndex, KTarget* pTarget);
	virtual int GetSkillExp_TargetCount(DWORD dwSkillIndex, KTarget* pTarget);
	virtual int GetSkillExp_SelfHP(DWORD dwSkillIndex, KTarget* pTarget);

public:
	int GetSkillExpectation(AI_SKILL_TYPE eSkillType, DWORD dwSkillIndex, KTarget* pTarget);
#endif //_SERVER

public:
	// AI��ز����趨
	union PARAMS
	{
		int	m_nParams[AI_PARAM_COUNT];
		struct _DEFAULT_PARAMS
		{
			int		nSearchRange;
			int		nWanderMaxRange;
			int		nWanderRange;
			int		nWanderDuration;
			int		nFollowAttackRange;
		}DefaultParams;
		struct _CRITTER_PARAMS
		{
			int		nSearchRange;
			int		nWanderMaxRange;
			int		nWanderRange;
			int		nWanderDuration;
			int		nFollowAttackRange;
		}CritterParams;
	}m_Params;

	// ------------- һЩ���ù��ߺ��� -----------------------------

protected:
	void GetAdjustPoint(int nSrcX, int nSrcY, int nDistance, int nDir, OUT int& nDstX, OUT int& nDstY); //��ȡһ����ļ�����ƫ�Ƶ�
	void Follow();
	void ClearTarget(void);
#if defined(_SERVER)
	void SaveReturnPoint(void);	
	void Wander(BOOL bForceBack = false);
	void Escape(void);
	void Patrol(void);

	void Alert(KCharacter* pTarget);
	BOOL CallHelp(int nRange);
	BOOL CallHeal(int nRange);
	void TurnToFight(void);
	void TurnToIdle(void);

	void CheckAIEvent(void);
	void FireAIEvent(AI_EVENT_CONDITION eAIEvent);
	BOOL CheckReturn(void);
	void CheckCallHeal(void);
	BOOL CheckWanderRange(int nOriginDistance2, int nWanderRange);
	BOOL CheckAttacked(void);
	BOOL CheckTargetInAlertRange(void);
    BOOL CheckTargetInAttackRange(void);    // ����Ŀ��,����ѵ�,��ת��׷��״̬

	//for fight
    void KeepAttackRange(KCharacter* pTarget, KSkill* pSkill);
    void KeepAttackRangeAStar(KCharacter* pTarget, KSkill* pSkill); // A*Ѱ·��ʱ����
	void KeepWanderRange(int nRange);
#endif	//_SERVER

public:
	union{
		struct _IDLE_DATA
		{
			int nIdleFrameCount;		//Idleʱ�������
		} m_IdleData;

		struct _WANDER_DATA
		{
			int nWanderFrame;           //�й䳬ʱ
		} m_WanderData;

		struct _PARTOL_DATA
		{
			int nDummy;
		} m_PartolData;

		struct _ALERT_DATA
		{
			int nAlartFrame;			//���䳬ʱ����������
		} m_AlertData;

		struct _FOLLOW_DATA
		{
			DWORD dwFollowTargetID;		//��ǰ���ڸ����Ŀ��
		} m_FollowData;

		struct _PURSUIT_DATA
		{
			int nTurnToFightFrame;		//����ս���ļ�����
			int nNpcSkillCommonCD;		//Npc���ܹ���CD
			int nCallHealFrame;			//�������Ƶļ�����
			DWORD dwCallByNpcID;		//�����ߵ�ID;
			DWORD dwKeepDisDivisor;		//Npc��Ŀ�걣�־���ĳ���,��ʼΪ1,ÿ�μ����ͷ�ʱ������벻��,�����1,�ɹ�������1
		} m_PursuitData;

		struct _KEEPAWAY_DATA
		{
			int nDummy;
		} m_KeepAwayData;

		struct _ESCAPE_DATA
		{
			int nEscapeIdleFrame;		//ÿ�������е�ͣ��ʱ��
			int nEscapeFrame;			//���ܵ�ʱ��
			int nEscapeRange;			//���ܵķ�Χ
		} m_EscapeData;

		struct _RETURN_DATA
		{	
			int nReturnFrame;			//���س�ʱ�ļ�����
		} m_ReturnData;

		struct _WAIT_DATA
		{
			int nTimeOutFrame;			// Wait��ʱʱ��
		} m_WaitData;
	};

public:
	KAI_STATE		m_eAIState;
	int				m_nAIEventTimer[MAX_AI_EVENT];
protected:
	AI_TYPE				m_eAIType;
	KAIParamTemplate*	m_pAIParam;
	KCharacter*			m_pSelf;

	int				m_nAIInterval;			//AI��ִ�м��
	int				m_nNextActiveFrame;		//AI֡������

	int				m_nOriginX;				//NPC��ԭʼ���꣬�ӱ༭����ȡ
	int				m_nOriginY;
	int             m_nOriginZ;
	int				m_nOriginDirection;		//NPCԭʼ����

	int				m_nReturnX;				//NPC����ս��ʱ���������
	int				m_nReturnY;
	int				m_nReturnZ;

	int				m_nNextPartolIndex;		//��һ��Ѱ·��	

	int				m_nNextIdleActionFrame;	//Npc�����ڿ��е�ʱ�򲻶ϳ���˵��,����Ǽ�����

    int             m_nNextAStarFrame;

	// ------------- AI�ɱ���� ------------------------------
	KAI_STATE		m_eAIMainState;

#if defined(_SERVER)
	KNpcTeam*		m_pNpcTeam;
	KPatrolPath*	m_pPatrolPath;

    KPathFinder     m_PathFinder;
#endif //_SERVER

	friend class KAIControl;
	friend class KAIEvent;
};

#endif //_KAI_BASE_
