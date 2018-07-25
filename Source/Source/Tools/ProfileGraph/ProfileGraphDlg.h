// ProfileGraphDlg.h : ͷ�ļ�
//

#pragma once


#include  "afxcmn.h"
#include  "NSChartCtrl.h"
#include  "ProfileGraphDocument.h"


// CProfileGraphDlg �Ի���
class CProfileGraphDlg : public CDialog
{
// ����
public:
	CProfileGraphDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CProfileGraphDlg();

// �Ի�������
	enum { IDD = IDD_PROFILEGRAPH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnTable();

	afx_msg void OnTvnSelchangedGraphcall(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedGraphcounter(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()



protected:
	////////////////////////////////////////////////
	// fill system info
	void  fillGraphSysInfo();


	////////////////////////////////////////////////
	// fill tree / list ctrl
	void  fillGraphCallTree();
	void  fillGraphCallTree( HTREEITEM hParent, CProfileGraphNode* pNode );

	void  fillGraphChart( HTREEITEM hTreeSel );
	void  fillGraphLine( HTREEITEM hTreeSel );

	void  fillGraphList( HTREEITEM hTreeSel );
	void  fillGraphList( char const* name, CProfileGraphNode* pChild, CProfileGraphNode* pNode );



protected:
	////////////////////////////////////////////////
	// member
	CProfileGraphDocument	m_GraphDoc;
	CNSChartCtrl			m_GraphChart;
	CStatic					m_GraphLine;
	CEdit					m_GraphSysInfo;

	
	CTreeCtrl m_treeGraphCall;
	CListCtrl m_lstGraphCounter;


	////////////////////////////////////////////////
	// bitmap member
	CDC		  m_CompatibleDC;
	CBitmap	  m_CompatibleBitmap;

};
