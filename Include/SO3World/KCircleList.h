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

template<typename T, int nSize>
class KCircleList
{
public:
	KCircleList();
	~KCircleList();

	BOOL Push(T& Obj);

	BOOL Pop(T* pObj);

    BOOL Pick(T* pObj);

private:
    // ע��,�����ʵ����ֻ�ܴ��nSize-1��Ԫ��,m_nHeadָ���λ����Զ�ǿյ�
	T		    m_ObjList[nSize];	// ��������
	int         m_nHead;		    // ͷָ��
	int		    m_nTail;		    // βָ��
};

template<typename T, int nSize>
KCircleList<T, nSize>::KCircleList()
{
    assert(nSize > 2);
	m_nHead    = 0;
    m_nTail    = 0;
}

template<typename T, int nSize>
KCircleList<T, nSize>::~KCircleList()
{
}


template<typename T, int nSize>
BOOL KCircleList<T, nSize>::Push(T& Obj)
{
	int nNextPos = (m_nHead + 1) % nSize;

	if (nNextPos == m_nTail)
		return false;

	m_ObjList[m_nHead]  = Obj;
	m_nHead             = nNextPos;

	return true;
}

template<typename T, int nSize>
BOOL KCircleList<T, nSize>::Pop(T* pObj)
{
	if (m_nTail == m_nHead)
		return false;

    if (pObj)
    {
	    *pObj = m_ObjList[m_nTail];
    }

	m_nTail = (m_nTail + 1) % nSize;

	return true;
}

template<typename T, int nSize>
BOOL KCircleList<T, nSize>::Pick(T* pObj)
{
	if (m_nTail == m_nHead)
		return false;

    if (pObj)
    {
	    *pObj = m_ObjList[m_nTail];
    }

	return true;
}

#endif	//_KCIRCLE_LIST_H_

