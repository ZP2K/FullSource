// GameConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <io.h>

#include "GameConfig.h"
#include "GameConfigDlg.h"
#include "KGPublic.h"
#include "kglog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

KGameConfigDlg::KGameConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KGameConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void KGameConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_MainTab);
}

BEGIN_MESSAGE_MAP(KGameConfigDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OK, &KGameConfigDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_QUIT, &KGameConfigDlg::OnBnClickedBtnQuit)
	ON_BN_CLICKED(IDC_BTN_SYSINFO, &KGameConfigDlg::OnBnClickedBtnSysInfo)
	ON_BN_CLICKED(IDC_BTN_APPLY, &KGameConfigDlg::OnBnClickedBtnApply)
END_MESSAGE_MAP()


// KGameConfigDlg ��Ϣ�������

BOOL KGameConfigDlg::OnInitDialog()
{
    int nResult  = false;
    int nRetCode = false;
    TCHAR szApplicationPath[MAX_PATH];
    TCHAR szConfigFileName[MAX_PATH];
    TCHAR *pchLashSlash = NULL;

    nRetCode = GetModuleFileName(NULL, szApplicationPath, _countof(szApplicationPath));
    KGLOG_PROCESS_ERROR(nRetCode > 0);
    KGLOG_PROCESS_ERROR(nRetCode < _countof(szApplicationPath));

    pchLashSlash = _tcsrchr(szApplicationPath, _T('\\'));
    KGLOG_PROCESS_ERROR(pchLashSlash);
    *(pchLashSlash + 1) = _T('\0');

    nRetCode = _sntprintf(
        szConfigFileName, _countof(szConfigFileName), _T("%s%s"), szApplicationPath, _T("config.ini")
    );
    KGLOG_PROCESS_ERROR(nRetCode > 0);
    KGLOG_PROCESS_ERROR(nRetCode < _countof(szConfigFileName));

    nRetCode = _taccess(szConfigFileName, 0);
    KGLOG_PROCESS_ERROR(nRetCode == 0 && "config.ini not found.");

    CDialog::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_MainTab.Init();

    nResult = true;
Exit0:
    if (!nResult)
    {
        MessageBox(_T("�����ʼ��ʧ��!"));
        KGLogPrintf(KGLOG_ERR,	"�����ʼ��ʧ�ܣ��˳�����"); 
        EndDialog(FALSE);
    }	
    return TRUE;
}

void KGameConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR KGameConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL KGameConfigDlg::PreTranslateMessage(MSG* pMsg)     
{   
	if (pMsg->wParam == VK_RETURN)      
	{     
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg); 

}   

void KGameConfigDlg::OnBnClickedBtnOk()
{
	// TODO: Add your control notification handler code here
	m_MainTab.ConfigSave();
	OnBnClickedBtnQuit();
}

void KGameConfigDlg::OnBnClickedBtnQuit()
{
	// TODO: Add your control notification handler code here
	EndDialog(TRUE);
}

void KGameConfigDlg::OnBnClickedBtnSysInfo()
{
	//::WinExec("msinfo32.exe", NULL);
	//m_InfoPage.Init();
}
void KGameConfigDlg::OnBnClickedBtnApply()
{
	// TODO: Add your control notification handler code here
	m_MainTab.ConfigSave();
}
