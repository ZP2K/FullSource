#pragma once

#include <list>
#include "types.h"
#include "inireader.h"
#include "kbarrier.h"
#include "KLauncher.h"
#include "kforcefield.h"
#define MAX_LAUNCHER 20


namespace PropertyBase
{
class KDlg_PS_Main;
}

namespace ModelSFX
{

	//�õ����ڵ�ʱ�䣬 ����Ϊ��λ
	inline float GetCurrentTime()
	{
		return (float)timeGetTime() / 1000;
	}

	//�õ�һ��fMin<x<fMax���������
	inline float GetRandom(float fMin, float fMax)
	{
		float fRandNum = (float)rand() / RAND_MAX;
		return (fMin + (fMax - fMin ) * fRandNum);
	}

	
	//�õ�����ķ��ţ�1 |��������
	inline int GetRandomSgn()
	{
		int iTemp = rand();
		if ( (iTemp % 2) == 1 )
			return -1;
		else return 1;
	}

class KParticleSystem
{
//////////////////////////////////////////////////////////////////////////
//For Editor
	friend class PropertyBase::KDlg_PS_Main;
//For Editor end

public:
	HRESULT LoadFFFromFile(LPCSTR strFileName);
	HRESULT SaveForFieldToFile(LPCSTR strFileName);

	float Dif_Sin(float, float,float);
	KParticleSystem();
	~KParticleSystem();
	
	bool SaveLaunParToFile(LPCSTR strFileName, int iIndex);
	bool LoadLaunFromFile(LPCSTR strFileName);
	virtual void Init();//��ʼ��
	//���Ʒ�����
	void SetLauncher(int iLaunOffset, bool bStatus) 
	{ 
		if (bStatus)
		{
			float fTime = GetCurrentTime(); 
			m_vecLauncher[iLaunOffset]->m_fStartTime = fTime; 
			m_vecLauncher[iLaunOffset]->m_fLastUpdate = fTime;
			m_vecLauncher[iLaunOffset]->SetStatus(TRUE);
		}
		else 
		{
			m_vecLauncher[iLaunOffset]->SetStatus(FALSE);
		}
	}
	void SetBarrier(int iBarOffset, bool bStatus){ m_vecBarrier[iBarOffset]->SetBarrier(bStatus);}
	void SetForce(int iForOffset, bool bStatus) { m_vecForce[iForOffset]->SetForceField(bStatus);}
	
	//����ɾ�����������赲�������
	void AddLauncher( ModelSFX::KLauncher* launcher) { m_vecLauncher.push_back(launcher); }
	void AddForce( ModelSFX::KForceField *ff){ m_vecForce.push_back (ff);}
	void AddBarrier( ModelSFX::KBarrier *b){ m_vecBarrier.push_back (b);}
	void RemoveLauncher(int iLaunOffset) { m_vecLauncher.erase(&m_vecLauncher[iLaunOffset]);}
	void RemoveForce( int iForOffset) { m_vecForce.erase( &m_vecForce[iForOffset]);}
	void RemoveBarrier( int iBarOffset) { m_vecBarrier.erase( &m_vecBarrier[iBarOffset]);}
	
	//����ϵͳ
	void Update();//��������ϵͳ
	vector<ModelSFX::KLauncher*> m_vecLauncher;//�������б�
private:
	
	
///////////////////////////////////////////////////////////////////////////////////////////
	//��������
	
	//FILE* fpdebug;
	int m_TriggerIndex;
	float			m_fStartTime;//Particles System����ʱ��
	bool			m_bForce ;//particle system�Ƿ�������
	bool			m_bBarrier;//particles system�Ƿ����赲
	float m_fLastUpdate;
protected:
	CString m_strLuanName[20];
	int m_iLaunCount;
	//KIniReader m_ini;
	
	vector< ModelSFX::KForceField*> m_vecForce;//�����б�
	vector< ModelSFX::KBarrier*> m_vecBarrier;//�赲���б�
public:
	HRESULT FrameMove(void);
	vector<KBarrier*>* GetBarrier(void);
	vector<KForceField*>* GetForceField(void);
};



//////////////////////////////////////////////////////////////////////////
//������GetPointOnLine
//���ã��õ�ֱ�߶��ϵ�һ�������
//������
//		vOutPoint[out]:D3DXVECTOR3,�õ��ĵ�
//		vStart[in]:D3DXVECTOR3, �߶ε����
//		vEnd[in]:D3DXVECTOR3, �߶ε��յ�
//����ֵ: vOutPoint: D3DXVECTOR3
//////////////////////////////////////////////////////////////////////////
inline D3DXVECTOR3 GetPointOnLine(D3DXVECTOR3& vOutPoint, const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd)
{
	float fLength = GetRandom(0.0f, 1.0f);
	vOutPoint = vEnd - vStart;
	vOutPoint *= fLength;
	vOutPoint += vStart;
	return vOutPoint;
}

//////////////////////////////////////////////////////////////////////////
//������GetPointOnLine
//���ã��õ�ֱ�߶��ϵ�һ�������
//������
//		vOutPoint[out]:D3DXVECTOR3,�õ��ĵ�
//		vStart[in]:D3DXVECTOR3, �߶ε����
//		vEnd[in]:D3DXVECTOR3, �߶ε��յ�
//����ֵ: vOutPoint: D3DXVECTOR3
//////////////////////////////////////////////////////////////////////////
inline D3DXVECTOR3 GetPointOnLine(D3DXVECTOR3& vOutPoint, D3DXVECTOR3* pLineList)
{
	float fLength = GetRandom(0.0f, 1.0f);
	vOutPoint = *(pLineList + 1)  - *pLineList;
	vOutPoint *= fLength;
	vOutPoint += *pLineList;
	return vOutPoint;
}

//////////////////////////////////////////////////////////////////////////
//������GetPointOnLine
//���ã��õ��������ϵ�һ�������
//������
//		vOutPoint[out]:D3DXVECTOR3,�õ��ĵ�
//		pVertextList[in]:D3DXVECTOR3*, �����б�
//����ֵ: vOutPoint: D3DXVECTOR3
//////////////////////////////////////////////////////////////////////////
inline D3DXVECTOR3 GetPointOnTriangle(D3DXVECTOR3& vOutPoint, D3DXVECTOR3* pVertexList)
{
	float fFactor1 = GetRandom(0.0f, 1.0f);
	float fFactor2 = GetRandom(0.0f, 1.0f);
	D3DXVECTOR3 vtmp = (pVertexList[0] - pVertexList[1]) * fFactor1;
    vOutPoint =  vtmp +  (pVertexList[2] - pVertexList[1] - vtmp) * fFactor2 + 
				pVertexList[1];
	return vOutPoint;
}

////////////////////////////////////////////////////////////////////////////////////
//GetPointOnRect()
//���ã��õ�һ���ռ���һ���ı��α��ϵ������
//������
//		vPoint:[out]�õ��ĵ�
//		vA,vB,vC,vDҪ˳ʱ��
//		vA:[in]�ı��εĶ���
//		vB:[in]ͬ��
//		vC:[in]ͬ��
//		vD:[in]ͬ��
///////////////////////////////////////////////////////////////////////////////////
inline D3DXVECTOR3 GetPointOnRect(D3DXVECTOR3 & vPoint, const D3DXVECTOR3 &vA, const D3DXVECTOR3 &vB, const D3DXVECTOR3 &vC, const D3DXVECTOR3 &vD, bool bFace)
{
	if ( bFace )
	{
		D3DXVECTOR3 vAB = vB - vA;
		D3DXVECTOR3 vAD = vD - vA;
		float fScale;
		fScale = GetRandom( 0.0f, 1.0f);
		vAB = vAB * fScale;
		fScale = GetRandom( 0.0f, 1.0f);
		vAD = vAD * fScale;
		vPoint = vAB + vAD + vA;
		return vPoint;
	}
	else
	{
		DWORD dwEdge = rand() % 4;
		float fScale = GetRandom(0.0f, 1.0f);
		switch (dwEdge)
		{
		case 0:
			{
				vPoint = (vB - vA) * fScale + vA;
				break;
			}
		case 1:
			{
				vPoint = (vC - vB) * fScale + vB;
				break;
			}
		case 2:
			{
				vPoint = (vD - vC) * fScale + vC;
				break;
			}
		case 3:
			{
				vPoint = (vA - vD) * fScale + vD;
				break;
			}
		}
		return vPoint;
	}
}



////////////////////////////////////////////////////////////////////////////////////
//GetPointOnCircle()
//���ã��õ�һ���ռ���һ��Բ���ϵ������
//������
//		vPoint:[out]�õ��ĵ�
//		vCenter:[in]Բ��
//		vNormal:[in]Բ�ķ���
//		fRadius:[in]Բ�İ뾶
//////////////////////////////////////////////////////////////////////////
inline D3DXVECTOR3 GetPointOnCircle(D3DXVECTOR3 &vPoint, const D3DXVECTOR3 &vCenter, D3DXVECTOR3 &vNormal, const float fRadius, bool bFace)
{
	D3DXVECTOR3 vTemp;
	vTemp.x = GetRandom( -fRadius , fRadius);
	float fTemp = fRadius * fRadius - vTemp.x * vTemp.x;
	vTemp.y = GetRandom( -sqrt(fTemp) , sqrt(fTemp));
	vTemp.z = sqrt( fTemp  - vTemp.y * vTemp.y) * GetRandomSgn();
	D3DXVec3Normalize(&vNormal, &vNormal);
	D3DXVec3Cross(&vPoint, &vTemp, &vNormal);
	if ( !bFace )
	{
		D3DXVec3Normalize(&vPoint, &vPoint);
		vPoint *= fRadius;
	}
	vPoint += vCenter;
	return vPoint;
}

///////////////////////////////////////////////////////////////////
//GetVec3Len();
//���ã��õ���������
//����: vVector[in]:D3DXVECTOR3, ��Ҫ�󳤶ȵ�����
//����ֵ: float, ��������
//////////////////////////////////////////////////////////////////////////
inline float GetVec3Len(const D3DXVECTOR3 &vVector)
{	
	return sqrt(vVector.x * vVector.x + vVector.y * vVector.y + vVector.z * vVector.z);
}

};
