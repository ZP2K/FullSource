////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSettingPageRegionSplit.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-10 9:48:44
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _INCLUDE_KSCENESETTINGPAGEREGIONSPLIT_H_
#define _INCLUDE_KSCENESETTINGPAGEREGIONSPLIT_H_
#include "afxcmn.h"
#include "KSceneCustomTreeCtrl.h"
#include "KGCommonBuffer.h"
//#include "KGCommonList.h"
#include "KSceneCustomListContrl.h"
#include "KSceneSettingPageBase.h"
#include "afxwin.h"
#include <set>
// KSceneSettingPageRegionSplit dialog
interface IEKG3DRegionInfoManager;
typedef struct _KG3DREGION_INFORMER_LOADFILE_PARAM KG3DREGION_INFORMER_LOADFILE_PARAM;


class KSceneSettingPageRegionSplit : public KSceneSettingPageBase, public KSceneSceneTreeCtrlEventHandler
{
	DECLARE_DYNAMIC(KSceneSettingPageRegionSplit)

public:
	KSceneSettingPageRegionSplit();
	virtual ~KSceneSettingPageRegionSplit();

// Dialog Data
	enum { IDD = IDD_SCENESETTING_PAGEREGIONSPLIT };

#if defined(DEBUG) | defined(_DEBUG)
private:
	INT m_nFateCode;///����ָ�뾲̬ת����ʱ����Ϸ���
	INT IsValidInMemory(){return m_nFateCode == 0xFA7EC0DE;}
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int m_nTexID;
	int m_nOutputWndID;
	BOOL m_bShowRegionMap;
	BOOL m_bNeedShowRegionMap;

	//////////////////////////////////////////////////////////////////////////
	CButton m_ckShowRegionMap;
	CStatic m_stOutputWnd;

	

	struct TypeRIMGetter
	{
		DWORD_PTR m_WParam;

		IEKG3DRegionInfoManager* GetContainer();
	};

	typedef TypeRIMGetter TypeContainerGetter;
	typedef KSH::COMMON_CTRL::GetElemOnceAll<IEKG3DRegionInfoManager, DWORD> TypeElemGetter;
	typedef KSH::COMMON_CTRL::GetNameByValue<IEKG3DRegionInfoManager, DWORD> TypeNameGetter;

	typedef KSH::COMMON_CTRL::KSceneCustomListContrl<IEKG3DRegionInfoManager, DWORD
		,TypeContainerGetter, TypeElemGetter, TypeNameGetter
	>		TypeListControl;
	TypeListControl m_RegionList;	//�Զ��͵�List��ȡ��

	

	//KSH::COMMON_LIST::KGCommonList m_RegionList;	//�Զ��͵�List��ȡ��

	typedef KG_CUSTOM_HELPERS::TCommonParamBuffer<KG3DREGION_INFORMER_LOADFILE_PARAM, 69> TypeRIMLoadParamPtr;
	TypeRIMLoadParamPtr	m_pLoadParam;//���ڴ�ŷ��ص�Loadparam������

	KSceneSceneTreeCtrl m_Tree;
	CEdit m_edTexPath;
	
	DWORD	m_dwTimeHandle;	//����Ҫ���������RegionIndex�����Ͻǵľ��

	//���ؼ�ֻ�ܷ�һ��DWORD_PTR��ΪData�������Dlg�У����ؼ��е�Data��Ϊ�����Ȼ���������Container�ҵ���Ӧ������
	struct TypeRIMExtraParams 
	{
		bool m_bHasSaved;
		bool m_bLoadParamValid;
		TypeRIMLoadParamPtr	m_pLoadParam;
		std::tstring	m_Path;	//���û������Ļ�����û��Reload
		std::tstring	m_NameForRepresent;	//�ֿ�ͼ����
		FILETIME		m_LastWriteTime;	//�����޸�ʱ�䣬�����Զ�����
	};
	typedef std::map<DWORD, TypeRIMExtraParams>	TypeRIMExtraParamContainer;
	TypeRIMExtraParamContainer	m_ParamContainer;	//�����������֮��
public:
	
	int m_nFlag;
	int Update();
private:
	VOID BindCallbacks(BOOL bBind);
	//static DWORD_PTR CALLBACK FuncTreeSelChangeCallback(KSceneSceneTreeCtrl* pTree, HTREEITEM hItem, DWORD_PTR dwTreeParam, DWORD_PTR dwCustomParam);
	virtual BOOL OnItemSelectChange(KSceneSceneTreeCtrl& tree, HTREEITEM hItemInserted, HTREEITEM hItemLastSelect);
	
	BOOL AddOutputTexWnd(INT nTexID);
	VOID RemoveOutputTexWnd();
	BOOL ReloadRegionMap(LPCTSTR lpPath, KG3DREGION_INFORMER_LOADFILE_PARAM* pParam);
	IEKG3DRegionInfoManager* GetCurRIM(DWORD_PTR* pdwHandle = NULL, HTREEITEM* pTreeItem = NULL);

	KG3DREGION_INFORMER_LOADFILE_PARAM* GetLoadParam(IEKG3DRegionInfoManager* pRIM);
	CString	GetLoadPath();
	void GetMapName( std::tstring& csItemName );//�õ��ֿ�ͼ����

	bool PromoteSave();
	bool IsRIMSaved(DWORD dwHandle);
	BOOL EnableOuputRegionIndex(BOOL bEnable);
	BOOL EnableShowMask(BOOL bEnable);

	HTREEITEM AddAnRIMToTree(DWORD dwHandle, LPCTSTR pName, LPCTSTR pPath, KG3DREGION_INFORMER_LOADFILE_PARAM* pLoadParam);//������һ������Ϊ���򲻸ı�ԭ����Param
	HRESULT AddRIM(const CString csLoadPath, KG3DREGION_INFORMER_LOADFILE_PARAM* pLoadParam, LPCTSTR lpName, DWORD* pdwHandle );
	HRESULT DelRIM();

	BOOL	IsFileChanged(LPCTSTR lpPath, FILETIME OldFileTime);
	BOOL	GetFileTime(LPCTSTR lpPath, LPFILETIME lpFileTime);
public:
	virtual HRESULT	SynFromInterfaceToInner(DWORD_PTR dwOption = 0);
	virtual HRESULT	SynFromInnerToInterface(DWORD_PTR dwOption = 0);
	HRESULT ShowRegionMap(BOOL bShow = TRUE);	
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedSsListBnAdd();
	//afx_msg void OnBnClickedSsBnReadRegionSplitMap();
	afx_msg void OnBnClickedCkShowRegionSplitMap();
	afx_msg void OnBnClickedSsBnReadRegionSplitMapAgain();
	afx_msg void OnBnClickedSsListBnDel();	
	afx_msg void OnBnClickedSsListBnSave();
	afx_msg void OnBnClickedSsListBnRename();
	afx_msg void OnBnClickedCkShowRegionIndex();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCkShowMask();
private:
	unsigned int m_uRegionCount;
	unsigned int m_uWidth;
	unsigned int m_uHeight;
	unsigned int m_uPixelPerRegion;
	unsigned int m_uMemorySize;
	
};
#endif //_INCLUDE_KSCENESETTINGPAGEREGIONSPLIT_H_
