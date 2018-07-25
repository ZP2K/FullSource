/************************************************************************/
/* ѭ������								                                */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.02	Create												*/
/* Comment	 :															*/
/*		����һ���߳�ֻ����һ���߳�ֻд�Ĳ����������ǰ�ȫ��				*/
/*																		*/
/* P.S.		������һ��													*/
/*		�� �� �� �� �� ��												*/
/*		�� �� �� �� �� ��												*/
/*		�� �� ʱ ǲ ̽ �� ��											*/
/*		�� �� �� ë ô ��												*/
/*																		*/
/*		�� �� �� �� �� ��												*/
/*		ϴ ױ �� �� �� ��												*/
/*		�� �� �� �� �� �� ��											*/
/*		�� �� �� �� ͬ ��												*/
/*																		*/
/*		�ôʴ�Ϊ��������檳�������										*/
/************************************************************************/
#ifndef _KCIRCLE_LIST_H_
#define _KCIRCLE_LIST_H_

class KCircleList
{
public:
	KCircleList(void);
	~KCircleList(void);

	//��ʼ����ָ��ѭ�����г���
	BOOL Init(DWORD dwSize);

	//ѹ������ָ��
	BOOL Push(void* pObj);
	//��������ָ��
	void* Pop(void);
	//��⼴��������ָ��
	void* Check(void);

private:
	DWORD		m_dwSize;		//���г���
	void**		m_ObjList;		//��������
	DWORD		m_dwHead;		//ͷָ��
	DWORD		m_dwRear;		//βָ��
};
#endif	//_KCIRCLE_LIST_H_