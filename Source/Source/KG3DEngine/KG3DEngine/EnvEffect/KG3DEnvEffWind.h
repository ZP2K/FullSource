////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffWind.h
//  Version     : 1.0
//  Creator     : Li Kan
//  Create Date : 2007-1-15 10:55:25
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _INCLUDE_KG3DENVEFFWIND_H_
#define _INCLUDE_KG3DENVEFFWIND_H_

#include "KG3DEnvEffect.h"
#include "IEKG3DEnvEffWind.h"

namespace KG3D_ENVIRONMENT_EFFECT
{

	class KG3DEnvEffWind : public KG3DEnvEffect, public IEKG3DEnvEffWind
	{
	private:
		map<int, KG3DWIND_DESCSTRUCT> m_Wind;

	public:
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext);
		virtual HRESULT	FrameMove();
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface);///ʹ������������Եõ�EnvEffect��Ӧ�����IE�ӿ�

		//IE�ӿ�
	public:
		virtual HRESULT	RequestModification( const KG3DWIND_DESCSTRUCT* pStruct );///������pStruct�е�ModificationOption�趨��ʲô����
		virtual HRESULT	GetStruct( KG3DWIND_DESCSTRUCT* pResultStruct );
		virtual void DoVist(IEKG3DEnvWindVistor &vistor);
		virtual void DelWind(int windid);
		KG3DEnvEffWind();
		~KG3DEnvEffWind();
	};
};

#endif