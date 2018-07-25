////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DShaderRCBridge.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-8-4 10:09:41
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSHADERRCBRIDGE_H_
#define _INCLUDE_KG3DSHADERRCBRIDGE_H_

#include "../../ShaderLibrary/ShaderLibrary/ShaderLibraryMain.h"
////////////////////////////////////////////////////////////////////////////////
namespace KG3DShaderRCBridge
{
	enum
	{
		em_max_resource_id_digit = 8,	//�����8λ����ID
	};
	//HGLOBAL LoadShaderRC(UINT uID, LPCVOID* ppData, DWORD* pSize);	//�õ���HGLOBAL����ǿ���ʧ�ܣ��ǿ����ڷ��ص��ڴ�����֮��Ҫ��FreeResource�ͷš����ص��ڴ治��Ҫ�����ͷţ�FreeResource֮��ϵͳ���ͷ�

	//VOID	TryInitResources();	//��������������֮ǰҪ����һ�Σ������ظ�����
	//UINT	GetShaderResourceID(LPCTSTR pFileName);	//0��ʾʧ�ܣ�������ַ���������Сд��
	//LPCTSTR	GetShaderResourcePath(UINT uResourceID);

	//UINT	GetIDFromString(LPCTSTR filePathOrID);	//����0��ʧ��
	//BOOL IsFilePath(LPCTSTR filePathOrID);
	//BOOL IsIDValid(UINT uID);
	//LPCTSTR	GetStringFromID(UINT uID, TCHAR uBuffer[MAX_PATH]);	//����uBuffer

	HRESULT InitLibrary();
	HRESULT UninitLibrary();	//�������ñȽϺã������ͷ���Դ������й©���棬�����ͷ�Ҳ�������Է��ֵ���������
	IKG3DShaderLibrary* GetShaderLibraryInterface();
};

#endif //_INCLUDE_KG3DSHADERRCBRIDGE_H_
