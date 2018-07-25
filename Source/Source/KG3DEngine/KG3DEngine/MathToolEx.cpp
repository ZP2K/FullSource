////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : MathToolEx.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-5-22 11:47:59
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MathToolEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//KG3DPloy/////////////huangjinshou//////////////////////////////////////////////////
BOOL KG3DPoly::IsPointIn(D3DXVECTOR3 vPos,BOOL bTestHeight) 
{
	const int MAX = 1024 ;
	static D3DXVECTOR2 p[MAX];
	size_t n = 0;
	size_t i;
	int sum;
	if(bTestHeight)
	{
		if(!m_BBox.IsPointInside(vPos))
			return FALSE;
	}
	
	for(i = 0,n = 0;n < m_vecVertex.size() ; n++,i++)//��������ԭ���Ƶ����Ե��λ��
	{
		p[i].x = m_vecVertex[n].x - vPos.x; //���ﲻ���Ը�ΪvPos.x - m_vecVertex[n].x����������ļ��������ܴ���
		p[i].y = m_vecVertex[n].z - vPos.z; //���ﲻ���Ը�ΪvPos.x - m_vecVertex[n].x����������ļ��������ܴ���
	}
	n = m_vecVertex.size();
	p[n] = p[0];
	//BOOL bIn = FALSE;
	for (sum = 0,i =0 ; i < n; i++)
	{
	/*	if((fabs(p[i].x)<=0.00001F)&&(fabs(p[i].y)<=0.00001F))
		{
			bIn = TRUE;
			break;
		}*/
	
		if( (int)p[i].x == 0 && (int)p[i].y == 0) //�ڶ�����
			return TRUE; 
		else if (p[i].y - p[i+1].y != 0 && p[i].y * p[i+1].y < 0) //���㲻��X���ϣ��Ҷ�������һ�㶥����X��ͬһ��
		{
			float X = p[i].y *(p[i+1].x - p[i].x) / (p[i].y-p[i+1].y) +p[i].x;//�������������ɵ�����X��Ľ���
			if (X >= 0 && X >= min(p[i].x,p[i+1].x) && X <= max(p[i].x,p[i+1].x))//������X���������ϣ��������������ɵ��߶��ϣ�������1
			{
				sum++;
			}
		}	
	}
	//if(bIn)
	//	return TRUE;
	if((sum%2) == 1)//���Ե㷢�������������εıߵĽ���Ϊ������õ��ڶ������
		return TRUE;

	return FALSE;
}

HRESULT KG3DPoly::Clear()
{
	m_vecVertex.clear();
	m_BBox.Clear();
	return S_OK;
}

HRESULT KG3DPoly::AddVertex(D3DXVECTOR3& vPos)
{
	m_vecVertex.push_back(vPos);
	m_BBox.AddPosition(vPos);
	m_BBox.AddPosition(vPos+D3DXVECTOR3(0,m_fHeight,0));
	return S_OK;
}

HRESULT KG3DPoly::AddVertex(std::list<D3DXVECTOR3>*plistPos)
{
	Clear();

	list<D3DXVECTOR3>::iterator i = plistPos->begin();
	list<D3DXVECTOR3>::iterator iend = plistPos->end();
	while (i!=iend)
	{
		D3DXVECTOR3 vPos = *i;
		AddVertex(vPos);
		i++;
	}
	return S_OK;
}

HRESULT KG3DPoly::SetHeight(float fH)
{
	m_fHeight = fH;
	return S_OK;
}

HRESULT KG3DPoly::ComputeBBox()
{
	m_BBox.Clear();
	for(size_t i=0;i<m_vecVertex.size();i++)
	{
		D3DXVECTOR3 vPos = m_vecVertex[i];
		m_BBox.AddPosition(vPos);
		m_BBox.AddPosition(vPos+D3DXVECTOR3(0,m_fHeight,0));
	}
	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////

