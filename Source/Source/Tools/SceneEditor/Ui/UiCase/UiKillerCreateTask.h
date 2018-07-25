//////////////////////////////////////////////////////////////////////////////////////
//	�ļ���			:	UiKillerCreateTask.h
//	������			:	������
//	����ʱ��		:	2003-12-26 10:50:03
//	�ļ�˵��		:	����ͨ������Ի���
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __UIKILLERCREATETASK_H__
#define __UIKILLERCREATETASK_H__

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndEdit.h"

class KRoleNameEdit : public KWndEdit
{
private:
	char			m_Text[200];
public:
	KRoleNameEdit();
};


class KUiKillerCreateTask : public KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiKillerCreateTask*	OpenWindow(unsigned int uParam);	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiKillerCreateTask*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//������淽��
	static void			CloseWindow(bool bDestroy);		//�رմ���
private:
	KUiKillerCreateTask();
	virtual ~KUiKillerCreateTask() {}
	int		Initialize();								//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	OnModifyCount(bool bAdd);
	void	OnRewardInputChanged();
	void	OnTimeInputChanged();

private:
	static KUiKillerCreateTask*	m_pSelf;
	KRoleNameEdit		m_cKilleeName;					//��ɱ�߽�ɫ��
	KWndEdit32			m_cReward;	
	KWndText256			m_cActiveTime;					//����ʱ��(��λ:Сʱ)
	int					m_nActiveTimeNum;
	KWndButton          m_BtnIncrease, m_BtnDecrease;   //���Ӻͼ��ٵ�Hold�Ͱ�ť
	KWndText256			m_cCommision;					//ϵͳӶ��
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	int					m_nMinReward;
	int					m_nMoneyPerHour;
	int					m_nMaxActiveTime;
};


#endif //__UIKILLERCREATETASK_H__
