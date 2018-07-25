//////////////////////////////////////////////////////////////////////////////////////
//	�ļ���			:	UiKillerOwnTask.h
//	������			:	������
//	����ʱ��		:	2003-12-26 10:48:55
//	�ļ�˵��		:	�鿴�Լ�����������ȡ��ͨ������--�Ի���
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __UIKILLEROWNTASK_H__
#define __UIKILLEROWNTASK_H__

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"

#include "GameDataDef.h"
#include <vector>
using namespace std;

class KUiKillerOwnTask : public KWndShowAnimate
{
public:
	static KUiKillerOwnTask* OpenWindow();				//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiKillerOwnTask* GetIfVisible();			//�������������ʾ���򷵻�ʵ��ָ��
	static void		CloseWindow(bool bDestroy);			//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void		LoadScheme(const char* pScheme);	//������淽��
	void			ReceiveTask(KILLER_TASK* pTask);	//��Ʒ�仯����

private:
	KUiKillerOwnTask()
	{
		m_bRecvOver = FALSE;
		m_pTasks = NULL;
		m_nCurTaskNum = 0;
		m_nCurTaskNumMax = 0;
		m_bSortByReward = TRUE;
		m_bSortIncrease = FALSE;
	}
	virtual ~KUiKillerOwnTask()
	{ 
		ClearData();
	}
	void	Initialize();								//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	ClearData();
	void	ResortList();
	void	PushInSortedList(KILLER_TASK* pTask);
private:
	static	KUiKillerOwnTask*	m_pSelf;
	typedef struct {
		KILLER_TASK task;
		char szListItemText[64];
	}KILLER_TASK_ITEM;
	KILLER_TASK_ITEM*			m_pTasks;
	int							m_nCurTaskNum;
	int							m_nCurTaskNumMax;
	BOOL						m_bRecvOver;			//�Ƿ������������
	BOOL						m_bSortByReward;		//�Ƿ��ͽ�����,������������
	BOOL						m_bSortIncrease;		//�Ƿ���������
private:
	KWndButton			m_SortByRewardBtn;
	KWndButton			m_SortByTargetBtn;
	KWndList			m_TaskList;
	KWndScrollBar		m_TaskListCtrl;
	KWndButton			m_CancelTaskBtn;
	KWndButton			m_LeaveBtn;
};


#endif //__UIKILLEROWNTASK_H__
