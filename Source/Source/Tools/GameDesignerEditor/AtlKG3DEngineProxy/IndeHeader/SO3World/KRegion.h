/************************************************************************/
/* ��ͼ�ϵ�Region						                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.30	Create												*/
/* Comment	 :															*/
/*		��װ�˵�ͼ��һ��Region�����ݺͲ���								*/
/************************************************************************/
#ifndef _KREGION_H_
#define	_KREGION_H_

#include "Global.h"
#include "KCell.h"
#include "../../Source/Common/SO3World/Src/KSceneObjNode.h"

#define SCRIPT_COUNT_PER_REGION		16
#define SCRIPT_DATA_SIZE (sizeof(DWORD) * SCRIPT_COUNT_PER_REGION)

class KSceneObject;
class KScene;
class KPlayer;
class KNpc;
class KDoodad;
class KMissile;
class KCharacter;

class KRegion
{
public:
	KRegion();
	~KRegion();

	BOOL Init(KScene* pScene, int nRegionX, int nRegionY);
    void UnInit();

	void Activate(void);

    BOOL LoadTerrain();
private:
    BOOL LoadTerrainBufferV7(const BYTE* pbyData, size_t uDataLen);
    BOOL LoadTerrainBufferV6(const BYTE* pbyData, size_t uDataLen);
    
public:
	BOOL SaveTerrain();
private:
    BOOL CheckUpdate(const char cszFileName[], BYTE* pbyNewData, size_t uNewDataLen);

public:
	// ��Region�е����½Ǹ��ӣ���������ͼ�ϵ����꣨��λ�����ӣ�
	int					m_nLeftCell;
	int					m_nBottomCell;

	// ��Region�е����½Ǹ��ӵ����µ㣬��������ͼ�ϵ����꣨��λ���㣩
	int					m_nLeft;
	int					m_nBottom;

	// ��Region��������ͼ�ϵ����꣨��λ��Region��
	int				    m_nRegionX;
	int				    m_nRegionY;

	KScene*				m_pScene;										
    KCell*              m_pCells;
	DWORD*			    m_pdwScriptList;
    BOOL                m_bRegionView;
	int					m_nActiveFrame;

    int                 m_nInFightPlayerCount;

    void SetActive();

    BOOL IsActive();

	// ����ϰ�
	BOOL AddObstacle(int nXCell, int nYCell, KCell* pCell);
	BOOL AddObstacle(int nXCell, int nYCell, const KCell& crCellInfo, int nDynamicObstacleGroupIndex);
	// ɾ���ϰ�
	KCell* RemoveObstacle(int nXCell, int nYCell, int nZ);

    BOOL GetBaseCellInfo(int nXCell, int nYCell, KCell* pCell);
    BOOL SetBaseCellInfo(int nXCell, int nYCell, KCell& rCell);

    BOOL MergeObstacleData();

    void MergeCellList(KCell* pHead);
    BOOL ProcessDynamicCell(KCell* pHead, const KDynamicCell* pcCell);

    BOOL SetWaterHeight(int nXCell, int nYCell, WORD wWaterHeight);

	// ��ȡ�ϰ�: �±��� <= nZ <= �ϱ���
	KCell* GetObstacle(int nXCell, int nYCell, int nZ);

    KCell* GetLowestObstacle(int nXCell, int nYCell);

    // ��ȡZ�����µ�һ�����ڻ����nZ��Cell, nZ: ��������,��λ:��
	KCell* GetLowerObstacle(int nXCell, int nYCell, int nZ);

    // ��pnRelativeZ����ɹ����Z���������,���û�гɹ�,�򲻻��޸�*pnRelativeZ
    // �������Ŀ��Cell���ϰ����޷�����,��*pnObstacleDirection�����ϰ��ķ��߷���
    // ע��,������һ���ܹ��õ��ϰ����߷���,����Ҫ�ڵ��˺���ǰ��*pnObstacleDirection��Ϊ-1
    // ���ú�,�������CellΪ��,����*pnObstacleDirection��[0, DIRECTION_COUNT]��,��*pnObstacleDirection�ű�ʾ�ϰ�����
    KCell* GetAdaptedCellForObj(
        int nXCell, int nYCell, int nFootPosZ, int nClimb, 
        int nHeight, BOOL bIgnoreDynamicSwitch,
        int* pnRelativeZ, int *pnObstacleDirection
    );

    // ע��,����ĺ����ϸ����߶�,���������������Ѱ��һ����ӽ���Cell,�����ϸ�ƥ��߶�

    // ��pnRelativeZ����ɹ����Z���������,���û�гɹ�,�򲻻��޸�*pnRelativeZ
    KCell* GetProximalCellForObj(
        int nXCell, int nYCell, int nFootPosZ, int nHeight, 
        BOOL bIgnoreDynamicSwitch, 
        BOOL bIgnoreBlock, 
        int* pnRelativeZ
    );

    // ȡ��Cell�����е���һ��Cell,�ڲ��ᶯ̬�ϰ��Ŀ���
    KCell* GetPreCell(int nCellX, int nCellY, KCell* pCell);
    // ȡ����һ����Ч��Cell,�ڲ��ῼ�Ƕ�̬�ϰ�����
    KCell* GetNextCell(KCell* pCell, BOOL bIgnoreDynamicSwitch);

	BOOL CheckObstacle(int nXCell, int nYCell, int nLayer);

public:
	DWORD GetScriptIDByIndex(DWORD dwIndex);

	BOOL SetScriptID(KCell* pCell, DWORD dwScriptID);

	int	RecycleScriptIndex();

    BOOL ClearAllScript();
    BOOL ClearScriptSpecial(DWORD dwScriptID);

public:
	BOOL AddNpc(KNpc* pNpc);
	BOOL AddDoodad(KDoodad* pDoodad);
	BOOL AddPlayer(KPlayer* pPlayer);

	BOOL RemoveNpc(KNpc* pNpc);
	BOOL RemoveDoodad(KDoodad* pDoodad);
	BOOL RemovePlayer(KPlayer* pPlayer);

	// ����Region�еĳ�������
	template <class TFunc> BOOL TraverseNpc(TFunc& Func);
	template <class TFunc> BOOL TraverseDoodad(TFunc& Func);
	template <class TFunc> BOOL TraversePlayer(TFunc& Func);
	// ����Region�е�Cell
	template <class TFunc> BOOL TraverseCell(TFunc& Func);

	BOOL DeleteAllNpc(void);
	BOOL DeleteAllDoodad(void);
	BOOL DeleteAllPlayer(void);
	BOOL DeleteAllObject(void);
	BOOL DeleteAllCell(void);

public:
	KList			m_NpcList;			
	KList			m_DoodadList;		
	KList			m_PlayerList;		

private:
	enum KTERRAIN_DATA_VERSION
	{
        eTerrainDataVersion7 = 7
	};

	struct KRegionHeader
	{
		int nVersion;
		int nRegionX;
		int nRegionY;
		int nReserved;
	};

	BOOL LoadRegionData(void);

#if defined(_CLIENT)
	KCell   m_Cells[REGION_GRID_WIDTH * REGION_GRID_HEIGHT];	
    DWORD   m_dwScriptList[SCRIPT_COUNT_PER_REGION];
#endif
};

// ------------------------ ����Region�еĳ������� ----------------------------------->

template <class TFunc> 
BOOL KRegion::TraverseNpc(TFunc& Func)
{
    BOOL bResult  = false;
    BOOL bRetCode = false;

	KSceneObjNode* pNode = (KSceneObjNode*)m_NpcList.GetHead();
	while (pNode != NULL)
	{
		KNpc* pNpc = (KNpc*)(pNode->m_pSceneObject);

		bRetCode = Func(pNpc);
		KG_PROCESS_ERROR(bRetCode);

		pNode = (KSceneObjNode*)pNode->GetNext();
	}

    bResult = true;
Exit0:
	return bResult;
}

template <class TFunc> 
BOOL KRegion::TraverseDoodad(TFunc& Func)
{
    BOOL bResult  = false;
    BOOL bRetCode = false;

	KSceneObjNode* pNode = (KSceneObjNode*)m_DoodadList.GetHead();
	while (pNode != NULL)
	{
		KDoodad* pDoodad = (KDoodad*)(pNode->m_pSceneObject);

		bRetCode = Func(pDoodad);
		KG_PROCESS_ERROR(bRetCode);

		pNode = (KSceneObjNode*)pNode->GetNext();
	}

    bResult = true;
Exit0:
	return bResult;
}

template <class TFunc> 
BOOL KRegion::TraversePlayer(TFunc& Func)
{
    BOOL bResult  = false;
    BOOL bRetCode = false;

	KSceneObjNode* pNode = (KSceneObjNode*)m_PlayerList.GetHead();
	while (pNode != NULL)
	{
		KPlayer* pPlayer = (KPlayer*)(pNode->m_pSceneObject);

		bRetCode = Func(pPlayer);
		KG_PROCESS_ERROR(bRetCode);

		pNode = (KSceneObjNode*)pNode->GetNext();
	}

    bResult = true;
Exit0:
	return bResult;
}

template <class TFunc>
BOOL KRegion::TraverseCell(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

    for (int nY = 0; nY < REGION_GRID_HEIGHT; nY++)
    {
        for (int nX = 0; nX < REGION_GRID_WIDTH; nX++)
        {
            KCell* pCell = GetLowestObstacle(nX, nY);
	        while (pCell != NULL)
	        {
		        bRetCode = Func(pCell);
		        KG_PROCESS_ERROR(bRetCode); // ����false���򲻼�������

		        pCell = pCell->m_pNext;
	        }
        }
    }

	bResult = true;
Exit0:
	return bResult;
}

// <-------------------------- ����Region�еĳ������� -----------------------------------

// ����Region�е�����Player,֪ͨ"����Npc"
struct KRegisterNewNpcFunc 
{
    KRegisterNewNpcFunc(KNpc* pNpc) : m_pNpc(pNpc) {};
    BOOL operator () (KPlayer* pPlayer);
    KNpc* m_pNpc;
};

// ����Region�е�����Player,֪ͨ"����Doodad"
struct KRegisterNewDoodadFunc
{
    KRegisterNewDoodadFunc(KDoodad* pDoodad) : m_pDoodad(pDoodad) {};
    BOOL operator () (KPlayer* pPlayer);
    KDoodad* m_pDoodad;
};

#endif	//_KREGION_H_

