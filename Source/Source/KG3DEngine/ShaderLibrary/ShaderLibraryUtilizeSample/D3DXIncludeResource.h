////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : D3DXIncludeResource.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-7-28 16:51:42
//  Comment     : 
/*
shader�������Դ������������£�
1.shader�����exe��������exe����������Ǳ���ģ���������ԭ��"���÷����ȼ�ԭ��REP"��ָ���ģ����õ����Ⱦ��Ƿ��������ȡ�
������Դdll����ʹ����������Դdll����Ҳ���ܸ����Ͻ�������shader����һ�·��������⣬ֻҪ����ͬһ���ļ�һ�𷢲�Ҳ����ɲ�ƥ�䡣
��������shader���Ҳ����500k���£�Ҫ��ѹ��Ҳ�������������Բ���Ӱ�췢���Ĵ�С��
2.shader�仯֮�󣬹������Զ�ʶ�𣬲��Զ����´������������û�����壬һ������ɰ汾��ƥ��
3.shader����ʵʱ�༭���ؼ��أ����Ҳ�Ǳ���ģ���Ȼÿ�ζ����볬���鷳
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_D3DXINCLUDERESOURCE_H_
#define _INCLUDE_D3DXINCLUDERESOURCE_H_

////////////////////////////////////////////////////////////////////////////////

class KG3DShaderIncludeForRC : public ID3DXInclude
{
public:
	STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	STDMETHOD(Close)(THIS_ LPCVOID pData);
	KG3DShaderIncludeForRC();
	~KG3DShaderIncludeForRC();
private:
	HGLOBAL	m_hResource;
};

HGLOBAL MyLoadResourceShader(DWORD dwType, LPCVOID* ppData, DWORD* pSize);

LPCSTR	MyGetShaderPath();

enum
{	
	DEFST_SKYBOX = 0,
};

#endif //_INCLUDE_D3DXINCLUDERESOURCE_H_
