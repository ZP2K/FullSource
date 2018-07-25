// KPhysicsSimulator.h: interface for the KPhysicsSimulator class.
//

///////////////////////////////////////////////////////////////////////////////////////////////////
//˵��:KPhysicsSimulator����ϵͳ��,ֻ��һЩ��ŵĽ���
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//	����������ײ�����㷨��Discrete Methods
//	��ײ��Ӧ���㷨��"Impulse-based Dynamic Simulation"
//
//  �ܵ���˵��Щ�㷨��Ч����ߵ��㷨,��Ȼ���ܲ��ܴ���ĳЩ������ײ���
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	SpcyIntersect()��������ʵ��������֮����ཻ����
//	SpcyPlaneIntersect()��������ʵ�ָ�����ĳƽ��֮����ཻ����
//
//	PreProcessCollision()����������ײ��Ӧ��Ԥ����
//	ProcessCollision()����������ײ��Ӧ�Ĵ���
//
//	DoTimeStep()��������������ײ������ײ��Ӧ�Ĵ���
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	KConstraintFixedPoint���ǽ�������������Լ������,ֻ�Ǽ򵥵������˶�����,���Ի�δ���ϵ�������ϵͳ����,������Ҫ,�������Ͻ�����
///////////////////////////////////////////////////////////////////////////////////////////////////
//	������ڵ�����
//	1.����,���廹�ǿ��ܻ�����Ƕ�������,������һЩ����,��Ч�������Ǻܺ�,Ŀǰ���붼�ѱ�����,
//	  �ɲο��Ĵ�����SeparateObjects(),HandleAllConstraints()�������ɱ���m_bNonePenetration�����Ĵ���,Ŀǰ���ѱ�����
//
//	2.����,�������ͼ����ײ�����Ҫ��������
//    ��ΪĿǰʵ�ֵ�ֻ�Ǹ�����ƽ����ײ��Ч��
//
//	3.�ཻ������,��Ϊ��ײ���Ƕ����ȶ��Ǽ������һ������ֵ,�����ܼ����һ����ȷֵ,���Ӱ�쵽����ϵͳ��Ч��
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KPHYSICSSIMULATOR_H__AEF21872_9FF4_4BB9_83A8_201EE068F248__INCLUDED_)
#define AFX_KPHYSICSSIMULATOR_H__AEF21872_9FF4_4BB9_83A8_201EE068F248__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./KRigidBody.h"
//#include ".\KConstraintBodyBodyMaxDist.h"
//#include ".\KConstraintBodyBodyPoint.h"
#include ".\KConstraintFixedPoint.h"
#include ".\kcollisioninfo.h"
//#include ".\kcollisionutil.h"

namespace Physics
{
class KPhysicsSimulator  
{
public:
	
	DWORD m_dwLastTime;
public:
	HRESULT Render();
	HRESULT FrameMove();

	HRESULT AddRigidBody(LPRIGIDBODY pRigid);
	HRESULT RemoveRigidBody(LPRIGIDBODY pRigid);

	HRESULT AddConstraint(KConstraint* pConstraint);
	HRESULT RemoveConstraint(KConstraint* pConstraint);

	KPhysicsSimulator();
	virtual ~KPhysicsSimulator();

	BOOL CheckCollision(LPRIGIDBODY pRigid1, LPRIGIDBODY pRigid2);


	list<LPRIGIDBODY>listRigidBody;		//�������еĸ���
	list<KConstraint*>listConstraint;	//�������е�Լ��
	list<D3DXPLANE> listPlane;			//�������е�ƽ��

	BOOL m_bAddingGravity;		//TRUEΪ��������״̬, FALSEΪ����������״̬
	BOOL m_bUsingFriction;		//TRUEΪ����Ħ��������,FALSEΪ������Ħ��������
//	BOOL m_bNonePenetration;
	float m_fEpsilon;			//��ײ�ָ�ϵ��,0~1֮��,1Ϊ��ȫ������ײ,0Ϊ��ȫ�ǵ�����ײ
	
private:

	typedef struct _AABB
	{
		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
	}* LPAABB;

	struct StoredData		//�洢��ײ��Ϣ�Ľṹ
	{
		StoredData(LPRIGIDBODY pRigid)
		{ 
			m_Position = pRigid->m_Position;
			m_LineVel = pRigid->m_LineVel;
			m_AngVel = pRigid->m_AngVel;
			m_matOrient = pRigid->m_matOrient;

		}
		StoredData(){};
		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_LineVel;
		D3DXVECTOR3 m_AngVel;	
		D3DXMATRIX m_matOrient;
	};

	vector<StoredData> vecStoredData;
	vector<KCollisionInfo>vecCollisionInfo;



private:

	BOOL CheckVetexStatus( const D3DXVECTOR3& normal, const D3DXVECTOR3& point, const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2);
//	BOOL CheckCollisionWithVect(LPRIGIDBODY pRigid1, LPRIGIDBODY pRigid2, D3DXVECTOR3& vect );
	BOOL CheckSphereIntersect( const D3DXVECTOR3& vec1,  float r1, const D3DXVECTOR3& vec2,  float r2);
	BOOL SphereAABBIntersect( const D3DXVECTOR3& ct,  float r, const LPAABB A);
	BOOL AABBIntersect( const LPAABB A, const LPAABB B);
    BOOL CreateAABB(LPAABB A, KRigidBody::LPSPCY pSp);

	float VertexLineDist(const D3DXVECTOR3& V0, const D3DXVECTOR3& LV1, const D3DXVECTOR3& LV2);
	float LineDist(const D3DXVECTOR3& L1V1, const D3DXVECTOR3& L1V2, const D3DXVECTOR3& L2V1, const D3DXVECTOR3& L2V2, BOOL & paral);
//	BOOL IsZeroVector(const D3DXVECTOR3& vec);
	BOOL IsAcuteAngle(const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2, const D3DXVECTOR3& vec3);
	BOOL CheckSeparateVertex(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3, const D3DXVECTOR3& v4);

	BOOL SphereSpcyIntersect(KRigidBody::LPSPCY pSp, const D3DXVECTOR3& vecCenter, float radius);
//	BOOL SphereSpcyIntersectWithVect(KRigidBody::LPSPCY pSp, const D3DXVECTOR3& vecCenter, float radius, D3DXVECTOR3& vect );

//	void CalCollisionVelocity( D3DXVECTOR3& va , float ma , D3DXVECTOR3& vb , float mb, float e);
//	int StepRigidBody(LPRIGIDBODY pRigid, float TinyTime);

//	BOOL SpcyPlaneIntersect(LPRIGIDBODY pRigid, const D3DXVECTOR3& vect, const D3DXVECTOR3& normal);
//	BOOL SpcyPlaneIntersect1(LPRIGIDBODY pRigid, const D3DXVECTOR3& point, const D3DXVECTOR3& normal, D3DXVECTOR3& inter );
	BOOL LineSegIntersect(const D3DXVECTOR3& q1, const D3DXVECTOR3& q2, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2);
//	BOOL LineSegSpcyIntersect(LPRIGIDBODY pRigid, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);
//	BOOL TriSpcyIntersect(LPRIGIDBODY pRigid, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3);
	BOOL VertexInSpcy(LPRIGIDBODY pRigid, const D3DXVECTOR3& v1);

public:
	int DisableCollision( LPRIGIDBODY pRigid1, LPRIGIDBODY pRigid2 );

private:
	BOOL IsCollidable( LPRIGIDBODY pRigid, vector<LPRIGIDBODY> & nc );
	BOOL SpcyPlaneIntersect( const LPRIGIDBODY pRigid, D3DXPLANE & Plane, D3DXVECTOR3& Inter, float& PenetrationDepth );
	BOOL SpcyIntersect( const LPRIGIDBODY pRigid1, const LPRIGIDBODY pRigid2, D3DXVECTOR3& Inter, D3DXVECTOR3& DirTo1, float& PenetrationDepth );
	BOOL SpcySphereIntersect( const LPRIGIDBODY pRigid1, const LPRIGIDBODY pRigid2, D3DXVECTOR3& Inter, D3DXVECTOR3& DirTo1, float& PenetrationDepth );//Rigid2��Sphere
//  BOOL SphereIntersect( const LPRIGIDBODY pRigid1, const LPRIGIDBODY pRigid2, D3DXVECTOR3& Inter, D3DXVECTOR3& DirTo1, float& PenetrationDepth );//Rigid1,Rigid2��Sphere

	int UpdateAllVelocities(float Tinytime);
	int UpdateAllPositions(float Tinytime);
	int ClearAllForces(void);
	int GetAllForces(void);
	int DetectAllCollisions(void);
	int DetectCollisions( void );
	int DetectHeightMapCollision( LPRIGIDBODY pRigid, D3DXPLANE& plane);
	int DetectBodyCollision( LPRIGIDBODY pRigid1, LPRIGIDBODY pRigid2);
	int PreProcessCollision( KCollisionInfo& collision, float TinyTime );
	int ProcessCollision( KCollisionInfo& collision, float TinyTime, float epsilon );
	int HandleAllCollisions(float TinyTime);
//	int SeparateObjects( KCollisionInfo& collision, float factor );
//	int HandleAllConstraints(float TinyTime);
	int DoTimeStep(float Tinytime);

//private:
//	void CollisionSimulation( float TinyTime );
//	void SimulationTest( float TinyTime);
//	void PlaneCollisionSimulation(float Tinytime);
//	void TestConstraintMaxDist(float Tinytime);
//	void TestFixedPoint(float Tinytime);
//	void TestRotation(float TinyTime);
};

};
#endif // !defined(AFX_KPHYSICSSIMULATOR_H__AEF21872_9FF4_4BB9_83A8_201EE068F248__INCLUDED_)
