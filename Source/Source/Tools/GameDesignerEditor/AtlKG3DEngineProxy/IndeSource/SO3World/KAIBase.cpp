#include "stdafx.h"
#include "KAIBase.h"
#include "KMath.h"
#include "KSO3World.h"
#include "KScene.h"
#include "KGWConstList.h"
#include "KNpc.h"

KAIBase::KAIBase()
{

}

KAIBase::~KAIBase()
{

}

void KAIBase::SetOriginPoint(int nX, int nY, int nZ, int nDir)
{
	m_nOriginX = nX;
	m_nOriginY = nY;
	m_nOriginZ = nZ;
	m_nOriginDirection = nDir;
}

BOOL KAIBase::GetOriginPoint(int* pnRetX, int* pnRetY, int* pnRetZ, int* pnRetDir)
{
    BOOL bResult = false;

    KG_PROCESS_ERROR(pnRetX);
    KG_PROCESS_ERROR(pnRetY);
    KG_PROCESS_ERROR(pnRetZ);
    KG_PROCESS_ERROR(pnRetDir);

	*pnRetX     = m_nOriginX;
	*pnRetY     = m_nOriginY;
	*pnRetZ     = m_nOriginZ;
	*pnRetDir   = m_nOriginDirection;

    bResult = true;
Exit0:
	return bResult;
}

BOOL KAIBase::SetAIInterval(int nAIInterval)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(nAIInterval > 0);

	m_nAIInterval = nAIInterval;

	if (g_pSO3World->m_nGameLoop + m_nAIInterval < m_nNextActiveFrame)
	{
		m_nNextActiveFrame = g_pSO3World->m_nGameLoop + m_nAIInterval;
	}

	bRetCode = TRUE;
Exit0:
	return bRetCode;
}

BOOL KAIBase::SetAIInterval(double fRate)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(fRate > 0.0 && fRate <= 1.0);

	m_nAIInterval = (int)(g_pSO3World->m_Settings.m_ConstList.nAIInterval * fRate);
	if (m_nAIInterval < 1)
		m_nAIInterval = 1;

	if (g_pSO3World->m_nGameLoop + m_nAIInterval < m_nNextActiveFrame)
	{
		m_nNextActiveFrame = g_pSO3World->m_nGameLoop + m_nAIInterval;
	}

	bRetCode = TRUE;
Exit0:
	return bRetCode;
}

KAIParamTemplate* KAIBase::GetAIParam()
{
	return m_pAIParam;
}

#if defined(_SERVER)
BOOL KAIBase::SetNpcTeam(KNpcTeam* pNpcTeam)
{
	KGLOG_PROCESS_ERROR(pNpcTeam);

	m_pNpcTeam = pNpcTeam;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL KAIBase::SetPatrolPath(int nPatrolPathID, int nNextPointIndex)
{
	BOOL bRetCode = FALSE;
	KNpc* pNpc = NULL;

	KGLOG_PROCESS_ERROR(m_pSelf);
	KGLOG_PROCESS_ERROR(m_pSelf->m_pScene);
	KGLOG_PROCESS_ERROR(nNextPointIndex >= 0);
	KG_PROCESS_ERROR(IS_NPC(m_pSelf->m_dwID));

	pNpc = (KNpc*)(m_pSelf);
	pNpc->m_AIData.m_nPatrolPathID = nPatrolPathID;
	m_pPatrolPath = g_pSO3World->m_Settings.m_PatrolPathList.GetPatrolPath(pNpc->m_pScene->m_dwMapID, nPatrolPathID);
	KGLOG_PROCESS_ERROR(m_pPatrolPath);

	m_nNextPartolIndex = nNextPointIndex;

	bRetCode = TRUE;
Exit0:
	return bRetCode;
}

KNpcTeam* KAIBase::GetNpcTeam()
{
    return m_pNpcTeam;
}

#endif //_SERVER

BOOL KAIBase::LoadParamFromTemplate()
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(m_pAIParam);

	m_eAIMainState = m_pAIParam->eAIMainState;

Exit0:
	return bRetCode;
}

BOOL KAIBase::SetAIState(KAI_STATE eNewState)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(eNewState >= aisInvalid);

#ifdef _SERVER
	if (m_pSelf && m_eAIState == aisReturn)
	{
		//����޵б��
		m_pSelf->m_bSystemShield = false;
	}
#endif //_SERVER

	m_eAIState = eNewState;

	bRetCode = TRUE;
Exit0:
	return bRetCode;
}

AI_TYPE KAIBase::GetAIType(void)
{
	return m_eAIType;
}

// --------------- AI״̬�л����� ---------------------------------------
void KAIBase::DoIdle(int nIdleFrame)
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Idle\n", m_pSelf->m_szName);
	SetAIInterval(1.0);
	m_pSelf->Stop();
	if (m_eAIState == aisIdle)
	{
		int nTmpFrameCount = g_pSO3World->m_nGameLoop + nIdleFrame;
		m_IdleData.nIdleFrameCount = (m_IdleData.nIdleFrameCount > nTmpFrameCount) ? m_IdleData.nIdleFrameCount : nTmpFrameCount;
	}
	else
	{
		m_IdleData.nIdleFrameCount	= g_pSO3World->m_nGameLoop + nIdleFrame;
	}
	SetAIState(aisIdle);
	OnIdle();
}

void KAIBase::DoWander()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Wander\n", m_pSelf->m_szName);
	SetAIState(aisWander);
	OnWander();
}

void KAIBase::DoPatrol()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Patrol\n", m_pSelf->m_szName);
	if (IS_NPC(m_pSelf->m_dwID))
	{
		SetAIState(aisPatrol);
		OnPartol();
	}
}

void KAIBase::DoAlert()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Alert\n", m_pSelf->m_szName);
	m_AlertData.nAlartFrame = g_pSO3World->m_nGameLoop + m_pAIParam->nAlertOverTime;

	SetAIInterval(0.5);
	SetAIState(aisAlert);
	OnAlert();
}
void KAIBase::DoFollow()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Follow\n", m_pSelf->m_szName);
	SetAIState(aisFollow);
	OnFollow();
}

void KAIBase::DoPursuit()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Pursuit\n", m_pSelf->m_szName);
	//��¼��ս��ʱ��
	m_PursuitData.nTurnToFightFrame = g_pSO3World->m_nGameLoop;
	m_PursuitData.nCallHealFrame = 0;
	m_PursuitData.dwCallByNpcID = 0;
	m_PursuitData.dwKeepDisDivisor = 1;

	SetAIInterval(0.5);
	SetAIState(aisPursuit);
	OnPursuit();
}

void KAIBase::DoKeepAway()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to KeepAway\n", m_pSelf->m_szName);
	SetAIState(aisKeepAway);
	OnKeepAway();
}

void KAIBase::DoEscape()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Escape\n", m_pSelf->m_szName);
	//���ܵĺ�������״̬������Ҫ��ͣ����������
	m_EscapeData.nEscapeIdleFrame = 0;

	m_pSelf->Stop();
	SetAIState(aisEscape);
	OnEscape();
}

void KAIBase::DoReturn()
{
	//KGLogPrintf(KGLOG_DEBUG, "[AI State] {%s} Turn to Return\n", m_pSelf->m_szName);
	SetAIInterval(1.0);
	m_ReturnData.nReturnFrame = g_pSO3World->m_nGameLoop + AI_RETURN_OVERTIME_FRAME;
	m_pSelf->Stop();
	
#ifdef _SERVER
	//������
	m_pSelf->m_ThreatList.ClearAllThreat();

	//���DEBUF
	//m_pSelf->m_BuffList.DeathClean();

	//�����޵�
	m_pSelf->m_bSystemShield = true;
#endif
	SetAIState(aisReturn);

#ifdef _SERVER
	TurnToIdle();
#endif //_SERVER

	OnReturn();
}

void KAIBase::DoWait()
{
	SetAIState(aisWait);
	OnWait();
}


// --------------- AI��Ϊ״̬���¼����麯�� ---------------------------------------

BOOL KAIBase::Init(KCharacter* pSelf)
{
	BOOL bRetCode = FALSE;

	m_nNextActiveFrame = 0;
	m_eAIType = aitInvalid;
	m_nOriginX = 0;
	m_nOriginY = 0;
	m_nOriginZ = 0;
	m_pAIParam = NULL;
	m_nAIInterval = g_pSO3World->m_Settings.m_ConstList.nAIInterval * 2;
	m_nNextIdleActionFrame = 0;
    m_nNextAStarFrame = 0;
    m_eAIMainState = aisInvalid;

    SetAIState(aisIdle);

	m_pSelf = pSelf;
	KGLOG_PROCESS_ERROR(m_pSelf);

#if defined(_SERVER)
	m_pPatrolPath = NULL;
	m_pNpcTeam = NULL;
#endif //_SERVER

	memset(&m_Params, 0, sizeof(m_Params));
	memset(m_nAIEventTimer, 0, sizeof(m_nAIEventTimer));

	//�����NPC�Ļ������ز���ģ��
	if (IS_NPC(m_pSelf->m_dwID))
	{
		KNpc* pNpc = (KNpc*)(m_pSelf);
	
		KNpcTemplate* pNpcTemplate = pNpc->m_pTemplate;
		if (pNpcTemplate == NULL)
		{
			pNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(pNpc->m_dwTemplateID);
		}
		KGLOG_PROCESS_ERROR(pNpcTemplate);

		m_pAIParam = g_pSO3World->m_Settings.m_AIParamTemplateList.GetTemplate(pNpcTemplate->nAIParamTemplateID);
		KGLOG_PROCESS_ERROR(m_pAIParam);
	}
	else
	{
		m_pAIParam = g_pSO3World->m_Settings.m_AIParamTemplateList.GetTemplate(g_pSO3World->m_Settings.m_ConstList.nPlayerAITemplateID);
		KGLOG_PROCESS_ERROR(m_pAIParam);
	}

	LoadParamFromTemplate();


	bRetCode = TRUE;
Exit0:
	return bRetCode;
}

BOOL KAIBase::UnInit(void)
{
	BOOL bRetCode = FALSE;

#ifdef _SERVER
	KNpc* pNpc = NULL;

	if (IS_NPC(m_pSelf->m_dwID))
	{
		pNpc = (KNpc*)(m_pSelf);
	}
			
	if (m_pNpcTeam && pNpc)
	{
		bRetCode = m_pNpcTeam->RemoveMember(pNpc->m_nNpcTeamIndex);
		KGLOG_CHECK_ERROR(bRetCode);
	}
	

#endif //_SERVER

	bRetCode = TRUE;
Exit0:
	return bRetCode;
}

#ifndef _SERVER
void KAIBase::Activate(){}
void KAIBase::OnIdle(){}
void KAIBase::OnWander(){}
void KAIBase::OnPartol(){}
void KAIBase::OnAlert(){}
void KAIBase::OnFollow(){}
void KAIBase::OnPursuit(){}
void KAIBase::OnKeepAway(){}
void KAIBase::OnEscape(){}
void KAIBase::OnReturn(){}
void KAIBase::OnWait(){}
void KAIBase::OnAttacked(KCharacter* pAttacker, int nDamage){}
void KAIBase::OnClearThreat(KCharacter* pCharater){}
BOOL KAIBase::OnTryMove(int nDstCellHeight, BOOL bIsWater){ return TRUE;}
void KAIBase::OnPathResult(BOOL bSucceed){}
#endif //_CLIENT


#ifdef _SERVER

void KAIBase::Activate(void)
{
	KG_PROCESS_ERROR(m_pSelf);

	if (m_eAIType <= aitInvalid || m_eAIType >= aitTotal)
		return;

	if (g_pSO3World->m_nGameLoop < m_nNextActiveFrame)
		return;

	m_nNextActiveFrame = g_pSO3World->m_nGameLoop + m_nAIInterval;

	CheckAIEvent();
    // Npc�����ڽű��б�ɾ��,���Ե��ýű�֮��Ҫ�ж���
	KG_PROCESS_ERROR(m_pSelf->m_pScene && m_pSelf->m_pCell && m_pSelf->m_eMoveState != cmsOnDeath);

	if (IS_NPC(m_pSelf->m_dwID))
	{
		if (m_pSelf->m_bFightState)
		{
			//ս��״̬��
		}
		else
		{
			//��ս��״̬��
			//����NpcDialog��Idle�¼�
			if (m_nNextIdleActionFrame < g_pSO3World->m_nGameLoop)
			{
				//-------- AI Event:Idle ---------------------------------
				KTarget rTarget;
				rTarget.ClearTarget();

				g_pSO3World->m_Settings.m_SmartDialogList.FireEvent((KNpc*)m_pSelf, ntatIdle, rTarget);
				//--------------------------------------------------------

				m_nNextIdleActionFrame = g_pSO3World->m_nGameLoop + g_pSO3World->m_Settings.m_ConstList.nNpcIdleActionInterval;
			}
		}
	}

	switch (m_eAIState)
	{    
	case aisIdle:
		OnIdle();
		break;
	case aisWander:
		OnWander();
		break;
	case aisPatrol:
		OnPartol();
		break;
	case aisAlert:
		OnAlert();
		break;
	case aisFollow:
		OnFollow();
		break;
	case aisPursuit:
		OnPursuit();
		break;
	case aisKeepAway:
		OnKeepAway();
		break;
	case aisEscape:
		OnEscape();
		break;
	case aisReturn:
		OnReturn();
		break;
	case aisWait:
		OnWait();
		break;
	default:
		KGLOG_PROCESS_ERROR(FALSE);
		break;
	}
	return;
Exit0:
	return;
}

void KAIBase::OnIdle(void)
{
	BOOL bRetCode	= FALSE;

	bRetCode = CheckAttacked();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAlertRange();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

	KGLOG_PROCESS_ERROR(m_pSelf);
	
	if (g_pSO3World->m_nGameLoop >= m_IdleData.nIdleFrameCount)
	{
		//����֮ǰ��״̬�ָ�����
		switch(m_eAIMainState)
		{
        case aisInvalid:
            KGLOG_CHECK_ERROR(!"Invalid main state!");
            break;
		case aisIdle:
			//��Npcվ����ʱ��,����Npc�ķ���
			if (m_pSelf->m_eMoveState == cmsOnStand 
				&& m_pSelf->m_nFaceDirection != m_nOriginDirection) 
			{
				bRetCode = m_pSelf->Turn(m_nOriginDirection, true, true);
				KGLOG_PROCESS_ERROR(bRetCode);
			}
			break;
		case aisWander:
			DoWander();
			break;
		case aisPatrol:
			DoPatrol();
			break;
		case aisFollow:
			DoFollow();
			break;
		default:
			KGLOG_CHECK_ERROR(FALSE);
			DoIdle();
			break;
		}

		return;
	}
Exit1:
	return;
Exit0:
	return;
}

void KAIBase::OnWander(void)
{
	int nRetCode = false;
	BOOL bRetCode = FALSE;

	bRetCode = CheckAttacked();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAlertRange();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

	//�й�һ�ξ���
	Wander();
Exit1:
	return ;
}

void KAIBase::OnPartol(void)
{
	
	int nRetCode = false;
	BOOL bRetCode = FALSE;

	bRetCode = CheckAttacked();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAlertRange();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

	Patrol();
Exit1:
	return ;
}

void KAIBase::OnAlert(void)
{
	KCharacter* pTarget     = NULL;
	BOOL        bRetCode    = FALSE;

    bRetCode = m_pSelf->m_SelectTarget.GetTarget(&pTarget);
    KG_PROCESS_ERROR(bRetCode && pTarget);

	if (pTarget)
	{
		//Ŀ�곬����Χ
		int nDistance2 = g_GetDistance2(m_pSelf->m_nX, m_pSelf->m_nY, pTarget->m_nX, pTarget->m_nY);

		//���ȼ���������
		int nCheckDistance2 = (int)(m_pAIParam->nAlertRange * m_pAIParam->nAlertRange);

		KG_PROCESS_ERROR(nDistance2 <= nCheckDistance2);

		//����Ŀ��
		int nDirection  = g_GetDirection(m_pSelf->m_nX, m_pSelf->m_nY, pTarget->m_nX, pTarget->m_nY);
		if (m_pSelf->m_nFaceDirection != nDirection)
		{
			m_pSelf->Turn(nDirection, true, true);
		}
	

		//Ŀ��δ��ʧ�� ���Ƿ�ʱ
		if (g_pSO3World->m_nGameLoop > m_AlertData.nAlartFrame)
		{
			//��ʱ�����й���
			m_pSelf->m_ThreatList.ModifyThreat(pTarget, 0);
			TurnToFight();
			return;
		}
	}
	
	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

	return;
Exit0:
	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);
	 
	//ʲô��û���ˣ�������״̬����ʼ����
	ClearTarget();
	DoIdle(1);

	return; 
Exit1:
	return;
}

void KAIBase::OnFollow(void)
{
	//TODO:�������������Һ�NPC
	BOOL bRetCode = FALSE;

	bRetCode = CheckAttacked();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAlertRange();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

	Follow();
Exit1:
	return;
}

void KAIBase::OnPursuit(void)
{
	int             nRetCode            = false;
	BOOL            bSkipKeepRange      = false;
	KCharacter*     pTarget             = NULL;
	KNpc*			pNpc			    = NULL;		
	KNpcTemplate*	pTemplate		    = NULL;
    int             nSelectSkillIndex   = 0;
    int             nSkillExpectation   = 0;
    DWORD           dwSkillID           = 0;
    DWORD           dwSkillLevel        = 0;
    DWORD           dwSelectSkillID     = 0;
    DWORD           dwSelectSkillLevel  = 0;
    KTarget         Target;

	KGLOG_PROCESS_ERROR(m_pSelf);
	KGLOG_PROCESS_ERROR(IS_NPC(m_pSelf->m_dwID));

	pNpc = (KNpc*)(m_pSelf);
	KGLOG_PROCESS_ERROR(pNpc);

	// ����ޱ��Ƿ�Ϊ��
	KThreatNode* pThreatNode = pNpc->m_ThreatList.GetFirstThreat();
	if (!pThreatNode)
	{
		BOOL bReturn = true;

		// �������»�Ҫ�����ѵĳ�ޱ�
		if (m_pNpcTeam)
		{
			pTarget = m_pNpcTeam->GetMemberTarget();
			if (pTarget)
			{
				pNpc->m_ThreatList.ModifyThreat(pTarget, 0);
				bReturn = false;
			}
		}
		
		if (bReturn)
		{
			AI_TRACE("��ޱ�յ�,�����˻�ȥ!");
			DoReturn();
			goto Exit1;
		}

		pThreatNode = pThreatNode = pNpc->m_ThreatList.GetFirstThreat();
		KGLOG_PROCESS_ERROR(pThreatNode);
	}

	//����Ƿ����
	CheckCallHeal();

	// ��鼼��״̬
	KG_PROCESS_SUCCESS(m_pSelf->m_OTActionParam.eActionType != otActionIdle);

    ASSERT(IS_NPC(m_pSelf->m_dwID));
	pTemplate = ((KNpc*)m_pSelf)->m_pTemplate;
	KGLOG_PROCESS_ERROR(pTemplate);

	//û����ͨ�����ͱ�ʾ���ﲻ�ܹ���,ֱ�ӷ���
	KG_PROCESS_SUCCESS((pTemplate->dwSkillIDList[0] == 0) && (pTemplate->dwSkillLevelList[0] == 0));

	// ѡ���ܺ�Ŀ��
	if (g_pSO3World->m_nGameLoop >= pNpc->m_nSkillCommomCD)
	{
		for (int nSkillIndex = MAX_NPC_AI_SKILL - 1; nSkillIndex >= 0; nSkillIndex--)
		{
			dwSelectSkillID		= pTemplate->dwSkillIDList[nSkillIndex];
			dwSelectSkillLevel	= pTemplate->dwSkillLevelList[nSkillIndex];

            if (dwSelectSkillID == 0 || dwSelectSkillLevel == 0 || g_pSO3World->m_nGameLoop < pNpc->m_nSkillCastFrame[nSkillIndex])
			{
				continue;
			}

			nSkillExpectation = GetSkillExpectation((AI_SKILL_TYPE)pTemplate->nAISkillType[nSkillIndex], (DWORD)nSkillIndex, &Target);
			if (nSkillExpectation == AI_IMMEDIATELY_CAST_EXP)
			{
				dwSkillID           = dwSelectSkillID;
				dwSkillLevel        = dwSelectSkillLevel;
				nSelectSkillIndex   = nSkillIndex;
				break;
			}
			else
			{
				if (pTemplate->nSkillCastInterval[nSkillIndex] > g_pSO3World->m_Settings.m_ConstList.nNpcCommonShortCD)
                {
                    //����û���ͷųɹ��ͽ��빫����CD
					pNpc->m_nSkillCastFrame[nSkillIndex] = g_pSO3World->m_nGameLoop + g_pSO3World->m_Settings.m_ConstList.nNpcCommonShortCD;
                }
				else
                {
					pNpc->m_nSkillCastFrame[nSkillIndex] = g_pSO3World->m_nGameLoop + pTemplate->nSkillCastInterval[nSkillIndex];
                }
			}
		}
	}

	//��ȡ����Ŀ��
	if (Target.GetTargetType() == ttNpc || Target.GetTargetType() == ttPlayer)
	{
        nRetCode = Target.GetTarget(&pTarget);
        KGLOG_PROCESS_ERROR(nRetCode && pTarget);
	}

	// ����
	if (pTarget && pTarget != m_pSelf && m_pSelf->m_eMoveState == cmsOnStand)
	{
		int nDirection  = g_GetDirection(m_pSelf->m_nX, m_pSelf->m_nY, pTarget->m_nX, pTarget->m_nY);
		if (m_pSelf->m_nFaceDirection != nDirection)
		{
			m_pSelf->Turn(nDirection, true, true);
		}
	}

	// �ͷż���
	if (dwSkillID != 0 && dwSkillLevel != 0)
	{
        KSKILL_RECIPE_KEY RecipeKey;
        KSkill* pSkill = NULL;

        nRetCode = g_pSO3World->m_SkillManager.GetSkillRecipeKey(&RecipeKey, dwSkillID, dwSkillLevel, m_pSelf);
        if (!nRetCode)
        {
            KGLogPrintf(
                KGLOG_ERR, 
                "Can't find npc skill, npc template id = %lu, skill id = %lu, skill level = %lu. Please check you config file. ", 
                pNpc->m_dwTemplateID, dwSkillID, dwSkillLevel
            );
            goto Exit0;
        }

        pSkill = g_pSO3World->m_SkillManager.GetSkill(RecipeKey);
        KGLOG_PROCESS_ERROR(pSkill);

		nRetCode = pSkill->CanCast(m_pSelf, Target);
		if (nRetCode == srcSuccess)
		{
			if (pSkill->m_nPrepareFrames > 0)
			{
				m_pSelf->Stop();
				bSkipKeepRange = TRUE;
			}

			nRetCode = m_pSelf->CastSkill(dwSkillID, dwSkillLevel, Target);
		}

		if (nRetCode == srcTooFarTarget)
		{
			m_PursuitData.dwKeepDisDivisor++;
			if (m_PursuitData.dwKeepDisDivisor > 5)
            {
				m_PursuitData.dwKeepDisDivisor = 5;
            }
		}
		else
		{
			m_PursuitData.dwKeepDisDivisor = 1;
		}

		if (nRetCode == srcSuccess)
		{
			//�����ͷųɹ���������CD,��黹û��,����Ҫ�Ⱦ��巢��ȥ�˲Ž�CD
			pNpc->m_nSkillCastFrame[nSelectSkillIndex] = g_pSO3World->m_nGameLoop + pTemplate->nSkillCastInterval[nSelectSkillIndex];
	
			//���ù���CD
			pNpc->m_nSkillCommomCD = g_pSO3World->m_nGameLoop + g_pSO3World->m_Settings.m_ConstList.nNpcSkillCommonCD;

			// �ж�Ŀ����ƶ�״̬,���³�޷�Χ
			if (pTarget && pTarget->m_nVelocityXY == 0)
			{
				m_pSelf->m_ThreatList.UpdateKeepThreatField(); //���³������
				//m_pSelf->m_ThreatList.ModifyThreat(pTarget, 0);   //���³��
				bSkipKeepRange = true;
			}
		}
		else
		{
			//����û���ͷųɹ��ͽ��빫����CD
			if (pTemplate->nSkillCastInterval[nSelectSkillIndex] > g_pSO3World->m_Settings.m_ConstList.nNpcCommonShortCD)
            {
				pNpc->m_nSkillCastFrame[nSelectSkillIndex] = g_pSO3World->m_nGameLoop + g_pSO3World->m_Settings.m_ConstList.nNpcCommonShortCD;
            }
			else
            {
				pNpc->m_nSkillCastFrame[nSelectSkillIndex] = g_pSO3World->m_nGameLoop + pTemplate->nSkillCastInterval[nSelectSkillIndex];
            }
		}
	}

	//��ȡ��һ��޵�Ŀ��
	pTarget = pThreatNode->pCharacter;
	KGLOG_PROCESS_ERROR(pTarget);

	// ά�־���
	if (!bSkipKeepRange)
	{	
        KSkill* pSkill = NULL;
        KSKILL_RECIPE_KEY RecipeKey;

        nRetCode = g_pSO3World->m_SkillManager.GetSkillRecipeKey(&RecipeKey, pTemplate->dwSkillIDList[0], pTemplate->dwSkillLevelList[0], m_pSelf);
        KGLOG_PROCESS_ERROR(nRetCode);

        pSkill = g_pSO3World->m_SkillManager.GetSkill(RecipeKey);
		KGLOG_PROCESS_ERROR(pSkill);

        if (pNpc->m_nPathFindAstar)
        {
            KeepAttackRangeAStar(pTarget, pSkill);
        }
        else
        {
            KeepAttackRange(pTarget, pSkill);
        }
	}

Exit1:
Exit0:
	return;
}

void KAIBase::OnKeepAway(void)
{

}

void KAIBase::OnEscape(void)
{
	// �����Ƿ�Ҫ����
	BOOL bRetCode = FALSE;

	KG_PROCESS_ERROR(g_pSO3World->m_nGameLoop < m_EscapeData.nEscapeFrame);

	Escape();
	return;
Exit0:
	// �����ˣ�����
	DoPursuit();
	return;
}

void KAIBase::OnReturn(void)
{
	int nRetCode = cmsInvalid;
	BOOL bRetCode = FALSE;

	bRetCode = CheckReturn();
	KG_PROCESS_SUCCESS(bRetCode);

	nRetCode = m_pSelf->m_eMoveState;

	if (nRetCode == cmsOnStand)
	{
		int nDir = g_GetDirection(m_pSelf->m_nX, m_pSelf->m_nY, m_nReturnX, m_nReturnY);
		m_pSelf->Turn(nDir, true, true);

		// �ܻ�ReturnPoint
		//KGLogPrintf(KGLOG_DEBUG, "[AI] Return Run to (%d, %d)\n", m_nReturnX, m_nReturnY);
		m_pSelf->RunTo(m_nReturnX, m_nReturnY);
	}

	return;
Exit1:
	//����ԭ���ˣ�ͣһ��
	if (m_pNpcTeam)
		DoWait();
	else
		DoIdle(GAME_FPS);

	return;
}

void KAIBase::OnWait(void)
{
	BOOL bRetCode = FALSE;

	KGLOG_PROCESS_ERROR(m_pSelf);

	bRetCode = CheckAttacked();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAlertRange();
	KG_PROCESS_SUCCESS(bRetCode);

	bRetCode = CheckTargetInAttackRange();
	KG_PROCESS_SUCCESS(bRetCode);

	// ���С��״̬,����Npcһ��û�б�Ҫ����wait
	if (m_pNpcTeam && m_pNpcTeam->m_pLeader == m_pSelf)
	{
		bRetCode = m_pNpcTeam->CheckNpcWorkDone();
		if (bRetCode)
		{	
			int nIndex = 0;
			int nMemberCount = m_pNpcTeam->GetMemberCount();

			SetAIState(m_eAIMainState);

			//����С�ӳ�Ա�Ĺ����Ѿ����,����֮ǰ��״̬�ָ�����
			for (nIndex = 0; nIndex < nMemberCount; nIndex++)
			{	
				KNpc* pNpc = m_pNpcTeam->GetMember(nIndex);
				if (pNpc && pNpc != m_pSelf)
				{
					KAI_STATE eAiMainState = pNpc->m_AIController.GetMainState();
					SetAIState(eAiMainState);
				}
			}
		}
	}
Exit1:
	return;
Exit0:
	return;
}

BOOL KAIBase::OnTryMove(int nRelativeZ, BOOL bIsWater)
{
	BOOL bResult = FALSE;
	
	KG_PROCESS_ERROR(!bIsWater); // ��ʱ��������ˮ

	KG_PROCESS_ERROR(m_pSelf);
	KG_PROCESS_ERROR(m_pSelf->m_pCell);

	if (nRelativeZ > g_pSO3World->m_Settings.m_ConstList.nAIRelativeZ)
		goto Exit0; //���ߵ����²�������ȥ

	bResult = TRUE;
Exit0:
	return bResult;
}

void KAIBase::OnAttacked(KCharacter* pAttacker, int nDamage)
{
	BOOL bRetCode = FALSE;
	int nMoveState = 0;

	KG_PROCESS_ERROR(m_pSelf);

    ASSERT(pAttacker == NULL || pAttacker->m_dwID != m_pSelf->m_dwID);

	nMoveState = m_pSelf->getMoveState();
	KG_PROCESS_ERROR(nMoveState != cmsOnDeath);

	switch(m_eAIState)
	{
	case aisIdle:
	case aisWander:
	case aisPatrol:
	case aisAlert:
	case aisFollow:
	case aisWait:
		TurnToFight();
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

void KAIBase::OnClearThreat(KCharacter* pCharater)
{
	BOOL bResult = FALSE;

	bResult = IS_PLAYER(m_pSelf->m_dwID);
	KG_PROCESS_SUCCESS(bResult == TRUE);

	// Npc�������ʱ,��������ճ��
	if (m_pNpcTeam)
	{
		KNpc* pNpc = NULL;
		int nIndex = 0;
		int nMemberCount = m_pNpcTeam->GetMemberCount();

		for (nIndex = 0; nIndex < nMemberCount; nIndex++)
		{
			pNpc = m_pNpcTeam->GetMember(nIndex);
			if (pNpc && pNpc != m_pSelf && pNpc != pCharater)
			{
				bResult = pNpc->m_ThreatList.ClearThreat(pCharater);
				KGLOG_PROCESS_ERROR(bResult);
			}
		}
	}

Exit1:
	return;
Exit0:
	return;
}

void KAIBase::OnPathResult(BOOL bSucceed)
{
	BOOL bRetCode = FALSE;
	int nMoveState = 0;

	KG_PROCESS_ERROR(m_pSelf);

	nMoveState = m_pSelf->getMoveState();
	KG_PROCESS_ERROR(nMoveState != cmsOnDeath);

	//KGLogPrintf(KGLOG_DEBUG, "[AI] path event : %d , frame : %d\n", bSucceed, g_pSO3World->m_nGameLoop);

	switch (m_eAIState)
	{
	case aisIdle:        
		break;
	case aisWander:
		if (bSucceed)
		{
			// �й�һ�ξ��룬��һ�´�
			DoIdle(m_pAIParam->nWanderInterval);
		}
        else
        {
            // ǿ�������ߡ�
            Wander(true);
        }
		break;
	case aisPatrol:
		if (bSucceed)
		{
			// ����ɹ���,��һ�������˵
			if (m_pNpcTeam && m_pNpcTeam->m_pLeader != m_pSelf)
			{
				//��Npc�������Ҳ���Leader
				OnPartol();
			}
			else
			{
				int nPatrolID = ((KNpc*)m_pSelf)->m_AIData.m_nPatrolPathID;
				KG_PROCESS_ERROR(nPatrolID);

				KPatrolPath* pPatrolPath = g_pSO3World->m_Settings.m_PatrolPathList.GetPatrolPath(m_pSelf->m_pScene->m_dwMapID, nPatrolID);
				int nRestFrame = 0;
				int nFaceTo = -1;

				if(pPatrolPath)
				{
					KPatrolNode rPatrolNode;
					int nIndex = 0;
					int nMemberCount = 0;

					if (m_pNpcTeam)
					{
						nMemberCount = m_pNpcTeam->GetMemberCount();
						for (nIndex = 0; nIndex < nMemberCount; nIndex++)
						{
							KNpc* pMember = m_pNpcTeam->GetMember(nIndex);
							if (pMember && pMember->m_pScene)
							{
								pMember->m_AIController.SetPatrolPath(nPatrolID, m_nNextPartolIndex + 1);
							}
						}
					}
					else
					{
						m_nNextPartolIndex++;
					}

					bRetCode = pPatrolPath->GetPoint(m_nNextPartolIndex - 1, rPatrolNode);
					if (bRetCode)
					{
						nRestFrame = rPatrolNode.nRestFrame;
						nFaceTo = rPatrolNode.nFaceTo;
						
						//����NpcѰ·����Ľű�
						if (rPatrolNode.szScriptName[0] != '\0')
						{
							bRetCode= g_pSO3World->m_ScriptCenter.IsScriptExist(rPatrolNode.szScriptName);
							if (bRetCode && g_pSO3World->m_ScriptCenter.IsFuncExist(rPatrolNode.szScriptName, SCRIPT_ON_PATROL))
							{
								int nTopIndex = 0;
								g_pSO3World->m_ScriptCenter.SafeCallBegin(&nTopIndex);

								g_pSO3World->m_ScriptCenter.PushValueToStack((KNpc*)m_pSelf);
								g_pSO3World->m_ScriptCenter.PushValueToStack(m_nNextPartolIndex);	//Lua��������1��ʼ��ţ����Բ���1

								g_pSO3World->m_ScriptCenter.CallFunction(rPatrolNode.szScriptName, SCRIPT_ON_PATROL, 0);
								g_pSO3World->m_ScriptCenter.SafeCallEnd(nTopIndex);
							}
						}
					}
					KG_PROCESS_ERROR(m_pSelf->m_pScene && m_pSelf->m_pCell && m_pSelf->m_eMoveState != cmsOnDeath);  //Npc�����ڽű��б�ɾ��,���Ե��ýű�֮��Ҫ�ж���

					if (m_nNextPartolIndex >= pPatrolPath->GetPointCount())
						m_nNextPartolIndex = 0;
				}

				if (nFaceTo >= 0 && nRestFrame > 0)
				{
					m_pSelf->Turn(nFaceTo, true, true);
				}
				DoIdle(nRestFrame);
			}
		}
		else
		{
			/*
			// ���Ѱ·ʧ����,�򻻸���������
			int nDirection = m_pSelf->m_nFaceDirection + 40;
			if (nDirection >= DIRECTION_COUNT)
				nDirection -= DIRECTION_COUNT;
			KGLOG_PROCESS_ERROR(nDirection >= 0 && nDirection < DIRECTION_COUNT);
			m_pSelf->Turn(nDirection);
			DoPatrol();
			*/

		}
		break;
	case aisAlert:
		break;
	case aisFollow:
		if (bSucceed)
		{
			OnFollow();
		}
		break;
	case aisPursuit:
		if (bSucceed)
		{
			OnPursuit();
		}
		else
		{
			//For Debug
			//KGLogPrintf(KGLOG_DEBUG, "[AI] ׷����Ѱ·ʧ��\n");
		}
		break;
	case aisEscape:
		if (bSucceed)
		{
			// ����Ŀ���,��һ��,������
			m_EscapeData.nEscapeIdleFrame = g_pSO3World->m_nGameLoop + m_pAIParam->nEscapeInterval;
			m_pSelf->Stop();
			OnEscape();
		}
		break;
	case aisReturn:
		if (!bSucceed)
		{
			//����ʱѰ·ʧ�ܣ�����
			bRetCode = IS_PLAYER(m_pSelf->m_dwID);
			KG_ASSERT_EXIT(!bRetCode);
			KG_ASSERT_EXIT(m_pSelf->m_pScene);

			m_pSelf->MoveTo(m_nReturnX, m_nReturnY, m_nReturnZ);

			m_ReturnData.nReturnFrame = -1;
			m_pSelf->Stop();
			m_pSelf->m_ThreatList.ClearAllThreat();
		}
		else
		{
			m_ReturnData.nReturnFrame = -1;
		}
		break;
	case aisWait:
		break;
	default:
		KGLOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:
	return;
}

#endif //_SERVER
