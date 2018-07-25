////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffLight.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-11-22 10:53:37
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DENVEFFLIGHT_H_
#define _INCLUDE_KG3DENVEFFLIGHT_H_

////////////////////////////////////////////////////////////////////////////////

#include "KG3DEnvEffect.h"
#include "IEKG3DEnvEffLight.h"
//#include "TemplateLibs/LOKI_APP/KG3DLokiStructInterface.h"

namespace KG3D_ENVIRONMENT_EFFECT
{
	//struct KG3DEnvEffLightDescStruct0	//Ϊ�˼�����ǰ��KG3DLight�Ķ�ȡ����� 
	//{
	//	//�ļ�ͷ
	//	struct Header
	//	{
	//		enum {	LIGHT_FILE_MASK = 0x00004c54,	CURRENT_VERSION = 0x0,	};
	//		DWORD dwMask;
	//		DWORD dwVersion;
	//		Header(){dwMask = LIGHT_FILE_MASK; dwVersion = CURRENT_VERSION;	}
	//	};
	//	//��չ�
	//	struct _SkyLightInfo {

	//		D3DCOLORVALUE   Diffuse;
	//		float           fPower;//���˶�ȡ��û���õ�

	//	};
	//	_SkyLightInfo   m_SkyLightInfo;

	//	//����
	//	struct _ComLightInfo {

	//		D3DCOLORVALUE   Diffuse;
	//		float           fPower;//���˶�ȡ��û���õ�

	//	};
	//	_ComLightInfo   m_ComLightInfo;

	//	D3DLIGHT9       m_D3DLight;

	//	BOOL            m_bVolumnLight;     //ֻ����Render������ȥ��//�Ƿ��������
	//	D3DXVECTOR3     m_VolumnLightSzie;  //ֻ����Render������ȥ��//�����Դ�ĳߴ�
	//	BOOL            m_bEnabled;			//��������ƹ��ApplyLight
	//	BOOL            m_bCastShadow;      //���ã����ǿ���δ��ʹ�á��Ƿ������Ӱ
	//	DWORD           m_dwSceneAmbient;	//�ڲ����ã�����GetSet

	//	void SetDefault();
	//	
	//	//�ɵĶ�д
	//	HRESULT LoadFromFile(LPCTSTR tczName);
	//	HRESULT SaveToFile(LPCTSTR tczName);
	//};//KG3DEnvEffLightDescStruct0

	//////////////////////////////////////////////////////////////////////////

	struct KG3DDWGC_Key_Light;
	struct KG3DEnvEffLightDescStruct : public KG3DENVEFFLIGHT_DESCSTRUCT
	{
		void SynWithKey(const KG3DDWGC_Key_Light& Other);
		void SynWithLightParam(const KG3DLightParam& lightParam);
		void ToKey(KG3DDWGC_Key_Light& Other)const;
		void ToLightParam(KG3DLightParam& lightParam)const;
		//void SynFrom(const KG3DEnvEffLightDescStruct0& Other);
		void ToBaseStruct(KG3DENVEFFLIGHT_DESCSTRUCT& Other)const;		

		KG3DEnvEffLightDescStruct();
		KG3DEnvEffLightDescStruct(const KG3DENVEFFLIGHT_DESCSTRUCT& Other){this->operator=(Other);}
		friend bool operator==(const KG3DEnvEffLightDescStruct& a, const KG3DEnvEffLightDescStruct& b);
		friend bool operator!=(const KG3DEnvEffLightDescStruct& a, const KG3DEnvEffLightDescStruct& b);
	
		//С�Ķ�m_dwModificationOption�ĸĶ�
		
		void SetDefault();

		void operator=(const KG3DENVEFFLIGHT_DESCSTRUCT& Other)
		{
			BOOL bIsLightEnable = Other.m_dwModificationOption & KG3DENVEFFLIGHT_MDO_ENABLELIGHT;
			KG_CUSTOM_HELPERS::TTypeCopy<KG3DENVEFFLIGHT_DESCSTRUCT>(this, &Other);
			this->SetLightEnable(bIsLightEnable);
		}

		//�µĶ�д
		HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection)const;
	};//KG3DEnvEffLightDescStruct

	//////////////////////////////////////////////////////////////////////////

	struct KG3DDWGC_Key_Light : public KG3DDWGC_KeyBase 
	{
		//////////////////////////////////////////////////////////////////////////
		enum{DWGCKEY_TYPE = KG3DTYPE_ENVEFF_LIGHT,};
		virtual DWORD GetType()const{return DWGCKEY_TYPE;}
		/*
		typedef LOKI_APP::StructInterface::TElementConvertor<
					TYPELIST_1(D3DCOLOR)> ConverterType;
				static ConverterType& GetConverter()
				{
					static MAKE_DESC_ARRAY_1(pDescArray, m_Ambient);
					static ConverterType Convertor(pDescArray, _countof(pDescArray));
					return Convertor;
				}*/
		
		//////////////////////////////////////////////////////////////////////////
		//����
		DWORD m_dwModificationOption;
		enum{color_num = 7};//Diffuse,Ambient,Specular,4��Shader����
		D3DCOLOR	m_Colors[color_num];
		D3DVECTOR	m_Direction;
		//����
		KG3DDWGC_Key_Light();
		bool operator==(const KG3DDWGC_Key_Light& Other){return 0 == memcmp(this, &Other, sizeof(KG3DDWGC_Key_Light));}
		bool operator!=(const KG3DDWGC_Key_Light& Other){return ! this->operator==(Other);}
		inline D3DCOLOR& GetDiffuse(){return m_Colors[0];}
		inline D3DCOLOR& GetAmbient(){return m_Colors[1];}
		inline D3DCOLOR& GetSpecular(){return m_Colors[2];}
		inline size_t GetColorArgCount(){return color_num - 3;}
		inline D3DCOLOR& GetColorArg(size_t i){_ASSERTE(i < GetColorArgCount() && i + 3 < color_num); return m_Colors[i + 3];}

		virtual bool CalculateInterp(
			const KG3DDWGC_KeyBase& BaseKeyBefore, 
			const KG3DDWGC_KeyBase& BaseKeyAfter,
			float fPercent);

		bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		///<DW_Controller>
#if defined(DEBUG) | defined(_DEBUG)
		void DebugPrint(std::ostringstream& OSS);
#endif	
	};//KG3DDWGC_Key_Light

	//////////////////////////////////////////////////////////////////////////

	class KG3DEnvEffLight : public KG3DEnvEffect, public IEKG3DEnvEffLight	//IEKG3DEnvEffLight��typedef��IEKG3DLight
	{
		KG_TEST_DECLARE_CLASS_TEST(KG3DEnvEffect)
	public:
		KG3DEnvEffLight();
		~KG3DEnvEffLight(void);

		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext);
		virtual HRESULT	FrameMove();
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface);
		//<�ӿ�>
		virtual HRESULT RequestModification(const KG3DENVEFFLIGHT_DESCSTRUCT* pDescStruct);
		virtual HRESULT GetStruct(KG3DENVEFFLIGHT_DESCSTRUCT* pDescStruct);
		virtual HRESULT Copy(IEKG3DEnvEffLight* pOtherLight);//ֻ��SceneEditor������һ���ط����õ�
		//</�ӿ�>
		inline HRESULT RequestModification(INT){return this->RequestModification((KG3DEnvEffLightDescStruct*)NULL);}//���ڽ��NULL��ͻ	
		HRESULT RequestModification(const KG3DEnvEffLightDescStruct* pDesc);
		//HRESULT RequestModification(const KG3DEnvEffLightDescStruct0* pDescOld);//���ھɽṹ�Ķ�ȡ

		///<DW_Controller>
		virtual HRESULT DWGCRequestPopFrontState(float fCurBlendPercent = 0.0f);
		virtual HRESULT DWGCRequestSubStateChange(const KG3DDWGC_KeyBase* pRetKey);
		virtual HRESULT DWGCGetCurStateToKey(KG3DDWGC_KeyBase* pRetKey);		
		virtual HRESULT DWGCEnableSubState(BOOL bEnable);
		virtual HRESULT DWGCSetPercent(float fPercent);
		virtual HRESULT DWGCRequestModification(const KG3DDWGC_KeyBase* pKey);
		///</DW_Controller>

		static HRESULT LoadDefaultSunAndPlayerLight(IIniFile* pDefaultIni);
		static const KG3DLightParam& GetDefaultSunParam();
		static const KG3DLightParam& GetDefaultPlayerParam();
	public:
		HRESULT	Apply(DWORD);
		//���ҪSet�Ļ���Ҫͨ��RequestModification������,����
		BOOL	SetDiffuse(D3DCOLORVALUE Diffuse);
		//��Ϊ�ڶ�̬�����仯��ʱ���������µ���Щֵ�ǻ᲻�ϱ�ģ�KeyResult�����ڻ��彥������
		//D3DCOLOR GetSkyLightDiffuse(){return m_KeyResult.GetColorArg(0);}
		//D3DCOLOR GetComLightDiffuse(){return m_KeyResult.GetColorArg(1);}
		//D3DCOLOR GetSceneAmbient(){return m_KeyResult.GetColorArg(2);}//ԭ����SceneAmbient�ŵ�Args�۵ĵ�����λ��

		//D3DCOLOR GetDiffuse(){return m_KeyResult.GetDiffuse();}
		//D3DCOLOR GetAmbient(){return m_KeyResult.GetAmbient();}
		//D3DCOLOR GetSpecular(){return m_KeyResult.GetSpecular();}
		//D3DVECTOR GetDirection(){return m_KeyResult.m_Direction;}

		const KG3DLightParam&	GetParam()const;
		HRESULT SetParam(const KG3DLightParam& lightParam);	//�������趨��ƽʱ�������Ƚ���

		static KG3DLightParam GetDefaultLightParam();
		
		//�����ҪLight������������������GetStruct��ȡ��D3DLIGHT9��Ҳ�����Լ�дGetter����Setter����ͨ��RequestModification����

		BOOL IsLightEnable(){return m_DescStruct.IsLightEnable();}
	private:
		BOOL CopyData(const KG3DEnvEffLight& Other);
		VOID SetDescPrivate(const KG3DEnvEffLightDescStruct& descIn);
	private:
		const KG3DEnvEffLightDescStruct	m_DescStruct;//д��const����ôÿ�����Ķ�����ͨ��SetDescPrivate���Ӷ��а취ͬ��һЩ����

		KG3DDWGC_Key_Light	m_KeyResult;//���ڴ�Ž��Desc��SubKey���м�״̬
		KG3DLightParam		m_lightParam;	//���ڴ�Ž��Desc��SubKey���м�״̬
		KG3DDWGC_Key_Light	m_SubKey;

		FLOAT	m_fPercent;

		enum{enable_sub_state = 0x01,};
		DWORD m_State;
		VOID EnableSubState(BOOL bEnable){bEnable ? (m_State |= enable_sub_state) : (m_State &= (~enable_sub_state));}
		BOOL IsSubStateEnable(){return m_State & enable_sub_state;}
	};

};
typedef KG3D_ENVIRONMENT_EFFECT::KG3DEnvEffLight KG3DLight;
#endif //_INCLUDE_KG3DENVEFFLIGHT_H_
