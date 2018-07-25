#pragma once
#include "KG3DSceneEntity.h"
#include "KG3DFrustum.h"
#include "KG3DEditMoveGrid.h"
#include "KG3DEditRotationGrid.h"
#include "KG3DEditScaleGrid.h"
#include "KG3DToolsImpl.h"
#include "IEKG3DSceneSelectionTool.h"//by dengzhihui Refactor 2006��11��24��
#include "KG3DSceneSelectionTool.h"

#include "KG3DLocalScalCoord.h"
#include "KG3DLocalRotationCoord.h"
#include "KG3DLocalMoveCoord.h"

struct KG3DTransformTool;

class KG3DScene;

class KG3DCamera;
class KSceneModelEditorMeshBindDialog;
class KG3DSceneLayer;
interface IEKG3DSFXBillboard;
class KG3DSceneEntity;
class KG3DMessageReceiverBase;
class KG3DMessageSenderBase;
class KG3DMessage;
class IKG3DMessageSenderBase;
class KG3DMessageActionExecutionStruct;

class KG3DSceneSelectBase : public CKG3DConnectionPoint<IEKG3DSceneSelectBase>, public KG3DSceneSelectionTool
{
	friend class KSceneModelEditorMeshBindDialog;
private:
	DWORD m_dwLimitValue;
	
public:
	virtual void SetSelectOption(DWORD dwOption, DWORD dwOptionDesign);
	virtual DWORD GetSelectOption();
	virtual void SetLimitValue(DWORD dwLimitValue);
	virtual DWORD GetLimitValue();
	
	virtual HRESULT SetSelectable(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelectable);	//��ʱ����
	virtual HRESULT SetSelectable(const IEKG3DSceneEntity& Entity, BOOL bSelectable);
	virtual HRESULT SetSelectable(IEKG3DRepresentObject& obj, BOOL bSelectable);

	virtual HRESULT SetSelected(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bSelected);	//��ʷ����
	virtual HRESULT SetSelected(const IEKG3DSceneEntity& Entity, BOOL bSelected);

	virtual HRESULT SetSelected(IEKG3DRepresentObject& obj, BOOL bSelected);

	virtual HRESULT SelectAll();//ȫѡ���������EM_SELECTION_SELECT_ALL_MAX�Ļ���ʧ�ܣ�������ѡ�񡣲�Ȼ���ڴ󳡾��Ļ�������⣬�󲿷�3D������޷�����Ctrl+A֮�����������⡣
	virtual HRESULT SelectInvert();//��ѡ������������ƣ�ͬSelectAll

	virtual HRESULT TraverseSelected(KG3DRepresentObjectVisitor& visitor);
	virtual HRESULT TraverseSelected(KG3DSceneEntityVisitor& visitor);
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

	virtual TypeEntityAccessProxy& GetSelectionAccessProxy();
	virtual HRESULT GetAccessProxy(IEKG3DSceneSelectionToolAccessProxy** ppAccessProxy);

	virtual HRESULT GetSelectableList(KG3DSceneEntityList& entities);

	virtual HRESULT RegisterEventListener(DWORD messageID, IKG3DMessageReceiverBase& receiver);

	virtual HRESULT PointSelection(ULONG uIncreaseMode, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir, BOOL bNearestOrAll);//PointSelection������ѭ����ԭ�����ѡ����Ҳ�������ô������������BBOX����ײ�㣬��Ȼ��Щ��������ѡ��bNearestOrAll��ʾ�����Nearest��һ�а������ߣ������All���������ж�ѡ��
	virtual HRESULT RectSelection(ULONG uIncreaseMode, const D3DXVECTOR4& vLeftTopRightBottom
		, const D3DXMATRIX& matView, const D3DXMATRIX& matProj, const D3DXMATRIX& matViewport, FLOAT fSelRange);

	virtual ULONG	IsNewSelectionTool(){return FALSE;}

#if defined(DEBUG) | defined(_DEBUG)
	virtual KG3DMessageSender* TestBegin();
	virtual VOID TestEnd();
#endif
protected:
	//BOOL       m_bFocusing;//�����Աû�д��ڵı�Ҫ
	
	//BOOL m_bShowSelectableEntityMark;
	
	enum{em_max_distance_for_point_selection = 5,};
	

	DWORD GetIncreaseMode(){return m_dwIncreaseMode;}
	HRESULT SetIncreaseMode(DWORD dwMode){m_dwIncreaseMode = dwMode; return S_OK;}

	KG3DTransformTool& GetTransformToolInterface(DWORD dwType);
//public:
	//ahpho
	//KG3DSceneEntity	m_SelectedKingEntity;//ѡ�е�king //2009-1-13����ʵ����ѡ���б�����һ����ֱ�Ӳ���ѡ���б�õ����һ�������ˡ�

	//KG3DSceneEntityList m_listClipBoardEntity;	//���ڸ���//<Copy Refactor>

	//KG3DSceneEntityList m_listTempSelectedEntity;//2009-1-16 <m_listTempSelectedEntity Refactor> ȥ������m_listSelectedEntity�ϲ�������

    //ISelectFilter* m_pSelectFilter;	//<ISelectFilter Refactor>
private:	//�ƶ�Ϊprivate�ĳ�Ա��Ҫ�ٱ�¶Ϊprotected�ˣ��������в����������÷�������
	BOOL m_bSelecting;
	BOOL m_bMoving;
	BOOL m_bRotating;
	BOOL m_bScaling;

	//D3DXVECTOR3 m_Selection_Origin_A;//ѡ��׵�������λ��
	//D3DXVECTOR3 m_Selection_Direction_A;

	//D3DXVECTOR2 m_Selection_Origin2D_A;
	//D3DXVECTOR3 m_Selection_Origin_C;
	//D3DXVECTOR3 m_Selection_Direction_C;
	//D3DXVECTOR2 m_Selection_Origin2D_C;

	struct SelectionInput 
	{
		D3DXVECTOR3	vRaySrcA;	//ѡ��׵�������λ��
		D3DXVECTOR3 vRayDirA;

		D3DXVECTOR3 vRaySrcC;	//������ѡ���ĵ�
		D3DXVECTOR3 vRayDirC;

		D3DXVECTOR2	vPosWindowA;	//�ڴ��ڵ���Ļ����
		D3DXVECTOR2 vPosWindowC;

		DWORD dwSelMask;
		FLOAT fSizeLimit;

		DWORD dwSelMaskDesign;  //�߻��༭����

		SelectionInput();
	};
	SelectionInput	m_SelInput;

	D3DXVECTOR3 m_SelectionCenter;
	DWORD m_dwIncreaseMode;

	typedef std::map<std::tstring, KG3DSceneEntityList>	TypeCtList;
	TypeCtList	m_SelGroups;

	KG3DSceneEntityList m_listSelectedEntity;//ѡ�е�
	KG3DSceneEntityList m_listSelectableEntity;

	KGCH::TFrontAccessProxySolid<KG3DSceneEntityList> m_accessProxy;	//ע�⣬��������m_listSelectedEntity����������ΪҪ����������ʼ����
	IEKG3DSceneSelectionToolAccessProxy* m_pIEAccessProxy;

	KG3DScalCoordinate	m_coordScale;
	KG3DRotationCoordinateOld m_coordRotate;
	KG3DMoveCoordinate m_coordMove;

	KG3DMessageReceiverBase* m_pEngineMsgReceiver;
	KG3DMessageSenderBase*	m_pSender;
	KG3DScene&	m_Scene;	//���ﲻ������ϣ��Ժ�Scene����Ϊһ��ͳһ���������������ڵģ�

	enum{
		em_transformToolCount = KG3DTYPE_TRANSFORM_TOOL_END - KG3DTYPE_TRANSFORM_TOOL_BEGIN + 1,
	};
	KG3DTransformTool* m_TransformTools[em_transformToolCount];	//�����þۺϵķ�ʽ��ģ���KG3DTransformTool�Ľӿڡ�SceneSelectBaseͬʱ����ѡ�񹤾ߺ��ƶ�����ת�����Ź��ߣ�����ֻ�ܼ����ӿڶ��������档���µ��ƶ�����ת�����Ź����ǲ���Ҫ�����ӵġ�

	

	KG3DSceneEntityList* GetSelGroup(LPCTSTR strName);
	

	void ExecActionClick( KG3DMessageActionExecutionStruct* pExecActionStruct );
	void HandleWindowsMessages(UINT uWindowsMsgID, DWORD_PTR wParam, DWORD_PTR lParam);	//���ֱ༭��û��������EXECACTION���ƣ���ʱ���žɵ�Messages����

	static BOOL FindNearestOrFindAll(const D3DXVECTOR2& vUpLeft, const D3DXVECTOR2 vBottomRight);
	static HRESULT GenerateTempSelectedList(const D3DVIEWPORT9& Viewport, const D3DXMATRIX& View, const D3DXMATRIX& Projection,SelectionInput& selInput
										, KG3DSceneEntityList& listSelectable, KG3DSceneEntityList& listSelected, DWORD dwIncreaseMode);
	HRESULT LaunchSelect();
	static BOOL EntityIsSelectable(KG3DSceneEntityList& listSelectable, SelectionInput& selInput, KG3DSceneEntity& entity);	//ѡ�����

	//��Щ�����ٿ������������м̳У������Ҫ���ն�Ӧ����Ϣ����Observer��һ�ף���RegisterEventListener��ע��
	/*virtual*/ HRESULT SelectBegin(D3DXVECTOR3 Origin,D3DXVECTOR3 Direction,D3DXVECTOR2 Origin2D, DWORD dwOperation);
	/*virtual*/ HRESULT SelectBegin(D3DXVECTOR3 Origin,D3DXVECTOR3 Direction,const D3DXVECTOR2 Origin2DLeftUp,
		const D3DXVECTOR2 Origin2DRightBottom, DWORD dwOperation);
	HRESULT SelectFrameMove(D3DXVECTOR3 Origin,D3DXVECTOR3 Direction,D3DXVECTOR2 Origin2D/*,DWORD dwOperation*/);
	/*virtual*/ HRESULT SelectEnd(D3DVIEWPORT9& Viewport,D3DXMATRIX& Projection,D3DXMATRIX& View);	//��������̳У�Ӧ����RegisterEventListener�ķ�ʽ�õ���Ϣ

	/*virtual*/ HRESULT RotateBegin(D3DXVECTOR3& Origin,D3DXVECTOR3& Direction);
	/*virtual*/ HRESULT RotateFrameMove(D3DXVECTOR3& Origin,D3DXVECTOR3& Direction,float XX,float YY,float ZZ);
	/*virtual*/ HRESULT RotateEnd();
	HRESULT RotateDirect(const D3DXQUATERNION& Rot, const D3DXVECTOR3* pRotXYZ);//��Scene�������GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(XX)������

	/*virtual*/ HRESULT MoveBegin(D3DXVECTOR3& Origin,D3DXVECTOR3& Direction);
	/*virtual*/ HRESULT MoveFrameMove(/*KG3DCamera* pCamera,D3DXVECTOR3& Origin,D3DXVECTOR3& Direction,float XX,float YY*/);
	/*virtual*/ HRESULT MoveEnd();
	HRESULT MoveDirect(const D3DXVECTOR3& Trans);

	/*virtual*/ HRESULT ScaleBegin(D3DXVECTOR3& Origin,D3DXVECTOR3& Direction);
	/*virtual*/ HRESULT ScaleFrameMove(KG3DCamera* pCamera,D3DXVECTOR3& Origin,D3DXVECTOR3& Direction,float XX,float YY,float ZZ);
	/*virtual*/ HRESULT ScaleEnd();
	HRESULT ScaleDirect(const D3DXVECTOR3& Scales); 

	const D3DXVECTOR3& GetCenterPos(){return m_SelectionCenter;}

	//������Щ����֮���Ա����ô������ľ�̬����������Ϊ��Щ���Ƕ������㷨�����C��ʽ�ĺ���������������ͷ�������֮����໥���ã��������ܷ��ֳ����������;��
	//��Ľṹ�ͻ�����������
	static VOID InitSelection(KG3DSceneEntityList& listSelected, DWORD dwIncreaseMode);
	static VOID PushSelected(KG3DSceneEntityList& listSelected, DWORD dwIncreaseMode, KG3DSceneEntity& entitySelected);


	static HRESULT PointSelectionPrivate(ULONG uIncreaseMode, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir
		, SelectionInput& selInput, KG3DSceneEntityList& listSelectable, KG3DSceneEntityList& listSelected, BOOL bNearsetOrAll);

	static HRESULT RectSelectionPrivate(ULONG uIncreaseMode, const D3DXVECTOR4& vLeftTopRightBottom , const D3DXMATRIX& matView, const D3DXMATRIX& matProj
		, const D3DXMATRIX& matViewport, FLOAT fSelRange , SelectionInput& selInput, KG3DSceneEntityList& listSelectable, KG3DSceneEntityList& listSelected);
protected:
	virtual void HandleEngineMessages( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender );//��MessageReceiver��Ա���յ���Ϣ֮��ת��,���Ը��ǣ����ǵĻ����ǵõ�����������ͬ������
	
	KG3DScalCoordinate& GetCoordScale(){return m_coordScale;}	
	KG3DRotationCoordinateOld& GetCoordRotate(){return m_coordRotate;}
	KG3DMoveCoordinate& GetCoordMove(){return m_coordMove;}

	HRESULT Init();
	HRESULT UnInit();
public:
	//////////////////////////////////////////////////////////////////////////
	//virtual HRESULT AddSelectableEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer);
	//virtual HRESULT AddSelectableEntity(const KG3DSceneEntity& Entity, BOOL bSelected = FALSE);//�Ƿ�����ѡ��
	//virtual HRESULT RemoveSelectableEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, BOOL bEitherRemoveInSelectedList);	//��Engine�������������������������������Entity��Factory�Ĺ��ܣ���Entity������һ���ˡ�������ʱû�а취ȥ�����п�Ҳ�����ĳ�ֱ����Entity��������
	//virtual HRESULT RemoveSelectableEntity(const KG3DSceneEntity& Entity, BOOL bEitherRemoveInSelectedEntity = TRUE);//��Ȼ��Selectable�ˣ�ҲӦ�ô�Selected���Ƴ�

	//HRESULT AddSelectedEntity(KG3DSceneEntity Entity);
	//virtual HRESULT RemoveSelectedEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer);

	//virtual HRESULT GetSelectedEntityBBox(AABBOX& Box);//�õ�ѡ�������İ�Χ�У���Ҫ��������Z//�����ƶ���GetSelectionBBoxGetter
	//virtual HRESULT GetSelectedEntityRotation(D3DXQUATERNION* pRot, D3DXVECTOR3* pRotXYZ);	//û���õ�
	//virtual HRESULT RenderSelectedEntityCenter();//<RenderSelectedEntityCenter Refactor>2009-1-15 ȥ��,�ϲ���RenderSelectedEntityMark

	//���еĲ��ǲ���ѡ��Ĺ��ܣ���ȫ����ȫ�ֺ����ķ�ʽ��KG3DSceneFunctions��,����KG3DSceneSelectionTool�ӿ���ʵ��

	//HRESULT SelectNone(DWORD dwClass);	//�ĳ���ClearSelection
	//HRESULT SelectAll(DWORD dwClass);		
	//HRESULT SelectInvert(DWORD dwClass);

	//virtual HRESULT OnToggleStateMove();//<OnToggleStateMove Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
	//virtual HRESULT OnToggleStateRotation();//<OnToggleStateRotation Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
	//virtual HRESULT OnToggleStateScaling();//<OnToggleStateScaling Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
	

    // special for SFX
    //HRESULT ScaleDirectSize(float fSizeScale);//��������������ĳ���KSF::SetSelectionSFXScaleSize

	HRESULT RenderSelectedEntityMark(DWORD dwEditState,D3DVIEWPORT9& Viewport,D3DXMATRIX& Projection,D3DXMATRIX& View,BOOL bShowScreenMask);
	//HRESULT RenderSelectableEntityMark(D3DVIEWPORT9& Viewport,D3DXMATRIX& Projection,D3DXMATRIX& View);
	
	
	// ����ѡ���ʱ���Ƿ�ֻѡ��һ��Entity
	/*BOOL    IsDarkSelectedOneEntity(
		D3DVIEWPORT9& Viewport,
		D3DXMATRIX&   Projection,
		D3DXMATRIX&   View,
		vector<KG3DSceneLayer*>* pvecLayer,
		KG3DSceneEntity* pSelEntity
		);*/	//û�м�鵽ʹ��

	//HRESULT CopySelectedEntity();//<Copy Refactor>
	//virtual HRESULT ParseCopiedEntity();//2009-2-11 ��������Ķ���SceneEditBase��PasteSelection
	//virtual INT GetCopiedEntityCount() { return (INT)m_listClipBoardEntity.size(); }//2009-1-12<Copy Refactor>

	KG3DSceneSelectBase(KG3DScene& scene);
	virtual ~KG3DSceneSelectBase(void);

//(by dengzhihui Refactor 2006��11��24��
	//virtual INT GetSelectedEntitySize();
	//virtual HRESULT GetSelectedEntity(INT nIndex, IEKG3DSceneEntity** ppEntity);//��KG3DSceneSelectionToolAccessProxy�����ʳ�Ա
	virtual INT GetSelectableEntitySize();
	virtual HRESULT GetSelectableEntity(INT nIndex, IEKG3DSceneEntity** ppEntity);
	//virtual HRESULT RemoveSelectedEntity(SCENEENTITYTYPE EntityType,PVOID EntityPointer);
	//virtual HRESULT AddSelectedEntity(SCENEENTITYTYPE EntityType,PVOID EntityPointer);
	//virtual HRESULT AddSelectedEntity(IEKG3DSceneEntity* Entity);
	//virtual HRESULT AddSelectableEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer);
	//virtual HRESULT ClearSelectedEntity();
	virtual HRESULT CopySelectable2SelectedSet(LPVOID pBillboard);
	

	//)
};
