#include <tchar.h>
#include "DXUT.h"
#include "D3DXIncludeResource.h"
//#include <atlbase.h>
#include <string>
#include <map>
#include <algorithm>	//transform
#include <locale>	//tolower

//#ifndef tstring
//#if defined(UNICODE) || defined(_UNICODE)
//#define	tstring wstring
//#else
//#define tstring string
//#endif
//#endif

#ifdef _IN_CPP_FILE_
#error	_IN_CPP_FILE_ should not be defined before here
#endif

#define _IN_CPP_FILE_
#include "../ShaderLibrary/ShaderID.h" //only one cpp should include this
#undef  _IN_CPP_FILE_

#include "../ShaderLibrary/ShaderLibraryMain.h"

typedef std::map<std::string, DWORD>	TypeFileNameMap;
TypeFileNameMap		s_mapFileName;

namespace
{
	std::string	GetFileFullName(LPCSTR strFilePathOrFileName)
	{
		CHAR strFileName[MAX_PATH] = "";
		CHAR strExt[MAX_PATH] = "";
		_ASSERTE(NULL != strFilePathOrFileName);
		size_t errt = _splitpath_s(strFilePathOrFileName, NULL, 0, NULL, 0, strFileName
			, _countof(strFileName), strExt, _countof(strExt));
		_ASSERTE(0 == errt);
		errt;
		std::string strFileFullName = strFileName;
		strFileFullName += strExt;
		return strFileFullName;
	}
};

HRESULT STDMETHODCALLTYPE KG3DShaderIncludeForRC::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	_ASSERTE(NULL == m_hResource);

	DWORD uBypes = 0;

	_ASSERTE(NULL != pFileName && ! s_mapFileName.empty());

	std::string strFileFullName = GetFileFullName(pFileName);
	std::transform(strFileFullName.begin(), strFileFullName.end(), strFileFullName.begin(), tolower);
	TypeFileNameMap::iterator it = s_mapFileName.find(strFileFullName);
	_ASSERTE(it != s_mapFileName.end());
	if (it == s_mapFileName.end())
	{
		return E_FAIL;
	}

	DWORD dwFileRCID = it->second;
	HGLOBAL hglobalTmp = ShaderLibraryLoadShader(dwFileRCID, ppData, &uBypes);	//�ĳɿ��Զ�ͷ�ļ���������
	if(NULL == hglobalTmp)
		return E_FAIL;

	m_hResource = hglobalTmp;
	*pBytes = uBypes; 
	return S_OK;
}
HRESULT STDMETHODCALLTYPE KG3DShaderIncludeForRC::Close(LPCVOID pData)
{
	if (NULL != m_hResource)
	{
		::FreeResource(m_hResource);
		m_hResource = NULL;
	}
	return S_OK;
}

KG3DShaderIncludeForRC::KG3DShaderIncludeForRC()
:m_hResource(NULL)
{
	if (s_mapFileName.empty())	//��ʼ��ȫ�ֵĹ�ϣ��������shader�ļ�����ϣһ�飬�Ա���Open���������ļ�����ϣ�õ��ļ�
	{
		_ASSERTE(0 != _countof(g_shaderRCArray));	//���������rc�ļ�����
		for (size_t i = 0; i < _countof(g_shaderRCArray) - 1; ++i)	//���һ������NULL�����Ŀսṹ
		{
			LPCSTR strFilePath = g_shaderRCArray[i].strFilePath;
			_ASSERTE(NULL != strFilePath);
			std::string strFileFullName = GetFileFullName(strFilePath);
			std::transform(strFileFullName.begin(), strFileFullName.end(), strFileFullName.begin(), tolower);
			TypeFileNameMap::_Pairib ib = s_mapFileName.insert(std::make_pair(strFileFullName, g_shaderRCArray[i].uID));
			_ASSERTE(ib.second && _T("������벻�ɹ�����ʾ���ڹ�ϣֵ��ͬ���ַ�������ѳ�ͻ���ļ�����"));
		}
	}
}

KG3DShaderIncludeForRC::~KG3DShaderIncludeForRC()
{
	if (NULL != m_hResource)
	{
		::FreeResource(m_hResource);
		m_hResource = NULL;
	}
}
struct ShaderTypeID
{
	DWORD dwType;
	UINT	uID;
};

ShaderTypeID shaderTypeIDArray[] = 
{
	{
		DEFST_SKYBOX, SHADER_RC_SKYBOX,
	}
};
HGLOBAL MyLoadResourceShader(DWORD dwType, LPCVOID* ppData, DWORD* pSize)
{
	_ASSERTE(dwType - 0 < _countof(shaderTypeIDArray));
	UINT uID = shaderTypeIDArray[dwType - 0].uID;
	return ShaderLibraryLoadShader(uID, ppData, pSize);
}
//HGLOBAL MyLoadResourceShader(DWORD dwType, LPCVOID* ppData, DWORD* pSize )
//{
//	LPCVOID pReturn;
//	DWORD	dwSize;
//
//	_ASSERTE(dwType - 0 < _countof(shaderTypeIDArray));
//	UINT uID = shaderTypeIDArray[dwType - 0].uID;
//
//	HMODULE hModule = GetModuleHandle(_T("ShaderLibray.dll"));
//	HGLOBAL hglobalTmp = MyLoadResourceA(hModule, MAKEINTRESOURCEA(uID), SHADER_RC_TYPE_STRING, &pReturn, &dwSize);
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
//	_ASSERTE(2 == sizeof(WORD) && _T("�����С��ƥ�䣬����Ĵ���ȫ������"));
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

LPCSTR MyGetShaderPath()
{
	for (size_t i = 0; i < _countof(g_shaderRCArray); ++i)
	{
		if (g_shaderRCArray[i].uID == SHADER_RC_SKYBOX)
		{
			return g_shaderRCArray[i].strFilePath;
		}
	}
	return NULL;
}