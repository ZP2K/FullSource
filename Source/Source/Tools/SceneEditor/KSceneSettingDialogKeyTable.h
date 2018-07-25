////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSettingDialogKeyTable.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-2-9 9:58:57
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#ifndef _INCLUDE_KSCENESETTINGDIALOGKEYTABLE_H_
#define _INCLUDE_KSCENESETTINGDIALOGKEYTABLE_H_
#include "IEKG3DEnvironment.h"
////////////////////////////////////////////////////////////////////////////////



// KSceneSettingDialogKeyTable dialog
/*
interface IEKG3DEnvEffect;
interface IEKG3DEnvEffDWGC_KeyTable0;
class KSceneSettingDialogKeyTable : public CDialog
{
	DECLARE_DYNAMIC(KSceneSettingDialogKeyTable)

public:
	KSceneSettingDialogKeyTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~KSceneSettingDialogKeyTable();

// Dialog Data
	enum { IDD = IDD_SCENESETTING_DWGC_KEY_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	///�����Ҫ������ģ̬�Ի�������Ҫ���ǵ�����������
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBnDwgcKeyAdd();
	afx_msg void OnBnClickedBnDwgcKeyRemove();

	
public:
	HRESULT InitKeyTable(KG3DENVEFF_HANDLE EffHandle, HWND PropertySheetHandle);
private:
	VOID ReloadKeyTable();
	int GetCurSelData();
	//CDialog* GreatKeyEditDialog();
private:
	HWND m_PropertySheetHWnd;
	
	//IEKG3DEnvEffect* m_pEffect;
	KG3DENVEFF_HANDLE m_EnvEffHandle;
private:
	CString m_csCaption;
public:
	CListBox m_ListBox;
	afx_msg void OnBnClickedBnDwgcKeyMod();
	afx_msg void OnBnClickedBnDwgcKeySyn();
	afx_msg void OnLbnDblclkListDwgcKeyList();
};

*/

#endif //_INCLUDE_KSCENESETTINGDIALOGKEYTABLE_H_
