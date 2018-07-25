// ------------------------------------------------------------------------
// ---------------- Ĭ�ϱ���AI ---------------------------------

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

const int IDLE_FRAME = 8;

void KAIController::_Construct_Negative()
{
    ASSERT(m_pSelf);
    m_eAIState = aisIdle;
    m_nFrameCount = g_SO3World.m_nGameLoop + IDLE_FRAME;
    // ��Ҫ��DoIdle,��ʱ����س�ʼ����δ���
}

void KAIController::_Activate_Negative()
{
    _Activate();
}

void KAIController::_OnStateIdle_Negative()
{
    // ����Ƿ񱻹���,���������,��ת��׷��״̬
	KThreatNode* pThreatNode = m_pSelf->m_ThreatList.GetFirstThreat();
	if (pThreatNode)
	{
        AI_TRACE("���˴���,ƴ�� !");
        DoPursuit();
        return;
	}

    if (g_SO3World.m_nGameLoop > m_nFrameCount)
    {
        AI_TRACE("�𷢴���,������ !");
        DoPatrol();
        return;
    }
}

void KAIController::_OnStatePatrol_Negative()
{
    int nRetCode = false;

    // ����Ƿ񱻹���,���������,��ת��׷��״̬
	KThreatNode* pThreatNode = m_pSelf->m_ThreatList.GetFirstThreat();
	if (pThreatNode)
	{
        AI_TRACE("���˴���,ƴ�� !");
        DoPursuit();
        return;
	}

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

Exit0:
    return;
}

void KAIController::_OnStateAlert_Negative()
{
}

void KAIController::_OnStateFollow_Negative()
{
}

void KAIController::_OnStatePursuit_Negative()
{
	int nRetCode = false;

    // ����ޱ��Ƿ�Ϊ��
	KThreatNode* pThreatNode = m_pSelf->m_ThreatList.GetFirstThreat();
	if (!pThreatNode)
	{
        AI_TRACE("��ޱ�յ�,������,�������!");
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

	KNpcTemplate* pTemplate = g_WorldSettings.m_NpcTemplateList.GetTemplate(((KNpc*)m_pSelf)->m_dwTemplateID);
	KGLOG_PROCESS_ERROR(pTemplate);

	DWORD dwSkillID = pTemplate->dwSkillIDList[0];
	DWORD dwSkillLevel = pTemplate->dwSkillLevelList[0];

    char szMsg[64];

    KGLOG_PROCESS_ERROR(pTarget != m_pSelf);

    sprintf(szMsg, "��ͼ�ü���(%u, %u)����\"%s\"", dwSkillID, dwSkillLevel, pTarget->m_szName);
    AI_TRACE(szMsg);

    KGLOG_PROCESS_ERROR(dwSkillID != INVALID_SKILL_ID);

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

    nRetCode = m_pSelf->CastSkill(dwSkillID, dwSkillLevel, Target);

    switch (nRetCode)
    {
    case srcSuccess:
        AI_TRACE("������,��������� !");
        break;
    case srcTooCloseTarget:
        sprintf(szMsg, "��,̫���� !\n", nRetCode);
        AI_TRACE(szMsg);
        break;
    case srcTooFarTarget:
        sprintf(szMsg, "��,̫Զ�� !\n", nRetCode);
        AI_TRACE(szMsg);
        break;
    case srcOutOfAngle:
        AI_TRACE("�ǶȲ��� !");
        m_pSelf->Turn(m_pSelf->m_nFaceDirection + 17);
        break;
    case srcTargetInvisible:
        AI_TRACE("������Ŀ�� !");
        break;
    case srcNotEnoughLife:
    case srcNotEnoughMana:
    case srcNotEnoughStamina:
    case srcNotEnoughRage:
    case srcNotEnoughItem:
        AI_TRACE("�����,���Ĳ����� !");
        break;
    default:
        sprintf(szMsg, "�����,��ô���˰� (%d). \n", nRetCode);
        AI_TRACE(szMsg);
        break;
    }

    // ά�־���
    KSkill* pSkill = g_SkillManager.GetSkill_S(dwSkillID, dwSkillLevel);
    KGLOG_PROCESS_ERROR(pSkill);

    KeepAttackRange(pTarget, pSkill);

Exit0:
    return;
}

void KAIController::_OnStateKeepAway_Negative()
{
}


void KAIController::_OnStateEscape_Negative()
{
}

void KAIController::_OnStateReturn_Negative()
{
    int nRetCode = false;

    nRetCode = m_pSelf->GetMoveState();
    if (nRetCode == cmsOnStand)
    {
        m_pSelf->WalkTo(m_nOriginX, m_nOriginY);
    }

	KThreatNode* pThreatNode = m_pSelf->m_ThreatList.GetFirstThreat();
	if (pThreatNode)
	{
        DoPursuit();
        return;
	}
}

void KAIController::_OnEventAttacked_Negative(KCharacter* pAttacker, int nDamage)
{
}

void KAIController::_OnEventPathResult_Negative(BOOL bSucceed)
{
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
            //_OnStatePursuit_Negative();
        }
        break;
    case aisEscape:
        break;
    case aisReturn:
        if (!bSucceed)
        {
            m_pSelf->m_nX = m_nOriginX;
            m_pSelf->m_nY = m_nOriginY;
        }
        DoIdle(IDLE_FRAME);
        break;
    default:
        break;
    }

Exit0:
    return;
}


#endif
