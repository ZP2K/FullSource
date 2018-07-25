#ifndef _KOPERATION_SIMULATOR_H_
#define _KOPERATION_SIMULATOR_H_

#include "Global.h"
#include "IKOperationSimulator.h"

const int PLAYER_AI_INTERVAL    = 1;

const int MAX_DIRECTION_OFFSET  = 16;

const int ARRIVE_ORIGIN_TIMEOUT = GAME_FPS * PLAYER_AI_INTERVAL * 12;
const int MOVE_TIMEOUT          = DIRECTION_COUNT / MAX_DIRECTION_OFFSET * PLAYER_AI_INTERVAL * 2;
const int MOVE_RANGE_TIMEOUT    = GAME_FPS * PLAYER_AI_INTERVAL * 20;
const int GM_COMMAND_TIMEOUT    = GAME_FPS * 15;

const int MAX_WANDER_RANGE      = CELL_LENGTH * 64;
const int MIN_WANDER_RANGE      = CELL_LENGTH * 16;

class KMonster;
class KCharacter;

class KOperationSimulator : public IKOperationSimulator
{
public:
    KOperationSimulator(KMonster *pOwner);
    ~KOperationSimulator();

    virtual int Start()
    {        
        m_nRunFlag = TRUE;
        return TRUE;
    }
    virtual int Stop()
    {
        m_nRunFlag = FALSE;
        m_nNextActiveFrame = 0;
        m_nOriginFrame = 0;
        m_nGMCommandFrame = 0;
        m_LastArrivePosition.m_nArriveFrame = 0;
        m_LastBeyondPosition.m_nArriveFrame = 0;
        return TRUE;
    }

    virtual int Activate();   

private:    
    void OnBorn();
    void OnFollow();
    void OnWander();
    void OnFight();
private:
    void DoBorn()  {m_eState = ssBorn;}
    void DoFollow(){m_eState = ssFollow;}
    void DoWarder(){m_eState = ssWander;}
    void DoFight() {m_eState = ssFight;}

private:
    int Follow();
private:
    int CheckGMCommandTimeout();

    int MoveToDestRegion();

    int CheckArriveOrigin(); //����Ƿ��ܹ��ص�ԭ��
    int CheckMove();//����Ƿ��ܹ��ƶ������һֱû�����������ƶ����ٲ���������
    int CheckMoveRange();//����ƶ���Χ�Ƿ��С�������С������
    int RandomMove();//�������һ�ξ���

    int Wander();

    int FindZCoordinate(int nX, int nY, int &nZ);

    int CheckSelfState();

    void SetOriginPoint(int nX, int nY, int nZ, int nDir);
private:
    int SearchTarget();
    int CastSkill();
    int CheckTargetState();
    int CheckArriveTarget();
    int Fight();
    KCharacter *GetTarget();
    int EquipSkill();

private:
    KMonster *m_pOwner;
    int m_nRunFlag;

    enum SIMULATOR_STATE
    {
        ssBorn,
        ssFollow,
        ssWander,
        ssFight
    }m_eState;

    int m_nNextActiveFrame;

    int m_nOriginX;				//NPC��ԭʼ���꣬�ӱ༭����ȡ
    int m_nOriginY;
    int m_nOriginZ;
    int m_nOriginDirection;		//NPCԭʼ����   

    int m_nOriginFrame;         //��¼����ԭʼ����ʱ��֡��
    int m_nGMCommandFrame;      //��¼ִ��GMָ��ʱ��֡��

    struct ARRIVE_POSITION
    {
        int nX;
        int nY;
        int m_nArriveFrame;
    };
    ARRIVE_POSITION m_LastArrivePosition;                   //�ϴε���ĵص㼰ʱ��
    ARRIVE_POSITION m_LastBeyondPosition;

    int m_nFollowOrFightFlag;
    int m_nFollowFightTimeout;
    int m_nFindTargetTime;

    DWORD m_dwTargetID;
};

#endif //_KOPERATION_SIMULATOR_H_