/************************************************************************/
/* ���ݼ���                                                             */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu, Freeway Chen								*/
/* History	 :															*/
/*		2005.01.01	Create												*/
/*		2006.12.23	use vector for speed and space						*/
/* Comment	 :															*/
/*		����Ҫ��														*/
/*			1	����ID�����������										*/
/*			2	���ٱ���												*/
/*			3	����ɾ���Ͳ���											*/
/************************************************************************/
#ifndef _KBASEOBJECTSET_H_
#define _KBASEOBJECTSET_H_

#include <stdlib.h>
#include <hash_map>
#include <vector>
#include "SO3GlobalDef.h"
#include "Global.h"

using namespace std;
using namespace stdext;



template <class T>
class KBaseObjectSet
{
public:
	KBaseObjectSet(void);
	~KBaseObjectSet(void);

	//��ʼ������Ҫָ���������������ID��ǰ׺��ǰ׺��Relay���䣬������Ϸ����ȫ��Ψһ
	BOOL	Init(DWORD dwSize, DWORD dwPrefix);

	//�����¶���
	inline T *New(DWORD dwId = ERROR_ID);

	//�ͷŶ���
	inline BOOL Delete(T *pObj);

	//����ID��λObj
	inline T *GetObj(DWORD dwId);

	//�����ѷ����Obj
	template <class TFunc>
	inline BOOL Traverse(TFunc &Func);

	//��ȡ�洢����Ĵ�С
	inline DWORD GetSize(void);

	inline DWORD GetUsedSize(void);

private:
	//���飬�����洢
	DWORD					m_dwSize;
	//���б�
	typedef vector<T *>	    FREE_VECTOR, HEAD_VECTOR;
	FREE_VECTOR				m_FreeVector;
	HEAD_VECTOR				m_HeadVector;
	//ID�ֶε�Map
	typedef hash_map<DWORD, T *> MAP_ID_2_OBJ;	
	MAP_ID_2_OBJ			m_mapId2Obj;

	DWORD					m_dwIdGenerator;	//��һ��ʼ�����ʾ��Чֵ
												//32bit�У���4λ��Relay����
};

template <class T>
KBaseObjectSet<T>::KBaseObjectSet(void)
{
	m_dwIdGenerator = ERROR_ID;
	m_dwSize = 0;

	return;
}

template <class T>
KBaseObjectSet<T>::~KBaseObjectSet(void)
{
	//�ڴ�й©���
	ASSERT(m_mapId2Obj.size() == 0);
	ASSERT(m_FreeVector.size() == m_dwSize);

	HEAD_VECTOR::iterator	it;
    for (it = m_HeadVector.begin(); it != m_HeadVector.end(); ++it)
    {
		T *pHead = *it;
		KG_DELETE_ARRAY(pHead);
    }
	m_HeadVector.clear();

	m_FreeVector.clear();
	m_mapId2Obj.clear();

	return;
}

template <class T>
BOOL KBaseObjectSet<T>::Init(DWORD dwSize, DWORD dwPrefix)
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
	m_mapId2Obj.clear();
	m_FreeVector.clear();
	m_dwSize = 0;
	m_dwIdGenerator = ERROR_ID;

	//����Ĵ�С���ܳ���ID�����ֵ
	KGLOG_PROCESS_ERROR(dwSize < ((1 << (sizeof(m_dwIdGenerator) * 8 - ID_PREFIX_BIT_NUM)) - 1));

	//����ռ估��ʼ��
	m_dwSize = dwSize;
	T *pObjectSet = new T[dwSize];
	KGLOG_PROCESS_ERROR(pObjectSet);

	m_HeadVector.push_back(pObjectSet);
    m_FreeVector.reserve(m_dwSize);
    m_FreeVector.resize(dwSize);

    T **ppDest = &m_FreeVector[0];
    T *pObjEnd = pObjectSet + dwSize;
	for (T *pObj = pObjectSet; pObj < pObjEnd; pObj++)
	{ 
        pObj->m_dwID = ERROR_ID;
        *ppDest++ = pObj;
	}

	m_dwIdGenerator = (DWORD)(dwPrefix << (sizeof(m_dwIdGenerator) * 8 - ID_PREFIX_BIT_NUM)) + 1;

	nResult = true;
Exit0:
	return nResult;
}

template <class T>
T *KBaseObjectSet<T>::New(DWORD dwId/* = ERROR_ID*/)
{
    T *pResult   = NULL;
    T *pObj      = NULL;
    int nRetCode = false;
	pair<MAP_ID_2_OBJ::iterator, bool> InsRet;

    nRetCode = m_FreeVector.empty();
	KGLOG_CHECK_ERROR(!nRetCode);
	if (nRetCode)
	{
		DWORD dwSize = m_dwSize / 8;
		if (!dwSize)
			dwSize = 1;

		DWORD dwAllSize = m_dwSize + dwSize;
		
		KGLOG_PROCESS_ERROR(
			dwAllSize < ((1 << (sizeof(m_dwIdGenerator) * 8 - ID_PREFIX_BIT_NUM)) - 1));

		T *pObjectSet = new T[dwSize];
		KGLOG_PROCESS_ERROR(pObjectSet);

		m_dwSize = dwAllSize;

		m_HeadVector.push_back(pObjectSet);
        m_FreeVector.reserve(m_dwSize);
        m_FreeVector.resize(dwSize);

        T **ppDest = &m_FreeVector[0];
        T *pObjEnd = pObjectSet + dwSize;
	    for (T *pObj = pObjectSet; pObj < pObjEnd; pObj++)
	    { 
			pObj->m_dwID = ERROR_ID;
            *ppDest++ = pObj;
	    }
	}

	pObj = m_FreeVector.back();

	if (dwId == ERROR_ID)
		pObj->m_dwID = ++m_dwIdGenerator;
	else
		pObj->m_dwID = dwId;

	InsRet = m_mapId2Obj.insert(make_pair(pObj->m_dwID, pObj));
	KGLOG_PROCESS_ERROR(InsRet.second);

	m_FreeVector.pop_back();

    pResult = pObj;

Exit0:
	return pResult;
}

template <class T>
BOOL KBaseObjectSet<T>::Delete(T* pObj)
{
    int nResult = false;

	KGLOG_PROCESS_ERROR(pObj);
	KGLOG_PROCESS_ERROR(pObj->m_dwID != ERROR_ID);

	m_mapId2Obj.erase(pObj->m_dwID);
	
    pObj->m_dwID = ERROR_ID;
	m_FreeVector.push_back(pObj);

	nResult = true;

Exit0:
	return nResult;
}

template <class T>
inline DWORD KBaseObjectSet<T>::GetSize(void)
{
	return m_dwSize;
}

template <class T>
inline DWORD KBaseObjectSet<T>::GetUsedSize(void)
{
	return (DWORD)m_mapId2Obj.size();
}

template <class T>
T *KBaseObjectSet<T>::GetObj(DWORD dwId)
{
	MAP_ID_2_OBJ::iterator it = m_mapId2Obj.find(dwId);

	if (it == m_mapId2Obj.end())
		return NULL;

	return it->second;
}

template <class T>
template <class TFunc>
inline BOOL KBaseObjectSet<T>::Traverse(TFunc &Func)
{
	BOOL bRetCode = FALSE;

	for (
        MAP_ID_2_OBJ::iterator it = m_mapId2Obj.begin();
		it != m_mapId2Obj.end();
        NULL
    )
	{
		MAP_ID_2_OBJ::iterator itNext = it;
		++itNext;
		bRetCode = Func(it->first, it->second);
		KG_PROCESS_ERROR(bRetCode);
		it = itNext;
	}     

Exit0:

	return TRUE;
}

#endif	//_KBASEOBJECTSET_H_