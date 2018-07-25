#include <list>
#include <vector>
#include "TimePoint.h"

#ifndef _TYPES_H
#define _TYPES_H
using namespace std;
namespace ModelSFX
{

enum FieldType
{
	F_CENTR_SPHERE,//��Ӱ���ٶȣ� ֻ��Ӱ���䷽��
	F_CENTR_CYLINDER,//Drag Field
	F_DRAG,
	F_GRAVITY,//Ӱ��һ��������ٶ�
	F_VOTEX,
	F_VOTEX1,
	F_RADIAL
};

enum BarrierType
{
	B_TREFLECT,
	B_TTRIGGER,
	B_TDESTRUCT

};
enum FieldShape
{
	F_SPHERE,
	F_CYLINDER,
	F_BOX
};

enum BarrierShape
{
	B_SPHERE,
	B_CYLINDER,
	B_PLANE,
	B_TRIANGLELIST,
	B_TRIANGLESTRIP
};

enum LifeSpanMode
{
	LSM_LIVE_FOR_EVER,
	LSM_CONSTANT,
	LSM_RANDOM_RANGE
};

enum ParticleRenderType
{
	PR_NUMERIC,//?
	PR_POINTS,//OK
	PR_SPRITES,//OK
	PR_STREAK//?
};

enum ParticleType
{
	PT_VANISH,
	PT_TIGGER
};

enum ParticleShape
{
	PS_POINT,
	PS_BILLBOARD
};

struct CYLINDER
{
	D3DXVECTOR3 BtCenter;
	D3DXVECTOR3 UpCenter;
	float Radius;
};

struct SPHERE
{
	D3DXVECTOR3 Center;
	float Radius;
};
struct Particle
{
	D3DXVECTOR3 m_vec3VotexInc;//spcially for rotation force velocity increasement
	float m_fVotexInc;
	bool m_bInsideVotex;
	int m_iForceMask[100]; //max force field count 100
	D3DXVECTOR3 m_vDiffuse;
	D3DXVECTOR3	m_vVelocity;//���ӵ��ٶ�
	D3DXVECTOR3 m_vPosition;//���ӵ�λ��
	ParticleType m_eType;
	float m_fLifeSpan;//���ӵ�����ʱ��
	float m_fLastUpdate;//�ϴθ���ʱ��
	float m_fStartTime;//�������ɵ�ʱ��
	float m_fScaleVel;//���ӷ����ٶ�
	float m_fRadius[2];//�뾶��Height 0, Width 1��
	D3DCOLOR m_cColor;//���ӵ���ɫ
	float m_fRotSpd;//�����ٶ�
	float m_fRot;//�����ĽǶ�
	float m_fConserve;
	Particle()
	{
		memset(m_iForceMask, 0, sizeof(m_iForceMask));
		m_fVotexInc = 0.0f;
		m_bInsideVotex = false;
	}
};

//struct ForceField
//{
//	//D3DXVECTOR3	m_vVelocity;//�������ٶ�
//	//D3DXVECTOR3 m_vPosition;//�������ĵ�λ��
//	//BOUNDING_BOX m_bbBox;//�����Ŀռ����÷�Χ;ΪNULL����ȫ�ռ�
//	D3DXVECTOR3 m_vAcc;//�����ļ��ٶ�,������������� ��ôֻȡ.x��Ϊ��ת�Ľ��ٶ�
//	FieldType	m_eType;//��������״		
//	FieldShape m_eShape;
//	//float m_fScaleVel;//���������ٶ�
//	bool	m_bEffective;
//	float	m_fForcePar[10];
//};




//struct Barrier
//{
//	BarrierType m_btType;
//	BarrierShape m_btShape;//�赲�����״
//	float m_fBarPar[20];
//	bool m_bEffective;
//};
//	
//LAUNCHER_SHAPE�Ƿ���������״
enum LauncherShape
{	
	L_POINT,
	L_LINE_LIST,
	L_LINE_STRIP,
	L_CIRCLE_FACE,
	L_CIRCLE,
	L_RECTANGLE,
	L_RECTANGLE_FACE,
	L_SPHERE_SURFACE,
	L_TRIANGLE_LIST,
	L_TRIANGLE_STRIP,
	L_USER_DEFINE//�û����巢������״
};

enum LauncherType
{
	LT_DIRECTIONAL,
	LT_OMNI,
	LT_CURVE,
	LT_SURFACE
};

};
#endif