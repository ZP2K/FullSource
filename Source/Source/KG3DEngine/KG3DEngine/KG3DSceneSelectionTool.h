////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DSceneSelectionTool.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-1-8 16:43:42
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSCENESELECTIONTOOL_H_
#define _INCLUDE_KG3DSCENESELECTIONTOOL_H_

#include "IEKG3DSceneSelectionTool.h"
#include "KG3DSceneEntity.h"
////////////////////////////////////////////////////////////////////////////////
class IEKG3DSceneEntity;
class KG3DRepresentObject;
class KG3DRepresentObjectVisitor;
class KG3DSceneEntityVisitor;
class IEKG3DRepresentObject;
class IKG3DMessageReceiverBase;
class KG3DMessageSender;



enum
{
	EM_SELECTION_INCREASE_MODE_DEFAULT = 0,
	EM_SELECTION_INCREASE_MODE_ADD = 1,
	EM_SELECTION_INCREASE_MODE_SUBTRACT = 2,

	EM_SELECTION_GROUP_MAX = 20,

	EM_SELECTION_SELECT_ALL_MAX = 200,
	EM_SELECTION_POINT_SELECT_ALL_MAX = 20,	//PointSelection��ѡ��һ����ʱ����ദ�������Ŀ
	EM_SELECTION_MIN_TIME_GAP = 30,//����30����Ŵ���һ��ѡ�����Ϣ
};

namespace KG_CUSTOM_HELPERS
{
	struct FrontAccessProxy;
};

typedef KGCH::TFrontAccessProxy<KG3DSceneEntity> TypeEntityAccessProxy;


struct KG3DSceneSelectionTool : public IEKG3DSceneSelectionTool
{
	virtual HRESULT SetSelectable(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelectable) = 0;	//��ʱ����
	virtual HRESULT SetSelectable(const IEKG3DSceneEntity& Entity, BOOL bSelectable) = 0;
	virtual HRESULT SetSelectable(IEKG3DRepresentObject& obj, BOOL bSelectable) = 0;

	virtual HRESULT SetSelected(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelected) = 0; 
	virtual HRESULT SetSelected(const IEKG3DSceneEntity& Entity, BOOL bSelected) = 0;
	virtual HRESULT SetSelected(IEKG3DRepresentObject& obj, BOOL bSelected) = 0;

	virtual HRESULT SelectAll() = 0;//ȫѡ���������EM_SELECTION_SELECT_ALL_MAX�Ļ���ʧ�ܣ�������ѡ�񡣲�Ȼ���ڴ󳡾��Ļ�������⣬�󲿷�3D������޷�����Ctrl+A֮�����������⡣
	virtual HRESULT SelectInvert() = 0;//��ѡ������������ƣ�ͬSelectAll

	virtual HRESULT TraverseSelected(KG3DRepresentObjectVisitor& visitor) = 0;
	virtual HRESULT TraverseSelected(KG3DSceneEntityVisitor& visitor) = 0;
	virtual HRESULT TraverseSelectable(KG3DSceneEntityVisitor& Visitor) = 0;

	virtual HRESULT ClearSelectable() = 0;
	virtual HRESULT ClearSelection() = 0;

	virtual ULONG	GetSelectionCount() = 0;

	virtual ULONG	IsSelected(const KG3DRepresentObject& Obj) = 0;
	virtual ULONG	IsSelectable(const KG3DSceneEntity& entity) = 0;

	//����һ�������ѡ����Ĺ���
	virtual HRESULT AllocSelGroup(LPCTSTR pGroupName) = 0;
	virtual HRESULT ClearSelGroup(LPCTSTR pGroupName) = 0;
	virtual ULONG	GetSelGroupSize(LPCTSTR pGroupName) = 0;

	virtual HRESULT CopySelGroupToCurSelection(LPCTSTR pGroupName) = 0;
	virtual HRESULT CopyCurSelectionToSelGroup(LPCTSTR pGroupName) = 0;
	virtual HRESULT CopySelGroupToSelectable(LPCTSTR pGroupName) = 0;
	virtual HRESULT CopySelectableToSelGroup(LPCSTR pGroupName) = 0;

	//virtual HRESULT GetFrontAccessProxy(LPCTSTR pGroupName, KG_CUSTOM_HELPERS::FrontAccessProxy** ppProxy) = 0;	//�ᴴ��һ�����ʴ������������𲽵�ǰ�����ʡ�����������SelectionTool�ڲ�ά���ģ�ֻ���ں�����ʹ�������ʽ����SelectionTool�����ܱ������Proxy����Ȼ������ʽ��Traverse�����ã���Ҳ������ȫ��

	virtual TypeEntityAccessProxy& GetSelectionAccessProxy() = 0; //���ڲ�ά���ķ��ʴ�����ú�iteractor���ƵĹ��ܡ���Ϊ���ڲ�ά���ģ�����ֻ��һ��ʹ��һ��ʹ���ߡ�����ʹ�õ�ʱ��Ӧ��ʹ���κη�ʽ�ı�SelectionTool�ڲ������������������ܻ������std������һ�߷���һ��eraseҲ�Ǻ����׳���ģ�Ӧ�þ�������
	virtual HRESULT GetAccessProxy(IEKG3DSceneSelectionToolAccessProxy** ppAccessProxy) = 0;

	virtual HRESULT GetSelectableList(KG3DSceneEntityList& entities) = 0;	//����������ع���ʱ�������Ժ����Ҫ�����������߸ĳɱ����ʽ����entityû�����ü���������ȫ���������KGSceneEntityList*������û��Ԥ�������ָ�룬����Ҫ��LPVOID

	virtual HRESULT RegisterEventListener(DWORD messageID, IKG3DMessageReceiverBase& receiver) = 0;	//��������KM_SELECT

	virtual HRESULT PointSelection(ULONG uIncreaseMode, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir, BOOL bNearsetOrAll) = 0;//PointSelection������ѭ����ԭ�����ѡ����Ҳ�������ô������������BBOX����ײ�㣬��Ȼ��Щ��������ѡ��bNearsetOrAll�����TRUE��ֻѡ������ģ�����ѡ��һ����
	virtual HRESULT RectSelection(ULONG uIncreaseMode, const D3DXVECTOR4& vLeftTopRightBottom
			, const D3DXMATRIX& matView, const D3DXMATRIX& matProj, const D3DXMATRIX& matViewport, FLOAT fSelRange) = 0;

	virtual ULONG	IsNewSelectionTool() = 0;	//ѡ�񹤾��ع����֮��Ϳ���ȥ��

#if defined(DEBUG) | defined(_DEBUG)
	virtual KG3DMessageSender* TestBegin() = 0;	//�������ڲ��Ե�ʱ���ã����ڸ��ⲿ��������Ϊ,��Զ�����һ������
	virtual VOID TestEnd() = 0;
#endif
};

struct KG3DSceneSelectionToolDummy : public KG3DSceneSelectionTool
{
	virtual HRESULT SetSelectable(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelectable){return E_FAIL;}
	virtual HRESULT SetSelectable(const IEKG3DSceneEntity& Entity, BOOL bSelectable){return E_FAIL;}
	virtual HRESULT SetSelectable(IEKG3DRepresentObject& obj, BOOL bSelectable){return E_FAIL;}

	virtual HRESULT SetSelected(const IEKG3DSceneEntity& Entity, BOOL bSelected){return E_FAIL;}
	virtual HRESULT SetSelected(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelected){return E_FAIL;}
	virtual HRESULT SetSelected(IEKG3DRepresentObject& obj, BOOL bSelected){return E_FAIL;}

	virtual HRESULT SelectAll(){return E_FAIL;}//ȫѡ���������EM_SELECTION_SELECT_ALL_MAX�Ļ���ʧ�ܣ�������ѡ�񡣲�Ȼ���ڴ󳡾��Ļ�������⣬�󲿷�3D������޷�����Ctrl+A֮�����������⡣
	virtual HRESULT SelectInvert(){return E_FAIL;}//��ѡ������������ƣ�ͬSelectAll

	virtual HRESULT TraverseSelected(KG3DRepresentObjectVisitor& visitor){return E_FAIL;}
	virtual HRESULT TraverseSelected(KG3DSceneEntityVisitor& visitor){return E_FAIL;}
	virtual HRESULT TraverseSelectable(KG3DSceneEntityVisitor& Visitor){return E_FAIL;}

	virtual HRESULT ClearSelectable(){return E_FAIL;}
	virtual HRESULT ClearSelection(){return E_FAIL;}

	virtual ULONG	GetSelectionCount(){return 0;}

	virtual ULONG	IsSelected(const KG3DRepresentObject& Obj){return FALSE;}
	virtual ULONG	IsSelectable(const KG3DSceneEntity& entity){return FALSE;}

	virtual HRESULT AllocSelGroup(LPCTSTR pGroupName){return E_FAIL;}
	virtual HRESULT ClearSelGroup(LPCTSTR pGroupName){return E_FAIL;}
	virtual ULONG	GetSelGroupSize(LPCTSTR pGroupName){return 0;}

	virtual HRESULT CopySelGroupToCurSelection(LPCTSTR pGroupName){return E_FAIL;}
	virtual HRESULT CopyCurSelectionToSelGroup(LPCTSTR pGroupName){return E_FAIL;}
	virtual HRESULT CopySelGroupToSelectable(LPCTSTR pGroupName){return E_FAIL;}
	virtual HRESULT CopySelectableToSelGroup(LPCSTR pGroupName){return E_FAIL;}

	//virtual HRESULT GetFrontAccessProxy(LPCTSTR pGroupName, KG_CUSTOM_HELPERS::FrontAccessProxy** pProxy){return E_FAIL;}

	virtual TypeEntityAccessProxy& GetSelectionAccessProxy();
	virtual HRESULT GetAccessProxy(IEKG3DSceneSelectionToolAccessProxy** ppAccessProxy){return E_FAIL;}

	virtual HRESULT GetSelectableList(KG3DSceneEntityList& entities){return E_FAIL;}

	virtual HRESULT RegisterEventListener(DWORD messageID, IKG3DMessageReceiverBase& receiver){return E_FAIL;}

	virtual HRESULT PointSelection(ULONG uIncreaseMode, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir, BOOL bNearestOrAll){return E_FAIL;}
	virtual HRESULT RectSelection(ULONG uIncreaseMode, const D3DXVECTOR4& vLeftTopRightBottom
		, const D3DXMATRIX& matView, const D3DXMATRIX& matProj, const D3DXMATRIX& matViewport, FLOAT fSelRange){return E_FAIL;}

	virtual ULONG	IsNewSelectionTool(){return TRUE;}

#if defined(DEBUG) | defined(_DEBUG)
	virtual KG3DMessageSender* TestBegin(){return NULL;}
	virtual VOID TestEnd(){}
#endif
};

#endif //_INCLUDE_KG3DSCENESELECTIONTOOL_H_
