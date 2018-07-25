////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DFrameDrawer.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-8-29 14:58:23
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DFRAMEDRAWER_H_
#define _INCLUDE_KG3DFRAMEDRAWER_H_

////////////////////////////////////////////////////////////////////////////////
struct KG3DFrameDrawerBasic	//������D3D�Ļ��ƺ���
{
	enum{em_fvf = D3DFVF_XYZ,};
	/*
	����������������Ļ������Զ���Device����ȡһ�Σ��������Begin֮ǰҪ��Transform�Ļ���Ӧ���������裬��Ȼ�ᱻ���ǡ�
	Ĭ��ʹ��Ĭ�ϵ���Ⱦ״̬��Ⱦ�������ΪFALSE�Ļ�����Ҫ�Լ�������Ⱦ״̬
	*/
	virtual HRESULT Begin(const D3DXMATRIX* pWorld = NULL, const D3DXMATRIX* pView = NULL
						, const D3DXMATRIX* pProj = NULL, BOOL UseDefaultStates = TRUE) = 0;	
	
	virtual HRESULT End() = 0;
	virtual HRESULT SetTransform(const D3DXMATRIX* pWorld, const D3DXMATRIX* pView, const D3DXMATRIX* pPerspective) = 0;
	virtual HRESULT SetColor(const D3DCOLORVALUE& color) = 0;

	virtual ~KG3DFrameDrawerBasic() = 0{}
};
struct KG3DFrameDrawer : public KG3DFrameDrawerBasic
{
	virtual VOID	SetLineColor(D3DCOLOR color) = 0;
	virtual VOID	SetFrameColor(D3DCOLOR color) = 0;
	virtual VOID	SetParameters(BOOL bEnableFace, BOOL bEnableLine, D3DCOLOR FaceColor, D3DCOLOR LineColor) = 0;
	virtual HRESULT SetColor(const D3DCOLORVALUE& color) = 0;	//ͬʱ��Line��Frame����ɫ
	virtual HRESULT Draw(const D3DXVECTOR3& UpperLeft, const D3DXVECTOR3& UpperRight, const D3DXVECTOR3& LowerRight) = 0;
};

struct KG3DFrameDrawerCone : public KG3DFrameDrawer 
{
	virtual HRESULT DrawCone(const D3DXVECTOR3& vCenter, const D3DXVECTOR3& vNormal, FLOAT fBottomRadius, FLOAT fHeight) = 0;
};

struct KG3DFrameDrawerFan : public KG3DFrameDrawerCone
{
	//�뾶��Begin��Center��λ�þ���,Begin��End��Զ��˳ʱ��(Normal�����ַ���)��Angle��Clamp������2Pi
	virtual HRESULT DrawFan(const D3DXVECTOR3& vCenter, const D3DXVECTOR3& vNormal, const D3DXVECTOR3& Begin, FLOAT fAngle) = 0;
	virtual HRESULT DrawCircle(const D3DXVECTOR3& vCenter, const D3DXVECTOR3& vNormal, FLOAT Radius) = 0;	//��Բ
};

struct KG3DFrameDrawerBox : public KG3DFrameDrawer 
{
	virtual HRESULT SetAxis(const D3DXVECTOR3& vX, const D3DXVECTOR3& vZ) = 0;
	virtual HRESULT SetRange(const D3DXVECTOR3& vRangeXYZ) = 0;
	virtual HRESULT DrawBox(const D3DXVECTOR3& Center) = 0;
};

//���е�Drawer����һ��Begin֮������һ��������Begin��ֱ��End�����Shader������һ�µģ�һ��Effect����Begin���Ͳ���Begin����һ��Effect
extern KG3DFrameDrawerBasic& g_GetFrameDrawerBasic();	//���е�����ɫ���Ķ��������������Drawer�����Drawer���Begin��֮�󣬿���ֱ�ӵ���d3d�Ļ��ƺ���
extern KG3DFrameDrawer& g_GetFrameDrawerRect();
extern KG3DFrameDrawer& g_GetFrameDrawerTri();
extern KG3DFrameDrawerFan& g_GetFrameDrawerFan();
extern KG3DFrameDrawerCone& g_GetFrameDrawerCone();
extern KG3DFrameDrawerBox& g_GetFrameDrawerBox();

extern VOID g_CheckFrameDrawers(const D3DXVECTOR3& vPos, FLOAT fRange);	//���ڼ�������ȷ���


struct KG3DTexFrameDrawer : public KG3DFrameDrawer
{
	virtual HRESULT SetTexture(LPDIRECT3DTEXTURE9 pTexture) = 0;
};

#endif //_INCLUDE_KG3DFRAMEDRAWER_H_
