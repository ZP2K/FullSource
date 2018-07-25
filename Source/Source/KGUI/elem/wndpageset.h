#ifndef UI_ELEM_K_WND_PAGE_SET_H
#define UI_ELEM_K_WND_PAGE_SET_H

#include "./wndwindow.h"

namespace UI
{

//------------------------------------------------------------------------------------------
//	���ҳ�漯�ϴ���������M$ Win32����� PropertieSheet
//------------------------------------------------------------------------------------------

class KWndCheckBox;
class KWndWindow;

class KWndPageSet : public KWndWindow
{
private:
	struct KWNDPAGECHECKBOXPAIR
	{
		KWndWindow*	    pWndPage;	//ҳ�洰��ָ��
		KWndCheckBox*	pWndCheckBox;	//ҳ�洰�ڹ�����ťָ��
	};
	
	typedef std::vector<KWNDPAGECHECKBOXPAIR> KWndPageCheckBoxArray;

public:
	KWndPageSet();
	~KWndPageSet();

	void* operator new(size_t nSize, std::nothrow_t const&) throw();
	void operator delete(void* pData, std::nothrow_t const&) throw();

	void* operator new(size_t nSize);
	void operator delete(void* pData) throw();

protected:
	void* operator new[](std::size_t, std::nothrow_t const&) throw();
	void operator delete[](void*, std::nothrow_t const&) throw();

	void* operator new[](std::size_t) throw(std::bad_alloc);
	void operator delete[](void*) throw();

	void* operator new(size_t, void*) throw();
	void operator delete(void*, void*) throw();

	void* operator new[](std::size_t, void*) throw();
	void operator delete[](void*, void*) throw();

#ifdef KGUI_LUA_LOADER
public:
    static int  Create(IKUIObject **ppObj, Lua_State *pLua, int nTableIndex);
    virtual int Init(Lua_State *pLua, int nTableIndex);
#endif

public:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);//���ں���
	virtual int	CanCastTo(LPCSTR szType) const;
	virtual LPCSTR GetWndType() const;

	int ActivePage(int nPageIndex);	//����ָ����ҳ��
	int ActivePage(KWndWindow* pWndPage);	
	int ActivePage(KWndCheckBox* pWndCheckBox);

	int	GetActivePageIndex() const;	//ȡ�õ�ǰ����ҳ�������
	int GetLastActivePageIndex() const;
	int AddPage(KWndWindow* pWndPage, KWndCheckBox* pWndCheckBox);	//���ҳ��
	int RemovePage(KWndWindow* pWndPage);
	int RemovePage(KWndCheckBox *pWndCheckBox);
	int RemovePage(int nPageIndex);

	KWndWindow* GetActivePage(); //����ָ����ҳ��
	KWndCheckBox* GetActiveCheckBox();

protected:
	virtual void OnShow();
	virtual void OnHide();
	virtual void OnPaint();

private:
	void OnPageBtnClick(KWndWindow* pCheckBox);		//��Ӧ�����л�ҳ�水ť�Ĳ���
	int GetPageIndex(KWndWindow* pWndPage);
	int GetPageIndex(KWndCheckBox* pWndCheckBox);

	int IsPageExist(KWndWindow* pWndPage, KWndCheckBox* pWndCheckBox);

private:
	KWndPageCheckBoxArray m_aPageCheckBox;

	int	m_nAcitvePage;				//��ǰ�����ҳ��
	int	m_nLastAcitvePage;			//ǰһ������ҳ
};


inline int KWndPageSet::GetActivePageIndex() const 
{
	return m_nAcitvePage; 
}

inline int KWndPageSet::GetLastActivePageIndex() const
{
	return m_nLastAcitvePage;
}

}

#endif // UI_ELEM_K_WND_PAGE_SET_H
