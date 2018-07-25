////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KAIVM.h
//  Version     : 1.0
//  Creator     : Chen Jie, zhaochunfeng
//  Comment     : AI ״̬�����
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _KAIVM_H_ 
#define _KAIVM_H_ 

#include "KAIState.h"
#include "KAIAction.h"

class KAIState;
class KCharacter;
class KAILogic;

enum KAI_EVENT
{
    aevInvalid  = 0,

    aevOnPrimaryTimer,
    aevOnSecondaryTimer,
    aevOnTertiaryTimer,
    aevOnPathSuccess,
    aevOnPathFailed,
    aevOnDialog,
    aevOnAction,
    aevOnAttacked,
    aevOnSelfKilled,
    aevOnTargetKilled,
    aevOnInFear,
    aevOnOutFear,
    aevOnBeginPrepare,
    aevOnBreakPerpare,
    aevOnCastSkill,
    aevOnCastSkillFailed,
    aevOnOutOfBattleZone,

    aevTotal,
};

// �û��Զ���AI event�� KAI_USER_EVENT ��ʼ
#define KAI_USER_EVENT 1000

class KAIVM
{
public:
    KAIVM();

    BOOL Setup(KCharacter* pCharacter, int nAIType);

    int  GetAIType();

    void Active();

    void FireEvent(int nEvent, DWORD dwEventSrc, int nEventParam);

    BOOL SetState(int nState);
    void SetPrimaryTimer(int nFrame);
    void SetSecondaryTimer(int nFrame);
    void SetTertiaryTimer(int nFrame);

    void AddupTotalDamageValue(int nDeltaDamageValue);

    void DebugAICurrentStateInfo();

public:
    int                 m_nDebugCount;
    uint64_t            m_ullRunTimeStatistic;

private:
    KCharacter*         m_pOwner;
    int                 m_nAIType;
    KAILogic*           m_pAILogic;
    KAIState*           m_pState;
    int                 m_nStateID;
    int                 m_nActionID;
    int                 m_nPrimaryTimerFrame;
    int                 m_nSecondaryTimerFrame;
    int                 m_nTertiaryTimerFrame;
    int                 m_nCurrentEvent; // ��ʾ"����ĳ��Event�Ĵ���������"
    int                 m_nPendingEvent;
    DWORD               m_dwPendingEventSrc;
    int                 m_nPendingEventParam;
};

#endif
