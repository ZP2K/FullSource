#include "stdafx.h"
#include "KMissile.h"

#include "KMath.h"
#include "KRegion.h"
#include "KSO3World.h"

//���к����ķ���ֵ����������˵��������ѭ��TRUE���ӵ���������FALSE���ӵ�������
#define KG_CHECK_VANISH(Condition)	if (!(Condition)) goto ProcessVanish;

KMissile::KMissile(void)
{
	//����Ϊ������岻���������ٶȵ�Ӱ��
	m_nMass = 0;
}

KMissile::~KMissile(void)
{
	return;
}

BOOL KMissile::Init(void)
{
	BOOL bRetCode = FALSE;

	bRetCode = KSceneObject::Init();
	KGLOG_PROCESS_ERROR(bRetCode);

	m_nExistingFrame = 0;

	m_nCollidedTimes = 0;
	m_nCollideInterval = 0;		
	m_nCollideFrame = 0;
	m_bCollideFriend = FALSE;

	m_nDamageRadius = 0;

	m_pLauncher = NULL;
	m_dwLauncherID = 0;

	m_pSkill = NULL;
	m_pSubSkill = NULL;
	m_nFlyEventInterval = 0;
	m_nFlyEventFrame = 0;
	m_bCollideEvent = FALSE;
	m_bVanishEvent = FALSE;

	return TRUE;
Exit0:

	return FALSE;
}

BOOL KMissile::UnInit(void)
{
	BOOL bRetCode = FALSE;

	//ɾ���ӵ��ϵ������б�
	KAttribute* pAttribute = m_pAttribute;
	while (pAttribute)
	{
		KAttribute* pNext = pAttribute->m_pNext;

		bRetCode = g_SO3World.m_ObjectManager.m_AttributeSet.Delete(pAttribute);
		if (!bRetCode)
			KGLogPrintf(KGLOG_ERR, "Remove attribute from missile failed.");

		pAttribute = pNext;
	}

	m_pAttribute = NULL;

	return KSceneObject::UnInit();
}

//������ײ�Ĺ���
//1	ÿ����Ϸѭ����������ײһ��
//2 ÿ����Ϸѭ����ÿ������������ײһ��
BOOL KMissile::Activate(void)
{
	BOOL bRetCode = FALSE;

	//���ø���ͬ������
	bRetCode = KSceneObject::CheckGameLoop();
	if (!bRetCode)
		return TRUE;

	//�Ƿ���Ҫ���ӵ����������е����������Ӽ���
	if (m_nFlyEventInterval && m_nFlyEventFrame >= m_nExistingFrame)
	{
		bRetCode = OnEvent();
		KG_CHECK_VANISH(bRetCode);
		m_nFlyEventFrame -= m_nFlyEventInterval;
	}

	//���ʣ�µĸ��ӵ���ײ��˳��ʵ����ÿ����Ϸѭ�����ټ��һ����ײ�Ĺ��򣺣�
	if (m_nCollideInterval == 0)
	{
		//û�м����ÿ֡�������ײ
		bRetCode = EnumDamageRange();
		KG_CHECK_VANISH(bRetCode);
	}
	else
	{
		//�����Լ����ײ
		if (m_nCollideFrame >= m_nExistingFrame)
		{
			bRetCode = EnumDamageRange();
			KG_CHECK_VANISH(bRetCode);
			m_nCollideFrame -= m_nCollideInterval;
		}
	}

	//���֡��Ϊ�㣬������
	m_nExistingFrame--;
	KG_CHECK_VANISH(m_nExistingFrame > 0);

	return TRUE;

ProcessVanish:
	OnVanish();

	return 	TRUE;
}

//
BOOL KMissile::EnumDamageRange()
{
	BOOL bResult = FALSE;
	KRegion* pRegion = NULL;

	ASSERT(m_nDamageRadius > 0);

	bResult |= EnumObject(m_pRegion);

	for (int nIndex = 0; nIndex < ALL_DIRECTION; nIndex++)
	{
		pRegion = m_pRegion->m_pConnectRegion[nIndex];
		bResult |= EnumObject(pRegion);
	}

	return bResult;
}

BOOL KMissile::InDamageRange(KSceneObject *pSceneObject)
{
	BOOL bResult = FALSE;
	int nDirection = g_GetDirection(m_nX, m_nY, pSceneObject->m_nX, pSceneObject->m_nY);
	int nDistance2 = g_GetDistance2(m_nX, m_nY, pSceneObject->m_nX, pSceneObject->m_nY);

	if (nDistance2 > (m_nDamageRadius * m_nDamageRadius))
		goto Exit0;

	if (nDirection < m_nStartAngle)
		goto Exit0;

	if (nDirection > m_nEndAngle)
		goto Exit0;

	bResult = TRUE;
Exit0:
	return bResult;
}

BOOL KMissile::EnumObject(KRegion *pRegion)
{
	BOOL bRetCode = FALSE;
	KSceneObjNode* pNode = NULL;
	KPlayer* pPlayer = NULL;
	KNpc* pNpc = NULL;
	KDoodad* pDoodad = NULL;
		
	//���Player
	pNode = (KSceneObjNode*)pRegion->m_PlayerList.GetHead();
	while (pNode != NULL)
	{
		pPlayer = (KPlayer*)pNode->m_pSceneObject;
		KG_PROCESS_ERROR(pPlayer);

		bRetCode = InDamageRange(pNode->m_pSceneObject);
		pNode = (KSceneObjNode*)pNode->GetNext();
		if (!bRetCode)
			continue;

		bRetCode = pPlayer->OnCollided(this);
		if (bRetCode)
		{
			m_nCollidedTimes--;
			KG_CHECK_VANISH(m_nCollidedTimes > 0);
		}
	}

	//���Npc
	pNode = (KSceneObjNode*)pRegion->m_NpcList.GetHead();
	while (pNode != NULL)
	{
		pNpc = (KNpc*)pNode->m_pSceneObject;
		KG_PROCESS_ERROR(pNpc);

		bRetCode = InDamageRange(pNode->m_pSceneObject);
		pNode = (KSceneObjNode*)pNode->GetNext();
		if (!bRetCode)
			continue;

		bRetCode = pNpc->OnCollided(this);
		if (bRetCode)
		{
			m_nCollidedTimes--;
			KG_CHECK_VANISH(m_nCollidedTimes > 0);
		}
	}

	//���Doodad
	pNode = (KSceneObjNode*)pRegion->m_DoodadList.GetHead();
	while (pNode != NULL)
	{
		pDoodad = (KDoodad*)pNode->m_pSceneObject;
		KG_PROCESS_ERROR(pDoodad);

		bRetCode = InDamageRange(pNode->m_pSceneObject);
		pNode = (KSceneObjNode*)pNode->GetNext();
		if (!bRetCode)
			continue;

		bRetCode = pDoodad->OnCollided(this);
		if (bRetCode)
		{
			m_nCollidedTimes--;
			KG_CHECK_VANISH(m_nCollidedTimes > 0);
		}
	}

Exit0:
ProcessVanish:
	return TRUE;
}


//��������
void KMissile::OnVanish(void)
{
	BOOL bRetCode = FALSE;

	//����Ƿ���Ҫ�����Ӽ���
	if (m_bVanishEvent)
		OnEvent();

	//ɾ��Region�е��ӵ�
	KGLOG_PROCESS_ERROR(m_pRegion);

	bRetCode = m_pRegion->RemoveMissile(this);
	KGLOG_PROCESS_ERROR(bRetCode);

	//ɾ���ӵ�����
	bRetCode = UnInit();
	KGLOG_PROCESS_ERROR(bRetCode);

	g_SO3World.m_ObjectManager.m_MissileSet.Delete(this);

Exit0:

	return;  
}

//�Ӽ����¼�
//����FALSE��ʾ�ӵ���ʧ
BOOL KMissile::OnEvent(void)
{

	return TRUE;
}

BOOL KMissile::OnCollided(KMissile* pMissile)
{

	return FALSE;
}

