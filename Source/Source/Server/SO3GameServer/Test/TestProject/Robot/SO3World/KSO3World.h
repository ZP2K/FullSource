/************************************************************************/
/* ������Ϸ����							                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.31	Create												*/
/* Comment	 :															*/
/*		�������е�ȫ�����ݵ��������͵�ͼ�ļ���							*/
/************************************************************************/
#ifndef _KSO3WORLD_H_
#define _KSO3WORLD_H_


#include "Engine/KMutex.h"
#include "Global.h"
#include "KCircleList.h"

class KScene;
class KNpc;
class KPlayer;
class KDoodad;
class KSceneDBLoader;

class KSO3World
{
public:
	KSO3World(void);
	~KSO3World(void);

	BOOL Init(int nWorldIndex);
	BOOL UnInit(void);

    BOOL PreLoadScene(DWORD dwMapID, DWORD dwMapCopyIndex);
    KScene* GetScene(DWORD dwMapID, DWORD dwMapCopyIndex);
	BOOL DeleteScene(KScene* pScene, BOOL bDirectDel);	

	BOOL AddNpc(KNpc* pNpc, KScene* pScene, int nX, int nY, int nZ);
	BOOL RemoveNpc(KNpc* pNpc);

	BOOL AddDoodad(KDoodad* pDoodad, KScene* pScene, int nX, int nY, int nZ);
	BOOL RemoveDoodad(KDoodad* pDoodad);

	BOOL AddPlayer(KPlayer* pPlayer, KScene* pScene, int nX, int nY, int nZ);
	BOOL RemovePlayer(KPlayer* pPlayer);

    BOOL MovePlayer(KPlayer* pPlayer, DWORD dwMapID, int nX, int nY, int nZ);

    int m_nPreLoadScene;

public:
    time_t								m_CurrentTime;			//��ǰʱ�䣬ÿ����Ϸѭ������
private:
    int									m_nWorldIndex;

private:
    KScene* NewClientScene(DWORD dwMapID, DWORD dwMapCopyIndex);

private:
    KMutex m_Lock;

	struct KGetSceneFunc
	{
		KGetSceneFunc(void)
		{
			m_dwMapID = m_dwMapCopyIndex = 0;
			m_pScene = NULL;
		}
		//�����ص�����
		BOOL operator()(DWORD dwID, KScene* pScene);
		//ƥ������
		DWORD		m_dwMapID;
		DWORD		m_dwMapCopyIndex;
		//����ֵ
		KScene*		m_pScene;
	};

	struct KDeleteSceneFunc
	{
		BOOL operator()(DWORD dwID, KScene* pScene);
	};
};

extern KSO3World g_SO3World;

#endif	//_KSO3WORLD_H_