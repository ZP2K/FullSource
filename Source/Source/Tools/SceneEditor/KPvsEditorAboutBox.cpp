// KPvsEditorAboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KPvsEditorAboutBox.h"


// KPvsEditorAboutBox dialog

IMPLEMENT_DYNAMIC(KPvsEditorAboutBox, CDialog)

KPvsEditorAboutBox::KPvsEditorAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(KPvsEditorAboutBox::IDD, pParent)
    , m_strText(_T(""))
{

}

KPvsEditorAboutBox::~KPvsEditorAboutBox()
{
}

void KPvsEditorAboutBox::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT, m_strText);
}


BEGIN_MESSAGE_MAP(KPvsEditorAboutBox, CDialog)
    ON_BN_CLICKED(IDOK, &KPvsEditorAboutBox::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL KPvsEditorAboutBox::OnInitDialog()
{
    BOOL res = CDialog::OnInitDialog();

    m_strText = "���\r\n\r\n��� : ѡ������\r\n\r\n�Ҽ� : ��ת��ͷ\r\n\r\n���� : ���ž�ͷ\r\n\r\n\r\n"
        "����\r\n\r\nW : �ƶ�״̬\r\n\r\nE : ��ת״̬\r\n\r\nR : ����״̬\r\n\r\nQ : �ָ�ѡ��״̬\r\n\r\n"
        "[ : ��С��������\r\n\r\n[ : �Ŵ���������\r\n\r\ndelete : ɾ��ѡ������\r\n\r\n"
        "��סShift������������ : �л��Ѿ����ù������";

    UpdateData(FALSE);

    return res;
}
// KPvsEditorAboutBox message handlers

void KPvsEditorAboutBox::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}
