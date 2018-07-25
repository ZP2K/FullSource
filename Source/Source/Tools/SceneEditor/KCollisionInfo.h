#pragma once
namespace Physics
{
class KCollisionInfo
{
public:
	KCollisionInfo(void);
	KCollisionInfo(	LPRIGIDBODY pRigid1, LPRIGIDBODY pRigid2, D3DXPLANE* pPlane, const D3DXVECTOR3& DirTo1, const D3DXVECTOR3& ColPosition, float PenetrationDepth );
	~KCollisionInfo(void);

	LPRIGIDBODY m_pRigid1;		//��ײ��1
	LPRIGIDBODY m_pRigid2;		//��ײ��2
	D3DXPLANE* m_pPlane;		//��ײƽ��
	
	D3DXVECTOR3 m_DirTo1;		//��ײ����, ָ����ײ��1
	D3DXVECTOR3 m_ColPosition;	//��ײ��
	float m_fPenetrationDepth;	//Ƕ�����

	D3DXVECTOR3 m_R1;			//�������ײ��1����ײ��
	D3DXVECTOR3 m_R2;			//�������ײ��2����ײ��
	
	float m_VrExtra;			

	float m_fStaticFriction;	//��Ħ��ϵ��
	float m_fDynamicFriction;	//��Ħ��ϵ��

	float m_fDenominator;		//��ײ�����ķ�ĸ
};

};