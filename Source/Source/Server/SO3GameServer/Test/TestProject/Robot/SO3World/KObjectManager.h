/************************************************************************/
/* ��Ϸ������������                                                   */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.03.30	Create												*/
/* Comment	 :															*/
/*		������ǣ���ڴ����Ķ��󣬶����е��������౾��ûʲô����	*/
/************************************************************************/
#ifndef _KOBJECT_MANAGER_H_
#define _KOBJECT_MANAGER_H_

#include <vector>
using namespace std;

#include "KBaseObjectSet.h"
#include "KBaseObjLiteSet.h"

#include "KAttribute.h"
#include "KCircleList.h"


#include "KNpc.h"
#include "KPlayer.h"
#include "KDoodad.h"

#include "KItem.h"
#include "KLootList.h"

#include "KThreatList.h"

//#include "KSkill.h"
//#include "KBuffList.h"

class KScene;
class KRegion;
class KNpc;
class KPlayer;
class KItem;
class KDoodad;


class KObjectManager
{
public:
	KObjectManager(void);
	~KObjectManager(void);

	BOOL Init(int nWorldIndex);
//����
	KBaseObjectSet<KScene>				m_SceneSet;
	KBaseObjLiteSet<KRegion>			m_RegionSet;
	KBaseObjLiteSet<KCell>				m_CellSet;
//��������
	//KBaseObjectSet<KPlayer>				m_PlayerSet;
	//KBaseObjectSet<KNpc>				m_NpcSet;
	//KBaseObjectSet<KDoodad>				m_DoodadSet;
//����
	KBaseObjectSet<KItem>				m_ItemSet;
//����
	KBaseObjLiteSet<KLootList>			m_LootListSet;
	KBaseObjLiteSet<KAttribute>			m_AttributeSet;
	
	//����ֵ�ĳ�޽ڵ�
	KBaseObjLiteSet<KThreatLiteNode>	m_ThreatLiteNodeSet;

#if defined(_SERVER)
	KBaseObjLiteSet<KThreatNode>		m_ThreatNodeSet;
    KBaseObjLiteSet<KSKILL_BULLET>      m_BulletSet;
#endif	//_SERVER

};

extern KObjectManager g_ObjectManager;

// ---------------- ��������� ------------------------------>
// �ڷ����,��ͼ�ļ������첽���е�,����,�����߳�֪ͨ�����߳������ص�ʱ��,��Ҫ������Ԥ����(�Ա���Է���������)
// �����ڿͻ����Լ��༭������,Ԥ�����ǲ���Ҫ��,��������������Ŀ����Ϊ��ͳһ�ͻ���/������������䷽ʽ��ͬ��
// �����Ľӿڲ���.
// ʹ��Ԥ�����ʱ��,ע����һ��Ԥ����Ķ����������Ƿ�ǡ�ù���,���˻������˶��ǲ��Ե�

struct KObjAlloctor
{
    virtual KNpc *NewServerNpc(DWORD dwId = ERROR_ID) { return NULL; };
    virtual KNpc *NewClientNpc(DWORD dwId = ERROR_ID) { return NULL; };

    virtual KDoodad *NewServerDoodad(DWORD dwId = ERROR_ID) { return NULL; }
    virtual KDoodad *NewClientDoodad(DWORD dwId = ERROR_ID) { return NULL; }

    virtual KCell *NewCell() { return NULL; };

    virtual void DeleteServerNpc(KNpc *pNpc) {};
    virtual void DeleteClientNpc(KNpc *pNpc) {};

    virtual void DeleteServerDoodad(KDoodad *pDoodad) {};
    virtual void DeleteClientDoodad(KDoodad *pDoodad) {};

    virtual void DeleteCell(KCell *pCell) {};
};

//class KServerObjAlloctor : public KObjAlloctor
//{
//public:
//    BOOL Init(int nNpcCount, int nDoodadCount, int nCellCount);        
//    void UnInit();
//
//public:
//
//    KNpc* NewServerNpc(DWORD dwId);
//
//    KDoodad *NewServerDoodad(DWORD dwId);
//
//    KCell *NewCell();
//
//    void DeleteServerNpc(KNpc *pNpc);
//
//    void DeleteServerDoodad(KDoodad *pDoodad);
//
//    void DeleteCell(KCell *pCell);
//
//private:
//    vector<KNpc *>      m_NpcVector;
//    vector<KDoodad *>   m_DoodadVector;
//    vector<KCell *>     m_CellVector;
//};

class KClientObjAlloctor : public KObjAlloctor
{
public:

//    KNpc *NewServerNpc(DWORD dwId) { return g_ObjectManager.m_NpcSet.New(dwId); };
 //   KNpc *NewClientNpc(DWORD dwId) { return g_ObjectManager.m_NpcSet.New(dwId); };

//    KDoodad *NewServerDoodad(DWORD dwId) { return g_ObjectManager.m_DoodadSet.New(dwId); };
//    KDoodad *NewClientDoodad(DWORD dwId) { return g_ObjectManager.m_DoodadSet.New(dwId); };

   KCell *NewCell() { return g_ObjectManager.m_CellSet.New(); };

    //void DeleteServerNpc(KNpc *pNpc) { g_ObjectManager.m_NpcSet.Delete(pNpc); };
    //void DeleteClientNpc(KNpc *pNpc) { g_ObjectManager.m_NpcSet.Delete(pNpc); };

//    void DeleteServerDoodad(KDoodad *pDoodad) { g_ObjectManager.m_DoodadSet.Delete(pDoodad); };
//    void DeleteClientDoodad(KDoodad *pDoodad) { g_ObjectManager.m_DoodadSet.Delete(pDoodad); };

    //void DeleteCell(KCell *pCell) { g_ObjectManager.m_CellSet.Delete(pCell); };
};


// <---------------- ��������� ------------------------------

#endif	//_KOBJECT_MANAGER_H_