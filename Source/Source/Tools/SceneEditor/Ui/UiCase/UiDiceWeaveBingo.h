/*******************************************************************************
File        : UiDiceWeaveBingo.h
Creator     : Fyt(Fan Zhanpeng)
create data : 12-12-2003(mm-dd-yyyy)
Description : ɫ���ϻ������й������γ���ΰ����������(���ٽ�����������������)
********************************************************************************/

#if !defined(AFX_UIDICEWEAVEBINGO_H__9204EC3A_3A24_4298_A6B4_46A8FED47C86__INCLUDED_)
#define AFX_UIDICEWEAVEBINGO_H__9204EC3A_3A24_4298_A6B4_46A8FED47C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndpuretextbtn.h"
#include "../elem/wndbutton.h"
#include "../elem/wndimage.h"
#include "../elem/wndtext.h"


class KMyWndImage : public KWndImage
{
public:
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
};


class KUiDiceWeaveBingo : public KWndImage
{
public:
	KUiDiceWeaveBingo();
	virtual ~KUiDiceWeaveBingo();

	static        KUiDiceWeaveBingo* OpenWindow();      //�򿪴���
	static        KUiDiceWeaveBingo* GetIfVisible();    //�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE);	//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);		//������淽��

	void			UpdateUI(int nBingoMoney);	//���½������ʾ

	//һ��ɫ�ӽ��������
	void			DiceNumberIncome(int nDice, int nNumber);

	//�񽱽��������
	void			ResultIncome(int nResultStatus, char *szString = NULL);

public:
	enum enumDICE_WEAVER_BINGO_STATUS					//����������״̬
	{
		enumDWBS_INITIALIZE,		//��ʼ��
		enumDWBS_WAITING_BET,		//�ȴ���ע
		enumDWBS_BETTING,			//���ڶĲ�
		enumDWBS_FINISH_ONCE,		//��ɿ�~������Ч����������
		enumDWBS_WIN,				//ӮǮ��
		enumDWBS_LOSE,				//��Ǯ��
		enumDWBS_TRY_AGAIN/*?*/,	//ѯ���Ƿ�����һ��
	};

private:
	static        KUiDiceWeaveBingo *ms_pSelf;

private:
	void			Initialize();
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	Breathe();

	void			RequestBingoMoney();		//�������Ҫ����ۻ�����

	//��Ӧɫ��ֹͣ����
	void			OnStopDice(KMyWndImage* Dice);

	int				PlayEffect();				//�����н�Ч��
	int				PlayDiceRolling();			//����ɫ��ת��
	
	int				InitializeGame();			//��ʼ����Ϸ

	int				OnBet();					//��Ӧ��ע

private: /*Controls*/
	KWndText32		m_TotleBingoMoney;
	KWndText32		m_PlayerMoney;

	KWndText256		m_TextTip;
	KMyWndImage		m_Dice[3];
	KWndButton		m_AgainAndBet;
	KWndButton		m_Leave;

private:
	unsigned int	m_uColor;
	unsigned int	m_uWinkColor;

	int				m_aryStopTime[3];
	int				m_nStatus;

	int				m_nbIsBingoMoneyIncome;	//�Ƿ��Ѿ���ȡ�󽱽������Ŀ
	int				m_nbIsResultIncome;		//�Ƿ����Ѿ�������

	int				m_aryDiceResult[3];		//����ɫ�ӵĽ��
	int				m_nResultStatus;		//�����

	int				m_nTextFontSize;		//��ʾ���ֵĴ�С

	char			m_szPay[256];			//��ʾ��ע
	char			m_szNotEnoughMoney[256];//��ʾ��Ǯ����
	char			m_szBet[256];			//��ʾ��ֹͣ
	char			m_szLose[256];			//��ʾ����
	char			m_szEarnSome[256];		//��ʾӮ��һ���
	char			m_szEarnMore[256];		//��ʾӮ����һ��
	char			m_szBingo[256];			//��ʾӮ��
	char			m_szAgain[256];			//ѯ���Ƿ�����һ��

	//�ⲿ�����Ľ���ִ�
	char			m_szComingResultString[256];
	int				m_nPlayEffectTime;

	//��¼״̬���
	int				m_nStatusHandle;
};

#endif // !defined(AFX_UIDICEWEAVEBINGO_H__9204EC3A_3A24_4298_A6B4_46A8FED47C86__INCLUDED_)
