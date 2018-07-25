// KPvsEditorView.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KPvsEditorView.h"
#include "KPvsEditorDoc.h"
#include "IEEditor.h"
#include "MainFrm.h"
#include "KSceneCommonParamDialog.h"
#include "KSceneSceneEditorDialogDirMgr.h"
#include "KSceneSettingDialog.h"



// KPvsEditorView

IMPLEMENT_DYNCREATE(KPvsEditorView, CBaseView)

KPvsEditorView::KPvsEditorView()
    : m_pDoc(NULL),
      m_nOutputWindowID(0xffffffff),
      m_lpDialogDirMgr(NULL),
      m_pSceneSettingDialog(NULL)
{

}

KPvsEditorView::~KPvsEditorView()
{
    SAFE_DELETE(m_pSceneSettingDialog);
}

BEGIN_MESSAGE_MAP(KPvsEditorView, CBaseView)
    ON_WM_SIZE()
    ON_WM_SETCURSOR()
    ON_COMMAND(ID_COORD, OnSetCoord)
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_EDIT_COPY, &KPvsEditorView::OnEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CBaseView::OnEditorModeUpdate)
    ON_COMMAND(ID_EDIT_PASTE, &KPvsEditorView::OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CBaseView::OnEditorModeUpdate)	
    ON_COMMAND(ID_COPY, &KPvsEditorView::OnEditCopy)
    ON_UPDATE_COMMAND_UI(ID_COPY, &CBaseView::OnEditorModeUpdate)
    ON_COMMAND(ID_PAST, &KPvsEditorView::OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_PAST, &CBaseView::OnEditorModeUpdate)	
    ON_COMMAND(ID_PLAYERMODE, &OnSetPMode)
    ON_WM_DESTROY()

    ON_COMMAND(ID_ACTION_UP, &KPvsEditorView::OnActionUp)
    ON_UPDATE_COMMAND_UI(ID_ACTION_UP, &CBaseView::OnEditorModeUpdate)
    ON_COMMAND(ID_ACTION_DOWN, &KPvsEditorView::OnActionDown)
    ON_UPDATE_COMMAND_UI(ID_ACTION_DOWN, &CBaseView::OnEditorModeUpdate)

    ON_COMMAND(ID_ACTION_LEFT, &KPvsEditorView::OnActionLeft)	
    ON_UPDATE_COMMAND_UI(ID_ACTION_LEFT, &CBaseView::OnEditorModeUpdate)
    ON_COMMAND(ID_ACTION_RIGHT, &KPvsEditorView::OnActionRight)
    ON_UPDATE_COMMAND_UI(ID_ACTION_RIGHT, &CBaseView::OnEditorModeUpdate)

    ON_COMMAND(ID_ACTION_PGUP, &KPvsEditorView::OnActionPageUp)
    ON_UPDATE_COMMAND_UI(ID_ACTION_PGUP, &CBaseView::OnEditorModeUpdate)
    ON_COMMAND(ID_ACTION_PGDN, &KPvsEditorView::OnActionPageDown)
    ON_UPDATE_COMMAND_UI(ID_ACTION_PGDN, &CBaseView::OnEditorModeUpdate)

	ON_COMMAND(ID_ENGINECOMMAND, &KPvsEditorView::OnEngineCommand)
    ON_COMMAND(ID_DIRMGR, OnDirMgr)

    ON_COMMAND(ID_SCENESETTING, OnScenesetting)

END_MESSAGE_MAP()

void KPvsEditorView::OnEngineCommand()
{
	std::tstring strCommand = _T("");

	KSceneCommonParamDialog dlg;
	INT handleParam = dlg.AddAnParam(_T("�������"), strCommand,TRUE);

	if(handleParam < 0)
		return;

	if(IDOK != dlg.DoModal())
		return;

	BOOL bRet = dlg.GetData(handleParam, strCommand);
	if(! bRet)
		return;

	HRESULT hr = g_pEngineManager->ExecuteCommand(strCommand.c_str());
	if (FAILED(hr))
	{
		MessageBox(_T("��������ʧ�ܣ�������������ʹ���"));
	}
}

void KPvsEditorView::OnDirMgr()
{
    if(!m_lpDialogDirMgr)
    {
        m_lpDialogDirMgr = new KSceneSceneEditorDialogDirMgr;
       // m_lpDialogDirMgr->SetSceneEditor(m_pP);
        m_lpDialogDirMgr->Create(KSceneSceneEditorDialogDirMgr::IDD,this);
    }
    m_lpDialogDirMgr->ShowWindow(TRUE);
}


void KPvsEditorView::OnScenesetting()
{
    if (!m_pSceneSettingDialog)
    {
        m_pSceneSettingDialog = new KSceneSettingDialog;
    }
    if (m_pSceneSettingDialog)
    {
        if(m_pSceneSettingDialog->GetSafeHwnd())
        {
            m_pSceneSettingDialog->ShowWindow(SW_SHOW);
        }
        else
        {
            BOOL bRet = m_pSceneSettingDialog->Create(KSceneSettingDialog::IDD, this);
            if(bRet)
            {
                m_pSceneSettingDialog->ShowWindow(SW_SHOW);
            }
        }
    }

}

void KPvsEditorView::OnActionUp()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetScenePvsEditor())
        return;

    m_pDoc->GetScene()->GetScenePvsEditor()->OnCommand(ctUp);
}

void KPvsEditorView::OnActionDown()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetScenePvsEditor())
        return;

    m_pDoc->GetScene()->GetScenePvsEditor()->OnCommand(ctDown);
}

void KPvsEditorView::OnActionLeft()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetScenePvsEditor())
        return;

    m_pDoc->GetScene()->GetScenePvsEditor()->OnCommand(ctLeft);
}

void KPvsEditorView::OnActionRight()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetScenePvsEditor())
        return;

    m_pDoc->GetScene()->GetScenePvsEditor()->OnCommand(ctRight);
}

void KPvsEditorView::OnActionPageUp()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetScenePvsEditor())
        return;

    m_pDoc->GetScene()->GetScenePvsEditor()->OnCommand(ctPageUp);
}

void KPvsEditorView::OnActionPageDown()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetScenePvsEditor())
        return;

    m_pDoc->GetScene()->GetScenePvsEditor()->OnCommand(ctPageDown);
}

void KPvsEditorView::OnSetPMode()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetScenePvsEditor())
        return;

    m_pDoc->GetScene()->GetScenePvsEditor()->EnablePMode(!(m_pDoc->GetScene()->GetScenePvsEditor()->IsPMode()));
}

void KPvsEditorView::OnEditCopy()
{
    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetSelectBase())
        return;


    //m_pDoc->GetScene()->GetSelectBase()->CopySelectedEntity();
	
	//2009-1-12 <Copy Refactor>���Ƹĳ�3dsmax�ķ�ʽ������Ҫ��Ctrl+C��¼ѡ�񣬶���ֱ����Ctrl+V�͸��Ƴ���ǰ��ѡ�񡣼�¼ѡ�����������Ҫ����̫���������Ե����⣬����ҲӦ�þ�����Max�Ĳ�������
	MessageBox(_T("���Ʋ���ҪCtrl+C��ֱ�Ӱ�Ctrl+V�Ϳ��Ը���ѡ����������ο�3dsmax�Ĳ���"));
}

void KPvsEditorView::OnEditPaste()
{
    if (MessageBox(TEXT("�Ƿ�Ҫ����ѡ������?"), TEXT("Message"), MB_YESNOCANCEL | MB_ICONWARNING) != IDYES)
        return;

    if (!m_pDoc || !m_pDoc->GetScene() || !m_pDoc->GetScene()->GetSelectBase())
        return;

    //m_pDoc->GetScene()->GetSelectBase()->ParseCopiedEntity();
	IEKG3DSceneEditorBase* pEditBase = NULL;
	m_pDoc->GetScene()->GetSceneEditorBase(&pEditBase);
	_ASSERTE(NULL != pEditBase);
	if (NULL != pEditBase)
	{
		pEditBase->PasteSelection();
	}
}

void KPvsEditorView::OnSetFocus(CWnd* pOldWnd)
{
    CBaseView::OnSetFocus(pOldWnd);

    if (m_pDoc && m_pDoc->GetPvsScene() && m_pDoc->GetPvsScene()->GetScene())
        g_pEngineManager->SetFocusScene(m_pDoc->GetPvsScene()->GetScene());
}

void KPvsEditorView::OnSetCoord()
{
    if (!m_pDoc || !m_pDoc->GetPvsScene())
        return;

    IEKG3DScenePvsEditor* pScene = m_pDoc->GetPvsScene();

    CXTPCommandBars* pCommandBars = ((MFCFramework::CMainFrame*)AfxGetMainWnd())->GetCommandBars();

    if (!pCommandBars)
        return;

    CXTPToolBar* pToolBarStd = pCommandBars->GetToolBar(IDR_MAINFRAME);

    if (!pToolBarStd)
        return;

    CXTPControlComboBox* pControlCombo = (CXTPControlComboBox*)pToolBarStd->GetControl(18);
    if (!pControlCombo)
        return;

    int nSel = pControlCombo->GetCurSel();
    switch (nSel)
    {
    case 0 :
        pScene->SetCoordMode(KG3DCOORD_WORLD);
        break;
    case 1 :
        pScene->SetCoordMode(KG3DCOORD_LOCAL);
        break;
    default :
        break;
    }
}

void KPvsEditorView::OnDestroy()
{
    if (m_pDoc && m_pDoc->GetPvsScene() && m_pDoc->GetPvsScene()->GetScene())
        m_pDoc->GetPvsScene()->GetScene()->RemoveOutputWindow(m_nOutputWindowID);
}

void KPvsEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// KPvsEditorView diagnostics

#ifdef _DEBUG
void KPvsEditorView::AssertValid() const
{
	CBaseView::AssertValid();
}

#ifndef _WIN32_WCE
void KPvsEditorView::Dump(CDumpContext& dc) const
{
	CBaseView::Dump(dc);
}
#endif
#endif //_DEBUG


LRESULT KPvsEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = CBaseView::WindowProc(message, wParam, lParam);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    msg.hwnd    = m_hWnd;
    msg.wParam  = wParam;
    msg.lParam  = lParam;
    msg.message = message;
    
    if (message == WM_MOUSEMOVE)
        SetFocus();

    switch (message)
    {
    case WM_LBUTTONDOWN :
    case WM_LBUTTONUP   :
    case WM_MBUTTONDOWN :
    case WM_MBUTTONUP   :
    case WM_RBUTTONDOWN :
    case WM_RBUTTONUP   :
    case WM_MOUSEMOVE   :
    case WM_KEYDOWN     :
    case WM_KEYUP       :
    case WM_CHAR        :
        {
            g_pEngineManager->ProcessMessage(message, wParam, lParam);	//<PROCESS_MESSAGE>
        }
        break;
    default :
        break;
    }

    if (message == WM_KEYDOWN)
    {      
        if (wParam == VK_TAB)
        {
            m_pDoc->GetPvsScene()->GetPvs()->NextSubOctree();
        }
    }
  
    return lResult;
}

void KPvsEditorView::OnInitialUpdate()
{
    CBaseView::OnInitialUpdate();

    m_pDoc = static_cast<KPvsEditorDoc*>(GetDocument());

    if (m_pDoc && m_pDoc->GetPvsScene() && m_pDoc->GetPvsScene()->GetScene())
    {
        m_pDoc->GetPvsScene()->GetScene()->AddOutputWindow("Pvs Editor", GetSafeHwnd(), OUTPUTWND_PVSEDITOR_MAIN, 0, &m_nOutputWindowID);

		IEKG3DSceneEditorBase* pEditBase = NULL;
		m_pDoc->GetPvsScene()->GetScene()->GetSceneEditorBase(&pEditBase);
		if (NULL != pEditBase)
		{
			pEditBase->SetEditState(SCENESTATE_SELECT);
		}

        static PROXYKEYMAP KEYMAP[] =
        {	//           C  S  A  L  M  R  ?    Trigger     ID old
            //{VK_MENU , 0, 0, 1, 0, 1, 0, 0, WM_MOUSEMOVE,   EXEACTION_ROTATE_CAMERA,  0, 0},	//alt+MBT, ��ת�������ͷ
            {VK_RBUTTON, 0, 0, 0, 0, 0, 1, 0, WM_MOUSEMOVE,   EXEACTION_ROTATE_CAMERA,  0, 0},	//��ס�Ҽ����ţ���ת�������ͷ
            {VK_CONTROL, 1, 0, 0, 0, 1, 0, 0, WM_MOUSEMOVE,   EXEACTION_ZOOM_VIEW,  0, 0},	//ctrl+MBT, ���������ӽ�
            {VK_MBUTTON, 0, 0, 0, 0, 1, 0, 0, WM_MOUSEMOVE,   EXEACTION_PAN_VIEW,  0, 0},	//MBT, PAN��ͼ
            {VK_LBUTTON, 1, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_INVERT_ACTION,  0, 0},	//CTRL��LBT������ǰ����
		    {VK_LBUTTON, 0, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_LEFE_KEY_DOWN,  0, 0, 0},	//������£�������ѡ�񡢸ı���εȺܶද��

            {'W'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN    , EXEACTION_GO_FORWARD,  0, 2, 0},	//ǰ�� ��������
            {VK_LBUTTON, 0, 0, 0, 1, 0, 1, 0, WM_MOUSEMOVE  , EXEACTION_GO_FORWARD,  0, 0, 0},	//��ס���Ҽ����ţ���ֱ��
            {'S'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN    , EXEACTION_GO_BACKWARD,  0, 2, 0},	//���� ��������
            {'A'	   , 0, 0, 0, 1, 0, 0, 0, WM_KEYDOWN    , EXEACTION_TURN_LEFT,  0, 1, 0},	//��ת 
            {'D'	   , 0, 0, 0, 1, 0, 0, 0, WM_KEYDOWN    , EXEACTION_TURN_RIGHT,  0, 1, 0},	//��ת 
            {'A'	   , 0, 0, 0, 0, 1, 0, 0, WM_KEYDOWN    , EXEACTION_TURN_LEFT,  0, 1, 0},	//��ת 
            {'D'	   , 0, 0, 0, 0, 1, 0, 0, WM_KEYDOWN    , EXEACTION_TURN_RIGHT,  0, 1, 0},	//��ת 
            {'A'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN    , EXEACTION_MOV_LEFT,  0, 2, 0},	//���� ���Լ���
            {'D'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN    , EXEACTION_MOV_RIGHT,  0, 2, 0},	//���� ���Լ���

            {VK_RBUTTON, 0, 0, 0, 0, 0, 1, 0, WM_MOUSEMOVE , EXEACTION_CAM_CIRCLE,  0, 0, 0},	//��ס�Ҽ����ţ���ת�������
            {'Q'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , EXEACTION_MOV_UP,  0, 3, 0},	//���ߣ���������
            {'E'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , EXEACTION_MOV_DOWN,  0, 3, 0},	//�½�����������
            {' '	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , EXEACTION_JUMP,  0, 3, 0},	//������������
            {'A'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , EXEACTION_TURN_LEFT_VIEW_FOLLOWED,  0, 1, 0},	//��ת��ͬʱת�������
            {'D'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , EXEACTION_TURN_RIGHT_VIEW_FOLLOWED,  0, 1, 0},	//��ת

            {VK_LBUTTON, 1, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_SELECTION_ADD,  0, 0, 0},	//�ڵ�ǰѡ��Χ�У�������ѡ������
            {VK_LBUTTON, 0, 0, 1, 1, 0, 0, 0, WM_LBUTTONDOWN, EXEACTION_SELECTION_SUTRACT,  0, 0, 0},	//�ڵ�ǰѡ��Χ�У��ټ���ѡ������

            {0		   , 0, 0, 0, 0, 0, 0, 0, 0             , 0,  0, 0},	//����
        };

        m_InputProxy.Init(m_pDoc->GetPvsScene()->GetSceneEditBase(), this, KEYMAP);

    }


}

BOOL KPvsEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_pDoc && m_pDoc->GetPvsScene() && m_pDoc->GetPvsScene()->GetSceneEditBase())
    {
        if (m_pDoc->GetPvsScene()->GetSceneEditBase()->GetRtsActive())
        {
            switch (m_pDoc->GetPvsScene()->GetEditState())
            {
            case SCENESTATE_MOVE :
                SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE));
                return TRUE;
            case SCENESTATE_ROTATE :
                SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ROT));
                return TRUE;
            case SCENESTATE_SCALE :
                SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_SCL));
                return TRUE;
            default :
                break;
            }
        }
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void KPvsEditorView::OnSize(UINT nType, int cx, int cy)
{
    CBaseView::OnSize(nType, cx, cy);

    if (m_pDoc && m_pDoc->GetPvsScene() && m_pDoc->GetPvsScene()->GetScene() && cx != 0 && cy != 0)
        m_pDoc->GetPvsScene()->GetScene()->OnResizeWindow(NULL);

}