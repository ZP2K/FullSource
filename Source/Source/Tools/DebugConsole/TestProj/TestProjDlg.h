// TestProjDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define WM_EDIT_RECEIVE_STRING WM_USER + 100

// CTestProjDlg �Ի���
class CTestProjDlg : public CDialog
{
// ����
public:
	CTestProjDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTPROJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSend();
public:
    CEdit m_edit;
};
