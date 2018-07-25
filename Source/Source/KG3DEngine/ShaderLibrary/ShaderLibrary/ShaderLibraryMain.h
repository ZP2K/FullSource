////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : ShaderLibraryMain.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-7-30 16:18:10
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_SHADERLIBRARYMAIN_H_
#define _INCLUDE_SHADERLIBRARYMAIN_H_
 
#ifdef DLLTMP_EXPORTS
#define SHADERLIB_DLL_API extern "C" __declspec(dllexport)
#else
#define SHADERLIB_DLL_API extern "C" __declspec(dllimport)
#endif

struct DECLSPEC_NOVTABLE IKG3DShaderLibraryResource
{
	virtual ULONG STDMETHODCALLTYPE Release() = 0;
	virtual LPCVOID	STDMETHODCALLTYPE GetBuffer() = 0;	//��֤�ɹ�
	virtual	DWORD	STDMETHODCALLTYPE GetBufferSize() = 0;	//��֤�ɹ�
};

struct DECLSPEC_NOVTABLE IKG3DShaderLibrary//2009-8-25 ��ʱֻ��������ʹ�� 
{
	virtual ULONG STDMETHODCALLTYPE AddRef()  = 0;
	virtual ULONG STDMETHODCALLTYPE Release()  = 0;

	virtual ULONG IsLibraryBuildedInUnicode() = 0;	//ע�⣬�����漰�ַ�����Ҫע��ӿ��ַ�������
	virtual HRESULT STDMETHODCALLTYPE GetShaderResourceA(LPCSTR strFilePath, IKG3DShaderLibraryResource** ppResource) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetShaderResourceW(LPCWSTR strFilePath, IKG3DShaderLibraryResource** ppResource) = 0;
};
////////////////////////////////////////////////////////////////////////////////
typedef HRESULT (*FuncCreateKG3DShaderLibrary)(IKG3DShaderLibrary** pShaderLibrary);
//SHADERLIB_DLL_API HGLOBAL ShaderLibraryLoadShader(UINT uID, LPCVOID* ppData, DWORD* pSize);	//ʹ����֮��ҪFreeResource��֮�����õ���ָ������Ч��������Ҫfree�Ǹ�ָ��
SHADERLIB_DLL_API HRESULT CreateKG3DShaderLibrary(IKG3DShaderLibrary** pShaderLibrary);





#endif //_INCLUDE_SHADERLIBRARYMAIN_H_
