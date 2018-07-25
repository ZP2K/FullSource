////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DIntersection.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-8 10:01:51
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DIntersection.h"
#include "KG3DScene.h"
#include "KG3DSceneOutDoorSpaceMgr.h"
#include "KG3DSceneSceneEditor.h"
#include "KG3DSceneBlock.h"
#include "KG3DSceneOutDoorSpaceMgr.h"
#include "Terrain/KG3DOutDoorAssistantFuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////////////////

static const FLOAT stfTestRangeBiasPercent = 1.01f;	//��ײ��ʱ��Ҫ��΢�ſ�һЩ��Χ����������ƻ�������Ȼ�᲻׼

BOOL RayIntersection( KG3DScene& Scene, const D3DXVECTOR3& vSrc
					 , const D3DXVECTOR3& vNormalizedDir
					 , FLOAT* pfRet
					 , KG3DRepresentObject** ppObjectIntersected
					 , KG3DSceneEntity** ppEntityIntersected
					 , FLOAT fTestRange /*= EM_RAY_INTERSECTION_MAX_RANGE*/ 
					 , KG3DIntersectionFilter* pFilter /*= NULL*/)
{
#if defined(DEBUG) | defined(_DEBUG)
	{
		FLOAT normalLength = D3DXVec3Length(&vNormalizedDir);
		_ASSERTE(MT_FLOAT_EQUAL_BIAS(normalLength, 1, 0.01) && _T("�����������Ȳ�Ϊ1��������ײ�Ľ�������"));
	}
#endif
	
	FLOAT fDistanceRet = FLT_MAX;
	D3DXVECTOR3 vRetTemp(0,0,0);

	KG3DRepresentObject* pObject = NULL;
	KG3DSceneEntity* pEntityIntersectedRet = NULL;

	BOOL bIntersectEntity = FALSE; 
	BOOL bIntersectedTerrain = FALSE;

	if (fTestRange < FLT_EPSILON)
		return FALSE;//�����ܵ���ײ���ñ�ĺ���
	
	//���������������У����Է�Χ�ᱻ��΢�Ŵ�һЩ
	//������

	{
		bIntersectEntity = RayIntersectionEntity(Scene, vSrc, vNormalizedDir, &fDistanceRet
			, &pObject, &pEntityIntersectedRet, fTestRange, pFilter);
		if (bIntersectEntity)
		{
			vRetTemp = vSrc + vNormalizedDir * fDistanceRet;
		}		
	}
	
	//������
	{
		FLOAT fDistanceTerrain = fTestRange;
		bIntersectedTerrain = RayIntersectionTerrain(Scene, vSrc, vNormalizedDir, &fDistanceTerrain, fTestRange);
		if (bIntersectedTerrain)
		{
			_ASSERTE(fDistanceTerrain > - FLT_EPSILON && fDistanceTerrain < fTestRange);
			if (fDistanceTerrain < fDistanceRet	&& fDistanceTerrain > 0)
			{
				fDistanceRet = fDistanceTerrain;
				vRetTemp = vSrc + vNormalizedDir * fDistanceTerrain;
			}
			else
			{
				bIntersectedTerrain = FALSE;
			}
		}
	}

	BOOL bIntersected = bIntersectEntity || bIntersectedTerrain;
	if (bIntersected)
	{
		_ASSERTE(fDistanceRet > -FLT_EPSILON && fDistanceRet < fTestRange);
		if (NULL != pfRet)
		{
			*pfRet = fDistanceRet;
		}
		if (! bIntersectedTerrain)
		{
			if (ppObjectIntersected)
			{
				*ppObjectIntersected = pObject;
			}
			if (ppEntityIntersected)
			{
				*ppEntityIntersected = pEntityIntersectedRet;
			}
		}
	}

	return bIntersected;
}

BOOL RayIntersectionEntity( KG3DScene& Scene, const D3DXVECTOR3& vSrc , const D3DXVECTOR3& vNormalizedDir
						   , FLOAT* pDistance, KG3DRepresentObject** ppObj
						   , KG3DSceneEntity** ppEntityIntersected
						   , FLOAT fTestRange /*= EM_RAY_INTERSECTION_MAX_RANGE*/
						   , KG3DIntersectionFilter* pFilter /*= NULL*/)
{
	if (fTestRange < FLT_EPSILON)
		return FALSE;
	
	BOOL bRet = FALSE;
	FLOAT fTempDist = FLT_MAX;
	FLOAT fTestRangeUsed = fTestRange * stfTestRangeBiasPercent;	//�Ŵ�һ����Է�Χ����Ȼ�᲻׼

	KG3DSceneOutDoorSpaceMgr* pOutDoor = Scene.GetOutDoorSpaceMgr();
	if (NULL != pOutDoor)
	{
		bRet = Private::RayIntersectionEntity(*pOutDoor, vSrc, vNormalizedDir, &fTempDist, ppObj, fTestRangeUsed, pFilter);
	}

	if (bRet && ! (fTempDist > -FLT_EPSILON && fTempDist < fTestRange))
	{
		bRet = FALSE;	
	}

	if(bRet && NULL != pDistance)
		*pDistance = fTempDist;

	return bRet;
}

namespace Private
{
	FLOAT GetGoundHeight(KG3DSceneOutDoorSpaceMgr& outDoor, FLOAT fX, FLOAT fZ, DWORD dwMask)
	{
		_ASSERTE((EM_RAY_INTERSECTION_MASK_TERRAIN_PART & dwMask) && _T("����ײ���������"));
		FLOAT fRet = -FLT_MAX;
		FLOAT fMinHeightPossible = -FLT_MAX / 2;
		if (dwMask & EM_RAY_INTERSECTION_MASK_TERRAIN)
		{
			FLOAT fGoundHeight = outDoor.GetGroundHeight(fX, fZ);
			if (fGoundHeight < fMinHeightPossible) //û�м������
			{
				fGoundHeight = 0;
			}
			fRet = max(fRet, fGoundHeight);
		}
		if (dwMask & EM_RAY_INTERSECTION_MASK_WATER)
		{
			FLOAT fWaterHeight = -FLT_MAX;
			HRESULT hr  = outDoor.GetWaterHeight(fWaterHeight, fX, fZ);
			if (FAILED(hr))
			{
				fWaterHeight = 0;					
			}
			fRet = max(fRet, fWaterHeight);
		}

		_ASSERTE(fRet - fMinHeightPossible > -FLT_EPSILON);
		return fRet;
	}
};


BOOL RayIntersectionTerrain( KG3DScene& Scene, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir
							, FLOAT* pfDist, FLOAT fTestRange /*= EM_RAY_INTERSECTION_MAX_RANGE*/ 
							, DWORD dwMask /*= EM_RAY_INTERSECTION_MASK_TERRAIN*/)
{
	//����OutDoorIntersection�������ṹ
	struct GroundHeightGetter : public KG3DOutDoorIntersectionGroundHeightGetter 
	{
		KG3DSceneOutDoorSpaceMgr* m_pOutDoor;
		DWORD	m_dwMask;
		virtual FLOAT GetGroundHeight(FLOAT fX, FLOAT fZ)
		{
			return Private::GetGoundHeight(*m_pOutDoor, fX, fZ, m_dwMask);
		}
	};
	BOOL bIsMaskValid = (dwMask & EM_RAY_INTERSECTION_MASK_TERRAIN_PART);
	_ASSERTE(bIsMaskValid && _T("��ײ�ɰ����ָ�����ٵ��Σ�ˮ����֮һ"));
	KG_PROCESS_ERROR(bIsMaskValid);
	{

		static GroundHeightGetter s_getter;
		s_getter.m_pOutDoor = Scene.m_lpSceneOutDoorSpaceMgr;
		s_getter.m_dwMask = dwMask;

		KG3DOutDoorIntersection	outDoorIntersection(s_getter);
		RayIntersectionTerrainAccuracyStruct Accuracy;
		FLOAT fTempRet = 0;

		if (fTestRange < FLT_EPSILON)
			return FALSE;

		KG_PROCESS_ERROR(NULL != Scene.m_lpSceneOutDoorSpaceMgr);

		//���徫�Ƚṹ��
		Accuracy.SetMaxRange(fTestRange * stfTestRangeBiasPercent);	//�Ŵ�һ����Է�Χ����Ȼ�᲻׼
		if (Accuracy.m_fTestRangePerLineSection > Accuracy.m_fMaxRange)
		{
			Accuracy.m_fTestRangePerLineSection = Accuracy.m_fMaxRange;
		}

		BOOL bRet = outDoorIntersection.RayIntersectionTerrain(vSrc, vNormalizedDir, &fTempRet, &Accuracy);
		KG_PROCESS_ERROR(bRet);

		KG_PROCESS_ERROR(fTempRet > -FLT_EPSILON && fTempRet < fTestRange);

		if(NULL != pfDist)
			*pfDist = fTempRet;

		return TRUE;
	}
Exit0:
	return FALSE;
}

BOOL RayIntersectionVertical( KG3DScene& sceneIn, const D3DXVECTOR3& vSrcInput , FLOAT* pfIntersectionHeight, KG3DRepresentObject** ppObject
							 , DWORD dwMask, KG3DIntersectionFilter* pFiler /*= NULL*/ )
{
	static D3DXVECTOR3 vDirDown(0, -1, 0);
	const FLOAT fHeightBias = 10;
	
	FLOAT fDistanceRet = FLT_MAX;	

	static bool bForceReturn = false;
	if (bForceReturn)
	{
		return FALSE;
	}

	D3DXVECTOR3 vSrc = vSrcInput;
	vSrc.y += fHeightBias;	//��Ҫ����һ��߶�������ײ����Ȼһ�㸡�����ͺ����������������

	_ASSERTE(0 != dwMask);
	KG_PROCESS_ERROR(0 != dwMask);

	{
		KG3DSceneOutDoorSpaceMgr* pOutDoor = sceneIn.GetOutDoorSpaceMgr();
		KG_PROCESS_ERROR(NULL != pOutDoor);

		if (dwMask & EM_RAY_INTERSECTION_MASK_TERRAIN_PART)
		{	
			FLOAT fTerrainHeight = Private::GetGoundHeight(*pOutDoor, vSrc.x, vSrc.z, dwMask);

			//KG_PROCESS_ERROR(fTerrainHeight <= vSrc.y);//�����ԭ�����壬������Ϊ�˸���������Ĵ���
			KG_PROCESS_ERROR(fTerrainHeight - vSrc.y < FLT_EPSILON);
			fDistanceRet = vSrc.y - fTerrainHeight;
		}

		if (dwMask & EM_RAY_INTERSECTION_MASK_OBJECT)
		{
			struct Visitor : public KG3DEntityCtVisitorForShell
			{
				FLOAT m_fDistance;
				D3DXVECTOR3 m_vSrc;
				KG3DRepresentObject* m_pObjectIntersected;
				KG3DIntersectionFilter* m_pFilter;
				virtual HRESULT Accept(KG3DEntityObjectShell& objShell)
				{
					if ((objShell.m_vBBox_A.x <= m_vSrc.x)&&(objShell.m_vBBox_A.z <= m_vSrc.z)&&
						(objShell.m_vBBox_B.x >= m_vSrc.x)&&(objShell.m_vBBox_B.z >= m_vSrc.z))
					{
						if(NULL == objShell.m_lpObject)
							return S_OK;						

						FLOAT fRetTemp = -FLT_MAX;
						BOOL bRet = objShell.m_lpObject->RayIntersection(m_vSrc, vDirDown, &fRetTemp, FALSE, m_pFilter);
						if (! bRet)
							return S_OK;
						
						_ASSERTE(fRetTemp > -FLT_EPSILON);
						if (fRetTemp < m_fDistance)
						{
							m_fDistance = fRetTemp;
							m_pObjectIntersected = objShell.m_lpObject;
						}
					}
					return S_OK;
				}
			};

			_ASSERTE(fDistanceRet >= 0);

			Visitor visitorTemp;
			visitorTemp.m_fDistance = fDistanceRet;
			visitorTemp.m_pObjectIntersected = NULL;
			visitorTemp.m_vSrc = vSrc;
			visitorTemp.m_pFilter = pFiler;

			pOutDoor->Traverse(visitorTemp, vSrc.x, vSrc.z, vSrc.x, vSrc.z);

			if (visitorTemp.m_fDistance < fDistanceRet)
			{
				fDistanceRet = visitorTemp.m_fDistance;
				if ((dwMask & EM_RAY_INTERSECTION_MASK_OBJECT) && NULL != ppObject)
				{
					_ASSERTE(NULL != visitorTemp.m_pObjectIntersected);
					*ppObject = visitorTemp.m_pObjectIntersected;
				}
			}
		}
		
		KG_PROCESS_ERROR(fDistanceRet >= 0);

		FLOAT fRet = vSrc.y - fDistanceRet;

		const FLOAT fBiasInput = 0.1f;
		KG_PROCESS_ERROR(fRet < vSrcInput.y + fBiasInput);	//���߻����һ��飬�����ܱ��⸡�������ǽ������Ҫ��Input��y���õͲŶԡ���������жϲ�����fRet <= vSrcInput.y����Ϊ���õ��Ľ���У���Ϊ������fRet���ܻ��vSrcInput.y����0.001�����ֵ�����ʧ�ܣ�����д��bias��

		if (NULL != pfIntersectionHeight)
		{
			*pfIntersectionHeight = fRet;
		}
		
		return TRUE;
	}
Exit0:
	return FALSE;
}
BOOL Private::RayIntersectionEntity( KG3DSceneEntityContainer& Container, const D3DXVECTOR3& vSrc
						   , const D3DXVECTOR3& vNormalizedDir, FLOAT* pDistance, KG3DRepresentObject** ppObj
						   , FLOAT fTestRange
						   , KG3DIntersectionFilter* pFilter
						   , BOOL bNewFormatOrOldFormat)
{
	if (fTestRange < FLT_EPSILON)
		return FALSE;

	struct Visitor : public KG3DRepresentObjectVisitor
	{
		D3DXVECTOR3 m_vSrc;
		D3DXVECTOR3 m_vNormalizedDir;
		//D3DXVECTOR3 m_vDes;
		FLOAT		m_fDistance;
		FLOAT		m_fRange;
		KG3DRepresentObject* m_pObjIntersected;
		KG3DIntersectionFilter* m_pFilter;

		virtual HRESULT Accept(KG3DRepresentObject& Obj)
		{
			FLOAT fDisTemp = 0;
		
			if(Obj.RayIntersection(m_vSrc, m_vNormalizedDir, &fDisTemp, FALSE, m_pFilter))
			{
				if (fDisTemp < m_fDistance && fDisTemp > 0 && fDisTemp < m_fRange)
				{
					m_fDistance = fDisTemp;
					m_pObjIntersected = &Obj;
				}
			}
			return S_OK;//�����ѭ��
		}
	};

	Visitor visitor;
	visitor.m_vSrc = vSrc;
	visitor.m_vNormalizedDir = vNormalizedDir;
	//visitor.m_vDes = vSrc + vNormalizedDir * fTestRange;
	visitor.m_fDistance = FLT_MAX;	//����һ���ܴ��ֵ
	visitor.m_fRange = fTestRange;
	visitor.m_pObjIntersected = NULL;
	visitor.m_pFilter = pFilter;

	KG_PROCESS_ERROR(fTestRange > 0);
	bNewFormatOrOldFormat ? Container.Traverse(visitor) : Container.TraverseOldFormat(visitor);
	if (visitor.m_pObjIntersected != NULL)
	{
		KG_PROCESS_ERROR(visitor.m_fDistance < fTestRange);
		if (NULL != pDistance)
		{
			*pDistance = visitor.m_fDistance;
		}
		if (NULL != ppObj)
		{
			*ppObj = visitor.m_pObjIntersected;
		}
		return TRUE;
	}

Exit0:
	return FALSE;
}

#define RAY_INTERSECTION_CHECK_VISITED 0

BOOL Private::RayIntersectionEntity( KG3DSceneOutDoorSpaceMgr& OutDoor, const D3DXVECTOR3& vSrc
					 , const D3DXVECTOR3& vNormalizedDir, FLOAT* pDistance
					 , KG3DRepresentObject** ppObj
					 , FLOAT fTestRange /*= EM_RAY_INTERSECTION_MAX_RANGE*/
					 , KG3DIntersectionFilter* pFilter /*= NULL*/)
{
	if (fTestRange < FLT_EPSILON)
		return FALSE;

	struct Visitor : public KG3DEntityCtVisitor 
	{
		D3DXVECTOR3	m_vSrc;
		D3DXVECTOR3 m_vNormalizedDir;
		FLOAT		m_fTestRange;
		FLOAT m_fIntersectionPointToSrc;
		KG3DRepresentObject* m_pObject;
		KG3DIntersectionFilter* m_pFilter;
		DWORD	m_VisitedMask;
		enum
		{
			EM_VISITED_ZONE = 1 << 2,
			EM_VISITED_SECTION = 1 << 1,
			EM_VISITED_REGION = 1,
			EM_VISITED_ALL = EM_VISITED_ZONE | EM_VISITED_SECTION | EM_VISITED_REGION,
		};
#if RAY_INTERSECTION_CHECK_VISITED
#if defined(DEBUG) | defined(_DEBUG)//������ϵ�Ϳ��Եõ���ֵ��
		INT	m_nZoneVisited;
		INT m_nSectionVisited;
		INT m_nRegionVisited;
#endif
#endif
		virtual HRESULT Accept(KG3DSceneEntityContainer& Container)
		{	
#if RAY_INTERSECTION_CHECK_VISITED//������ϵ�Ϳ��Եõ���ֵ��
#if defined(DEBUG) | defined(_DEBUG)
			{
				DWORD dwType = Container.GetType();
				switch(dwType)
				{
				case KG3DTYPE_SCENE_ZONE:
					++m_nZoneVisited;
					break;
				case KG3DTYPE_SCENE_SECTION:
					++m_nSectionVisited;
					break;
				case KG3DTYPE_SCENE_REGION:
					++m_nRegionVisited;
					break;
				}
			}
#endif
#endif
			FLOAT fDistance = 0;
			KG3DRepresentObject* pObject = NULL;
			BOOL bRet = Private::RayIntersectionEntity(Container, m_vSrc, m_vNormalizedDir
				, &fDistance, &pObject, m_fTestRange, m_pFilter, TRUE);
			if (bRet && fDistance < m_fIntersectionPointToSrc && fDistance > 0 && fDistance < m_fTestRange)
			{
				m_fIntersectionPointToSrc = fDistance;
				_ASSERTE(pObject);
				m_pObject = pObject;

				DWORD dwType = Container.GetType();
				switch(dwType)
				{
				case KG3DTYPE_SCENE_ZONE:
					m_VisitedMask |= EM_VISITED_ZONE;
					break;
				case KG3DTYPE_SCENE_SECTION:
					m_VisitedMask |= EM_VISITED_SECTION;
					break;
				case KG3DTYPE_SCENE_REGION:
					m_VisitedMask |= EM_VISITED_REGION;
					break;
				}

				if (m_VisitedMask == EM_VISITED_ALL)
				{
					return (HRESULT)EM_VISITOR_STOP;	//ȫ�����ʹ�3���ˣ���Ϊ������Ƕ�׵ģ����Կ϶����ڵõ���������㣬�����Ͳ��ü�����
				}
				else
				{
					return (HRESULT)EM_VISITOR_BREAK;//��Ϊ��������ȣ�����һ���㼶�еõ�һ������Ϳ���ȥ��һ���㼶��
				}
			}
			return (HRESULT)EM_VISITOR_CONTINUE;

		}
	};

	//_KG3D_DEBUG_OUTPUT0("Begin Inter\n");
	Visitor visitor;
	D3DXVECTOR3 vDes = vSrc + vNormalizedDir * fTestRange;
#if 0	//���ڼ����㷨��ʱ����
	static D3DXVECTOR3 stTest(0,300,0);
	visitor.m_vSrc = stTest;//vSrc;
	//visitor.m_vNormalizedDir = vNormalizedDir;
	static D3DXVECTOR3 v45 = D3DXVECTOR3(1,0,1);
	D3DXVec3Normalize(&v45, &v45);
	visitor.m_fTestRange = fTestRange;
	visitor.m_vNormalizedDir = v45;
	visitor.m_fIntersectionPointToSrc = FLT_MAX;
	visitor.m_pObject = NULL;

	D3DXVECTOR2 v2DSrc = D3DXVECTOR2(-120, -120);
	D3DXVECTOR2 v2DDes = D3DXVECTOR2(EM_RAY_INTERSECTION_MAX_RANGE, EM_RAY_INTERSECTION_MAX_RANGE);
#else
	visitor.m_vSrc = vSrc;
	visitor.m_vNormalizedDir = vNormalizedDir;
	visitor.m_fTestRange = fTestRange;
	visitor.m_fIntersectionPointToSrc = FLT_MAX;
	visitor.m_pObject = NULL;


	D3DXVECTOR2 v2DSrc = D3DXVECTOR2(vSrc.x, vSrc.z);
	D3DXVECTOR2 v2DDes = D3DXVECTOR2(vDes.x, vDes.z);
#endif
	visitor.m_pFilter = pFilter;
	visitor.m_VisitedMask = 0;

#if RAY_INTERSECTION_CHECK_VISITED
#if defined(DEBUG) | defined(_DEBUG)
	visitor.m_nZoneVisited = visitor.m_nSectionVisited = visitor.m_nRegionVisited = 0;
#endif
#endif

	KG_PROCESS_ERROR(fTestRange > 0);
	OutDoor.Traverse(visitor, v2DSrc, v2DDes);
	//_KG3D_DEBUG_OUTPUT0("End Inter\n");
	KG_PROCESS_ERROR(NULL != visitor.m_pObject);	//û����ײ

	if (pDistance)
	{
		*pDistance = visitor.m_fIntersectionPointToSrc;
	}
	if (ppObj)
	{
		*ppObj = visitor.m_pObject;
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL Private::RayIntersectionEntity( KG3DSceneEntity& Entity, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir, FLOAT* pRet, KG3DIntersectionFilter* pFilter /*= NULL*/)
{
	if (NULL != pFilter)
	{
		HRESULT hr = pFilter->Accept(Entity);
		if (FAILED(hr))
			return FALSE;
	}
	//������Entity�ϸ��̬�ˣ������Ժ�Ҫȥ���ģ��������ɡ�C��ʽ�Ĳ�������Щ
	switch(Entity.GetType())
	{
	case SCENEENTITY_OBJECT:
		{
			KG3DRepresentObject* pObj = static_cast<KG3DRepresentObject*>(Entity.m_pPointer);
			if (pObj)
			{
				return pObj->RayIntersection(vSrc, vNormalizedDir, pRet, FALSE, pFilter);
			}
		}
		break;
	case SCENEENTITY_MODEL:
		{
			KG3DModel* pModel = static_cast<KG3DModel*>(Entity.m_pPointer);
			if (pModel)
			{
				return pModel->RayIntersection(vSrc, vNormalizedDir, pRet,NULL, FALSE, pFilter);
			}
		}
	    break;
	default:
	    break;
	}
	return FALSE;
}
#if 0
BOOL Private::RayIntersectionEntityMulti( KG3DSceneEntity& Entity , const D3DXVECTOR3* pvSrcArray , const D3DXVECTOR3* pvNormalizedDirArray , FLOAT* pRetArray , size_t Count , KG3DIntersectionFilter* pFilter /*= NULL*/ )
{
	if (NULL == pvSrcArray || NULL == pvNormalizedDirArray || Count > MAX_INTERSECTION_RAY_COUNT)
		return NULL;

	if (NULL != pFilter)
	{
		HRESULT hr = pFilter->Accept(Entity);
		if (FAILED(hr))
			return FALSE;
	}

	static FLOAT RetArray[MAX_INTERSECTION_RAY_COUNT];
	FLOAT* pRetArrayToUse = pRetArray == NULL ? RetArray : pRetArray;
	BOOL bRet = FALSE;

	//������Entity�ϸ��̬�ˣ������Ժ�Ҫȥ���ģ��������ɡ�C��ʽ�Ĳ�������Щ
	switch(Entity.GetType())
	{
	case SCENEENTITY_OBJECT:
		{
			KG3DRepresentObject* pObj = static_cast<KG3DRepresentObject*>(Entity.m_pPointer);
			if (pObj)
			{
				for (size_t i = 0; i < Count; ++i)
				{
					bRet |= pObj->RayIntersection(pvSrcArray[i], pvNormalizedDirArray[i]
						, pRetArrayToUse[i], pFilter);
				}
			}
		}
		break;
	case SCENEENTITY_MODEL:
		{
			KG3DModel* pModel = static_cast<KG3DModel*>(Entity.m_pPointer);
			if (pModel)
			{
				for (size_t i = 0; i < Count; ++i)
				{
					bRet |= pModel->RayIntersection(pvSrcArray[i], pvNormalizedDirArray[i]
							, pRetArrayToUse[i], pFilter);
				}
			}
		}
		break;
	default:
		break;
	}
	return bRet;
}

#endif

#ifdef KG_ENABLE_TDD
KG_TEST_BEGIN(KG3DIntersection)
{
	DWORD dwMask = EM_RAY_INTERSECTION_MASK_TERRAIN_PART;
	_ASSERTE(dwMask & EM_RAY_INTERSECTION_MASK_TERRAIN);
	_ASSERTE(dwMask & EM_RAY_INTERSECTION_MASK_WATER);

	_ASSERTE(! (dwMask & EM_RAY_INTERSECTION_MASK_OBJECT));

	dwMask = EM_RAY_INTERSECTION_MASK_TERRAIN;
	_ASSERTE(dwMask & EM_RAY_INTERSECTION_MASK_TERRAIN_PART);
	dwMask = EM_RAY_INTERSECTION_MASK_WATER;
	_ASSERTE(dwMask & EM_RAY_INTERSECTION_MASK_TERRAIN_PART);
}
KG_TEST_END(KG3DIntersection)
#endif