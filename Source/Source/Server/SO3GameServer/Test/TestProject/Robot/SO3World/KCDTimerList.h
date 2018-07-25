/************************************************************************/
/* ���Cooldown��ʱ����						                            */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.11.03	Create												*/
/*		2006.05.15	Modified interval precision from second to frame	*/
/************************************************************************/
#ifndef _KCDTIMER_LIST_H_
#define _KCDTIMER_LIST_H_

class KPlayer;

class KCDTimerList
{
	EXPORT_TEST(KCDTimerList);
public:
	KCDTimerList(void);
	~KCDTimerList(void);

	char* Load(char* pData);
	char* Save(char* pData);

	//���ü�ʱ������ʱΪnInterval֡
	void ResetTimer(DWORD dwTimerID, int nInterval);
	//����ʱ��������TrueΪ��ʱ���Ѿ���ʱ������FALSEΪδ����
	BOOL CheckTimer(DWORD dwTimerID);
	//�����ʱ���ļ�ʱ
	void ClearTimer(DWORD dwTimerID);
	//��ȡ��ʱʱ������λ��֡
	BOOL GetTimerData(DWORD dwTimerID, int& rnInterval, int& rnEndFrame);

    KPlayer *m_pPlayer;

private:
	struct _TIMER_DATA 
	{
		int		nEndFrame;
		int		nInterval;
	};
	typedef std::map<DWORD, _TIMER_DATA> TIMER_LIST;
	TIMER_LIST	m_TimerList;
};

#endif	//_KCDTIMER_LIST_H_