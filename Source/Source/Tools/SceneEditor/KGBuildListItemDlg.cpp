// KGBuildListItemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KGBuildListItemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////


// KGBuildListItemDlg �Ի���

IMPLEMENT_DYNAMIC(KGBuildListItemDlg, CDialog)
KGBuildListItemDlg::KGBuildListItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KGBuildListItemDlg::IDD, pParent)
{
}

KGBuildListItemDlg::~KGBuildListItemDlg()
{
}

void KGBuildListItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KGBuildListItemDlg, CDialog)
END_MESSAGE_MAP()


// KGBuildListItemDlg ��Ϣ�������
