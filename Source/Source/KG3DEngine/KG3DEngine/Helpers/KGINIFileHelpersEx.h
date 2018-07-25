////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGINIFileHelperEx.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-12-6 14:14:30
//  Comment     : ���������صĲ���
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGINIFILEHELPEREX_H_
#define _INCLUDE_KGINIFILEHELPEREX_H_
#include "KGINIFileHelpers.h"
////////////////////////////////////////////////////////////////////////////////
class IIniFile;
namespace KG_CUSTOM_HELPERS
{
	namespace Private
	{
		//D3DCOLORVALUE////////////////////////////////////////////////////////////////
		bool SaveINIFileHelper(IIniFile* pIniFile, const char* szSection, const char* szKeyName, const D3DCOLORVALUE& DataRet);
		bool LoadINIFileHelper(IIniFile* pIniFile, const char* szSection, const char* szKeyName, D3DCOLORVALUE* DataRet);
		//D3DVECTOR//////////////////////////////////////////////////////////////////////////
		bool SaveINIFileHelper(IIniFile* pIniFile, const char* szSection, const char* szKeyName, const D3DVECTOR& DataRet);
		bool LoadINIFileHelper(IIniFile* pIniFile, const char* szSection, const char* szKeyName, D3DVECTOR* DataRet);
		//D3DLIGHT9//////////////////////////////////////////////////////////////////////////
		bool SaveINIFileHelper(IIniFile* pIniFile, const char* szSection, const char* szKeyName, const D3DLIGHT9& DataRet);
		bool LoadINIFileHelper(IIniFile* pIniFile, const char* szSection, const char* szKeyName, D3DLIGHT9* DataRet);
	};

	//�Ƚϴ�Ľṹ�壬��Save/LoadINIFileBigData���������в�ͬ
	//ģ�����ʽ������Unicode��װ����Unicode���ݵ��ײ�ķ�Unicode�����У�ͬʱ�õ�ģ�����Ͱ�ȫ
	template<class _SaveType>
	inline bool SaveINIFileBigData(IIniFile* pIniFile, LPCTSTR szSection, LPCTSTR szKeyName, const _SaveType& DataRet)
	{
#if defined(_UNICODE) | defined(UNICODE)
		UnicodeConvertor CvtSection, CvtKeyName, CvtDataRet;
		return Private::SaveINIFileHelper(pIniFile, CvtSection.ToA(szSection), CvtKeyName.ToA(szKeyName), DataRet);
#else
		return Private::SaveINIFileHelper(pIniFile, szSection, szKeyName, DataRet);
#endif	
	}
	template<class _LoadType>
	inline bool LoadINIFileBigData(IIniFile* pIniFile, LPCTSTR szSection, LPCTSTR szKeyName, _LoadType* pDataRet)
	{
#if defined(_UNICODE) | defined(UNICODE)
		UnicodeConvertor CvtSection, CvtKeyName;
		return Private::LoadINIFileHelper(pIniFile, CvtSection.ToA(szSection), CvtKeyName.ToA(szKeyName), pDataRet);
#else
		return Private::LoadINIFileHelper(pIniFile, szSection, szKeyName, pDataRet);
#endif

	}
};

namespace KGCH = KG_CUSTOM_HELPERS;
#endif //_INCLUDE_KGINIFILEHELPEREX_H_
