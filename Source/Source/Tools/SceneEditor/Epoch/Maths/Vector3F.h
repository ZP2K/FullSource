// File: Vector3F.h
// Desc: 3D float vector

#ifndef VECTOR3F_H
#define VECTOR3F_H

#include"MathsCommon.h"

namespace Maths
{

class DLLAPI Vector3F
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};

public:
	Vector3F(float fx = 0.f, float fy = 0.f, float fz = 0.f) : x(fx), y(fy), z(fz) {}
	Vector3F(const Vector3F& in): x(in.x), y(in.y), z(in.z) {}
	Vector3F& operator = (const Vector3F& in);
	~Vector3F() {}

	Vector3F(const D3DVECTOR& dv): x(dv.x), y(dv.y), z(dv.z) {}
	Vector3F(const D3DXVECTOR3& dxv3): x(dxv3.x), y(dxv3.y), z(dxv3.z) {}

	Vector3F& operator = (const D3DVECTOR& dv);
	Vector3F& operator = (const D3DXVECTOR3& dxv3);

	operator D3DVECTOR ();
	operator D3DXVECTOR3 () { return D3DXVECTOR3(x, y, z); }

	operator D3DVECTOR* ();
	operator D3DXVECTOR3* ();
	operator D3DVECTOR& ();
	operator D3DXVECTOR3& ();

	void ToD3DVECTOR(D3DVECTOR& dv) const;
	void ToD3DXVECTOR3(D3DXVECTOR3& dv) const;
	
	Vector3F& operator += (const Vector3F& in);
	Vector3F& operator -= (const Vector3F& in);
	Vector3F& operator *= (const float& in);
	Vector3F& operator /= (const float& in);
	Vector3F operator - () const;

	void AssignFromSphere(float theta, float rho, float phi);  // ���������괴������

	float Square() const;  // ����x��y��zƽ����
	float Length() const;  // ���������ĳ��ȡ���Ҳ����x��y��zƽ���͵�ƽ����
	void Normalize();  // ��׼��
	bool IsNormalized() const;

	void AddRespectively(const Vector3F& v, float tx, float ty, float tz);

	static const Vector3F Zero();
	static const Vector3F XAxis();
	static const Vector3F YAxis();
	static const Vector3F ZAxis();
};


// ******
// ��Vector3F�йص�ȫ�ֺ�����

inline const Vector3F operator + (const Vector3F& a, const Vector3F& b)
{
	return Vector3F(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline const Vector3F operator - (const Vector3F& a, const Vector3F& b)
{
	return Vector3F(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline const Vector3F operator * (const Vector3F& a, float b)
{
	return Vector3F(a.x * b, a.y * b, a.z * b);
}

inline const Vector3F operator * (float a, const Vector3F& b)
{
	return Vector3F(a * b.x, a * b.y, a * b.z);
}

inline const Vector3F operator / (const Vector3F& a, float b)
{
	float inv = 1.f / NotZero(b);

	return Vector3F(a.x * inv, a.y * inv, a.z * inv);
}

// ������˺���
// �õ�����һ��ͬʱ��ֱ��a��b������(��Ϊc)����������ָָ��b����Ĵָָ��a���������ĳ���ķ������c�ķ���
inline const Vector3F operator ^ (const Vector3F& a, const Vector3F& b)
{
	return Vector3F(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// ������˺�����������һ��Ϊĳƽ��ı�׼����������һ��Ϊƽ����һ�㣬����֮������ƽ�浽ԭ��ľ��뼴Ϊ�㵽��ľ��롣
// ����һ��Ϊƽ����һ�㣬����֮��Ϊ��ƽ�浽ԭ��ľ���
inline const float operator * (const Vector3F& a, const Vector3F& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// ���
inline bool operator == (const Vector3F& a, const Vector3F& b)
{
	return (FEqual(a.x, b.x) && FEqual(a.y, b.y) && FEqual(a.z, b.z));
}

inline bool operator != (const Vector3F& a, const Vector3F& b)
{
	return (FNotEqual(a.x, b.x) || FNotEqual(a.y, b.y) || FNotEqual(a.z, b.z));
}

inline bool operator > (const Vector3F& a, const Vector3F& b)
{
	return (FCompare(a.x, b.x) == 1 && FCompare(a.y, b.y) == 1 && FCompare(a.z, b.z) == 1);
}

inline bool operator < (const Vector3F& a, const Vector3F& b)
{
	return (FCompare(a.x, b.x) == -1 && FCompare(a.y, b.y) == -1 && FCompare(a.z, b.z) == -1);
}

inline bool operator >= (const Vector3F& a, const Vector3F& b)
{
	return (a > b)? true: (a == b);
}

inline bool operator <= (const Vector3F& a, const Vector3F& b)
{
	return (a < b)? true: (a == b);
}

inline float DistanceBetweenPoints(const Vector3F& a, const Vector3F& b)  // ����֮��ľ���
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;

	return sqrtf(dx * dx + dy * dy + dz * dz);
}

inline void SetMin(const Vector3F& a, const Vector3F& b, Vector3F& vOut)
{
	vOut.x = (a.x < b.x)? a.x: b.x;
	vOut.y = (a.y < b.y)? a.y: b.y;
	vOut.z = (a.z < b.z)? a.z: b.z;
}

inline void SetMax(const Vector3F& a, const Vector3F& b, Vector3F& vOut)
{
	vOut.x = (a.x > b.x)? a.x: b.x;
	vOut.y = (a.y > b.y)? a.y: b.y;
	vOut.z = (a.z > b.z)? a.z: b.z;
}

// ��Ա����

inline Vector3F& Vector3F::operator = (const Vector3F& in)
{
	if(&in != this)
	{
		x = in.x; y = in.y; z = in.z;
	}
	
	return *this;
}

inline Vector3F& Vector3F::operator += (const Vector3F& in)
{
	x += in.x;
	y += in.y;
	z += in.z;

	return *this;
}

inline Vector3F& Vector3F::operator -= (const Vector3F& in)
{
	x -= in.x;
	y -= in.y;
	z -= in.z;

	return *this;
}

inline Vector3F& Vector3F::operator *= (const float& in)
{
	x *= in;
	y *= in;
	z *= in;

	return *this;
}

inline Vector3F& Vector3F::operator /= (const float& in)
{
	float inv = 1.f / NotZero(in);
	x *= inv;
	y *= inv;
	z *= inv;

	return *this;
}

inline Vector3F Vector3F::operator - () const
{
	return Vector3F(-x, -y, -z);
}

inline void Vector3F::AssignFromSphere(float theta, float rho, float phi)
{
	x = (float)(phi * cos(rho) * sin(theta));
	y = (float)(phi * sin(rho));
	z = (float)(phi * cos(rho) * cos(theta));
}

inline float Vector3F::Square() const
{
	return x * x + y * y + z * z;
}

inline float Vector3F::Length() const
{
	return sqrtf(Square());
}

inline void Vector3F::Normalize()
{
	float inv = 1.f / NotZero(Length());

	x *= inv; y *= inv; z *= inv;
}

inline bool Vector3F::IsNormalized() const
{
	return FEqual(Square(), 1.f);
}

inline void Vector3F::AddRespectively(const Vector3F& v, float tx, float ty, float tz)
{
	x += (v.x - x) * tx;
	y += (v.y - y) * ty;
	z += (v.z - z) * tz;
}

}


#endif