#pragma once


// KGValueEditGetValueDlg �Ի���

class KGValueEditGetValueDlg : public CDialog
{
	DECLARE_DYNAMIC(KGValueEditGetValueDlg)

public:
	KGValueEditGetValueDlg(CWnd* pParent, CString szTitle, int nValueSize);   // ��׼���캯��
	virtual ~KGValueEditGetValueDlg();
	enum { IDD = IDD_VALUE_EDIT_GET_VALUE };
	CString GetValue();
	virtual BOOL OnInitDialog();
public:
	int m_nValueSize;
	CString m_szValue;
	CString m_szTitle;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
