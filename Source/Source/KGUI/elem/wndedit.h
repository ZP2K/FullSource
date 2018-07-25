////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : wndedit.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2006-7-18 10:43:13
//  Comment     : 
//                Edited by Chen Jie 2006-12-30
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_WNDEDIT_H_
#define _INCLUDE_WNDEDIT_H_

////////////////////////////////////////////////////////////////////////////////
#include "./wndwindow.h"
#include "./Components/itemtext.h"
#include "../driver/kcolor.h"

#define	WNDEDIT_ES_FILTER				0x000000FF
#define	WNDEDIT_ES_ENGLISH_AVAILABLE	0x00000001		//�������������ַ������š�����
#define	WNDEDIT_ES_IME_AVAILABLE		0x00000002		//��������ime�ַ��������ַ�����
#define	WNDEDIT_ES_CHAR_TYPE_FILTER		(WNDEDIT_ES_ENGLISH_AVAILABLE | WNDEDIT_ES_IME_AVAILABLE)
#define	WNDEDIT_ES_MASK_CHARACTER		0x00000010		//��ʾʱ����������滻Ҫ��ʾ���ַ�
#define	WNDEDIT_ES_MULTI_LINE			0x00000020		//���б༭
//#define	WNDEDIT_ES_WIN32_WND			0x0040		//����Win32����
#define	WNDEDIT_F_HAVE_FOCUS			0x00000100		//Ϊ��ǰӵ�����뽹��Ĵ���
#define	WNDEDIT_F_SHOW_CARET			0x00000200		//��ʾ���
#define	WNDEDIT_F_SHOW_DATA_UNFORMATED	0x00000400		//��ʾ����û�о���ʽ��
#define	WNDEDIT_F_SEL_STARTED			0x00000800		//���ڽ���ѡ��
#define	WNDEDIT_F_ENABLE_ANIMATE		0x00001000		//������Ʒ�ת��Ϊ�������
#define WNDEDIT_F_STR_TRANSFORMED       0x00002000      //�Ѵ���ʾ����ת�����ַ���
#define WNDEDIT_F_CURSOR_MOVED          0x00004000      //����λ�÷����˱仯
#define WNDEDIT_F_DRAW_DATA_CHANGED     0x00008000      //�����б����˱仯
#define WNDEDIT_F_LIMIT_MULTIBYTE		0x00010000      //����ĳ��ȵĿ���ʹ��MultiByteΪ���㵥λ

#define K_INPUT_CURSOR				    L"|"
#define K_INPUT_MASK                    L"*"
#define K_INPUT_MASK_CHAR               L'*'
#define K_INPUT_CURSOR_NAME			    "_Cursor"
#define K_INPUT_MASK_NAME               "_Mask"
#define K_INPUT_TEXT_NAME               "_Text"
#define K_INPUT_TERM_NAME               "_Term"     //TERMINATOR, ������־
#define K_INPUT_OBJ_NAME				"_Obj"

namespace UI
{

class KWndEdit : public KWndWindow
{
private:
    typedef std::vector<KItemNull *> KItemArray;

#ifdef KGUI_LUA_LOADER
public:
    static int  Create(IKUIObject **ppObj, Lua_State *pLua, int nTableIndex);
    virtual int Init(Lua_State *pLua, int nTableIndex);
#endif

public:
	virtual void	SetSize(float fWidth, float fHeight);					//���ô��ڴ�С                        
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void    OnPaint();

	virtual void    AbsMove(float fXOffset, float fYOffset);

	virtual void    Enable(int nEnable);

    int             SetText(LPCWSTR wszText);
	LPCWSTR         GetText();
	int             GetPassword(char szDestMD5String[64]);
	int             GetTextLength(BOOL bMultiByte);
	int             ClearText();

    int             InsertObject(LPCSTR szTypeName, LPCWSTR wszShowText, int nUserData);
    int             GetTextLuaStruct(Lua_State* L);

	void            SetFontScheme(int nFontScheme);
	void			SetFontColor(DWORD dwColor);
	int             GetFontScheme();

	int             SelectAll();
	int				CancelSelect();
	int             SetType(int nType);	//������������� 0����/1Ӣ��/2��Ӣ��

	int				SetLimit(int nLimit);
	int				GetLimit();

	void			SetLimitMultiByte(int nMultiByte);
	int				IsLimitMultiByte();

	int				IsPassword();
	virtual int     CanCastTo(LPCSTR szType) const;
    virtual LPCSTR	GetWndType() const;

	int				AppendString(LPCWSTR wszString);
    int             Backspace();

	BOOL IsStatus(DWORD dwStatus) const;
	void SetStatus(DWORD dwStatus, BOOL bEnable);

	void SetFontSpacing(float fSpacing);
	void SetRowSpacing(float fSpacing);
	void SetFocusBgColor(DWORD dwColor);
    void SetSelectBgColor(DWORD dwColor);
	void SetSelectFontScheme(int nFontScheme);
	void SetCaretFontScheme(int nFontScheme);

	int CreateCursorHandle();
	int CreateTextHandle();

	BOOL SetCurSel(int nStart, int nEnd, BOOL bMultiByte);
protected:
	virtual void    OnBreathe();
	virtual void    OnScale(float fScaleX, float fScaleY);

	LRESULT	        OnLButtonDown(float fX, float fY);
	LRESULT	        OnLButtonUp(float fX, float fY);
	LRESULT	        OnRButtonDown(float fX, float fY);
	LRESULT         OnRButtonUp(float fX, float fY);
	LRESULT         OnMouseMove(WPARAM wParam, float fX, float fY);

	LRESULT         OnMouseWheel(WPARAM wParam);

	LRESULT         OnLButtonDblClk();

	LRESULT         OnChar(WPARAM wParam);
	LRESULT         OnImeChar(WPARAM wParam);

	LRESULT         OnKeyDown(WPARAM wKey, LPARAM lModifier);
	LRESULT         OnSysKeyDown(WPARAM wParam, LPARAM lParam);

	LRESULT         OnKeyDownSpecial(WPARAM wKey);

	LRESULT         OnSetFocus();
	LRESULT         OnKillFocus();

private:
	int             CopyString();
	int             PasteString();
	int             DeleteString();

	int				GetTextRowHeight(int nIndex, float &fRowHeight);
	int				IsTextRowVisible(int nIndex);
	int             GetTextPos(float fX, float fY, DWORD *pdwRetPos, int *pnAfterFlag);
 	int             FormatShowData();
	int             LetCaretShow();

	void            CorrectImePos();

    int             IsInSelectRegion(DWORD dwPos);
    void            UpdateSelRegion();
	void            SetSelStartPos(DWORD dwPos, int nAfterFlag);
	void            SetSelEndPos(DWORD dwPos, int nAfterFlag);
	void            SetCaretPos(DWORD dwPos, int nAfterFlag);
    DWORD           GetCaretPos();

    float           GetFontHeight(int nFontScheme);

	int             IgnoreKey(WPARAM wKey);
	int             HoldUpKey(WPARAM wKey);

    void            MoveCaretCharPrev();
    void            MoveCaretCharNext();
	void            MoveCaretWordPrev();
	void            MoveCaretWordNext();
	void            MoveCaretRowPrev();
	void            MoveCaretRowNext();
	void            MoveCaretPagePrev();
	void            MoveCaretPageNext();
	void            MoveCaretRowHome();
	void            MoveCaretRowEnd();

	int             AppendChar(WCHAR wcChar);
	int             AppendFilteredChar(WCHAR wcChar);

	void			Remove(KItemNull *pItem);

private:
	DWORD           m_dwStatus;

    std::wstring    m_strText;

	float           m_fFontSpacing;         //�ּ��
	float           m_fRowSpacing;			//�м��

    DWORD           m_dwSelStartPos;        //ѡ��������ʼ��
    DWORD           m_dwSelEndPos;          //ѡ�����������

	DWORD           m_dwCaretPos;           //�������λ��
    int             m_nCaretAfterFlag;      //��꿿�ַ�ǰ��ʾ���ǿ��ַ�����ʾ
	DWORD           m_dwCaretTickCount;     //���Ʋ������ʾ��ʱ��

	int				m_nTextLimit;          //�����û��������󳤶�, -1��ʾ������

	DWORD           m_dwFocusBgColor;       //�н���ʱ�ı�����ɫ
    DWORD           m_dwSelectBgColor;         //ѡ�в��ֵı���ɫ

	int	            m_nFontScheme;          //ƽʱ������
	int             m_nSelectFontScheme;      //ѡ��ʱ������
	int             m_nCaretFontScheme;     //����õ�����

	DWORD			m_dwFontColor;			//ƽʱ��������ɫ

    KItemText*      m_pCursor;              //���ڴ�Ź��
	KItemHandle*    m_pTextHandle;	        //���ڴ����ʾ���ݵ�Handle

	KItemArray      m_aDraw;

public:
	KWndEdit();
	virtual	~KWndEdit();

	void* operator new(size_t nSize, std::nothrow_t const&) throw();
	void  operator delete(void* pData, std::nothrow_t const&) throw();

	void* operator new(size_t nSize);
	void  operator delete(void* pData) throw();

protected:
	void* operator new[](std::size_t, std::nothrow_t const&) throw();
	void  operator delete[](void*, std::nothrow_t const&) throw();

	void* operator new[](std::size_t) throw(std::bad_alloc);
	void  operator delete[](void*) throw();

	void* operator new(size_t, void*) throw();
	void  operator delete(void*, void*) throw();

	void* operator new[](std::size_t, void*) throw();
	void  operator delete[](void*, void*) throw();
};


inline BOOL KWndEdit::IsStatus(DWORD dwStatus) const
{
	return (m_dwStatus & dwStatus) != 0; 
}

}

#endif //_INCLUDE_WNDEDIT_H_
