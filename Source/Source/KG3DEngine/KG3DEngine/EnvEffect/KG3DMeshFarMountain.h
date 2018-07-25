////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DMeshFarMountain.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-10-19 17:46:21
//  Comment     :
//
////////////////////////////////////////////////////////////////////////////////
# pragma once
#ifndef _INCLUDE_KG3DMESHFARMOUNTAIN_H_
#define _INCLUDE_KG3DMESHFARMOUNTAIN_H_

////////////////////////////////////////////////////////////////////////////////


//#include "KG3DMessenger.h"
#include "IEKG3DMeshFarMountain.h"
#include "KG3DEnvEffect.h"
//#include "KG3DGraphicsStructEx.h"

class KG3DTexture;
namespace GraphicsStruct
{
	class RenderStateAlphaWithoutSettingAlphaTest;
};

namespace KG3D_ENVIRONMENT_EFFECT
{
	//////////////////////////////////////////////////////////////////////////
	struct KG3DDWGC_Key_FarMount : public KG3DDWGC_KeyBase
	{
		enum { DWGCKEY_TYPE = KG3DTYPE_ENVEFF_SKYSYSTEM_FARMOUNTAIN };

		virtual DWORD GetType() const { return DWGCKEY_TYPE; }

        DWORD		m_dwModificationOption;		//ֻ�ܹ�����ɫ����ɫͬ�������
		//D3DCOLOR	m_d3dc_Color;				//����ͬ������ɫ
		FLOAT		m_fFogBlendFactor;			//����ɫ����ͼɫ�Ļ��ϵ��

		KG3DDWGC_Key_FarMount();

		virtual bool CalculateInterp(const KG3DDWGC_KeyBase& BaseKeyBefore, const KG3DDWGC_KeyBase& BaseKeyAfter, float fPercent);

		bool SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection );
		bool LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection );

#if defined(DEBUG) | defined(_DEBUG)
		void DebugPrint(std::ostringstream& oss);
#endif
	};

	//////////////////////////////////////////////////////////////////////////
	struct KG3DMeshFarMountDescStruct : public KG3DMESHFARMOUNTAIN_DESCSTRUCT
	{
		//��Ҫ��ͼ�����κ����麯�����࣬�������е���������ؾͲ�������������
		KG3DMeshFarMountDescStruct();		
		KG3DMeshFarMountDescStruct( const KG3DMeshFarMountDescStruct &OtherStruct );
		KG3DMeshFarMountDescStruct( const KG3DMESHFARMOUNTAIN_DESCSTRUCT &OtherStruct );
		void SynWithKey(const KG3DDWGC_Key_FarMount& Key);///<DW_Frame>
		void operator=( const KG3DMeshFarMountDescStruct& OtherStruct );
		void operator=( const KG3DMESHFARMOUNTAIN_DESCSTRUCT& OtherStruct );
		void ToBaseStruct( KG3DMESHFARMOUNTAIN_DESCSTRUCT &OtherStruct );
		void ToKey(KG3DDWGC_Key_FarMount& OtherStruct);///<DW_Frame>
		bool SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection );
		bool LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection );
		bool SetDefaultValue();
	};


    class KG3DMeshFarMountain : 
        public KG3DEnvEffect,
        //public KG3DMessageSingleSenderReceiver, 
        public IEKG3DMeshFarMountain
	{
	private:
		//BBox		m_BBox;
		//LPD3DXMESH	m_pD3DXMesh;
		struct TypeVertex 
		{
			FLOAT x,y,z;
			FLOAT uPercent;
		};
		enum{
			em_slice = 32,
			em_fvf = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE1(0),
			
		};
		LPDIRECT3DVERTEXBUFFER9	m_pVB;
		KG3DMeshFarMountDescStruct	m_DescStruct;	
		KG3DTexture* m_pColorMap;
		//unsigned	m_uTextureHeight;
		//unsigned	m_uTextureWidth;
		//bool	m_bIsTextureLoaded;
		//TCHAR	m_tczTexturePathBuffer[MAX_PATH];
		
		bool	m_bIsInitialized;
		TCHAR	m_tczName[32];				//���ڱ�־����

		//D3DCOLOR m_d3dc_FogBlendedColor;	//DescStruct�еĲ���FogColor��FogBlendFactor���֮��õ��Ľ��
		//bool	m_bBlockReloadMessage;		//������������ʱ�����������ص���Ϣ

	//public:
	/*	struct RenderState
		{
			GraphicsStruct::RenderStateAlphaWithoutSettingAlphaTest	m_AlphaState;
			GraphicsStruct::SamplerStateAddressVClamp m_AddressVClamp;
			RenderState();
			~RenderState();
		};*/

	public:
		///<�ӿ�>
		virtual HRESULT GetName( LPTSTR szName, UINT uBufferCount );
		virtual HRESULT SetName( LPTSTR szName );
		virtual HRESULT	RequestModification( const KG3DMESHFARMOUNTAIN_DESCSTRUCT* pStruct );///������pStruct�е�ModificationOption�趨��ʲô����
		virtual HRESULT	GetStruct( KG3DMESHFARMOUNTAIN_DESCSTRUCT* pResultStruct );
		///</�ӿ�>

		///<KG3DEnvEffect>
        virtual HRESULT OnLostDevice();
        virtual HRESULT OnResetDevice();
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext);
		virtual HRESULT FrameMove();
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection );
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection );
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface);
		///</KG3DEnvEffect>

		LPTSTR	Name() { return m_tczName; }
		const KG3DMeshFarMountDescStruct* GetStruct(){ return &m_DescStruct; };
		HRESULT RequestModification( const KG3DMeshFarMountDescStruct* pStruct );
		
		///<��̬����>
		///<DW_Controller>
	private:
		KG3DDWGC_Key_FarMount m_SubKey;
		bool	m_bEnableBlending;
		//bool	m_bIsSubStateInitialized;
		float	m_fBlendPercent;

	public:
		typedef	KG3DDWGC_Key_FarMount	TypeKey;		

	public:
		HRESULT DWGCRequestPopFrontState(float fCurBlendPercent = 0.0f);
		HRESULT DWGCRequestSubStateChange(const KG3DDWGC_KeyBase* pRetKey);
		HRESULT DWGCGetCurStateToKey(KG3DDWGC_KeyBase* pRetKey);		
		HRESULT DWGCEnableSubState(BOOL bEnable);
		HRESULT DWGCSetPercent(float fPercent);
		HRESULT DWGCRequestModification(const KG3DDWGC_KeyBase* pKey);
		///</DW_Controller>

		///<��̬����>
	private:
		HRESULT CustomInit();
		HRESULT SetTexture( LPCTSTR tczTexturePath );///����������·���������g_Def_WorkDirectory
		//HRESULT ProcessMesh(LPD3DXMESH	pMeshIn);///�½���ʱ�������õ�
		//HRESULT ModifyMesh(const float* pfRadius = NULL, const float* pnHeight = NULL, const float* pBotmZPos = NULL, const float *pfZRotation = NULL, const float *pfTextureCordUTiles = NULL );///�ı���״����Ƚϲ����Ƿ���ڲ���Structһ�£����һ�¾Ͳ���ᣬ����ָ��bForceModify
		//HRESULT ModifyTexture(const D3DCOLOR* pColor = NULL, const float* pBotmBlendHeight = NULL, const D3DCOLOR* pFogColor = NULL, const float* pfFogBlendFactor = NULL );///������ͼ�·��Ľ���,��Ƚϲ����Ƿ���ڲ���Structһ�£����һ�¾Ͳ���ᣬ����ָ��bForceRedraw
		//inline void ModifyFinalBlendedColor(D3DCOLOR SurfaceColor, D3DCOLOR FogColor, float fPercent);
		//virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender );///����IKG3DMessageReceiver�е�ͬ������
	
		HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption ) { return S_OK; };///����������

	public:
		KG3DMeshFarMountain(); 
		~KG3DMeshFarMountain();
	};

};
#endif //_INCLUDE_KG3DMESHFARMOUNTAIN_H_
