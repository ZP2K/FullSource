// ***************************************************************
//
//  Description: 
//  -------------------------------------------------------------
//	Author: Zeb
//  -------------------------------------------------------------
//	Create Date: 2006-04-25
//	-------------------------------------------------------------
//	History: 
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
//
// ***************************************************************
// DebugConsoleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DebugConsole.h"
#include "DebugConsoleDlg.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDebugConsoleDlg �Ի���




CDebugConsoleDlg::CDebugConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugConsoleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hMapFile = NULL;
}

void CDebugConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INFO, m_editInfo);
    DDX_Control(pDX, IDC_BUTTON_CLEAR, m_buttonClear);
    DDX_Control(pDX, IDC_BUTTON_ABOUT, m_buttonAbout);
    DDX_Control(pDX, IDC_BUTTON_SAVE_TO_FILE, m_buttonSave);
}

BEGIN_MESSAGE_MAP(CDebugConsoleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CDebugConsoleDlg::OnBnClickedButtonAbout)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDebugConsoleDlg::OnBnClickedButtonClear)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_FILE, &CDebugConsoleDlg::OnBnClickedButtonSaveToFile)
END_MESSAGE_MAP()


// CDebugConsoleDlg ��Ϣ�������

BOOL CDebugConsoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

    m_hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security 
        PAGE_READWRITE,          // read/write access
        0,                       // max. object size 
        BUFFER_SIZE,                // buffer size  
        _T("DebugConsole"));                 // name of mapping object

    if (m_hMapFile == NULL || m_hMapFile == INVALID_HANDLE_VALUE) 
        return FALSE;

    if (m_hMapFile && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(m_hMapFile); 
        m_hMapFile = NULL; 
        return FALSE;
    } 

    LPTSTR pBuf;
    pBuf = (LPTSTR) MapViewOfFile(m_hMapFile,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,                   
        0,                   
        BUFFER_SIZE);           

    if (pBuf == NULL) 
        return FALSE;

    pBuf[0] = '\0';

    UnmapViewOfFile(pBuf);

    m_editInfo.SetLimitText(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDebugConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDebugConsoleDlg::OnPaint()
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
HCURSOR CDebugConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDebugConsoleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    switch (message)
    {
    case WM_EDIT_RECEIVE_STRING:
        {
            LPSTR pBuf;
            pBuf = (LPSTR)MapViewOfFile(m_hMapFile, 
                FILE_MAP_READ,
                0,                    
                0,                    
                BUFFER_SIZE);                   

            if (pBuf == NULL) 
                break;

            char szBuffer[BUFFER_SIZE];
            _snprintf_s(szBuffer, BUFFER_SIZE, BUFFER_SIZE, "%s", pBuf);
            //CString info;
            //m_editInfo.GetWindowText(info);
            //info += CString(szBuffer);
            //m_editInfo.SetWindowText(info);
            m_editInfo.SetSel(-1, -1);
            m_editInfo.ReplaceSel(szBuffer);
            UnmapViewOfFile(pBuf);
            //m_editInfo.SetSel(info.GetLength(), info.GetLength());
        }
        break;
    default:
        break;
    };

    return CDialog::WindowProc(message, wParam, lParam);
}

void CDebugConsoleDlg::OnBnClickedButtonAbout()
{
    // TODO: Add your control notification handler code here
    CAboutDlg dlg;
    dlg.DoModal();
}

void CDebugConsoleDlg::OnBnClickedButtonClear()
{
    // TODO: Add your control notification handler code here
    m_editInfo.SetWindowText(_T(""));
}

BOOL CDebugConsoleDlg::DestroyWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    if (m_hMapFile)
        CloseHandle(m_hMapFile);

    return CDialog::DestroyWindow();
}

void CDebugConsoleDlg::OnSize(UINT nType, int cx, int cy)
{
    if (cy < 200 || cx < 200)
        return;

    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if (!m_editInfo.m_hWnd || !m_buttonClear.m_hWnd || !m_buttonAbout.m_hWnd)
        return;

    RECT rc;
    GetClientRect(&rc);

    RECT r = rc;
    r.left += 10;
    r.right -= 10;
    r.top += 10;
    r.bottom -= 30;
    m_editInfo.MoveWindow(&r);

    r.right = r.left + 70;
    r.top = r.bottom + 5;
    r.bottom = r.top + 20;
    m_buttonClear.MoveWindow(&r);

    r.left = r.right + 10;
    r.right = r.left + 70;
    m_buttonSave.MoveWindow(&r);

    r.right = rc.right - 10;
    r.left = r.right - 70;
    m_buttonAbout.MoveWindow(&r);
}

void CDebugConsoleDlg::OnBnClickedButtonSaveToFile()
{
    // TODO: Add your control notification handler code here
    TCHAR szFilters[] = _T("All Files (*.*)|*.*||");

    CFileDialog fileDlg(
        FALSE, 
        NULL,
        NULL,
        OFN_EXPLORER|OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, 
        szFilters,
        this
        );

    if (fileDlg.DoModal() == IDOK)
    {
        CString str = fileDlg.GetPathName();
        FILE *fp = NULL;
        _tfopen_s(&fp, str, _T("wt"));
        if (!fp)
            return;

        CString info;
        m_editInfo.GetWindowText(str);
        _ftprintf(fp, _T("%s"), str.GetBuffer(0));
        fclose(fp);
        return;
    }

}
