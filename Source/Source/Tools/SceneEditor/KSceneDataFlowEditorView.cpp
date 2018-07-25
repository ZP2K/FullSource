// KSceneDataFlowEditorView.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KSceneDataFlowEditorView.h"
#include "KSceneDataFlowEditorDoc.h"
#include "IEEditor.h"
// KSceneDataFlowEditorView

IMPLEMENT_DYNCREATE(KSceneDataFlowEditorView, CView)

#define GET_SCENE() \
	KSceneDataFlowEditorDoc* pDoc = (KSceneDataFlowEditorDoc*) GetDocument();\
	IEKG3DScene* pScene = pDoc->GetScene();\
	if(!pScene) return;

#define GET_EDITOR() \
	IEKG3DSceneDataFlowEditor* pEditor = NULL;\
	pScene->GetSceneDataFlowEditor(&pEditor); \
	if(!pEditor)	\
	return;

#define GET_BASE()\
	IEKG3DSceneEditorBase* pBase = NULL;\
	pEditor->GetSceneEditorBase(&pBase);\
	_ASSERTE(pBase);\
	if (!pBase) return;

KSceneDataFlowEditorView::KSceneDataFlowEditorView()
{
	m_nOutputWindowID = -1;
}

KSceneDataFlowEditorView::~KSceneDataFlowEditorView()
{
	GET_SCENE();

	KG_PROCESS_ERROR(pScene);

	if(m_nOutputWindowID!=-1)
	{
		pScene->RemoveOutputWindow(m_nOutputWindowID);
		m_nOutputWindowID = -1;
	}
Exit0:
	;
}

BEGIN_MESSAGE_MAP(KSceneDataFlowEditorView, CBaseView)
ON_WM_SIZE()
ON_COMMAND(ID_SELECTBYNAME, CBaseView::OnPopMenuSelect)
ON_COMMAND(ID_SELECT,CBaseView::OnPopMenuSelect)
ON_COMMAND(ID_MOVE,CBaseView::OnPopMenuMove)
END_MESSAGE_MAP()


// KSceneDataFlowEditorView drawing

void KSceneDataFlowEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// KSceneDataFlowEditorView diagnostics


#ifdef _DEBUG
void KSceneDataFlowEditorView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void KSceneDataFlowEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// KSceneDataFlowEditorView message handlers

void KSceneDataFlowEditorView::OnInitialUpdate()
{
	CBaseView::OnInitialUpdate();

	GET_SCENE();
	KGLOG_PROCESS_ERROR(pScene);

	static PROXYKEYMAP KEYMAP[] =
	{	//           C  S  A  L  M  R  ?    Trigger     ID  Prev Ig Exclusive
		{VK_MENU   , 0, 0, 1, 0, 1, 0, 0, WM_MOUSEMOVE,   EXEACTION_ROTATE_CAMERA,  0, 0, 0},	//alt+MBT, ��ת�������ͷ
		{VK_CONTROL, 1, 0, 0, 0, 1, 0, 0, WM_MOUSEMOVE,   EXEACTION_ZOOM_VIEW,  0, 0, 0},	//ctrl+MBT, ���������ӽ�
		{VK_MBUTTON, 0, 0, 0, 0, 1, 0, 0, WM_MOUSEMOVE,   EXEACTION_PAN_VIEW,  0, 0, 0},	//MBT, PAN��ͼ
		{VK_LBUTTON, 1, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_INVERT_ACTION,  0, 0, 0},	//CTRL��LBT������ǰ����

		{VK_LBUTTON, 0, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_LEFE_KEY_DOWN,  0, 0, 0},	//������£�������ѡ�񡢸ı���εȺܶද��
		{VK_LBUTTON, 1, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_SELECTION_ADD,  0, 0, 0},	//�ڵ�ǰѡ��Χ�У�������ѡ������
		{VK_LBUTTON, 0, 0, 1, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_SELECTION_SUTRACT,  0, 0, 0},	//�ڵ�ǰѡ��Χ�У��ټ���ѡ������

		{0		   , 0, 0, 0, 0, 0, 0, 0, 0             , 0,  0, 0},	//����
	};

	GET_EDITOR();
	GET_BASE();

	m_InputProxy.Init(pBase, this, KEYMAP);

	pScene->AddOutputWindow("DataFlow Main", GetSafeHwnd(), OUTPUTWND_DATAFLOWEDITOR_MAIN, 0,
		&m_nOutputWindowID);
	pScene->SetCurrentOutputWindow(m_nOutputWindowID);


Exit0:
	;
}

void KSceneDataFlowEditorView::OnSize(UINT nType, int cx, int cy)
{
	CBaseView::OnSize(nType, cx, cy);

	GET_SCENE();
	pScene->OnResizeWindow(NULL);
}


LRESULT KSceneDataFlowEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = CBaseView::WindowProc(message, wParam, lParam);

	return hr;
}