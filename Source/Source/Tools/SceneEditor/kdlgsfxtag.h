#pragma once
#include "afxwin.h"
#include "IEKG3DAnimationTag.h"

// KDlgSfxTag dialog

class KSceneModelEditorAnimationPanel;
class KDlgSfxTag : public CDialog
{
	DECLARE_DYNAMIC(KDlgSfxTag)

public:
	KDlgSfxTag(IEKG3DAnimationSFXTag* pSfxTag,CWnd* pParent = NULL);   // standard constructor
	virtual ~KDlgSfxTag();

	// Dialog Data
	enum { IDD = IDD_MODELEDITOR_SFX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	// �Ƿ�Ҫ���ݶ������ŵ��ٶ����޸�ʵ�ʲ��ŵ��ٶ�
	BOOL m_bAdaptSpeedToMotion;
	// ��Ч�Ƿ�ᱻ��һ����Чȡ��
	BOOL m_bInterruptable;
	// �Ƿ�֪ͨ�����߼�
	BOOL m_bNotify;
protected:
	// �������ʱ�Ĵ���
	BOOL m_bMotionFinishEvent;
	// �����ٶ� 
	float m_fSpeed;
	// ��������
	BOOL m_bSetWorldPosition;
	IEKG3DAnimationSFXTag *m_pSfxTag;
	KSceneModelEditorAnimationPanel *m_pAnimationPanel;
	UINT m_uFrame;
public:
	virtual BOOL OnInitDialog();
protected:
	void FillComFinishState();
	void FillControlData();
	void FillComTagName();
	void SetComTagNameCurSel(int nCurSel);
	void SaveState();
	CComboBox m_ComTagName;
	CString m_csTagName;
	CString m_csSfxName;
	CComboBox m_ComFinishState;
	static  UINT m_uFrameCopy;
	int m_LastCursel;
public:
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSfxselect();
	void SetFrame(UINT uFrame);
	void SetAnimationPanel(KSceneModelEditorAnimationPanel *pAnimationPanel);
	afx_msg void OnCbnDropdownComboTagname();
	afx_msg void OnCbnSelchangeComboTagname();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangeEditTagname();
};
