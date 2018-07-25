////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneCustomScrollBar.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-25 15:07:36
//  Comment     : ���ڿ����������塣����Ϣ���䣬������WM_VSCROLL��WM_HSCROLL
/*Ȼ���ٴ���󶨵�CWnd������Ϣ�����͵�ʱ�����ݶԷ���ScrollBar�ı�������
������С������Է���ScrollBarҲ�Ƕ�̬���ֵĻ���Ҳ��Ҫ�Է������ScrollBar
�����ſ��Զ�̬������С
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KSCENECUSTOMSCROLLBAR_H_
#define _INCLUDE_KSCENECUSTOMSCROLLBAR_H_

////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>

// KSceneCustomScrollBar
class KSceneCustomScrollBar : public CScrollBar
{
	DECLARE_DYNAMIC(KSceneCustomScrollBar)

public:
	KSceneCustomScrollBar();
	virtual ~KSceneCustomScrollBar();
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
private:
	inline void OnScroll32(UINT message, UINT nSBCode, UINT nPos);
	void ReCalculateScrollScales();
protected:
	DECLARE_MESSAGE_MAP()
private:
	std::vector<CWnd*> m_vecWnd;
	std::vector<float>	m_vecScrollScale;
	INT m_nBarType;
	bool m_bLockMessagePocess;
public:
	template<UINT _BindIndex>
	BOOL BindAnWnd(CWnd* pCWnd, CWnd** ppOld = NULL)
	{ 
		_ASSERTE(pCWnd);
		if (_BindIndex == m_vecWnd.size())
		{
			m_vecWnd.push_back(pCWnd);
			m_vecScrollScale.push_back(0);
			return TRUE;
		}
		else if (_BindIndex > m_vecWnd.size())
		{
			return FALSE;
		}
		if (ppOld)
			*ppOld = m_vecWnd[_BindIndex];
		m_vecWnd[_BindIndex] = pCWnd;
		return TRUE;
	}
	void ReAlignBindedWindows();
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};




#endif //_INCLUDE_KSCENECUSTOMSCROLLBAR_H_
