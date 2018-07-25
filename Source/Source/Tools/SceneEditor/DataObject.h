// DataObject.h: interface for the KComponentObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAOBJECT_H__E19A5558_C467_4853_B824_8150FA780A2D__INCLUDED_)
#define AFX_DATAOBJECT_H__E19A5558_C467_4853_B824_8150FA780A2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KRegion.h"

#define OBJECT_DEFAULT 0
#define OBJECT_SKYBOX  1
#define OBJECTTYPE_GIRLNO1   100
#define OBJECTTYPE_NPC       200
#define OBJECTTYPE_PLAYER    300

#define TERRIANCELLWIDTH 50.0F
#define TERRIANCELLTEXTUREWIDTH 100.0F
//#define TERRIANREGIONCELL 32
#define TERRIANREGIONWIDTH 1600.0F

#define BARRIER_NONE          0x00000000
#define BARRIER_CAN_WALK      0x00000001
#define BARRIER_CAN_JUMP      0X00000002
#define BARRIER_BULLET_PASS   0X00000004
#define BARRIER_OBJECT_PLACE  0X00000008

#define BARRIER_OPERATOR_ADD     0
#define BARRIER_OPERATOR_REMOVE  1
#define BARRIER_OPERATOR_SET     2

#define HEIGHTDOWN_UP            10
#define HEIGHTDOWN_DOWN          11
#define HEIGHTDOWN_SET           12

#define SCRABBLE_SANP2GRID       1
#define SCRABBLE_VERTICAL        (1<<1)
#define SCRABBLE_PITCHONLY       (1<<2)
#define SCRABBLE_MODIFYTERRAIN   (1<<3)
#define SCRABBLE_MODIFYSELF      (1<<4)
#define SCRABBLE_NOONBARRIER     (1<<5)
#define SCRABBLE_RUNTIME         (1<<6)
#define SCRABBLE_WATER           (1<<7)

#define HEIGHTMAPDATATYPE_FLOATHEIGHTONLY   1
#define HEIGHTMAPDATATYPE_FLOATHEIGHTNORMAL 2

typedef class KHeightMapBase
{
public:
	AABBOX      m_BBox;//�߶�ͼ�İ�Χ����

	int         nWidth; //���� x ����
	int         nHeight;//��� z ����
	int         m_nDataType;//��������, 

	float       CellWidth;//���ӵĿ��
	void*       pData;    //�߶�����
	void*       pDataBack;//�߶����ݵı���,����ʵ��UNDO/REDO
	int         nDataSize;//���ݵĿ��
	float       fEffect;  //�ø߶Ȳ�Ļ��ϵ��
	D3DXVECTOR2 m_LeftUp; //���Ͻ�����

	KHeightMapBase();
	virtual ~KHeightMapBase();

	virtual HRESULT CleanUp();//�������,�ָ�Ĭ�ϲ�������

	HRESULT CheckBorder_Left (KHeightMapBase* DestMap);//���Է��߶�ͼ�ı߽�߶ȵ��ڱ��߶�ͼ
	HRESULT CheckBorder_Right(KHeightMapBase* DestMap);
	HRESULT CheckBorder_Up   (KHeightMapBase* DestMap);
	HRESULT CheckBorder_Down (KHeightMapBase* DestMap);

	HRESULT ModifyHeight(D3DXVECTOR2 Center,float Size,int Shape,int Operator,float Value);//�޸ĸ߶���Ϣ
	HRESULT ModifyHeight(ModelUI::FRECT Rect,int Shape,int Operator,float Value,float Force);

	float   GetVertexHeight(int nX,int nZ);
	float   GetVertexHeight(const int Index);
	D3DXVECTOR3 GetVertexPosion(int nX,int nZ);//��ȡ���ݶ���λ��
	D3DXVECTOR3 GetVertexPosionBack(int nX,int nZ);//��ȡ���ݵ����ݶ���λ��
	D3DXVECTOR3 GetVertexNormal(int nX,int nZ);

	void    SetVertexNormal(D3DXVECTOR3 Normal,int nX,int nZ);//�������ݶ���ķ�����
	void    SetVertexPosition(D3DXVECTOR3 Pos,int nX,int nZ);//�������ݶ���λ��
	HRESULT SetLeftUp(D3DXVECTOR2 InXZ);//�������Ͻ�λ��

	virtual BOOL GetPosition(D3DXVECTOR3* pOutPos,D3DXVECTOR2 InXZ);//��ѯλ��
	virtual BOOL GetNormal(D3DXVECTOR3* pOutNormal,D3DXVECTOR2 InXZ);//��ѯ������

	void    CreateTestMap();

	HRESULT Create(D3DXVECTOR2 Left_Up,int Width,int Height,int DataType,
		           float Cell_Width,float BaseHeight);//�����߶�ͼ

	HRESULT FromTexture(LPDIRECT3DTEXTURE9 pTexture,float HeightBase,float HeightScale);//��ͼ�󴴽�
	HRESULT Save2File(FILE* pFile);//���浽�ļ�
	HRESULT LoadFromFile(FILE* pFile);//���ļ���ȡ

	void    BackUpData();//�����ݱ��浽������
	void    RestoreData();//�ӱ��������лָ�����
}*LPHEIGHTMAP;


typedef class KComponentObject  
{
public:
	struct _ObjectFileHeaderBase
	{
		DWORD FileMark;
		DWORD Type;
		DWORD SubType;
		DWORD BlockLength;

		_ObjectFileHeaderBase()
		{
			FileMark = 0x4F6C7A2B;
			Type     = 0;
			SubType  = 0;
			BlockLength = 0;
		}
	};

	struct _BuildingObjectHeader : _ObjectFileHeaderBase
	{
		int  nStart_X;
		int  nStart_Z;
		int  nWidth;
		int  nHeight;

		DWORD  BlockHeightMapDown;
		DWORD  BlockHeightMapUp;
		DWORD  BlockBarrier;
		DWORD  BlockMeshes;
		DWORD  BlockSetting;
		DWORD  BlockScrabble;
		DWORD  BlockTime;
		DWORD  BlockView;
		DWORD  BlockRender;
		DWORD  BlockScript;
		DWORD  BlockEnvent;
		DWORD  BlockPreperty;
		DWORD  BlockMethod;

		DWORD  Extend[20];

		_BuildingObjectHeader()
		{
			Type = OBJECT_DEFAULT;
			SubType = 0;

			nStart_X  = 0;
			nStart_Z  = 0;
			nWidth    = 0;
			nHeight   = 0;

			BlockHeightMapDown = 0;
			BlockHeightMapUp   = 0;
			BlockBarrier       = 0;
			BlockMeshes        = 0;
			BlockSetting       = 0;
			BlockScrabble      = 0;
			BlockTime          = 0;
			BlockView          = 0;
			BlockRender        = 0;
			BlockScript        = 0;
			BlockEnvent        = 0;
			BlockPreperty      = 0;
			BlockMethod        = 0;

			ZeroMemory(Extend,sizeof(DWORD)*20);
		}
	};
	//struct to keep xmesh
	typedef struct _MeshKeeper
	{
		DWORD      m_dMeshID;
		D3DXMATRIX m_Matrix;
		_MeshKeeper()
		{
			m_dMeshID = 0;
			D3DXMatrixIdentity(&m_Matrix);
		}
	}*LPMESHKEEPER;

	typedef struct _CellInfo
	{
		DWORD Barrier;
		DWORD Keep;
		_CellInfo()
		{
			Barrier     = BARRIER_CAN_WALK | BARRIER_CAN_JUMP | BARRIER_BULLET_PASS | BARRIER_OBJECT_PLACE;
			Keep = 0;
		}
	}*LPCELLINFO;

	typedef struct _GridInfo
	{
		float Height_Down;
		float Height_Up;
		_GridInfo()
		{
			Height_Down = 0;
			Height_Up   = 0;
		}
	}*LPGRIDINFO;

public:

	int        m_nStart_X;
	int        m_nStart_Z;
	int        m_nWidth;
	int        m_nHeight;
	LPCELLINFO m_lpCellInformation;
	//LPGRIDINFO M_lpGridInformation;
	LPD3DXMESH m_lpMeshGridInfo;

	LPHEIGHTMAP m_lpHeightDown;

	DWORD ID;
	DWORD m_dwType;

	vector<_MeshKeeper>m_vecMeshKeeper;

	//LPMESHKEEPER m_lpMeshKeeper;
	//DWORD m_dNumMesh;
	BOOL         m_bHeightDownNeedUpdate;

	D3DXMATRIX   m_Matrix;

	BBox         m_BBox;


	D3DXVECTOR3  m_Position;
	D3DXVECTOR3  m_Rotation;
	D3DXVECTOR3  m_Scaling;

	D3DXVECTOR3  m_PositionBack;
	D3DXVECTOR3  m_RotationBack;
	D3DXVECTOR3  m_ScalingBack;


	D3DXVECTOR3  m_PickLine;
	float        m_fBrushSize;

	DWORD        m_dwScrabbleOption;//ƴ����Ϣ
	string       m_FileName;
	DWORD        m_dwGroupIndex;//�����Ӧ��������е�����
	DWORD        m_dwIndexInGroup;
protected:
	//D3DXVECTOR3  m_PositionReal;//ʵ�ʵ�λ����Ϣ

public:
	virtual HRESULT RenderWithEffect(DWORD Option,PVOID pData);

	virtual HRESULT LoadFromFile(LPSTR pFileName);
	virtual HRESULT Save2File(LPSTR pFileName);


	virtual BOOL    SelectByPickLine(float* Distance,D3DXVECTOR3* pOrg,D3DXVECTOR3* pDir);
	HRESULT SetGridHeightDown(DWORD Operator,float Value);
	HRESULT SetCellBarrier(DWORD Operator,DWORD dwBarrier);
	HRESULT SetPickLine(D3DXVECTOR3* pOrg,D3DXVECTOR3* pDir);

	HRESULT UpdataCellInfoHeightDown();
	HRESULT BuildCellInfoHeightDown();

	virtual HRESULT RenderCellInfoBarrier(DWORD dwBarrier);
	virtual HRESULT RenderCellInfoHeightUp();
	virtual HRESULT RenderCellInfoHeightDown();
	virtual HRESULT CreateCellInformation();
	
	HRESULT DeleteMesh(DWORD Index);

	virtual	PropertyBase::KPropertyBase* Acquire();
	virtual HRESULT Copy(KComponentObject** pNewObject);

	virtual HRESULT LoadByIni(IIniFile* pIniFile,LPSTR pFileName);//���� ini �ļ�������һ�����

	virtual ModelUI::FRECT GetXZRect();

	virtual HRESULT RenderShadow(DWORD ShadowType);
	virtual HRESULT BackupData();

	HRESULT Edit_Scaling(D3DXVECTOR3 * pScal);
	HRESULT Edit_Rotate(D3DXVECTOR3* pRot);
	HRESULT Edit_Move(D3DXVECTOR3* pPos);
	HRESULT DrawBBox();
	HRESULT ComputeBBox();
	virtual HRESULT CleanUp();
	HRESULT GetMesh(LPMODEL* ppXMesh,DWORD ID);
	HRESULT LoadMaterial(LPSTR pFileName);
	HRESULT LoadMesh(LPSTR pFileName);
	HRESULT AddMesh(DWORD MeshID);
	HRESULT GetMeshKeeper(LPMESHKEEPER * pMeshKeeper,DWORD ID);
	HRESULT Get1NewXMeshKeeper(LPMESHKEEPER * pMeshKeeper);
	HRESULT LoadFromX(LPSTR pFileName);

	virtual HRESULT Render();
	virtual HRESULT FrameMove();
	virtual void Initialize(PVOID pObjectTable);

	KComponentObject();
	virtual ~KComponentObject();

}* LPOBJECT;

#endif // !defined(AFX_DATAOBJECT_H__E19A5558_C467_4853_B824_8150FA780A2D__INCLUDED_)
