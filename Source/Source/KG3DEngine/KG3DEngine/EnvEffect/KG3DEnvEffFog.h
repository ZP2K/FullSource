////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffFog.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-1-15 10:55:25
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DENVEFFFOG_H_
#define _INCLUDE_KG3DENVEFFFOG_H_
#include "IEKG3DEnvEffFog.h"
#include "KG3DEnvEffect.h"
//#include "KG3DMessenger.h"
//#include "TemplateLibs/LOKI_APP/KG3DLokiStructInterface.h"
////////////////////////////////////////////////////////////////////////////////
namespace KG3D_ENVIRONMENT_EFFECT
{
	struct KG3DDWGC_Key_Fog : public KG3DDWGC_KeyBase 
	{
		enum{DWGCKEY_TYPE = KG3DTYPE_ENVEFF_FOG,};
		virtual DWORD GetType()const{return DWGCKEY_TYPE;}

		DWORD m_dwModificationOption;
		///KG3DENVEFFFOG_MDO_FOG_BASE
		D3DCOLOR   m_FogColor;
		///KG3DENVEFFFOG_MDO_MODE_PARAM
		FLOAT      m_fFogStartValue;
		FLOAT      m_fFogEndValue;

		KG3DDWGC_Key_Fog();

		virtual bool CalculateInterp(
			const KG3DDWGC_KeyBase& BaseKeyBefore, 
			const KG3DDWGC_KeyBase& BaseKeyAfter,
			float fPercent);

		bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);

#if defined(DEBUG) | defined(_DEBUG)
		void DebugPrint(std::ostringstream& OSS);
#endif	
	};
	//////////////////////////////////////////////////////////////////////////
	struct KG3DEnvEffFogDescStruct: public KG3DENVEFFFOG_DESCSTRUCT  
	{
		KG3DEnvEffFogDescStruct();
		KG3DEnvEffFogDescStruct(const KG3DEnvEffFogDescStruct& OtherStruct);
		KG3DEnvEffFogDescStruct(const KG3DENVEFFFOG_DESCSTRUCT& OtherStruct);
		void SynWithKey(const KG3DDWGC_Key_Fog& Key);///<DW_Frame>
		void operator=(const KG3DEnvEffFogDescStruct& OtherStruct); 
		void operator=(const KG3DENVEFFFOG_DESCSTRUCT& OtherStruct);
		void ToBaseStruct(KG3DENVEFFFOG_DESCSTRUCT& OtherStruct);
		void ToKey(KG3DDWGC_Key_Fog* pKey);///<DW_Frame>
		bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
	};

    class KG3DEnvEffFog : 
        public KG3DEnvEffect, 
        public IEKG3DEnvEffFog
	{		
	public:
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext);
		virtual HRESULT	FrameMove();
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface);
		
		///IEKG3DEnvEffFog�Ľӿ�
		virtual HRESULT RequestModification(const KG3DENVEFFFOG_DESCSTRUCT* pDescStruct, DWORD dwModificationOption);
		virtual HRESULT GetStruct(KG3DENVEFFFOG_DESCSTRUCT* pDescStruct);
		virtual HRESULT GetCaps(KG3DENVEFFFOG_CAPS_STRUCT* pRetCaps);
		virtual HRESULT GetFogColor(D3DCOLOR* pRetColor);
		virtual HRESULT SetFogColor(D3DCOLOR Color);
		
		///�ǽӿ�
		HRESULT SetFogParam(const KG3DFogParam& Param);
		KG3DFogParam GetFogParam();
		HRESULT SetDistancePercent(FLOAT fPercent);//�����ڿ��Ӿ����Զ��ʱ��ı����Զ��
		HRESULT RequestModification(const KG3DEnvEffFogDescStruct& DescStruct, DWORD dwModificationOption);
		const KG3DEnvEffFogDescStruct* GetStruct();
		HRESULT Apply();	//Ӧ���ڲ��Ĳ�������ȫ�ֵ�Fog��һ�㲻��Ҫ�ر��ר�ŵ��ã�Fog���Լ�����Apply��

		static KG3DFogParam GetDefaultFogParam();

		//������ˮ�µ�ʱ�򣬱��ⲿ����Fog�������������֮��ǵ����ʵ���ʱ���ͷ�
		HRESULT DemandFullControl();
		ULONG	IsInFullControl();
		HRESULT ReleaseFullControl();

		KG3DEnvEffFog();
		~KG3DEnvEffFog();
		
		///<��̬����>
	private:///���ڶ�̬����������
		KG3DDWGC_Key_Fog m_SubKey;
		KG3DDWGC_Key_Fog m_curKey;
		BOOL	m_bEnableBlending;
		float	m_fBlendPercent;
	public:
		typedef		KG3DDWGC_Key_Fog	TypeKey;		
	public:
		HRESULT DWGCRequestPopFrontState(float fCurBlendPercent = 0.0f);
		HRESULT DWGCRequestSubStateChange(const KG3DDWGC_KeyBase* pRetKey);
		HRESULT DWGCGetCurStateToKey(KG3DDWGC_KeyBase* pRetKey);		
		HRESULT DWGCEnableSubState(BOOL bEnable);
		HRESULT DWGCSetPercent(float fPercent);
		HRESULT DWGCRequestModification(const KG3DDWGC_KeyBase* pKey);		
		///</��̬����>
	private:
		HRESULT GetCaps(KG3DENVEFFFOG_CAPS_STRUCT& CapsStruct);
		static FLOAT CalFogEnd(FLOAT fEnd, FLOAT fDisPercent);
	private:
		KG3DEnvEffFogDescStruct     m_DescStruct;
		KG3DENVEFFFOG_CAPS_STRUCT   m_CapsStruct;
		FLOAT						m_fDistancePercent;//���ڸ���ZFar����
		bool						m_bIsUnderFullControl;
	};
};

typedef KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffFog KG3DFog;//ռס������֣�˭Ҳ���ã���Ȼ�����������ƣ��������Ƶ������ͬһ�������棬�Ǿ������������ˡ�Ҫ��Fog���ֵģ����������������

#endif //_INCLUDE_KG3DENVEFFFOG_H_
