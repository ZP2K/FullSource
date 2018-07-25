// KModelSubd.h: interface for the KModelSubd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KMODELSUBD_H__8F696C42_6FD2_449E_A832_1FE05ED81CFF__INCLUDED_)
#define AFX_KMODELSUBD_H__8F696C42_6FD2_449E_A832_1FE05ED81CFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XMesh.h"

#define REFINE_VALUE 0.4f
#define HIGH_DIFFERENT_VALUE 1.0f

class KModelSubd : public KModel  
{
public:

	typedef struct ParentFace
	{	
		int index;
		int NeighborFace[3];
		int EdgeVertex[3];
		bool bSubd1To2;///�Ƿ��ѱ�1-2ϸ��
		bool bSubd1To4;//�Ƿ��ѱ�1-4ϸ��
		int Vers[3];
		int ChildFace[4];
		D3DXVECTOR3 Normal;
		ParentFace()
		{
			for (int i = 0 ; i < 3 ; i++)
			{
				NeighborFace[i] = -1;
				EdgeVertex[i] = -1;
				Vers[i] = -1;
				ChildFace[i] = -1;
			}
			ChildFace[3] = -1;
		}
		~ParentFace () 
		{
			//SAFE_DELETE_ARRAY(NeighborFace);
			//SAFE_DELETE_ARRAY(EdgeVertex);
		}
	
	}*LPParentFace;

	struct Face3D
	{
		int Vers[3];

		int index;
		int neighborFace[3];// the index of neighbour face
		int parentFace; //the index of parent face
		//bool bSubdivision; //whether been visited
		int EdgeVertex[3]; // Index of edge vertex
		
		float RefineParam;///����ϸ�����жϲ���
		float HighDif;

		bool bSubd1To2;///�Ƿ��ѱ�1-2ϸ��
		bool bSubd1To4;//�Ƿ��ѱ�1-4ϸ��
		int iFaceNeighborN; ///	�õ������ĸ���
		
		D3DXVECTOR3 Normal;///����ķ�����

		Face3D()
		{
			index = 0 ;
			parentFace =0;
			//bSubdivision =0;
			for ( int i = 0 ; i < 3 ; i++ )
			{
				neighborFace[i] = -1 ;
				Vers[i] = -1 ;
			}
				
		}
		~Face3D()
		{
			//SAFE_DELETE_ARRAY(Vers);
			//SAFE_DELETE_ARRAY(neighborFace);
			index = 0 ;
			parentFace =0;			
		}
	public:
		
		//float GetRefineParam();
		void SetRefineParam( KModelSubd *pSubd );
		//void SetRefineParam( KModelSubd *pSubd , int nIndex);
		//void SetHighDif();

		//float SetRefineParam();///���ظ����ϸ������
		//float GetArea(KModelSubd *pSubd); ///���ظ�������
		//float GetArea(KModelSubd *pSubd, int nIndex); ///���ظ�������
		float CalcHighDif();///�����������ĸ߶Ȳ�

		//Vertex3D GetOddVertex(int index);///�õ������ϵ�ODD ��
		int IsIncludeVertex(int vertex)///�ж�һ�����Ƿ����һ����
		{	
			int iResult = false;
			for (int i = 0 ; i < 3 ; i++)
				if ( Vers[i] == vertex)
				{
					iResult = true;
					goto Exit0;
				}
		Exit0:
				return iResult;
		}
		int IsIncludeVertex(KModelSubd *pSubd, int nIndex, int nVertex);///�ж�һ�����Ƿ����һ����
	} *LPFace3D;

	typedef struct Vertex3D
	{
		D3DXVECTOR3 position ;///�õ����ά����
		float tu;
		float tv;
		D3DXVECTOR3 normal;
		int index;
		int iFaceN;///������ĸ���
		int degree;///������ĸ���
		vector <int> pFaceN;
		//int pFaceN[20];///��¼���������indexֵ
		vector <int> pVertexN;
		//int pVertexN[20];///��¼���������indexֵ
		float fHeight;
		D3DXVECTOR3 Height; ///�����ĸ߶�����������

		Vertex3D()
		{			
			tu = -1;
			tv = -1;			
			index = -1;
/*
			for (int i = 0 ; i < 20 ; i++ )
			{
				pFaceN[i] = -1 ;
				pVertexN[i] = -1 ;
			}
			*/
			//pFaceN = 0;
			//pVertexN = 0;
		}

		~Vertex3D()
		{
			//SAFE_DELETE_ARRAY(pFaceN);
			//SAFE_DELETE_ARRAY(pVertexN);
		}	
		int SetDegree();		
	} *LPVertex3D;

public:
	
	LPD3DXMESH        m_pMeshSubdSys; //mesh been subdivision

	vector <Face3D> m_VecFace3D; ///�洢�����е�����Ϣ
	vector <Vertex3D>  m_VecVers3D;///�洢�����еĵ���Ϣ
	int m_iNumVers;///�����нڵ�ĸ���
	int m_iNumFaces;///��������ĸ���

	float m_fCameroLength;///����������ľ���

	int m_nSubdivisionN;///����ϸ�ֵĴ���

	LPDIRECT3DTEXTURE9 m_lpTexture;///�������ͼ

	float m_fMaxArea;///�������������������
	float m_fMinArea;///���������������С���

	int m_nHeight;///�����ߵĸ߶�
	float m_fVertexHeightDif;///�ж�ֵ��������еĶ����ͨ���߶�ֵ�ò��г�����ֵ��Ҫ����ϸ��
public:

	KModelSubd();
	virtual ~KModelSubd();
	///����KModel
	HRESULT FrameMove();
	HRESULT Render();

	HRESULT LoadMesh(LPSTR pFileName);//����Ҫϸ����mesh
	HRESULT Acquire();//��ѯ����

	int LoadTexture(LPDIRECT3DTEXTURE9 pTexture); ///����絼��texture
	int SaveSubdMesh();///�����ݴ洢��m_pMeshSubdSys
	int InitializeSubd();
	int Refinement();	

	//int LineInterpolate();/// ֱ�ӵ����Բ�ֵ
	int LineInterpolateMid(Vertex3D &vertex,int faceIndex, int edgeIndex);///����faceIndex�ĵ�edgeIndex�����е�λ�ò����µ�vertex
	int LineInterpolate(int faceIndex);/// �Ը������ֱ�ӵ����Բ�ֵ
	
	int LoopSubdivision( int faceIndex );///�Ը������ϸ�ִ���
	int ButterflySubdivsion( int faceIndex);
	void NPath();

	////////���㼸��λ��
	int CalcOddVertex(Vertex3D & oddVertex,int faceIndex, int edgeIndex);// ��loop��������faceindex�ϵĵ�edgeIndex�ϵ�odd index
	
	void CalcEvenVertex(int faceIndex, int vertexIndex);// ��faceindex�ϵĵ�vertexIndex�ϵ�Even index
	int CalcVertexNeighborValue();///����ÿ����Ķ������ٽӵ�
	//////////////////////////////////////////////////////////////////////////

	///��������λ��
	//void BuildAdjacency(vector <Face3D> & vecFace);///����topology	
	void BuildAdjacency(vector <ParentFace> & vecFace);///����topology	
	int ModifyNeighborFace(int faceIndex, int edgeIndex);///��������ڽ����ϵıߵ�
		
	void CreateFourNewFaces( const ParentFace parentFace);///����parentFaceIndex���ѳ��ĸ�����
	void CreateTwoNewFaces(const ParentFace parentFace);///����parentFaceIndex���ѳ�����������
	void CloneFromParentFace(const ParentFace parentFace);///����parentFace��¡���µ�������ȥ
	
	///������������Լ����ȫ�ֺ���
	void MidVector(D3DXVECTOR3 & midVec, const D3DXVECTOR3 vector1, const D3DXVECTOR3 vector2); ///���������������е�����
	int CalcFaceNormal(D3DXVECTOR3 &faceNormal,int faceIndex);///������faceIndex�ķ�����
	int CalcFaceNormal2( D3DXVECTOR3 &faceNormal,const D3DXVECTOR3 parentNormal );///������faceIndex�ķ�����
		
	int CalcVertexNormal(D3DXVECTOR3 &vertexNormal,int vertexIndex);///�����vertexIndex�ķ�����

	///����ϸ������
	float GetFaceArea(int nIndex);
	//float GetFaceArea(const Face3D myFace, int nIndex);
	float SetRefineParam(int nIndex);
	//float SetRefineParam(const Face3D myFace, int nIndex);
	float SetHighDif(const int nFaceIndex);

	//////////////////////////////////////////////////////////////////////////
	
	int CalcTextureHighValue( LPDIRECT3DTEXTURE9 pTexture);
	int ChangeMeshHigh();
	int ChangeMeshHigh2();
	
	int SetMaxMinArea();
	int IsSameHeightVertexofFace(int  faceIndex);
};

#endif // !defined(AFX_KMODELSUBD_H__8F696C42_6FD2_449E_A832_1FE05ED81CFF__INCLUDED_)
