////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DSceneObjectPlacementTool.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-1 11:46:39
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DSCENEOBJECTPLACEMENTTOOL_H_
#define _INCLUDE_IEKG3DSCENEOBJECTPLACEMENTTOOL_H_
interface IEKG3DRepresentObject;
interface IEKG3DRepresentObjectSet;
////////////////////////////////////////////////////////////////////////////////
struct IEKG3DSceneObjectPlacementTool //: public IUnknown
{
	virtual HRESULT SetObjectInHandle(IEKG3DRepresentObject* pObject, BOOL bReleaseWhenAnotherObjectIsSettedToHandle) = 0;//������˵ڶ�����������ô�ⲿ���ù��������ڣ������ⲿҪ����������
	//virtual HRESULT SetObjectSetInHandle(IEKG3DRepresentObjectSet* pObject, BOOL bReleaseWhenAnotherObjectIsSettedToHandle) = 0;
	virtual HRESULT GetObjectInHandle(IEKG3DRepresentObject** ppObject/*, IEKG3DRepresentObjectSet** ppObjectSet*/) = 0;//��������һ��
	virtual HRESULT RemoveObjectInHandle() = 0;

	virtual HRESULT DoPlaceObjectByOther(IEKG3DRepresentObject *pInObject, IEKG3DRepresentObject **pOutObj) = 0; //��ĳ��obj��λ�ô�����һ��obj,by likan
};

#endif //_INCLUDE_IEKG3DSCENEOBJECTPLACEMENTTOOL_H_
