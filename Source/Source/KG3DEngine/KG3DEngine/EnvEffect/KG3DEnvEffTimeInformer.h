////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffTimeInformer.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-12-30 17:50:37
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _INCLUDE_KG3DENVEFFTIMEINFORMER_H_
#define _INCLUDE_KG3DENVEFFTIMEINFORMER_H_

#include "IEKG3DEnvEffDynamicWeather.h"
#include "KG3DMessenger.h"
//#include "FileType.h"
class KG3DTimeClock;
////////////////////////////////////////////////////////////////////////////////
namespace KG3D_ENVIRONMENT_EFFECT
{
	////////////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/*ʹ�÷�������������࣬Ȼ���ָ���DynamicWeather�е�Informer����֤��DynamicWeather���������ڴ��ڡ�
	Ȼ��DynamicWeather�е�TimeInformer������GetNowTime������ȡ��һ��ʱ��                                                                      */
	/************************************************************************/
	
	struct KG3DEnvEffTI_DescStruct : public KG3DENVEFFTI_DESCSTRUCT 
	{
		enum{
			em_default_boost_factor = 1,
		};
		KG3DEnvEffTI_DescStruct();
		KG3DEnvEffTI_DescStruct(const KG3DEnvEffTI_DescStruct& OtherStruct);
		KG3DEnvEffTI_DescStruct(const KG3DENVEFFTI_DESCSTRUCT& OtherStruct);
		void operator=(const KG3DEnvEffTI_DescStruct& OtherStruct);
		bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		//operator KG3DENVEFFTI_DESCSTRUCT();
	};
	class KG3DEnvEffTimeInformer : public KG3DMessageSingleReceiverSender, public IEKG3DEnvEffTimeInformer
	{
	private:
		KG3DTimeClock* m_pClock;
		KG3DEnvEffTI_DescStruct m_DescStruct;
		DWORD m_dwGetTimeGap;
		DWORD m_dwFrameLastGetTimeTime;
		DWORD m_dwState;
		enum
		{
			emIsStopped = 0x01,
		};		

	public:
		static const float TIME_INFORMER_MIN_BOOST_FACTOR; ///�Ŵ�10��
		
		HRESULT FrameMove();	///����ʵ�����ƶ��̵߳Ļ���
		HRESULT RequestModification(const KG3DEnvEffTI_DescStruct* pDescStruct, DWORD dwModificationOption);
		inline const KG3DEnvEffTI_DescStruct* GetStruct();
		
		///<�ӿ�>		
		virtual HRESULT RequestModification(const KG3DENVEFFTI_DESCSTRUCT* pDescStruct, DWORD dwModificationOption);
		virtual HRESULT GetStruct(KG3DENVEFFTI_DESCSTRUCT* pDescStruct);
		HRESULT SaveMapSetting(IIniFile* pIniFile);
		HRESULT LoadMapSetting(IIniFile* pIniFile);
		INT	GetCurHour();	///��Ҫ���ڳ�ʼ��
		
		VOID StopTimeChecking(bool bStop);
		bool IsTimeCheckingStopped(){return m_dwState & emIsStopped;}
		///</�ӿ�>

		static KG3DEnvEffTimeInformer* Build(LPCTSTR, DWORD_PTR ptimeClock, DWORD_PTR pKG3DMessageReceiver);

		~KG3DEnvEffTimeInformer();
	private:
		DWORD m_StandardSecond;
		DWORD m_LastGettedSecond;
		
		DWORD GetSecond();
		HRESULT InitTimmer();

#if defined(DEBUG) | defined(_DEBUG)
	protected:
#else
	private:
#endif
		KG3DEnvEffTimeInformer(KG3DTimeClock& timeClock, KG3DMessageReceiverBase& receiverToBeInformed);
		

		DWORD GetHour(DWORD dwSecond);
	};

};///namespace
#endif //_INCLUDE_KG3DENVEFFTIMEINFORMER_H_
