#pragma once
//#include "xmesh.h"

namespace ModelSFX
{

class KModelBelt :
	public KModel
{
private:
	typedef struct _SrcLine
	{
		float Distance;
		D3DXVECTOR3 A;
		D3DXVECTOR3 B;

		_SrcLine()
		{
			A  = D3DXVECTOR3(0,0,0);
			B  = D3DXVECTOR3(0,0,0);
			Distance = 0;
		}
	}SRCLINE,* LPSRCLINE;

	HRESULT CreateVerticesBuffer();
	LPDIRECT3DVERTEXBUFFER9  m_lpVBuf;
	BOOL m_bGotOne;	//���������ڳ�ʼ��Ʈ��,	����ֻ����InitialBelt()����һ��
	SRCLINE  m_CurLine;  

	list<SRCLINE>m_SrcLineList;
	vector<SRCLINE>m_vecSrcLine;

	int m_nNumFaces;					//�����δ���ͼԪ��Ŀ�������Ŀ

public:
	float m_fMaxDist;					//��ֵ��ļ�����ֵ
	float m_fMaxLength;					//Ʈ�����ȵ����ֵ
	SRCLINE  m_BaseLine;				//��Ʈ�����߶�

	int m_nMaxLineBuffer;				//���㻺��Ϊ��ֵ������
	D3DXVECTOR3 m_WindForce;			//���� 
	D3DXVECTOR3 m_Gravity;				//���� 
	BOOL m_bShowWind;					//�Ƿ�ʹ�÷���
	BOOL m_bExtend;						//�Ƿ�Ŵ�Ʈ��ĩ�˵��˶�
	BOOL m_bShowSin;					//�Ƿ�ʹ�����Ҳ�

	BOOL m_bShow;						//�ñ�������

	float m_fExtendNum;					//Ʈ��ĩ��Ʈ���Ӿ�ϵ��
	float m_fSwing;						//���Ҳ����
	float m_fCyc;						//�����ڵ���ϵ��;

	int m_nListMaxSize;				//��������m_SrcLineList��Line����Ŀ


public:
	KModelBelt(void);
	virtual ~KModelBelt(void);
	HRESULT Render();
	HRESULT Acquire();
	HRESULT LoadMesh(LPSTR pFileName);

private:
	HRESULT InitialBelt(SRCLINE Line); 
	HRESULT UpdateVertices(void);	
	HRESULT AddLine(SRCLINE Line);
	HRESULT	UpdateBladeLine(void);
//	HRESULT UpdateAtRest(SRCLINE Line);
	HRESULT UpdateExtend(void);

};

};