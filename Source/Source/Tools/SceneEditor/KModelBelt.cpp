/*
**************************************************************************
*
* Project		: SceneEditor
* FileName		: KModelBelt.cpp
* Coded by		: HJQ
* Date			: 2004-07-30
* Comment	    : Ʈ����
* Copyright 2004 Kingsoft Corp.
* All Rights Reserved.
*
**************************************************************************
*/

/*
**************************************************************************
*	
* Modification History
* ====================
*
* Date      Programmer        Description
* ------------------------------------------------------------------------
* 04-08-07  HJQ				�������Ҳ�,
*							������Ʈ���˶�״̬�ͷ��˶�״̬������߶εķ���
*
**************************************************************************
*/

#include "stdafx.h"
#include ".\kmodelbelt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace ModelSFX
{

KModelBelt::KModelBelt(void)
{
	m_lpVBuf = NULL;

//	m_fMaxDist = 0.0f;
	m_BaseLine.A = D3DXVECTOR3( 0,10,0);
	m_BaseLine.B = D3DXVECTOR3( 0,100,0);

	m_fMaxDist =  5.0f;
	m_fMaxLength = 500.0f;
	m_nMaxLineBuffer = 900;

	m_bGotOne = TRUE;
	m_nNumFaces = 0;

	m_bShowWind = TRUE;
	m_WindForce = D3DXVECTOR3(0,0,0);
	m_Gravity = D3DXVECTOR3(0,9.8f,0);
//	m_Gravity = D3DXVECTOR3(0,0,0);

	m_bExtend = TRUE;
	m_fExtendNum = 10.0f;

	m_bShowSin = TRUE;
	m_fSwing = 30.0f;		
	m_fCyc = 50.0f;

	m_bShow = FALSE;

	m_nListMaxSize = 300;
}

KModelBelt::~KModelBelt(void)
{
}

HRESULT KModelBelt::Acquire()
{
	if (!m_bNeedReAcquire)
		return S_OK;

	PropertyBase::_PropertyPage* pPage = NULL;
	int PageIndex = -1;

	PageIndex = this->AddPage("Information");

	pPage = this->FindPage(PageIndex);
	if(pPage)
	{
		PropertyBase::_PropertyGroup Group(0,"Ʈ��",FALSE);
		int index_Group = pPage->AddGroup(Group);
		PropertyBase::_PropertyGroup* pGroup = pPage->FindGroup(index_Group);


		pGroup->AddPropertyFloat("Ʈ������",&m_fMaxLength,1,1000,1);	
		pGroup->AddPropertyFloat("ƽ����ֵԽС��Խƽ����",&m_fMaxDist,1,100,1);	

		pGroup->AddPropertyBool( "ʹƮ��β��Ʈ���Ӿ�", &m_bExtend );
		pGroup->AddPropertyFloat("Ʈ��β��Ʈ���Ӿ�ϵ��", &m_fExtendNum, 0,500,1);
		
		pGroup->AddPropertyBool( "ʹ�����Ҳ�", &m_bShowSin );

		pGroup->AddPropertyFloat("���Ҳ����", &m_fSwing, 1,100,1);
		pGroup->AddPropertyFloat("�����ڵ���ϵ��", &m_fCyc, 0,100,1);

		pGroup->AddPropertyBool( "ʹ�÷���", &m_bShowWind );
		pGroup->AddPropertyFloat("����X����", &m_WindForce.x, -50,50,1);
		pGroup->AddPropertyFloat("����Y����", &m_WindForce.y, -50,50,1);
		pGroup->AddPropertyFloat("����Z����", &m_WindForce.z, -50,50,1);

		
		pGroup->AddPropertyFloat("����Y����", &m_Gravity.y, 1.0f,10.0f,0.1f);
		
		pGroup->AddPropertyFloat("BaseLine.A", &m_BaseLine.A.y, -1000,1000,1);
		pGroup->AddPropertyFloat("BaseLine.B", &m_BaseLine.B.y, -1000,1000,1);

	}
	m_bNeedReAcquire = FALSE;
	return S_OK;
}


HRESULT KModelBelt::LoadMesh(LPSTR pFileName)
{
	CreateVerticesBuffer();
	m_dNumMaterial = 1;
	m_lpMaterial = new MATERIAL[m_dNumMaterial];
	ZeroMemory(m_lpMaterial,sizeof(MATERIAL)*m_dNumMaterial);
	
	DWORD Def_Option = MATERIAL_OPTION_ZBUFFER_TRUE|
		MATERIAL_OPTION_FILL_SOLID|
		MATERIAL_OPTION_SHADE_GOURAUD|
		MATERIAL_OPTION_CULL_NONE|
		MATERIAL_OPTION_SPECULARENABLE;
	
	for(DWORD i=0;i<m_dNumMaterial;i++)
	{
		m_lpMaterial[i].m_sMaterial9.Diffuse.r = 0.7f ;
		m_lpMaterial[i].m_sMaterial9.Diffuse.g = 0.7f ;
		m_lpMaterial[i].m_sMaterial9.Diffuse.b = 0.7f ;
		m_lpMaterial[i].m_sMaterial9.Diffuse.a = 1.0f ;
		m_lpMaterial[i].m_sMaterial9.Ambient = m_lpMaterial[i].m_sMaterial9.Diffuse;
		m_lpMaterial[i].m_sMaterial9.Specular = m_lpMaterial[i].m_sMaterial9.Diffuse;
		m_lpMaterial[i].m_sMaterial9.Power = 15;
		m_lpMaterial[i].m_dOption = Def_Option;
	}

	TCHAR Name[256];
//	wsprintf(Name,"%s\\Blade.Mtl",g_Def_ModelDirectory);
	wsprintf(Name,"%s\\����.Mtl",g_Def_ModelDirectory);
	LoadMaterial(Name);
	
	wsprintf(m_szMeshFileName,"Sword Blade");
	return S_OK;
}

HRESULT KModelBelt::Render()
{
	SetMatrix();

	D3DXMATRIX MatCur = m_Matrix;
	g_pd3dDevice->GetTransform( D3DTS_WORLD , &MatCur);
	D3DXVECTOR3  A,B;
	D3DXVec3TransformCoord(&A,&m_BaseLine.A,&MatCur);
	D3DXVec3TransformCoord(&B,&m_BaseLine.B,&MatCur);

	m_CurLine.A = A;
	m_CurLine.B = B;


	if( m_bGotOne )		//��ʼ��Ʈ��
	{
		InitialBelt(m_CurLine);
		m_bGotOne = FALSE;
	}
/*	
	if(!m_bShow)		//���˶�ʱƮ����״̬
	{

		SRCLINE LineA = m_SrcLineList.front();

		UpdateAtRest(m_CurLine);
		UpdateExtend();

		LineA = m_SrcLineList.front();


		int j = 0;
	}
	else				//�˶�ʱƮ����״̬
	{
		AddLine(m_CurLine);

	}
*/


//	AddLine(m_CurLine);
//	UpdateAtRest(m_CurLine);
	AddLine(m_CurLine);

	if(m_bExtend)
	{
		UpdateExtend();
	}

	D3DXMATRIX MatId;
	D3DXMatrixIdentity(&MatId);
	g_pd3dDevice->SetTransform( D3DTS_WORLD , &MatId);

	UpdateVertices();  

	KModel::SetMaterial(0);
	g_pd3dDevice->SetStreamSource( 0, m_lpVBuf,0, sizeof( VFormat::FACES_DIFFUSE_TEXTURE1 ) );
	g_pd3dDevice->SetFVF( D3DFVF_Faces_Diffuse_Texture1 );

//	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);	
	g_pd3dDevice->DrawPrimitive  ( D3DPT_TRIANGLESTRIP , 0 , m_nNumFaces);

//	SRCLINE LineK = m_vecSrcLine[0];

	RestoreMatrix();

	return S_OK;
}

HRESULT KModelBelt::CreateVerticesBuffer()
{
	SAFE_RELEASE(m_lpVBuf);
	if ( FAILED( g_pd3dDevice->CreateVertexBuffer( m_nMaxLineBuffer*2*sizeof(VFormat::FACES_DIFFUSE_TEXTURE1),
			0,D3DFVF_Faces_Diffuse_Texture1,
			D3DPOOL_MANAGED, &m_lpVBuf,NULL ) ) )
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
  * @brief ���ݳ�ʼ���߶Σ���ʼ��Ʈ��
  * @param Line ��ʼ���߶�
  */
HRESULT KModelBelt::InitialBelt(SRCLINE Line)
{
	if ( m_fMaxDist <= 0.1f )
		return E_FAIL;

	int InsertLineNum = (int)(m_fMaxLength/m_fMaxDist);

	SRCLINE LineA = Line;

	for( int nStep = 0; nStep<=InsertLineNum; nStep++ )
	{
		LineA.A.y -= m_fMaxDist;
		LineA.B.y -= m_fMaxDist;

		m_SrcLineList.push_back(LineA);
	}

	return S_OK;
}



/**
  * @brief ����Ʈ����󳤶ȣ���m_SrcLineList��ѡȡһ���������߶μ�¼��m_vecSrcLine����ͬʱ����ֵ����
  */
HRESULT	KModelBelt::UpdateBladeLine(void)
{
	if ( m_SrcLineList.empty() )
		return E_FAIL;

	if ( m_fMaxDist <= 0.1f )
	{
		return E_FAIL;
	}

	m_vecSrcLine.clear();

	D3DXVECTOR3 TA1 = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 TA2 = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 TB1 = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 TB2 = D3DXVECTOR3(0,0,0);

	float fTotalLength = 0.0f;

	list<SRCLINE>::iterator i = m_SrcLineList.begin();

	SRCLINE LastLine;

	LastLine = *i;


	while(i!=m_SrcLineList.end())
	{
		SRCLINE LineA = *i;
		list<SRCLINE>::iterator k = i;
		k++;

//		fTotalLength += max(D3DXVec3Length(&(LineA.A - LastLine.A)),D3DXVec3Length(&(LineA.B - LastLine.B)));
		fTotalLength += 0.5f*(D3DXVec3Length(&(LineA.A - LastLine.A)) + D3DXVec3Length(&(LineA.B - LastLine.B)));
		if(fTotalLength>m_fMaxLength)
		{
			return S_OK;
		}

		m_vecSrcLine.push_back( LineA );
		LastLine = LineA;
		i++;

		if (k!=m_SrcLineList.end())
		{
			SRCLINE LineB = *k;
			
			k++;
			if (k!=m_SrcLineList.end())
			{
				SRCLINE NextLine = *k;
				TA2 = NextLine.A - LineB.A;
				TB2 = NextLine.B - LineB.B;
			}

			float fCurMaxLength = min(D3DXVec3Length(&(LineA.A - LineB.A)),D3DXVec3Length(&(LineA.B - LineB.B)));

			if(( fCurMaxLength>m_fMaxDist ))
			{
				int InsertLineNum = (int)(fCurMaxLength /  m_fMaxDist) ;

				for( int nStep=InsertLineNum; nStep > 0 ; nStep-- )
				{

					SRCLINE LineC;
					float s = 1.0f-(float)(nStep)/(InsertLineNum+1);

					float m_fTangent = 1.0f;

					TA1 *=m_fTangent;
					TA2 *=m_fTangent;
					TB1 *=m_fTangent;
					TB2 *=m_fTangent;

					D3DXVec3Hermite( &LineC.A, &LineA.A , &TA1, &LineB.A, &TA2, s );
					D3DXVec3Hermite( &LineC.B, &LineA.B , &TB1, &LineB.B, &TB2, s );

//					fTotalLength += max(D3DXVec3Length(&(LineC.A - LastLine.A)),D3DXVec3Length(&(LineC.B - LastLine.B)));
					fTotalLength += 0.5f*(D3DXVec3Length(&(LineC.A - LastLine.A)) + D3DXVec3Length(&(LineC.B - LastLine.B)));
					if(fTotalLength>m_fMaxLength)
					{
						return S_OK;
					}

					m_vecSrcLine.push_back(LineC);
					LastLine = LineC;
				}
			}

			TA1 = LineB.A - LineA.A;
			TB1 = LineB.B - LineA.B;
		}

	}

	return S_OK;
}


/**
  * @brief ��m_vecSrcLine��¼�Ķ��㣬��˳��д��m_lpVBuf�����������δ�
  */
HRESULT KModelBelt::UpdateVertices(void)
{
	UpdateBladeLine();

	if( m_vecSrcLine.empty() )
		return E_FAIL;

	VFormat::FACES_DIFFUSE_TEXTURE1 * pVer = NULL;
	if ( FAILED( m_lpVBuf->Lock( 0, 0, (void**)&pVer, 0 ) ) )
		return E_FAIL;

	D3DCOLOR Color = 0xFFFFFFFF;	

	int nVecSize = m_vecSrcLine.size();

	if( nVecSize > m_nMaxLineBuffer )	
		nVecSize = m_nMaxLineBuffer;

	float Step1 = 1.0f/(float)(nVecSize-1);
//	float Step2 = 1.0f /(float)nVecSize;

	for( int nStep=0; nStep<nVecSize; nStep++ )
	{
		SRCLINE LineA = m_vecSrcLine[nStep];
		
		D3DXVECTOR3 A = LineA.A;
		D3DXVECTOR3 B = LineA.B;

		float fTu =  Step1* (float)nStep;
		if( fTu>=0.99f )
			fTu = 0.9f;

		pVer[2*nStep].p = A;pVer[2*nStep].diffuse = Color;pVer[2*nStep].tu1 = fTu;pVer[2*nStep].tv1 = 0;
		pVer[2*nStep+1].p = B;pVer[2*nStep+1].diffuse = Color;pVer[2*nStep+1].tu1 = fTu;pVer[2*nStep+1].tv1 = 1;
	
//		g_cGraphicsTool.DrawPosition( &A);
//		g_cGraphicsTool.DrawPosition( &B);
	}

	m_nNumFaces = 2*(nVecSize-1);

	m_lpVBuf->Unlock();
	return S_OK;
}


/**
  * @brief �˶�ʱΪƮ���������߶�
  * @param Line ���߶�
  */
/*
HRESULT KModelBelt::AddLine(SRCLINE Line)
{
	SRCLINE LineA = Line;
	SRCLINE LineB = m_SrcLineList.front();
	float fCurLength = max(D3DXVec3Length(&(LineA.A - LineB.A)),D3DXVec3Length(&(LineA.B - LineB.B)));
	
	if( fCurLength < m_fMaxDist )
	{
		m_SrcLineList.pop_front();
	}
	
	m_SrcLineList.push_front(Line);

	if( m_SrcLineList.size() > 100 )
	{
		m_SrcLineList.pop_back();
	}

	return S_OK;
}
*/


/**
  * @brief ΪƮ���������߶�
  * @param Line ���߶�
  */
//HRESULT KModelBelt::UpdateAtRest(SRCLINE Line)
HRESULT KModelBelt::AddLine(SRCLINE Line)
{

//	SRCLINE LineA = Line;
//	SRCLINE LineB = m_SrcLineList.front();

//	D3DXVECTOR3 Dist = D3DXVECTOR3(0,9.8f,0);
//	D3DXVECTOR3 Dist = D3DXVECTOR3(5.0f,9.8f,5.0f);
	D3DXVECTOR3 Dist = D3DXVECTOR3(0,0,0);
	
	if(m_bShowWind)
	{
		Dist = m_Gravity + m_WindForce;
	}
	else
	{
		Dist = m_Gravity;
	}

//	Dist = D3DXVECTOR3(0,0,0);

	//D3DXVec3Length(&Dist) ֵ����Ϊ0
	if( D3DXVec3Length(&Dist) > 1.0f )
	{
		list<SRCLINE>::iterator i = m_SrcLineList.begin();

		//������������������Ҳ�
		if(( i != m_SrcLineList.end() )&&(m_bShowSin))
		{
			SRCLINE LineA = *i;
			D3DXVECTOR3 Dist1 =	Dist + LineA.A;

			D3DXPLANE  plane1;

			D3DXPlaneFromPoints(&plane1, &LineA.A, &LineA.B, &Dist1 );

			D3DXVECTOR3	N;
			N.x	= plane1.a;
			N.y	= plane1.b;
			N.z	= plane1.c;

			D3DXVec3Normalize(&N,&N);

			float fSwing = 3.0f;
			float fCyc = m_fCyc * 0.0001f;

			fSwing  = m_fSwing * 0.1f;
			(*i).A += fSwing*sinf(timeGetTime() * fCyc) * N;
			(*i).B += fSwing*sinf(timeGetTime() * fCyc) * N;
		}

		while ( i != m_SrcLineList.end() )
		{
			//		SRCLINE LineX = *i;

			(*i).A -= Dist;
			(*i).B -= Dist;

			i++;
		}

	}
	else
		return E_FAIL;

	m_SrcLineList.push_front(Line);

	if( (int)m_SrcLineList.size() > m_nListMaxSize )
	{
		m_SrcLineList.pop_back();
	}

	return S_OK;
}


/**
  * @brief �Ŵ�Ʈ��ĩ�˵��˶�
  */
HRESULT KModelBelt::UpdateExtend(void)
{
	if( m_SrcLineList.empty() )
		return E_FAIL;

	SRCLINE LineA =m_SrcLineList.front();

	D3DXVECTOR3 Dist = D3DXVECTOR3(0,0,0);
	
	if(m_bShowWind)
	{
		Dist = m_Gravity + m_WindForce;
	}
	else
	{
		Dist = m_Gravity;
	}
	
	//D3DXVec3Length(&Dist)ֵ����Ϊ0
	if( D3DXVec3Length(&Dist) > 1.0f )
	{
		Dist += LineA.A;

		D3DXPLANE  plane1;

		D3DXPlaneFromPoints(&plane1, &LineA.A, &LineA.B, &Dist );

		D3DXVECTOR3 N;
		N.x = plane1.a;
		N.y = plane1.b;
		N.z = plane1.c;

		D3DXVec3Normalize(&N,&N);

		list<SRCLINE>::iterator i = m_SrcLineList.begin();
		i++;
		SRCLINE* LineX;

		D3DXVECTOR3 offset;

		float kk = 0.001f;

		kk = m_fExtendNum / 10000.0f;
		int nStep = 0;

		while ( i != m_SrcLineList.end() )
		{
			LineX = &(*i);
			//		float jj = nStep*kk;

			offset = N * D3DXPlaneDotCoord( &plane1, &LineX->A) * kk * (float)nStep;

			LineX->A += offset;

			offset = N*D3DXPlaneDotCoord( &plane1, &LineX->B)*kk*(float)nStep;

			LineX->B +=offset;

			i++;
			nStep++;
		}

		LineX = NULL;
	}
	else
		return E_FAIL;

	return S_OK;
}

};
