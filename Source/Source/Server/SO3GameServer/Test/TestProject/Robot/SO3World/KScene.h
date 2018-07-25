/************************************************************************/
/* ��ͼ��								                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.31	Create												*/
/* Comment	 :															*/
/*		��װ��һ����Ϸ�����е�ͼ�����в���������						*/
/************************************************************************/
#ifndef _KSCENE_H_
#define	_KSCENE_H_

#include <list>
#include <map>
#include <string>
#include "Engine/KList.h"
#include "Global.h"
#include "KBaseObject.h"
#include "KCell.h"
#include "Luna.h"
#include "Engine/KThread.h"

class KRegion;
class KScene;
class KNpc;
class KDoodad;
class KObjAlloctor;
class KServerObjAlloctor;
struct KREVIVE_POSITION;

enum SCENE_STATE
{
	ssInvalid = 0,			//��Ч״̬
	ssLoading,				//����������
	ssLoaded,				//��������
	ssLoadFailed,			//����ʧ��
	ssNormal,				//����״̬
	ssDeleting,				//��ȫɾ����
	ssDeleted,				//��ɾ��

	ssTotal
};


enum RESIZE_SCENE_ALIGN_TYPE
{
	rsatInvalid = 0,

	rsatLeftTop,
	rastLeftBottom,
	rastRightTop,
	rastRightBottom,

	rastTotal
};

class KScene : public KBaseObject
{
//�ⲿ�ӿں���
public:
	// �趨�ر������Ϣ
	BOOL SetCellData(int nXCell, int nYCell, KCell::KBaseInfo* pBaseInfo, WORD wLowLayer, WORD wHighLayer);
	// ��ȡ�ر������Ϣ
	BOOL GetCellData(int nXCell, int nYCell, KCell::KBaseInfo& BaseInfo, WORD& wLowLayer, WORD& wHighLayer);


	BOOL NewObstacle(int nXCell, int nYCell, KCell::KBaseInfo* pBaseInfo, WORD wLowLayer, WORD wHighLayer);
	BOOL DelObstacle(int nXCell, int nYCell, int nZ);
	BOOL GetObstacle(int nXCell, int nYCell, int nZ, KCell::KBaseInfo& BaseInfo, WORD& wLowLayer, WORD& wHighLayer);
	BOOL CheckObstacle(int nXCell, int nYCell, int nLayer);

	DWORD GetScriptID(int nXCell, int nYCell);
//	BOOL SetScriptID(int nXCell, int nYCell, DWORD dwScriptID);


	// Ԥ�����ϰ�����
	typedef void (*PREPROCESS_PROGRESS_CALLBACK_FUNC)(int nYCell, int nCellWidth, int nCellHeight);
    BOOL PreprocessCalculateData();

private:
    BOOL ReviseCalculateGradient();
	BOOL CalculateObstacleRange();

public:
    // �ɼ��Լ��,�ϰ����,�������֮���Ƿ�ɼ�
    // X, Y����Ϊ"��", Z����ΪAltitude,�����⵽�ϰ��򷵻�false,���򷵻�true
    BOOL IsVisible(int nSrcX, int nSrcY, int nSrcZ, int nDstX, int nDstY, int nDstZ);
    
    BOOL IsReachable(int nSrcX, int nSrcY, int nSrcZ, int nDstX, int nDstY, int nDstZ, int nHeight);


public:
	DWORD							m_dwMapID;						//��ͼ���
	DWORD							m_dwMapCopyIndex;				//�������
	SCENE_STATE						m_eSceneState;					//��ͼ״̬
	DWORD							m_dwCreatorID;					//��ͼ�����Ĵ����ߵ�ID
	char							m_szName[_NAME_LEN];			//��ͼ����
    BOOL                            m_bReviveInSitu;                //����Ƿ�����ԭ�ظ���
	DWORD							m_dwScriptID;					//��ͼ�ű�ID


public:
	KScene();
	virtual ~KScene();

	BOOL Init(void);
    void UnInit();

    BOOL PreLoadScene();
    BOOL ClientLoadRegion(int nRegionX, int nRegionY);

	char m_szFilePath[MAX_PATH];



	BOOL SetRegionWidth(int nWidth);
	BOOL SetRegionHeight(int nHeight);
	int GetRegionWidth(void);
	int GetRegionHeight(void);

	BOOL SetRegion(int nRegionX, int nRegionY, KRegion* pRegion);
	KRegion* GetRegion(int nRegionX, int nRegionY);

	// �����ص�����õ�Regionָ��
	KRegion* GetRegionByPoint(int nX, int nY);
	BOOL DelRegion(int nRegionX, int nRegionY);
	BOOL DeleteAllRegion(void);
	BOOL DeleteAllObstacle(void);

	//NPC��غ���
	BOOL AddNpc(KNpc* pNpc);
	BOOL RemoveNpc(KNpc* pNpc);
    BOOL MoveNpc(KNpc* pNpc, int nX, int nY, int nZ);

	// ����װ������غ���
	BOOL AddDoodad(KDoodad* pDoodad);
	BOOL RemoveDoodad(KDoodad* pDoodad);
	BOOL MoveDoodad(KDoodad* pDoodad, int nX, int nY, int nZ = 0);

	//�����غ���
	BOOL AddPlayer(KPlayer* pPlayer);
	BOOL RemovePlayer(KPlayer* pPlayer);


//˽������
private:
	KRegion*						m_RegionList[MAX_REGION_WIDTH][MAX_REGION_HEIGHT];
	int							    m_nRegionWidth;		//Region��X�����ϵĸ���
	int  							m_nRegionHeight;	//Region��Y�����ϵĸ���

    KMutex m_Lock;

	friend class KSO3World;
	friend class KCharacter;

public:
	// �ű��ӿں���
	DECLARE_LUA_CLASS(KScene);

	DECLARE_LUA_STRING(Name, _NAME_LEN);
	DECLARE_LUA_DWORD(MapID);
	DECLARE_LUA_DWORD(ID);
	DECLARE_LUA_DWORD(MapCopyIndex);
	DECLARE_LUA_ENUM(SceneState);
    DECLARE_LUA_BOOL(ReviveInSitu);
};

//�õ�ȫ�����ص��������ڵ�Regionָ��
inline KRegion* KScene::GetRegionByPoint(int nX, int nY)
{
	KRegion*    pRegion     = NULL;
	int         nRegionX    = nX / REGION_GRID_WIDTH / CELL_LENGTH;
	int         nRegionY    = nY / REGION_GRID_HEIGHT / CELL_LENGTH;

	KG_PROCESS_ERROR(nRegionX >= 0 && nRegionX < m_nRegionWidth);
	KG_PROCESS_ERROR(nRegionY >= 0 && nRegionY < m_nRegionHeight);

	pRegion = m_RegionList[nRegionX][nRegionY];

Exit0:
	return pRegion;
}


#endif