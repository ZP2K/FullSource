// SwordTestDoc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "SwordTestDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

// KSwordTestDoc

IMPLEMENT_DYNCREATE(KSwordTestDoc, CDocument)

KSwordTestDoc::KSwordTestDoc()
{
	//m_lpSwordTest = NULL;
}

BOOL KSwordTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

KSwordTestDoc::~KSwordTestDoc()
{
}


BEGIN_MESSAGE_MAP(KSwordTestDoc, CDocument)
END_MESSAGE_MAP()


// KSwordTestDoc ���

#ifdef _DEBUG
void KSwordTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void KSwordTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// KSwordTestDoc ���л�

void KSwordTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

//HRESULT KSwordTestDoc::SetScene(KSceneSwordTest *pScene)
//{
//	m_lpSwordTest = pScene;
//	return S_OK;
//}

// KSwordTestDoc ����
