////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSFXEditorDoc.cpp
//  Version     : 1.0
//  Creator     : Zeb
//  Create Date : 2005-10-18 17:17:39
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "KSceneSFXEditorDoc.h"


////////////////////////////////////////////////////////////////////////////////
//#include "KG3DEngineManager.h"//by dengzhihui Refactor 2006��11��6��
//#include "SceneEditorCommon.h"	//by dengzhihui Refactor 2006��11��6��
#include "IEKG3DScene.h"
#include "IEKG3DSceneSFXEditor.h"
#include "KGAlertMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(KSceneSFXEditorDoc, CDocument)

KSceneSFXEditorDoc::KSceneSFXEditorDoc(void)
{
    m_lpSceneSFXEditor = NULL;
}

KSceneSFXEditorDoc::~KSceneSFXEditorDoc(void)
{
    
}

namespace {
    void AddAlertMessage(const char strMessage[]) {
        g_AlertMessageBox.AddMessage(strMessage);
    }
}

BOOL KSceneSFXEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!__super::OnOpenDocument(lpszPathName))
		return FALSE;

    if (!::IsWindow(g_AlertMessageBox.GetSafeHwnd()))
        g_AlertMessageBox.Create(g_AlertMessageBox.IDD);
    g_AlertMessageBox.ShowWindow(SW_HIDE);
    g_AlertMessageBox.Clear();
    g_pEngineManager->RegistPushAlertMessageFunction(AddAlertMessage);

	// TODO:  Add your specialized creation code here
	IEKG3DScene* pScene = NULL;
	g_pEngineManager->NewOneScene(SCENETYPE_SFXEDITOR,(IKG3DScene**)&pScene); //by dengzhihui Refactor 2006��11��6��
	
    pScene->GetSceneSFXEditor(&m_lpSceneSFXEditor);
    
	g_pEngineManager->SetFocusScene(pScene);	//by dengzhihui Refactor 2006��11��6��
    if (m_lpSceneSFXEditor)
    {
        HRESULT hr = m_lpSceneSFXEditor->OpenSFX(lpszPathName);

		if (hr == ERROR_CONTINUE)
        {
            AddAlertMessage("warning : can not find the names file");
			//MessageBox(AfxGetMainWnd()->m_hWnd, "��ȡ����ʧ�ܣ����ϣ˼�����", "������ʾ", MB_OK);
        }

		if (hr == ERROR_INVALID_DATA)
		{
            AddAlertMessage("warning : the names file is not match the sfx");
			//MessageBox(AfxGetMainWnd()->m_hWnd, "�����ļ�����Ч��ƥ�䣬���飡", "������ʾ", MB_OK);
			//return FALSE;
		}

		if (FAILED(hr))
        {
            AddAlertMessage("error : faint~ unknow!!");
            g_AlertMessageBox.CenterWindow(AfxGetMainWnd());
            g_AlertMessageBox.ShowWindow(SW_SHOW);

			//MessageBox(AfxGetMainWnd()->m_hWnd, "��SFXʧ��", "������ʾ��", MB_OK);
			return FALSE;
        }

        if (g_AlertMessageBox.Size() > 0)
        {
            g_AlertMessageBox.CenterWindow(AfxGetMainWnd());
            g_AlertMessageBox.ShowWindow(SW_SHOW);
        }

        SetModifiedFlag(TRUE);
    }
	    
    return TRUE;
}

BOOL KSceneSFXEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: Add your specialized code here and/or call the base class
    if (m_lpSceneSFXEditor)
    {
		HRESULT hr = m_lpSceneSFXEditor->SaveToFile(lpszPathName);
		if (hr == ERROR_CONTINUE)
			MessageBox(AfxGetMainWnd()->m_hWnd, "��¼���ֳ�����Ч���ֶ�ʧ��", "������ʾ", MB_OK);
		if (FAILED(hr))
		{
			MessageBox(AfxGetMainWnd()->m_hWnd, "�浵ʧ�ܣ�", "������ʾ", MB_OK);
			return FALSE;
		}
    }

    return TRUE;
}

void KSceneSFXEditorDoc::OnCloseDocument()
{
	if (m_lpSceneSFXEditor)
	{
		if (m_lpSceneSFXEditor->IsModifyMode())
			m_lpSceneSFXEditor->SyncBindInfo();

        IEKG3DScene* piScene = NULL;
        m_lpSceneSFXEditor->GetScene(&piScene);
        if (piScene)
        {
		    g_pEngineManager->RemoveScene(piScene);//by dengzhihui Refactor 2006��11��6��
        }
		m_lpSceneSFXEditor = NULL;
	}

	CDocument::OnCloseDocument();
}

BOOL KSceneSFXEditorDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    IEKG3DScene* pScene = NULL;
    g_pEngineManager->NewOneScene(SCENETYPE_SFXEDITOR,(IKG3DScene**)&pScene);//by dengzhihui Refactor 2006��11��6��
    pScene->GetSceneSFXEditor(&m_lpSceneSFXEditor);
    g_pEngineManager->SetFocusScene(pScene);//by dengzhihui Refactor 2006��11��6��

    if(m_lpSceneSFXEditor)
    {
        m_lpSceneSFXEditor->NewSFX();
    }

    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
HRESULT KSceneSFXEditorDoc::GetSceneSFXEditor(
    IEKG3DSceneSFXEditor **ppiRetSceneSFXEditor
)
{
    HRESULT hResult  = E_FAIL;

    KGLOG_PROCESS_ERROR(ppiRetSceneSFXEditor);

    *ppiRetSceneSFXEditor = m_lpSceneSFXEditor;

//Exit1:
    hResult = S_OK;
Exit0:
    return hResult;
}
