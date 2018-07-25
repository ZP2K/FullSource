#include "stdafx.h"

#include "KAIBase.h"
#include "KMath.h"
#include "KSO3World.h"

#ifdef _SERVER
void KAIBase::InitSkillCD()
{
	BOOL bRetCode = FALSE;
	KNpc* pNpc = NULL;
	int nIndex = 0;
	KNpcTemplate* pNpcTemplate = NULL;

	KGLOG_PROCESS_ERROR(m_pSelf);

	bRetCode = IS_NPC(m_pSelf->m_dwID);
	KG_PROCESS_ERROR(bRetCode);

	pNpc = (KNpc*)m_pSelf;

	pNpcTemplate = pNpc->m_pTemplate;
	if (pNpcTemplate == NULL)
	{
		pNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(pNpc->m_dwTemplateID);
		KGLOG_PROCESS_ERROR(pNpcTemplate);
	}

	for (nIndex = 0; nIndex < MAX_NPC_AI_SKILL; nIndex++)
	{
		if (pNpcTemplate->dwSkillIDList[nIndex] == 0
			|| pNpcTemplate->dwSkillLevelList[nIndex] == 0)
			break;

		switch(pNpcTemplate->nAISkillType[nIndex])
		{
		case aistPeriodSelf:
		case aistPeriodFriend:
		case aistPassiveRespond:
		case aistChannelingBreak:
		case aistTargetCount:
		case aistSelfHP:
			pNpc->m_nSkillCastFrame[nIndex] = 0; // A��CD
			break;
		case aistPeriodTarget:
			//���ܵ�CD��ʼ��Ϊ����CD��50%��100%֮��,��֤�����   C��CD
			pNpc->m_nSkillCastFrame[nIndex] = g_pSO3World->m_nGameLoop 
				+ pNpcTemplate->nSkillCastInterval[nIndex] / 2
				+ g_Random(pNpcTemplate->nSkillCastInterval[nIndex] / 2);
			break;
		//B��CD�߻���ʱû������
		default:
			KGLOG_PROCESS_ERROR(FALSE);
		    break;
		}
	}

	return;
Exit0:
	return;
}

void KAIBase::CheckAttackTarget()
{
	BOOL bRetCode = FALSE;


	KGLOG_PROCESS_ERROR(m_pSelf);

	

Exit0:
	return;
}

int KAIBase::GetSkillExpectation(AI_SKILL_TYPE eSkillType, DWORD dwSkillIndex, KTarget* pRetTarget)
{
	int nResult = 0;

	KGLOG_PROCESS_ERROR(m_pSelf);
	KGLOG_PROCESS_ERROR(eSkillType > aistInvalid && eSkillType < aistTotal);
    KGLOG_PROCESS_ERROR(pRetTarget);

	switch(eSkillType)
	{
	case aistPeriodTarget:
		nResult = GetSkillExp_PeriodTarget(dwSkillIndex, pRetTarget);
		break;
	case aistPeriodSelf:
		nResult = GetSkillExp_PeriodSelf(dwSkillIndex, pRetTarget);
		break;
	case aistPeriodFriend:
		nResult = GetSkillExp_PeriodFriend(dwSkillIndex, pRetTarget);
	    break;
	case aistPassiveRespond:
		nResult = GetSkillExp_PassiveRespond(dwSkillIndex, pRetTarget);
	    break;
	case aistChannelingBreak:
		nResult = GetSkillExp_ChannelingBreak(dwSkillIndex, pRetTarget);
		break;
	case aistTargetCount:
		nResult = GetSkillExp_TargetCount(dwSkillIndex, pRetTarget);
		break;
	case aistSelfHP:
		nResult = GetSkillExp_SelfHP(dwSkillIndex, pRetTarget);
	    break;
	default:
		KGLOG_PROCESS_ERROR(FALSE);
	    break;
	}

Exit0:
	return nResult;
}

// ---------------------------------- ����ѡ���� --------------------------------------

//1.����CD:C��CD
//2.�ͷ�ʱ��:һ��CD���˾ͻ᳢���ͷż���
//3.�ͷ�Ŀ��:��д���ܵ�Ŀ���(*)ȷ��
//4.�ͷż���:��д���ʱ���
//5.�����ͷ�:
//	a)	�����ͷųɹ���ʼ����CD
//	b)	�����ͷ�ʧ�ܺ�,���Լ����n��
int KAIBase::GetSkillExp_PeriodTarget(DWORD dwSkillIndex, KTarget* pTarget)
{
	int  nResult     = 0;
    BOOL bRetCode    = FALSE;

    ASSERT(pTarget);

	//TODO:Ŀ���ѡ��,�������ͷ�,��ʱ�õ�һ��޴���
	KThreatNode* pThreatNode = m_pSelf->m_ThreatList.GetFirstThreat();
	KG_PROCESS_ERROR(pThreatNode);

	KCharacter* pCharacter = pThreatNode->pCharacter;
	KGLOG_PROCESS_ERROR(pCharacter);

    bRetCode = pTarget->SetTarget(pCharacter);
    KGLOG_PROCESS_ERROR(bRetCode);

	nResult = AI_IMMEDIATELY_CAST_EXP;
Exit0:
	return nResult;
}

//1.����CD:A��CD
//2.�ͷ�ʱ��:һ��CD���˾ͻ᳢���ͷż���
//3.�ͷ�Ŀ��:�ͷ����Լ�
//4.�ͷż���:��д���ʱ���
//5.�����ͷ�:
//	a)	�����ͷųɹ���ʼ����CD
//	b)	�����ͷ�ʧ�ܺ�,���Լ����n��
int KAIBase::GetSkillExp_PeriodSelf(DWORD dwSkillIndex, KTarget* pTarget)
{

	int				nResult		= 0;
	BOOL			bRetCode	= FALSE;
	int				nRandom		= 0;
	KNpcTemplate*	pNpcTemplate = NULL;
	
    ASSERT(pTarget);

    pNpcTemplate = ((KNpc*)m_pSelf)->m_pTemplate;
	if (pNpcTemplate == NULL)
	{
		pNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(((KNpc*)m_pSelf)->m_dwTemplateID);
		KGLOG_PROCESS_ERROR(pNpcTemplate);
	}
	
	//���㼸��
	nRandom = g_Random(MAX_SKILL_RATE);
	KG_PROCESS_ERROR(nRandom < pNpcTemplate->nSkillRate[dwSkillIndex]);

	bRetCode = pTarget->SetTarget(m_pSelf);
	KGLOG_PROCESS_ERROR(bRetCode);

	nResult = AI_IMMEDIATELY_CAST_EXP;
Exit0:
	return nResult;
}

//1.����CD:A��CD
//2.�ͷ�ʱ��:һ��CD���˾ͻ᳢���ͷż���
//3.�ͷ�Ŀ��:��ǰĿ��ĳ���б��е�ǰ8��Ŀ�������ȡ���ͷ�Ŀ��
//4.�ͷż���:��д���ʱ���
//5.�����ͷ�:
//	a)	�����ͷųɹ���ʼ����CD
//	b)	�����ͷ�ʧ�ܺ�,���Լ����n��
int KAIBase::GetSkillExp_PeriodFriend(DWORD dwSkillIndex, KTarget* pTarget)
{
	int				nResult		    = 0;
	BOOL			bRetCode	    = FALSE;
	int				nRandom		    = 0;
	int				nCount		    = MAX_SELECT_TARGET_COUNT;
	KCharacter*		pEnemy		    = NULL;
	KThreatNode*	pNode		    = NULL;
	KCharacter*		pCharacter	    = NULL;
	KNpcTemplate*   pNpcTemplate    = NULL;

    ASSERT(pTarget);

	pNpcTemplate = ((KNpc*)m_pSelf)->m_pTemplate;
	if (pNpcTemplate == NULL)
	{
		pNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(((KNpc*)m_pSelf)->m_dwTemplateID);
		KGLOG_PROCESS_ERROR(pNpcTemplate);
	}

	pNode = m_pSelf->m_ThreatList.GetFirstThreat();
	KG_PROCESS_ERROR(pNode);

	pEnemy = pNode->pCharacter;
	KG_PROCESS_ERROR(pEnemy);

	pNode = pEnemy->m_ThreatList.GetFirstThreat();
	while (pNode && nCount)
	{
		nRandom = g_Random(MAX_SKILL_RATE);
		if (nRandom < pNpcTemplate->nSkillRate[dwSkillIndex])
		{
			pCharacter = pNode->pCharacter;
			KG_PROCESS_ERROR(pTarget);

			break;
		}

		nCount--;
		pNode = pEnemy->m_ThreatList.GetNextNode(pNode);
	}

	if (pCharacter)
	{
		bRetCode = pTarget->SetTarget(pCharacter);
		KGLOG_PROCESS_ERROR(bRetCode);

		nResult = AI_IMMEDIATELY_CAST_EXP;
	}

Exit0:
	return nResult;
}

//1.����CD:A��CD
//2.�ͷ�ʱ��:CD��������к����ߣ���ô�Ժ������ͷż��ܡ�
//3.�ͷ�Ŀ��:������
//4.�ͷż���:��������Ӧ����
//5.�����ͷ�:
//	a)	�����ͷųɹ���ʼ����CD
//	b)	�����ͷ�ʧ�ܺ�,���Լ����n��
int KAIBase::GetSkillExp_PassiveRespond(DWORD dwSkillIndex, KTarget* pTarget)
{
	int			nResult			= 0;
	BOOL        bRetCode        = FALSE; 
	int         nMaxLife        = 0;
	int         nCurrentLife    = 0;
	double      fLostLifeRate   = 0;
	KCharacter* pCharacter      = NULL;

    ASSERT(pTarget);

    if (m_PursuitData.dwCallByNpcID && IS_NPC(m_PursuitData.dwCallByNpcID))
	{
		pCharacter = g_pSO3World->m_NpcSet.GetObj(m_PursuitData.dwCallByNpcID);
		KG_PROCESS_ERROR(pCharacter);

		//���˾Ͳ�����
		KG_PROCESS_ERROR(pCharacter->m_eMoveState != cmsOnDeath);

		//�жϾ���
		//TODO:

		nMaxLife = pCharacter->m_nMaxLife;
		nCurrentLife = pCharacter->m_nCurrentLife;
		KGLOG_PROCESS_ERROR(nMaxLife);

		fLostLifeRate = (nMaxLife - nCurrentLife) / (double)nMaxLife;

		KG_PROCESS_ERROR(fLostLifeRate >= CALL_HEAL_LIMIT);

		bRetCode = pTarget->SetTarget(pCharacter);
		KG_PROCESS_ERROR(bRetCode);

		nResult = AI_IMMEDIATELY_CAST_EXP;
	}

Exit0:
	m_PursuitData.dwCallByNpcID = 0; //��������м�¼
	return nResult;
}

//1.����CD:A��CD
//2.�ͷ�ʱ��:���Լ�����б��е�ǰ8��Ŀ��������һ����ʼ������ͨ����ʱ�򣬳����ͷ�
//3.�ͷ�Ŀ��:��������ͨ����Ŀ��
//4.�ͷż���:��д���ʱ���
//5.�����ͷ�:
//	a)	�����ͷųɹ���ʼ����CD
//	b)	�����ͷ�ʧ�ܺ�,���Լ����n��
int KAIBase::GetSkillExp_ChannelingBreak(DWORD dwSkillIndex, KTarget* pTarget)
{
    int				nResult		    = 0;
	BOOL			bRetCode	    = FALSE;
	int				nRandom		    = 0;
	int				nCount		    = MAX_SELECT_TARGET_COUNT;
	KThreatNode*	pNode		    = NULL;
	KCharacter*		pCharacter		= NULL;
	KNpcTemplate*	pNpcTemplate    = NULL;

    ASSERT(pTarget);

	pNpcTemplate = ((KNpc*)m_pSelf)->m_pTemplate;
	if (pNpcTemplate == NULL)
	{
		pNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(((KNpc*)m_pSelf)->m_dwTemplateID);
		KGLOG_PROCESS_ERROR(pNpcTemplate);
	}

	nRandom = g_Random(MAX_SKILL_RATE);
	KG_PROCESS_ERROR(nRandom < pNpcTemplate->nSkillRate[dwSkillIndex]);

	pNode = m_pSelf->m_ThreatList.GetFirstThreat();
	while (pNode && nCount)
	{
		KGLOG_PROCESS_ERROR(pNode->pCharacter);

		if (pNode->pCharacter->m_OTActionParam.eActionType == otActionSkillPrepare
			|| pNode->pCharacter->m_OTActionParam.eActionType == otActionSkillChannel
			|| pNode->pCharacter->m_OTActionParam.eActionType == otActionItemSkill)
		{
			//Ŀ�������ͷż���,�����д���
			pCharacter = pNode->pCharacter;
			break;
		}

		nCount--;
		pNode = m_pSelf->m_ThreatList.GetNextNode(pNode);
	}

	if (pCharacter)
	{
		bRetCode = pTarget->SetTarget(pCharacter);
		KGLOG_PROCESS_ERROR(bRetCode);

		nResult = AI_IMMEDIATELY_CAST_EXP;
	}

Exit0:
	return nResult;
}

//1.����CD:A��CD
//2.�ͷ�ʱ��:�������жϣ������Լ�����б��еĳ�Ա������ȷ���ͷż��ʳ����ͷ�
//3.�ͷ�Ŀ��:��д���ܵ�Ŀ���(*)ȷ��
//4.�ͷż���:��д���ʱ���
//5.�����ͷ�:
//	a)	�����ͷųɹ���ʼ����CD
//	b)	�����ͷ�ʧ�ܺ�,���Լ����n��
int KAIBase::GetSkillExp_TargetCount(DWORD dwSkillIndex, KTarget* pTarget)
{
	int             nResult         = 0;
	BOOL            bRetCode        = FALSE;
	int             nThreatCount    = 0;
	int             nCount          = MAX_SELECT_TARGET_COUNT;
	int             nRandom         = 0;
	KCharacter*     pCharacter      = NULL;
	KThreatNode*    pNode           = NULL;
	KNpcTemplate*	pNpcTemplate    = NULL;

    ASSERT(pTarget);

    pNpcTemplate = ((KNpc*)m_pSelf)->m_pTemplate;
	if (pNpcTemplate == NULL)
	{
		pNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(((KNpc*)m_pSelf)->m_dwTemplateID);
		KGLOG_PROCESS_ERROR(pNpcTemplate);
	}

	pNode = m_pSelf->m_ThreatList.GetFirstThreat();
	while (pNode && nCount)
	{
		nThreatCount++;		
		nCount--;
		pNode = m_pSelf->m_ThreatList.GetNextNode(pNode);
	}

	nRandom = g_Random(MAX_SKILL_RATE);
	KG_PROCESS_ERROR(nRandom < (nThreatCount * pNpcTemplate->nSkillRate[dwSkillIndex]));

	//TODO:Ŀ���б�δ���
	pNode = m_pSelf->m_ThreatList.GetFirstThreat();
	KG_PROCESS_ERROR(pNode);

	pCharacter = pNode->pCharacter;
	KG_PROCESS_ERROR(pCharacter);

	bRetCode = pTarget->SetTarget(pCharacter);
	KG_PROCESS_ERROR(bRetCode);

	nResult = AI_IMMEDIATELY_CAST_EXP;
Exit0:
	return nResult;
}

//1.����CD:A��CD
//2.�ͷ�ʱ��:HPֵ����ĳ���̶�ֵ��ʱ��
//3.�ͷ�Ŀ��:�Լ�
//4.�ͷż���:��д���ʱ���
//5.�����ͷ�:
//	a)	�����ͷųɹ���ʼ����CD
//	b)	�����ͷ�ʧ�ܺ�,���Լ����n��
int KAIBase::GetSkillExp_SelfHP(DWORD dwSkillIndex, KTarget* pTarget)
{
	int             nResult         = 0;
	BOOL            bRetCode        = FALSE;
	int             nMaxLife        = 0;
	int             nCurrentLife    = 0;
	double          fLifeRate		= 0;
	KNpcTemplate*	pNpcTemplate    = NULL;

    ASSERT(pTarget);

	pNpcTemplate = ((KNpc*)m_pSelf)->m_pTemplate;
	if (pNpcTemplate == NULL)
	{
		pNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(((KNpc*)m_pSelf)->m_dwTemplateID);
		KGLOG_PROCESS_ERROR(pNpcTemplate);
	}

	nMaxLife = m_pSelf->m_nMaxLife;
	nCurrentLife = m_pSelf->m_nCurrentLife;
	KGLOG_PROCESS_ERROR(nMaxLife);

	fLifeRate = (nCurrentLife) / (double)nMaxLife;

	KG_PROCESS_ERROR((int)(fLifeRate * 1024) < pNpcTemplate->nSkillRate[dwSkillIndex]);

	bRetCode = pTarget->SetTarget(m_pSelf);
	KG_PROCESS_ERROR(bRetCode);

	nResult = AI_IMMEDIATELY_CAST_EXP;
Exit0:
	return nResult;
}

#endif //_SERVER
