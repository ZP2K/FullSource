#pragma once

#include "IEKG3DScene.h"
#include "IEEditor.h"
#include "KSceneSettingPropertySheet.h"

class KSceneSettingPageBase;
class KSceneSceneEdit;

class KSceneSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(KSceneSettingDialog)

public:
	KSceneSettingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~KSceneSettingDialog();

// Dialog Data
	enum { IDD = IDD_SCENESETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    enum SCENESETTINGTYPE
    {
        SETTING_LOW,
        SETTING_ADVANCED
    };

protected:
	KSceneSettingPropertySheet	m_PropertySheet;
	std::vector<KSceneSettingPageBase*>	m_vecPages;

public:
	virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void OnCancel();
	virtual void OnOK();
private:
	bool m_bIsMyselfModal;
public:
	virtual INT_PTR DoModal();///<CTH>��������������򵥵�����һ����־�Ϳ��Ժ����׵��ж�������������ǲ���һ��ģ̬�Ի�����
	///Ȼ��Ϳ�����OnOk��OnCancel������������־��������,�еĵط����Dialog��ģ̬�ġ�
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedDiscard();
    afx_msg void OnBnClickedRadioSettinglow();
    afx_msg void OnBnClickedRadioSettingadvanced();
};
