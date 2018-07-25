// MaterialUniformToolsDlg.h : ͷ�ļ�
//

#pragma once
#include "IEKG3DMaterial.h"
#include "KG3DMaterialSetter.h"
#include "afxcmn.h"
#include "KMaterialUniformToolAlphaOperation.h"

// CMaterialUniformToolsDlg �Ի���
class CMaterialUniformToolsDlg : public CDialog
{
// ����
public:
	CMaterialUniformToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MATERIALUNIFORMTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

 
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
public:
	CString m_strBaseDir;
public:
	afx_msg void OnBnClickedButtonAmbient();
public:
	afx_msg void OnBnClickedButtonSpecular();
public:
	afx_msg void OnBnClickedButtonDiffuse();
public:
	afx_msg void OnBnClickedButtonEmissive();
public:
	float m_fAlpha;
	COLORREF m_Colors[BUTTON_COUNT];
	CBitmap  m_BmpMaps[BUTTON_COUNT];
	CBitmapButton  m_Buttons[BUTTON_COUNT];

	void CreateBitmapForButton(DWORD Index);
	void UpdateColorButton(DWORD Index);
	void GetMaterialOption(MaterialOption& Option);
	void FindFilesInSubDir(LPCSTR strDirName, 
						   const MaterialOption& Option, 
		                   KG3DMaterialSetter& Setter);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonClear();
	void OnBnClickedButtonAlphaop();
	CListCtrl m_ctlResult;
	KMaterialUniformToolAlphaOperation m_dlgAlphaOperation;
	_MtlAlphaOperation m_AlphaOperation;
public:
	afx_msg void OnBnClickedButtonSelectfile();
	std::vector<std::string> m_vecSelectFiles;
};
