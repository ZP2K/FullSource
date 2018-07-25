////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSceneTimeLineGridCtrl.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-3-12 14:46:52
//  Comment     : CGridCtrl�ľ���˵����ο�http://www.codeproject.com/miscctrl/gridctrl.asp
// �Ѿ���ϸ���������License��ʽ��ȫFree������ʹ����ȫ�Ϸ���
// CGridCtrl�ؼ���ʱֻʹ���ڱ༭���У���δ����Base��֮�࣬���Գ��˱༭������Ĺ����������ʹ��
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _INCLUDE_KSCENESCENETIMELINEGRIDCTRL_H_
#define _INCLUDE_KSCENESCENETIMELINEGRIDCTRL_H_
#include "ThirdPartControls\CGridCtrl\GridCtrl.h"
////////////////////////////////////////////////////////////////////////////////

//#include "KSceneCallbackFrameWork.h"
class KSceneSceneGridCtrl;
struct KSceneSceneGridCtrlEventHandler
{
	virtual BOOL IsEditStringValid(KSceneSceneGridCtrl& ctrl, CPoint point, LPCTSTR pStrEdit)	{return TRUE;}
	virtual BOOL IsEditCharValid(KSceneSceneGridCtrl& ctrl, CPoint point, UINT uChar)	{return TRUE;}
	virtual BOOL IsLButtomDoubleClickValid(KSceneSceneGridCtrl& ctrl, CPoint point)	{return TRUE;}
	virtual BOOL IsLButtonDownValid(KSceneSceneGridCtrl& ctrl, CPoint point)	{return TRUE;}
	virtual BOOL IsCopyValid(KSceneSceneGridCtrl& ctrl, CPoint point)	{return TRUE;}
	virtual BOOL IsCutValid(KSceneSceneGridCtrl& ctrl, CPoint point)	{return TRUE;}
	virtual BOOL IsPasteValid(KSceneSceneGridCtrl& ctrl, CPoint point)	{return TRUE;}
	virtual ~KSceneSceneGridCtrlEventHandler() = 0{}
};

class KSceneSceneGridCtrl : public CGridCtrl
{
	DECLARE_DYNCREATE(KSceneSceneGridCtrl)
public:
	struct InitAppearanceStruct 
	{
		INT m_nRowCount;
		INT m_nColumnCount;
		INT m_nFixedRowCount;
		INT m_nFixedColumnCount;
		INT m_nDefColumnWidth;
		INT m_nFixedColumnWidth;
		INT m_nDefRowHeight;
		bool m_bEditEnable;
		bool m_bInitBasicTitles;
		bool m_bEnableHightLighFixed;
		bool m_bSingleRowSelection;
		bool m_bSingleColomnSelection;
		bool m_bEnableRowResize;
		bool m_bEnableColResize;				
		CString m_csCornerDescription;
		InitAppearanceStruct();
		BOOL IsValid();
	};

	//typedef KSUIHELPER::CWndCallbackFrameWork<KSceneSceneGridCtrl, POINT, 8> TypeCallbackFrameWork;
	//typedef TypeCallbackFrameWork::TypeFuncCallback TypeFuncCallback;
	//enum
	//{
	//	CALLBACK_VALIDATE_EDIT = 0,	//ObjParam = LPCTSTR Ҫ�޸ĵ��ַ���
	//	CALLBACK_ON_EDIT = 1,		//ObjParam = Pointer To StructOnEdit
	//	CALLBACK_DL_CLICKED = 2,	//ObjParam = Null
	//	CALLBACK_L_DOWN = 3,		//ObjParam = NULL
	//	CALLBACK_COPY = 4,
	//	CALLBACK_CUT = 5,
	//	CALLBACK_PASTE = 6,
	//};
	struct StructOnEdit 
	{
		CPoint m_CPoint;
		UINT	m_uChar;
	};			

public:
	BOOL InitAppearance(InitAppearanceStruct* pStruct);
	BOOL AddRow(LPCTSTR tczHead, BOOL bSetHeadData, LPARAM lparamHead, INT nRow = -2);///nRowȡĬ��ֵ�Զ��õ���ǰѡ���,-1����ӵ�β��
	BOOL AddRow(INT nRow = -2);///nRowȡĬ��ֵ�Զ��õ���ǰѡ���,-1����ӵ�β��
	BOOL DelRow(INT nRow = -2);
	BOOL MoveRow(BOOL bUpOrDown, INT nRow = -2);
	BOOL UpRow(INT nRow = -2);
	BOOL DownRow(INT nRow = -2);
	BOOL ClearRow(INT nRow = -2, BOOL ClearFixed = FALSE);
	HRESULT GetSelectedMinRowHeadData(LPARAM* pData);
	INT GetSelectedMinRow();
	void MarkCell(INT nRow, INT nColumn, BOOL bEnable = TRUE);
	void HighLightCell(INT nRow, INT nColumn, BOOL bEnable = TRUE);
	void HighLightRow(INT nRow, BOOL bHighLight = TRUE);
	void UnHighLightAllRow();
	void UnHighLightAllColumn();
	void HighLightColumn(INT nColumn, BOOL bHighLight = TRUE);

	//<ScrollBar>
	BOOL BindHScrollBar(CScrollBar* pScrollBar);
	//<ScrollBar>

	INT FindRowWhichsDataEqualsGiven(LPARAM lData
		, INT nBegin = -1
		, BOOL FindFixed = FALSE
		, INT nWhichColumn = 0
		, INT nWhichFinded = 1);

	HRESULT SetEventHandler(KSceneSceneGridCtrlEventHandler* pEventHandler);	//ע���������ڣ������Grid��������HandlerҪ�������������ΪNULL

	//<�ı������Ϊ>
	virtual void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
	virtual BOOL ValidateEdit(int nRow, int nCol, LPCTSTR str);
	virtual void OnEditPaste();
	virtual void OnEditCopy();
	virtual void OnEditCut();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//</�ı������Ϊ>
protected:
	inline COLORREF GetMarkColor(){return RGB(200, 200, 200);}
	inline COLORREF GetHighLightColor(){return RGB(180, 180, 180);}
	inline COLORREF GetHighLightAndMarkColor(){return RGB(150, 150, 150);}

	//<ScrollBar>
	virtual void ResetScrollBars();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	bool IsUseExternalScrollBar(){return m_dwState & emUseExternalScrollBar ? true : false;}
	void UseExternalScrollBar(BOOL bUse){bUse ? m_dwState & emUseExternalScrollBar : m_dwState & (~emUseExternalScrollBar);}
	//</ScrollBar>

	//InitAppearanceStruct m_InitApperaceStruct;

private:
	INT m_nLastHighLightRow;
	INT m_nLastHighLightColumn;
	bool m_bEnableHighLightFixed;
	enum
	{
		emUseExternalScrollBar = 0x01,
	};
	DWORD m_dwState;

	CScrollBar* m_pScrollBarHorz;//<ScrollBar>

	KSceneSceneGridCtrlEventHandler* m_pEventHandler;
public:
	KSceneSceneGridCtrl();
	~KSceneSceneGridCtrl();
	DECLARE_MESSAGE_MAP()
};


#endif //_INCLUDE_KSCENESCENETIMELINEGRIDCTRL_H_
