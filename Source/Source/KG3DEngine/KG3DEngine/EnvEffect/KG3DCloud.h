////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DCloud.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-10-18 11:32:12
//  Comment     : С��RequestModification��NULLֵ����������Ե����⣺������
//					RequestModification����Type����NULL���ķ�ʽ�����
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DCLOUD_H_
#define _INCLUDE_KG3DCLOUD_H_

////////////////////////////////////////////////////////////////////////////////
#include "KG3DMesh.h"

#include "IEKG3DCloud.h"
#include "IEKG3DCloud.h"
#include "KG3DGraphicsStructEx.h"
#include "KG3DEnvEffect.h"
#include "KG3DShaderHolder.h"

class KG3DTexture;

namespace KG3D_ENVIRONMENT_EFFECT
{
	class CloudShaderParams;
	struct KG3DCloudDescStruct : public KG3DCLOUD_DESCSTRUCT
	{
		KG3DCloudDescStruct();		
		KG3DCloudDescStruct( const KG3DCloudDescStruct &OtherStruct );///�������캯��
		KG3DCloudDescStruct( const KG3DCLOUD_DESCSTRUCT &OtherStruct );
		void operator=( const KG3DCloudDescStruct& OtherStruct );
		void operator=( const KG3DCLOUD_DESCSTRUCT& OtherStruct );
		void ToBaseStruct( KG3DCLOUD_DESCSTRUCT &OtherStruct );
	};
	//////////////////////////////////////////////////////////////////////////
	struct KG3DDWGC_Key_Cloud : public KG3DDWGC_KeyBase 
	{
		enum{ DWGCKEY_TYPE = KG3DTYPE_ENVEFF_SKYSYSTEM_CLOUD };

		virtual DWORD GetType()const {return DWGCKEY_TYPE;}
		DWORD m_dwModificationOption;
		FLOAT	m_fVisibility;
		DWORD	m_diffuseColor;
		DWORD	m_ambientColor;

		virtual bool CalculateInterp(
			const KG3DDWGC_KeyBase& BaseKeyBefore, 
			const KG3DDWGC_KeyBase& BaseKeyAfter,
			float fPercent);

		virtual bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);

#if defined(DEBUG) | defined(_DEBUG)
		virtual void DebugPrint(std::ostringstream& oss);
#endif

	};
	//////////////////////////////////////////////////////////////////////////
	class KG3DCloud : 
		public KG3DEnvEffect, 
		public IEKG3DCloud
	{
	public:

		typedef	KG3DDWGC_Key_Cloud	TypeKey;

		enum { DWGCKEY_TYPE = TypeKey::DWGCKEY_TYPE };

		///<�ӿ�>
	public:
		virtual HRESULT	RequestModification(const KG3DCLOUD_DESCSTRUCT* pStruct);///������pStruct�е�ModificationOption�趨��ʲô����
		virtual HRESULT	GetStruct( KG3DCLOUD_DESCSTRUCT* pResultStruct );
		///</�ӿ�>

		///<��̬����>
	private:
		TypeKey m_SubKey;
		FLOAT	m_fPercent;
	public:

		HRESULT DWGCRequestPopFrontState(float fCurBlendPercent = 0.0f);
		HRESULT DWGCRequestSubStateChange(const KG3DDWGC_KeyBase* pRetKey);
		HRESULT DWGCGetCurStateToKey(KG3DDWGC_KeyBase* pRetKey);		
		HRESULT DWGCEnableSubState(BOOL bEnable);
		HRESULT DWGCSetPercent(float fPercent);
		HRESULT DWGCRequestModification(const KG3DDWGC_KeyBase* pKey);

		///</��̬����>

	public:
		const KG3DCloudDescStruct*	GetStruct();
		virtual HRESULT	RequestModification(const KG3DCloudDescStruct* pStruct);///����NULL�Ļ������³�ʼ��

		virtual HRESULT FrameMove();

		virtual HRESULT BeginRender(const D3DXMATRIX& matViewProj);
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext);
		virtual HRESULT EndRender();

		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection );
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection );
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface);

		//���ڸĸ߶ȵ�ʱ��֪ͨ�ϲ�
		BOOL IsHeightChanged();
		VOID OnHeightChangeHandled();
	private:
		//virtual void OnReceiveMessage(const KG3DMessage& Message , IKG3DMessageSenderBase* pSender);
		HRESULT SetColorTexture(LPCTSTR tszPath);
		HRESULT SetMaskTexture(LPCTSTR tszPath);
		HRESULT SetDefaultTexture(DWORD dwType);///KG3DCLOUD_MD_TEX,KG3DCLOUD_MD_MASK,֮һ
		HRESULT	SetTexture(DWORD dwType, LPCTSTR tszPath);
		HRESULT	ModifyMesh(const KG3DCloudDescStruct* pStruct = NULL);///�������NULL��ǿ���ػ棬�޸�Height��Size��Repeat
		HRESULT	ModifyMovement(const KG3DCloudDescStruct* pStruct = NULL);///�޸�Speed�ͷ���
		static	CloudShaderParams& GetShaderParams();
	public:	
		static KG3DCloud* Build(LPCTSTR, DWORD_PTR pIniFile, DWORD_PTR strSection);
		virtual ULONG STDMETHODCALLTYPE  Release();
	private:
		/*struct RenderState
		{
			GraphicsStruct::SaveMatrix	m_matTexSave;
			GraphicsStruct::TextureStageState	m_TexStageFlag;
			GraphicsStruct::FVFSetter	m_FVFSetter;
			GraphicsStruct::RenderState	m_AlphaBlendEnable;

			RenderState(bool bUseToGadRay = false);
			~RenderState();
		};*/

		enum

		{
			_KG3DCLOUD_MD_TEX,
			_KG3DCLOUD_MD_MASK,
		};

		struct KG3DCloudUpdateDataStruct ///���ڱ�����õ�����Framemove��Render������
		{
			FLOAT	m_fSpeedXcos;
			FLOAT	m_fSpeedYcos;
			D3DXMATRIX	m_matTexTrans;

			KG3DCloudUpdateDataStruct()
			{
				ZeroMemory(this, sizeof(KG3DCloudUpdateDataStruct));
				D3DXMatrixIdentity(&m_matTexTrans);
			}
		};	

		//////////////////////////////////////////////////////////////////////////
		KG3DCloudDescStruct	m_DescStruct;
		LPDIRECT3DVERTEXBUFFER9 m_lpFace;
		KG3DTexture*	m_pColorTex;
		KG3DTexture*	m_pMaskTex;
		KG3DCloudUpdateDataStruct m_UpdateDataStruct;

		TCHAR        m_tszColorTexPathBuffer[MAX_PATH];
		TCHAR        m_tszMaskTexPathBuffer[MAX_PATH];	

		INT		m_State;

		enum
		{
			emEnableSubState        = 0x01,
			emIsHeightChangeHandled = 0x02
		};

		BOOL IsSubStateEnable();
		VOID SubStateEnable(BOOL bEnable);
		BOOL IsHeightChangeHandled();
		VOID SetHeightChangeHandled(BOOL bHandled);

		KG3DShaderHolder	m_ShaderHolder;
	private:
		KG3DCloud();
		~KG3DCloud();
	};

};
#endif //_INCLUDE_KG3DCLOUD_H_
