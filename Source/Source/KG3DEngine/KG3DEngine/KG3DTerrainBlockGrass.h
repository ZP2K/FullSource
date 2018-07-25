#pragma once

#include "KG3DTemplates.h"
#define  GRASS_NUM  16  //ÿ��TerrainBlock�ϵĲ���Ŀ��GRASS_NUM * GRASS_NUM
class KG3DTexture;
class KG3DMemoryFile;
class KG3DTerrainBlock;
class KG3DSceneSceneEditor;
class KG3DSceneOutDoorSpaceMgr;
struct KG3DModelSubsetInfo;

struct KG3DTerrainGrassVertexSrc
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vUV;
};
struct KG3DTerrainGrassVertex
{
	D3DXVECTOR3 vPos;
    D3DCOLOR    dwNormal;
    D3DCOLOR    dwUV;
};

class KG3DTerrainGrassPattern
{
public:
	string m_szName;
    KG3DTerrainGrassVertex * m_lpVertexBuffer;
	DWORD m_dwNumFace;
	DWORD m_dwNumVertex;

	HRESULT Init();
	HRESULT UnInit();

	HRESULT CreateFromMesh(LPCTSTR pFileName);

	KG3DTerrainGrassPattern();
	~KG3DTerrainGrassPattern();
};

struct KG3DTerrainGrass 
{
	D3DCOLOR dwColor;
	D3DXVECTOR3 vPosition;//λ��
	D3DXVECTOR3 vNormal;
	float fWidth;
	float fHeight;
	BYTE  byteIndex;//��ͼ�еĵڼ���
	BYTE  bytePatternIndex;
};

class KG3DTerrainBlockGrass;

class KG3DTerrainGrassSet
{
protected:
	ULONG m_ulRefCount;

public:
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	string m_strTextureFileName;
	KG3DTexture* m_lpGrassTexture;

	DWORD  m_dwStrideX;
	DWORD  m_dwStrideY;

	HRESULT Init();
	HRESULT UnInit();

	HRESULT SetTexture(LPCSTR strFileName);

	HRESULT LoadPattern(LPCTSTR pFileName,int& nIndex);
	KG3DTerrainGrassPattern* FindPattern(int nIndex);
	vector<KG3DTerrainGrassPattern*>m_vecGrassPattern;
	
	HRESULT SaveToInformationBuffer(DWORD& dwLength,KG3DMemoryFile* pFile);
	HRESULT LoadFromInformationBuffer(DWORD dwLength,KG3DMemoryFile* pFile);

	KG3DTerrainGrassSet();
	~KG3DTerrainGrassSet();
};

#define MAX_GRASSVERTEX 1024*1024*4    //ÿ�������ϵĶ�����Ŀ���4M��С

//extern KG3DTerrainGrassVertex g_zGrassVertexBuffer[MAX_GRASSVERTEX];

struct KG3DTerrainBlockGrassShareVB_High
{
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	DWORD m_dwNumFacesTotal;
	DWORD m_dwUsedFaces;

	DWORD dwLastTime;
	PVOID pUser;
	BOOL  m_bUserLock;
	BOOL  m_bOkToRender;
	list <KG3DTerrainBlockGrassShareVB_High*>::iterator nPosition;

	KG3DTerrainBlockGrassShareVB_High();
	~KG3DTerrainBlockGrassShareVB_High();

	void GetUVFromIndex(DWORD dwTexIndex,D3DXVECTOR3& vec2UV,
		DWORD m_dwStrideX,DWORD m_dwStrideY);
	
	HRESULT FillGrassVertex(DWORD& dwFaceCount,KG3DTerrainBlockGrass* pGrass,KG3DTerrainGrassVertex* pVertex);
	HRESULT FillXMesh(DWORD& dwPosition,LPD3DXMESH pXMesh,D3DXMATRIX* pMat);
	/*HRESULT FillXMeshSubset(DWORD& dwPosition,LPD3DXMESH pXMesh,DWORD dwSubSetID,D3DXMATRIX* pMat);*/

	HRESULT CheckSize(DWORD dwFace);

	HRESULT Create(KG3DTerrainBlockGrass* pBlock = NULL);

	HRESULT CreateFromModels(DWORD dwTotalFace,vector<KG3DModel*>*pvecModel,PVOID pCallUser);
	//HRESULT CreateFromModels(DWORD dwTotalFace,list<KG3DModelSubsetInfo*>&listSubsetToBuffer,PVOID pCallUser);

	HRESULT UnInit();
	HRESULT Init();

	HRESULT OnLostDevice();
	HRESULT OnResetDevice();

	DWORD   GetResourceUse()
	{
		return m_dwNumFacesTotal * sizeof(KG3DTerrainGrassVertex) * 3;
	}
};

class KG3DTerrainBlockGrass
{
	friend class KG3DTerrainBlockModifier;
	friend struct KG3DTerrainBlockGrassShareVB_High;
	friend class KG3DTerrainBlock;

protected:
	KG3DTerrainBlockGrassShareVB_High* m_lpShareGrassVB;	
	DWORD m_lpAutoGrassData[GRASS_NUM * GRASS_NUM];//�Զ����ɵĲ�
	float m_lpAutoGrassWidth[GRASS_NUM * GRASS_NUM];//�ݵĿ��
	float m_lpAutoGrassHeight[GRASS_NUM * GRASS_NUM];//�ݵĸ߶�
	BOOL m_bAutoGrass;
	
	DWORD m_dwBlockType; //��ʶ��BLOCK�ǲݣ�ʯͷ�����ǹ�ľ
	/////////////////////////////////////////////////////
	KG3DTerrainGrassSet* m_lpGrassSet;
	UINT  m_uHandleGrassSet;
	DWORD m_dwNumGrass; //ֻ���ڼ�¼��������ñ�ˢˢ��ȥ�Ĳ���Ŀ�������ر�������Ĳݲ��ڼ����ڣ�
	KG3DTerrainGrass* m_lpGrassData;

	DWORD m_dwNumGrassFace;

	list<KG3DTerrainGrass> m_listGrass; //�������ɾ���ݲ�������Ų����ݣ�ֻ���ڱ༭���оɵĲ�����õ�


	BOOL m_bNeedRefreshGrass;

	DWORD GetTotalGrassFaceCount();
	ULONG m_ulRefCount;
public:
	DWORD m_dwLastVisibleCount;
public:
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	HRESULT PrepareShareGrassVB();
	HRESULT UnPrepareShareGrassVB();

	DWORD GetGrassCount(){return m_dwNumGrass;};

	KG3DTerrainBlock* m_lpParentTerrainBlock;

	HRESULT CreateGrassVertex();

	LPDIRECT3DTEXTURE9 GetGrassTexture();
	LPDIRECT3DVERTEXBUFFER9 GetShareGrassVBuffer(DWORD& dwFaces);

	HRESULT SetGrassSet(KG3DTerrainGrassSet* pSet,UINT uHandleSet);

	HRESULT CreateGrass(DWORD dwNum);

	HRESULT Init(DWORD dwType);
	HRESULT UnInit();

	KG3DTerrainBlockGrass();
	~KG3DTerrainBlockGrass(void);

public:
	HRESULT AddGrass(KG3DTerrainGrass& sGrass);
	HRESULT RemoveGrass(FRECT rect);
	HRESULT ClearGrassList();//���listGrass�������
	HRESULT GetVecGrass(vector<KG3DTerrainGrass>* pVecGrass);

	HRESULT FitGround(KG3DSceneOutDoorSpaceMgr* pOutDoorSpaceMgr);

	HRESULT GetAutoGrassData(int nX,int nZ,BOOL& bCreat,BYTE& byDetailTexture,BYTE& byTexIndex,BYTE& byPattrenIndex,float& fWidth,float& fHeight);
	HRESULT SetAutoGrassData(int nX,int nZ,BOOL bCreat,BYTE byDetailTexture,BYTE byTexIndex,BYTE byPattrenIndex,float fWidth,float fHeight);

	DWORD GetBlockType()
	{
		return m_dwBlockType;
	}
};

extern TKG3DResourcePool<KG3DTerrainBlockGrass> g_cTerrainBlockGrassPool;
extern TKG3DResourcePool<KG3DTerrainBlockGrassShareVB_High> g_cTerrainBlockGrassShareVBHighPool;
