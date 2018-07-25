////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSettingPageLensFlare.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-7-24 15:44:19
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KSCENESETTINGPAGELENSFLARE_H_
#define _INCLUDE_KSCENESETTINGPAGELENSFLARE_H_
#include "KGCommonBuffer.h"
#include "KSceneSettingPageBase.h"
////////////////////////////////////////////////////////////////////////////////
#pragma once

class IEKG3DMeshLensFlare;
typedef	struct _IEKG3DMeshLensFlareDescStruct	IEKG3DMeshLensFlareDescStruct;
struct LensFlareInfo;
class KSceneSettingPageLensFlare : public KSceneSettingPageBase
{
	DECLARE_DYNAMIC(KSceneSettingPageLensFlare)
private:
	enum{even_id = 1,};

	//ֻ�����ڽ��潻��
	CTreeCtrl   m_tree;
	CString     m_csText;
	int         m_nTexCellNumX;
	int         m_nTexCellNumY;

	FLOAT       m_fSize;
	FLOAT       m_fHeight;
	float		m_fVisibility;
	//BOOL		m_bDrawDummy;
	float		m_fBoundAspect;
	float		m_fDummySize;

	//�ڲ�����
	D3DVIEWPORT9            m_viewPort;

	KG_CUSTOM_HELPERS::TCommonParamBuffer<IEKG3DMeshLensFlareDescStruct, 288>	m_pDescStruct;///�������һ������ǰ�������Ϳ��Ե�ָ��ʹ�õ��࣬�ڲ����Զ�����ʵ��͹���Buffer������Ҫ��cpp����ͷ�ļ�
	KG_CUSTOM_HELPERS::TCommonParamBuffer<LensFlareInfo, 20>	m_pLensInfo;

	INT m_nWhichLensFlare;///��־��ǰ��LensFlare���ĸ�
	UINT_PTR m_uTimmer;
	CRect	m_CurSelectRect;

	size_t	m_uWhichItemShouldBeSelected;	//SetItem֮��Ҫ����ѡ����Ϊ��Height֮��Lens��λ�ÿ��ܻ��
	BOOL	m_bItemSelectionPosChanged;
private:
	IEKG3DMeshLensFlare* GetLensFlareInterface();
	void DrawTex();	//���ұߵ�ѡ���
public:
	KSceneSettingPageLensFlare();
	virtual ~KSceneSettingPageLensFlare();
	enum { IDD = IDD_SCENESETTING_LENSFLARE };

	HRESULT SynFromInnerToInterface(DWORD_PTR dwOption = 0);
	HRESULT SynFromInterfaceToInner(DWORD_PTR dwOption = 0);

    int UpdateSelectionFrame(INT x, INT y);//����ѡ�������Ⱦ��ѡ�����
    BOOL UpdateTree();///����OnSelChange�����SynFromInnerToInterface
private:
	void SynFromInnerToInterface_LensFlareInfo();
protected :
	virtual void DoDataExchange(CDataExchange* pDX);	
	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
    afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR uIDEvent);
    afx_msg void OnBnClickedTexture();
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedDel();
    afx_msg void OnBnClickedDefult();
	//afx_msg void OnBnClickedSsBnUpdate();
	//afx_msg void OnBnClickedSsBnCancel();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnBnClickedSsSwitch();

	afx_msg void OnBnClickedSsState1();
	afx_msg void OnBnClickedReloadShader();
};


#endif //_INCLUDE_KSCENESETTINGPAGELENSFLARE_H_
