/************************************************************************/
/* ���ID���������ݻ������ģ����                                       */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.04.21	Create												*/
/* Comment	 :															*/
/*		ԭ��Engine�е�Cache��������ַ��������ģ�����ĳ��˸���DWORD����*/
/*		Cache�ĸ��»����ǣ�Cacheδ��ʱ����ɾ���κ�����					*/
/*			Cache��ʱ������ɾ���ʱ��δʹ�õ�����						*/
/*		ע�⣺Cache�в����Դ洢ָ�룬��Ϊ��������ݿ������κ�ʱ��ɾ�� */
/************************************************************************/
#ifndef _KIDCACHE_H_
#define _KIDCACHE_H_

#include <stdlib.h>
#include <map>

class KIDCache
{
public:
	KIDCache(void);
	~KIDCache(void);

	//��ʼ������Ҫָ�����������
	BOOL Init(DWORD dwMaxSize);
	//�����¶���
	void* AddData(DWORD dwID, void* pData, DWORD dwSize);
	//����ID��Cache�в��Ҷ��󣬲鲻������NULL
	void* GetData(DWORD dwID, DWORD& rdwSize);
	//��ջ���
	BOOL Clear(void);
	BOOL LoadFromBuffer(char* pData, DWORD dwSize);
	BOOL SaveToBuffer(char* pData, DWORD dwSize);

private:
	struct KIDCacheNode : KNode
	{
		DWORD			m_dwID;
		DWORD			m_dwSize;
		char			m_szData[0];
	};

	DWORD				m_dwMaxSize;
	KList				m_CacheList;

	typedef std::map<DWORD, KIDCacheNode*>	MAP_ID_2_NODE;	
	MAP_ID_2_NODE		m_mapId2Node;
};

#endif	//_KIDCACHE_H_
