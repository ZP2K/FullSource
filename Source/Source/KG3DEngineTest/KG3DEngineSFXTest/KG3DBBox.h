////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DBBox.h
//  Version     : 1.0
//  Creator     : 
//  Create Date : 2008-8-14 10:41:24
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DBBOX_H_
#define _INCLUDE_KG3DBBOX_H_

////////////////////////////////////////////////////////////////////////////////
class BBox
{
public:
	D3DXVECTOR3    A;
	D3DXVECTOR3    B;
	BOOL           bInit;

	BBox();
	BBox(D3DXVECTOR3 PosA,D3DXVECTOR3 PosB);

	void operator = (const BBox& Box);
	BOOL operator == (const BBox& Box);
	BBox& operator+= (const BBox& oth);

	//FRECT ToRect();

	D3DXVECTOR3 GetCenter() const;
	float GetLength()const;
	float GetWidth()const;
	float GetHeight()const;

	void Clear();

	float GetDiameter()const;//�õ�ֱ����ԭ����GetRadius�������õ�����ֱ��

	void AddPosition(D3DXVECTOR3 Pos);
	void AddPosition(D3DXVECTOR3* Pos,int nNum);
	void AddPosition(D3DXVECTOR3 PosA,D3DXVECTOR3 PosB,D3DXMATRIX * pMatrix);

	void AddY(FLOAT Y);//����Y�����ϳſ�Box
	void AddY(const BBox& Box);

	void Extract(D3DXVECTOR3* pPositions)const;

	void GetPlane(D3DXPLANE* pPlane);

    void Transform(const D3DXMATRIX& mat);
	void TransformCoord(D3DXVECTOR3* pPositions, const D3DXMATRIX * pMatrix)const;//չ��8�㲢��Matrix�任

	void Project(D3DXVECTOR3* pPositions, D3DVIEWPORT9 *pViewport, D3DXMATRIX *pProjection,
				D3DXMATRIX *pView, D3DXMATRIX *pWorld);

	bool IsIntersectRect(D3DXVECTOR2& vA,D3DXVECTOR2& vC);
	bool IsIntersectLine2D(float a, float b, float c, D3DXVECTOR2& v1, D3DXVECTOR2& v2);
	bool IsIntersectLine2D(float A,float B,float C);
	bool IsTwoBoxIntersect(const BBox &aabb);

	void GetXZRect(D3DXVECTOR2& vA,
		D3DXVECTOR2& vC);

	bool IsPointInside(const D3DXVECTOR3& vec3Point);

	bool IsLineSectionIntersect(const D3DXVECTOR3& vec3Start,
		const D3DXVECTOR3& vec3End);

	bool IsFaceIntersect(const D3DXVECTOR3& vA, 
		const D3DXVECTOR3& vB,	const D3DXVECTOR3& vC);

	HRESULT ComputeOrthoCameraWithLight(D3DXVECTOR3& vCameraPos,D3DXVECTOR3& vCameraFront,
		float& fWidth,float& fHeight,float& fFar ,
		const D3DXVECTOR3& vLightDirction);

	BOOL IsPointInside2D(float fx,float fz);

	BOOL IsSorted();
	BOOL IsPointInsideSorted(const D3DXVECTOR3& vPoint);

	BOOL IsRayIntersect3D(D3DXVECTOR3& Inter,const D3DXVECTOR3& rStartPos,const D3DXVECTOR3& rDir,D3DXMATRIX* pMatrix = NULL,float* pfOutHitDis = NULL);//�ɵĴ��룬�Ժ���Ҫ���ˣ�����RayIntersection��

	BOOL RayIntersection(const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir, FLOAT* pfDistance, const D3DXMATRIX* pMatrix = NULL);//�����Matrix����ô�൱��Box�任���Ǹ�Matrix��Ȼ���ټ��㡣����������Box�ڲ�����ô���ǻ�õ�����ġ��������Ҫ�����������ôֻҪ�ٶ�vSrc�ж�IsPointInside�����ˡ�
	//BOOL RayIntersection2D(const D3DXVECTOR2& vSrc2D, const D3DXVECTOR2& vNormalizedDir2D, FLOAT* pfDistance);
private:
	BOOL RayIntersectionPrivate(const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir, FLOAT* pfDistance, BOOL bCheckZ);//��������Z�Ļ����ͱ��2D�ཻ
};



typedef BBox* LPAABBOX;
typedef BBox AABBOX;

#endif //_INCLUDE_KG3DBBOX_H_
