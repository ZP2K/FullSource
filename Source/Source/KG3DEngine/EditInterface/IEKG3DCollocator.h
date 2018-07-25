////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DCollocator.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-12-3 17:07:35
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DCOLLOCATOR_H_
#define _INCLUDE_IEKG3DCOLLOCATOR_H_

////////////////////////////////////////////////////////////////////////////////

namespace KG_CUSTOM_HELPERS
{
	struct StrPathSplited;
};

interface IEKG3DCollocator 
{
	virtual HRESULT GetSavePath(DWORD dwType, LPCTSTR scenePath	//dwType��EM_SAVE_OP_ENVIRONMENT�ȣ�ScenePath�ǳ��������·��
		, KG_CUSTOM_HELPERS::StrPathSplited* pRetPathSplited	//����ΪNULL�����ط��ѵ�·�����õ��̷���·�����ܷ���
		, LPTSTR pathRet, size_t uPathBufferSize) = 0;	//pathRet����ΪNULL������������·��
};

#endif //_INCLUDE_IEKG3DCOLLOCATOR_H_
