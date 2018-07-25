//////////////////////////////////////////////////////////////////////////////////////
//	�ļ���			:	UiKillerAllTask.h
//	������			:	������
//	����ʱ��		:	2003-12-26 9:55:17
//	�ļ�˵��		:	�鿴����ͨ������ĶԻ���
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __UIKILLERALLTASK_H__
#define __UIKILLERALLTASK_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"

#include "GameDataDef.h"
#include <vector>
using namespace std;

class KUiKillerAllTask : public KWndShowAnimate
{
public:
	static KUiKillerAllTask* OpenWindow();				//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiKillerAllTask* GetIfVisible();			//�������������ʾ���򷵻�ʵ��ָ��
	static void		CloseWindow(bool bDestroy);			//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void		LoadScheme(const char* pScheme);	//������淽��
	void			ReceiveTask(KILLER_TASK* pTask);	//��Ʒ�仯����

private:
	KUiKillerAllTask()
	{
		m_bRecvOver = FALSE;
		m_pTasks = NULL;
		m_nCurTaskNum = 0;
		m_nCurTaskNumMax = 0;
		m_bSortByReward = TRUE;
		m_bSortIncrease = FALSE;
	}
	virtual ~KUiKillerAllTask()
	{ 
		ClearData();
	}
	void	Initialize();								//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	ClearData();
	void	ResortList();
	void	PushInSortedList(KILLER_TASK* pTask);
private:
	static	KUiKillerAllTask*	m_pSelf;
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
	KWndButton			m_AcceptBtn;
	KWndButton			m_LeaveBtn;
};


#endif //__UIKILLERALLTASK_H__
