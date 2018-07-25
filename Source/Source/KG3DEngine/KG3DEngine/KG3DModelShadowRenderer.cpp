////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DModelShadowRendererSolid.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-9-17 15:35:22
/*  Comment     : ModelShadowRenderer������Ч�ʣ�09-03-18���ڲ��Գ����������κ�
��Ч������£�¼�õĿ��;���Ӱ�ӺͲ����;���Ӱ�ӵ�֡�ٱ���145/150��
Ӱ���ڴ�������Buffer���ڵ�ʱ��������StrenchRect����FrameMoveӦ���ǻ���ٵģ�������ʵ
��¼�õļ��ٱ���75/79��������Ϊ��FrameMove��Ӱ�ӵķ����첽�ˣ����Ի���Ӱ������StrenchRect
����FrameMove���첽�ô�����
*/
////////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "KG3DModelShadowRenderer.h"
#include "KG3DTextureTable.h"
#include "KG3DTexture.h"
#include "KG3DSceneEntityMgr.h"
#include "KG3DGraphicsStructEx.h"
#include "KG3DSceneEntityMgr.h"
#include "KG3DModel.h"
#include "KG3DGraphicsTool.h"
//#include "KG3DEngineManager.h"
//#include "KG3DGraphiceEngine.h"
#include "KG3DDefaultShaderGroup.h"
#include "KG3DShaderHolder.h"
#include "KG3DShaderManager.h"
#include "KG3DSystemEventListener.h"
#include "KG3DGraphiceEngine.h"
#include "KG3DMeshHelpers.h"

#include "KGPerformence.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct KG3DModelShadowRendererEffectParams 
{
	union
	{
		struct
		{
			D3DXHANDLE	hBiasX;
			D3DXHANDLE	hBiasY;
			D3DXHANDLE	hMaxShadow;
			D3DXHANDLE	hScaleFactor;
			D3DXHANDLE	hPos;
			D3DXHANDLE	hMatViewProj;
			D3DXHANDLE	hVecPosScale;
			D3DXHANDLE	hAlpha;
			
			D3DXHANDLE hTecPreparation;
			D3DXHANDLE hTecToBackBuffer;
			D3DXHANDLE hTecFinalBlend;
		};

		struct  
		{
			D3DXHANDLE ParamHandles[8];
			D3DXHANDLE TecHandles[3];
		};
	};

	KG3DModelShadowRendererEffectParams(){ZeroMemory(this, sizeof(KG3DModelShadowRendererEffectParams));}

	std::pair<LPCTSTR*, size_t>		GetParamNames()
	{
		static LPCTSTR Names[] =  
		{
			_T("g_fBiasX"),
			_T("g_fBiasY"),
			_T("g_fMaxShadow"),
			_T("g_fScaleFactor"),
			_T("g_vecPos"),
			_T("g_matViewProj"),
			_T("g_vecPosScale"),
			_T("g_fAlpha"),
			
			_T("tecShadowPreparation"),
			_T("tecToBackBuffer"),
			_T("tecShadowFinalBlend"),
		};
		return std::pair<LPCTSTR*, size_t>(Names, _countof(Names));
	}

	HRESULT Init(LPD3DXEFFECT p)
	{
		_ASSERTE(NULL != p);
		_ASSERTE(sizeof(ParamHandles) + sizeof(TecHandles) == sizeof(*this));

		std::pair<LPCTSTR*, size_t> pirNames = GetParamNames();
		_ASSERTE(pirNames.second == _countof(ParamHandles) + _countof(TecHandles));
		LPCTSTR* Names = pirNames.first;

		for (size_t i = 0; i < _countof(ParamHandles); ++i)
		{
			ParamHandles[i] = p->GetParameterByName(NULL, Names[i]);
			KG_PROCESS_ERROR(NULL != ParamHandles[i]);
		}

		for (size_t i = 0; i < _countof(TecHandles); ++i)
		{
			TecHandles[i] = p->GetTechniqueByName(Names[ _countof(ParamHandles)+ i]);
			KG_PROCESS_ERROR(NULL != TecHandles[i]);
		}
		return S_OK;
Exit0:
		return E_FAIL;
	}
};

static KG3DModelShadowRendererEffectParams stEffectParams;

namespace
{
	struct CustomShaderEventHandler : public KG3DShaderEventHandler
	{
		virtual HRESULT OnReload(LPD3DXEFFECT p){_ASSERTE(p); return stEffectParams.Init(p);}
		virtual ULONG STDMETHODCALLTYPE Release(){return 0;}
		virtual KG3DShaderEventHandler* Clone()const;
	};

	static CustomShaderEventHandler s_ShaderEventHandler;

	KG3DShaderEventHandler* CustomShaderEventHandler::Clone() const
	{
		return &s_ShaderEventHandler;
	}
};


/////////////////////////////////////////////////////////////////////////
/*
�����������Դ�����Զ������豸��ʧ���ο���Ա�����Աߵ�ע��
*/
class KG3DModelShadowRendererSolid : public KG3DModelShadowRenderer, public KG3DMessageSingleSenderReceiver
{
public:
	static KG3DModelShadowRendererSolid* Build(LPCTSTR, DWORD_PTR, DWORD_PTR);

	STDMETHOD(FrameMove)(THIS);
	virtual ULONG STDMETHODCALLTYPE Release( void);
	virtual HRESULT Render(KGCH::TFrontAccessProxy<KG3DModel*>& SortedModelsIt);
	virtual HRESULT InputShadowDummy(const D3DXVECTOR3& Pos, FLOAT fScaleFactor);//�ⲿ�ģ���Ҫ���ƽŵ�Ӱ�ӵģ�������Input����������Ⱦ������Input��Ȼ����Render�лᱻFlush������ΪModelShadowRender��Singleton������Ҫ�ڵ�����Ⱦ�����б��ֶ����ռ������һ��Render��Input��Render����Ȼ������������������û��������⡣

	virtual ULONG	SynchronizedStrenchRect(BOOL bSynchronized);

	virtual void __stdcall OnReceiveMessage(const KG3DMessage& Message, IKG3DMessageSenderBase* pSender);

	~KG3DModelShadowRendererSolid(void);
private:
	KG3DModelShadowRendererSolid(KG3DSystemEventListener& lst);

	STDMETHOD_(ULONG, GetType)(THIS){return KG3DTYPE_MODEL_SHADOW_RENDERER;}
	STDMETHOD(Init)();
	STDMETHOD(UnInit)();

	HRESULT RenderShadowBegin();
	HRESULT RenderShadow(const D3DXVECTOR3& vecPos, FLOAT fScaleFactor, const D3DXVECTOR3& vecCameraPos, LPD3DXEFFECT pEffect);
	HRESULT RenderShadowEnd();
	HRESULT RenderShadowToBackBuffer(BOOL bUseTwoRenderTarget, UINT uBackBufferWidth, UINT uBackBufferHeight);

	KG3DTexture*	m_pShadowTexFullScreen;	//��ȫ���õ�Stencil�Ĵ�С,���Զ������豸��ʧ
	KG3DTexture*	m_pShadowTexSmaller;	//����÷���,���Զ������豸��ʧ
	struct TypeShadowVolumeVertex
	{
		FLOAT x,y,z;
		enum{fvf = D3DFVF_XYZ,};
	};

	struct ShadowVolumeParams	
	{
		FLOAT fRadiusScalePercent;
		FLOAT fTopHeightScalingPercent;
		UINT  NumVerticesOnTop;
		FLOAT fBottomHeightScalingPercent;
		FLOAT fShadowMaxAlpha;

		//LOD
		FLOAT fLod0TangentSquare;	//��֮�ڵĻ�ʹ����߾��ȵ�Ӱ��
		FLOAT fLod1TangentSqure;	//�еȾ��ȣ�������û��Ӱ����
		UINT  NumLod1IBSize;

		ShadowVolumeParams(){ZeroMemory(this,sizeof(ShadowVolumeParams));}
	};
	ShadowVolumeParams m_SVParams;

	LPD3DXMESH	m_pMesh;	//��Ӱ�壬Ӧ����Managed�͵�

	KG3DShaderHolder		m_ShaderHolder;//���Զ������豸��ʧ

	bool	m_bForceSinglePassShadow;	//Ӳ��Ӱ��
	bool	m_bCanUseTwoSidedStencil;	//˫��ģ�建�����ض��Կ�֧�ֵ�
	bool	m_bSynchronizedStenchRect;	//�Ƿ���Render��ʱ�����ŵڶ�RenderTarget

	RECT	m_rectViewGainInRender;

	enum
	{
		em_max_extra_shadows = 100,
		em_max_shadow_scale_factor = 90,
	};
	std::vector<D3DXVECTOR4>	m_vecExtraModels;
private:
	HRESULT InitShadowVolume();

	HRESULT UninitShadowVolume();

	INT PrepareParamsForEachShadow(const D3DXVECTOR3& vecModel, FLOAT fShadowScaleFactor
		, const D3DXVECTOR3& CommonParam, LPD3DXEFFECT pEffect);//����LOD
	VOID RenderShadowVolume();
};

//////////////////////////////////////////////////////////////////////////

KG3DModelShadowRenderer& stGetModelShadowRendererDummy()
{
	static KG3DModelShadowRendererDummy stDummy;
	return stDummy;
}

/*KG3DModelShadowRenderer��ȫ�ֵ�Singleton�����ֺ�d3d�����������йأ��������������ں�����
���Ҫ������Ĵ���Ļ�������ϸ��������塣
1.���ܼ򵥵���NULL��ΪSingleton���������������ڲ�ͣ�ĳ��Դ���������
2.auto_ptr��ǿ�󣬵�����ϵͳ�������ֱ�����ʱ��d3d�����ڣ�����ɸ���һ��ı������Ӷ��谭�������ı����ͷ�
3.������mayer Singleton����Ϊ��Ҫd3d
4.��������������ֵָ����ǵķ����򵥶���ʵ�ʣ��ĵĻ�Ҫ��������������
5.��ʹModelShaderRender���ͷţ����ڲ���d3d��ԴҲ������й©���⣬��Ϊע����ȫ���¼���d3d������֪ͨ��
*/

#define SPECIAL_PTR ((KG3DModelShadowRenderer*)(0x00000001))

static KG3DModelShadowRenderer* s_pModelShadowRenderer = SPECIAL_PTR;

KG3DModelShadowRenderer& g_GetModelShadowRenderer()
{
	if (SPECIAL_PTR == s_pModelShadowRenderer)
	{
		s_pModelShadowRenderer = KG3DModelShadowRendererSolid::Build(NULL, NULL, NULL);
	}
	_ASSERTE(SPECIAL_PTR != s_pModelShadowRenderer);
	if (NULL != s_pModelShadowRenderer)
	{
		return *s_pModelShadowRenderer;
	}
	return stGetModelShadowRendererDummy();
}

VOID g_ModelShadowRendererRelease()
{
	if(SPECIAL_PTR != s_pModelShadowRenderer)
	{
		SAFE_RELEASE(s_pModelShadowRenderer);
	}
}
KG3DModelShadowRendererSolid::KG3DModelShadowRendererSolid(KG3DSystemEventListener& lst)
:m_pShadowTexSmaller(NULL)
,m_pShadowTexFullScreen(NULL)
,m_pMesh(NULL)
,m_bCanUseTwoSidedStencil(false)
,m_bForceSinglePassShadow(false)
,m_bSynchronizedStenchRect(true)	//Ĭ�Ͼ�ͬ������ͬ���첻�˶��٣���1/74֡���ң��������׳�Bug
{
	bool bRet = lst.RegisterReceiver(this);
	_ASSERTE(bRet);
	KG_USE_ARGUMENT(bRet);

	RECT tempRect = {0,0,1,1};
	m_rectViewGainInRender = tempRect;
}

KG3DModelShadowRendererSolid::~KG3DModelShadowRendererSolid(void)
{
}

HRESULT KG3DModelShadowRendererSolid::Init()
{
	KG3DTexture* pTexFull = NULL;
	KG3DTexture* pTexSmaller = NULL;
	HRESULT hr = E_FAIL;  

	//�������
	{
		bool bStencilAvailable = (g_cGraphicsEngine.GetD3DCaps().StencilCaps & D3DSTENCILCAPS_INCR) && (g_cGraphicsEngine.GetD3DCaps().StencilCaps & D3DSTENCILCAPS_DECR);
		//�е��Կ���֧��INCR��������֧��INCRSAT���ӵ�һ���̶Ⱦ͵�����DECRͬ�������Կ����������ڵ�SV��
		KG_PROCESS_ERROR(bStencilAvailable);
		//�õ�˫��Stencil������־
		m_bForceSinglePassShadow = (g_cGraphicsEngine.GetD3DCaps().StencilCaps & D3DSTENCILCAPS_TWOSIDED) ? true : false;

	}

	enum
	{
		full_screen_shadow_tex_bigger = 4,
		format_use = D3DFMT_X8R8G8B8,
	};
	_KG3D_DEBUG_STATIC_CHECK_T(full_screen_shadow_tex_bigger <= 4, SizeToSmall);	
	
	_ASSERTE(! m_pShadowTexFullScreen && ! m_pShadowTexSmaller);	
	hr = UnInit();
	KG_COM_PROCESS_ERROR(hr);

	{
		m_SVParams.fRadiusScalePercent = g_cEngineOption.ModelShadowParams[0];
		m_SVParams.fTopHeightScalingPercent = g_cEngineOption.ModelShadowParams[1];
		m_SVParams.fBottomHeightScalingPercent = g_cEngineOption.ModelShadowParams[2];
		m_SVParams.NumVerticesOnTop = (UINT)g_cEngineOption.ModelShadowParams[3];

		m_SVParams.fLod0TangentSquare = g_cEngineOption.ModelShadowParams[4];//����LOD������Ϊ׼�������Ǹ�����
		m_SVParams.fLod1TangentSqure = g_cEngineOption.ModelShadowParams[5];//��ʱֻ������LOD

		m_bForceSinglePassShadow = MT_FLOAT_UNEQUAL_BIAS(g_cEngineOption.ModelShadowParams[6], 0.0f, 0.1f);

		m_SVParams.fShadowMaxAlpha = g_cEngineOption.ModelShadowParams[7];

		//���Ϸ���
		MT_LIMIT_TO_RANGE(m_SVParams.fRadiusScalePercent, 10, 0);
		MT_LIMIT_TO_RANGE(m_SVParams.fTopHeightScalingPercent, 20, 0);
		MT_LIMIT_TO_RANGE(m_SVParams.fBottomHeightScalingPercent, 20, 0);

		if (m_SVParams.NumVerticesOnTop % 4 != 0)
		{
			m_SVParams.NumVerticesOnTop = m_SVParams.NumVerticesOnTop - m_SVParams.NumVerticesOnTop % 4 + 4;//�յ�4�ı���
		}

		MT_LIMIT_TO_RANGE(m_SVParams.NumVerticesOnTop, 128, 4);//���128����
		MT_LIMIT_TO_RANGE(m_SVParams.fShadowMaxAlpha, 1, 0);

		MT_LIMIT_TO_RANGE(m_SVParams.fLod0TangentSquare, 2000, 1);	//Lod0֮�ڲŻ�����õ�Ӱ��
		MT_LIMIT_TO_RANGE(m_SVParams.fLod1TangentSqure, m_SVParams.fLod0TangentSquare + FLT_MAX/2, m_SVParams.fLod0TangentSquare);

		m_SVParams.fLod0TangentSquare = 50 / m_SVParams.fLod0TangentSquare;	//���˵Ľŵ�Ӱ�ӵİ뾶Ϊ��׼���õ�Tangent
		m_SVParams.fLod1TangentSqure = 50 / m_SVParams.fLod1TangentSqure;

		m_SVParams.NumLod1IBSize = 8 * 3;	//��8���ˮ����������Ӱ�塣


		_ASSERTE(m_SVParams.fShadowMaxAlpha < 1+ FLT_EPSILON);
	}


	

	//��������RenderTarget��һ��һС
	hr = g_cTextureTable.GetRenderTargetTexture(0,0,(D3DFORMAT)format_use,&pTexFull, 1);//����backBuffer��С��RT
	KG_COM_PROCESS_ERROR(hr);
	{
		//RenderTarget�Ļ���Texture���Զ������豸�ָ�������ָ����Default�Ĵ�С��RT���Զ��ڻָ���ʱ�����µ��豸��С
		m_pShadowTexFullScreen = pTexFull;
		pTexFull = NULL;
	}	
	hr = g_cTextureTable.GetRenderTargetTexture(0,0,(D3DFORMAT)format_use,&pTexSmaller, full_screen_shadow_tex_bigger);//����backBuffer��full_screen_shadow_tex_bigger��֮1��С��RT

	KG_COM_PROCESS_ERROR(hr);
	hr = pTexSmaller->ColorFill(NULL, 0xffffffff);	
	m_pShadowTexSmaller = pTexSmaller;
	pTexSmaller = NULL;

	//������Ӱ��
	hr = InitShadowVolume();//���Ҫ��Effectˢ��֮ǰ
	KG_COM_PROCESS_ERROR(hr);

	//��Effect����Ĳ������������
	m_ShaderHolder = g_GetDefaultShaderGroup().GetDefaultShader(DEFST_MODEL_SHADOW);

	{
		hr = g_GetShaderManager().SetShaderEventHandler(m_ShaderHolder, &s_ShaderEventHandler);
		_ASSERTE(SUCCEEDED(hr));

		LPD3DXEFFECT pEffectTemp = m_ShaderHolder.GetEffect();
		_ASSERTE(NULL != pEffectTemp);
		hr = stEffectParams.Init(pEffectTemp);
		KG_COM_PROCESS_ERROR(hr);
		
		const D3DPRESENT_PARAMETERS& PresentParameters = g_cGraphicsEngine.GetPresentParameters();
			
		FLOAT BackWidth = (FLOAT)PresentParameters.BackBufferWidth;
		FLOAT SmallMapWidth = BackWidth/full_screen_shadow_tex_bigger;

		FLOAT BackHeight = (FLOAT)PresentParameters.BackBufferHeight;
		FLOAT SmallMapHeight = BackHeight/full_screen_shadow_tex_bigger;

		_ASSERTE(NULL != m_pShadowTexSmaller);//����ͼ��ȡ��������ֵ���ܺͼ���Ĳ�һ��
		{
			unsigned uWidth = 0, uHeight = 0;
			if(SUCCEEDED(m_pShadowTexSmaller->GetSize(&uWidth, &uHeight)))
			{
				SmallMapWidth = (FLOAT)uWidth;
				SmallMapHeight = (FLOAT)uHeight;
			}
		}

		FLOAT fMaxAlpha = m_SVParams.fShadowMaxAlpha;
		_ASSERTE(fMaxAlpha > -FLT_EPSILON && fMaxAlpha < 1+FLT_EPSILON);

		hr = pEffectTemp->SetFloat(stEffectParams.hBiasY, 1.0f/SmallMapHeight);
		KG_COM_PROCESS_ERROR(hr);
		hr = pEffectTemp->SetFloat(stEffectParams.hBiasX, 1.0f/SmallMapWidth);
		KG_COM_PROCESS_ERROR(hr);
		hr = pEffectTemp->SetFloat(stEffectParams.hMaxShadow, fMaxAlpha);
		KG_COM_PROCESS_ERROR(hr);
		

		pEffectTemp = NULL;
	}

	return S_OK;
Exit0:
	SAFE_RELEASE(pTexFull);
	SAFE_RELEASE(pTexSmaller);
	SAFE_RELEASE(m_pMesh);
	return E_FAIL;
	
}

HRESULT KG3DModelShadowRendererSolid::UnInit()
{
#if defined(_DEBUG) | defined(DEBUG)
	UINT uRef = 0;
	if(NULL != m_pShadowTexFullScreen)
	{
		uRef = m_pShadowTexFullScreen->Release();
		m_pShadowTexFullScreen = NULL;
	}
	if(NULL != m_pShadowTexSmaller)
	{
		uRef = m_pShadowTexSmaller->Release();
		m_pShadowTexSmaller = NULL;
	}
	
#else
	SAFE_RELEASE(m_pShadowTexFullScreen);
	SAFE_RELEASE(m_pShadowTexSmaller);
#endif
	
	UninitShadowVolume();
	return S_OK;
}

ULONG STDMETHODCALLTYPE KG3DModelShadowRendererSolid::Release()
{
	this->UnInit();
	KG3DCommonObjectSimple::Release();
	return 0;
}

HRESULT STDMETHODCALLTYPE KG3DModelShadowRendererSolid::FrameMove()
{
	if (! m_bSynchronizedStenchRect && g_cEngineOption.ModelShadowType == EM_MODEL_SHADOW_TYPE_LOW)
	{
		HRESULT hr = m_pShadowTexFullScreen->StretchRect(*m_pShadowTexSmaller, &m_rectViewGainInRender, NULL, D3DTEXF_LINEAR);
		KG_USE_ARGUMENT(hr);
	}
	return S_OK;
}
HRESULT KG3DModelShadowRendererSolid::RenderShadowToBackBuffer(BOOL bUseTwoRenderTarget
															   , UINT uBackBufferWidth, UINT uBackBufferHeight)
{
	struct TypeScreenRectVertex
	{
		FLOAT x,y,z;
		FLOAT tu,tv;
		enum{fvf = D3DFVF_XYZ | D3DFVF_TEX1};	//ע�������ֻ��VertexShader���������FVF����D3DFVF_XYZRHW�����������VS��
	};

	static TypeScreenRectVertex ScreenRect[] = {
		{ 0, 0,	0, 0, 0},
		{ 1, 0,	0, 1, 0},
		{ 0, 1, 0, 0, 1},
		{ 1, 1, 0, 1, 1},
	};

	HRESULT hr = E_FAIL;

	LPD3DXEFFECT pEffect = m_ShaderHolder.GetEffect();
	_ASSERTE(NULL != pEffect);
	_ASSERTE(NULL != m_pShadowTexSmaller && NULL != m_pShadowTexFullScreen);

	{
		D3DXVECTOR4 vecPosScale((FLOAT)uBackBufferWidth, (FLOAT)uBackBufferHeight, 0,0);
		hr = pEffect->SetVector(stEffectParams.hVecPosScale, &vecPosScale);//��Shader������ĵ�λ���γſ�����Ļ��С
	}

	D3DVIEWPORT9 viewTemp;
	g_pd3dDevice->GetViewport(&viewTemp);
	
	KG_PROCESS_SUCCESS(m_SVParams.fShadowMaxAlpha < FLT_EPSILON);
	
	//�������ʹ�õ�RenderTarget����ô�Ȼ���һ�������ϣ�Ȼ�����ţ�Ȼ���ٻ���ȥ
	if (bUseTwoRenderTarget && m_bForceSinglePassShadow)
	{
		{
			//��RenderTarget������һ��������,����Stencil�ϵ���Ϣ�ٳ���Ӱ�ĺڿ��
			GraphicsStruct::RenderTargetSave	SaveRenderTarget(0);	//��˳�㱣��Viewport�ģ�����������Ա�ViewPort
			GraphicsStruct::RenderStateAlphaWithoutSettingAlphaTest alphaState(FALSE);

			hr = m_pShadowTexFullScreen->SetAsRenderTarget(g_pd3dDevice, 0);		
			hr = g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xffffffff, 0, 0);//ע�⣬������Z��Stencil��
			hr = g_pd3dDevice->SetFVF(TypeScreenRectVertex::fvf);

			const UINT PassToBegin = 0;
			hr = pEffect->SetTechnique(stEffectParams.hTecToBackBuffer);
			if (SUCCEEDED(pEffect->Begin(NULL, 0)))
			{
				if(SUCCEEDED(pEffect->BeginPass(PassToBegin)))
				{
					hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, ScreenRect, sizeof(TypeScreenRectVertex));
					pEffect->EndPass();
				}
				pEffect->End();
			}
		}

		{
			//����Target��Ȼ�����»���ȥ
			//��Ϊͨ�����ڶ�����ȫ����С�ģ�����RenderTarget�ϻ��Ƶõ�Ҳ����ȫRenderTarget��С�ġ�
			RECT rectTemp = {(int)viewTemp.X, (int)viewTemp.Y, (int)viewTemp.X + (int)viewTemp.Width
				, (int)viewTemp.Y + (int)viewTemp.Height};
			if (m_bSynchronizedStenchRect)
			{
				hr = m_pShadowTexFullScreen->StretchRect(*m_pShadowTexSmaller, &rectTemp, NULL, D3DTEXF_LINEAR);
			}
			else
			{
				m_rectViewGainInRender = rectTemp;
			}

			hr = m_pShadowTexSmaller->SetTexture(g_pd3dDevice, 0);		
			hr = g_pd3dDevice->SetFVF(TypeScreenRectVertex::fvf);
			hr = pEffect->SetTechnique(stEffectParams.hTecFinalBlend);

			if (SUCCEEDED(pEffect->Begin(NULL, 0)))
			{
				if(SUCCEEDED(pEffect->BeginPass(0)))
				{
					g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, ScreenRect, sizeof(TypeScreenRectVertex));
					pEffect->EndPass();
				}
				pEffect->End();
			}
		}
	}
	else
	{
		GraphicsStruct::RenderStateAlphaWithoutSettingAlphaTest alphaState(TRUE
			, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);

		hr = pEffect->SetFloat(stEffectParams.hMaxShadow, m_SVParams.fShadowMaxAlpha);
		hr = g_pd3dDevice->SetFVF(TypeScreenRectVertex::fvf);

		const UINT PassToBegin = 0;
		hr = pEffect->SetTechnique(stEffectParams.hTecToBackBuffer);
		if (SUCCEEDED(pEffect->Begin(NULL, 0)))
		{
			if(SUCCEEDED(pEffect->BeginPass(PassToBegin)))
			{
				hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, ScreenRect, sizeof(TypeScreenRectVertex));
				pEffect->EndPass();
			}
			pEffect->End();
		}
	}

Exit1:
	return S_OK;
}

HRESULT KG3DModelShadowRendererSolid::RenderShadowBegin()
{
	HRESULT hr = E_FAIL;

	//���DepthStencil�ĸ�ʽ
#if defined(DEBUG) | defined(_DEBUG)
	{
		LPDIRECT3DSURFACE9 pDepthStencilSurface = NULL;
		if(SUCCEEDED(g_pd3dDevice->GetDepthStencilSurface(&pDepthStencilSurface)))
		{
			D3DSURFACE_DESC Desc;
			pDepthStencilSurface->GetDesc(&Desc);
			//_ASSERTE(Desc.Format == D3DFMT_D24S8 && _T("�л�Ӱ��û���л�����ȷ��DepthStencil��ʽ��Ӱ�Ӳ���������ʾ"));

			SAFE_RELEASE(pDepthStencilSurface);
		}
	}
#endif
	/*KG_PROCESS_ERROR(g_cGraphicsEngine.GetPresentParameters().MultiSampleType == D3DMULTISAMPLE_NONE 
		&& g_cEngineOption.ModelShadowType == EM_MODEL_SHADOW_TYPE_LOW
		&& _T("����Depth�����ݵ�ԭ�򣬿�AA�Ļ����ŵ�ƬƬ�ǲ����������Ƶģ���Ҫ�ŵ�ƬƬ����AA�Ļ�\
		��Ҫ��������AA��RenderTarget��̫���ˡ���AA���Ǹ߶˻�������Ӧ���õͶ�Ӱ�ӵ�"));*/
	KG_PROCESS_ERROR(g_cEngineOption.ModelShadowType == EM_MODEL_SHADOW_TYPE_LOW);
	KG_PROCESS_SUCCESS(m_SVParams.fShadowMaxAlpha < FLT_EPSILON);	

	g_pd3dDevice->SetFVF(TypeShadowVolumeVertex::fvf);

	{
		LPD3DXEFFECT p = m_ShaderHolder.GetEffect();
		_ASSERTE(NULL != p);

		{
			KG3DCamera* pCam = g_cGraphicsTool.GetCamera();
			KG_PROCESS_ERROR(NULL != pCam);
			
			//<TEMP>
			//pCam->SetCamera();

			D3DXMATRIX matViewProj = pCam->GetViewMatrix() * pCam->GetProjectionMatrix();
			p->SetMatrix(stEffectParams.hMatViewProj, &matViewProj);
			p->CommitChanges();
		}

		hr = p->SetTechnique(stEffectParams.hTecPreparation);
		KG_COM_PROCESS_ERROR(hr);
		hr = p->Begin(NULL, 0);
		KG_COM_PROCESS_ERROR(hr);

		UINT uPassToUse = m_bCanUseTwoSidedStencil ? 0 : 1;//֧��˫��Stencil����һ�ξͻ��꣬

		hr = p->BeginPass(uPassToUse);
		if (FAILED(hr))
		{
			p->End();
		}
		KG_COM_PROCESS_ERROR(hr);	
	}
Exit1:
	return S_OK;
Exit0:
	return E_FAIL;
}


HRESULT KG3DModelShadowRendererSolid::RenderShadowEnd()
{
	LPD3DXEFFECT pEffect = m_ShaderHolder.GetEffect();
	_ASSERTE(NULL != pEffect);
	pEffect->EndPass();
	pEffect->End();

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//��ԭ��Ĭ�ϵ���û��
	return S_OK;
}

HRESULT KG3DModelShadowRendererSolid::RenderShadow( const D3DXVECTOR3& vecPos, FLOAT fScaleFactor
											  , const D3DXVECTOR3& vecCamPos, LPD3DXEFFECT pEffect)
{
	INT nLOD = -1;
	if (m_bCanUseTwoSidedStencil)
	{
		nLOD = PrepareParamsForEachShadow(vecPos, fScaleFactor, vecCamPos, pEffect);
		if(nLOD < 2)
			RenderShadowVolume();
	}
	else	//�������ʹ��˫��Stencil�������ٶȻ��󽵵�
	{
		nLOD = PrepareParamsForEachShadow(vecPos, fScaleFactor, vecCamPos, pEffect);

		if (nLOD < 2)
		{
			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			g_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR);

			RenderShadowVolume();

			//������
			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			g_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);

			RenderShadowVolume();
		}
	}

	return S_OK;
}

INT KG3DModelShadowRendererSolid::PrepareParamsForEachShadow(const D3DXVECTOR3& vecModelPos
															 , FLOAT fShadowScaleFactor
															, const D3DXVECTOR3& vecCamPos
															, LPD3DXEFFECT pEffect)
{	

	FLOAT CurModelViewDistance = D3DXVec3Length(&(vecModelPos - vecCamPos));
	FLOAT fLODFactor = CurModelViewDistance > 0 ? fShadowScaleFactor / CurModelViewDistance : FLT_MAX;	

	if (fLODFactor - m_SVParams.fLod1TangentSqure < 0)
		return 2;
	INT nLODReturn = 0;
	if(fLODFactor - m_SVParams.fLod0TangentSquare < 0)
		nLODReturn = 1;


	//����Ӱ�� //<TEMP>

	_ASSERTE(NULL != pEffect);
	D3DXVECTOR4 vVecTemp(vecModelPos, fShadowScaleFactor); 
	pEffect->SetVector(stEffectParams.hVecPosScale, &vVecTemp);

	pEffect->CommitChanges();
	
	return nLODReturn;
}

VOID KG3DModelShadowRendererSolid::RenderShadowVolume()
{
	_ASSERTE(NULL != m_pMesh);
	m_pMesh->DrawSubset(0);
}

HRESULT KG3DModelShadowRendererSolid::InitShadowVolume()
{
	//���ȳ�ʼ��һ�ѱ���
	//�ŵ�ԲƬ��SV�Ǹ����µ�����ӵ�Բ׶�������������Ļ���Ӱ��������С����б����Ҳ����ʾ����Բ
	//׵������������ʡ�ܶ���
	
	_ASSERTE(NULL == m_pMesh);
	SAFE_RELEASE(m_pMesh);
	/*	//ԭ�����õ���ʯ�͵���Ӱ��
	HRESULT hr = D3DXMeshCreateDiamond(g_pd3dDevice, D3DFVF_XYZ, D3DXMESH_MANAGED, m_SVParams.NumVerticesOnTop
		, 1.f, 1.f, 1.f, &m_pMesh);
	*/
	HRESULT hr = D3DXCreateSphere(g_pd3dDevice, 1.f, m_SVParams.NumVerticesOnTop
		, m_SVParams.NumVerticesOnTop, &m_pMesh, NULL);
	KG_PROCESS_ERROR(SUCCEEDED(hr) && _STRINGER(KG3DModelShadowRendererSolid::InitShadowVolume)_T("����ʧ��"));

	//����Բ׶
	{
		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, m_SVParams.fRadiusScalePercent, 1, m_SVParams.fRadiusScalePercent);
		hr = D3DXMeshTransformation(m_pMesh, &matScale, NULL);
		KG_COM_PROCESS_ERROR(hr);
		struct VisitorScaleForUpperPoints : public VisitorForD3DXVECTOR3 
		{
			virtual HRESULT Accept(const D3DXVECTOR3& vPos)
			{
				if(vPos.y > 0)
					return S_OK;
				return E_FAIL;
			}
		};

		VisitorScaleForUpperPoints vistorForUpper;
		D3DXMatrixScaling(&matScale, 1, m_SVParams.fTopHeightScalingPercent, 1);
		hr = D3DXMeshTransformation(m_pMesh, &matScale, &vistorForUpper);

		struct VisitorScaleForLowerPoints : public VisitorForD3DXVECTOR3 
		{
			virtual HRESULT Accept(const D3DXVECTOR3& vPos)
			{
				if(vPos.y < 0)
					return S_OK;
				return E_FAIL;
			}
		};

		VisitorScaleForLowerPoints vistorForLower;
		D3DXMatrixScaling(&matScale, 1, m_SVParams.fBottomHeightScalingPercent, 1);
		hr = D3DXMeshTransformation(m_pMesh, &matScale, &vistorForLower);
	}
	return S_OK;
Exit0:
	SAFE_RELEASE(m_pMesh);
	return E_FAIL;
}

HRESULT KG3DModelShadowRendererSolid::UninitShadowVolume()
{
	SAFE_RELEASE(m_pMesh);
	return S_OK;
}

HRESULT KG3DModelShadowRendererSolid::InputShadowDummy(const D3DXVECTOR3& Pos, FLOAT fScaleFactor)
{
	if(m_vecExtraModels.size() >= em_max_extra_shadows)
		return E_FAIL;

	m_vecExtraModels.push_back(D3DXVECTOR4(Pos.x, Pos.y, Pos.z, fScaleFactor));
	return S_OK;
}


bool KG3DModelShadowRenderer::IsModelShadowNeedToBeRender(KG3DModel& modelRef)
{
	if(modelRef.GetType() != MESHTYPE_MODELST)
		return false;

	if(modelRef.GetBindType()!=ENUM_BIND_NONE)
		return false;
	/*if(0 == modelRef.GetNumModel())
		return false;*/

	return true;
}


HRESULT KG3DModelShadowRendererSolid::Render(KGCH::TFrontAccessProxy<KG3DModel*>& SortedModelsIt )
{	
	KG_PERF_START(FALSE);

	HRESULT hr = E_FAIL;
	KG3DCamera* pCam = g_cGraphicsTool.GetCamera();
	KG_PROCESS_ERROR(NULL != pCam);
	KG_PROCESS_ERROR(g_cEngineOption.ModelShadowType == EM_MODEL_SHADOW_TYPE_LOW);
	KG_PROCESS_ERROR(! g_cEngineOption.bPostEffectEnable && _T("�����������õ���Ⱦ����������"));

	hr = this->RenderShadowBegin();
	KG_COM_PROCESS_ERROR(hr);
	{
		//����Ϳ��Stencil
		D3DXVECTOR3 vecCamPos = pCam->GetPosition();

		LPD3DXEFFECT pEffect = m_ShaderHolder.GetEffect();
		_ASSERTE(NULL != pEffect);

		KG_PERF_SAMPLE();

		_ASSERTE(SortedModelsIt.IsContentThisType(typeid(KG3DModel*)));
		for (SortedModelsIt.Begin(); ! SortedModelsIt.IsEnd(); SortedModelsIt.StepForward())
		{
			KG3DModel* p = SortedModelsIt.GetCur();
			_ASSERTE(NULL != p);
			
			if (! IsModelShadowNeedToBeRender(*p))
				continue;
			
			//��ȡ��World�����еķ�������
			FLOAT fModelScaling = 1;
			{
				D3DXVECTOR3 vScale(1,0,1);
				p->KG3DTransformation::GetScaling(&vScale);
				fModelScaling = min(vScale.x, vScale.z);
				MT_LIMIT_TO_RANGE(fModelScaling, em_max_shadow_scale_factor, 0);
			}

			FLOAT fScaleFactor = p->m_fShadowScaleFactor * fModelScaling;
			
			this->RenderShadow(D3DXMatrixGetTranslationPart(p->m_matWorld), fScaleFactor, vecCamPos, pEffect);
		}

		KG_PERF_SAMPLE();

		if (! m_vecExtraModels.empty())
		{
			for (size_t i = 0; i < m_vecExtraModels.size(); ++i)
			{
				const D3DXVECTOR4& vec4 = m_vecExtraModels[i];

				this->RenderShadow((const D3DXVECTOR3&)vec4, vec4.w, vecCamPos, pEffect);
				//������Щ�����Ӱ�ӣ��༭�����õ�
			}
			
		}

		KG_PERF_SAMPLE();
		
		this->RenderShadowEnd();

		bool bUseTwoRenderTarget 
			= (D3DMULTISAMPLE_NONE == g_cGraphicsEngine.GetPresentParameters().MultiSampleType);

		UINT uBackBufferWidth = g_cGraphicsEngine.GetPresentParameters().BackBufferWidth;
		UINT uBackBufferHeight = g_cGraphicsEngine.GetPresentParameters().BackBufferHeight;

		RenderShadowToBackBuffer(bUseTwoRenderTarget, uBackBufferWidth, uBackBufferHeight);

		KG_PERF_SAMPLE();

		KG_PERF_STOP();
	}
	m_vecExtraModels.clear();
	return S_OK;
Exit0:
	m_vecExtraModels.clear();
	return E_FAIL;
}

KG3DModelShadowRendererSolid* KG3DModelShadowRendererSolid::Build( LPCTSTR, DWORD_PTR, DWORD_PTR )
{
	KG3DModelShadowRendererSolid* p = new KG3DModelShadowRendererSolid(g_GetSystemEventListener());
	HRESULT hr = E_FAIL;
	KGLOG_PROCESS_ERROR(NULL != p && _T("KG3DModelShadowRendererSolid��ʼ��ʧ��"));
	hr = p->Init();
	KG_COM_PROCESS_ERROR(hr);
	return p;
Exit0:
	SAFE_RELEASE(p);
	return NULL;
}

void __stdcall KG3DModelShadowRendererSolid::OnReceiveMessage( const KG3DMessage& Message, IKG3DMessageSenderBase* pSender )
{
	if (Message.m_uMessage == KM_D3DDEVICE_RELEASE)
	{
		this->UnInit();
	}
}

ULONG KG3DModelShadowRendererSolid::SynchronizedStrenchRect( BOOL bSynchronized )
{
	bool bOld = m_bSynchronizedStenchRect;
	m_bSynchronizedStenchRect = bSynchronized ? true : false;
	return bOld;
}

