////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DMeshLensFlare.h
//  Version     : 1.0
//  Creator     : 
//  Create Date : 2006-11-13 17:08:15
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DMESHLENSFLARE_H_
#define _INCLUDE_KG3DMESHLENSFLARE_H_

////////////////////////////////////////////////////////////////////////////////

//#include "kg3dmesh.h"
#include "KG3DEnvEffect.h"
#include "KG3DEnvEffDW_KeyBase.h"
#include "IEKG3DMeshLensFlare.h"
#include "KG3DShaderHolder.h"
//#include "TemplateLibs/LOKI_APP/KG3DLokiStructInterface.h"

class KG3DTexture;
class IIniFile;

namespace KG3D_ENVIRONMENT_EFFECT
{
	struct KG3DDWGC_Key_LensFlare : public KG3DDWGC_KeyBase 
	{
		
		virtual DWORD GetType()const{return KG3DTYPE_ENVEFF_LENSFLARE;}
		
		DWORD m_dwModificationOption;
		FLOAT m_fVisibility;

		KG3DDWGC_Key_LensFlare();
		virtual bool CalculateInterp(const KG3DDWGC_KeyBase& BaseKeyBefore, const KG3DDWGC_KeyBase& BaseKeyAfter, float fPercent);
		bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);

#if defined(DEBUG) | defined(_DEBUG)
		void DebugPrint(std::ostringstream& OSS);
#endif	
	};
	struct KG3DDWGC_Key_LensFlareSun : public KG3DDWGC_Key_LensFlare
	{
		enum{DWGCKEY_TYPE = KG3DTYPE_ENVEFF_LENSFLARE_SUN};
		virtual DWORD GetType()const{return DWGCKEY_TYPE;}
	};
	struct KG3DDWGC_Key_LensFlareMoon : public KG3DDWGC_Key_LensFlare 
	{
		enum{DWGCKEY_TYPE = KG3DTYPE_ENVEFF_LENSFLARE_MOON};
		virtual DWORD GetType()const{return DWGCKEY_TYPE;}
	};

	struct KG3DMeshLensFlareDescStruct : public IEKG3DMeshLensFlareDescStruct
	{
		KG3DMeshLensFlareDescStruct();
		KG3DMeshLensFlareDescStruct(const KG3DMeshLensFlareDescStruct& Other);
		KG3DMeshLensFlareDescStruct& operator=(const KG3DMeshLensFlareDescStruct& Other);
		bool ToKey(KG3DDWGC_Key_LensFlare& Key);
		bool SynWithKey(const KG3DDWGC_Key_LensFlare& Key);
		bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
	};

	class KG3DMeshLensFlare : public KG3DEnvEffect, public IEKG3DMeshLensFlare
	{
	public:
		enum{EM_MAX_LENSFLARE = 30,};

		HRESULT KG3DMeshLensFlare::RenderFirstFlare(FLOAT nDecay = 1.0f);//���������ģ���SkySystem�ڻ�Զɽ֮ǰ���õģ�Ŀ���ǰ�̫��������ɽ���ƺ��棬˥��������0��1

		//���и�LensInfo�Ĳ������������´���VertexBuffer��С��
		virtual ULONG GetLensFlareCount();
		virtual HRESULT GetLensFlareItem(size_t uPos, LensFlareInfo* ppFlare);
		virtual HRESULT SetLensFlareItem(size_t uPos, const LensFlareInfo* pFlareInfo, size_t* pNewPos);//��Height��ʱ��ᴥ������С��

		virtual HRESULT DefaultBuild();
		virtual HRESULT AddLensFlare(const LensFlareInfo& lensFlare);
		virtual HRESULT DelLensFlare(size_t uPos);

		virtual HRESULT GetTexID(DWORD* pID);
		
		//<KG3DEnvEffect>
        virtual HRESULT OnLostDevice();
        virtual HRESULT OnResetDevice();
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext);
		virtual HRESULT PostRender();
		virtual HRESULT FrameMove();
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR tszSection);
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR tszSection);
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface);

		virtual HRESULT RequestModification(const IEKG3DMeshLensFlareDescStruct* pDescStruct);
		virtual HRESULT GetDescStruct(IEKG3DMeshLensFlareDescStruct* pRetStruct);
		//</KG3DEnvEffect>

		STDMETHOD(Init)();
		STDMETHOD(UnInit)();

		virtual HRESULT ReloadShader();
		///<��̬����>
		virtual void GetDefaultDesc(IEKG3DMeshLensFlareDescStruct* pDescStruct);
	private://���ڶ�̬����������
		KG3DDWGC_Key_LensFlare m_SubKey;
		BOOL	m_bEnableBlending;
		float	m_fBlendPercent;

		typedef		KG3DDWGC_Key_LensFlare	TypeKey;		
	public:
		HRESULT DWGCRequestPopFrontState(float fCurBlendPercent = 0.0f);
		HRESULT DWGCRequestSubStateChange(const KG3DDWGC_KeyBase* pRetKey);
		HRESULT DWGCGetCurStateToKey(KG3DDWGC_KeyBase* pRetKey);		
		HRESULT DWGCEnableSubState(BOOL bEnable);
		HRESULT DWGCSetPercent(float fPercent);
		HRESULT DWGCRequestModification(const KG3DDWGC_KeyBase* pKey);		
		///</��̬����>

	private:
		KG3DTexture* m_pTextureFlare;
		vector<LensFlareInfo>  m_vecLensFlare;
		KG3DMeshLensFlareDescStruct	m_DescStruct;
		LPDIRECT3DQUERY9	m_pQuery;	//����������ѯ
		FLOAT				m_fLastOcclusionRet;

		FLOAT   m_fAlpha;	//�����͸����
		FLOAT	m_fAlphaWithoutFirst;	//���˵�һ��͸���ȣ���һ����̫�����������ı���

		//�µ�LensFlare����
		LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;
		size_t					m_VertexCountInBuffer;
		enum{
			NEW_FVF = D3DFVF_XYZ | D3DFVF_TEX1
		};
		struct NewVertexStruct 
		{
			FLOAT xPostProj, yPostProj, Height;
			FLOAT xIndex, yIndex;
		};
		LPD3DXEFFECT m_pEffect;
		KG3DShaderHolder m_shaderHolder;
		D3DXMATRIX	m_matUsedInShader;
		enum
		{
			EM_MATRIX_ROTATION,
			EM_FLOAT2_SUN_POS_POSTPROJ,
			EM_FLOAT2_TEXTURE_SPLIT,
			EM_FLOAT_BLEND_FACTOR,
			EM_FLOAT_OC_FACTOR,
			EM_FLOAT_SCREEN_ASPECT,
			EM_PARAM_COUNT,
		};
		//D3DXHANDLE	m_ShaderHandles[EM_PARAM_COUNT];
		enum
		{
			EM_OC_FVF = D3DFVF_XYZ,
			EM_OC_POINT_ON_EDGE = 8,
			EM_OC_POINT_COUNT = EM_OC_POINT_ON_EDGE * EM_OC_POINT_ON_EDGE,
			EM_OC_POINT_SIZE = sizeof(FLOAT)*3,
			EM_VERTEX_PER_FACE = 6,
		};
		enum
		{
			em_default_x_split = 3,
			em_default_y_split = 3,
			em_default_lens_num = em_default_x_split * em_default_y_split,
		};
		LPDIRECT3DVERTEXBUFFER9	m_pOCVertexBuffer;
	protected:
		D3DXMATRIX	m_matRotationOfFlares;
		D3DXVECTOR4 m_vSunPosPostProj;
		D3DXVECTOR2	m_vScreenSize;
	private:
		FLOAT CalculateOffScreenFactor(D3DXVECTOR4 vSunPosScreen);
		VOID CalSunOcclusionFactor(D3DXVECTOR2 vScreenSize, D3DXVECTOR4 vSunPosScreen);
		D3DXVECTOR2	GetScreenSize();//ֻ������Render�ĵط���		
		FLOAT	CalculateVisibility();
		HRESULT DefaultBuildPrivate();

		static bool IsSunOnTheBackSideOfCamera(D3DXVECTOR4& vSunPosPostProj);
		static D3DXVECTOR4 CalculateScreenPos(const D3DXVECTOR3& vPosIn, const D3DXMATRIX& viewMat
											, const D3DXMATRIX& projMat);
		static bool SortComp(LensFlareInfo pElem1, LensFlareInfo pElem2);

		virtual VOID	FillVertexBuffer();
		HRESULT ReBuildVertexBuffer(size_t uLensFlareCount);
		HRESULT ReBuildOcclusionQuery();
		//HRESULT RecordEffectParamHandles();
		HRESULT SortLensInfo();		
		HRESULT SetFlareTexture(LPCTSTR tczPath);
		
		VOID	FillOCVertexBuffer();
		virtual bool ValidateLensFlareInfo( LensFlareInfo &TempLI );
		VOID RenderOcclusionCullingElement(const D3DXVECTOR4 vSunPosPostProj);
		FLOAT RetrieveOcclusionCullingData(); 
		HRESULT AddLensFlare(LensFlareInfo lensFlare, BOOL bReflashResource);

		void SetShaderParams(LPD3DXEFFECT pDevice, D3DXVECTOR2 vSunProj, FLOAT fAlpha);

		KG_TEST_DECLARE_CLASS_TEST(KG3DMeshLensFlare);
	protected:
		KG3DMeshLensFlare(int, KG3DENVEFF_TYPE emType);
		virtual ~KG3DMeshLensFlare(void);
	private://��ʵ�֣���ֹʹ��
		KG3DMeshLensFlare();
		KG3DMeshLensFlare(const KG3DMeshLensFlare&);
		KG3DMeshLensFlare& operator=(const KG3DMeshLensFlare&);
		
	};

	///��ȫʹ��ԭ���Ĵ���
	class KG3DEnvEffLensFlareSun : public KG3DMeshLensFlare
	{
	public:
		KG3DEnvEffLensFlareSun();
		~KG3DEnvEffLensFlareSun();

        const D3DXVECTOR4* GetSunPosition() { return &m_vSunPosPostProj; }
	};

	class KG3DEnvEffLensFlareMoon : public KG3DMeshLensFlare
	{
	public:
		KG3DEnvEffLensFlareMoon();
		~KG3DEnvEffLensFlareMoon();

		virtual void GetDefaultDesc(IEKG3DMeshLensFlareDescStruct* pDescStruct);

		///�ĸĴ��Section������
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR tszSection);
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR tszSection);

	};
};


#endif //_INCLUDE_KG3DMESHLENSFLARE_H_
