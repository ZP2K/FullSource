// KRigidBody.h: interface for the KRigidBody class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//˵��:KRigidBody������,ֻ��һЩ��ŵĽ���
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//����Ҫ��Ҫע��m_fInertiaת�������ļ�����,���ʹ����һ����ǡ����ֵ,��Ӱ�쵽����ϵͳ��Ч��
//
//Ŀǰֻ����һ������������ʶת������,�����ȡ�ø��õ�Ч��,���Կ���ʹ��һ��������
//�����ά��ת��������Ϣ,�ٶԴ�����һЩ����
///////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KRIGIDBODY_H__7C732D4B_5BFA_482D_ACDC_D65D77C84F88__INCLUDED_)
#define AFX_KRIGIDBODY_H__7C732D4B_5BFA_482D_ACDC_D65D77C84F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Physics
{

//#define USING_FRICTION

typedef class KRigidBody  
{
public:

	typedef struct _SPCY
	{
		//������뾶
		D3DXVECTOR3 CtrPosition;	//���ĵ�����
		D3DXVECTOR3 Normal;			//������
		float		Height;			//Բ�����߶�
		float		Radius;			//Բ�����Բ��뾶
		
	}*LPSPCY;

	LPSPCY pSp;


	D3DXVECTOR3 m_Position; //��������
	float       m_fMass; //����
	float		m_fInvMass; //�����ĵ���
	float		m_fInertia;//ת������
	float		m_fInvInertia;//ת�������ĵ���

	D3DXVECTOR3 m_LineVel;		//�������ٶ�	
	D3DXVECTOR3 m_AngVel;		//���ٶ�
//	D3DXVECTOR3 m_RotAxis;      
//	D3DXVECTOR3 m_LineAcc;       //�߼��ٶ�
	D3DXVECTOR3 m_Force;         //����
	D3DXVECTOR3 m_Torque;        //������

	D3DXMATRIX m_matOrient;      //����ֲ�����ϵ�� ��Ӧ������
//	D3DXMATRIX m_matInertia;
//	D3DXMATRIX m_matInvInertia;
//	D3DXMATRIX m_matWorldI;
//	D3DXMATRIX m_matWorldInvI;

//#ifdef USING_FRICTION
	float m_fStaticFriction; //��Ħ��ϵ��
	float m_fDynamicFriction; //��Ħ��ϵ��
//#endif

	//BOOL m_bImmovable; //�����ƶ�������

private:
	D3DXVECTOR3 m_RelPos;  //pSp���ĵ�����������
	D3DXVECTOR3 m_RelNormal; //pSp->normal���������

	vector<int>vecCollision;  //��¼���������ײ�ĸ���
	vector<KRigidBody*>vecNonCollidable; //�����뱾������ײ�ĸ����б�
	
public:
	KRigidBody();
	virtual ~KRigidBody();

//	int Initialise( D3DXVECTOR3 Pos, float fMass, float fInertia, D3DXMATRIX matOrient );
	int Initialise( D3DXVECTOR3 Pos, float fMass, float fInertia );	
	int Initialise( D3DXVECTOR3 Pos, float fMass );	
	int SetSP(D3DXVECTOR3 Pos, D3DXVECTOR3 Normal, float Height, float Radius);
	int SetMass(float mass);
	int SetInertia(float inertia);
//	int SetInertia(float Ixx, float Iyy, float Izz);
	int ApplyWorldImpulse(const D3DXVECTOR3& Impulse);
	int ApplyWorldAngImpulse(const D3DXVECTOR3& Impulse);
	int ApplyWorldImpulse(const D3DXVECTOR3& Impulse, const D3DXVECTOR3& Pos);
	int UpdateVelocity( float TinyTime, float factor = 0.0f );
	int UpdatePostion( float TinyTime );
	int ClearForce(void);

	vector<int>& Collision(){ return vecCollision; };
	vector<KRigidBody*>& NonCollidable(){ return vecNonCollidable; };

}*LPRIGIDBODY;

};
#endif // !defined(AFX_KRIGIDBODY_H__7C732D4B_5BFA_482D_ACDC_D65D77C84F88__INCLUDED_)
