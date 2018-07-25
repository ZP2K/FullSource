////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DGraphicsStructEx.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 16:58:29
//  Comment     : ��ΪSceneEditor�м�ӵļ��ʹ����GraphicStruct.h
// ֻҪ��������Ӻ������йصĶ����ͻ���벻����ֻ�ܶ���������ļ�
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DGRAPHICSTRUCTEX_H_
#define _INCLUDE_KG3DGRAPHICSTRUCTEX_H_
#include "GraphicsStruct.h"
////////////////////////////////////////////////////////////////////////////////

/*
��Щ�඼��Ϊ�˸ı���Ⱦ״̬֮�����Զ���ԭ(������������)
ʹ�÷�ʽ����
ClassABCD abcd(XX,XX);
Ҫע��������
*/
namespace GraphicsStruct
{
	//////////////////////////////////////////////////////////////////////////
	struct RenderStateAlphaWithoutSettingAlphaTest///�趨������AlphaBlend��������û��AlphaTest
	{

		DWORD m_dwBackAlphaBlendEnable;
		DWORD m_dwBackSrcBlend;
		DWORD m_dwBackDestlend;
		RenderStateAlphaWithoutSettingAlphaTest(DWORD AlphaBlendEnable = TRUE,DWORD SrcBlend = D3DBLEND_SRCALPHA, DWORD DestBlend = D3DBLEND_INVSRCALPHA);
		~RenderStateAlphaWithoutSettingAlphaTest();
	};
	struct RenderStateGlobleCameraZFar///�����趨ȫ�ֵ�Camera��ZFar���
	{
		D3DXMATRIX m_matProj;
		RenderStateGlobleCameraZFar(FLOAT fZFar = -FLT_EPSILON);	///Ĭ�ϵ���������
		~RenderStateGlobleCameraZFar();		
	};
	struct RenderStateWorldMatrixMoveWithCamera///�������������Camera�˶�
	{
		D3DXMATRIX m_matWorldSave;

		RenderStateWorldMatrixMoveWithCamera();
		~RenderStateWorldMatrixMoveWithCamera();
	};
	struct SamplerStateAddressVClamp///ʹ��V�������
	{
		SamplerState	m_SamplerState;
		SamplerStateAddressVClamp(DWORD dwSampler = 0);
		~SamplerStateAddressVClamp();
	};
	struct MaterialWhite	//��ɫ����
	{
		D3DMATERIAL9	m_Material;
		MaterialWhite();
		~MaterialWhite();
	};
	struct LightDisable		//��ֹ�ƹ�
	{
		DWORD m_nIndex;
		BOOL m_Value;
		LightDisable(DWORD nWhichLight);
		~LightDisable();
	};
	struct RenderTargetSave		//����RenderTarget������������ͬһ��������ʹ��
	{
		LPDIRECT3DSURFACE9	m_pTarget;
		D3DVIEWPORT9		m_ViewportSave;//��RenderTargetҪ˳�㱣��ViewPort����Ȼ������
		DWORD m_uIndex;
		RenderTargetSave(DWORD uIndex);
		~RenderTargetSave();
	};
	struct DelpthStencilSave	//����DepthStencil
	{
		LPDIRECT3DSURFACE9	m_pDelpthStencil;
		DelpthStencilSave();
		~DelpthStencilSave();
	};
	struct RenderTargetAndDepthStencilSave : public RenderTargetSave, public DelpthStencilSave	//һ��SetRenderTargetҪͬʱ��RT��Depth��ViewPort������һ�α�����
	{
		RenderTargetAndDepthStencilSave(DWORD uIndex):RenderTargetSave(uIndex){}
	};
	struct TextureDisable		//��ֹĳIndex��Stage����ͼ
	{
		LPDIRECT3DBASETEXTURE9	m_pTexSave;
		DWORD m_uIndex;
		TextureDisable(DWORD Index);
		~TextureDisable();
	};
	struct StencilEnable		//��Stencil����֮��һ��Ҫ�رգ�����Ҫ������� 
	{
		StencilEnable();
		~StencilEnable();
	};
	struct StencilTwoSidedEnable	//��˫��Stencil�������׳������Դ�֮��ҲҪ�Զ��ص�
	{
		StencilTwoSidedEnable();
		~StencilTwoSidedEnable();
	};
	struct ColorWriteDisable		//��ֹ��ɫ���
	{
		ColorWriteDisable();
		~ColorWriteDisable();
	};
	struct SaveMatrix		//����ĳ��D3DPT�ľ���
	{
		D3DXMATRIX m_Mat;
		D3DTRANSFORMSTATETYPE m_Type;
		SaveMatrix(D3DTRANSFORMSTATETYPE dwType);//D3DTS_WORLD
		~SaveMatrix();
	};
	struct FVFSetter		//����FVF
	{
		DWORD dwFVF;
		FVFSetter(DWORD dwFVFNew);
		~FVFSetter();
	};
};

class KG3DCameraSphere
{
public :
	void Update(D3DXVECTOR3& vCenter, float fRaduis);

	bool CheckModelCross(KG3DModel* pModel);
	bool CheckFacesCross(D3DXVECTOR3* pFaceVertex, DWORD dwFaceNum);

	inline ObstructdInfo* GetObstructdInfo() {return &m_ObsInfo;}
	inline D3DXVECTOR3*   GetCenter() {return &m_vCenter;}
	inline float          GetRadius() {return  m_fRadius;}

	KG3DCameraSphere();
	virtual ~KG3DCameraSphere();

protected :
	bool _ChkModelCross(KG3DModel* pModel);

protected :
	D3DXVECTOR3 m_vCenter;
	float       m_fRadius;

	ObstructdInfo m_ObsInfo;
};


#endif //_INCLUDE_KG3DGRAPHICSTRUCTEX_H_
