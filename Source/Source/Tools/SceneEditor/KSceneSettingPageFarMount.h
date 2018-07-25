////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSettingPageFarMount.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-10-28 11:34:17
//  Comment     : Զɽ����Ĺ�����棬��ӦIDD_SCENESETTING_FARMOUNT
//                ��KSceneSettingDialog��һ����ҳ��
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _INCLUDE_KSCENESETTINGPAGEFARMOUNT_H_
#define _INCLUDE_KSCENESETTINGPAGEFARMOUNT_H_

////////////////////////////////////////////////////////////////////////////////
#include "IEKG3DScene.h"
#include "IEKG3DMeshFarMountain.h"
//#include "afxwin.h"
#include "KColorDialog.h"
#include "KSceneSettingPageBase.h"
//#include "afxcmn.h"
//#include "KSceneSettingPageKeyEditBase.h"
// KSceneSettingPageFarMount dialog

class KSceneSettingPageFarMount : public KSceneSettingPageBase//: public KSceneSettingPageKeyEditBase
{
	DECLARE_DYNAMIC(KSceneSettingPageFarMount)

public:
	KSceneSettingPageFarMount();
	virtual	~KSceneSettingPageFarMount();
	// Dialog Data
	enum { IDD = IDD_SCENESETTING_FARMOUNT };
public:
	int m_nFlag;///�����Ƿ���Ⱦ
	
	int		Update();
	virtual	BOOL OnSetActive();
private:
	///���淽�����ڻ�������潻��
	virtual HRESULT	SynFromInterfaceToInner(DWORD_PTR dwOption = 0 );///��������ȽϽ��沢�Լ�����ModificationOption
	virtual HRESULT	SynFromInnerToInterface(DWORD_PTR dwOption = 0);
private:
	HRESULT	RestoreDefaultInterface();
	HRESULT	UpdateTree();
	KG3DMESHFARMOUNTAIN_DESCSTRUCT	m_DlgStruct;///���ڱ������
	//TCHAR	m_TexPathBuffer[MAX_PATH];///��ͼ��ȫ·����Ӧ��ͨ��m_DlgStruct����
	int		GetSeletedFarMountIndex( IEKG3DMeshFarMountain* pFarMount = NULL );///�������������ڵõ���ǰ��Tree��ѡ���FarMount���Ƿ���Ч,�������ָ��Ļ����Ϳ��Լ��ָ���Ƿ���Ч
	IEKG3DMeshFarMountain*	GetValidFarMountPointer( IEKG3DMeshFarMountain* pFarMount = NULL );///����ָ���Ƿ���Ч��Ĭ�ϴ�Tree����
	//VOID	CheckFogColorSyn(IEKG3DMeshFarMountain* pFarMount);
public:
	HRESULT	UpdateTexture();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// ��ͼ����ȫ·��
	CString m_csTexPath;
	// ����뾶
	FLOAT m_fRadius;
	// ����߶�
	FLOAT m_fHeight;
	// �����U�ظ�����
	FLOAT m_fTexU;
	// ����ײ��߶�
	FLOAT m_fBotmZPos;
	// ����Z��ת��
	FLOAT m_fZRotation;
	// �ײ�����߶�
	FLOAT m_fBotmBlendHeight;
	// ������ɫ
	//KColorPickerButton m_bnColor;
	// ���ں�����,0��1
	FLOAT m_fFogBlendFactor;
	// ���ں���ɫ
	KColorPickerButton m_bnFogColor;

    FLOAT m_fTexFactor;

	CTreeCtrl m_Tree;
	CButton m_ckSynWithFog;
	CEdit m_edTexPath;
public:
	afx_msg void OnBnClickedSsFarmountBnAdd();
	afx_msg void OnBnClickedSsFarmountBnDel();
	afx_msg void OnBnClickedSsFarmountBnUp();
	afx_msg void OnBnClickedSsFarmountBnDown();
	afx_msg void OnTvnSelchangedSsFarmountTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSsFarmountBnTexpath();
	afx_msg void OnBnClickedSsFarmountBnUpdatemesh();
	afx_msg void OnBnClickedSsFarmountBnUpdateblend();
	afx_msg void OnBnClickedCkEnableColor();
	afx_msg void OnBnClickedSsBnUpdateother();
	//afx_msg void OnBnClickedSurfaceColor();
};


#endif //_INCLUDE_KSCENESETTINGPAGEFARMOUNT_H_
