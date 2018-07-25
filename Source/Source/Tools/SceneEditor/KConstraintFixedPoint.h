#pragma once
#include "KConstraint.h"

namespace Physics
{

class KConstraintFixedPoint :
	public KConstraint
{
public:
	KConstraintFixedPoint(void);
	~KConstraintFixedPoint(void);
	int Initialise(LPRIGIDBODY pRigid1, D3DXVECTOR3 Rigid1Pos, LPRIGIDBODY pRigid2, D3DXVECTOR3 Rigid2Pos, float MaxDist );
    int PreApply(float TinyTime);
	BOOL Apply(float TinyTime);

private:
	LPRIGIDBODY m_pRigid1;		//Լ����1
	LPRIGIDBODY m_pRigid2;		//Լ����2
	D3DXVECTOR3 m_Rigid1Pos;	//Լ����1��Լ����(�������������ϵ)
	D3DXVECTOR3 m_Rigid2Pos;	//Լ����2��Լ����(�������������ϵ)
	float m_fMaxDist;			//��Լ��������������

//	D3DXVECTOR3 m_WorldPos;
	D3DXVECTOR3 m_R1;			//Լ����1��Լ����(����ھֲ�����ϵ)
	D3DXVECTOR3 m_R2;			//Լ����2��Լ����(����ھֲ�����ϵ)
	D3DXVECTOR3 m_CurRelPos1;   //Լ����1��Լ������Լ����2��Լ����Ĳ�ֵ(�������������ϵ)

};

};