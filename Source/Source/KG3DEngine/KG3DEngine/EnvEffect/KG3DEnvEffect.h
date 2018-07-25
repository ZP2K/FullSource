////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffect.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-12-8 9:43:59
//  Comment     : 
/************************************************************************/
/*������һ��Effect�ķ�����
1.����Effect�������һ���������еĵط���ǿת�����Ҫȥ��������KG3DTypes�м�һ��Type���ڹ��캯���е���KG3DEnvEffect�Ĺ��캯����ʼ��Type
2.��KG3DCollocator��ע����������������
3.����Render��һ��Ĵ��麯�����������Effect����дGetSubEffect��Щ����
4.��Environment��AddEnvEffect�м���switch
5.�����Ҫ��Scene��ͳһGet�ӿڣ���IEKG3DTypes�м�Type,����Scene�м�Switch,�����EnvEffect�����������ʲôҲ��д
6.���Ҫʹ�ö�̬������
	1)������Key���ͣ�ʵ��EnvEffect�ļ�����̬������ط���������TypeKey
	2)KG3DDWGC_KeyTrack::AddKeyTables
7.���Ҫʹ���Լ���RTTI����KG3DRTTIType.cpp��ADD_RTTI

�򵥵Ŀ�����<KG3DEnvEffect SubClass Register Point>��������д������Щ������Ŀ��
*/
/************************************************************************/
//
////////////////////////////////////////////////////////////////////////////////
#pragma  once
#ifndef _INCLUDE_KG3DENVEFFECT_H_
#define _INCLUDE_KG3DENVEFFECT_H_
//#include "FileType.h"
#include "IEKG3DEnvironment.h"
#include "KG3DEnvEffDW_KeyBase.h"

namespace KG3D_ENVIRONMENT_EFFECT
{
	class KG3DEnvEffect;
	//ѡ��ʵ��һ�֣�С�Ĳ�������Ϊ���Ǵ��飬����û�а취�����ڼ����������û�и�����ȷ���麯��
	struct KG3DEnvEffectVisitor
	{
		virtual HRESULT Accept(KG3DEnvEffect& Eff){return S_OK;}
		virtual HRESULT Accept(KG3DEnvEffect& Eff, const KG3DENVEFF_HANDLE& HandleOfTheEff){return S_OK;}
		virtual ~KG3DEnvEffectVisitor() = 0{}
	};//���е�Visitor����Traverse�����ĵ����������ģ������ǲ���Ҫ������������

	//�������Խ��м�飬��û����ʵ��������±Ƚϵ�Ч�����˱༭��صĴ���֮�⣬Ӧ������ʵ���еõ����ԡ�
	class KG3DEnvEffectClassLibrary 
	{
	public:
		ULONG GetTypeCount();
		KG3DEnvEffectClassProperty*  GetProperty(DWORD dwType);
		BOOL	IsTypeTop(DWORD dwType);
		KG3DEnvEffectClassProperty*  GetFirstTypeProperty();
		KG3DEnvEffectClassProperty*  GetNextTypeProperty(KG3DEnvEffectClassProperty* pPreProperty);
		BOOL	RegisterType(KG3DEnvEffectClassProperty& propertyInput);

		KG3DEnvEffectClassLibrary();
	private:
		size_t m_uCount;
		KG3DEnvEffectClassProperty	m_arrayProperties[KG3DENVEFF_MAX];
	};

	KG3DEnvEffectClassLibrary& g_GetEnvEffectClassLibrary();

	struct KG3DDWGC_KeyBase;
	struct KG3DEnvEffRenderContext;
	class KG3DEnvEffect : public IEKG3DEnvEffect///ע������������������ǰ�棬ֱ��ǿת�Ϳ��Եõ��ڲ���ʵ��
	{
		/************************************************************************/
		/*TypeTreeNode���ڵ�Ŀ��������ע�ᣬ����Factory���������������������
		���ٲ��ң�·�ɱ�
		KG3DEnvEffect�Ǹ߶�Ϊ2������������ϸ�����3����������KG3DEnvironment����
		��ΪTop��EnvEffect����������һϵ�е���EnvEffect��ͨ����������Ϊ��
		������һЩͳһ������
		*/
		/************************************************************************/
		//<TypeTreeNode>
	public:
		static std::tstring	GetHandleString(KG3DENVEFF_HANDLE Handle);//���ԷǾ�̬��
		static KG3DENVEFF_HANDLE	GetHandleFromString(LPCTSTR Str);//���󷵻�KG3DENVEFF_TYPE(KG3DTYPE_ENVEFF_INVALID_EFFECT, 0)//���ԷǾ�̬��
		//</TypeTreeNode>

		
	//////////////////////////////////////////////////////////////////////////
	private://��շ���
		KG3DEnvEffect();
		KG3DEnvEffect(const KG3DEnvEffect&);//��������
		KG3DEnvEffect& operator=(const KG3DEnvEffect&);
	protected:
		KG3DEnvEffect(int nID, KG3DENVEFF_TYPE emType);//�������������ʹ��,����ֱ���½�,Type�������������

	public:
		virtual ~KG3DEnvEffect();

	private:
		int m_nID;
		DWORD	m_dwType;
		/************************************************************************/
		/*
		KeyTable��EnvEffect��1��1��صģ���EnvEffect������ʱ��KeyTableҲӦ�ö�Ӧ������
		����Ҫ�����󶨡�
		����������ȡ�ؼ�֡TableҲ��ܷ���
		*/
		/************************************************************************/
		//<KeyTable>	
		KG3D_CUSTOM_HELPERS::TKG3DLifeTimeDependedPointer<KG3DDWGC_KeyTableGeneratorBase*>	m_pKeyTableGenerator;
		KG3D_CUSTOM_HELPERS::TKG3DLifeTimeDependedPointer<KG3DDWGC_KeyTableBase*>	m_pKeyTable;
		//</KeyTable>
	protected:
		enum
		{
			EM_ENABLE = 1,
			EM_ENABLE_CONTROL_BY_DW  = 2,
			EM_IS_ID_AND_TYPE_INITIALIZED = 4,
			EM_IS_DWGC_AVAILABLE = KG3DENVEFF_STATE_FILTER_DWGC_AVAILABLE,
		};
		KG_CUSTOM_HELPERS::KGBitSet<32>	m_BitSet;

	public:
		enum{ MAX_NAME = 32 };

	public:
		BOOL IsEnable(){return m_BitSet.Is<EM_ENABLE>();}
		BOOL SetEnable(BOOL bEnable);
		ULONG GetType(){return m_dwType;}
	
		void SetEnableControlByDynamicWeather(BOOL bEnable);
		inline BOOL IsEnableControlByDynamicWeather() { return m_BitSet.Is<EM_ENABLE_CONTROL_BY_DW>(); }
		inline BOOL IsDWGCAvailable() { return m_BitSet.Is<EM_IS_DWGC_AVAILABLE>(); }

	public:
		//virtual STDMETHOD(Release) = 0;
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext) = 0;
		virtual HRESULT	FrameMove() = 0;
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection) = 0;
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection) = 0;
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface) = 0;///ʹ������������Եõ�EnvEffect��Ӧ�����IE�ӿ�

        virtual HRESULT OnLostDevice();
        virtual HRESULT OnResetDevice();		
		virtual HRESULT PostRender();			
		
		//����Effect���븲�����溯���Ĺ�����
		virtual KG3DEnvEffect* GetSubEffect(DWORD dwType, int nFindex){return NULL;}
		virtual KG3DEnvEffect* GetSubEffect(INT nIndex, KG3DENVEFF_HANDLE* pHandle = NULL){return NULL;}///���һ����������ΪNULL
		virtual INT GetSubEffectCount(){return 0;}
		virtual INT GetSubEffectCount(DWORD dwSubType){return 0;}
		virtual VOID GetSubEffectTypeList(std::vector<DWORD>& Types){}
		virtual VOID GetExistSubEffectTypeList(std::vector<DWORD>& Types){}
		
		//Ҫʹ�ö�̬�������븲������ĺ���
		/*
		��̬������4��Ԫ����ɣ�CurState,SubState��һ��Percent��һ��EnableSubState���
		����State�ֱ𱣴����ں���Ҫ�仯����״̬��
		Percent�������������ʱ�䣬Effect��������State֮��ʲôλ�á�ֵ��Ϊ0-1
		EnableSubState��ʾ�Ƿ���Ҫ����SubState��Ⱦ���ڲ����б仯��ʱ����ر�SubState���Լ���
		*/
		///<DW_Controller>
		virtual HRESULT DWGCRequestPopFrontState(float fCurBlendPercent = 0.0f){return E_FAIL;}//Ҫ��SubState������CurState�������CurBlendPercent��0��1֮�䣬��ôȡ���м�״̬
		virtual HRESULT DWGCRequestSubStateChange(const KG3DDWGC_KeyBase* pRetKey){return E_FAIL;}
		virtual HRESULT DWGCGetCurStateToKey(KG3DDWGC_KeyBase* pRetKey){return E_FAIL;}		
		virtual HRESULT DWGCEnableSubState(BOOL bEnable){return E_FAIL;}
		virtual HRESULT DWGCSetPercent(float fPercent){return E_FAIL;}
		virtual HRESULT DWGCRequestModification(const KG3DDWGC_KeyBase* pKey){return E_FAIL;}
		///</DW_Controller>

	public:
		///<��Ĭ��ʵ��>
		virtual HRESULT	Enable(BOOL bEnable);
		virtual HRESULT IsEnable(BOOL* pbIsEnable);		
		//virtual	HRESULT	GetType(DWORD* pdwType);
		virtual HRESULT GetEnvEffName(LPTSTR tszName, INT nBufferSize);
		virtual HRESULT	GetID(int*	nID);
		virtual	HRESULT	EnableControlByDynamicWeather(BOOL	bEnable);
		virtual	HRESULT IsEnableControlByDynamicWeather(BOOL* pbEnable);

		virtual ULONG	GetState(size_t WhichState);
		
		//<KeyTable>
		KG3DDWGC_KeyTableBase* GetKeyTable();//�õ��ڲ��󶨵�Table
		KG3DDWGC_KeyTableBase* SetKeyTable(KG3DDWGC_KeyTableGeneratorBase& Gen, KG3DDWGC_KeyTableBase& KeyTable);
		//</KeyTable>
		///</��Ĭ��ʵ��>
		
		
		///<IE��װ>//ֻҪ������д��GetSubEffect�ķ�Com��׼�汾������Ļ�������İ汾�������
	public:
		virtual HRESULT GetSubEffect(INT nIndex, IEKG3DEnvEffect** ppEnvEffect, KG3DENVEFF_HANDLE* pHandle);///���һ����������ΪNULL
		virtual HRESULT GetSubEffectCount(INT* pnCount);
		///</IE��װ>
	};


	class KG3DEnvEffectDummy : public KG3DEnvEffect
	{
	public:
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext){return E_NOTIMPL;}
		virtual HRESULT	FrameMove() {return E_NOTIMPL;}
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection){return E_NOTIMPL;}
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection){return E_NOTIMPL;}
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface){return E_NOTIMPL;}
		KG3DEnvEffectDummy();
	};

	struct KG3DEnvEffRenderContext
	{
		D3DXMATRIX	matViewProj;
		D3DXVECTOR3	vCamPos;
		D3DXCOLOR	vFogColor;
	};
};

#endif //_INCLUDE_KG3DENVEFFECT_H_
