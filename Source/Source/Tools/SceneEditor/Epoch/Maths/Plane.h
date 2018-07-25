// File: Plane.h
// Desc:

#ifndef PLANE_H
#define PLANE_H

#include"Vector3F.h"
#include"Polygon.h"
#include"Sphere.h"
#include"LineSeg.h"
#include"AxisAlignedBoundingBox.h"

#include<vector>
using std::vector;

namespace Maths
{

class DLLAPI Plane
{
public:
	Vector3F m_vNormal;  // ƽ��ķ����������ġ�ǰ�������󡱵�˵���Դ�Ϊ����
	float m_fDistanceToOrigin;  // ƽ�浽����ԭ��ľ���

public:
	Plane(): m_vNormal(1.f, 0.f, 0.f), m_fDistanceToOrigin(0.f) {}
	Plane(float fx, float fy, float fz, float fd, bool bNormalized = true);
	Plane(const Vector3F& vNormal, float fDistanceToOrigin): m_vNormal(vNormal), m_fDistanceToOrigin(fDistanceToOrigin) {}
	Plane(const Plane& p): m_vNormal(p.m_vNormal), m_fDistanceToOrigin(p.m_fDistanceToOrigin) {}
	Plane& operator = (const Plane& p);
	~Plane() {}

	// ����һЩʵ�õĹ��캯����
	Plane(const Vector3F& a, const Vector3F& b, const Vector3F& c);  // 3��(�����ߵ�)�ռ�� ȷ��һ��ƽ�棻ע�⴫�������˳���������ƽ��ĳ���
	Plane(const Vector3F& vNormal, const Vector3F& vPoint);  // һ���������ͷ�������һ�� ȷ��һ��ƽ��
	Plane(const Polygon<Vector3F>& polygon);  // һ��Polygon ȷ��һ��ƽ��

	// ��λ������
	RelativeLocation TestPoint(const Vector3F& mvPoint) const;  // �㶨λ
	RelativeLocation TestPolygon(const Polygon<Vector3F>& mpPolygon) const;  // ����(�����)��λ
	RelativeLocation TestPointList(const Vector3F* pmvPointList, int iNum) const;  // ���ж�λ
	RelativeLocation TestSphere(const Sphere& msSphere) const;  // ��λ
	RelativeLocation TestSphere(const Vector3F& Location, float SquareOfRadius) const;  // ��λ
	RelativeLocation TestAABB(const AxisAlignedBoundingBox& aabb) const;  // AABB��λ
	RelativeLocation TestAABB(const vector<Vector3F>& vCorners) const;  // AABB��λ

	Vector3F SplitLine(const Vector3F& a, const Vector3F& b) const;  // ���߶�ab��ƽ��Ľ���
	Vector3F ProjectionPoint(const Vector3F& v) const;  // ��ռ���һ����ƽ���ϵ�ͶӰ��
	float DistanceToPoint(const Vector3F& vPoint) const;  // ��ƽ�浽�ռ�һ��ľ���

	bool Clip(const Polygon<Vector3F>& polyIn, Polygon<Vector3F>* ppolyOut) const;  // �ü�����������

	// ���ڴ���BSP
	bool Split(const Polygon<Vector3F>& mpIn, Polygon<Vector3F>* pmpFront, Polygon<Vector3F>* pmpBack) const;
	bool Split(const LineSeg& mlsIn, LineSeg* pmlsFront, LineSeg* pmlsBack) const;
	bool Split(const vector<Vector3F>& vIn, vector<Vector3F>& vFront, vector<Vector3F>& vBack) const;

	void Flip();  // ��ת��ƽ��
};

// ȫ�ֺ�����

inline bool operator == (const Plane& a, const Plane& b)
{
	if(FastFabs(a.m_fDistanceToOrigin - b.m_fDistanceToOrigin) < EPSILON)
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
	if(FastFabs(a.m_fDistanceToOrigin - b.m_fDistanceToOrigin) > EPSILON || a.m_vNormal != b.m_vNormal)
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

inline Plane::Plane(const Vector3F& a, const Vector3F& b, const Vector3F& c)
{
	m_vNormal = (b - a) ^ (c - a);
	m_vNormal.Normalize();

	m_fDistanceToOrigin = - (m_vNormal * a);
}

inline Plane::Plane(const Vector3F& mvNormal, const Vector3F& mvPoint): m_vNormal(mvNormal), m_fDistanceToOrigin(-(m_vNormal * mvPoint))
{
}

inline Plane::Plane(const Polygon<Vector3F>& mpPolygon)
{
	Vector3F a = mpPolygon.m_vList[0];
	Vector3F b = mpPolygon.m_vList[1];
	Vector3F c = mpPolygon.m_vList[2];

	m_vNormal = (b - a) ^ (c - a);
	m_vNormal.Normalize();

	m_fDistanceToOrigin = - (m_vNormal * a);
}

inline RelativeLocation Plane::TestPoint(const Vector3F& mvPoint) const
{
	float dp = mvPoint * m_vNormal + m_fDistanceToOrigin;

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

inline Vector3F Plane::SplitLine(const Vector3F& a, const Vector3F& b) const
{
	float aDot = a * m_vNormal;
	float bDot = b * m_vNormal;

	float scale = (-m_fDistanceToOrigin - aDot) / (bDot - aDot);

	return (a + (scale * (b - a)));
}

inline Vector3F Plane::ProjectionPoint(const Vector3F& mv) const
{
	float fDistToPlane = mv * m_vNormal;

	return Vector3F(mv - (m_fDistanceToOrigin + fDistToPlane) * m_vNormal);
}

inline float Plane::DistanceToPoint(const Vector3F& mvPoint) const
{
	return FastFabs(mvPoint * m_vNormal + m_fDistanceToOrigin);
}

inline void Plane::Flip()
{
	m_vNormal = - m_vNormal;
	m_fDistanceToOrigin = - m_fDistanceToOrigin;
}

}

#endif