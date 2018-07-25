/************************************************************************/
/* �ӵ�			                                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.31	Create												*/
/************************************************************************/
#ifndef _KMissile_H_
#define _KMissile_H_

#include "Global.h"
#include "KSceneObject.h"
#include "KCharacter.h"

class KSkill;
class KScene;
class KRegion;
struct KCell;

class KMissile : public KSceneObject
{
//�ӵ�����
public:
	int			m_nFrames;		//֡��

	int			m_nDamageRadius;		//�˺��뾶
	int			m_nStartAngle;
	int			m_nEndAngle;

//��������
public:
	KCharacter*	m_pLauncher;			//������ָ��
	DWORD		m_dwLauncherID;			//������ID
	DWORD		m_dwDice;				//�����ߵ���������ֵ��1��100��
	int			m_nAttackRating;		//�������ڷ����ӵ�ʱ��������
										//���ܺ�����Ŀ��ʱ�����ߵ������ʲ�ͬ�����Լ�¼���ӵ���

	KSkill*		m_pSkill;				//�����ӵ��ļ���
	KSkill*		m_pSubSkill;			//�ӵ������Ӽ���
	int			m_nFlyEventInterval;	//�����е����Ӽ��ܵ����ڣ�����Ϊ���ʾ��Ч
	int			m_nFlyEventFrame;		//�´ε����Ӽ��ܵ�֡������׼���ӵ�����Ĵ���֡��
	BOOL		m_bCollideEvent;		//�Ƿ���Ҫ����ײʱ�����Ӽ���
	BOOL		m_bVanishEvent;			//�Ƿ���Ҫ������ʱ�����Ӽ���

	SCENE_OBJ_RELATION_TYPE	m_AllowRelation;	//�ӵ��������õĹ�ϵ
public:
	KMissile(void);
	~KMissile(void);

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL Activate(void);

	//���ٵ�����Ŀ��У������
	//void AdjustDirection(void);

	//�ӵ���ײ�¼������ӵ�����
	BOOL OnCollided(KMissile* pMissile);
private:
	BOOL EnumDamageRange();
	BOOL EnumObject(KRegion *pRegion);
	BOOL InDamageRange(KSceneObject *pSceneObject);

	//�ӵ������¼�
	void OnVanish(void);
	//�Ӽ����¼�
	BOOL OnEvent(void);
};
#endif	//_KMissile_H_
