// GameConfigDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"

#include "MainTabCtrl.h"
#include "KClientSetting.h"
#include "KClientEnvironment.h"


// KGameConfigDlg �Ի���
class KGameConfigDlg : public CDialog
{
// ����
public:
	KGameConfigDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GAMECONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	KMainTabCtrl m_MainTab;
	//KClientEnvironment m_InfoPage;
	//KClientSetting  m_ConfigPage;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnQuit();
	afx_msg void OnBnClickedBtnSysInfo();
	afx_msg void OnBnClickedBtnApply();
};
