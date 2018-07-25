////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DShaderManager.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-8-29 10:06:33
//  Comment     : �ο�CPP��˵��
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSHADERMANAGER_H_
#define _INCLUDE_KG3DSHADERMANAGER_H_
#include "KG3DShaderHolder.h"
////////////////////////////////////////////////////////////////////////////////

struct KG3DShaderVisitor
{
	virtual HRESULT Accept(LPD3DXEFFECT pEffect) = 0;
	virtual KG3DShaderVisitor* Clone()const = 0;
	virtual ~KG3DShaderVisitor() = 0{}
};

struct KG3DShaderBuildParams 
{
	TCHAR strFilePathOrResourceIDString[MAX_PATH];
	std::vector<D3DXMACRO> macroArray;				//�����Macro���ַ�ָ��Ҳ�ǲ��ܱ����
	TCHAR strIncludePath[MAX_PATH];
	DWORD dwFlag;
	size_t uShaderParamHandleSize;
	BOOL	bUseEffectPool;							//�����0��ʹ��EffectPool��EffectPool��Ψһ��
	const KG3DShaderEventHandler* pEventHandler;	//�õ����֮��ֻ����ʱ�ã����ܱ��档�ڲ�û�м����ü���

	KG3DShaderBuildParams(){ZeroMemory(this, sizeof(*this));}

	D3DXMACRO_ARRAY GetMacroArray()
	{
		if (macroArray.empty())
		{
			return D3DXMACRO_ARRAY::GetEmptyMacroArray(); 
		}
		D3DXMACRO_ARRAY retArray = {
			&macroArray[0],		//��Ϊvector�ڴ��������ģ����Կ���ֱ�ӷų�ȥ
			macroArray.size(),
		};
		return retArray;
	}
};

struct KG3DShaderManager
{
	virtual KG3DShaderHolder Create(LPDIRECT3DDEVICE9 pDevice,
		LPCTSTR pSrcFile,					//���Զ�תСд
		D3DXMACRO_ARRAY macroArray = D3DXMACRO_ARRAY::GetEmptyMacroArray(),	//��Ҫ�ִ�Сд
		LPCTSTR pIncludeFilePath = NULL,	//���Զ�תСд��DX�ĺ�����LPD3DXINCLUDE�ģ������Ѿ���æ��װ�ˣ�ֱ�Ӵ��ļ���·���Ϳ����ˣ������ļ�������Shader��include�ؼ���ָ��
		DWORD Flags = 0,
        size_t uShaderParamHandleSize = 0,
		BOOL bUseEffectPool = NULL,	//�Ƿ�ʹ��EffectPool�����ʹ����ô�õ���ShaderManager�ڲ�ά����ΨһPool��EffectPool����Ҫ���
		std::tstring* pErrorString = NULL,
		const KG3DShaderEventHandler* pEventHandler = NULL	//û����������Ͳ���Ҫ����������Clone�������Ƿ�newһ�������Լ���Clone�����о�������Ҫ��֤��������
		) = 0;//���ص�KG3DShaderHolder�õ��Ϳ������ˣ�Effect����ʧ�ܿ�����Reload�����¼��أ�����ʧ�ܵ�Effect���Լ����õ�
	
	virtual HRESULT ReloadAll() = 0;
	virtual HRESULT Reload(KG3DShaderPtrHolder& shaderPtrHolder) = 0;
	virtual HRESULT ReloadWithMacroModification(KG3DShaderPtrHolder& shaderPtrHolder, D3DXMACRO_ARRAY macroArrayFull) = 0;	//��Macro����Ļ�����Ҫ���¼���shader
	virtual HRESULT Find(LPCTSTR shaderName, KG3DShaderHolder& retHolder) = 0;	//�ٶȺ��������ã�����������ϣ��,ֻ����lower�ַ���

	virtual HRESULT GetShaderBuildParams(KG3DShaderHolder& shaderHolder, KG3DShaderBuildParams* pBuildParams) = 0;	//�õ���Param�����ڵ��õĺ����������ã���Ҫ���������ָ�룬���ڲ���֤��Ч��Ҳ��Ķ���Щָ������ü�����������˾�������������
	//�豸�ָ��Լ��������õ���
	virtual HRESULT SetShaderEventHandler(KG3DShaderHolder& shaderHolder, KG3DShaderEventHandler* pEventHandler) = 0;//��������Clone

	static std::tstring GetResourceString(LPCTSTR strPath);	//���ص���strBuffer�����ַ�
	static BOOL GetPathFromResourceString(LPCTSTR strResourceString, std::tstring* pRet);
	static BOOL IsResourceString(LPCTSTR strIn);

	virtual ~KG3DShaderManager() = 0{}
};

KG3DShaderManager& g_GetShaderManager();

namespace Private
{
	HRESULT	ReloadShaderHolder(KG3DShaderPtrHolder& Holder);
	LPD3DXEFFECT GetSingletonDummyEffect();
	//LPD3DXEFFECT CreateDummyEffect();
	bool IsDummyEffect(LPD3DXEFFECT p);
	bool IsShaderHolderInited(KG3DShaderHolder& Holder);
};



#endif //_INCLUDE_KG3DSHADERMANAGER_H_
