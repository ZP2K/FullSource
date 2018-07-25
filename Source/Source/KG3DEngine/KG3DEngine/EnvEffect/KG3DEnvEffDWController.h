////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffDWController.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-7-4 14:35:19
//  Comment     : �ѿ�������Ч������Ĺ���ί�е������ʵ��
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DENVEFFDWCONTROLLER_H_
#define _INCLUDE_KG3DENVEFFDWCONTROLLER_H_

#include "KG3DEnvEffDW_KeyTrack.h"

////////////////////////////////////////////////////////////////////////////////
#if 1
namespace KG3D_CUSTOM_HELPERS
{
	template<class, unsigned int> class TKG3DLoopQueue;
};
namespace KG3D_ENVIRONMENT_EFFECT
{
	class KG3DEnvEffect;
	class KG3DEnvEffDynamicWeather;
	
	enum
	{
		DWGC_INTERP_BLEND_SIZE = 2,
		DWGC_BLEND_GAP_HOUR2HOUR_SECOND = 3599,///��xx����fPecent��0��1
		DWGC_BLEND_GAP_REGION2REGION_SECOND = 16,///xx�����fPecent��1
	};

	enum
	{
		KG3DEnvEffDWController_MDO_GAP_H2H = 0x02,
		KG3DEnvEffDWController_MDO_GAP_R2R = 0x04,
		KG3DEnvEffDWController_MDO_GAP_BOOST = 0x08,
		KG3DEnvEffDWController_MDO_ALL = 0xff,
	};

	struct KG3DEnvEffDWControllerDescStruct
	{
		DWORD m_dwModificationOption;	///��KG3DEnvEffDWController_MDO_GAP_H2H��־�����Ǹ�h2h��Key����r2r��Key
		float m_fBlendGapH2H_ms;
		float m_fBlendGapR2R_ms;
		float m_fBlendBoost;
		KG3DEnvEffDWControllerDescStruct():m_dwModificationOption(0)
			,m_fBlendGapH2H_ms(DWGC_BLEND_GAP_HOUR2HOUR_SECOND * KG3DENVEFFTI_MILLISECOND_IN_A_SECOND)
			,m_fBlendGapR2R_ms(DWGC_BLEND_GAP_REGION2REGION_SECOND * KG3DENVEFFTI_MILLISECOND_IN_A_SECOND)
			,m_fBlendBoost(1.0f){}
	};

	class KG3DEnvEffDWController
	{
	public:
		struct BlendStruct 
		{
			DWORD m_dwMDO;
			KG3DENVEFF_DWGCKEY_HANDLE m_KeyHandle;
			DWORD_PTR m_dwExtraData;
			BlendStruct(const KG3DENVEFF_DWGCKEY_HANDLE& OtherKeyHandle, DWORD dwMDO = 0, DWORD_PTR dwExtraData = 0)
				:m_KeyHandle(OtherKeyHandle)
				,m_dwMDO(dwMDO)
				,m_dwExtraData(dwExtraData)
			{
				
			}
			BlendStruct(){}
		};
		typedef	KG3DEnvEffDynamicWeather TypeControlObj;
		enum { common_buffer_size = sizeof(BlendStruct) * DWGC_INTERP_BLEND_SIZE + sizeof(DWORD_PTR)*2,};
		typedef KG3D_CUSTOM_HELPERS::TKG3DLoopQueue<BlendStruct, DWGC_INTERP_BLEND_SIZE> TypeInterpBlendQueue;
		typedef KG_CUSTOM_HELPERS::TCommonParamBuffer<TypeInterpBlendQueue, common_buffer_size> TypeInterpBlendQueuePtr;
	private:
		TypeControlObj* m_pObj;
		TypeInterpBlendQueuePtr m_pInterpBlendQueue;//�����Ĳ�ֵ���У������Ŷӽ���
		
		DWORD m_dwBlendStartMiliSecond;
		float m_fBlendPercent;
		
		float m_fNextBlendGap;
		BOOL m_bIsBlending;

		KG3DEnvEffDWControllerDescStruct m_DescStruct;
		
		std::queue<BlendStruct>	m_BlendQueue;
	public:
		KG3DEnvEffDWController();
		~KG3DEnvEffDWController();
	private:
		void UnInitBlendData();
		bool InitBlendData();
	public:
		HRESULT RequestModification(KG3DEnvEffDWControllerDescStruct& DescStruct);
		KG3DEnvEffDWControllerDescStruct GetDescStruct(){return m_DescStruct;}
		
		void SetBindedControlObjectAndInitControlState(TypeControlObj* pObj); ///NULL�Ļ����������;
		HRESULT SetNextBlendStruct(const BlendStruct&);
		HRESULT FrameMove();
	};
};
#endif
#endif //_INCLUDE_KG3DENVEFFDWCONTROLLER_H_
