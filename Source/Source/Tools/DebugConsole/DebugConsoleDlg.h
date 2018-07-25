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
// DebugConsoleDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define WM_EDIT_RECEIVE_STRING WM_USER + 100

#define BUFFER_SIZE 4096

// CDebugConsoleDlg �Ի���
class CDebugConsoleDlg : public CDialog
{
// ����
public:
	CDebugConsoleDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DEBUGCONSOLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

    HANDLE m_hMapFile;

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
    CEdit m_editInfo;
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnBnClickedButtonAbout();
public:
    afx_msg void OnBnClickedButtonClear();
public:
    virtual BOOL DestroyWindow();
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
public:
    CButton m_buttonClear;
public:
    CButton m_buttonAbout;
    CButton m_buttonSave;
public:
    afx_msg void OnBnClickedButtonSaveToFile();
};
