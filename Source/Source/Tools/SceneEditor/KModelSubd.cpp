// KModelSubd.cpp: implementation of the KModelSubd class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "sceneeditor.h"
#include "KModelSubd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////

/**
* @brief ���캯������ʵ���������и�ֵ
**/
KModelSubd::KModelSubd()
{
	m_iNumVers = 0;
	m_iNumFaces = 0;
	m_nHeight = 50;
	m_fVertexHeightDif = 0.1f;
}

KModelSubd::~KModelSubd()
{

}

HRESULT KModelSubd::FrameMove()
{
	return S_OK;
}

/**
* @param nFaceIndex �������ֵ
* @brief ���õ�nFaceIndex��ĸ߶Ȳ�
* @return �������ĸ߶Ȳ��y��Ϊ�߶ȣ�
*/
float KModelSubd::SetHighDif(const int nFaceIndex)
{
	float fHigh, fMaxHeight;
	//fMaxHeight= ( nFaceIndex * 13 ) % 10 * 0.01;
	
	int nVertex0, nVertex1, nVertex2; 
	nVertex0 = m_VecFace3D[nFaceIndex].Vers[0];
	nVertex1 = m_VecFace3D[nFaceIndex].Vers[1];
	nVertex2 = m_VecFace3D[nFaceIndex].Vers[2];
	fMaxHeight = abs(m_VecVers3D[nVertex0].position.y - m_VecVers3D[nVertex1].position.y);
	fHigh = abs(m_VecVers3D[nVertex0].position.y - m_VecVers3D[nVertex2].position.y);
	if ( fHigh > fMaxHeight)
		fMaxHeight = fHigh;
	fHigh = abs(m_VecVers3D[nVertex1].position.y - m_VecVers3D[nVertex2].position.y);
	if ( fHigh > fMaxHeight)
		fMaxHeight = fHigh;
	
	return fMaxHeight;
}
/**
* @param nIndex �������ֵ
* @brief���õ�nIndex��Refine���� 
* @return  ���ϸ������ֵ
*/
float KModelSubd::SetRefineParam(int nIndex)
{
	//RefineParam = 1 * ;
	float fParam;
	float fArea;
	fArea = GetFaceArea(nIndex);
	//fParam = ( int ( fArea * 100 ) )% 10 * 0.1f;	
	//fParam = (nIndex *7  )% 10 * 0.1f + ((int) ( fArea * 13)) % 10 * 0.1;
	fParam = fArea * (m_nSubdivisionN +1);
	return fParam;
}

/**
* @param *pSubd Ŀǰ�����ָ��
* @param nIndex Ҫ������������ֵ
* @param nVertex �������ֵ
* @brief �жϵ�nIndex�����Ƿ��������ֵΪnVertex�ĵ�
* @return false ��ʾʧ�ܣ�true ��ʾ�ɹ�
*/
int KModelSubd::Face3D::IsIncludeVertex(KModelSubd *pSubd, int nIndex, int nVertex)
{
	int nResult = false;
	for (int i = 0 ; i < 3 ; i++)
		if ( pSubd->m_VecFace3D[nIndex].Vers[i] == nVertex)
		{
			nResult = true;
			goto Exit0;
		}
Exit0:
		return nResult;
}

/**
* @param nIndex �������ֵ
* @brief ��ȡ��nIndex������
* @return ������
*/
float KModelSubd::GetFaceArea(int nIndex)
{
	///�������θ��ߵı߳�
	int A,B,C;
	D3DXVECTOR3 vector_A,vector_B,vector_C;
	D3DXVECTOR3 t_vector;
	float fA,fB,fC,fS;
	float fArea;

	A = m_VecFace3D[nIndex].Vers[0];
	B = m_VecFace3D[nIndex].Vers[1];
	C = m_VecFace3D[nIndex].Vers[2];
	vector_A = m_VecVers3D[A].position;
	vector_B = m_VecVers3D[B].position;
	vector_C = m_VecVers3D[C].position;
	t_vector = vector_A - vector_B ;	
	fA = D3DXVec3Length(&t_vector);
	t_vector = vector_B - vector_C ;	
	fB = D3DXVec3Length(&t_vector);
	t_vector = vector_C - vector_A ;	
	fC = D3DXVec3Length(&t_vector);
	fS = ( fA + fB + fC ) / 2 ;

	fArea = sqrt(fS * ( fS - fA ) * ( fS - fB ) * ( fS - fC ));
	return fArea;
}
/*
* @param void
* @brief ���õ�ǰ����ÿ����Ķ���(���ڵ�ĸ���),��������Щ���indexֵ
*        ����ÿ�����pFaceN,pVertexN,degree,iFaceN��ֵ
* @return false ��ʾʧ�ܣ�true ��ʾ�ɹ�
*/

int KModelSubd::CalcVertexNeighborValue()
{
	int nResult = false;
	int nVertexIndex;
	int nOtherVertex;
	int nDegree = 0;
	int nFace ;
	int nVertex;
	
	for ( nFace = 0 ; nFace < m_iNumFaces ; nFace++ )
	{
		for ( nVertex = 0 ; nVertex < 3 ; nVertex++ )
		{			
			///�Ե�ǰ�ĵ��ж�ͬһ��������������������
			nVertexIndex = m_VecFace3D[nFace].Vers[nVertex];
			nOtherVertex = m_VecFace3D[nFace].Vers[(nVertex+1)%3];
			nDegree = m_VecVers3D[nVertexIndex].pVertexN.size();///Ŀǰ�Ѿ���ŵĹ�����ĸ���
			for (int i = 0; i < nDegree ; i++ )
			{///���������Ƿ���nOtherVertex��,����Ѿ�����,�������˳�ѭ��
				if ( m_VecVers3D[nVertexIndex].pVertexN[i] == nOtherVertex )
					break;
			}
			if ( i >= nDegree )
			{///�������ѭ����û�з��ֵ���nOtherVertex�ĵ�,�����pVertexN��
				m_VecVers3D[nVertexIndex].pVertexN.push_back(nOtherVertex);
			}
			nOtherVertex = m_VecFace3D[nFace].Vers[(nVertex+2)%3];			
			for (int i = 0; i < nDegree ; i++ )
			{///���������Ƿ���nOtherVertex��,����Ѿ�����,�������˳�ѭ��
				if ( m_VecVers3D[nVertexIndex].pVertexN[i] == nOtherVertex )
					break;
			}
			if ( i >= nDegree )
			{///�������ѭ����û�з��ֵ���nOtherVertex�ĵ�,�����pVertexN��
				m_VecVers3D[nVertexIndex].pVertexN.push_back(nOtherVertex);
			}

			m_VecVers3D[nVertexIndex].pFaceN.push_back(nFace);
		}
	}
	///�����iFaceN��degree�������Ը�ֵ
	for ( nVertex = 0 ; nVertex < m_iNumVers ; nVertex++ )
	{
		m_VecVers3D[nVertex].iFaceN = m_VecVers3D[nVertex].pFaceN.size();
		m_VecVers3D[nVertex].degree = m_VecVers3D[nVertex].pVertexN.size();
	}

	nResult = true;
	return nResult;
}
/**
* @param pFileName ������ļ���
* @brief  load mesh,������ʼ���Ĺ���
* @return S_FALSE ��ʾʧ�ܣ�S_OK ��ʾ�ɹ�
*/
HRESULT KModelSubd::LoadMesh(LPSTR pFileName)
{
	HRESULT hrResult;
	hrResult = S_FALSE;
	KModel::LoadMesh(pFileName);
	m_pMeshSubdSys = KModel::m_pMeshSys;

	///�õ������vertex��face��Numֵ
	m_iNumVers = m_pMeshSubdSys->GetNumVertices();
	m_iNumFaces  = m_pMeshSubdSys->GetNumFaces();
		
	VFormat::FACES_NORMAL_TEXTURE1 * pVers = NULL;
	WORD* pIndex = NULL;
	DWORD * pAttrib = NULL;
	if (FAILED(m_pMeshSubdSys->LockVertexBuffer(0,(void**)&pVers)))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->LockIndexBuffer (0,(void**)&pIndex)))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->LockAttributeBuffer(0,(DWORD**)&pAttrib)))
		goto Exit0;	

	///���ڼ����ٽӵ�
	LPD3DXBUFFER m_pAdjacencyBuffer = NULL;   // Contains the adjaceny info loaded with the mesh
	if ( FAILED( D3DXCreateBuffer(m_pMeshSys->GetNumFaces()*3*sizeof(DWORD),
		&m_pAdjacencyBuffer)))
		goto Exit0;
	if ( FAILED(  m_pMeshSys->GenerateAdjacency(0,
		(DWORD*)m_pAdjacencyBuffer->GetBufferPointer())))
		goto Exit0;

	DWORD * pAdjacen = (DWORD*)m_pAdjacencyBuffer->GetBufferPointer();
	

	///����ÿ��face��������Ϣ��KModelSubd�������m_VecFace3D��
	for(int i=0;i<m_iNumFaces;i++)
	{
		
		WORD A,B,C;
		DWORD SubsetID;
		A = pIndex[i*3  ];
		B = pIndex[i*3+1];
		C = pIndex[i*3+2];
		SubsetID = pAttrib[i];

		D3DXVECTOR3 Pos_A = pVers[A].p;
		D3DXVECTOR3 Pos_B = pVers[B].p;
		D3DXVECTOR3 Pos_C = pVers[C].p;
		///�洢��
		Face3D myFace ;
		myFace.index = i;
		
		myFace.iFaceNeighborN = 0;
		for (int j = 0 ; j < 3 ; j++ )
		{
			myFace.Vers[j] = pIndex[i*3+j];
			myFace.neighborFace[j] = pAdjacen[3*i + j];
			if (myFace.neighborFace[j] != -1) 
				myFace.iFaceNeighborN = myFace.iFaceNeighborN + 1;
			myFace.EdgeVertex[j] = -1;
		}
		
		m_VecFace3D.push_back(myFace);
	}
	///����ÿ���㣬������Ϣ��KModelSubd�������m_VecVers3D��
	for (int nVer = 0 ; nVer < m_iNumVers ; nVer++ )
	{
		Vertex3D myVertex;
		myVertex.index = nVer;
		myVertex.position = pVers[nVer].p;
		myVertex.normal = pVers[nVer].Normal;
		myVertex.tu = pVers[nVer].tu1;
		myVertex.tv = pVers[nVer].tv1;	
		myVertex.degree = 0;
		myVertex.iFaceN = 0;
		myVertex.pFaceN.clear();
		myVertex.pVertexN.clear();
		m_VecVers3D.push_back(myVertex);
	}

	m_nSubdivisionN = 0;
	

	if (FAILED(m_pMeshSubdSys->UnlockVertexBuffer()))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->UnlockIndexBuffer()))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->UnlockAttributeBuffer()))
		goto Exit0;


	hrResult = S_OK;
Exit0:


	return hrResult;
}

/**
* @param & midVec ������е������
* @param vector1 , ����1
* @param vector2 , ����2
* @brief �������vector1,vector2���ߵ��е㣬���ظ��е�ֵ������
* @return void
*/
void KModelSubd::MidVector(D3DXVECTOR3 & midVec, const D3DXVECTOR3 vector1, const D3DXVECTOR3 vector2)
{
	midVec.x = ( vector1.x + vector2.x ) /2 ;
	midVec.y = ( vector1.y + vector2.y ) /2 ;
	midVec.z = ( vector1.z + vector2.z ) /2 ;
}

/**
* @param vector <parent.>
* @brief ����������˽ṹ
* @return void
*/
void KModelSubd::BuildAdjacency(vector <ParentFace> & vecFace)
{
	///������������ٽ���
	for (int iFace = 0 ; iFace < m_iNumFaces ; iFace++)
	{
		if (vecFace[iFace].bSubd1To4)///����������ĸ�����
		{
			int iNeighborFace0 = vecFace[iFace].NeighborFace[0];
			int iNeighborFace1 = vecFace[iFace].NeighborFace[1];
			int iNeighborFace2 = vecFace[iFace].NeighborFace[2];
			
			//////////////////////////////////////////////////////////////////////////	
			///set the first faces
			int iChildFace = vecFace[iFace].ChildFace[0];
			int iPoint = vecFace[iFace].Vers[0];
			
			if ( iNeighborFace0 != -1 )
			{///���õ�һ��neighbor face
					for ( int j = 0 ; j < 3 ; j++ )
					{
						int iNeighborChild = vecFace[iNeighborFace0].ChildFace[j];
						if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
						{
							m_VecFace3D[iChildFace].neighborFace[0] = iNeighborChild;
							break;
						}
					}		
			}
			m_VecFace3D[iChildFace].neighborFace[1] = vecFace[iFace].ChildFace[3];/// the second neighbor face
			if ( iNeighborFace2 != -1 )
			{///the third neighbor face
				for ( int j = 0 ; j < 3 ; j++ )
				{
					int iNeighborChild = vecFace[iNeighborFace2].ChildFace[j];
					if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
					{
						m_VecFace3D[iChildFace].neighborFace[2] = iNeighborChild;
						break;
					}
				}
			}
			for ( int k = 0 ; k < 3 ; k++ )
			{
				if (m_VecFace3D[iChildFace].neighborFace[k] != -1)
					m_VecFace3D[iChildFace].iFaceNeighborN = m_VecFace3D[iChildFace].iFaceNeighborN + 1 ;
			}
			//////////////////////////////////////////////////////////////////////////
			///set the second faces 
			iChildFace = vecFace[iFace].ChildFace[1];
			iPoint = vecFace[iFace].Vers[1];
			if ( iNeighborFace0 != -1 )
			{///���õ�һ��neighbor face
				for ( int j = 0 ; j < 3 ; j++ )
				{
					int iNeighborChild = vecFace[iNeighborFace0].ChildFace[j];
					if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
					{
						m_VecFace3D[iChildFace].neighborFace[0] = iNeighborChild;
						break;
					}
				}		
			}
			if ( iNeighborFace1 != -1 )
			{///the second neighbor face
				for ( int j = 0 ; j < 3 ; j++ )
				{
					int iNeighborChild = vecFace[iNeighborFace1].ChildFace[j];
					if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
					{
						m_VecFace3D[iChildFace].neighborFace[1] = iNeighborChild;
						break;
					}
				}
			}
			m_VecFace3D[iChildFace].neighborFace[2] = vecFace[iFace].ChildFace[3];/// the third neighbor face
			for ( int k = 0 ; k < 3 ; k++ )
			{
				if (m_VecFace3D[iChildFace].neighborFace[k] != -1)
					m_VecFace3D[iChildFace].iFaceNeighborN = m_VecFace3D[iChildFace].iFaceNeighborN + 1 ;
			}
			//////////////////////////////////////////////////////////////////////////
			///set the third face
			iChildFace = vecFace[iFace].ChildFace[2];
			iPoint = vecFace[iFace].Vers[2];
			m_VecFace3D[iChildFace].neighborFace[0] = vecFace[iFace].ChildFace[3];/// the first neighbor face
			if ( iNeighborFace1 != -1 )
			{///���õ�һ��second face
				for ( int j = 0 ; j < 3 ; j++ )
				{
					int iNeighborChild = vecFace[iNeighborFace1].ChildFace[j];
					if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
					{
						m_VecFace3D[iChildFace].neighborFace[1] = iNeighborChild;
						break;
					}
				}		
			}
			if ( iNeighborFace2 != -1 )
			{///the second neighbor face
				for ( int j = 0 ; j < 3 ; j++ )
				{
					int iNeighborChild = vecFace[iNeighborFace2].ChildFace[j];
					if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
					{
						m_VecFace3D[iChildFace].neighborFace[2] = iNeighborChild;
						break;
					}
				}
			}
			for ( int k = 0 ; k < 3 ; k++ )
			{
				if (m_VecFace3D[iChildFace].neighborFace[k] != -1)
					m_VecFace3D[iChildFace].iFaceNeighborN = m_VecFace3D[iChildFace].iFaceNeighborN + 1 ;
			}
			//////////////////////////////////////////////////////////////////////////
			///set the fourth face
			iChildFace = vecFace[iFace].ChildFace[3];
			m_VecFace3D[iChildFace].neighborFace[0] = vecFace[iFace].ChildFace[1];
			m_VecFace3D[iChildFace].neighborFace[1] = vecFace[iFace].ChildFace[2];
			m_VecFace3D[iChildFace].neighborFace[2] = vecFace[iFace].ChildFace[0];
			m_VecFace3D[iChildFace].iFaceNeighborN = 3;	
		}///(vecFace[iFace].bSubd1To4)///����������ĸ�����
		else if (vecFace[iFace].bSubd1To2)
		{
			int iChildFace = vecFace[iFace].ChildFace[0];
			int nPoint1,nPoint2;///���ٽ����������������
			int iParentNeighborFace;///�ڽ���
			int nNeighborChildFace;///�ٽ��������
			for (int j = 0 ; j < 3 ; j++ )
			{
				if ( vecFace[iFace].EdgeVertex[j] != -1 )
				{
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////			
			///set the first face			
			///the first neighbor face(���û���ٽ������-1)
			iParentNeighborFace = vecFace[iFace].NeighborFace[(j+2)%3];
			nPoint1 = vecFace[iFace].Vers[j];
			nPoint2 = vecFace[iFace].Vers[(j+2)%3];
			
			if (vecFace[iParentNeighborFace].bSubd1To2 == true)
			{///������ڽ�����1-2ϸ���˵Ļ�,����neighbor face��������childFace
				for (int n = 0 ; n < 2 ; n++ )
				{
					nNeighborChildFace = vecFace[iParentNeighborFace].ChildFace[n];
					if ( (m_VecFace3D[nNeighborChildFace].IsIncludeVertex(this,nNeighborChildFace,nPoint1))
						&& ( m_VecFace3D[nNeighborChildFace].IsIncludeVertex(this,nNeighborChildFace,nPoint2) ) )
						break;
				}
				m_VecFace3D[iChildFace].neighborFace[0] = nNeighborChildFace;
			}
			else
			{///ֱ�Ӹ�ֵ,���Ϊ-1Ҳֱ�Ӹ�ֵ
				if (iParentNeighborFace == -1)
					m_VecFace3D[iChildFace].neighborFace[0] = -1 ;
				else
					m_VecFace3D[iChildFace].neighborFace[0] = vecFace[iParentNeighborFace].ChildFace[0];
			}			
			///the second neighbor face
			int iPoint ;
			iPoint = vecFace[iFace].Vers[j];			
			iParentNeighborFace = vecFace[iFace].NeighborFace[j];
			if ( iParentNeighborFace != -1 )
			{
				for ( int k = 0 ; k < 3 ; k++)
				{
					int iNeighborChild = vecFace[iParentNeighborFace].ChildFace[k];
					if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
					{
						m_VecFace3D[iChildFace].neighborFace[1] = iNeighborChild;
						break;
					}
				}
			}
			///the third neighbor face
			m_VecFace3D[iChildFace].neighborFace[2] = vecFace[iFace].ChildFace[1];
			for ( int k = 0 ; k < 3 ; k++ )
			{
				if (m_VecFace3D[iChildFace].neighborFace[k] != -1)
					m_VecFace3D[iChildFace].iFaceNeighborN = m_VecFace3D[iChildFace].iFaceNeighborN + 1 ;
			}		
			//////////////////////////////////////////////////////////////////////////
			///set the second face
			iChildFace = vecFace[iFace].ChildFace[1];
			nPoint1 = vecFace[iFace].Vers[(j+1)%3];
			nPoint2 = vecFace[iFace].Vers[(j+2)%3];

			///the first neighbor face
			m_VecFace3D[iChildFace].neighborFace[0] = vecFace[iFace].ChildFace[0];
			///the second neighbor face
			iPoint = vecFace[iFace].Vers[(j+1)%3];
			iParentNeighborFace = vecFace[iFace].NeighborFace[j];
			if ( iParentNeighborFace != -1 )
			{
				for ( int k = 0 ; k < 3 ; k++)
				{
					int iNeighborChild = vecFace[iParentNeighborFace].ChildFace[k];
					if (m_VecFace3D[iNeighborChild].IsIncludeVertex(this,iNeighborChild,iPoint))
					{
						m_VecFace3D[iChildFace].neighborFace[1] = iNeighborChild;
						break;
					}
				}
			}
			///the third neighbor face			
			iParentNeighborFace = vecFace[iFace].NeighborFace[(j+1)%3];
			if (vecFace[iParentNeighborFace].bSubd1To2 == true)
			{
				for (int n = 0 ; n < 2 ; n++ )
				{
					nNeighborChildFace = vecFace[iParentNeighborFace].ChildFace[n];
					if ( (m_VecFace3D[nNeighborChildFace].IsIncludeVertex(this,nNeighborChildFace,nPoint1))
						&& ( m_VecFace3D[nNeighborChildFace].IsIncludeVertex(this,nNeighborChildFace,nPoint2) ) )
						break;

				}
				m_VecFace3D[iChildFace].neighborFace[2] = nNeighborChildFace;
			}
			else
			{
				if (iParentNeighborFace == -1)
					m_VecFace3D[iChildFace].neighborFace[2] = -1 ;
				else
					m_VecFace3D[iChildFace].neighborFace[2] = vecFace[iParentNeighborFace].ChildFace[0];
			}			
			///����////
			for ( int k = 0 ; k < 3 ; k++ )
			{
				if (m_VecFace3D[iChildFace].neighborFace[k] != -1)
					m_VecFace3D[iChildFace].iFaceNeighborN = m_VecFace3D[iChildFace].iFaceNeighborN + 1 ;
			}
		}
		else
		{///����û�б�ϸ����
			int iChildFace = vecFace[iFace].ChildFace[0];
			int nNeighborChildFace;
			int nPoint1,nPoint2;
			int iNeighborFace;
			for ( int k = 0 ; k < 3 ; k++ )
			{
				iNeighborFace = vecFace[iFace].NeighborFace[k];
				nPoint1 = vecFace[iFace].Vers[k];
				nPoint2 = vecFace[iFace].Vers[(k+1)%3];
				if (vecFace[iNeighborFace].bSubd1To2 == true)
				{
					for (int n = 0 ; n < 2 ; n++ )
					{
						nNeighborChildFace = vecFace[iNeighborFace].ChildFace[n];
						if ( (m_VecFace3D[nNeighborChildFace].IsIncludeVertex(this,nNeighborChildFace,nPoint1))
							&& ( m_VecFace3D[nNeighborChildFace].IsIncludeVertex(this,nNeighborChildFace,nPoint2) ) )
							break;
					}
					m_VecFace3D[iChildFace].neighborFace[k] = nNeighborChildFace;
				}
				else
				{
					if (iNeighborFace == -1 )
						m_VecFace3D[iChildFace].neighborFace[k] = -1 ;
					else
						m_VecFace3D[iChildFace].neighborFace[k] = vecFace[iNeighborFace].ChildFace[0];	
				}				
				if ( iNeighborFace != -1 )
					m_VecFace3D[iChildFace].iFaceNeighborN = m_VecFace3D[iChildFace].iFaceNeighborN +1;
			}

		}

	}///for (int iFace = 0 ; iFace < m_iNumFaces ; iFace++)

	///���õ��iFace��pFaceN
}

/**
* @param &faceNormal �����������ķ�����
* @param parentNormal Ҫ������ĸ���
* @brief �������ʼ���������ķ�������
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::CalcFaceNormal2(D3DXVECTOR3 & faceNormal,const D3DXVECTOR3 parentNormal)
{
	faceNormal.x = parentNormal.x;
	faceNormal.y = parentNormal.y;
	faceNormal.z = parentNormal.z;
	return true;
}
/*
* @param & faceNormal ,���������ķ�����
* @param parentNormal Ҫ������ĸ���
* @brief �������ʼ��ʱ������ķ�����
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::CalcFaceNormal(D3DXVECTOR3 & faceNormal,int faceIndex)
{
	int iResult =false;
	int iVertex1,iVertex2,iVertex0;
	D3DXVECTOR3 p0,p1,p2;
	iVertex0 = m_VecFace3D[faceIndex].Vers[0];
	iVertex1 = m_VecFace3D[faceIndex].Vers[1];
	iVertex2 = m_VecFace3D[faceIndex].Vers[2];
	p0 = m_VecVers3D[iVertex0].position;
	p1 = m_VecVers3D[iVertex1].position;
	p2 = m_VecVers3D[iVertex2].position;
	
	D3DXPLANE placeFace;
	D3DXPlaneFromPoints(&placeFace,&p0,&p2,&p1);
	faceNormal.x = placeFace.a;
	faceNormal.y = placeFace.b;
	faceNormal.z = placeFace.c;
	iResult =true;

	return iResult;
}

/**
* @param & vertexNormal ���������ķ�����
* @param vertexIndex, �������ֵ
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
**/
int KModelSubd::CalcVertexNormal(D3DXVECTOR3 & vertexNormal, int vertexIndex)
{
	int iResult = false;
	float x,y,z;
	x = 0; y = 0; z = 0 ;
	int iFaceN ;
	iFaceN = m_VecVers3D[vertexIndex].iFaceN ;
	for (int i = 0; i < iFaceN ; i++)
	{
		int iNeighborFace = m_VecVers3D[vertexIndex].pFaceN[i];
		x += m_VecFace3D[iNeighborFace].Normal.x;
		y += m_VecFace3D[iNeighborFace].Normal.y;
		z += m_VecFace3D[iNeighborFace].Normal.z;
	}
	///ȡ��Χ���ٽ����ƽ��ֵ
	x /= iFaceN;
	y /= iFaceN;
	z /= iFaceN;
	vertexNormal.x = x ;
	vertexNormal.y = y ;
	vertexNormal.z = z ;

	return iResult;
}

/**
* @param faceIndex �������ֵ
* @brief ����������Բ�ֵ
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
**/
int KModelSubd::LineInterpolate(int faceIndex)
{
	int iResult = false;

	Vertex3D mid;

	for (int iEdge = 0 ; iEdge < 3 ; iEdge++)
	{
		if (m_VecFace3D[faceIndex].EdgeVertex[iEdge] == -1)///��������ϵñߵ㻹û������
		{
			//LineInterpolateMid(*midVertex,faceIndex,iEdge);
			LineInterpolateMid(mid,faceIndex,iEdge);
			//m_VecVers3D.push_back(*midVertex);
			mid.index = m_VecVers3D.size();
			m_VecVers3D.push_back(mid);
			m_VecFace3D[faceIndex].EdgeVertex[iEdge] = mid.index;
		}
	}
	m_VecFace3D[faceIndex].bSubd1To4 = true;
	
	///������Χ����
	for (int iEdge = 0 ; iEdge < 3 ; iEdge++)
		ModifyNeighborFace( faceIndex, iEdge );


	iResult = true;
	return iResult;
}
/*
* @param parentFace ��������ĸ��������ֵ
* @brief ����parentFaceIndex���ѳ��ĸ�����,�����뵽m_VecFace3D��ĩβ
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
void KModelSubd::CreateFourNewFaces( const ParentFace parentFace)
{
	Face3D newFace;
	int nIndex;
	//////////////////////////////////////////////////////////////////////////	
	nIndex = m_VecFace3D.size();
	newFace.index = nIndex;
	newFace.bSubd1To4 = false;
	newFace.bSubd1To2 = false;
	for ( int j = 0 ;j < 3 ; j++)
	{
		newFace.EdgeVertex[j] = -1;
		newFace.neighborFace[j] = -1;
	}
	newFace.parentFace = parentFace.index;
	newFace.iFaceNeighborN = 0;
	newFace.Vers[0] = parentFace.Vers[0];
	newFace.Vers[1] = parentFace.EdgeVertex[0];
	newFace.Vers[2] = parentFace.EdgeVertex[2];
	m_VecFace3D.push_back(newFace);
//////////////////////////////////////////////////////////////////////////
	nIndex = m_VecFace3D.size();
	newFace.index = nIndex;
	newFace.bSubd1To4 = false;
	newFace.bSubd1To2 = false;
	for ( int j = 0 ;j < 3 ; j++)
	{
		newFace.EdgeVertex[j] = -1;
		newFace.neighborFace[j] = -1;
	}
	newFace.parentFace = parentFace.index;
	newFace.iFaceNeighborN = 0;
	newFace.Vers[0] = parentFace.EdgeVertex[0];
	newFace.Vers[1] = parentFace.Vers[1];
	newFace.Vers[2] = parentFace.EdgeVertex[1];

	m_VecFace3D.push_back(newFace);
	//////////////////////////////////////////////////////////////////////////
	nIndex = m_VecFace3D.size();
	newFace.index = nIndex;
	newFace.bSubd1To4 = false;
	newFace.bSubd1To2 = false;
	for ( int j = 0 ;j < 3 ; j++)
	{
		newFace.EdgeVertex[j] = -1;
		newFace.neighborFace[j] = -1;
	}
	newFace.parentFace = parentFace.index;
	newFace.iFaceNeighborN = 0;
	newFace.Vers[0] = parentFace.EdgeVertex[2];
	newFace.Vers[1] = parentFace.EdgeVertex[1];
	newFace.Vers[2] = parentFace.Vers[2];

	m_VecFace3D.push_back(newFace);
	//////////////////////////////////////////////////////////////////////////
	nIndex = m_VecFace3D.size();
	newFace.index = nIndex;
	newFace.bSubd1To4 = false;
	newFace.bSubd1To2 = false;
	for ( int j = 0 ;j < 3 ; j++)
	{
		newFace.EdgeVertex[j] = -1;
		newFace.neighborFace[j] = -1;
	}
	newFace.parentFace = parentFace.index;
	newFace.iFaceNeighborN = 0;
	newFace.Vers[0] = parentFace.EdgeVertex[0];
	newFace.Vers[1] = parentFace.EdgeVertex[1];
	newFace.Vers[2] = parentFace.EdgeVertex[2];

	m_VecFace3D.push_back(newFace);
}

/*
* @param parentFace ��������ĸ��������ֵ
* @brief ����parentFaceIndex���ѳ���������,���뵽m_VecFace3D��ĩβ
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
void KModelSubd::CreateTwoNewFaces(const ParentFace parentFace)
{
	Face3D newFace;
	int nIndex;
	int nEdge;
	for ( int j = 0 ; j < 3 ; j++ )
	{
		if ( parentFace.EdgeVertex[j] != -1 )
		{
			nEdge = j;
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	nIndex = m_VecFace3D.size();
	newFace.index = nIndex;
	newFace.bSubd1To4 = false;
	newFace.bSubd1To2 = false;
	for ( int j = 0 ;j < 3 ; j++)
	{
		newFace.EdgeVertex[j] = -1;
		newFace.neighborFace[j] = -1;
	}
	newFace.parentFace = parentFace.index;
	newFace.iFaceNeighborN = 0;
	newFace.Vers[0] = parentFace.Vers[(nEdge+2)%3];
	newFace.Vers[1] = parentFace.Vers[nEdge];
	newFace.Vers[2] = parentFace.EdgeVertex[nEdge];

	m_VecFace3D.push_back(newFace);
	//////////////////////////////////////////////////////////////////////////
	nIndex = m_VecFace3D.size();
	newFace.index = nIndex;
	newFace.bSubd1To4 = false;
	newFace.bSubd1To2 = false;
	for ( int j = 0 ;j < 3 ; j++)
	{
		newFace.EdgeVertex[j] = -1;
		newFace.neighborFace[j] = -1;
	}
	newFace.parentFace = parentFace.index;
	newFace.iFaceNeighborN = 0;
	newFace.Vers[0] = parentFace.Vers[(nEdge+2)%3];
	newFace.Vers[1] = parentFace.EdgeVertex[nEdge];
	newFace.Vers[2] = parentFace.Vers[(nEdge+1)%3];

	m_VecFace3D.push_back(newFace);
}

/*
* @param parentFace ��������ĸ��������ֵ
* @brief �Ӹ���Clone���µ�������ȥ
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
void KModelSubd::CloneFromParentFace(const ParentFace parentFace)
{
	Face3D newFace;
	int nIndex;

	nIndex = m_VecFace3D.size();
	newFace.index = nIndex;
	newFace.bSubd1To4 = false;
	newFace.bSubd1To2 = false;
	for ( int j = 0 ;j < 3 ; j++)
	{
		newFace.EdgeVertex[j] = -1;
		newFace.neighborFace[j] = -1;
	}
	newFace.parentFace = parentFace.index;
	newFace.iFaceNeighborN = 0;
	newFace.Vers[0] = parentFace.Vers[0];
	newFace.Vers[1] = parentFace.Vers[1];
	newFace.Vers[2] = parentFace.Vers[2];

	m_VecFace3D.push_back(newFace);
}

/*
* @param void
* @brief ��KModelSubd��ȫ�ֱ����ĳ�ʼ����ֵ
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::InitializeSubd()
{
	int nResult = false;
	///����Mesh�и�����Ķ���,�ڽ���ĸ�����ֵ
	if (m_nSubdivisionN == 0)
	{
		for (int i = 0 ; i < m_iNumFaces ; i++ )
		{
			CalcFaceNormal(m_VecFace3D[i].Normal,i);

		}

		CalcVertexNeighborValue();
		
		if (m_lpTexture)
		{
			CalcTextureHighValue(m_lpTexture);
			ChangeMeshHigh();

		}
		SetMaxMinArea();
	}

	for (int i = 0 ; i < m_iNumFaces ; i++ )
	{
		
		
		m_VecFace3D[i].parentFace = -1;
		m_VecFace3D[i].bSubd1To2 = false;
		m_VecFace3D[i].bSubd1To4 = false;
		
		float fRefine = 0.0;
		fRefine = SetRefineParam(i);
		
		m_VecFace3D[i].RefineParam = fRefine;
		float fHigh;
		fHigh = SetHighDif(i);
		m_VecFace3D[i].HighDif =fHigh;
		
		float fArea;
		fArea = GetFaceArea(i);

	}

	nResult = true;
		return nResult;
}

/*
* @param void
* @brief ����ϸ����������
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::Refinement()
{
	int iResult = false;
	int iNumFace = m_iNumFaces;
	int iNumVers = m_iNumVers;
	int nNumNewFaces = 0;
	
	vector <ParentFace> vec_ParentFace;
	ParentFace tempParent;

	InitializeSubd();
	for (int i = 0 ; i < iNumFace ; i++ )
	{
		if ( m_VecFace3D[i].HighDif > HIGH_DIFFERENT_VALUE ) 
			if ( !IsSameHeightVertexofFace( i ) )
				LoopSubdivision(i);
		//else if ( ( m_VecFace3D[i].RefineParam < m_fMaxArea ) && ( m_VecFace3D[i].RefineParam > m_fMinArea ))
			//	LineInterpolate(i);
	} 
	m_iNumVers = m_VecVers3D.size();//�޸������е�ĸ���

	///������Ϣ�ȴ洢��parentFace��,�Ա��������������,�����˵���
	for (int i = 0 ; i < iNumFace ; i++ )
	{
		//Face3D * p3D ;
		//p3D = &m_VecFace3D[i];
		tempParent.bSubd1To2 = m_VecFace3D[i].bSubd1To2;
		tempParent.bSubd1To4 = m_VecFace3D[i].bSubd1To4;
		tempParent.index = m_VecFace3D[i].index;
		for (int j = 0 ; j < 3 ; j++)
		{
			tempParent.EdgeVertex[j]= m_VecFace3D[i].EdgeVertex[j];
			tempParent.NeighborFace[j] = m_VecFace3D[i].neighborFace[j];
			tempParent.Vers[j] = m_VecFace3D[i].Vers[j];
		}
		tempParent.Normal = m_VecFace3D[i].Normal;
		vec_ParentFace.push_back(tempParent);
	}

	///create new Faces
	m_VecFace3D.clear();
	
	ParentFace *pParentFace;
	for (int iFace = 0 ; iFace < iNumFace ; iFace++ )
	{
		pParentFace = &vec_ParentFace[iFace];
		if ( vec_ParentFace[iFace].bSubd1To4 == true ) 
		{
			//CreateFourNewFaces(vec_ParentFace[iFace],m_iNewFaces);			
			CreateFourNewFaces(vec_ParentFace[iFace]);			
			vec_ParentFace[iFace].ChildFace[0] = nNumNewFaces++;
			vec_ParentFace[iFace].ChildFace[1] = nNumNewFaces++;
			vec_ParentFace[iFace].ChildFace[2] = nNumNewFaces++;
			vec_ParentFace[iFace].ChildFace[3] = nNumNewFaces++;
		}
		else if (vec_ParentFace[iFace].bSubd1To2 == true)			
		{
			//CreateTwoNewFaces(vec_ParentFace[iFace],m_iNewFaces);
			CreateTwoNewFaces(vec_ParentFace[iFace]);
			vec_ParentFace[iFace].ChildFace[0] = nNumNewFaces++;
			vec_ParentFace[iFace].ChildFace[1] = nNumNewFaces++;
		}
		else
		{
			//CloneFromParentFace(vec_ParentFace[iFace],m_iNewFaces);
			CloneFromParentFace(vec_ParentFace[iFace]);
			vec_ParentFace[iFace].ChildFace[0] = nNumNewFaces++;
			//m_iNewFaces++;
		}

	}

	///build topology
	BuildAdjacency(vec_ParentFace);

	m_iNumFaces = nNumNewFaces;

	///���²����ĵ���潨����,iFaceN,��������ص�����
	CalcVertexNeighborValue();

	for (int iFace = 0 ; iFace < m_iNumFaces ; iFace++ )
	{	
		//CalcFaceNormal( m_VecFace3D[iFace].Normal,iFace);
		
		int nParent;
		nParent = m_VecFace3D[iFace].parentFace;

		CalcFaceNormal2( m_VecFace3D[iFace].Normal,vec_ParentFace[nParent].Normal);		
		
	}
	for ( int nVer = 0 ; nVer < m_iNumVers ; nVer++ )
	{
		CalcVertexNormal(m_VecVers3D[nVer].normal,nVer);
	}

	///���ݺ�ɫͨ���޸ĵ�ĸ߶�ֵ,
	if (m_lpTexture)
	{
		CalcTextureHighValue(m_lpTexture);
		ChangeMeshHigh();
	}
	m_nSubdivisionN++;
	iResult = true;


	return iResult;
}


/**
* @param & oddVertex ������
* @param faceIndex �������ֵ
* @param edgeIndex �ߵ�����ֵ
* @brief ��faceIndex�ϵĵ�edgeIndex���߼������ļ���λ��,ʹ��loop����
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::CalcOddVertex(Vertex3D & oddVertex,int faceIndex, int edgeIndex)
{
	int iResult =false;

	float x,y,z;
	float tu,tv;

	int iCurVertex,iNextVertex;///��oddVertex������������
	iCurVertex = m_VecFace3D[faceIndex].Vers[edgeIndex];
	iNextVertex = m_VecFace3D[faceIndex].Vers[( edgeIndex + 1 ) % 3 ];
	if (m_VecFace3D[faceIndex].neighborFace[edgeIndex] != -1)
	{//interior
		int ineighborFaceIndex = m_VecFace3D[faceIndex].neighborFace[edgeIndex];
		int iOtherVertex = m_VecFace3D[faceIndex].Vers[(edgeIndex + 2) %3];
		int iNeighborVertex;
		for (int i = 0 ; i < 3 ; i++ )		
			if ( (m_VecFace3D[ineighborFaceIndex].Vers[i] != iCurVertex) && ( m_VecFace3D[ineighborFaceIndex].Vers[i] != iNextVertex ) )
			{
				iNeighborVertex = m_VecFace3D[ineighborFaceIndex].Vers[i];
				break;
			}
		/*
		* The weight of iCurVertex,iNextVertex are  3/8 , and those of iOtherVertex and  iNeighborVertex are 1/8
		*/
		x = (float)( 3 * ( m_VecVers3D[iCurVertex].position.x + m_VecVers3D[iNextVertex].position.x )
			+ 1 * ( m_VecVers3D[iOtherVertex].position.x + m_VecVers3D[iNeighborVertex].position.x )) /8;
		y = (float)( 3 * ( m_VecVers3D[iCurVertex].position.y + m_VecVers3D[iNextVertex].position.y )  
			+ 1 * ( m_VecVers3D[iOtherVertex].position.y + m_VecVers3D[iNeighborVertex].position.y )) /8;
		z = (float)( 3 * ( m_VecVers3D[iCurVertex].position.z + m_VecVers3D[iNextVertex].position.z )  
			+ 1 * ( m_VecVers3D[iOtherVertex].position.z + m_VecVers3D[iNeighborVertex].position.z )) / 8;
		///��tu,tv��ֵ
		tu = (float)( 3 * ( m_VecVers3D[iCurVertex].tu + m_VecVers3D[iNextVertex].tu )
			+ 1 * ( m_VecVers3D[iOtherVertex].tu + m_VecVers3D[iNeighborVertex].tu )) /8;
		tv = (float)( 3 * ( m_VecVers3D[iCurVertex].tv + m_VecVers3D[iNextVertex].tv )
			+ 1 * ( m_VecVers3D[iOtherVertex].tv + m_VecVers3D[iNeighborVertex].tv )) /8;
	}
	else 
	{///boundary 
		x = (float) ( m_VecVers3D[iCurVertex].position.x + m_VecVers3D[iNextVertex].position.x ) /2 ;
		y = (float) ( m_VecVers3D[iCurVertex].position.y + m_VecVers3D[iNextVertex].position.y ) /2 ;
		z = (float) ( m_VecVers3D[iCurVertex].position.z + m_VecVers3D[iNextVertex].position.z ) /2 ;
		tu = (float)( m_VecVers3D[iCurVertex].tu + m_VecVers3D[iNextVertex].tu )/2;
		tv = (float)( m_VecVers3D[iCurVertex].tv + m_VecVers3D[iNextVertex].tv )/2;
	}

	oddVertex.position.x = x;
	oddVertex.position.y = y;
	oddVertex.position.z = z;
	oddVertex.tu = tu;
	oddVertex.tv = tv;
	//oddVertex.index = m_iNumVers++;
/**
	tu,tvֵ���ƺ�������
*/
	iResult= true;

	return iResult;
}

/**
* @param faceIndex �������ֵ
* @param vertexIndex �������ֵ
* @brief�޸�faceIndex�ϵĵ�vertexIndex ��even ��
* @return void
*/
void KModelSubd::CalcEvenVertex(int faceIndex, int vertexIndex)
{
	float x,y,z;
	x=y=z=0;
	int k ;
	float fBeta;
	float fAlfa;
	int iEvenVertex;
	iEvenVertex = m_VecFace3D[faceIndex].Vers[vertexIndex];///�õ���even�������ֵ
	k = m_VecVers3D[iEvenVertex].degree;
	//Vertex3D * p3D ;
	//p3D = & m_VecVers3D[iEvenVertex];

	fAlfa = 3/8 + 1/4 * cos((2*PI)/k);
	fBeta = (float) ( 5/8 - fAlfa * fAlfa )  /k;

	for (int iAdajance = 0 ; iAdajance < k ; iAdajance++)
	{
		int iVertex = m_VecVers3D[iEvenVertex].pVertexN[iAdajance];
		x +=(float) fBeta * m_VecVers3D[iVertex].position.x ;
		y +=(float) fBeta * m_VecVers3D[iVertex].position.y ;
		z +=(float) fBeta * m_VecVers3D[iVertex].position.z ;
	}
	x += (1 - k * fBeta ) * m_VecVers3D[iEvenVertex].position.x;
	y += (1 - k * fBeta ) * m_VecVers3D[iEvenVertex].position.y;
	z += (1 - k * fBeta ) * m_VecVers3D[iEvenVertex].position.z;
	
	m_VecVers3D[iEvenVertex].position.x = x ;
	m_VecVers3D[iEvenVertex].position.y = y ;
	m_VecVers3D[iEvenVertex].position.z = z ;
	/**
	*û���޸�tu,tvֵ,
	*/

}
/**
* @param & vertex ����Ķ�������
* @param faceIndex �������ֵ
* @param edgeIndex �ߵ�����ֵ
* @brief����faceIndex�ĵ�edgeIndex�������Բ���һ����vertex
* @remark ���д洢�˸õ��position, index, tu, tv ֵ
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::LineInterpolateMid(Vertex3D &vertex,int faceIndex, int edgeIndex)
{
	int iResult = false;
	Vertex3D  vertexCur;

	Vertex3D  vertexNext;

	int iCurIndex,iNextIndex;
	iCurIndex = m_VecFace3D[faceIndex].Vers[edgeIndex];
	iNextIndex = m_VecFace3D[faceIndex].Vers[(edgeIndex+1)%3];
	vertexCur =  m_VecVers3D[iCurIndex] ;
	vertexNext = m_VecVers3D[iNextIndex] ;
	MidVector(vertex.position,vertexCur.position,vertexNext.position);
	
	vertex.tu = ( vertexCur.tu + vertexNext.tu ) / 2 ;
	vertex.tv = ( vertexCur.tv + vertexNext.tv ) / 2 ;
	
	iResult = true;

	return iResult;
}

/**
 * @brief �޸�faceIndex�ϵĵ�edgeIndex ���ڽ����ϵ�odd vertex,
 * @param faceIndex �������ֵ
 * @param edgeIndex ��ı�����ֵ
 * @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
 * @remark ����һ���ݹ�,ֱ���޸�m_VecFace3D�е�����
 * 
**/
int KModelSubd::ModifyNeighborFace(int faceIndex, int edgeIndex)
{
	int iResult = false;
	int iNeighborFace = m_VecFace3D[faceIndex].neighborFace[edgeIndex];	///faceIndex��ĵ�edgeIndex���ϵ��ڽ���
	
	Vertex3D * newVertex = NULL;
	newVertex = new Vertex3D();
	if (!newVertex)
		goto Exit0;
	if (iNeighborFace != -1)
	{	
		if (m_VecFace3D[iNeighborFace].bSubd1To4 != true)
		{
			for (int i = 0 ; i < 3 ; i++ )
			{	
				///set the edge vertex of the neighbor face
				if (m_VecFace3D[iNeighborFace].neighborFace[i] == faceIndex)
				{
					m_VecFace3D[iNeighborFace].EdgeVertex[i] = m_VecFace3D[faceIndex].EdgeVertex[edgeIndex];
					break;
				}
			}
			if (m_VecFace3D[iNeighborFace].bSubd1To2 == true)
			{
				////����otherEdge
				int iOtherEdge;
				for (int i = 0 ; i < 3 ; i++ )
				{	
					///set the edge vertex of the neighbor face
					if  ( m_VecFace3D[iNeighborFace].EdgeVertex[i] == -1 ) 
					{
						iOtherEdge = i;					
						break;
					}					
				}
				
				if ( ( m_VecFace3D[iNeighborFace].HighDif <= HIGH_DIFFERENT_VALUE) ||
					( IsSameHeightVertexofFace( iNeighborFace ) ) )
				{///���ڽ��治�����ٴ�ϸ��������,��Ҫ����2-4ϸ�ֵ���
					LineInterpolateMid( *newVertex, iNeighborFace, iOtherEdge);
					newVertex->index = m_VecVers3D.size();
					m_VecVers3D.push_back(*newVertex);
					m_VecFace3D[iNeighborFace].EdgeVertex[iOtherEdge] =  newVertex->index;
					m_VecFace3D[iNeighborFace].bSubd1To4 = true;
					ModifyNeighborFace(iNeighborFace, iOtherEdge);
				}

			}
			else 
			{
				m_VecFace3D[iNeighborFace].bSubd1To2 = true;
			}
		}
		iResult = true;
	}
	else 
	{
		goto Exit0;
	}	
	
Exit0:
	if ( newVertex )
		delete newVertex;

	return iResult;
}

/**
* @param faceIndex �������ֵ
* @brief ����faceIndex����loopϸ��
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
* @remark �ڸú�����,ֻ�����ɱߵ�(odd vertex),���޸� even vertex
*         ���������µ���ȵ����е��涼loop����,������.�Ա�����ٽ�����ж�
**/

int KModelSubd::LoopSubdivision(int faceIndex)
{
	int iResult;
	int iNumVers = m_iNumVers;
	int iNumFaces = m_iNumFaces;

	//Face3D face3d;
	Vertex3D oddVertex3D;
	for (int iEdge = 0 ; iEdge < 3 ; iEdge++)
	{
		if (m_VecFace3D[faceIndex].EdgeVertex[iEdge] == -1)///��������ϵñߵ㻹û������
			//if ( CalcOddVertex( oddVertex3D, faceIndex,iEdge ) != false)
			{
				//oddVertex->index = m_iNumVers++;///���²����ĵ������еĺ���
				CalcOddVertex( oddVertex3D, faceIndex,iEdge );
				oddVertex3D.degree = 0;
				oddVertex3D.iFaceN = 0;
				oddVertex3D.pVertexN.clear();
				oddVertex3D.pFaceN.clear();
				oddVertex3D.index = m_VecVers3D.size();
				m_VecVers3D.push_back(oddVertex3D);
				m_VecFace3D[faceIndex].EdgeVertex[iEdge] = oddVertex3D.index;
				///������������
			}
	}

	for (int iVertex = 0 ; iVertex < 3 ; iVertex++)
		 CalcEvenVertex(faceIndex,iVertex); 

	///�ø����Ѿ�1-4ϸ����
	m_VecFace3D[faceIndex].bSubd1To4 = true;
	
	// face3d = m_VecFace3D[faceIndex];

	for (int iEdge = 0 ; iEdge < 3 ; iEdge++)
		if (m_VecFace3D[faceIndex].neighborFace[iEdge] != -1)
			 ModifyNeighborFace( faceIndex, iEdge );
	
	iResult = true;

	return iResult;

}
/**
* @param void
* @brief render������
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
**/
HRESULT KModelSubd::Render()
{
	DrawMesh(m_pMeshSubdSys);
	//g_pd3dDevice->SetTexture(0,NULL);
	//DrawMeshSubset(m_pMeshSubdSys,0);
	//m_pWaterUp->DrawSubset(0);

	return S_OK;
}
/**
* @param void
* @brief �������ݵ�m_pMeshSubdSys
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::SaveSubdMesh()
{
	int iResult = false;
	
	VFormat::FACES_NORMAL_TEXTURE1 * pVers = NULL;
	WORD* pIndex = NULL;
	DWORD * pAttrib = NULL;	

	if (FAILED(D3DXCreateMeshFVF(m_iNumFaces,m_iNumVers,D3DXMESH_MANAGED,
		(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1),g_pd3dDevice,&m_pMeshSubdSys )))
	{
		goto Exit0;
	}
	if (FAILED(m_pMeshSubdSys->LockVertexBuffer(0,(void**)&pVers)))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->LockIndexBuffer (0,(void**)&pIndex)))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->LockAttributeBuffer(0,(DWORD**)&pAttrib)))
		goto Exit0;	

	///��ֵ����
	for ( int iVers = 0 ; iVers < m_iNumVers ; iVers++ )
	{
		pVers[iVers].Normal = m_VecVers3D[iVers].normal;
		pVers[iVers].p = m_VecVers3D[iVers].position;
		pVers[iVers].tu1 = m_VecVers3D[iVers].tu;
		pVers[iVers].tv1 = m_VecVers3D[iVers].tv;		
	}
	for (int iFace = 0 ; iFace < m_iNumFaces ; iFace++ )
	{
		pIndex[3*iFace] = m_VecFace3D[iFace].Vers[0];
		pIndex[3*iFace+1] = m_VecFace3D[iFace].Vers[1];
		pIndex[3*iFace+2] = m_VecFace3D[iFace].Vers[2];
		pAttrib[iFace] = 0;
	}
	if (FAILED(m_pMeshSubdSys->UnlockVertexBuffer()))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->UnlockIndexBuffer()))
		goto Exit0;
	if (FAILED(m_pMeshSubdSys->UnlockAttributeBuffer()))
		goto Exit0;

	iResult = true;
Exit0:

	return iResult;
}
/**
* @param pTexture �������ͼ
* @brief ���ݵ����Texture�еĸ����RGBֵ,����������͹���ĸ߶�
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::CalcTextureHighValue( LPDIRECT3DTEXTURE9 pTexture)
{
	int nResult = false;
	if (!pTexture)
		goto Exit0;
	D3DSURFACE_DESC Desc;
	Desc.Width =0;/// width of texture 
	Desc.Height =0;/// height of texture

	if (FAILED(pTexture->GetLevelDesc(0,&Desc)))///�õ���0��ͼ��desc��Ϣ
		goto Exit0;
	if ((Desc.Format!=D3DFMT_A8R8G8B8)&&(Desc.Format!=D3DFMT_X8R8G8B8))
		goto Exit0;	

	D3DLOCKED_RECT lockedRect;
	lockedRect.pBits = NULL;

	if (FAILED(pTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_READONLY )))
		goto Exit0;

	DWORD* pTextData = (DWORD*) lockedRect.pBits;
	for (int nVertex = 0 ; nVertex < m_iNumVers; nVertex++ )
	{
		float fRow,fColumn;///�к���
		fColumn= m_VecVers3D[nVertex].tu *Desc.Width;
		fRow = m_VecVers3D[nVertex].tv * Desc.Height;

		DWORD * pdwTemp = (DWORD *)pTextData +(DWORD) fRow * lockedRect.Pitch/4 + (DWORD)fColumn;
		unsigned char uncRed;
		uncRed = (BYTE)((*pdwTemp & 0x00ff0000)>>16);
		float fRed;
		fRed = (float)uncRed/255;
		m_VecVers3D[nVertex].fHeight = fRed;
	}
	pTexture->UnlockRect(0);

	nResult = true;
Exit0:
	return nResult;
}
/**
* @param pTexture �������ͼ
* @brief ����絼��texture
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
**/
int KModelSubd::LoadTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	if ( pTexture)
	{
		m_lpTexture = pTexture;
		return true;
	}
	else
		return false;
}

/**
* @param void
* @brief �Գ�ʼ����,����ÿ�����fHeight����,�ı�����position��ֵ,ʹÿ�����ɸ߶�ֵ���ڼ����ϸı�λ��
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
*/
int KModelSubd::ChangeMeshHigh()
{
	D3DXVECTOR3 vectorHeight ;
	D3DXVECTOR3 vectorPosition ;

	for (int i = 0; i < m_iNumVers ; i++ )
	{

		//m_VecVers3D[i].position.y =   m_nHeight * abs(m_VecVers3D[i].fHeight);
		float fPositiony;

		if ( m_VecVers3D[i].fHeight > 0.05 )
		{ 

			fPositiony = m_VecVers3D[i].position.y + m_nHeight * m_VecVers3D[i].fHeight 
				- m_VecVers3D[i].position.y * m_VecVers3D[i].normal.y;
			if (fPositiony > m_VecVers3D[i].position.y)
				m_VecVers3D[i].position.y = fPositiony;
		}
		else 

		{
			m_VecVers3D[i].position.y = m_VecVers3D[i].position.y;
		}

		//m_VecVers3D[i].position.z = m_VecVers3D[i].position.z + m_nHeight * m_VecVers3D[i].Height.z 
		//	- m_VecVers3D[i].position.z * m_VecVers3D[i].normal.z;	

	}   

	return true;
}

/**
* @param void
* @brief ���������е�������С���
* @return false ��ʾʧ�ܣ�true��ʾ�ɹ�
**/
int KModelSubd::SetMaxMinArea()
{
	float fMax,fMin,fCur;
	fMax = fMin = GetFaceArea(0);
	for ( int i = 1 ; i < m_iNumFaces ; i++ )
	{
		fCur = GetFaceArea(i);
		if ( fCur > fMax )
			fMax = fCur;
		if ( fCur < fMin )
			fMin = fCur;
	}
	m_fMinArea = fMin;
	m_fMaxArea = fMax;

	return true;
}

/**
* @param faceIndex ��ʾ�������ֵ
* @brief �ж�һ�����ͨ��ͨ��ȷ���Ķ���߶�ֵ�Ƿ����
* @return false ��ʾʧ�ܣ�true ��ʾ�ɹ�
**/
int KModelSubd::IsSameHeightVertexofFace( int  faceIndex)
{
	int nResult = false;
	float  fVertexHeight;
	int nVertexIndex;
	nVertexIndex = m_VecFace3D[faceIndex].Vers[0];
	fVertexHeight = m_VecVers3D[nVertexIndex].fHeight;
	for ( int nVertex = 0; nVertex< 3 ; nVertex++ )
	{
		nVertexIndex = m_VecFace3D[faceIndex].Vers[nVertex];
		fVertexHeight = m_VecVers3D[nVertexIndex].fHeight;
		if ( fVertexHeight > m_fVertexHeightDif ) ///��������˸ø߶�ֵ����Ϊ����Ķ���ͨ���߶�ֵ����ͬ
		{			
			return nResult;
		}
	}
	nResult = true;
	return nResult;
}

HRESULT KModelSubd::Acquire()
{
	//	if (!m_bNeedReAcquire)
	//		return S_OK;

	PropertyBase::_PropertyPage* pPage = NULL;
	int PageIndex = -1;

	PageIndex = this->AddPage("Information");

	pPage = this->FindPage(PageIndex);
	if(pPage)
	{
		PropertyBase::_PropertyGroup Group(0,"����ϸ��",FALSE);
		int index_Group = pPage->AddGroup(Group);
		PropertyBase::_PropertyGroup* pGroup = pPage->FindGroup(index_Group);

		pGroup->AddPropertyFloat("�����Զ��",&m_fCameroLength,0.0f,100,1);

	}
	//	m_bNeedReAcquire = FALSE;
	return S_OK;
}
