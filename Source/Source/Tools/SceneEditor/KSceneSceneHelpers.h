////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSceneHelpers.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-2-13 10:26:00
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KSCENESCENEHELPERS_H_
#define _INCLUDE_KSCENESCENEHELPERS_H_

////////////////////////////////////////////////////////////////////////////////
typedef struct _KG3DCOBJ_HANDLE KG3DENVEFF_HANDLE;
namespace KSCENEEDITOR_HELPERCLASSES
{
	typedef struct _ENABLE_WINDOW_PARAM
	{
		BOOL m_bEnable;
		INT* m_npJumpList;
		INT	m_nIDCount;
		_ENABLE_WINDOW_PARAM()
		{
			ZeroMemory(this, sizeof(_ENABLE_WINDOW_PARAM));
		}
	}ENABLE_WINDOW_PARAM;
	BOOL LockChildWindows(HWND hWnd, BOOL bLock, INT* pJumpIDList, INT nIDCount);
	BOOL CALLBACK MyEnableWindow( HWND hWnd, LPARAM lPARAM);///�Զ���ص����������ڿؼ���������ֹ�ؼ�
	/*
	���溯�����ڴ�����һ��CheckButton�͵õ�һ��CheckButton״̬�����⡣
	�ֱ���CButton��CWnd�İ汾
	�軹�ǵõ���bToButtonOrToValue������
	����ڶ���������BOOL&,CheckButton�Ľ���������軹�ǵö���ͨ������������أ������۵ڶ���������ʲô�������ʱ��
	����Ĭ��ֵʹ�á����������ķ��غ͵ڶ��������ķ���һ�£����ڼ򻯱�̣���������һ��BOOL��ֱ��ʹ�÷���ֵ�Ϳ����ˣ�
	*/
	BOOL SynACButtonWithABool(CButton& cbButton, BOOL& bRetAndDefault, BOOL bToButtonOrToValue = TRUE);///����bValue
	BOOL SynACButtonWithABool(CButton& cbButton, unsigned uRetAndDefault, BOOL bToButtonOrToValue = TRUE);///����汾�ļ򻯰棬����TRUE
	BOOL SynACheckWndWithABool(CWnd* cbButton, BOOL& bRetAndDefault, BOOL bToButtonOrToValue = TRUE);
	BOOL SynACheckWndWithABool(CWnd* cbButton, unsigned uRetAndDefault, BOOL bToButtonOrToValue = TRUE);

	BOOL SynWndWithText(CWnd* pED, LPTSTR lpStr, BOOL bToWndOrToValue = TRUE, UINT uBufferSize = 0);
	BOOL SynWndWithText(CWnd* pED, CString& Cs, BOOL bToWndOrToValue = TRUE);

	template<typename _Type>
	BOOL ValueToWnd(_Type Value, CWnd* pED)
	{
		KG_PROCESS_ERROR(pED);
		{
			CString cs;
			KG_PROCESS_ERROR(KG_CUSTOM_HELPERS::TValueToString(Value, cs));
			KG_PROCESS_ERROR(SynWndWithText(pED, cs, TRUE));
		}
		return TRUE;
Exit0:
		return FALSE;
	}
	template<typename _Type>
	BOOL WndToValue(CWnd* pED, _Type& Value)
	{
		KG_PROCESS_ERROR(pED);
		{
			CString cs;
			KG_PROCESS_ERROR(SynWndWithText(pED, cs, FALSE));
			KG_PROCESS_ERROR(KG_CUSTOM_HELPERS::TStringToValue(cs, Value));	
		}
		return TRUE;
Exit0:
		return FALSE;
	}

	
	LPCTSTR GetRTTITypeName(DWORD dwType);
	CString GetRTTITypeName(const KG3DENVEFF_HANDLE& EnvEffHandle);
	
	void CWndDestroyer(CWnd* pCWnd);

	VOID CWndReSizeClient(CWnd* pCWnd, INT nNewWidth, INT nNewHeight, BOOL bNeedRePaint = FALSE);
	//VOID CWndMoveX(CWnd* pCWnd, INT nNewX);
	VOID CWndMoveY(CWnd* pCWnd, INT nNewY, BOOL bIsClientY = TRUE, CWnd* pOwnerWnd = NULL, BOOL bNeedRePaint = FALSE);
	INT CWndGetWindowWidth(CWnd* pCWnd);
	INT CWndGetWindowHeight(CWnd* pCWnd);
	CSize CWndGetWindowSize(CWnd* pCWnd);
	INT CTreeCtrlGetChildItemCount(CTreeCtrl& Tree, HTREEITEM hDes);
	VOID CTreeCtrlDeleteAllChildItems(CTreeCtrl& Tree, HTREEITEM hDes);
	BOOL CTreeCtrlCopyAllChildItems(CTreeCtrl& Tree, HTREEITEM hSrc, HTREEITEM hDec);
	HTREEITEM CTreeCtrlInsertItem(CTreeCtrl& Tree
								, CString Text
								, HTREEITEM hParent = TVI_ROOT
								, HTREEITEM hInsertAfter = TVI_LAST
								, LPARAM* plParam = NULL
								, INT nImage = -1
								, INT nSelectedImage = -1
								, UINT* puState = NULL
								, UINT  uStateMask = 0
								);
	BOOL CTreeCtrlIsHandleValid(CTreeCtrl& Tree,HTREEITEM hItem);	//hItem�Ǹ�����ʱ��᷵��FALSE����֪Ϊʲô������Ǹ����Ļ���CTreeCtrl���ڲ�ʵ�ֻỪ���ĵ������ο�TVI_ROOT�Ķ��壬Ҳ�Ǹ������������ڲ�����������;��
	
	struct CTreeCtrlVisitor 
	{
		virtual void Accept(CTreeCtrl& Tree, HTREEITEM hItem) = 0;
		virtual ~CTreeCtrlVisitor() = 0{};
	};
	enum
	{
		emDepthFirst = 0,
	};
	VOID CTreeCtrlTraverseDepthFirst(CTreeCtrl& Tree, CTreeCtrlVisitor& Visitor);
	inline VOID CTreeCtrlTraverse(CTreeCtrl& Tree, CTreeCtrlVisitor& Visitor, DWORD Stratege)
	{
		switch(Stratege)
		{
		case emDepthFirst:
			CTreeCtrlTraverseDepthFirst(Tree, Visitor);
			return;
			break;
		default:
		    break;
		}
		_ASSERTE(NULL);
	}
	VOID	CtreeCtrlSelectByOrder(CTreeCtrl& Tree, UINT Order);//ѡ��Tree�ĵ�N�����ڵ�
};
namespace KSH = KSCENEEDITOR_HELPERCLASSES;
#endif //_INCLUDE_KSCENESCENEHELPERS_H_
