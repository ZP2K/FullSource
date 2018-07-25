////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DOutDoorAssistantFuncs.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-7 10:56:40
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KG3DOutDoorAssistantFuncs.h"
#include "KG3DSceneOutDoorSpaceMgr.h"
#include "KG3DMathFunctions.h"
#include "KG3DRepresentObject.h"	//GetObjectCount

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////


KG3DOutDoorIntersection::KG3DOutDoorIntersection(KG3DOutDoorIntersectionGroundHeightGetter& DataInterface)
:m_Data(DataInterface)
{

}

/*
�㷨����ϸ���ͣ�
1.��Ray��ɲ���ʽ P(t) = vSrc + vDir * t;
2.�ֶΣ���Src�Ƕ˿�ʼ��μ��
3.���һ����ײ���У�Start��ĵ��α��ߵͣ�End��ĵ��α��߸ߣ��γ�һ��X��
��ô��Ȼ�����м��Ȼ����һ�����㡣��ʼ�����Բ�ֵѸ�ٵ����������Ľ���
4.��������γ�X�ͣ���ô�м������ͻ�𣬻��ǿ����н��㣬����1/2ϸ�֣�ֱ������ĳ�������˳�������
�õ�һ��X�ͣ�Ȼ��������Ĳ���3������
5.���û�н��㣬�����һ��ֱ�������ɡ�

�㷨�ڱȽ����µ�Ray�й����Ϻã�8��GetHeight���ܵõ������
�Ƚϻ�����������Զ�ĵ������ҽ��㣬���ܻ�ﵽ21�β��ң�һ�㲻����15��
ƽ��8�����ҵõ������

�ڶ��͵ĵ����ϣ�����ᵯ�������εĶ��㣬����������أ����ֻ�Ƿ�����ǿ��Խ��ܵģ�1/5�¶ȵĵ��ηŶ���ûʲô��
�������Ƕ�ö�ٿ��Ե������ȡ�

�����Ҫ������ľ�ȷ�Ľ�������Լ�����ײ�ߣ��Ӵ󾫶ȣ�����������ɺܴ󸺵�

*/
static RayIntersectionTerrainAccuracyStruct stDefAccuracyStruct;
BOOL KG3DOutDoorIntersection::RayIntersectionTerrain( D3DXVECTOR3 vSrc, D3DXVECTOR3 vNormalizedDir
											  , FLOAT* pfDist
											  , RayIntersectionTerrainAccuracyStruct* pAccuracy/*NULL*/)
{
	FLOAT fRet = 0;

	RayIntersectionTerrainAccuracyStruct CurAccuracy = stDefAccuracyStruct;
	if (NULL != pAccuracy && pAccuracy != &stDefAccuracyStruct)
	{
		CurAccuracy = *pAccuracy;
	}

	m_nGetGoundHeightCount = 0;//���ڼ�ص�������


	//����Ray
	IntersectionRay Ray;
	Ray.m_vSrc = vSrc;
	Ray.m_vNormalizedDir = vNormalizedDir;	
	{
		D3DXVECTOR3 vProjDir = D3DXVECTOR3(vNormalizedDir.x, 0, vNormalizedDir.z);
		D3DXVec3Normalize(&vProjDir, &vProjDir);
		Ray.m_fDirCosine = D3DXVec3Dot(&vNormalizedDir, &vProjDir);//��y=0ƽ��ļн�//������ֻҪ�Ѳ���ʽ�е�t��������Ϳ���֪����y=0ƽ��������ľ�����
	}

	//����Ǵ�ֱ���µģ���ôֱ�ӵõ�GroundHeight�ͺ���
	if (MT_FLOAT_EQUAL(vNormalizedDir.x, 0.0f) && MT_FLOAT_EQUAL(vNormalizedDir.z, 0.0f))
	{
		FLOAT fHeight = GetGroundHeight(vSrc.x, vSrc.z);
		fRet = vSrc.y - fHeight;
		KG_PROCESS_ERROR(fRet > 0);
		goto Exit1;
	}


	{	
		/*
		�������ֽ�ֱ�߳��߶Σ��ӽ���Զ����ཻ��
		��������Ҫһ�ζ��������һ����1����Χ�У��γɲ�����ֱ������Ϻ����£��γ���X�ͣ���ô�������Χ��
		��Ȼ�н���
		���ý��㽥���ƽ����ܺܿ�õ�׼ȷ�Ľ���
		*/
		
		INT	iStepCount = (INT)ceil(CurAccuracy.m_fMaxRange / CurAccuracy.m_fTestRangePerLineSection);
		if(iStepCount < 1)
		{
			iStepCount = 1;
		}
		INT iStep = 0;

		IntersectionTestPoint PStart, PEnd;

		PStart.m_fT = (FLOAT)(iStep) * CurAccuracy.m_fTestRangePerLineSection;
		PStart.m_Pos = vSrc + vNormalizedDir * PStart.m_fT;
		PStart.m_fDeltaToTerrain = PStart.m_Pos.y - GetGroundHeight(PStart.m_Pos.x, PStart.m_Pos.z);

		PEnd.m_fT = (FLOAT)(iStep + 1) * CurAccuracy.m_fTestRangePerLineSection;
		PEnd.m_Pos = vSrc + vNormalizedDir * PEnd.m_fT;
		PEnd.m_fDeltaToTerrain = PEnd.m_Pos.y - GetGroundHeight(PEnd.m_Pos.x, PEnd.m_Pos.z);

		while(iStep < iStepCount)
		{
			if (PStart.m_fDeltaToTerrain < 0 
				&& PEnd.m_fDeltaToTerrain < 0)
			{
				//�鵽��������ζ����ߵ����棬�γ�һ�����أ����账��
				goto Exit0;				
			}

			//����Ƿ���ڽ���
			BOOL bIntersectInThisRange = PStart.m_fDeltaToTerrain * PEnd.m_fDeltaToTerrain < 0;//���

			if (bIntersectInThisRange)
			{
				//���Start��End�õ��ĸ߷ֱ����ߵ���������棬�γ�һ��X����ô�м�϶����н���ģ��ý��㽥���ƽ�
				fRet = GetIntersectionPoint(Ray, PStart, PEnd, CurAccuracy);
				goto Exit1;
			}
			else
			{
				//������Start��End�жϳ��м����û�н��㣬������Ҫ�ѷ�Χ�п�����ϸ���
				IntersectionTestPoint RetStart, RetEnd;
				BOOL bRet = TestIntersecionInRange(Ray, PStart, PEnd, RetStart, RetEnd, CurAccuracy);
				if (bRet)
				{
					fRet = GetIntersectionPoint(Ray, RetStart, RetEnd, CurAccuracy);
					goto Exit1;
				}
				else
				{
					if (m_nGetGoundHeightCount > CurAccuracy.m_nMaxGetHeightCount)
					{
						goto Exit0;
					}
				}
			}

			//�����һ��
			++iStep;
			
			PStart = PEnd;

			PEnd.m_fT = (FLOAT)(iStep + 1) * CurAccuracy.m_fTestRangePerLineSection;
			PEnd.m_Pos = vSrc + vNormalizedDir * PEnd.m_fT;
			PEnd.m_fDeltaToTerrain = PEnd.m_Pos.y - GetGroundHeight(PEnd.m_Pos.x, PEnd.m_Pos.z);

		}//End of While
	
		if (iStep >= iStepCount)
		{
			goto Exit0;
		}
	}
Exit1:
	//_KG3D_DEBUG_OUTPUT1("GetHeight%d", m_nGetGoundHeightCount);
	if(NULL != pfDist)
		*pfDist = fRet;
	return TRUE;
Exit0:
	return FALSE;
	
}

FLOAT KG3DOutDoorIntersection::GetGroundHeight( FLOAT fX, FLOAT fZ )
{
	++m_nGetGoundHeightCount;
#if defined(DEBUG) | defined(_DEBUG)
	FLOAT fHeight = m_Data.GetGroundHeight(fX, fZ);
	return fHeight;
#else
	return m_Data.GetGroundHeight(fX, fZ);
#endif
	
}


FLOAT KG3DOutDoorIntersection::GetIntersectionPoint( const IntersectionRay& Ray 
															, const IntersectionTestPoint& StartP
															, const IntersectionTestPoint& EndP
															, const RayIntersectionTerrainAccuracyStruct& CurAccuracy)
{
	FLOAT fRet = CurAccuracy.m_fMaxRange;

	IntersectionTestPoint PA = StartP;//PA����T�Ƚ�С���Ǹ�
	IntersectionTestPoint PB = EndP;
#if defined(DEBUG) | defined(_DEBUG)	
	INT nLoopCount = 0;
#endif

	do 
	{
#if defined(DEBUG) | defined(_DEBUG)
		++nLoopCount;
#endif
		

		IntersectionTestPoint PInter;
		FLOAT fABPointDist = 0;
		{
			FLOAT fDeltaStart = abs(PA.m_fDeltaToTerrain);
			FLOAT fDeltaEnd = abs(PB.m_fDeltaToTerrain);
			_ASSERTE(PA.m_fDeltaToTerrain * PB.m_fDeltaToTerrain < 0);

			fABPointDist = (PB.m_fT - PA.m_fT);
			_ASSERTE(fABPointDist > -FLT_EPSILON);

			//����ײ����һ�����棬�е������ϣ�����������ϣ�������ײ��ǰ�������˵㣬�͵õ��ĵ��θ�
			//�����Ϊ������ƽֱ�Ļ������Եõ�����������ϣ����κ���ײ�ߵĽ���PInter��
			//ͨ���������������Ľ���Ѹ��������

			PInter.m_fT =  fABPointDist* fDeltaStart / (fDeltaStart + fDeltaEnd);//��ƽ���߶���õ���
			PInter.m_fT += PA.m_fT;

			PInter.m_Pos = Ray.m_vSrc + Ray.m_vNormalizedDir * PInter.m_fT;
		}
		FLOAT CurCheckPointHeight = GetGroundHeight(PInter.m_Pos.x, PInter.m_Pos.z);

		PInter.m_fDeltaToTerrain = PInter.m_Pos.y - CurCheckPointHeight;

		enum{em_gap_per_point = 100,	//ÿ100��һ����
			em_max_get_height_count = 60
		};
		

		if (abs(PInter.m_fDeltaToTerrain) < CurAccuracy.m_fMinGap //����õ��ĵ��θ߶Ⱥͼ����Ѿ��ܽӽ�
			|| fABPointDist * Ray.m_fDirCosine< CurAccuracy.m_fMaxAccuracy	//�����������ܽӽ��������ǵĲ�ֵ�����������
			|| m_nGetGoundHeightCount >= CurAccuracy.m_nMaxGetHeightCount)	
		{

#if 1
			const FLOAT fGapPerPoint = (FLOAT)em_gap_per_point;
			//�������ͺ�����ײ�õ�����,��Ϊ���ĸ�������ת���ѵ�֪����������һ���
			FLOAT lowerLeftX = floor(PInter.m_Pos.x/fGapPerPoint) * fGapPerPoint;
			FLOAT lowerLeftZ = floor(PInter.m_Pos.z/fGapPerPoint) * fGapPerPoint;

			D3DXVECTOR3 vlowerLeft(lowerLeftX, GetGroundHeight(lowerLeftX, lowerLeftZ), lowerLeftZ);
			D3DXVECTOR3 vlowerRight(lowerLeftX + fGapPerPoint
						, GetGroundHeight(lowerLeftX + fGapPerPoint, lowerLeftZ), lowerLeftZ);
			D3DXVECTOR3 vUpperRight(lowerLeftX + fGapPerPoint
				, GetGroundHeight(lowerLeftX + fGapPerPoint, lowerLeftZ + fGapPerPoint), lowerLeftZ + fGapPerPoint);
			D3DXVECTOR3 vUpperLeft(lowerLeftX
				, GetGroundHeight(lowerLeftX, lowerLeftZ + fGapPerPoint), lowerLeftZ + fGapPerPoint);

			FLOAT fDist1 = FLT_MAX, fDist2 = FLT_MAX;
			BOOL bInter1 = KG3D_CUSTOM_HELPERS::IntersectRayTri(Ray.m_vSrc, Ray.m_vNormalizedDir
				, vlowerLeft, vUpperRight, vlowerRight, NULL, NULL, &fDist1);
			BOOL bInter2 = KG3D_CUSTOM_HELPERS::IntersectRayTri(Ray.m_vSrc, Ray.m_vNormalizedDir
				, vlowerLeft, vUpperLeft, vUpperRight, NULL, NULL, &fDist2);

			if (bInter1 || bInter2)
			{
				fRet = min(fDist1, fDist2);
				break;
			}
#endif
			
			D3DXVECTOR3 vFakeInterPoint = PInter.m_Pos;
			vFakeInterPoint.y = CurCheckPointHeight;

			fRet = D3DXVec3Length(&(vFakeInterPoint - Ray.m_vSrc));

			break;
		}
		
			

		_ASSERTE(nLoopCount < em_max_get_height_count);

		if (PInter.m_fDeltaToTerrain * PA.m_fDeltaToTerrain < 0)
		{
			PB = PInter;
		}
		else
		{
			PA = PInter;
		}

	} while(1);
	return fRet;
}
BOOL KG3DOutDoorIntersection::TestIntersecionInRange( const IntersectionRay& Ray
													   , const IntersectionTestPoint& StartP
													   , const IntersectionTestPoint& EndP
													   , IntersectionTestPoint& RetStart
													   , IntersectionTestPoint& RetEnd
													   , const RayIntersectionTerrainAccuracyStruct& CurAccuracy)
{

	FLOAT fTDelta = EndP.m_fT - StartP.m_fT;
	_ASSERTE(fTDelta > 0);

	FLOAT fDistance = fTDelta * Ray.m_fDirCosine;
	if (fDistance < CurAccuracy.m_fMinTestRange)
	{
		return FALSE;		
	}
	
	{
		//���ʱ��Ӧ���ߵ����㶼�ڵ������档��Ϊһ��ʼ�Ͳ��������������
		//��һ������һ��TestPoint�Ľ��������Ļ���
		//��������һ��X���ˣ����Կ�ʼ�����ƽ���
		_ASSERTE(StartP.m_fDeltaToTerrain > 0 - FLT_EPSILON && EndP.m_fDeltaToTerrain > 0 - FLT_EPSILON);

		FLOAT fMaxYDelta = min(abs(StartP.m_fDeltaToTerrain), abs(EndP.m_fDeltaToTerrain));
		FLOAT fMaxTangent = fMaxYDelta / fDistance;
		if (fMaxTangent > CurAccuracy.m_fMaxTangent)
		{
			//����˵����������κܸߣ��������û��һ�ֺܼ�Ķ��£���ô��Ϊ�����ε��ζ��������м��н���
			return FALSE;
		}
	}

	if (m_nGetGoundHeightCount > CurAccuracy.m_nMaxGetHeightCount)
	{
		return FALSE;
	}

	IntersectionTestPoint MidPoint;
	MidPoint.m_fT = fTDelta/2 + StartP.m_fT;
	MidPoint.m_Pos = Ray.m_vSrc + Ray.m_vNormalizedDir * MidPoint.m_fT;
	MidPoint.m_fDeltaToTerrain = MidPoint.m_Pos.y - GetGroundHeight(MidPoint.m_Pos.x, MidPoint.m_Pos.z);

	if (MidPoint.m_fDeltaToTerrain < FLT_EPSILON)
	{
		RetStart = StartP;
		RetEnd = MidPoint;
		return TRUE;
	}
	
	//�������һ�ߣ��ٲ�Զ����һ��
	BOOL bRet = TestIntersecionInRange(Ray, StartP, MidPoint, RetStart, RetEnd, CurAccuracy);
	if (! bRet)
	{
		bRet = TestIntersecionInRange(Ray, MidPoint, EndP, RetStart, RetEnd, CurAccuracy);
	}
	return bRet;
}
#if defined(KG_ENABLE_TDD)
#include "Terrain/KG3DGridHeightGetter.h"
KG_TEST_BEGIN(KG3DOutDoorIntersection)
{	
	struct TestVertexHeightGetter: public KG3DGridVertexHeightGetter
	{
		virtual D3DXVECTOR4 GetBlockRect()
		{
			return D3DXVECTOR4(0,0,1600,1600);
		}
		virtual ULONG		GetBlockCellCount(){return BLOCK_CELL_COUNT;}
		virtual FLOAT		GetVertexHeight(int nX,int nZ)
		{
			if(nX >= 0 && nX < _countof(stTestVertices[0]) && nZ >= 0 && nZ < _countof(stTestVertices))
				return stTestVertices[nZ][nX];
			return 0;
		}
	};

	struct TestGroundHeightStruct : public KG3DOutDoorIntersectionGroundHeightGetter 
	{
		virtual FLOAT GetGroundHeight(FLOAT fX, FLOAT fZ)
		{
			TestVertexHeightGetter Getter;
			return KG3DGridHeightGetterFunc(fX, fZ, Getter);
		}
	};

	TestGroundHeightStruct HeightGetter;
	KG3DOutDoorIntersection intersectTemp(HeightGetter);
	
	struct TestCase 
	{
		D3DXVECTOR3 vSrc;
		D3DXVECTOR3 vDir;
		FLOAT fRet;
	};

	static FLOAT	stTestVertices[6][16] = //ģ���һ������ͻ��ĵ���
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0},
		{0,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0},
		{0,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0},
		{0,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	_ASSERTE(_countof(stTestVertices[0]) == 16 && _countof(stTestVertices) == 6);

	const FLOAT squareTwo = 1.4142f;
	TestCase Cases[] = 
	{
		{D3DXVECTOR3(0,100,300),	D3DXVECTOR3(1,-1,0), 50 * squareTwo},
		{D3DXVECTOR3(100,1000,100),	D3DXVECTOR3(0,-1,0), 900},
		{D3DXVECTOR3(500, 100, 100), D3DXVECTOR3(-1, -1, 0), 50 * squareTwo},
		{D3DXVECTOR3(300, 100, 0), D3DXVECTOR3(0, -1, 1), 50 * squareTwo},
		{D3DXVECTOR3(300, 100, 500), D3DXVECTOR3(0, -1, -1), 50 * squareTwo},
		{D3DXVECTOR3(300, 120, 200), D3DXVECTOR3(1, -1, 0), 20 * squareTwo}
	};

	for(size_t i = 0; i < _countof(Cases); ++i)
	{
		D3DXVec3Normalize(&Cases[i].vDir, &Cases[i].vDir);
		FLOAT fRet = 0;
		BOOL bRet = intersectTemp.RayIntersectionTerrain(Cases[i].vSrc, Cases[i].vDir, &fRet, NULL);
		_ASSERTE(bRet);

		_ASSERTE(MT_FLOAT_EQUAL_BIAS(fRet, Cases[i].fRet, 1));
	}	
}
KG_TEST_END(KG3DOutDoorIntersection)
#endif



RayIntersectionTerrainAccuracyStruct::RayIntersectionTerrainAccuracyStruct() 
	:m_fMaxRange(EM_RAY_INTERSECTION_MAX_RANGE)
	,m_fTestRangePerLineSection(800)
	,m_fMinTestRange(100)
	,m_fMaxTangent(5)
	,m_fMaxAccuracy(/*100*/50)
	,m_fMinGap(2)
	,m_nMaxGetHeightCount(30)
{

}

VOID RayIntersectionTerrainAccuracyStruct::SetMaxRange( FLOAT fRange ) /*����ײ������Ӵ��ʱ�򣬾���Ҳ��Ӧ�Ŵ󣬲�Ȼ����Զ���뺯����˵���ܷ� */
{
	_ASSERTE(fRange > 0);
	FLOAT fDefaultMaxRange = (FLOAT)EM_RAY_INTERSECTION_MAX_RANGE;
	if(fRange - fDefaultMaxRange < FLT_EPSILON)//��С�ڵ���
	{
		m_fMaxRange = fRange;
		return;
	}

	FLOAT fScalePercent = fRange / fDefaultMaxRange;

	RayIntersectionTerrainAccuracyStruct DefaultStruct;

	m_fTestRangePerLineSection = fScalePercent * DefaultStruct.m_fTestRangePerLineSection;
	m_fMinTestRange = fScalePercent * DefaultStruct.m_fMinTestRange;
	m_nMaxGetHeightCount = (INT)(max(fScalePercent, 2.f) * (FLOAT)DefaultStruct.m_nMaxGetHeightCount);
}

ULONG KG3D_OUTDOOR::GetObjectCount( KG3DSceneOutDoorSpaceMgr& outDoorMgr, DWORD dwTypeFilter )
{
	D3DXVECTOR2 vLowerLeft, vUpperRight;
	outDoorMgr.GetSceneRect(&vLowerLeft, &vUpperRight);

	if (dwTypeFilter == REPRESENTOBJECT_NONE)
	{
		struct ObjectCounter : public KG3DEntityCtVisitorForObj
		{
			size_t m_uCount;			
			virtual HRESULT Accept(KG3DRepresentObject& obj)
			{
				++m_uCount;
				return S_OK;
			}
		};

		ObjectCounter counterTemp;
		counterTemp.m_uCount = 0;

		outDoorMgr.Traverse(counterTemp, vLowerLeft.x, vLowerLeft.y, vUpperRight.x, vUpperRight.y);

		return static_cast<ULONG>(counterTemp.m_uCount);
	}
	else
	{
		struct ObjectCounter : public KG3DEntityCtVisitorForObj
		{
			size_t m_uCount;
			DWORD m_dwFilter;
			virtual HRESULT Accept(KG3DRepresentObject& obj)
			{
				if(obj.GetType() == m_dwFilter)
					++m_uCount;
				return S_OK;
			}
		};

		ObjectCounter counterTemp;
		counterTemp.m_uCount = 0;
		counterTemp.m_dwFilter = dwTypeFilter;

		outDoorMgr.Traverse(counterTemp, vLowerLeft.x, vLowerLeft.y, vUpperRight.x, vUpperRight.y);

		return static_cast<ULONG>(counterTemp.m_uCount);
	}
	//return 0;	
}