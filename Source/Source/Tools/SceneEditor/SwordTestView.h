#pragma once


// KSwordTestView ��ͼ

class KSwordTestView : public CView
{
	DECLARE_DYNCREATE(KSwordTestView)

protected:
	KSwordTestView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~KSwordTestView();

public:
	//GraphicsStruct::KCamera  m_Camera;

	virtual void OnDraw(CDC* pDC);      // ��д�Ի��ƴ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	int	 m_nOutputWindowID;

	//{{AFX_VIRTUAL(KSwordTestView)
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(KSceneEditorView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

};


