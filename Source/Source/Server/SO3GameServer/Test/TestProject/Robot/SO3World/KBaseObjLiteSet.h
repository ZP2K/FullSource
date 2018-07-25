/************************************************************************/
/* �򻯵����ݼ���                                                       */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu, Freeway Chen    							*/
/* History	 :															*/
/*		2005.01.14	Create												*/
/*		2006.12.23	use vector for speed and space						*/
/* Comment	 :															*/
/*		����Ҫ��														*/
/*			1	���ٱ���												*/
/*			2	����ɾ���Ͳ���											*/
/*			�������KBaseObjSet�ļ��࣬ȡ����ID��Map��ȡ�����Զ�����ID*/
/************************************************************************/
#ifndef _KBASEOBJLITESET_H_
#define _KBASEOBJLITESET_H_

#include <stdlib.h>
#include <vector>

using namespace std;

template <class T>
class KBaseObjLiteSet
{
public:
	KBaseObjLiteSet(void);
	~KBaseObjLiteSet(void);

	//��ʼ������Ҫָ�����������
	BOOL	Init(DWORD dwSize);

	//�����¶���
	T *New();

	//�ͷŶ���
	BOOL Delete(T *pObj);

    BOOL NewArray(int nNewCount, T **ppRetArray);
    BOOL DeleteArray(int nCount, T **ppArray);

	//��ȡ�洢����Ĵ�С
	DWORD GetSize();

private:
	//���飬�����洢
	DWORD					m_dwSize;
	//���б�
	typedef vector<T *>	    FREE_VECTOR, HEAD_VECTOR;
	FREE_VECTOR				m_FreeVector;
	HEAD_VECTOR				m_HeadVector;
};

template <class T>
inline KBaseObjLiteSet<T>::KBaseObjLiteSet(void)
{
	m_dwSize = 0;

	return;
}

template <class T>
inline KBaseObjLiteSet<T>::~KBaseObjLiteSet(void)
{
	//�ڴ�й©���
	ASSERT(m_FreeVector.size() == m_dwSize);

	HEAD_VECTOR::iterator	it;
    for (it = m_HeadVector.begin(); it != m_HeadVector.end(); ++it)
    {
		T *pHead = *it;
		KG_DELETE_ARRAY(pHead);
    }
	m_HeadVector.clear();
	m_FreeVector.clear();

	return;
}

template <class T>
inline BOOL KBaseObjLiteSet<T>::Init(DWORD dwSize)
{
    int nResult = false;
	//�ͷſռ估�������
	HEAD_VECTOR::iterator	it;
	for (it = m_HeadVector.begin(); it != m_HeadVector.end(); ++it)
	{
		T *pHead = *it;
		KG_DELETE_ARRAY(pHead);
	}
	m_HeadVector.clear();
	m_FreeVector.clear();
	m_dwSize = 0;

	//����ռ估��ʼ��
	m_dwSize = dwSize;
	T *pObjectSet = new T[dwSize];
	KG_PROCESS_ERROR(pObjectSet);

	m_HeadVector.push_back(pObjectSet);

    m_FreeVector.reserve(m_dwSize);
    m_FreeVector.resize(dwSize);

    T **ppDest = &m_FreeVector[0];
    T *pObjEnd = pObjectSet + dwSize;
	for (T *pObj = pObjectSet; pObj < pObjEnd; pObj++)
	{ 
        *ppDest++ = pObj;
	}

    nResult = true;
Exit0:

	return nResult;
}

template <class T>
inline T *KBaseObjLiteSet<T>::New()
{
    T *pResult = NULL;
    int nRetCode = false;

    nRetCode = m_FreeVector.empty();
	KGLOG_CHECK_ERROR(!nRetCode);
	if (nRetCode)
	{
		DWORD dwSize = m_dwSize;	// LiteSet�ռ䲻��ʱ��һ����С
		
		T *pObjectSet = new T[dwSize];
		KGLOG_PROCESS_ERROR(pObjectSet);

		m_dwSize += dwSize;

		m_HeadVector.push_back(pObjectSet);

        m_FreeVector.reserve(m_dwSize);
        m_FreeVector.resize(dwSize);

        T **ppDest = &m_FreeVector[0];
        T *pObjEnd = pObjectSet + dwSize;
	    for (T *pObj = pObjectSet; pObj < pObjEnd; pObj++)
	    { 
            *ppDest++ = pObj;
	    }
	}

	pResult = m_FreeVector.back();
	m_FreeVector.pop_back();

Exit0:
	return pResult;
}

template <class T>
inline BOOL KBaseObjLiteSet<T>::Delete(T *pObj)
{
    int nResult = false;

	KGLOG_PROCESS_ERROR(pObj);

	m_FreeVector.push_back(pObj);

	nResult = true;

Exit0:
	return nResult;
}

template <class T>
inline BOOL KBaseObjLiteSet<T>::NewArray(int nNewCount, T **ppRetArray)
{
    int nResult = false;
    int nRetCode = false;
    int nFreeCount = 0;
    
    KG_PROCESS_ERROR(nNewCount > 0);
    KG_PROCESS_ERROR(ppRetArray);

    nFreeCount = (int)m_FreeVector.size();
    if (nNewCount <= nFreeCount)
    {
        memcpy(ppRetArray, &m_FreeVector[nFreeCount - nNewCount], nNewCount * sizeof(T *));

        m_FreeVector.resize(nFreeCount - nNewCount);
        KG_PROCESS_SUCCESS(TRUE);
    }

    DWORD dwSize = nNewCount;

	T *pObjectSet = new T[dwSize];
	KGLOG_PROCESS_ERROR(pObjectSet);

	m_dwSize += dwSize;
	m_HeadVector.push_back(pObjectSet);
    m_FreeVector.reserve(m_dwSize);

    T *pObjEnd = pObjectSet + dwSize;
    for (T *pObj = pObjectSet; pObj < pObjEnd; pObj++)
    { 
	    *ppRetArray++ = pObj;
    }

Exit1:
    nResult = true;
Exit0:
	return nResult;
}

template <class T>
inline BOOL KBaseObjLiteSet<T>::DeleteArray(int nCount, T **ppArray)
{
    int nResult = false;

    KGLOG_PROCESS_ERROR(nCount > 0);
	KGLOG_PROCESS_ERROR(ppArray);

    m_FreeVector.insert(m_FreeVector.end(), ppArray, ppArray + nCount);

    nResult = true;

Exit0:
	return nResult;
}


template <class T>
inline DWORD KBaseObjLiteSet<T>::GetSize()
{
	return m_dwSize;
}

#endif	//_KBASEOBJLITESET_H_