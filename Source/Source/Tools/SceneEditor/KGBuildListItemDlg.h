#pragma once


// KGBuildListItemDlg �Ի���

class KGBuildListItemDlg : public CDialog
{
	DECLARE_DYNAMIC(KGBuildListItemDlg)

public:
	KGBuildListItemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~KGBuildListItemDlg();

// �Ի�������
	enum { IDD = IDD_BUILDITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
