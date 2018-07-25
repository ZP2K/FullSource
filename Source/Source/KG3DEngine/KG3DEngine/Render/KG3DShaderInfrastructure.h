////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DShaderInfrastructure.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-6-10 15:08:18
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSHADERINFRASTRUCTURE_H_
#define _INCLUDE_KG3DSHADERINFRASTRUCTURE_H_
#include "KG3DShaderHolder.h"
////////////////////////////////////////////////////////////////////////////////
struct KG3DShaderMacroDef 
{
	typedef std::pair<std::tstring, std::tstring> TypeSingleDef;	//first��Name��second��Definition
	std::vector<TypeSingleDef>	macroDefs;	//Ԥ����

	D3DXMACRO_ARRAY ToD3DXMACRO_ARRAY(std::vector<D3DXMACRO>& bufferTemp)const ;

	BOOL FromD3DXMACRO_ARRAY(const D3DXMACRO_ARRAY& macroArray);

	friend bool operator==(KG3DShaderMacroDef& a, KG3DShaderMacroDef& b);
};


struct KG3DShaderBuildStruct 
{
	LPD3DXEFFECT		pEffect;
	ULONG				uRef;	//PtrHolder�����ü�����������
	LPDIRECT3DDEVICE9	pDevice;	

	DWORD				dwFlags;
	BOOL				bUseEffectPool;	//EffectPool��Ψһ�ģ��������ʹ��EffectPool�ʹ�����Ψһ���Ǹ�Pool��
	UINT				uGarbageWeight;

	//ע�⣬����ĳ�Ա�ǿ���ʹ��Binary���Ƶģ�����ʹ��Binary���Ƶĳ�Ա�ź���
	KG3DShaderEventHandler* pEventHandler;

	std::tstring		strFilePathOrResourceID;	//�����ResouceID�Ļ���ת���ַ�����¼
	std::tstring		strIncludeFilePath;
	std::tstring		strErrorString;
	std::vector<unsigned char> vecEffectParamHandleBuffer;

	KG3DShaderMacroDef		macroDef;

	//////////////////////////////////////////////////////////////////////////
	KG3DShaderBuildStruct(){ZeroMemory(this, sizeof(KG3DShaderBuildStruct));}
	KG3DShaderBuildStruct(const KG3DShaderBuildStruct& Other)
	{
		this->operator=(Other);
	}
	~KG3DShaderBuildStruct()
	{
		if(NULL != pEventHandler)
		{
			pEventHandler->Release(); 
			pEventHandler = NULL;
		}
	}

	KG3DShaderBuildStruct& operator=(const KG3DShaderBuildStruct& Other);
	friend bool operator==(KG3DShaderBuildStruct& a, KG3DShaderBuildStruct& b);	
	friend bool operator!=(KG3DShaderBuildStruct& a, KG3DShaderBuildStruct& b)
	{
		return !(a == b);
	}
};


#endif //_INCLUDE_KG3DSHADERINFRASTRUCTURE_H_
