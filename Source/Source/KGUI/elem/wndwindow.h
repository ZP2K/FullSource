/*****************************************************************************************
//	�������棬���洰����ϵ�ṹ����������ڶ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
------------------------------------------------------------------------------------------
*****************************************************************************************/
#ifndef UI_ELEM_K_WND_WINDOW_H
#define UI_ELEM_K_WND_WINDOW_H


#ifdef KGUI_LUA_LOADER
#include "./uiobject.h"
#endif

//============���ڷ��================
#define WND_S_VISIBLE				0x80000000	//�ɼ�
#define WND_S_AREA_BY_EVENT_ITEM    0x40000000  //��ItemHandle�е�ע����¼���item�����䷶Χ
#define	WND_S_TOPMOST				0x20000000	//�ö�����
#define	WND_S_MOVEABLE				0x10000000	//�ɱ��϶�
#define WND_S_MOUSE_PENETRABLE      0x08000000  //�����ȫ��͸���������κ���Ϣ
#define	WND_S_DISABLE				0x04000000	//���ڲ��ɱ�����
#define	WND_S_TOBEDESTROY			0x02000000	//window is to be destroy
#define	WND_S_SIZE_WITH_ALL_CHILD	0x00100000	//���Ӵ��ڵķ�Χ���Եļ�����Ϊ�Լ��ķ�Χ���Եļ���
#define WND_S_SELF_HOLD_MOUSE_HOVER 0x00080000
#define WND_S_DISABLE_BRING_TO_TOP  0x00040000  //��ֹbringtotop
#define WND_S_EVENT_LIST_CHANGED    0x00020000  //�¼��б����˸ı�
#define WND_S_DRAW_LIST_CHANGED     0x00010000  //�����б����˸ı�
#define WND_S_BREAHTE_WHEN_HIDE     0x00008000  //��ʹ����ҳ����breathe

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

namespace UI
{
	enum { WND_TYPE_NAME_LEN = 32 };
	enum { WND_SECTION_NAME_LEN = 64 };

	enum 
    {
        WND_DO_NOT_DESTROY_MY_BROTHER = 0,
        WND_DESTROY_MY_BROTHER = 1,
    };

    enum
    {
        WND_SIDE_BEGIN,

        WND_SIDE_TOP		= WND_SIDE_BEGIN,
        WND_SIDE_BOTTOM,
        WND_SIDE_LEFT,
        WND_SIDE_RIGHT,
        WND_SIDE_TOPLEFT,
        WND_SIDE_TOPRIGHT,
        WND_SIDE_BOTTOMLEFT,
        WND_SIDE_BOTTOMRIGHT,
        WND_SIDE_CENTER,
        WND_SIDE_LEFTCENTER,
        WND_SIDE_RIGHTCENTER,
        WND_SIDE_TOPCENTER,
        WND_SIDE_BOTTOMCENTER,

        WND_SIDE_END,
    };

    class KItemHandle;
    class KItemNull;
    class KItemEventMgr;

    class KWndWindow 
#ifdef KGUI_LUA_LOADER
		: public IKUIObject
#endif
    {
    private:
        typedef std::vector<KItemNull*> KItemList;

    public:
        KWndWindow();
        virtual	~KWndWindow();

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
        static int   Create(IKUIObject **ppObj, Lua_State *pLua, int nTableIndex);
        virtual void Release();
        virtual int  Init(Lua_State *pLua, int nTableIndex);
        virtual int  AddChild(IKUIObject *pChild);
#endif

    public:
        static int GetSide(LPCSTR szSide);

        virtual int	PtInWindow(int x, int y);

        virtual void	            Paint();												//����
        virtual void	            PaintDebugInfo();
        virtual void	            SetSize(float fWidth, float fHeight);					//���ô��ڴ�С
        virtual	void	            Enable(int bEnable);									//��ֹ��������ʹ���ڱ�����
        virtual int 	            Scale(float fScaleX, float fScaleY);					//�Ŵ���С
        virtual KWndWindow*	        TopChildFromPoint(int x, int y);						//�õ�����ָ������λ�õ����ϲ㴰��
        virtual LRESULT             WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);		//���ں���
        virtual int					CanCastTo(LPCSTR szType) const;
        virtual LPCSTR				GetWndType() const;

        virtual int					SetAlpha(int nAlpha);
        int							SetSelfAlpha(int nAlpha);
        int							GetAlpha();

        void	                    Destroy(int nDestroyMyBrother);
        void             			BringToTop();											//�Ѵ����ö�
        void 			            GetSize(float *pWidth, float *pHeight) const;			//��ȡ���ڴ�С
        void			            SetCursorAbove();										//ʹ���ָ�����ƶ��������ڴ˴����е�λ����
        BOOL				        IsVisible() const;										//�жϴ����Ƿ���ʾ
        BOOL				        IsDisable() const;
        BOOL			            IsStyle(DWORD dwStyle) const;
        void			            SetStyle(DWORD dwStyle, BOOL bEnable);					//�޸Ĵ��ڷ��
        void			            Breathe();												//�ô��ڻ
        void						Show();													//��ʾ����
        void						Hide();													//���ش���
        void			            SplitSmallFamily();										//���Լ������Ӵ��ڣ��Ӵ�������������������
        void			            LeaveAlone();											//�������޴��������ң�һ��ǣ��

		float GetRelX() const;
		float GetRelY() const;

		float GetAbsX() const;
		float GetAbsY() const;

		void SetRelPos(float fX, float fY);	//���ô���λ�ã��������
        void SetAbsPos(float fX, float fY);	//���ô���λ�ã���������

        virtual void			    AbsMove(float fXOffset, float fYOffset);				//��������ĵ���

        int 			            AddChild(KWndWindow *pChild);							//����Ӵ���
        int 			            AddBrother(KWndWindow *pBrother, int nJustBehindMe = false);						//����ֵܴ���
        void 			            GetAllChildLayoutRect(float *pfLeft, float *pfTop, float *pfRight, float* pfBottom) const;		//ȡ�ð��������Ӵ��ڷֲ��������С����
        KWndWindow*		            GetPrev() const;										//�õ�ǰһ���ֵܴ���
        KWndWindow*		            GetNext() const;										//�õ���һ���ֵܴ���
        KWndWindow*		            GetParent() const;										//�õ�������
        KWndWindow*		            GetFirstChild() const;									//�õ���һ���Ӵ���
        KWndWindow*		            GetOwner();												//��ò����и����ڵ����ȴ���
        void                        SetName(LPCSTR szName);
        LPCSTR                      GetName() const;

        int                         SetItemHandle(KItemHandle *pItemHandle);
        int                         ReleaseItemHandle();
        KItemHandle*                GetItemHandle() const;

        void						UpdateEvent();
        void						OnItemRelease(KItemNull *pItem);

        void						UpdateDrawList();

		void SetTip(int nTipIndex, int nShowTipType, int nRichText);
		void SetAreaTestFile(LPCSTR szFilePath);

        int	GetLuaRef();
        void SetLuaRef(int nRef);

        bool SetPoint(float fX, float fY);
        bool SetPoint(int nSide, float fX, float fY, KWndWindow const* pRelWnd, int nRelSide, float fRelOffsetX, float fRelOffsetY);

        int CreateItemHandle(IIniFile* pIni, LPCSTR pszSectionName);

        int UpdateLevel(int nLevel);

    protected:
        virtual void				OnDestroy();
        virtual void	            OnBreathe();											//���ڵĳ�����Ϊ
        virtual void	            OnPaint();												//�������
        virtual void	            OnShow();												//��ʾ����
        virtual void	            OnHide();												//���ش���
        virtual void				OnSize();
        virtual void				OnScale(float fScaleX, float fScaleY);
        virtual void				OnMove();
        void						EventCheck();
        void						DrawCheck();

    private:
        BOOL PtInThisWindow(float x, float y) const;

#ifndef DISABLE_UI_EDIT
        LRESULT						EditModeProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

    protected:
        float		                m_fRelX;			//���Ͻ�X���꣬����ڸ�����
        float		                m_fRelY;			//���Ͻ�Y���꣬����ڸ�����
        float		                m_fAbsX;			//�������Ͻǵľ�������x
        float		                m_fAbsY;			//�������Ͻǵľ�������y
        float		                m_fWidth;			//���
        float		                m_fHeight;			//�߶�

        DWORD						m_dwAlpha;			//Alpha

        char                        m_szName[64];

    private:
        DWORD		                m_dwStyle;			//���ڷ��,��Wnd.h�д��ڷ��Ķ���

		KWndWindow*	                m_pPrev;			//ǰһ��ͬ������
        KWndWindow*	                m_pNext;			//��һ��ͬ������
        KWndWindow*	                m_pFirstChild;		//��һ���Ӵ���
        KWndWindow*	                m_pParent;			//������

        KItemHandle*				m_pItemHandle;      //�ҽ�ItemHandle
        KItemEventMgr*				m_pEventMgr;
        KItemList	                m_aDrawItem;

        int							m_nTipIndex;
        int							m_nAreaIndex;

        int							m_nLuaRef;	//��lua�е�����

        int                         m_nLevel;
    };


    //----------------------------------------------------------------------------------------------------------
    inline BOOL KWndWindow::IsStyle(DWORD dwStyle) const
    { 
        return (m_dwStyle & dwStyle) != 0; 
    }

    inline KWndWindow* KWndWindow::GetPrev() const 
    { 
        return m_pPrev; 
    }

    inline KWndWindow* KWndWindow::GetNext() const 
    { 
        return m_pNext; 
    }

    inline KWndWindow* KWndWindow::GetParent() const
    { 
        return m_pParent; 
    }

    inline KWndWindow* KWndWindow::GetFirstChild() const
    { 
        return m_pFirstChild; 
    }

    inline KItemHandle* KWndWindow::GetItemHandle() const
    {
        return m_pItemHandle;
    }

    inline int KWndWindow::GetLuaRef()
    {
        return m_nLuaRef;
    }

    inline void KWndWindow::SetLuaRef(int nRef)
    {
        m_nLuaRef = nRef;
    }

    inline void KWndWindow::UpdateEvent()
    {
        SetStyle(WND_S_EVENT_LIST_CHANGED, TRUE);
    }

    inline void KWndWindow::UpdateDrawList()
    {
        SetStyle(WND_S_DRAW_LIST_CHANGED, TRUE);
    }

    void* CastWndWindow(KWndWindow* pWnd, LPCSTR szType);

} // namespace UI

#endif // UI_ELEM_K_WND_WINDOW_H

