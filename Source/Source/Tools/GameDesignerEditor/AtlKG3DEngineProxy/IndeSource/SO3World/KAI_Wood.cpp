#include "stdafx.h"

#include "KAI_Wood.h"
#include "KThreatList.h"
#include "KCharacter.h"
#include "KSO3World.h"

#ifdef _SERVER
void KAI_Wood::Activate(void)
{
	if (m_eAIType <= aitInvalid || m_eAIType >= aitTotal)
		return;

	if (g_pSO3World->m_nGameLoop < m_nNextActiveFrame)
		return;

	m_nNextActiveFrame = g_pSO3World->m_nGameLoop + g_pSO3World->m_Settings.m_ConstList.nAIInterval;

	CheckAIEvent();
	KG_PROCESS_ERROR(m_pSelf->m_pScene && m_pSelf->m_pCell && m_pSelf->m_eMoveState != cmsOnDeath);  //Npc�����ڽű��б�ɾ��,���Ե��ýű�֮��Ҫ�ж���

	//ľ׮ֻ����һ��״̬
	OnIdle();

	return;
Exit0:
	return;
}
	
void KAI_Wood::OnIdle()
{
	
}

void KAI_Wood::OnWander()
{

}

void KAI_Wood::OnPartol()
{

}

void KAI_Wood::OnAlert()
{

}

void KAI_Wood::OnFollow()
{

}

void KAI_Wood::OnPursuit()
{

}

void KAI_Wood::OnKeepAway()
{

}

void KAI_Wood::OnEscape()
{
	
}

void KAI_Wood::OnReturn()
{

}

void KAI_Wood::OnWait()
{

}

void KAI_Wood::OnAttacked(KCharacter* pAttacker, int nDamage)
{
    m_pSelf->m_ThreatList.UpdateKeepThreatField();
}

void KAI_Wood::OnPathResult(BOOL bSucceed)
{

}

#endif //_SERVER
