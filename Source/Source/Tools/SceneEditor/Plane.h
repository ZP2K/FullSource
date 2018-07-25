// File: Plane.h
// Desc:

#ifndef PLANE_H
#define PLANE_H

#ifndef EPSILON
#define EPSILON 0.00001f  // �����жϸ������Ƿ����0
#endif

enum RelativeLocation
{
	RL_FRONT,  // ��ƽ����˵��ǰ���԰�Χ��(��)��˵����
	RL_BACK,  // ��ƽ����˵�Ǻ󣬶԰�Χ��(��)��˵����
	RL_SPLIT,  // �ཻ
	RL_COPLANAR  // ����
};

#include<vector>
using std::vector;


inline bool FEqualZero(const float& f)
{
	return (f >= -EPSILON)? (f <= EPSILON): false;
}

class Plane
{
public:
	D3DXVECTOR3 m_vNormal;  // ƽ��ķ����������ġ�ǰ�������󡱵�˵���Դ�Ϊ����
	float m_fDistanceToOrigin;  // ƽ�浽����ԭ��ľ���

public:
	Plane(): m_vNormal(1.f, 0.f, 0.f), m_fDistanceToOrigin(0.f) {}
	Plane(float fx, float fy, float fz, float fd, bool bNormalized = true);
	Plane(const D3DXVECTOR3& vNormal, float fDistanceToOrigin): m_vNormal(vNormal), m_fDistanceToOrigin(fDistanceToOrigin) {}
	Plane(const Plane& p): m_vNormal(p.m_vNormal), m_fDistanceToOrigin(p.m_fDistanceToOrigin) {}
	Plane& operator = (const Plane& p);
	~Plane() {}

	// ����һЩʵ�õĹ��캯����
	Plane(const D3DXVECTOR3& a, const D3DXVECTOR3& b, const D3DXVECTOR3& c);  // 3��(�����ߵ�)�ռ�� ȷ��һ��ƽ�棻ע�⴫�������˳���������ƽ��ĳ���
	Plane(const D3DXVECTOR3& vNormal, const D3DXVECTOR3& vPoint);  // һ���������ͷ�������һ�� ȷ��һ��ƽ��
	//Plane(const Polygon<D3DXVECTOR3>& polygon);  // һ��Polygon ȷ��һ��ƽ��

	// ��λ������
	RelativeLocation TestPoint(const D3DXVECTOR3& mvPoint) const;  // �㶨λ
	//RelativeLocation TestPolygon(const Polygon<D3DXVECTOR3>& mpPolygon) const;  // ����(�����)��λ
	///RelativeLocation TestPointList(const D3DXVECTOR3* pmvPointList, int iNum) const;  // ���ж�λ
	//RelativeLocation TestSphere(const Sphere& msSphere) const;  // ��λ
	//RelativeLocation TestSphere(const D3DXVECTOR3& Location, float SquareOfRadius) const;  // ��λ
	//RelativeLocation TestAABB(const AxisAlignedBoundingBox& aabb) const;  // AABB��λ
	//RelativeLocation TestAABB(const vector<D3DXVECTOR3>& vCorners) const;  // AABB��λ

	D3DXVECTOR3 SplitLine(const D3DXVECTOR3& a, const D3DXVECTOR3& b) const;  // ���߶�ab��ƽ��Ľ���
	D3DXVECTOR3 ProjectionPoint(const D3DXVECTOR3& v) const;  // ��ռ���һ����ƽ���ϵ�ͶӰ��
	float DistanceToPoint(const D3DXVECTOR3& vPoint) const;  // ��ƽ�浽�ռ�һ��ľ���

	//bool Clip(const Polygon<D3DXVECTOR3>& polyIn, Polygon<D3DXVECTOR3>* ppolyOut) const;  // �ü�����������

	// ���ڴ���BSP
	//bool Split(const Polygon<D3DXVECTOR3>& mpIn, Polygon<D3DXVECTOR3>* pmpFront, Polygon<D3DXVECTOR3>* pmpBack) const;
	//bool Split(const LineSeg& mlsIn, LineSeg* pmlsFront, LineSeg* pmlsBack) const;
	bool Split(const vector<D3DXVECTOR3>& vIn, vector<D3DXVECTOR3>& vFront, vector<D3DXVECTOR3>& vBack) const;

	void Flip();  // ��ת��ƽ��
};

// ȫ�ֺ�����

inline bool operator == (const Plane& a, const Plane& b)
{
	if(fabs(a.m_fDistanceToOrigin - b.m_fDistanceToOrigin) < EPSILON)
	{
		if(a.m_vNormal == b.m_vNormal)
		{
			return true;
		}
	}

	return false;
}

inline bool operator != (const Plane& a, const Plane& b)
{
	if(fabs(a.m_fDistanceToOrigin - b.m_fDistanceToOrigin) > EPSILON || a.m_vNormal != b.m_vNormal)
	{
		return true;
	}

	return false;
}

// ��Ա������

inline Plane& Plane::operator = (const Plane& mp)
{
	if(&mp != this)
	{
		m_vNormal = mp.m_vNormal;
		m_fDistanceToOrigin = mp.m_fDistanceToOrigin;
	}
	
	return *this;
}

inline Plane::Plane(const D3DXVECTOR3& a, const D3DXVECTOR3& b, const D3DXVECTOR3& c)
{
	D3DXVec3Cross(&m_vNormal, &(b - a), &(c - a));
	D3DXVec3Normalize(&m_vNormal, &m_vNormal);

	m_fDistanceToOrigin = - D3DXVec3Dot(&m_vNormal, &a);
}

inline Plane::Plane(const D3DXVECTOR3& mvNormal, const D3DXVECTOR3& mvPoint): m_vNormal(mvNormal), m_fDistanceToOrigin(-D3DXVec3Dot(&m_vNormal, &mvPoint))
{
}

//inline Plane::Plane(const Polygon<D3DXVECTOR3>& mpPolygon)
//{
//	D3DXVECTOR3 a = mpPolygon.m_vList[0];
//	D3DXVECTOR3 b = mpPolygon.m_vList[1];
//	D3DXVECTOR3 c = mpPolygon.m_vList[2];
//
//	m_vNormal = (b - a) ^ (c - a);
//	m_vNormal.Normalize();
//
//	m_fDistanceToOrigin = - (m_vNormal * a);
//}

inline RelativeLocation Plane::TestPoint(const D3DXVECTOR3& mvPoint) const
{
	float dp = D3DXVec3Dot(&mvPoint, &m_vNormal) + m_fDistanceToOrigin;

	if(dp > EPSILON)
	{
		return RL_FRONT;
	}
	else if(dp < -EPSILON)
	{
		return RL_BACK;
	}
	else
	{
		return RL_COPLANAR;
	}
}

inline D3DXVECTOR3 Plane::SplitLine(const D3DXVECTOR3& a, const D3DXVECTOR3& b) const
{
	float aDot = D3DXVec3Dot(&a, &m_vNormal);
	float bDot = D3DXVec3Dot(&b, &m_vNormal);

	float scale = (-m_fDistanceToOrigin - aDot) / (bDot - aDot);

	return (a + (scale * (b - a)));
}

inline D3DXVECTOR3 Plane::ProjectionPoint(const D3DXVECTOR3& mv) const
{
	float fDistToPlane = D3DXVec3Dot(&mv, &m_vNormal);

	return D3DXVECTOR3(mv - (m_fDistanceToOrigin + fDistToPlane) * m_vNormal);
}

inline float Plane::DistanceToPoint(const D3DXVECTOR3& mvPoint) const
{
	return fabs(D3DXVec3Dot(&mvPoint, &m_vNormal) + m_fDistanceToOrigin);
}

inline void Plane::Flip()
{
	m_vNormal = - m_vNormal;
	m_fDistanceToOrigin = - m_fDistanceToOrigin;
}



#endif