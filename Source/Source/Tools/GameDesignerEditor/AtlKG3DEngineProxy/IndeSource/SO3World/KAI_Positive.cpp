// ------------------------------------------------------------------------
// ---------------- Ĭ������AI ---------------------------------

#include "stdafx.h"

#if defined(_SERVER)

#include "KAIController.h"
#include "KMath.h"
#include "KSO3World.h"
#include "KObjectManager.h"
#include "KWorldSettings.h"
#include "KSkill.h"
#include "KSkillManager.h"
#include "KAISearchTactics.h"

static const int IDLE_FRAME  =  8;

void KAIController::_Construct_Positive()
{
    int     nRetCode = false;
    KNpc*   pNpc     = NULL;

    KG_ASSERT_EXIT(m_pSelf);

    nRetCode = g_IsPlayer(m_pSelf->m_dwID);
    KG_ASSERT_EXIT(!nRetCode);

    pNpc = (KNpc*)m_pSelf;

    m_eAIState      = aisIdle;
    m_nFrameCount   = g_SO3World.m_nGameLoop + IDLE_FRAME;

    // ��Ҫ��DoIdle,��ʱ����س�ʼ����δ���
	KNpcTemplate* pTemplate = g_WorldSettings.m_NpcTemplateList.GetTemplate(pNpc->m_dwTemplateID);
	KG_ASSERT_EXIT(pTemplate);

    m_nAlarmRange = pTemplate->nAlarmRange;


Exit0:
    return;
}

void KAIController::_Activate_Positive()
{
    _Activate();
}

void KAIController::_OnStateIdle_Positive()
{
	BOOL bRetCode = FALSE;

	bRetCode = CheckAttacked();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

    if (g_SO3World.m_nGameLoop > m_nFrameCount)
    {
        AI_TRACE("�𷢴���,������ !");
        DoPatrol();
        return;
    }
Exit1:
	return;
}

void KAIController::_OnStatePatrol_Positive()
{
    int nRetCode = false;
	BOOL bRetCode = FALSE;

	bRetCode = CheckAttacked();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

    nRetCode = m_pSelf->GetMoveState();
    if (nRetCode == cmsOnStand)
    {
        int         nAngel = m_pSelf->m_nFaceDirection - 40 + (int)g_Random(80);        
        const int   nRange = 32 * 2 * 16; // 16��
        if (nAngel < 0)
            nAngel += DIRECTION_COUNT;
        if (nAngel >= DIRECTION_COUNT)
            nAngel -= DIRECTION_COUNT;
        KGLOG_PROCESS_ERROR(nAngel >= 0 && nAngel < DIRECTION_COUNT);
        int nDeltaX = nRange * g_nCos[nAngel] / SIN_COS_NUMBER;
        int nDeltaY = nRange * g_nSin[nAngel] / SIN_COS_NUMBER;
		m_pSelf->WalkTo(m_pSelf->m_nX + nDeltaX, m_pSelf->m_nY + nDeltaY);
		return;        
    }

    KeepWanderRange(CELL_LENGTH * 2 * 100);

Exit1:
	return ;
Exit0:
    return ;
}

void KAIController::_OnStateAlert_Positive()
{
}

void KAIController::_OnStateFollow_Positive()
{
}

void KAIController::_OnStatePursuit_Positive()
{
	int nRetCode = FALSE;
	BOOL bSkipKeepRange = FALSE;

    // ����ޱ��Ƿ�Ϊ��
	KThreatNode* pThreatNode = m_pSelf->m_ThreatList.GetFirstThreat();
	if (!pThreatNode)
	{
        AI_TRACE("��ޱ�յ�,�����˻�ȥ!");
        DoReturn();
        return;
	}

	KCharacter* pTarget = pThreatNode->pCharacter;
	KG_ASSERT_EXIT(pTarget);
    KGLOG_PROCESS_ERROR(pTarget->m_pScene);    
    KGLOG_PROCESS_ERROR(pTarget->m_pCell);    

    // ��鼼��״̬
	if (m_pSelf->m_OTActionParam.eActionType != otActionIdle)
    {
        AI_TRACE("��������������,�ݲ�����!");
        return;
    }

	KGLOG_PROCESS_ERROR(m_pSelf->m_eKind != ckPlayer);

	KNpc* pNpc = dynamic_cast<KNpc*>(m_pSelf);
	KGLOG_PROCESS_ERROR(pNpc);

	// �ͷ���ͨ���������Դ���
	KNpcTemplate* pTemplate = g_WorldSettings.m_NpcTemplateList.GetTemplate(((KNpc*)m_pSelf)->m_dwTemplateID);
	KGLOG_PROCESS_ERROR(pTemplate);

	DWORD dwSkillID = pTemplate->dwSkillIDList[0];
	DWORD dwSkillLevel = pTemplate->dwSkillLevelList[0];

    char szMsg[64];

    KGLOG_PROCESS_ERROR(pTarget != m_pSelf);

    sprintf(szMsg, "��ͼ�ü���(%u, %u)����\"%s\"", dwSkillID, dwSkillLevel, pTarget->m_szName);
    AI_TRACE(szMsg);

    KG_PROCESS_ERROR(dwSkillID != INVALID_SKILL_ID);

    KTarget Target;
    if (g_IsPlayer(pTarget->m_dwID))
    {
        Target.eTargetType = ttPlayerPointer;
    }
    else
    {
        Target.eTargetType = ttNpcPointer;
    }
    Target.pCharacter = pTarget;

    int   nDirection  = g_GetDirection(m_pSelf->m_nX, m_pSelf->m_nY, pTarget->m_nX, pTarget->m_nY);

    if (m_pSelf->m_nFaceDirection != nDirection)
    {
        m_pSelf->Turn(nDirection);
    }

	// ��⼼���ͷ�Ƶ��
	// ��ʱ����
	if ((g_SO3World.m_nGameLoop - pNpc->m_nSkillCastFrame[0]) >= AI_ATTACK_INTERVAL)
	{
		nRetCode = m_pSelf->CastSkill(dwSkillID, dwSkillLevel, Target);

		switch (nRetCode)
		{
		case srcSuccess:
			//KGLogPrintf(KGLOG_DEBUG, "������,��������� !");

			// ���Դ��룺�ж�Ŀ����ƶ�״̬
			if (pTarget && pTarget->m_eMoveState == cmsOnStand)
			{
				m_pSelf->m_ThreatList.UpdateKeepThreatField();
				bSkipKeepRange = TRUE;
			}
			break;
		case srcTooCloseTarget:
			sprintf(szMsg, "��,̫���� !\n", nRetCode);
			//KGLogPrintf(KGLOG_DEBUG, szMsg);
			break;
		case srcTooFarTarget:
			sprintf(szMsg, "��,̫Զ�� !\n", nRetCode);
			//KGLogPrintf(KGLOG_DEBUG, szMsg);
			break;
		case srcOutOfAngle:
			//KGLogPrintf(KGLOG_DEBUG, "�ǶȲ��� !");
			m_pSelf->Turn(m_pSelf->m_nFaceDirection + 17);
			break;
		case srcTargetInvisible:
			//KGLogPrintf(KGLOG_DEBUG, "������Ŀ�� !");
			break;
		case srcNotEnoughLife:
		case srcNotEnoughMana:
		case srcNotEnoughStamina:
		case srcNotEnoughRage:
		case srcNotEnoughItem:
			//KGLogPrintf(KGLOG_DEBUG, "�����,���Ĳ����� !");
			break;
		default:
			sprintf(szMsg, "�����,��ô���˰� (%d). \n", nRetCode);
			//KGLogPrintf(KGLOG_DEBUG, szMsg);
			break;
		}

		//ˢ�¼��ܵ��ͷ�ʱ��
		//KGLogPrintf(KGLOG_DEBUG, "CastSkill \t\t\t\t In %d\n", g_SO3World.m_nGameLoop);
		pNpc->m_nSkillCastFrame[0] = g_SO3World.m_nGameLoop;
	}

	if (bSkipKeepRange == FALSE)
	{
		// ά�־���
		KSkill* pSkill = g_SkillManager.GetSkill_S(dwSkillID, dwSkillLevel);
		KGLOG_PROCESS_ERROR(pSkill);

		KeepAttackRange(pTarget, pSkill);
	}
Exit0:
    return;
}

void KAIController::_OnStateKeepAway_Positive()
{
}


void KAIController::_OnStateEscape_Positive()
{
}

void KAIController::_OnStateReturn_Positive()
{
    int nRetCode = false;

	// TODO: ��NPC�Ƴ�ս�������Ҹ����޵�״̬

	// �ܻ�ReturnPoint
	m_pSelf->RunTo(m_nReturnX, m_nReturnY);
}

void KAIController::_OnEventAttacked_Positive(KCharacter* pAttacker, int nDamage)
{
	BOOL bRetCode = FALSE;

	switch(m_eAIState)
	{
	case aisIdle:
	case aisPatrol:
	case aisAlert:
	case aisFollow:
		TurnToFight();
		DoPursuit();
		m_pSelf->m_ThreatList.UpdateKeepThreatField();
	    break;
	case aisPursuit:
		break;
	case aisEscape:
		break;
	case aisReturn:
		break;
	default:
		KGLOG_PROCESS_ERROR(FALSE);
	    break;
	}

Exit0:
	return;
}

void KAIController::_OnEventPathResult_Positive(BOOL bSucceed)
{
    BOOL bRetCode = false;

	//KGLogPrintf(KGLOG_DEBUG, "[AI] path event : %d , frame : %d\n", bSucceed, g_SO3World.m_nGameLoop);

    switch (m_eAIState)
    {
    case aisIdle:        
        break;
    case aisPatrol:
        if (bSucceed)
        {
            // ����ɹ���,��һ�������˵
            DoIdle(IDLE_FRAME);
        }
        else
        {
            // ���Ѱ·ʧ����,�򻻸���������
            int nDirection = m_pSelf->m_nFaceDirection + 40;
            if (nDirection >= DIRECTION_COUNT)
                nDirection -= DIRECTION_COUNT;
            KGLOG_PROCESS_ERROR(nDirection >= 0 && nDirection < DIRECTION_COUNT);
            m_pSelf->Turn(nDirection);
            DoPatrol();
        }
        break;
    case aisAlert:
        break;
    case aisFollow:
        break;
    case aisPursuit:
        if (bSucceed)
        {
            _OnStatePursuit_Positive();
        }
        break;
    case aisEscape:
        break;
	case aisReturn:
        if (!bSucceed)
        {
            //bRetCode = g_IsPlayer(m_pSelf->m_dwID);
            //KG_ASSERT_EXIT(!bRetCode);
            //KG_ASSERT_EXIT(m_pSelf->m_pScene);
            //m_pSelf->m_pScene->MoveNpc((KNpc*)m_pSelf, m_nOriginX,  m_nOriginY);
        }
		else
		{
			//KGLogPrintf(KGLOG_DEBUG, "ARRIVED Point x:%d y:%d \t IN %d\nSavePoint     x:%d y:%d\n", 
			//	m_pSelf->m_nX, m_pSelf->m_nY, g_SO3World.m_nGameLoop, m_nReturnX, m_nReturnY);
			DoIdle(IDLE_FRAME);
		}
        break;
    default:
		KGLOG_PROCESS_ERROR(FALSE);
        break;
    }

Exit0:
    return;
}


#endif
