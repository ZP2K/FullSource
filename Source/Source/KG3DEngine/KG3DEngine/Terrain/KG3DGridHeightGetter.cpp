////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DGridHeightGetter.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-9-17 14:56:46
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DGridHeightGetter.h"


////////////////////////////////////////////////////////////////////////////////


FLOAT KG3DGridHeightGetterFunc( FLOAT fX, FLOAT fZ, KG3DGridVertexHeightGetter& GH )
{
	D3DXVECTOR4 vBlockRect = GH.GetBlockRect();

	INT blockCellCount = GH.GetBlockCellCount();

	_ASSERTE(0.f != vBlockRect.z && 0.f != vBlockRect.w);

	//������1��Region֮��
	float fScaledX = (fX - vBlockRect.x) / vBlockRect.z * blockCellCount;	
	float fScaledZ = (fZ - vBlockRect.y) / vBlockRect.w * blockCellCount;

	INT iX = (INT)floor(fScaledX);//ȥβ�õ�����λ�ã�1.1��1��-1.1��-2
	INT iZ = (INT)floor(fScaledZ);

	FLOAT fXTail = fScaledX - (FLOAT)iX;
	FLOAT fZTail = fScaledZ - (FLOAT)iZ;

	_ASSERTE(fXTail > -FLT_EPSILON && fZTail > -FLT_EPSILON && fXTail < 1 && fZTail < 1);

	//����Region��С��ѹ��Region����
	if(iX > blockCellCount)
		iX = blockCellCount;
	if(iZ > blockCellCount)
		iZ = blockCellCount;

	INT iXNext = iX + 1 <= blockCellCount ? iX + 1 : blockCellCount;
	INT iZNext = iZ + 1 <= blockCellCount ? iZ + 1 : blockCellCount;

	/*
	���ε�����������������ƴ�ɵĸ��ӣ�Ҫ�������ϻ������½��������β�ֵ
	�����¿�ʼ����ʱ��4�㶨��Ϊ��1��2��3��4
	����Ĺ�ʽ��ԭ��һ����ֻ�Ǳ�����ʽ���ѣ���������������һ�����Ƶ������ģ�����Ȥ�Ŀ����Լ���ֽ�ϻ�����
	���µ�߶���͵�ʱ������Ƶ�����ʽ1�����ϵ���͵�ʱ������Ƶ�����ʽ2
	����������������Ƶ����Ĺ�ʽ��ʽ��򵥣������ܸ���
	*/
	BOOL IsPoint_UpperLeft_Or_LowerRight = fZTail > fXTail;	//�н���45�����ϣ�һ��������������
	if (IsPoint_UpperLeft_Or_LowerRight)
	{
		FLOAT h1 = GH.GetVertexHeight(iX, iZ);
		FLOAT h3 = GH.GetVertexHeight(iXNext, iZNext);
		FLOAT h4 = GH.GetVertexHeight(iX, iZNext);

		return h1 + (h4 - h1) * fZTail + (h3 - h4) * fXTail;
	}

	FLOAT h1 = GH.GetVertexHeight(iX, iZ);
	FLOAT h2 = GH.GetVertexHeight(iXNext, iZ);
	FLOAT h3 = GH.GetVertexHeight(iXNext, iZNext);

	return h1 + (h3 - h2) * fZTail + (h2 - h1) * fXTail;
}

#if defined(KG_ENABLE_TDD)
KG_TEST_BEGIN(KG3DTerrainBlock)
{
	static FLOAT	stTestVertices[4][16] = 
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	_ASSERTE(_countof(stTestVertices[0]) == 16);

	struct GroundHeightGetter : public KG3DGridVertexHeightGetter
	{
		virtual FLOAT GetVertexHeight(int nX,int nZ)
		{
			_ASSERTE(nX < _countof(stTestVertices[0]) && nZ < _countof(stTestVertices[0]));
			_ASSERTE(nX >= 0 && nZ >= 0);
			return stTestVertices[nZ][nX];
		}
		virtual ULONG GetBlockCellCount(){return 16;}
		virtual D3DXVECTOR4 GetBlockRect()
		{
			return D3DXVECTOR4(0,0,1600,1600);
		}
	};

	GroundHeightGetter TestObjInstance;

	//ÿ��ֱ���x,z��Ԥ�ڵĽ��
	FLOAT stTestCases[][3] = 
	{
		{0,		0,		0},
		{100,	100,	100},
		{150,	150,	100},
		{200,	200,	100},
		{300,	300,	0},
		{0,		100,	0},
		{100,	200,	100},
		{50,	50,		50},
		{100,	50,		50},
		{250,	250,	50},
		{50,	100,	50}
	};

	_ASSERTE(3 == _countof(stTestCases[0]));

	for (size_t i = 0; i < _countof(stTestCases); ++i)
	{
		FLOAT fRet = KG3DGridHeightGetterFunc(stTestCases[i][0], stTestCases[i][1], TestObjInstance);
		_ASSERTE(MT_FLOAT_EQUAL(fRet, stTestCases[i][2]));
	}
}
KG_TEST_END(KG3DTerrainBlock)
#endif