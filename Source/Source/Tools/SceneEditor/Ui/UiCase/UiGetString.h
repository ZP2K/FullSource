// -------------------------------------------------------------------------
//	�ļ���		��	UiGetString.h
//	������		��	Wooy(Wu yue)
//	����ʱ��	��	2003-3-18
//	��������	��	����ִ�����
// -------------------------------------------------------------------------
#ifndef __UiGetString_H__
#define __UiGetString_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"


class KUiGetString : protected KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiGetString*	OpenWindow(const char* pszTitle,
				const char* pszInitString,				
				KWndWindow* pRequester, unsigned int uParam,
				int nMinLen = 4, int nMaxLen = 16,
				BOOL bForbitMove = FALSE,
				BOOL bInputNumber = FALSE);	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiGetString*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//������淽��
	static void			CloseWindow(bool bDestroy);		//�رմ���
	BOOL				IsGameSpaceForbitMove() { return m_bForbidMove; }
private:
	KUiGetString();
	~KUiGetString() {}
	void	Initialize(BOOL bNumber);								//��ʼ��
	void	Show();
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	OnCheckInput();

private:
	static KUiGetString*	m_pSelf;
	BOOL				m_bForbidMove;
	BOOL				m_bIsNumberInput;
	KWndText32			m_Title;
	int					m_nMinLen, m_nMaxLen;
	KWndEdit32			m_StringEdit;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	KWndWindow*			m_pRequester;
	unsigned int		m_uRequesterParam;
};


#endif // __UiGetString_H__