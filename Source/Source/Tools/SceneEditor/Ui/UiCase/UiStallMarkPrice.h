/* 
 * File:     UiStallMarkPrice.h
 * Desc:     ��̯���
 * Author:   flying
 * Creation: 2003/10/23
 */
#if !defined _UISTALLMARKPRICE_HEADER
#define _UISTALLMARKPRICE_HEADER

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"

class KUiStallMarkPrice : public KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiStallMarkPrice*	OpenWindow(int nMoney, int nMaxMoney, int nObjIdx,
										   unsigned int uParam, KWndWindow* pMoneyWnd);	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiStallMarkPrice*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//������淽��
	static void			CloseWindow(bool bDestroy);		//�رմ���
	static void   SetObjectName(char *pObjectName);
private:
	KUiStallMarkPrice();
	virtual ~KUiStallMarkPrice() {}
	int		Initialize();								//��ʼ��
	void	Show(KWndWindow* pMoneyWnd);
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	CheckInput();

private:
	static KUiStallMarkPrice*	m_pSelf;
	KWndEdit32			m_cPrice;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	int					m_nMaxMoney;
	int					m_nObjIndex;
	static	KWndText256   m_ObjectName;
};

#endif
