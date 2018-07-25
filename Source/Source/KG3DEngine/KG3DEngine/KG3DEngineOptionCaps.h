//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KG3DEngineOptionCaps.h
//  Version     :   1.0
//  Creater     :   ShenLiqiao
//  Date        :   2009/3/2 09:18:26
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KG3DENGINEOPTIONCAPS_H_
#define _KG3DENGINEOPTIONCAPS_H_

#include <Windows.h>
#include <vector>
#include "KG3DInterface.h"

class KG3DEngineOptionCaps : IKG3DEngineOptionCaps
{
public:
    KG3DEngineOptionCaps();
    ~KG3DEngineOptionCaps();

    virtual HRESULT GetEngineOptionCaps(KG3DEngineOption& EngineOption);

    HRESULT GetAdaperModeCaps(UINT uAdapter, D3DFORMAT Format);
    BOOL CheckValidateAdaptMode(UINT uWidth, UINT uHeight);

    virtual std::vector<KADAPTERMODE>* GetAdapterModes();
    virtual std::vector<int>* GetMultiSampleType();
    virtual DWORD GetMaxAnisotropy();
    virtual DWORD GetMinAnisotropy();
    virtual BOOL GetRenderGrass();
    virtual BOOL GetGrassAnimation();
    virtual BOOL GetGrassAlphaBlend();
    virtual int  GetMinTextureScale();
    virtual int  GetMaxTextureScale();
    virtual float GetMinCameraDistance();
    virtual float GetMaxCameraDistance();
    virtual float GetMinCameraAngle();
    virtual float GetMaxCameraAngle();
    virtual BOOL GetPostEffectEnable();
    virtual BOOL GetBloomEnable();
    virtual BOOL GetHDREnable();
    virtual BOOL GetDOFEnable();
    virtual BOOL GetShockWaveEnable();
    virtual BOOL GetHSIEnable();
    virtual BOOL GetMotionBlur();
    virtual BOOL GetDetail();
    virtual BOOL GetGodRay();
    virtual BOOL GetSSAO();
    virtual BOOL GetGlobalFog();
    virtual BOOL GetCurveCMYK();
    virtual int*  GetModelShadowType();
    virtual BOOL GetEnableDynamicConverMap();
    virtual BOOL GetCpuSkin();
    virtual int  GetMaxNumCpuThread();
    virtual int  GetMinNumCpuThread();
    virtual BOOL GetFlexBodySmooth();
    virtual int  GetMaxTerrainDetail();
    virtual int  GetMinTerrainDetail();
    virtual int  GetMaxVegetationDensity();
    virtual int  GetMinVegetationDensity();
    virtual int  GetMinWaterDetail();
    virtual int  GetMaxWaterDetail();
    virtual std::vector<int>* GetShaderModel();

protected:

private:

    vector<KADAPTERMODE> m_AdapterModes;
    std::vector<int>     m_vecMutliSampleType;    

    int m_nMinSampMipFilter;
    int m_nMinSampMinFilter;
    int m_nMinSampMagFilter;

    int m_nMaxSampMipFilter;
    int m_nMaxSampMinFilter;
    int m_nMaxSampMagFilter;

    DWORD m_dwMinAnisotropy;
    DWORD m_dwMaxAnisotropy;

    BOOL m_bRenderGrass;				//�Ƿ���Ⱦ��
    BOOL m_bGrassAnimation;			//�Ƿ�Բݲ��Ŷ���
    BOOL m_bGrassAlphaBlend;
    int  m_nMinTextureScale;			//���ʾ���
    int  m_nMaxTextureScale;

    float m_fMinCameraDistance;     //���������
    float m_fMaxCameraDistance;
    float m_fMinCameraAngle;        //������ӽ�
    float m_fMaxCameraAngle;

    // ����ͻ���Ҫ�ṩ������Ⱦ��ѡ�����config�ļ��н�����Ϊ1
    BOOL m_bPostEffectEnable;		//�Ƿ���������Ⱦ

    //������Ⱦ��ѡ��
    BOOL m_bBloomEnable;			//�Ƿ��ữ
    BOOL m_bHDREnable;
    BOOL m_bDOFEnable;
    BOOL m_bShockWaveEnable;		//�Ƿ�ShockWave
    BOOL m_bHSIEnable;				//�Ƿ�HSI��ɫ
    BOOL m_bMotionBlur;
    BOOL m_bDetail;
    BOOL m_bGodRay;
    BOOL m_bSSAO;
    BOOL m_bGlobalFog;
    BOOL m_bCurveCMYK;

    //ģ����Ӱ��ѡ��
    int  m_nModelShadowType[4];		//���ͣ�0��ʾ�رգ�1��ʾ�ýŵ�ƬƬ��2��ʾʵʱ��Ӱ

    BOOL m_bEnableDynamicConverMap;
    BOOL m_bCpuSkin;

    int  m_nMaxNumCpuThread;
    int  m_nMinNumCpuThread;

    BOOL m_bFlexBodySmooth;//ƽ������

    int  m_nMaxTerrainDetail;
    int  m_nMinTerrainDetail;
    int  m_nMaxVegetationDensity;
    int  m_nMinVegetationDensity;
    int  m_nMinWaterDetail;
    int  m_nMaxWaterDetail;
    std::vector<int> m_vecShaderModel;

    HRESULT GetMultiSampleTypeListCaps();
    HRESULT GetTextureSamplerFilterCaps(KG3DEngineOption& EngineOption);

    HRESULT GetRenderGrassCaps();
    HRESULT GetGrassAnimationCaps(KG3DEngineOption& EngineOption);
    HRESULT GetGrassAlphaBlendCaps(KG3DEngineOption& EngineOption);
    HRESULT GetTextureScaleCaps();
    HRESULT GetCameraDistanceCaps();
    HRESULT GetCameraAngleCaps();

    HRESULT GetPostEffectCaps(KG3DEngineOption& EngineOption);
    HRESULT GetModelShadowTypeCaps(KG3DEngineOption& EngineOption);
    HRESULT GetEnableDynamicConverMapCaps();
    HRESULT GetCpuSkinCaps();
    HRESULT GetNumCpuThreadCaps();
    HRESULT GetFlexBodySmoothCaps();
    HRESULT GetTerrainDetailCaps();
    HRESULT GetVegetationDensityCaps();
    HRESULT GetWaterDetailCaps();

    UINT    GetAllowableAdapterModeCount(UINT uAdapter, D3DFORMAT Format);
    HRESULT GetAllowableAdapterMode(D3DDISPLAYMODE* pAdapterMode, UINT* puCount, UINT uAdapter, D3DFORMAT Format); 
    HRESULT GetDepthStencilFormat(UINT uAdapter, D3DDEVTYPE DeviceType, D3DFORMAT* pDepthStencilFormat, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat);
    HRESULT GetAdapater(PUINT puAdapter, D3DDEVTYPE* pDeviceType);

    HRESULT GetPostEffectEnableCaps(D3DCAPS9 D3DCaps);
    HRESULT GetBloomEnableCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetHDREnableCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetDOFEnableCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetShockWaveEnableCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetHSIEnableCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetMotionBlurCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetDetailCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetGodRayCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetSSAOCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetGlobalFogCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);
    HRESULT GetCurveCMYKCaps(D3DCAPS9 D3DCaps, KG3DEngineOption& EngineOption);

    HRESULT GetShaderModelCaps();

	// Inherited via IKG3DEngineOptionCaps
	virtual HRESULT UpdateCaps(const KG3DEngineOption & EngineOption) override;
	virtual const KG3D_ENGINE_OPTION_CAPS * GetCaps() override;
	virtual BOOL GetEnableScaleOutput() override;
	virtual BOOL GetScaleOutputSmooth() override;
};

#endif  // _KG3DENGINEOPTIONCAPS_H_

extern KG3DEngineOptionCaps g_cEngineOptionCaps;