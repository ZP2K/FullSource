////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DSceneObjectPlacementTool.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-1 11:34:33
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSCENEOBJECTPLACEMENTTOOL_H_
#define _INCLUDE_KG3DSCENEOBJECTPLACEMENTTOOL_H_
#include "KG3DMessageWMGListner.h"
#include "IEKG3DSceneObjectPlacementTool.h"
#include "KG3DCommonObject.h"

class KG3DRepresentObject;
class KG3DRepresentObjectSet;
class KG3DSceneSceneEditor;
////////////////////////////////////////////////////////////////////////////////
class KG3DSceneSceneEditor;
class KG3DSceneObjectPlacementTool : public KG3DCommonObjectSimple, public IEKG3DSceneObjectPlacementTool, public KG3DMessageMSGListner
{
public:
	STDMETHOD_(ULONG, GetType)();
	STDMETHOD_(LPVOID,ToAnotherInterface)(DWORD_PTR Param);
	static KG3DCommonObject* Build(LPCTSTR lpFile, DWORD_PTR pScene, DWORD_PTR);

	virtual HRESULT SetObjectInHandle(IEKG3DRepresentObject* pObject, BOOL bReleaseWhenAnotherObjectIsSettedToHandle);//������˵ڶ�����������ô�ⲿ���ù��������ڣ������ⲿҪ����������
	//virtual HRESULT SetObjectSetInHandle(IEKG3DRepresentObjectSet* pObject, BOOL bReleaseWhenAnotherObjectIsSettedToHandle);
	virtual HRESULT GetObjectInHandle(IEKG3DRepresentObject** ppObject/*, IEKG3DRepresentObjectSet** ppObjectSet*/);//��������һ��
	virtual HRESULT RemoveObjectInHandle();

	virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender );

	KG3DSceneObjectPlacementTool(KG3DSceneSceneEditor* pEditor);
	~KG3DSceneObjectPlacementTool();
	
	HRESULT DoPlaceSpecObject(KG3DRepresentObject *pObject,BOOL& bToFile);	// last update by likan 2009-11-11
public:
	//�������ڲ�ʹ�ã������Ҫ��ĳ��λ�÷�һ������Ļ�����Ҫ�÷��ù��ߵ���
	//��Ӧ����BuildKG3DRepresentObject+KG3DRepresentObject::SetTranslation
	//+KG3DSceneSceneEditor::AddRepresentObject
	HRESULT DoPlaceObject();	//��ʱ��public�ģ�Ӧ�ÿ��Ըĳ�private��
	HRESULT DoPlaceObjectByOther(IEKG3DRepresentObject *pInObject, IEKG3DRepresentObject **pOutObj); //��ĳ��obj��λ�ô�����һ��obj,by likan

private:
	STDMETHOD(Render)();
	STDMETHOD(FrameMove)();

	KG3DRepresentObject* m_pObjInHandle;
	//KG3DRepresentObjectSet* m_pObjSetInHandle;
	enum
	{
		EM_RELEASE_WHEN_SET_HANDLE = 0x01,
	};
	KG_CUSTOM_HELPERS::KGBitSet<32>		m_BitSet;
	DWORD	m_dwLastPlaceTime;	//Ϊ�˷�ֹ�����ĵ����������Ҫ��һ�����
	KG3DSceneSceneEditor* m_pSceneEditor;
};

#endif //_INCLUDE_KG3DSCENEOBJECTPLACEMENTTOOL_H_
