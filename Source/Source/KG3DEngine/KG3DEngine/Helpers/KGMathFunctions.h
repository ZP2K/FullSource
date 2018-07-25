////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGMathFunctions.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 17:21:37
//  Comment     : MathTool.cpp�ͺܶණ���й�ϵ�����Բ��������һ��������ϣ�
//�����ԪҪ�����ٰ�����Ķ���,MathTool.h�������
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGMATHFUNCTIONS_H_
#define _INCLUDE_KGMATHFUNCTIONS_H_
#include "KGCommonWidgets.h"

////////////////////////////////////////////////////////////////////////////////
D3DCOLORVALUE Color2Value(D3DCOLOR Color);
VOID Color2Value(D3DCOLOR Color, D3DCOLORVALUE& Value);

///�ֽڲ���
#define D3DCOLOR_GETRED(SRC)   (((SRC)&0x00ff0000)>>16)
#define D3DCOLOR_GETGREEN(SRC) (((SRC)&0x0000ff00)>> 8)
#define D3DCOLOR_GETBLUE(SRC)  ((SRC)&0x000000ff)
#define D3DCOLOR_GETALPHA(SRC) ((SRC)>>24)

#define D3DCOLOR_GETRED_FLOAT(SRC)   ((((SRC)&0x00ff0000)>>16) / 255.0f)
#define D3DCOLOR_GETGREEN_FLOAT(SRC) ((((SRC)&0x0000ff00)>> 8) / 255.0f)
#define D3DCOLOR_GETBLUE_FLOAT(SRC)  (((SRC)&0x000000ff) / 255.0f)
#define D3DCOLOR_GETALPHA_FLOAT(SRC) (((SRC)>>24) / 255.0f)
#define D3DCOLOR_SETALPHA(SRC, ALPHA) (SRC = (((SRC)&0x00ffffff)|((ALPHA)<<24)))

///ֵ���������max��min��Ĳ�ȷ����
#define KG3DMT_MAX(a, b)  (((a) > (b)) ? (a) : (b)) 
#define KG3DMT_MIN(a, b)  (((a) < (b)) ? (a) : (b))

///���Ƶ���Χ��
#define MT_LIMIT_TO_RANGE(snum, smax, smin)	\
	do	\
{	\
	_ASSERTE((smin)<=(smax));	\
	(snum) > (smax) ? (snum) = (smax) : ((snum) < (smin) ? (snum) = (smin) : NULL);	\
}while(false)
#define MT_LIMIT_D3DCOLOR_RANGE(snum)	MT_LIMIT_TO_RANGE((snum), 255, 0)

template<typename _Type>
inline BOOL MT_TInRange(_Type Object, _Type LowerBount, _Type HigherBound)///���Ͱ�ȫ�İ汾
{
	if (Object >= LowerBount && Object <= HigherBound)
	{
		return TRUE;
	}
	return FALSE;
}

#define IS_FLOAT_NAN(fNum)	(_isnan(fNum))		//����ѧ������ʱ��������޷��ڸ���ռ��ʶ���ͻ������sqrt(-1.f);������ 1.#IND000�͸���1#IND000
#define IS_FLOAT_INFINITE(fNum)	(! _finite(fNum))//��0��ʱ�򣬻�õ������� 1.#INF000���߸����� -1.#INF000

#define MT_MAX_VALUE_FLOAT	(1000000.0f)
#define MT_MIN_VALUE_FLOAT	(-MT_MAX_VALUE_FLOAT)
#define IS_FLOAT_VALID(fNum)	(! IS_FLOAT_INFINITE(fNum) && ! IS_FLOAT_NAN(fNum))	//����ͬʱ�ж�����

#define ASSERT_FLOAT_IS_VALIED(fNum) \
	ASSERT(IS_FLOAT_VALID(fNum) && _T("Float Not Valid"))

///ת��
inline DWORD FtoDW(FLOAT f){return *((DWORD*)&f);}
BOOL LPCTSTR2D3DCOLOR(LPCTSTR czSource, int nMaxStringLen, D3DCOLOR& ResultColor);//ARGB
BOOL D3DCOLOR2LPTSTR(D3DCOLOR Color, LPTSTR czResult, int nBufferSize);//ARGB
void LPCTSTR2IntInRange( LPCTSTR czSource, int* pnDes, int nLowerBound, int nUpperBound );
D3DCOLOR ColorValue(D3DCOLORVALUE co);
COLORREF ColorValue2ref(D3DCOLORVALUE co);
D3DCOLORVALUE Colorref2value(COLORREF Color,float a);
D3DCOLORVALUE Colorref2value_GDI(COLORREF Color,float a);
D3DCOLOR Value2Color(const D3DCOLORVALUE& D3DColorValue);
inline void Value2Color( const D3DCOLORVALUE &D3DColorValue, D3DCOLOR &D3DColor ){D3DColor = Value2Color(D3DColorValue);}
void Color2Ref( const D3DCOLOR &D3DColor, COLORREF &CRColor );
//void Color2Ints(const D3DCOLOR& D3DColor, int& nRed, int& nGreen, int& nBlue, int& nAlpha);

inline UCHAR D3DCOLOR2Gray( D3DCOLOR Color )
{
	INT nGray = ((D3DCOLOR_GETRED(Color))*38 + (D3DCOLOR_GETGREEN(Color))*75 + (D3DCOLOR_GETBLUE(Color))*15) >> 7;
	//INT nGray = (nR*38 + nG*75 + nB*15) >> 7;	///�Ҷ���λ��ʽ
	MT_LIMIT_D3DCOLOR_RANGE(nGray);
	return *(UCHAR*)(&nGray);
}
inline D3DCOLOR D3DCOLOR2GrayColor( D3DCOLOR Color, BOOL bAlphaTo255 = TRUE)
{
	INT nGray = ((D3DCOLOR_GETRED(Color))*38 + (D3DCOLOR_GETGREEN(Color))*75 + (D3DCOLOR_GETBLUE(Color))*15) >> 7;
	MT_LIMIT_D3DCOLOR_RANGE(nGray);
	return bAlphaTo255 ? D3DCOLOR_XRGB(nGray, nGray, nGray) : D3DCOLOR_ARGB(D3DCOLOR_GETALPHA(Color), nGray, nGray, nGray);
}
#define D3DCOLOR2INTS(color, nA, nR, nG, nB)	\
	do	\
{	\
	(nA) = D3DCOLOR_GETALPHA((color));	\
	(nR) = D3DCOLOR_GETRED((color));	\
	(nG) = D3DCOLOR_GETGREEN((color));	\
	(nB) = D3DCOLOR_GETBLUE((color));	\
} while(false)

template<size_t _Size>
inline VOID D3DCOLOR2IntArray(D3DCOLOR color, INT (&Array)[_Size])//ARGB
{
	_KG3D_DEBUG_STATIC_CHECK_T(_Size >= 4, SizeNotEnough);
	D3DCOLOR2INTS(color, Array[0], Array[1], Array[2], Array[3]);	
}

//����
VOID D3DXMatrixViewPort(const D3DVIEWPORT9& ViewPort, D3DXMATRIX& matRet);//��Viewport����Viewport����
D3DXVECTOR3& D3DXMatrixGetTranslationPart(D3DXMATRIX& mat);	//�õ������λ�õĲ��֣������Ͳ�����41,42�ĸ�ֵ�ˣ��Ƚ�����
const D3DXMATRIX& D3DXMatrixGetIdentity();
D3DXVECTOR3*	D3DXMatrixGetPosOfView(const D3DXMATRIX* pMatView, D3DXVECTOR3* pOut);//����ֵ��NULL����pOut
D3DXVECTOR3*	D3DXMatrixGetDirOfView(const D3DXMATRIX* pMatView, D3DXVECTOR3* pOut);//����ֵ��NULL����pOut
D3DXVECTOR3		D3DXMatrixGetRightOfView(const D3DXMATRIX& matView);
D3DXVECTOR3		D3DXMatrixGetUpOfView(const D3DXMATRIX& matView);
BOOL	D3DXMatrixProjIsOrtho(const D3DXMATRIX& mat);
BOOL	D3DXMatrixGetZNearFromProj(const D3DXMATRIX& mat, FLOAT& Ret);	//ȫ����������������ϵΪ׼
BOOL	D3DXMatrixGetZFarFromProj(const D3DXMATRIX& mat, FLOAT& Ret);
BOOL	D3DXMatrixProjResetZFar(D3DXMATRIX& mat, FLOAT newZFar);	//������Clipping���á�ZFar����ZNear��ʱ���ʧ��
BOOL	D3DXMatrixProjResetZNearFar(D3DXMATRIX& mat, FLOAT newZNear, FLOAT newZFar);	//������Clipping��ͬʱ������������(�÷�1.��ZNear��ZFar�ŵ������Զ�˺ͽ��ˣ�����ȥ����������������)
const D3DXVECTOR3& D3DXVec3GetNormalOfPlane(size_t i);//0,1,2�ֱ���xyz
const D3DXMATRIX& D3DXMatrixGetRotationMatrixOfAxis(size_t i);//0,1,2�ֱ���xyz���Ӧ�ģ���xzƽ���ϵ�������ת����Ӧ��ľ���

//��ά��ײ
D3DXVECTOR2 PointDistanceToLine(const D3DXVECTOR2& vP, const D3DXVECTOR2& vSrc, const D3DXVECTOR2& vNormalizedDir);//��2D���㵽�ߵ������
D3DXVECTOR3 PointDistanceToLine(const D3DXVECTOR3& vP, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir);//��3D���㵽�������

FLOAT	TriangleArea(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3);
size_t	GetVertexStrideFromFVF(DWORD FVF);//�Ӷ���FVF�õ������С�����󷵻�0

//Ͷ��ϵ��
__forceinline D3DXVECTOR3 ProjectVectorToDirection(const D3DXVECTOR3& vVec, const D3DXVECTOR3& vDir)//����Ͷ�䣬���������㣬vDirҪ�ǵ�λ����
{
	return D3DXVec3Dot(&vVec, &vDir) * vDir;
}
D3DXVECTOR3 ProjectVectorToPlane(const D3DXVECTOR3& vVec, const D3DXVECTOR3& vPlaneNormal);	//���������Ͷ�䣬vDirҪ�ǵ�λ����
D3DXVECTOR3 ProjectPointToPlane(const D3DXVECTOR3& vP, const D3DXPLANE& plane);		//�㵽���Ͷ��
D3DXVECTOR3 ProjectPointToPlane(const D3DXVECTOR3& vP, const D3DXVECTOR3& vOnePointOnPlane, const D3DXVECTOR3& vNormalOfPlane);
D3DXVECTOR3 ProjectPointToRay(const D3DXVECTOR3& vP, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vDir);	//�㵽�ߵ�Ͷ�䣬vDirҪ�ǵ�λ����

HRESULT	CalculatePickRay(const D3DXVECTOR2& vScreenPos, const D3DXMATRIX& matViewProjection
						 , const D3DVIEWPORT9& viewport, D3DXVECTOR3& vSrcRet, D3DXVECTOR3& vNormalizedDirRet);
HRESULT CalculatePickRay(const D3DXVECTOR2& vScreenPos, const D3DXMATRIX& matViewProjViewport
							, D3DXVECTOR3& vSrcRet, D3DXVECTOR3& vNormalizedDirRet);	//������View*Proj*ViewportMatrix.����ViewportMatrix������D3DXMatrixViewPort�õ�

///���Բ�ֵ
void TwoColorBlend(const D3DCOLOR& Color1, const D3DCOLOR& Color2, D3DCOLOR& ColorResult, float fPercent, bool bBlendAlpha = false);
#define	MT_INT_BLEND(n1, n2, nr, fPercent)	\
	do	\
	{	\
		if ((n1) == (n2))	\
			(nr) = (n1);	\
		else				\
			((nr) = (int)((n1)*(1-(fPercent)) + (n2)*(fPercent)));	\
	} while(false)

#define	MT_FLOAT_BLEND(f1, f2, fr, fPercent)	\
	do	\
	{	\
		if ((f1) == (f2))	\
			(fr) = (f1);	\
		else				\
			((fr) = ((f1)*(1-(fPercent)) + (f2)*(fPercent)));	\
	} while(false)
#define  MTAbsolute(num)	((num) = (num) >= 0 ? (num) : 0 - (num))

#define MT_FLOAT_TRIM_TAIL(F)	((INT)F)//��ͬϵͳ�ǲ�ͬ��

///FLOAT��Vector��ƫ�ƱȽϣ�FLOATֱ��!=Ч�����ã������ܽӽ���Ҳ����Ϊ�ǲ�����
#define  MT_FLOAT_UNEQUAL_BIAS(F1, F2, bias)	((F2)-(F1) > (bias) || (F1)-(F2) > (bias))
#define  MT_FLOAT_EQUAL_BIAS(F1, F2, bias) (!MT_FLOAT_UNEQUAL_BIAS((F1), (F2), (bias)))
#define  MT_FLOAT_UNEQUAL(F1, F2)	(MT_FLOAT_UNEQUAL_BIAS((F1), (F2), FLT_EPSILON))
#define  MT_FLOAT_EQUAL(F1, F2)		(MT_FLOAT_EQUAL_BIAS((F1), (F2), FLT_EPSILON))

#define MT_VECTOR_UNEQUAL(V1, V2, bias)	( MT_FLOAT_UNEQUAL_BIAS((V1).x, (V2).x, (bias))	\
	|| MT_FLOAT_UNEQUAL_BIAS((V1).y, (V2).y, (bias))	\
	|| MT_FLOAT_UNEQUAL_BIAS((V1).z, (V2).z, (bias)))

#define MT_VECTOR_UNEQUAL_2D(V1, V2, bias)	( MT_FLOAT_UNEQUAL_BIAS((V1).x, (V2).x, (bias))	\
											|| MT_FLOAT_UNEQUAL_BIAS((V1).y, (V2).y, (bias)))	
#define MT_VECTOR_EQUAL(V1, V2, bias)	(!(MT_VECTOR_UNEQUAL((V1), (V2), (bias))))
#define MT_VECTOR_EQUAL_2D(V1, V2, bias)	(!(MT_VECTOR_UNEQUAL_2D((V1), (V2), (bias))))

#define MT_FLOAT_IS_A_LESS_THAN_B(A, B)	((B) - (A) > 0)
#define MT_FLOAT_IS_A_LESS_OR_EQUAL_B(A, B)	((B) - (A) > -FLT_EPSILON)

#define MT_FLOAT_IS_IN_RANGE(SRC, MIN_NUM, MAX_NUM)	MT_FLOAT_IS_A_LESS_OR_EQUAL_B((MIN_NUM), (SRC)) && MT_FLOAT_IS_A_LESS_THAN_B((SRC), (MAX_NUM))

///��������
#define MT_ROUND_HELPER(num)	( (num) > floor((num))+0.5-FLT_EPSILON ? (INT)(num)+1 : (INT)(num) )
inline INT MT_Round(double num)	//��Ϊfloat�����Զ�ת��Ϊdouble�����������ṩfloat�İ汾
{
	if ((num) < 0)
	{
		num = -num;
		INT nRet = MT_ROUND_HELPER(num);
		return -nRet;
	}
	return MT_ROUND_HELPER(num);
}
__forceinline INT FourCullFiveAdd(double num)
{
	return MT_Round(num);
}

//������ֵ��������

///�õ���ɢֵ��ģ�����Ҫ�ֹ���
template<typename _ReturnType, _ReturnType _Min, _ReturnType _Max, UINT uSteps>
static _ReturnType MT_GetDiscreted(_ReturnType Src)	
{
	_KG3D_DEBUG_STATIC_CHECK_T(uSteps > 0 && _Max > _Min, ParamMistake);
	static const _ReturnType GapPerStep = (_Max - _Min)/uSteps;
	if (Src <= _Min)
		return _Min;
	if (Src >= _Max)
		return _Max;
	return (_ReturnType)(INT)((Src - _Min)/GapPerStep)*GapPerStep + _Min;
}

FLOAT GetRectDiagonalLength(RECT rect);

//�߽ṹ
struct KG3DLine 
{
	D3DXVECTOR3 m_vSrc;
	D3DXVECTOR3 m_vDes;

	KG3DLine()	{ZeroMemory(this, sizeof(KG3DLine)); }
	KG3DLine(const KG3DLine& Other)	{memcpy_s(this, sizeof(KG3DLine), &Other, sizeof(KG3DLine));}
	KG3DLine(const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vDes)	:m_vSrc(vSrc),m_vDes(vDes){}

	D3DXVECTOR3& operator[](size_t uWhichPoint){return (reinterpret_cast<D3DXVECTOR3 *>(this))[uWhichPoint];}		//֧����[]�﷨ֱ�ӷ������еĵ㣬���lineA[0] = XX
	const D3DXVECTOR3&  operator[](size_t uWhichPoint) const{return (reinterpret_cast<const D3DXVECTOR3 *>(this))[uWhichPoint];}
};

struct KG3DRay 
{
	D3DXVECTOR3 m_vSrc;
	D3DXVECTOR3 m_vNormalizedDir;

	KG3DRay()	{ZeroMemory(this, sizeof(KG3DRay)); m_vNormalizedDir.y = -1;}
	KG3DRay(const KG3DRay& Other)	{memcpy_s(this, sizeof(KG3DRay), &Other, sizeof(KG3DRay));}
	KG3DRay(const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir)	:m_vSrc(vSrc),m_vNormalizedDir(vNormalizedDir){}
};

enum
{
	EM_RAY_INTERSECTION_MAX_RANGE = 25600,//������ײ�ļ�ⷶΧ
};

struct VisitorForD3DXVECTOR3
{
	virtual HRESULT Accept(const D3DXVECTOR3& pos) = 0;
	virtual ~VisitorForD3DXVECTOR3() = 0{}
};

#endif //_INCLUDE_KGMATHFUNCTIONS_H_
