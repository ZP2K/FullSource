////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KAI_Player.h
//  Version     : 1.0
//  Creator     : Zhu Chengmin
//  Comment     : ����õ�AI,���а�������ұ����ƺ����Ϊ(�����)����Ҹ���Ŀ��(�ͻ���)��
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _KAI_PLAYER_
#define _KAI_PLAYER_

#include "KAIBase.h"

class KAI_Player : public KAIBase
{
#if defined(_CLIENT) 
	virtual void Activate();
	
	virtual void OnFollow();
	virtual void OnPathResult(BOOL bSucceed);
#endif

#ifdef _SERVER
public:
	virtual void OnIdle();
	virtual void OnWander();
	virtual void OnPartol();
	virtual void OnAlert();
	//virtual void OnFollow();
	virtual void OnPursuit();
	virtual void OnKeepAway();
	virtual void OnEscape();
	virtual void OnReturn();
	virtual void OnWait();
    virtual BOOL OnTryMove(int nRelativeZ, BOOL bIsWater);
	virtual void OnAttacked(KCharacter* pAttacker, int nDamage);
	//virtual void OnPathResult(BOOL bSucceed);
#endif //_SERVER
};

#endif //_KAI_PLAYER_
