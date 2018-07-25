////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DSceneSelectionTool.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-16 17:49:45
//  Comment     : �Ժ�ѡ��ͨ���������
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DSCENESELECTIONTOOL_H_
#define _INCLUDE_IEKG3DSCENESELECTIONTOOL_H_

////////////////////////////////////////////////////////////////////////////////
class IEKG3DSceneEntity;
class KG3DRepresentObjectVisitor;
class KG3DSceneEntityVisitor;
class IEKG3DRepresentObject;

interface IEKG3DSceneSelectionToolAccessProxy
{
	/*virtual VOID Begin() = 0;
	virtual BOOL IsEnd() = 0;
	virtual VOID StepForward() = 0;
	virtual IEKG3DSceneEntity* GetCur() = 0;*/

	virtual IEKG3DSceneEntity* Reset() = 0;	//��IEnumһ�£�����������ǰ��,�����Ԫ�ؾͷ��ص�һ��ֵ
	virtual IEKG3DSceneEntity* Next() = 0;

	virtual ~IEKG3DSceneSelectionToolAccessProxy() = 0{}
};

interface IEKG3DSceneSelectionTool
{
	virtual HRESULT SetSelectable(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelectable) = 0;	//��ʱ����
	virtual HRESULT SetSelectable(const IEKG3DSceneEntity& Entity, BOOL bSelectable) = 0;

	virtual HRESULT SetSelected(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelected) = 0;	//��ʱ����
	virtual HRESULT SetSelected(const IEKG3DSceneEntity& Entity, BOOL bSelected) = 0;

	virtual HRESULT SetSelected(IEKG3DRepresentObject& obj, BOOL bSelected) = 0;

	virtual HRESULT ClearSelection() = 0;
	virtual ULONG	GetSelectionCount() = 0;

	virtual HRESULT SelectAll() = 0;
	virtual HRESULT SelectInvert() = 0;

	virtual HRESULT GetAccessProxy(IEKG3DSceneSelectionToolAccessProxy** ppAccessProxy) = 0;	//�������������,������������鷳��COM��׼����ö���õ�IEnum���������ʽ�����
};

interface IEKG3DSceneSelectBase
{
	//virtual HRESULT OnToggleStateRotation() = 0;//<OnToggleStateRotation Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
	//virtual HRESULT OnToggleStateMove() = 0;//<OnToggleStateMove Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
	//virtual HRESULT OnToggleStateScaling() = 0;

	//virtual HRESULT AddSelectableEntity(KG3DSceneEntity* Entity) = 0;
	//virtual HRESULT AddSelectableEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer) = 0;
	//virtual HRESULT AddSelectedEntity(IEKG3DSceneEntity* Entity) = 0;
	//virtual HRESULT AddSelectedEntity(SCENEENTITYTYPE EntityType,PVOID EntityPointer) = 0;

	//virtual HRESULT RemoveSelectableEntity(KG3DSceneEntity* Entity) = 0;
	//virtual HRESULT RemoveSelectedEntity(IEKG3DSceneEntity* Entity) = 0;
	//virtual HRESULT RemoveSelectableEntity(SCENEENTITYTYPE EntityType,PVOID EntityPointer, BOOL bEitherRemoveInSelectedList) = 0;
	//virtual HRESULT RemoveSelectedEntity(SCENEENTITYTYPE EntityType,PVOID EntityPointer) = 0;

	//virtual HRESULT CopySelectedEntity() = 0;//<Copy Refactor>
	//virtual HRESULT ParseCopiedEntity() = 0;//2009-2-11 ��������Ķ���SceneEditBase��PasteSelection
	//virtual INT GetCopiedEntityCount() = 0;//2009-1-12 <Copy Refactor>

	//virtual INT GetSelectedEntitySize() = 0;//��IEKG3DSceneSelectionTool��GetSelectionCount
	//virtual HRESULT GetSelectedEntity(INT nIndex, IEKG3DSceneEntity** ppEntity) = 0;

	virtual INT GetSelectableEntitySize() = 0;
	virtual HRESULT GetSelectableEntity(INT nIndex, IEKG3DSceneEntity** ppEntity) = 0;

	//virtual HRESULT ClearSelectedEntity() = 0;
	virtual HRESULT CopySelectable2SelectedSet(LPVOID pBillboard) = 0;

	//svirtual HRESULT SelectNone(DWORD dwClass) = 0;	//�ĳ���IEKG3DSceneSelectionTool::ClearSelection
	//virtual HRESULT SelectAll(DWORD dwClass) = 0;		////�ĳ���IEKG3DSceneSelectionTool
	//virtual HRESULT SelectInvert(DWORD dwClass) = 0;	////�ĳ���IEKG3DSceneSelectionTool
};

//ѡ��Ĺ�����
//interface ISelectFilter {
//	virtual bool operator()(IEKG3DSceneEntity* pEntity) = 0;
//};//<ISelectFilter Refactor>

#endif //_INCLUDE_IEKG3DSCENESELECTIONTOOL_H_
