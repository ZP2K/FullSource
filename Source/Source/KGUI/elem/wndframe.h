/*
//������ϵ֮Frame
//Create bu hu hangyin 2005.09.02

*/


#ifndef UI_ELEM_K_WND_FRAME_H
#define UI_ELEM_K_WND_FRAME_H

#include "./wndwindow.h"


namespace UI
{

	enum
	{
		UI_WND_FRAME_FOLLOW_MOUSE_MOVE	= 0x00000001,
		UI_WND_FRAME_ENABLE_DRAG		= 0x00000002,
		UI_WND_FRAME_ON_DRAGGING		= 0x00000004,
		UI_WND_FRAME_MSG_FROM_CHILD		= 0x00000008,
		UI_WND_FRAME_TAB_CHANGE_FOCUS	= 0x00000010,
		UI_WND_FRAME_MOUSE_FOLLOW_FOCUS	= 0x00000020,
		UI_WND_FRAME_HAS_DRAGGED		= 0x00000040,
		UI_WND_FRAME_DISABLE_BREATH		= 0x00000080,

		UI_WND_FRAME_DESTROY_LUA_TABLE	= 0x00000100,

		UI_WND_FRAME_FADE_IN			= 0x00000200,
		UI_WND_FRAME_FADE_OUT			= 0x00000400,

		UI_WND_FRAME_SHOW_WHEN_HIDE_UI	= 0x00000800,
		UI_WND_FRAME_POST_RENDER_EVENT	= 0x00001000,
		UI_WND_FRAME_RENDER_SAMPLING	= 0x00002000,
	};

	class KWndFrame :public KWndWindow
	{
	private:
        typedef std::vector<std::string> KWndNameArray;

		enum { SCRIPT_FILE_NAME_LEN = 80 };
		enum { SCRIPT_TABLE_NAME_LEN = 80 };

        enum FOCUS 
        {
            FOCUS_PREV,
            FOCUS_NEXT
        };

	public:
		KWndFrame();
		virtual ~KWndFrame();

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

        int         GetLayer() { return m_nLayer; }
#endif

	public:
		virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);	
		virtual void    SetRelPos(float fX, float fY);	//���ô���λ�ã��������
		virtual int		PtInWindow(int x, int y);
		virtual int		CanCastTo(LPCSTR szType) const;
        virtual LPCSTR	GetWndType() const;

		void		FireCreate();

		int			EnableDrag(int nEnableFlag);
		int			IsDragable(); 

		void		SetFocusWnd(KWndWindow *pWnd);//�������뽹��
		KWndWindow*	GetFocusWnd();//��ȡ���뽹��

		void		SetLuaClass(LPCSTR szClass);
		LPCSTR		GetLuaClass() const;

		int			EnableFollowMouseMove(int nEnableFlag);
		int			IsFollowMouseMove() const;

		int	FocusPrev();
		int	FocusNext();
		int	FocusHome();
		int	FocusEnd();
		void AppendFocus(LPCSTR szWndName);

        BOOL IsStatus(DWORD dwStatus) const;
        void SetStatus(DWORD dwStatus, BOOL bEnable);

		void SetDragArea(float fX, float fY, float fWidth, float fHeight);
		void SetFollowMouseOffset(float fX, float fY);

		void FireDestroy();

		void SetDefaultTableRef(int nRef);
		int GetDefaultTableRef();

		void FadeIn(int nLoopCount);
		void FadeOut(int nLoopCount);

		void ShowWhenUIHide();
		void HideWhenUIHide();
		int IsVisibleWhenUIHide();

	protected:
		virtual void OnPaint();
		virtual void OnBreathe();
		virtual void OnDestroy();
		virtual void OnShow();
		virtual void OnHide();
		virtual void OnScale(float fScaleX, float fScaleY);				

	private:
		int SetFocusByIndex(int nIndex);
		int ChangeFocus(FOCUS focus);
		int	FindTabListIndexByWndSectionName(LPCSTR pszString, int nBeginIndex = 0);

		int		TryBeginDragWnd(int x, int y);				//�ǲ���Ҫ�����϶�����
		void	OnDragWnd(int x, int y);					//�϶�����
		void	OnDragWndEnd(int x, int y);					//�϶�����
		LRESULT	OnKeyDown(WPARAM wKey, LPARAM lModifier);	//��Ӧ���̰�������
		LRESULT ChildProcessInput(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		DWORD		m_dwStatus;

		float		m_fDragAreaX;
		float		m_fDragAreaY;
		float		m_fDragAreaWidth;
		float		m_fDragAreaHeight;

		float		m_fLastMouseHoldX;							//�������x
		float		m_fLastMouseHoldY;							//�������y

		float		m_fFollowMouseOffsetX;						//�������ƫ��x
		float		m_fFollowMouseOffsetY;						//�������ƫ��y  

		char		m_szLuaClass[SCRIPT_TABLE_NAME_LEN];		//������
		int			m_nDefaultTableRef;							//Ĭ�ϴ�������tab��refֵ

		KWndWindow*	m_pWndFocus;								//��ǰ���ռ�����Ϣ����������Ϣ�ĵĴ���

		int			m_nFadeCount;
		int			m_nFadeFrame;

		KWndNameArray m_FocusWndNameList; 

#ifdef KGUI_LUA_LOADER
        int         m_nLayer;
#endif
	};


	inline BOOL KWndFrame::IsStatus(DWORD dwStatus) const
	{
		return (m_dwStatus & dwStatus) != 0; 
	}

	inline void KWndFrame::SetDefaultTableRef(int nRef)
	{
		m_nDefaultTableRef = nRef;
	}

	inline int KWndFrame::GetDefaultTableRef()
	{
		return m_nDefaultTableRef;
	}

}

#endif