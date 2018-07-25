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

#define SCRIPT_COUNT_PER_REGION		16

class KSceneObject;
class KScene;
class KPlayer;
class KNpc;
class KDoodad;
class KMissile;
class KCharacter;
class KObjAlloctor;

class KRegion
{
public:
	KRegion();
	~KRegion();

	BOOL Init(KScene* pScene, int nRegionX, int nRegionY);
    void UnInit();

    BOOL ClientLoad();

private:

    BOOL LoadTerrainData(const char cszFileName[], KObjAlloctor& rObjectAlloctor);
    BOOL LoadObjectsData(const char cszFileName[], KObjAlloctor& rObjectAlloctor);

private:

public:
	//��Region�е����½Ǹ��ӣ���������ͼ�ϵ����꣨��λ�����ӣ�
	int					m_nLeftCell;
	int					m_nBottomCell;

	//��Region�е����½Ǹ��ӵ����µ㣬��������ͼ�ϵ����꣨��λ���㣩
	int					m_nLeft;
	int					m_nBottom;

	//��Region��������ͼ�ϵ����꣨��λ��Region��
	int				    m_nRegionX;
	int				    m_nRegionY;

	KScene*				m_pScene;										//ָ��������Scene
	KRegion*			m_pConnectRegion[CONNECTED_REGION_TOTAL];		//���ڵĵ�ͼָ��
	KCell				m_Cells[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];	//����
	DWORD				m_dwScriptList[SCRIPT_COUNT_PER_REGION];

	int					m_nActive;										//��Ҹ���

	//����ϰ�
	BOOL AddObstacle(int nXCell, int nYCell, KCell* pCell);
	BOOL AddObstacle(int nXCell, int nYCell, KCell::KBaseInfo* pBaseInfo, WORD wLowLayer, WORD wHighLayer);
	// ɾ���ϰ�
	KCell* RemoveObstacle(int nXCell, int nYCell, int nZ);

    BOOL GetBaseCellInfo(int nXCell, int nYCell, KCell* pCell);
    BOOL SetBaseCellInfo(int nXCell, int nYCell, KCell& rCell);

    BOOL MergeObstacleData();

    BOOL SetWaterHeight(int nXCell, int nYCell, WORD wWaterHeight);

	// ��ȡ�ϰ�: �±��� <= nZ <= �ϱ���
	KCell* GetObstacle(int nXCell, int nYCell, int nZ);

    KCell* GetLowestObstacle(int nXCell, int nYCell);
    KCell* GetTopmostObstacle(int nXCell, int nYCell);

    // ��ȡZ�����µ�һ�����ڻ����nZ��Cell, nZ: ��������,��λ:��
	KCell* GetLowerObstacle(int nXCell, int nYCell, int nZ);

    // ��pnRelativeZ����ɹ����Z���������,���û�гɹ�,�򲻻��޸�*pnRelativeZ
    // �������Ŀ��Cell���ϰ����޷�����,��*pnObstacleDirection�����ϰ��ķ��߷���
    // ע��,������һ���ܹ��õ��ϰ����߷���,����Ҫ�ڵ��˺���ǰ��*pnObstacleDirection��Ϊ-1
    // ���ú�,�������CellΪ��,����*pnObstacleDirection��[0, DIRECTION_COUNT]��,��*pnObstacleDirection�ű�ʾ�ϰ�����
    KCell* GetAdaptedCellForObj(
        int nXCell, int nYCell, int nFootPosZ, int nClimb, int nHeight, 
        int* pnRelativeZ, int *pnObstacleDirection
    );
    // ��pnRelativeZ����ɹ����Z���������,���û�гɹ�,�򲻻��޸�*pnRelativeZ
    KCell* GetProximalCellForObj(int nXCell, int nYCell, int nFootPosZ, int nHeight, int* pnRelativeZ);

	BOOL CheckObstacle(int nXCell, int nYCell, int nLayer);

	DWORD GetScriptID(int nXCell, int nYCell);
	DWORD GetScriptIDByIndex(DWORD dwIndex);
	BOOL SetScriptID(KCell* pCell, DWORD dwScriptID);
	BOOL SetScriptName(int nXCell, int nYCell, char* pszScriptName);

//�ڲ��ӿں���
public:

	int	RecycleScriptIndex();

	//����Ĳ���������,ֻ���ۼ�
	BOOL GetNpcCount(int* pnServerCount, int* pnClientCount);
	//����Ĳ���������,ֻ���ۼ�
	BOOL GetDoodadCount(int* pnServerCount, int* pnClientCount);

	inline BOOL	IsActive() {return m_nActive;};

	BOOL AddNpc(KNpc* pNpc);
	BOOL AddDoodad(KDoodad* pDoodad);
	BOOL AddPlayer(KPlayer* pPlayer);

	BOOL RemoveNpc(KNpc* pNpc);
	BOOL RemoveDoodad(KDoodad* pDoodad);
	BOOL RemovePlayer(KPlayer* pPlayer);

	BOOL DeleteAllCell(void);

//�洢���
private:
	enum MAP_FILE_VERSION
	{
		mfvVersion1_0_0 = 1,
		mfvVersion1_0_1,		// ��������
		mfvVersion1_0_2,		// ScriptID�洢��ʽ����
		mfvVersion1_0_3,		// �ڴ洢�ṹ�м���ID,��֧��Client only�Ķ���
		mfvCurrentVersion,
	};

	struct KRegionHeader
	{
		int nVersion;
		int nRegionX;
		int nRegionY;
		int nReserved;
	};

	struct KObjHeader
	{
		int nVersion;
		int nRegionX;
		int nRegionY;
		DWORD dwNpcTotal;
		DWORD dwDoodadTotal;
		int nReserved;
	};

	BOOL LoadRegionData(void);

#if defined(_CLIENT)
	BOOL LoadNpcDoodad(void);
#endif	//_CLIENT


};
#endif	//_KREGION_H_

