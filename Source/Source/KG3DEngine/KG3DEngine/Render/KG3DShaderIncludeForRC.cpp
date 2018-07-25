////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DShaderIncludeForRC.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-8-4 10:14:50
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KG3DShaderIncludeForRC.h"
#include "KG3DShaderRCBridge.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE KG3DShaderIncludeForRC::Open(D3DXINCLUDE_TYPE, LPCSTR pFileName, LPCVOID, LPCVOID *ppData, UINT *pBytes)
{
//	_ASSERTE(NULL != pFileName);
//
//	{
//		std::tstring strFileFullName;
//		BOOL bRet = KGCH::StrPathGetFullFileName(pFileName, strFileFullName);
//		_ASSERTE(bRet && _T("�����shader�е�includeд����Щʲô"));
//		KG_PROCESS_ERROR(bRet);
//
//		std::transform(strFileFullName.begin(), strFileFullName.end(), strFileFullName.begin(), tolower);
//		UINT uID = KG3DShaderRCBridge::GetShaderResourceID(strFileFullName.c_str());
//		KG_PROCESS_ERROR(0 != uID);
//
//		DWORD uBypes = 0;
//		LPCVOID	pData = NULL;
//		HGLOBAL hglobalTmp = KG3DShaderRCBridge::LoadShaderRC(uID, &pData, &uBypes);	//�ĳɿ��Զ�ͷ�ļ���������
//		KG_PROCESS_ERROR(NULL != hglobalTmp);
//
//		if(NULL != pBytes)
//			*pBytes = uBypes; 
//		if(NULL != ppData)
//			*ppData = pData;
//		return S_OK;
//	}
//Exit0:
//	KGLogPrintf(KGLOG_ERR, _T("��Դ��û�а��������ļ��������Դ�ļ�:%s"), pFileName);
//	return E_FAIL;

	IKG3DShaderLibraryResource* pResource  = NULL;
	IKG3DShaderLibrary* pLibrary = KG3DShaderRCBridge::GetShaderLibraryInterface();
	KG_PROCESS_ERROR(NULL != pLibrary && NULL != pFileName && NULL != ppData && NULL != pBytes);
	{
		HRESULT hr = pLibrary->GetShaderResourceA(pFileName, &pResource);
		
		KG_COM_PROCESS_ERROR(hr);
		_ASSERTE(pResource);

		_ASSERTE(m_vecResources.size() < em_max_include_files);
		m_vecResources.push_back(pResource);

		_ASSERTE(NULL != pResource->GetBuffer());
		*ppData = pResource->GetBuffer();
		*pBytes = pResource->GetBufferSize();
		return S_OK;
	}
Exit0:
	return E_FAIL;	
}
HRESULT STDMETHODCALLTYPE KG3DShaderIncludeForRC::Close(LPCVOID)
{
	/*if (NULL != m_hResource)
	{
		::FreeResource(m_hResource);
		m_hResource = NULL;
	}*/	

	//�����Թر���Դ����Ϊ��������ͬʱ���˼���ͷ�ļ���һһ��¼����Դ�Ƚ��鷳�����Ҷ��shader������һЩͷ�ļ�����ͣ�Ĺرմ򿪻����������ʧ�����Բ�����FreeResource��ϵͳ����dll�����������Լ�������Щ�ڴ�ģ��ο�MSDN����FreeResource��������Ϊ���������������ʱ��ͳһFree�ͺ���

	return S_OK;
}

KG3DShaderIncludeForRC::KG3DShaderIncludeForRC()
{
	//KG3DShaderRCBridge::TryInitResources();
	m_vecResources.reserve(em_max_include_files);
}

KG3DShaderIncludeForRC::~KG3DShaderIncludeForRC()
{
	/*if (NULL != m_hResource)
	{
		::FreeResource(m_hResource);
		m_hResource = NULL;
	}*/
	for (size_t i = 0, iend = m_vecResources.size(); i < iend; ++i)
	{
		IKG3DShaderLibraryResource*& p = m_vecResources[i];
		SAFE_RELEASE(p);
	}
}


