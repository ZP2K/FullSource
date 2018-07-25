////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGValueEditQuestTypeDlg.cpp
//  Version     : 1.0
//  Author      : ZhaoXinyu (Chiyer)
//  CopyRight   : Kingsoft Corporation All rights Reserved
//  History     : 2006-1-5 17:09:08
//  Comment     :
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SceneEditor.h"
#include "KGValueEditQuestTypeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(KGValueEditQuestTypeDlg, CDialog)
KGValueEditQuestTypeDlg::KGValueEditQuestTypeDlg(
	ITabFile* pTabFile, IIniFile* pIniFile, LPKGLISTITEM pListItem, CWnd* pParent /* = NULL  */
): CDialog(KGValueEditQuestTypeDlg::IDD, pParent)
{
	m_pTabFile   = pTabFile;
	m_pIniFile	 = pIniFile;
	m_pListItem  = pListItem;
	m_pParant	 = pParent;
}

KGValueEditQuestTypeDlg::~KGValueEditQuestTypeDlg()
{
}

void KGValueEditQuestTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_comboxType);
}

void KGValueEditQuestTypeDlg::OnCbnSelect()
{
	CString    szString;
	int        nIndex;

	UpdateData();

	nIndex = m_comboxType.GetCurSel();
	m_comboxType.GetLBText(nIndex, szString);

	GetDlgItem(IDC_G)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_K)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_T)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_P)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_E)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_O)->ShowWindow(SW_HIDE);

	if (szString == "�ռ�")
	{
		GetDlgItem(IDC_G)->ShowWindow(SW_SHOW);
	}
	else if (szString == "ɱ��")
	{
		GetDlgItem(IDC_K)->ShowWindow(SW_SHOW);
	}
	else if (szString == "����")
	{
		GetDlgItem(IDC_P)->ShowWindow(SW_SHOW);
	}
	else if (szString == "�Ի�")
	{
		GetDlgItem(IDC_T)->ShowWindow(SW_SHOW);
	}
	else if (szString == "����")
	{
		GetDlgItem(IDC_E)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_O)->ShowWindow(SW_SHOW);
	}

}

BOOL KGValueEditQuestTypeDlg::OnInitDialog()
{
	int nResult  = false;
	int nRetCode = false;

	USER_ITEM_DATA itemData;
	m_pListItem->GetStructData(&itemData, sizeof(itemData));

	CClientDC dc(this);

	nRetCode = CDialog::OnInitDialog();

	KG_PROCESS_ERROR(nRetCode);

	SetWindowText(_T("���Ա༭_��������"));

	m_comboxType.AddString("�ռ�");
	m_comboxType.AddString("ɱ��");
	m_comboxType.AddString("����");
	m_comboxType.AddString("�Ի�");
	m_comboxType.AddString("����");
	m_comboxType.AddString("����");

	nRetCode = m_comboxType.SelectString(0, itemData.szPropertyValue);
	KG_PROCESS_ERROR(nRetCode != LB_ERR);

	OnCbnSelect();

	nResult = true;
Exit0:
	return nResult;
}

void KGValueEditQuestTypeDlg::OnBnOK()
{
	USER_ITEM_DATA itemData;
	int     nIndex;
	CString szValue;
	m_pListItem->GetStructData(&itemData, sizeof(itemData));

	UpdateData();

	nIndex = m_comboxType.GetCurSel();
	m_comboxType.GetLBText(nIndex, szValue);

	m_pTabFile->WriteString(
		itemData.nRowsIndexInFile, itemData.szPropertyName, szValue
	);

	strncpy_s(
		itemData.szPropertyValue, szValue.GetBuffer(),
		sizeof(itemData.szPropertyValue)
	);
	itemData.szPropertyValue[sizeof(itemData.szPropertyValue) - 1] ='\0';

	m_pListItem->SetStructData(&itemData);
	if (m_pParant)
		m_pParant->SendMessage(WM_USER_UPDATE_LIST_ITEM, (WPARAM)m_pListItem);

	OnOK();
}

BEGIN_MESSAGE_MAP(KGValueEditQuestTypeDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO, OnCbnSelect)
	ON_BN_CLICKED(IDOK, OnBnOK)
END_MESSAGE_MAP()

