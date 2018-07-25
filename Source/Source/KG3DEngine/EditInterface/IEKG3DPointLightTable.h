////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DPointLightTable.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-3-6 11:59:15
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DPOINTLIGHTTABLE_H_
#define _INCLUDE_IEKG3DPOINTLIGHTTABLE_H_

////////////////////////////////////////////////////////////////////////////////
interface IEKG3DPointLight;
interface IEKG3DRepresentObject;
struct IEKG3DPointLightTable
{
	virtual HRESULT GetRepresentObjetOfLight(IEKG3DPointLight* pLight, IEKG3DRepresentObject** ppRetRepresent) = 0;//���û��pLight���ͷ���һ���յģ����򷵻�Light��Ӧ��
};

#endif //_INCLUDE_IEKG3DPOINTLIGHTTABLE_H_
