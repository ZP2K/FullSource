#pragma once


// KGValueEditItemTypeIndexNumDlg �Ի���

class KGValueEditItemTypeIndexNumDlg : public CDialog
{
	DECLARE_DYNAMIC(KGValueEditItemTypeIndexNumDlg)

public:
	KGValueEditItemTypeIndexNumDlg(CWnd* pParent, int nItem);   // ��׼���캯��
	virtual ~KGValueEditItemTypeIndexNumDlg();

// �Ի�������
	enum { IDD = IDD_ITEM_TYPE_INDEX_NUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
