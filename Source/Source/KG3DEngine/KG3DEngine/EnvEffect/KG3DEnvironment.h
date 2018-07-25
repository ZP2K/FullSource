////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvironment.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-11-29 16:37:14
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _INCLUDE_KG3DENVIRONMENT_H_
#define _INCLUDE_KG3DENVIRONMENT_H_

#include "IEKG3DEnvironment.h"
#include "KG3DEnvEffList.h"
#include "KG3DMessenger.h"
#include "KG3DTypes.h"
#include "KG3DEnvEffRenderer.h"


////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
//@remark : �����DynamicWeather��Environment�õĹ��õ��������൱��һ����
//enum��ָ�����pair�Ĺ������顣Ŀ������enumֵ���ٴ�ȡ�����ʵ���޹��ԡ�
//DynamicWeather�õ�����һ�������ָ�룬ֻ��ȡ�����ܽ����κ��޸�
//ֻ����Ӳ�ͬ��Effect����ֻ��һ��std�����İ�װ����Ϊû�кܴ�ͬ������ע���ͷš�
/************************************************************************/
namespace KG3D_ENVIRONMENT_EFFECT
{
	class KG3DEnvEffect;///ֻ����������
	class KG3DEnvEffFog;
	class KG3DSkySystem;
	class KG3DEnvEffLightSet;
	class KG3DEnvEffDynamicWeather;
	class KG3DEnvEffLensFlareSun;
	class KG3DEnvEffLensFlareMoon;
};

/************************************************************************/
//KG3DEnvironment,���е�KG3DEnvEffect�Ĺ�����
/************************************************************************/
class IIniFile;

class KG3DEnvironment : public IEKG3DEnvironment
{
public:
	typedef KG3D_ENVIRONMENT_EFFECT::KG3DEnvEff_List	ENVEFF_LIST;
	
	KG3DEnvironment();
	~KG3DEnvironment();

    HRESULT Init();
    HRESULT UnInit();

    HRESULT OnLostDevice();
    HRESULT OnResetDevice();

	HRESULT FrameMove();
	HRESULT Render(DWORD dwFilter = 0);
	HRESULT PostRender();	//���ں���Ⱦ
	HRESULT SaveMapSetting(IIniFile* pIniFile);
	HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
	HRESULT AddEnvEffect(DWORD dwType, IEKG3DEnvEffect** ppEnvEffect, IIniFile* pIniFile, LPCTSTR pSection);//����NULL�Ͳ�����ָ��
	//<�ӿ�>

    HRESULT RenderSun();

    const D3DXVECTOR4* GetSunPosition();

public:
	virtual HRESULT NewEnvEffect(DWORD dwType, IEKG3DEnvEffect** ppEnvEffect);
	virtual	HRESULT DelEnvEffect(DWORD dwType, INT_PTR nIndex);
	virtual HRESULT GetEnvEffect(DWORD dwType, INT nIndex, IEKG3DEnvEffect** ppEnvEffect);//nIndex����ڼ���ͬ�͵�Effect
	virtual HRESULT EnableControlByDynWeather(DWORD dwType, BOOL bEnable);
	//////////////////////////////////////////////////////////////////////////
	virtual ULONG GetTopEnvEffectCount();
	//virtual HRESULT GetTopEnvEffect(INT nIndex, IEKG3DEnvEffect** pEffect, KG3DENVEFF_HANDLE* pHandle);
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT GetEnvEffectCount(DWORD dwType, INT* pnCount);
	/*virtual HRESULT GetAllEnvEffectThatCanBeAddedCount(INT_PTR* pnResult);
	virtual HRESULT GetEnvEffectThatCanBeAdded(INT nIndex, DWORD* pdwType);*/
	virtual KG3DEnvEffectClassProperty* GetFirstClassProperty();
	virtual KG3DEnvEffectClassProperty* GetNextClassProperty(KG3DEnvEffectClassProperty* pPrev);
	//////////////////////////////////////////////////////////////////////////
	//virtual HRESULT GetFirstTopEnvEffect(KG3DENVEFF_HANDLE* pHandle, IEKG3DEnvEffect** ppEffect);
	//virtual HRESULT GetNextTopEnvEffect(KG3DENVEFF_HANDLE PrevHandle, KG3DENVEFF_HANDLE* pNextHandle, IEKG3DEnvEffect** ppEffect);

	//virtual IEKG3DEnvEffect* ResetIterator();
	//virtual IEKG3DEnvEffect* NextIterator();
	virtual HRESULT GetFirstTopEnvEffect(KGCH::IteratorHolder& itHolder, IEKG3DEnvEffect** ppEffect);
	virtual HRESULT GetNextTopEnvEffect(KGCH::IteratorHolder& itHolder, IEKG3DEnvEffect** ppEffect);
	

	virtual HRESULT Save(LPCTSTR pFilePath);
	virtual HRESULT Reload(LPCTSTR pFilePath);
private://С�������Ĵ�С����KG3DScene������CommonBuffer����̬ռ�ռ�
	ENVEFF_LIST	m_EnvEffList;

	friend KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffRenderer;
	KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffRenderer	m_Renderer;

	//���������������Ϳ���Ԥ�ȴ�����Fog��LightSet����Ҫ�Ŀռ�
	typedef KG_CUSTOM_HELPERS::TCommonParamBuffer<KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffFog, 144> TypeFogBuffer;
	typedef KG_CUSTOM_HELPERS::TCommonParamBuffer<KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffLightSet, 812> TypeLightSetBuffer;
	TypeFogBuffer m_pFogBuffer;
	TypeLightSetBuffer	m_pLightSetBuffer;
	VOID EnsureFogAndLight(IIniFile* pIniFile = NULL);//��֤Fog��LightSet�Ĵ���
	VOID EnsureDynamicWeather();
	//���ڼ��ٵķ���
//private:
//	template<DWORD _dwType> 
//    struct TEnvEffGetterBooster
//    {
//        typedef LPVOID Result; 
//    };
//
//#define	ADD_ENVEFF_GETTER_BOOSTER(TopType, ClassName)	template<>	struct TEnvEffGetterBooster<TopType>{ typedef ClassName Result; }
//
//	ADD_ENVEFF_GETTER_BOOSTER(KG3DTYPE_ENVEFF_FOG, KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffFog);
//	ADD_ENVEFF_GETTER_BOOSTER(KG3DTYPE_ENVEFF_SKYSYSTEM_SYSTEM, KG3D_ENVIRONMENT_EFFECT::KG3DSkySystem);
//	ADD_ENVEFF_GETTER_BOOSTER(KG3DTYPE_ENVEFF_DYNAMIC_WEATHER, KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffDynamicWeather);
//	ADD_ENVEFF_GETTER_BOOSTER(KG3DTYPE_ENVEFF_LIGHTSET, KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffLightSet);
//	ADD_ENVEFF_GETTER_BOOSTER(KG3DTYPE_ENVEFF_LENSFLARE_SUN, KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffLensFlareSun);
//	ADD_ENVEFF_GETTER_BOOSTER(KG3DTYPE_ENVEFF_LENSFLARE_MOON, KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffLensFlareMoon);
//
//	template<DWORD _dwType>
//	typename TEnvEffGetterBooster<_dwType>::Result* GetTopEnvEffect()
//	{
//		typedef typename TEnvEffGetterBooster<_dwType>::Result* TypeResult;
//		struct HandleGetter 
//		{
//			UINT m_Handle;
//			HandleGetter(ENVEFF_LIST& List){m_Handle = List.GetTopEnvEffHandle(_dwType); _ASSERTE(List.IsValidHandle(m_Handle));}
//		};
//		static HandleGetter handelGetter(m_EnvEffList);
//		return dynamic_cast<TypeResult>(m_EnvEffList.GetTopEnvEffect(handelGetter.m_Handle));		
//	}
public://����Ϊ�ӿڵķ���
	KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffect* GetEnvEffect(DWORD dwType, INT nIndex);
	KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffFog& GetFog();
	KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffLightSet& GetLightSet();

	HRESULT ClearEnvEffect();

private:
    DWORD m_dwSettingType;
public:
    virtual HRESULT SetSettingType(DWORD dwSettingType){m_dwSettingType = dwSettingType; return S_OK;}
    virtual DWORD GetSettingType(){return m_dwSettingType;}
};

#endif //_INCLUDE_KG3DENVIRONMENT_H_
