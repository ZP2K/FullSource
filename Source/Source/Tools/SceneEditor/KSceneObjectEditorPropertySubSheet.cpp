// KSceneObjectEditorPropertySubSheet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KSceneObjectEditorPropertySubSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////


// KSceneObjectEditorPropertySubSheet �Ի���

IMPLEMENT_DYNAMIC(KSceneObjectEditorPropertySubSheet, CPropertyPage)
KSceneObjectEditorPropertySubSheet::KSceneObjectEditorPropertySubSheet()
	: CPropertyPage(KSceneObjectEditorPropertySubSheet::IDD)
{
}

KSceneObjectEditorPropertySubSheet::~KSceneObjectEditorPropertySubSheet()
{
}

void KSceneObjectEditorPropertySubSheet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KSceneObjectEditorPropertySubSheet, CPropertyPage)
END_MESSAGE_MAP()


// KSceneObjectEditorPropertySubSheet ��Ϣ�������
