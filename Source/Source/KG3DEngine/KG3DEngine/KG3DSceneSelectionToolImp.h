////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DSceneSelectionToolSolid.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-5-13 15:33:57
//  Comment     : ���ͷ�ļ���Ҫ��¶��ȥ��������IE�汾�ĽӿھͿ�����
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSCENESELECTIONTOOLIMP_H_
#define _INCLUDE_KG3DSCENESELECTIONTOOLIMP_H_
#include "KG3DCommonObject.h"
#include "KG3DMessageWMGListner.h"
#include "KG3DSceneEntity.h"
#include "KG3DSceneSelectionTool.h"
#include "KG3DFrustum.h"
////////////////////////////////////////////////////////////////////////////////
class KG3DScene;
class KG3DScene;
//class PrivateObjectContainer;
class KG3DRepresentObjectVisitor;

class KG3DSceneSelectionToolSolid : public KG3DCommonObjectSimple
	, public KG3DMessageMSGListner
	, public KG3DSceneSelectionTool
{
public:
	//<KG3DCommonObject>
	STDMETHOD_(ULONG, GetType)();
	STDMETHOD_(LPVOID, ToAnotherInterface)(DWORD_PTR Param);

	STDMETHOD(Render)();
	STDMETHOD(FrameMove)();
	//</KG3DCommonObject>

	//<Messenger>
	virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender );
	//</Messenger>

	//<SelectionTool>
	virtual HRESULT SetSelectable( SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelectable );//��ʱ����
	virtual HRESULT SetSelectable(const IEKG3DSceneEntity& Entity, BOOL bSelectable);//����Ƿ�Selectable�Ļ������Զ�ȥ��Selected
	virtual HRESULT SetSelectable(IEKG3DRepresentObject& obj, BOOL bSelectable);

	virtual HRESULT SetSelected(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelected);//��ʱ����
	virtual HRESULT SetSelected(const IEKG3DSceneEntity& Entity, BOOL bSelected);
	virtual HRESULT SetSelected(IEKG3DRepresentObject& obj, BOOL bSelected);

	virtual HRESULT SelectAll();//ȫѡ���������EM_SELECTION_SELECT_ALL_MAX�Ļ���ʧ�ܣ�������ѡ�񡣲�Ȼ���ڴ󳡾��Ļ�������⣬�󲿷�3D������޷�����Ctrl+A֮�����������⡣
	virtual HRESULT SelectInvert();//��ѡ������������ƣ�ͬSelectAll

	virtual HRESULT TraverseSelected(KG3DSceneEntityVisitor& Visitor);
	virtual HRESULT TraverseSelected(KG3DRepresentObjectVisitor& Visitor);
	virtual HRESULT TraverseSelectable(KG3DSceneEntityVisitor& Visitor);

	virtual HRESULT ClearSelectable();
	virtual HRESULT ClearSelection();

	virtual ULONG	GetSelectionCount();

	virtual ULONG	IsSelected(const KG3DRepresentObject& Obj);
	virtual ULONG	IsSelectable(const KG3DSceneEntity& entity);

	virtual HRESULT AllocSelGroup(LPCTSTR pGroupName);
	virtual HRESULT ClearSelGroup(LPCTSTR pGroupName);
	virtual ULONG	GetSelGroupSize(LPCTSTR pGroupName);

	virtual HRESULT CopySelGroupToCurSelection(LPCTSTR pGroupName);
	virtual HRESULT CopyCurSelectionToSelGroup(LPCTSTR pGroupName);

	virtual HRESULT CopySelGroupToSelectable(LPCTSTR pGroupName);
	virtual HRESULT CopySelectableToSelGroup(LPCSTR pGroupName);

	virtual HRESULT GetSelectableList(KG3DSceneEntityList& entities);

	virtual TypeEntityAccessProxy& GetSelectionAccessProxy();
	virtual HRESULT GetAccessProxy(IEKG3DSceneSelectionToolAccessProxy** ppAccessProxy);

	virtual HRESULT RegisterEventListener(DWORD messageID, IKG3DMessageReceiverBase& receiver);
	
	virtual HRESULT PointSelection(ULONG uIncreaseMode, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir, BOOL bNearestOrAll);//PointSelection������ѭ����ԭ�����ѡ����Ҳ�������ô������������BBOX����ײ�㣬��Ȼ��Щ��������ѡ��
	virtual HRESULT RectSelection(ULONG uIncreaseMode, const D3DXVECTOR4& vLeftTopRightBottom
		, const D3DXMATRIX& matView, const D3DXMATRIX& matProj, const D3DXMATRIX& matViewport, FLOAT fSelRange);



	virtual ULONG	IsNewSelectionTool(){return TRUE;}

#if defined(DEBUG) | defined(_DEBUG)
	virtual KG3DMessageSender* TestBegin();
	virtual VOID TestEnd();
#endif
	//</SelectionTool>

	static KG3DCommonObject* Build(LPCTSTR lpFile, DWORD_PTR wParam, DWORD_PTR);

	KG3DSceneSelectionToolSolid(KG3DScene& scene);
private:
	~KG3DSceneSelectionToolSolid();//��Release����

	static DWORD m_ObjectSelectMask; // ����ѡ������ add by suntao

	static VOID	PushSelected(KG3DSceneSelectionTool& selTool, ULONG uIncreaseMode, const KG3DSceneEntity* pEntity, KG3DRepresentObject* pObject);
	static VOID	InitSelection(KG3DSceneSelectionTool& selTool, ULONG uIncreaseMode);

	//ע������ѡ������Scene��SelectionTool�Ƿֿ����ģ���û�мٶ�SelectionTool�Ǵ�����Scene�ģ���������Ҳ�����������η����ԭ��
	static VOID	PointSelectionPrivate(KG3DScene& scene, ULONG uIncreaseMode
		, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vDir
		, KG3DSceneSelectionToolSolid& selToolToPushResul, BOOL bNearestOrAll);	

	static VOID	RectSelectionPrivate(KG3DScene& scene, ULONG uIncreaseMode
		, const KG3DRay& rayA, const KG3DRay& rayC, FLOAT fSelRange
		, KG3DSceneSelectionToolSolid& selToolToPushResult);

	HRESULT	SetOneSelected(KG3DRepresentObject& obj, BOOL bSelected);

	typedef std::set<KG3DRepresentObject*> TypeObjCt;

	TypeObjCt*	GetSelGroup(LPCTSTR pName);
	struct SelectionData;
private:
	KG3DScene& m_parentScene;	

	typedef std::set<KG3DSceneEntity>	TypeEntityCt;

	TypeEntityCt	m_SelectedEntities;
	TypeEntityCt	m_SelectableEntities;//���ھɵĸ�ʽ�����������Ƿ��ѡ�ģ����Ƕ����¸�ʽ�����Ӧ����һ������

	//PrivateObjectContainer* m_pObjectContainer;//�����Object��������ã�Object������ʱ����Զ�ȥ������������õ�	
	TypeObjCt m_selectedObjCt;

	typedef std::map<std::tstring, TypeObjCt>	TypeCtList;
	TypeCtList	m_selGroups;

	KGCH::TFrontAccessProxySolid<TypeEntityCt>	m_accessProxy;

	//����ѡ�������
	enum
	{
		em_selection_none = 0,
		em_selection_point = 1,
		em_selection_rect = 2,
	};
	enum
	{
		emPointSelectionOrRectSectionGap = 5,//ѡ������ʱ������ѡ����ѡ��
	};
	struct SelectionData 
	{
		RECT		rectSel;
		D3DXMATRIX	matViewProjViewport;
		KG3DRay		selRayA;
		KG3DRay		selRayC;
		FLOAT		fSelRange;
		KG3DFrustum	m_frustum;
		SelectionData();
	};
	SelectionData m_selData;

	INT			m_nSelState;	//�����ж�Ҫ��ѡ���ǿ�ѡ��
	ULONG		m_uIncreaseMode;		//Ĭ�ϣ���ѡ�����Ǽ�ѡ

	KG3DMessageSender	m_Sender;

	BOOL		m_bSelectionExecutionAvailable;	//ѡ����Ҫ����ʱ��������һ��FrameMoveֻ����һ��

	struct CustomProxy : public IEKG3DSceneSelectionToolAccessProxy 
	{
		KG3DSceneSelectionToolSolid& m_selTool;
		
		virtual IEKG3DSceneEntity* Reset()
		{
			m_selTool.m_accessProxy.Begin();
			if(! m_selTool.m_accessProxy.IsEnd())
				return &m_selTool.m_accessProxy.GetCur();
			return NULL;
		}
		virtual IEKG3DSceneEntity* Next()
		{
			if(! m_selTool.m_accessProxy.IsEnd())
			{
				m_selTool.m_accessProxy.StepForward();
				if(! m_selTool.m_accessProxy.IsEnd())
					return &m_selTool.m_accessProxy.GetCur();
			}			
			return NULL;
		}

		CustomProxy(KG3DSceneSelectionToolSolid& tool):m_selTool(tool){}
	};

	CustomProxy m_IEAccessProxy;
};
#endif //_INCLUDE_KG3DSCENESELECTIONTOOL_H_

