////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DModelShadowRendererSolid.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-9-17 15:35:13
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DMODELSHADOWRENDERER_H_
#define _INCLUDE_KG3DMODELSHADOWRENDERER_H_

#include "KG3DCommonObject.h"
////////////////////////////////////////////////////////////////////////////////
class KG3DTexture;
struct KG3DModelShadowRendererEffectParams;
class KG3DModel;

struct KG3DModelShadowRenderer : public KG3DCommonObjectSimple
{
	virtual HRESULT Render(KGCH::TFrontAccessProxy<KG3DModel*>& SortedModelsIt) = 0;	//��֤ͬʱֻ��һ��Scene���ã���Ȼ���������StrentchRect�Ĳ����첽
	STDMETHOD(FrameMove)(THIS)PURE;
	virtual ULONG STDMETHODCALLTYPE Release( void)PURE;
	virtual HRESULT InputShadowDummy(const D3DXVECTOR3& Pos, FLOAT fScaleFactor) = 0;//�ⲿ�ģ���Ҫ���ƽŵ�Ӱ�ӵģ�������Input����������Ⱦ������Input��Ȼ����Render�лᱻFlush������ΪModelShadowRender��Singleton������Ҫ�ڵ�����Ⱦ�����б��ֶ����ռ������һ��Render��Input��Render����Ȼ������������������û��������⡣
	virtual ULONG	SynchronizedStrenchRect(BOOL bSynchronized) = 0;	//���ؾɵ�״̬
	virtual ~KG3DModelShadowRenderer() = 0{}

	static bool IsModelShadowNeedToBeRender(KG3DModel& modelRef);
};

struct KG3DModelShadowRendererDummy : public KG3DModelShadowRenderer
{
	STDMETHOD_(ULONG, GetType)(){return 0;}
	virtual HRESULT Render(KGCH::TFrontAccessProxy<KG3DModel*>& SortedModelsIt){return E_FAIL;}
	STDMETHOD(FrameMove)(THIS){return E_FAIL;}
	virtual ULONG STDMETHODCALLTYPE Release( void){return 0;}
	virtual HRESULT InputShadowDummy(const D3DXVECTOR3& Pos, FLOAT fScaleFactor){return E_FAIL;}
	virtual ULONG	SynchronizedStrenchRect(BOOL bSynchronized){return TRUE;}
};

KG3DModelShadowRenderer& g_GetModelShadowRenderer();//��Ҫʱ���Զ�����
VOID	g_ModelShadowRendererRelease();


#endif //_INCLUDE_KG3DMODELSHADOWRENDERER_H_
