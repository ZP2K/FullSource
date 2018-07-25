#pragma once
#include "KG3DScene.h"
#include ".\kg3dsceneselectbase.h"
#include "IEEditor.h"

interface IEKG3DSceneLayer;
class     KG3DSceneLayer;
class	  KG3DMessageReceiver;
class	KG3DTransformTool;
struct KG3DSelectionBBoxGetter;

class KG3DSceneEditorBase :
	public KG3DScene, public KG3DSceneSelectBase, public IEKG3DSceneEditorBase
{
public:
	virtual HRESULT SetEditState(DWORD dwState); 
	virtual HRESULT GetMouseXY(INT* pX, INT* pY);
	virtual HRESULT SetMouseXY(INT pX, INT pY);
	virtual ULONG GetEditState(){return m_dwEditState;}

	virtual void    SetRtsActive(BOOL active) { m_RtsActive = active; }
    virtual BOOL    GetRtsActive() { return m_RtsActive; }
    BOOL m_RtsActive;
	BOOL    m_bEditing;         //�Ƿ��ڱ༭״̬

	float   m_fWheel;

    int     m_bEnableMoveCam;
	
	KG3DSceneLayer         *m_pRootLayer;
	
	//vector<KG3DSceneLayer *>m_vecSceneLayer;
	virtual KG3DSceneSelectionTool&	GetSelectionTool();
	virtual KG3DTransformTool& GetTransformTool(DWORD dwType);//Type��KG3DTYPE_TRANSLATION_TOOL��

	virtual HRESULT GetSelectionToolInterface(IEKG3DSceneSelectionTool** ppTool);

	virtual void __stdcall OnReceiveMessage(const KG3DMessage& Message , IKG3DMessageSenderBase* pSender);
public:
	void MsgOnMouseMove(int fwKeys,int xPos,int yPos,KG3DSceneOutputWnd *pWnd);

	virtual HRESULT Init();
	virtual HRESULT UnInit();

	//virtual HRESULT EnableEditCameraKeyFrame(BOOL bEdit);
	virtual HRESULT GetFirstSelectedModel(IEKG3DModel*& pModel);

	virtual HRESULT FrameMove();
	virtual HRESULT ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);//<PROCESS_MESSAGE>
public:
	KG3DSceneEditorBase(void);
	virtual ~KG3DSceneEditorBase(void);
	//(by dengzhihui 2006��9��13�� 15:38:32 //���ӿ�ݼ������⴦��
	virtual INT ExecAction(INT nActionID, INT nExecState, WPARAM wParam, LPARAM lParam);
	virtual INT ExecActionSelect(INT nActionID, INT nExecState, INT nXOff, INT nYOff);
	virtual void SyncPickRay();	//���ⲿ�������Ϣ����

	//void ZoomView(short nDelta, POINT pt);
	virtual HRESULT SetEditing(DWORD dwEditing);
	virtual DWORD GetEditing();

	/*enum enumCommandType{ctUp, ctDown, ctLeft, ctRight, ctPageDown, ctPageUp, ctHome, ctEnd, ctInc, ctDec, 
	ctHideObject, ctHideTerrain, ctHideGrass, ctAddKeyFrame, ctDropObject};*/
	virtual void OnCommand(enumCommandType eCommandType);
	//virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);//<PROCESS_MESSAGE>//2009-1-14 ע����û�м�鵽�κ�����
	virtual HRESULT GetRootLayer(IEKG3DSceneLayer** pLayer);
	//virtual INT GetSceneLayerSize();
	//virtual HRESULT GetSceneLayer(INT nIndex , IEKG3DSceneLayer** pLayer);
	//virtual HRESULT ClearSelectedEntity();
	virtual HRESULT GetSceneSelector(IEKG3DSceneSelectBase** ppSelector);
	virtual HRESULT GetEditState(DWORD* pdwState);

	virtual HRESULT PasteSelection();

	virtual HRESULT AddRenderEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer);
	virtual HRESULT RemoveRenderEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer);
	virtual HRESULT AddRenderEntity(KG3DSceneEntity Entity, BOOL bIsSelectable = TRUE, BOOL bIsSelected = FALSE);
	virtual HRESULT RemoveRenderEntity(IKG3DResourceBase *pEntity, unsigned uClass, BOOL bSelectable = TRUE);
	virtual HRESULT RemoveRenderEntity(KG3DSceneEntity Entity, BOOL bEitherRemoveInSelectedList = TRUE);
	/*
	����PVS�ȱ༭�����ǲ���Ҫ���ӵĵ��ι���ģ�ֻ��Ҫ�ż�������͹��ˣ�ԭ������listSelectable������ġ�
	����������������Ⲣ������Selectable�����壬��SceneSceneEditor������ܳ�ͻ��
	����������ʱ�ع������⻯�Ĺ��������ѳ�SceneSceneEditor֮������ԭ����SetEntitySelectable������

	���ع����ǰ����ʱ�����þɵ�ѡ����
	*/
	
	HRESULT InsertEntitySelectable(KG3DSceneEntity& entity, BOOL bInsertAndSetSelectable);//<Selectable Refactor>//��BOOLΪFALSE��ʱ��ͬʱ�����ȥ
	virtual HRESULT IEInsertEntitySelectable(SCENEENTITYTYPE dwType, LPVOID pPoint, BOOL bInsertAndSetSelectable);//<Selectable Refactor>
private:
	DWORD   m_dwEditState;//�ýӿڲ��������EditState�ı仯�Ƿǳ����еģ������÷�����������Ȼ���ܽػ�Ļ����ܶණ����������
	KG3DMessageReceiver*		m_pCustomReceiver;	//����SelectionTool�ȵ���Ϣ

	KG3DSelectionBBoxGetter*	m_pSelectionBoxGetter;	//Strategeģʽ��ѡ�����Ĺ���Ӧ����С������������Box���㷨�Ǹ߲��㷨��Ӧ������ײ�ʵ��
protected:
	KG3DSceneEntityList	m_CtEntities;
protected:
	virtual void MsgOnLButtonDown(WPARAM wParam, LPARAM lParam);//<PROCESS_MESSAGE>
	virtual void MsgOnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void MsgOnRButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void MsgOnRButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void MsgOnMButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void MsgOnMButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void MsgOnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual void MsgOnMouseMove(WPARAM wParam, LPARAM lParam);//�¹��߽���֮��ͬ����������ɾ��
	virtual void MsgOnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void MsgOnKeyUp(WPARAM wParam, LPARAM lParam);
	

	virtual HRESULT DeleteSelectedEntity();

	KG3DSelectionBBoxGetter& GetSelectionBBoxGetter();
	HRESULT	SetSelectionBBoxGetter(KG3DSelectionBBoxGetter& newGetter, KG3DSelectionBBoxGetter** ppOld);//���Set�����Ļ����������ڻᱻ�ӹܣ��ɵĻ��ڽ���д�����������Ҫ�������Delete

private:
	static VOID TDDCheck(LPVOID pExtraData);
	static VOID TDDCheckAfterInitialized(LPVOID pExtraData);
protected:
	virtual VOID	ZoomToSelection();
};
