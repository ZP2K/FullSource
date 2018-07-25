////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : ShaderLibraryMain.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-7-30 16:18:24
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ShaderLibraryMain.h"
#include <atlbase.h>

#ifdef _IN_CPP_FILE_
#error	_IN_CPP_FILE_ should not be defined before here
#endif

#define _IN_CPP_FILE_
#include "ShaderID.h"
#undef  _IN_CPP_FILE_

#define CHINESE_CODEPAGE	936
//HGLOBAL ShaderLibraryLoadShader(HMODULE hModule, UINT uID, LPCVOID* ppData, DWORD* pSize );
HGLOBAL ShaderLibraryLoadResource( HMODULE hModule, LPCTSTR strName, LPCTSTR strType,LPCVOID* ppData, DWORD* pSize );

//HMODULE	g_hModuleMySelf = NULL;
//HMODULE WINAPI GetSelfModuleHandle()
//{
//	MEMORY_BASIC_INFORMATION mbi;
//
//	if (NULL != g_hModuleMySelf)
//	{
//		return g_hModuleMySelf;
//	}
//
//	SIZE_T uTmp = ::VirtualQuery(&GetSelfModuleHandle, &mbi, sizeof(mbi));
//	_ASSERTE(0 != uTmp);
//	g_hModuleMySelf =(HMODULE)mbi.AllocationBase;
//	_ASSERTE(NULL != g_hModuleMySelf);
//	return g_hModuleMySelf;
//}

HMODULE WINAPI GetSelfModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi;

	SIZE_T uTmp = ::VirtualQuery(&GetSelfModuleHandle, &mbi, sizeof(mbi));
	_ASSERTE(0 != uTmp);
	return (HMODULE)mbi.AllocationBase;
}

class KG3DShaderLibraryResource : public IKG3DShaderLibraryResource
{
	LPCVOID	m_pBuffer;
	DWORD	m_dwBufferSize;
	HGLOBAL	m_hResource;
public:
	KG3DShaderLibraryResource(LPCVOID pBuffer, DWORD dwSize, HGLOBAL hGlobal)
		:m_pBuffer(pBuffer)
		,m_dwBufferSize(dwSize)
		,m_hResource(hGlobal)
	{
		_ASSERTE(NULL != m_pBuffer);
		_ASSERTE(0 != m_dwBufferSize);
		_ASSERTE(NULL != m_hResource);
	}
	~KG3DShaderLibraryResource()
	{
		_ASSERTE(NULL != m_hResource); 
		::FreeResource(m_hResource);
	}

	virtual ULONG STDMETHODCALLTYPE Release(){delete this; return 0;}
	virtual LPCVOID	STDMETHODCALLTYPE GetBuffer() {_ASSERTE(NULL != m_pBuffer); return m_pBuffer;}
	virtual	DWORD	STDMETHODCALLTYPE GetBufferSize(){_ASSERTE(0 != m_dwBufferSize); return m_dwBufferSize;}
};
class KG3DShaderLibrary : public IKG3DShaderLibrary 
{
public:
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual ULONG IsLibraryBuildedInUnicode();
	virtual HRESULT STDMETHODCALLTYPE GetShaderResourceA(LPCSTR strFilePath, IKG3DShaderLibraryResource** ppResource)
	{
		return GetShaderResource(ATL::CA2TEX<MAX_PATH>(strFilePath, CHINESE_CODEPAGE), ppResource);
	}
	virtual HRESULT STDMETHODCALLTYPE GetShaderResourceW(LPCWSTR strFilePath, IKG3DShaderLibraryResource** ppResource)
	{
		return GetShaderResource(ATL::CW2TEX<MAX_PATH>(strFilePath, CHINESE_CODEPAGE), ppResource);
	}

	KG3DShaderLibrary();
private:
	HRESULT STDMETHODCALLTYPE GetShaderResource(LPCTSTR strFilePath, IKG3DShaderLibraryResource** pResource);
	HRESULT Init();
	UINT	GetShaderResourceID(LPCTSTR pFileName);	//0��ʾʧ�ܣ�������ַ���������Сд��
	LPCTSTR	GetShaderResourcePath(UINT uResourceID);

	HGLOBAL GetShaderResourceByFileFullName(LPCTSTR strFilePath, LPCVOID* pResource, DWORD* pBytes);
	HGLOBAL GetShaderResourceByID(UINT uID, LPCVOID* ppData, DWORD* pSize);	//ʹ����֮��ҪFreeResource��֮�����õ���ָ������Ч��������Ҫfree�Ǹ�ָ��

	typedef std::map<std::tstring, DWORD>	TypeFileNameMap;
	TypeFileNameMap		m_mapFileName;

	ULONG m_uRef;

	HMODULE	m_hModule;
};
KG3DShaderLibrary::KG3DShaderLibrary()
	:m_uRef(1)
{
	Init();
	m_hModule = GetSelfModuleHandle();
	_ASSERTE(NULL != m_hModule);
}
ULONG STDMETHODCALLTYPE	KG3DShaderLibrary::AddRef()
{
	return ++m_uRef;
};
ULONG STDMETHODCALLTYPE KG3DShaderLibrary::Release()
{
	if (--m_uRef <= 0)
	{
		delete this;
	}
	return m_uRef;
};
HGLOBAL KG3DShaderLibrary::GetShaderResourceByFileFullName(LPCTSTR strFilePath, LPCVOID* ppData, DWORD* pBytes)
{
	std::tstring strFileFullName;
	BOOL bRet = KGCH::StrPathGetFullFileName(strFilePath, strFileFullName);
	_ASSERTE(bRet && _T("�����shader�е�includeд����Щʲô"));
	KG_PROCESS_ERROR(bRet);

	std::transform(strFileFullName.begin(), strFileFullName.end(), strFileFullName.begin(), tolower);
	UINT uID = this->GetShaderResourceID(strFileFullName.c_str());
	KG_PROCESS_ERROR(0 != uID);

	DWORD uBypes = 0;
	LPCVOID	pData = NULL;
	HGLOBAL hglobalTmp = this->GetShaderResourceByID(uID, &pData, &uBypes);	//�ĳɿ��Զ�ͷ�ļ���������
	KG_PROCESS_ERROR(NULL != hglobalTmp);

	//m_hResource = hglobalTmp;
	if(NULL != pBytes)
		*pBytes = uBypes; 
	if(NULL != ppData)
		*ppData = pData;
	return hglobalTmp;
Exit0:
	return NULL;
}

HGLOBAL KG3DShaderLibrary::GetShaderResourceByID( UINT uID, LPCVOID* ppData, DWORD* pSize )
{
	LPCVOID pReturn;
	DWORD	dwSize;

	HGLOBAL hglobalTmp = ShaderLibraryLoadResource(m_hModule, MAKEINTRESOURCE(uID), SHADER_RC_TYPE_STRING, &pReturn, &dwSize);

	if (NULL == hglobalTmp)
	{
		return NULL;
	}

	//���Unicode�ļ�ͷ,�еĻ�ȥ����Shader�����ʱ�򲻴����ļ�ͷ
	//EF BB BF UTF-8
	//FE FF UTF-16/UCS-2, little endian
	//FF FE UTF-16/UCS-2, big endian
	//FF FE 00 00 UTF-32/UCS-4, little endian.
	WORD	s_wUnicodeFileHeader = 0xfeff;

	if (dwSize <= 2)
	{
		::FreeResource(hglobalTmp);
		return NULL;
	}

	_ASSERTE(2 == sizeof(WORD) && _T("�����С��ƥ�䣬����Ĵ���ȫ������"));
	if (*(WORD*)pReturn == s_wUnicodeFileHeader)
	{
		pReturn = (WORD*)pReturn + 1;
		dwSize -= sizeof(WORD);
	}

	if(NULL != ppData)
		*ppData = pReturn;

	if(NULL != pSize)
		*pSize = dwSize;

	return hglobalTmp;
}

HRESULT KG3DShaderLibrary::Init()
{
	if (m_mapFileName.empty())	//��ʼ��ȫ�ֵĹ�ϣ��������shader�ļ�����ϣһ�飬�Ա���Open���������ļ�����ϣ�õ��ļ�
	{
		_ASSERTE(0 != _countof(g_shaderRCArray));	//���������rc�ļ�����
		_ASSERTE(0 != g_shaderRCArray[0].uID);

		UINT uLastID = g_shaderRCArray[0].uID - 1;
		for (size_t i = 0; i < _countof(g_shaderRCArray) - 1; ++i)	//���һ������NULL�����Ŀսṹ
		{
			_ASSERTE(uLastID + 1 == g_shaderRCArray[i].uID && _T("ID �����ǵ����ģ������ӿ�����˳�����"));
			
			uLastID = g_shaderRCArray[i].uID;
			LPCSTR strFilePath = g_shaderRCArray[i].strFilePath;
			_ASSERTE(NULL != strFilePath);
			
			std::tstring strFileFullName;
			BOOL bRet = KGCH::StrPathGetFullFileName(strFilePath, strFileFullName);
			_ASSERTE(bRet && _T("������ܳɹ�����·���������shader·���ǲ���д����"));
			
			//��Сд������Ϊ��ϣ����map
			std::transform(strFileFullName.begin(), strFileFullName.end(), strFileFullName.begin(), tolower);
			TypeFileNameMap::_Pairib ib = m_mapFileName.insert(std::make_pair(strFileFullName, g_shaderRCArray[i].uID));
			_ASSERTE(ib.second && _T("������벻�ɹ�����ʾ���ڹ�ϣֵ��ͬ���ַ�������ѳ�ͻ���ļ�����"));
		}

		_ASSERTE(NULL == g_shaderRCArray[_countof(g_shaderRCArray)-1].strFilePath);
	}
	return S_OK;
}

UINT KG3DShaderLibrary::GetShaderResourceID( LPCTSTR pFileName )
{
	_ASSERTE(NULL != pFileName && !m_mapFileName.empty());
	TypeFileNameMap::iterator it = m_mapFileName.find(std::tstring(pFileName));
	_ASSERTE(it != m_mapFileName.end());
	if (it == m_mapFileName.end())
	{
		return 0;
	}
	return it->second;
}

LPCTSTR KG3DShaderLibrary::GetShaderResourcePath( UINT uResourceID )
{
	UINT uIndex = uResourceID - SHADER_RC_MIN_VALUE;
	if (uIndex >= _countof(g_shaderRCArray))
	{
		return NULL;
	}
	return g_shaderRCArray[uIndex].strFilePath;
}

HRESULT KG3DShaderLibrary::GetShaderResource( LPCTSTR strFilePath, IKG3DShaderLibraryResource** ppResource )
{
	KG3DShaderLibraryResource* pNewRC = NULL;

	LPCVOID pBuffer = NULL;
	DWORD dwSize = 0;
	HGLOBAL hGlobal = NULL;
	
	KG_PROCESS_ERROR(NULL != ppResource && NULL != strFilePath);
	hGlobal = this->GetShaderResourceByFileFullName(strFilePath, &pBuffer, &dwSize);
	KG_PROCESS_ERROR(hGlobal);

	try
	{
		pNewRC = new KG3DShaderLibraryResource(pBuffer, dwSize, hGlobal);	//������ⲿ���̣�new�ǻ����쳣��
		KG_PROCESS_ERROR(NULL != pBuffer);

		if (NULL != pNewRC)
		{
			*ppResource = pNewRC;
		}
	}
	catch (...)
	{
		goto Exit0;	
	}
	
	return S_OK;
Exit0:
	SAFE_RELEASE(pNewRC);
	return E_FAIL;
}

ULONG KG3DShaderLibrary::IsLibraryBuildedInUnicode()
{
#if defined(UNICODE) | defined(_UNICODE)
	return TRUE;
#else
	return FALSE;
#endif
}
HGLOBAL ShaderLibraryLoadResource( HMODULE hModule, LPCTSTR strName, LPCTSTR strType,LPCVOID* ppData, DWORD* pSize )
{
	HGLOBAL hglobalTmp = NULL;
	__try						 
	{
		HRSRC hrsrcTmp = ::FindResource(hModule, strName, strType);
		if(NULL == hrsrcTmp)
			return NULL;

		DWORD dwSize = ::SizeofResource(hModule, hrsrcTmp);
		if(0 == dwSize)
			return NULL;

		hglobalTmp = ::LoadResource(hModule, hrsrcTmp);
		if(NULL == hglobalTmp)
			return NULL;

		LPVOID lpMemory = ::LockResource(hglobalTmp);
		if(NULL == lpMemory)
		{
			::FreeResource(hglobalTmp);
			hglobalTmp = NULL;
			return NULL;
		}

		if(NULL != ppData)
			*ppData = lpMemory;
		if(NULL != pSize)
			*pSize = dwSize;

		return hglobalTmp;
	}									 
	__except(EXCEPTION_EXECUTE_HANDLER)	 
	{									 

	}

	if(NULL != hglobalTmp)
		FreeResource(hglobalTmp);
	return NULL;
}
//HGLOBAL ShaderLibraryLoadShader(HMODULE hModule, UINT uID, LPCVOID* ppData, DWORD* pSize )
//{
//	LPCVOID pReturn;
//	DWORD	dwSize;
//
//	HGLOBAL hglobalTmp = ShaderLibraryLoadResourceA(hModule, MAKEINTRESOURCEA(uID), SHADER_RC_TYPE_STRING, &pReturn, &dwSize);
//
//	if (NULL == hglobalTmp)
//	{
//		return NULL;
//	}
//
//	//���Unicode�ļ�ͷ,�еĻ�ȥ����Shader�����ʱ�򲻴����ļ�ͷ
//	//EF BB BF UTF-8
//	//FE FF UTF-16/UCS-2, little endian
//	//FF FE UTF-16/UCS-2, big endian
//	//FF FE 00 00 UTF-32/UCS-4, little endian.
//	WORD	s_wUnicodeFileHeader = 0xfeff;
//
//	if (dwSize <= 2)
//	{
//		::FreeResource(hglobalTmp);
//		return NULL;
//	}
//
//	//_ASSERTE(2 == sizeof(WORD) && _T("�����С��ƥ�䣬����Ĵ���ȫ������"));
//	if (*(WORD*)pReturn == s_wUnicodeFileHeader)
//	{
//		pReturn = (WORD*)pReturn + 1;
//		dwSize -= sizeof(WORD);
//	}
//
//	if(NULL != ppData)
//		*ppData = pReturn;
//
//	if(NULL != pSize)
//		*pSize = dwSize;
//
//	return hglobalTmp;
//}

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	return TRUE;
}

SHADERLIB_DLL_API HRESULT CreateKG3DShaderLibrary(IKG3DShaderLibrary** pShaderLibrary)
{
	KG3DShaderLibrary* pLibraryInstance = new KG3DShaderLibrary();
	if (NULL != pShaderLibrary)
	{
		*pShaderLibrary = pLibraryInstance;
		return S_OK;
	}
	return E_FAIL;
}
#ifdef _MANAGED
#pragma managed(pop)
#endif