////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DBlockTraverseData.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-5-26 11:30:40
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DBlockTraverseData.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////

#define FLT_BIG_EPSILON	0.001f	//��FLT_EPSILON�����У���Щ�߽�����������
KG3DBlockTraverseData KG3DBlockTraverseData::Build( const D3DXVECTOR2& vSrc, const D3DXVECTOR2& vDes, FLOAT fSceneBlockWidth )
{
	

	/*��֤һ�ο�Խһ��Zone�������ÿ����Zone��Step
	��ֽ�ϻ����ӿ��Է��֣���б��(P(t) = vSrc + t * vDir)ɨ�������Խǵĸ��ӣ���ô������������
	���ڵ��Ǹ�����Ҳ�����ױ�ɨ��������һ����ɨ�����ָ������º����ϵĸ��ӣ���ô���ϻ������µĸ���Ҳ
	�����ױ�ɨ�����Ӻ��ߵ�λ�á�������ɨ���������£����ϣ����ϵĸ��ӣ������ϵĵ����£�

	0--1--2
	3--4--5
	6--7--8

	�ڵ�3��4��1�������������ߣ�����ֱ�ΪA,B,C����ô���Է��֣�ÿ��ԽAB�ٿ�ԽBCһ���ܿ�Խ��������
	��AB��BC�ĳ����ܹ���ΪStep����ֽ�ϻ����Ϳ������ˡ���Ϊ������ֱ�����꣬���Լ����AB��BC�ľ���֮��
	Ҫ�ٷֽ��X��Y�ķ������洢��D3DXVECTOR2���档��ΪAB��BC�����ǲ��ȵģ�����Ҫ�ֳ�����Step����һ�ο�Խ
	AB�Ļ�����һ�ξͿ�ԽBC

	��������㣬�е�ʱ���õ���ͳһ���꣬�����ӿ����1���е�ʱ���õ���ʵ�����꣬�����ӿ����SceneBlockWidth
	*/

	KG3DBlockTraverseData retData;

	_ASSERTE(fSceneBlockWidth > 0);
	D3DXVECTOR2 vNormalizedDir;
	D3DXVECTOR2 vCosSin;
	{
		D3DXVECTOR2 vDistance = vDes - vSrc;		
		D3DXVec2Normalize(&vNormalizedDir, &vDistance);
		vCosSin = D3DXVECTOR2(vNormalizedDir.x / 1, vNormalizedDir.y / 1);
	}

	//�������������������ʼ����
	D3DXVECTOR2 vLowerLeftCornerIndex = D3DXVECTOR2(static_cast<FLOAT>(GetSceneBlockIndex(vSrc.x, fSceneBlockWidth))
		,	static_cast<FLOAT>(GetSceneBlockIndex(vSrc.y, fSceneBlockWidth)));
	retData.m_vStartIndex = vSrc / fSceneBlockWidth;

	_ASSERTE(vLowerLeftCornerIndex.x * fSceneBlockWidth - vSrc.x < FLT_BIG_EPSILON && 
		vLowerLeftCornerIndex.y * fSceneBlockWidth - vSrc.y < FLT_BIG_EPSILON && _T("vLowerLeftCornerIndexӦ����ʱ�����½�"));

	//�����ֱ��ˮƽҪ���⴦��������Ǳ߽�����ͺܸ���
	bool bVertical = abs(vNormalizedDir.x) < FLT_BIG_EPSILON;
	bool bHorizontal = abs(vNormalizedDir.y) < FLT_BIG_EPSILON;
	if (! bVertical && ! bHorizontal)	
	{
		{
			/*
			�ɴ��ߵĽ������֪��AB�ĳ��ȱ�Ȼ�Ǹ��ӿ�ȳ�cos(dir)����BC��sin(dir)
			��ΪStep�Ƕ�ά�ģ��������ٴγ�cos,sin�ֽ�Ϊx,y
			*/
			FLOAT fLengthOfStep1 = 1 * abs(vCosSin.x);
			FLOAT fLengthOfStep2 = 1 * abs(vCosSin.y);
			retData.m_vIndexStep1 = fLengthOfStep1 * vCosSin;
			retData.m_vIndexStep2 = fLengthOfStep2 * vCosSin;
		}

		/*			
		���������˵�ģ������A����ô�Ϳ�����A+Step * Count������Zone��Index��
		�ü��������A������ʼ���꣬��������������Zone��ʱ��A+Step * Count���ܱ�֤һ�ο�Խһ��Zone;
		ע����һ�ο�ԽAB����һ�ο�ԽBC
		*/


		/*�����ߵķ�����������ʼ����ķ��򣬶��ҶԽǸ����������㣬ѡ����һ�����㿪ʼ
		����ͼ�����ָ��ӣ�����������¿�ʼ�����ߵķ���б���ϣ���ô��3��7�����Ľ��㶼���Խ���
		�����3��ʼ����ô��һ�ο�Խ����Step1����7��ʼ����Step2
		*/

		struct QuadrantStratege
		{
			D3DXVECTOR2 vFirstCrossTestPointsGapBetweenLeftBottomPoint;
			D3DXVECTOR2 vSecondCrossTestPointsGapBetweenLeftBottomPoint;
		};

		static QuadrantStratege quadrantStrategeArray[4] = 
		{
			{D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 0)},
			{D3DXVECTOR2(1, 1), D3DXVECTOR2(0, 0)},
			{D3DXVECTOR2(1, 0), D3DXVECTOR2(0, 1)},
			{D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1)},
		};

		INT nQuradrant = 0;
		if(vNormalizedDir.x > 0)
			nQuradrant = vNormalizedDir.y > 0 ? 1 : 4;
		else
			nQuradrant = vNormalizedDir.y > 0 ? 2 : 3;

		_ASSERTE(nQuradrant >= 1 && nQuradrant <= 4);

		D3DXVECTOR2 vGridCorner = quadrantStrategeArray[nQuradrant - 1].vFirstCrossTestPointsGapBetweenLeftBottomPoint;
		D3DXVECTOR2 vGridSrc = vSrc / fSceneBlockWidth - vLowerLeftCornerIndex;	//ת������1Ϊ��λ��ͳһ����

		D3DXVECTOR2 vCross = PointDistanceToLine(vGridCorner, vGridSrc, vNormalizedDir);
#if defined(DEBUG) | defined(_DEBUG)
		{
			//���ǿ϶���Block��ĸ���2�ľ����ڣ�����������б��ʮ��ȴ����Խǣ������������һ�ǵ����
			D3DXVECTOR2 vGap = vCross - vGridSrc;
			_ASSERTE(abs(D3DXVec2Length(&vGap)) < 1.42);//1.42���ǵ�λ1���ӶԽ��߳���
		}
#endif
		//�����ĸ������ԭ������㻹��ͬһ���������棬��������赽�Ǹ��ط�
		BOOL bABFirst = TRUE;
		BOOL IsIntheSameGrid = MT_FLOAT_IS_IN_RANGE(vCross.x, 0, 1.0f) && MT_FLOAT_IS_IN_RANGE(vCross.y, 0, 1.0f);
		if(IsIntheSameGrid)
		{
			retData.m_vStartIndex = vCross + vLowerLeftCornerIndex;
		}
		else
		{
			//��������Src����ͬһ����������ģ�Src�����£���б���ϣ�
			//���������ͼ�������B�����Ļ�����Ҫ�ȱ���BC���ٱ���AB

			vGridCorner = quadrantStrategeArray[nQuradrant - 1].vSecondCrossTestPointsGapBetweenLeftBottomPoint;
			vCross = PointDistanceToLine(vGridCorner, vGridSrc, vNormalizedDir);

			_ASSERTE(MT_FLOAT_IS_IN_RANGE(vCross.x, 0, 1.0f) 
				&& MT_FLOAT_IS_IN_RANGE(vCross.y, 0, 1.0f) 
				&& _T("��㲻��ͬһ�����ӣ������Ľ�����ܻ�������"));

			std::swap(retData.m_vIndexStep1, retData.m_vIndexStep2);//��������£�����ͼ���ȿ�ԽBC�������step���෴��

			retData.m_vStartIndex = vCross + vLowerLeftCornerIndex;

			bABFirst = FALSE;				
		}

	}
	else
	{
		retData.m_vIndexStep1 = vNormalizedDir;
		retData.m_vIndexStep2 = retData.m_vIndexStep1;
		retData.m_vStartIndex = vSrc / fSceneBlockWidth;
	}

#if 0
	//���߾������ӵĽǵ�ʱ�򣬻ᷢ������һ��Step�̵ܶ������Ӧ�úϲ���
	if(D3DXVec2Length(&retData.m_vIndexStep1) < FLT_EPSILON)
	{
		_ASSERTE(D3DXVec2Length(&retData.m_vIndexStep2) > FLT_EPSILON);
		retData.m_vIndexStep1 = retData.m_vIndexStep2;
	}
	else if(D3DXVec2Length(&retData.m_vIndexStep2) < FLT_EPSILON)
	{
		_ASSERTE(D3DXVec2Length(&retData.m_vIndexStep1) > FLT_EPSILON);
		retData.m_vIndexStep2 = retData.m_vIndexStep1;
	}
#endif

	//��Խ�ķ�ʽ������AB����BC��AB=vCosSin.x��BC=vCosSin.y����λ�Ǹ�����
	{
		
		FLOAT fTraverseCount = 0;
		if(! bVertical && ! bHorizontal)
			fTraverseCount = D3DXVec2Length(&(vDes - retData.m_vStartIndex * fSceneBlockWidth)) * 2 /((abs(vCosSin.x) + abs(vCosSin.y)) * fSceneBlockWidth);
		else
			fTraverseCount = D3DXVec2Length(&(vDes - vSrc))/ fSceneBlockWidth;
		FLOAT fRemain = fTraverseCount - MT_FLOAT_TRIM_TAIL(fTraverseCount);
		INT nStepRemain = 0;
		if (fRemain > FLT_BIG_EPSILON)
		{
			++nStepRemain;
		}
		if (fRemain > D3DXVec2Length(&retData.m_vIndexStep1))
		{
			++nStepRemain;
		}
		retData.m_nStep = MT_FLOAT_TRIM_TAIL(fTraverseCount) + nStepRemain;
	}

	return retData;
}

bool operator==(const KG3DBlockTraverseData& dataA, const KG3DBlockTraverseData& dataB)
{
	return MT_VECTOR_EQUAL_2D(dataA.m_vStartIndex, dataB.m_vStartIndex, FLT_BIG_EPSILON)
		&& dataA.m_nStep == dataB.m_nStep
		&& MT_VECTOR_EQUAL_2D(dataA.m_vIndexStep1, dataB.m_vIndexStep1, FLT_BIG_EPSILON)
		&& MT_VECTOR_EQUAL_2D(dataA.m_vIndexStep2, dataB.m_vIndexStep2, FLT_BIG_EPSILON);
}

bool operator!=(const KG3DBlockTraverseData& dataA, const KG3DBlockTraverseData& dataB)
{
	return !(dataA == dataB);
}

#if defined(KG_ENABLE_TDD)
KG_TEST_BEGIN(KG3DBlockTraverseData)
{
	enum
	{
		em_bw = 1600,	//bw��blockwidth����˼
		em_bw_double = em_bw * 2,
		em_bw_half = em_bw /2,
	};
	struct TestCase 
	{
		D3DXVECTOR2 vSrc;
		D3DXVECTOR2 vDes;
		KG3DBlockTraverseData retData;
	};

	TestCase testCases[] = 
	{
		{
			D3DXVECTOR2(119830.77f, 35201.230f),
			D3DXVECTOR2(121026.63f, 35202.672f),
			{
				D3DXVECTOR2(75.f, 22.0009f),
				3,
				D3DXVECTOR2(0.0012f, 0), 
				D3DXVECTOR2(0.999999f, 0.001205f)
			}
			
		},
		//X�����
		{
			D3DXVECTOR2(em_bw_half, em_bw_half),
			D3DXVECTOR2(em_bw + em_bw_half, em_bw_half),

			{
				D3DXVECTOR2(0.5f, 0.5f),
				1,
				D3DXVECTOR2(1, 0), 
				D3DXVECTOR2(1, 0)
			},
		},
		{
			D3DXVECTOR2(em_bw_half, em_bw_half),
			D3DXVECTOR2(em_bw_double + em_bw_half, em_bw_half),
			{
				D3DXVECTOR2(0.5f, 0.5f),
				2,
				D3DXVECTOR2(1, 0), 
				D3DXVECTOR2(1, 0)
			},
		},
		//Y����
		{
			D3DXVECTOR2(em_bw_half, em_bw_half),
				D3DXVECTOR2(em_bw_half, em_bw_double + em_bw_half),
			{
				D3DXVECTOR2(0.5f, 0.5f),
				2,
				D3DXVECTOR2(0, 1), 
				D3DXVECTOR2(0, 1), 
			},
		},
		//����ֱ����4������
		{
			D3DXVECTOR2(-em_bw, -em_bw),
			D3DXVECTOR2(em_bw, em_bw),

			{
				D3DXVECTOR2(-0.5f, -0.5f),
				3,
				D3DXVECTOR2(0.5f,0.5f), 
				D3DXVECTOR2(0.5f,0.5f)
			},
		},
		{
			D3DXVECTOR2(em_bw, em_bw),
				D3DXVECTOR2(-em_bw, -em_bw),

			{
				D3DXVECTOR2(1.5f, 1.5f),
				5,
				D3DXVECTOR2(-0.5f,-0.5f), 
				D3DXVECTOR2(-0.5f,-0.5f)
			},
		},
		{
			D3DXVECTOR2(em_bw, -em_bw),
			D3DXVECTOR2(-em_bw, em_bw),

			{
				D3DXVECTOR2(1, -1),
				4,
				D3DXVECTOR2(-0.5f,0.5f), 
				D3DXVECTOR2(-0.5f,0.5f)
			},
		},
		{
			D3DXVECTOR2(-em_bw, em_bw),
			D3DXVECTOR2(em_bw, -em_bw),

			{
				D3DXVECTOR2(-1, 1),
				4,
				D3DXVECTOR2(0.5f,-0.5f), 
				D3DXVECTOR2(0.5f,-0.5f)
			},
		},
		
	};

	for (size_t i = 0; i < _countof(testCases); ++i)
	{
		TestCase& caseElem = testCases[i];
		KG3DBlockTraverseData data = KG3DBlockTraverseData::Build(caseElem.vSrc, caseElem.vDes, em_bw);
		_ASSERTE(data == caseElem.retData);
	}
	
}
KG_TEST_END(KG3DBlockTraverseData)
#endif