////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DIntersection.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-8 9:57:54
/*  Comment     : ��C��ʽд��ײ���棬ʡ�ĺ͸������γ�ǿ��ϡ����ͷ�ļ������˺ܶ��������
��Ҫ��Scene�йص�������ĵط��������ͷ�ļ������Ǹ��߲��㷨���Ͳ�ʵ�ֲ�Ӧ�ú�����ϡ�
����ж�����Ҫ�Ͳ������ģ���KG3DIntersectionBase.h
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DINTERSECTION_H_
#define _INCLUDE_KG3DINTERSECTION_H_

#include "KG3DIntersectionBase.h"

class KG3DScene;
class KG3DSceneEntityContainer;
class KG3DSceneOutDoorSpaceMgr;

enum EM_RAY_INTERSECTION_MASK
{
	EM_RAY_INTERSECTION_MASK_TERRAIN = 0x01,
	EM_RAY_INTERSECTION_MASK_WATER = 0x02,
	EM_RAY_INTERSECTION_MASK_TERRAIN_PART = 0xff,	//���������Σ�ˮ�����Ժ�������صĲ���
	EM_RAY_INTERSECTION_MASK_OBJECT = 0x100,	
};


////////////////////////////////////////////////////////////////////////////////
/*����ڣ������κ���������е�TestRange��������ʾ�����߳��ȣ����ڽ���Ĺ��ˣ���KG3DIntersectionFilter��
���ص���ײ�������ppObjectIntersectedҲ������ppEntityIntersected���Ӻ��¾ɵ��Ρ�����ֵҪ����ʹ�á�
����Ǵ�vSrc��ʼ��fTestRange֮�ڵ���ײ���Ѳ��Է�Χ�ŵ����޲����Ǹ������⣬���Ϊ���ٶ�Ӧ�þ���ѹ�����Է�Χ
*/
BOOL RayIntersection(KG3DScene& Scene
					 , const D3DXVECTOR3& vSrc
					 , const D3DXVECTOR3& vNormalizedDir
					 , FLOAT* pfRet	//��vSrc������ľ��룬��vSrc+ vNormalizedDir * T���Եõ���������
					 , KG3DRepresentObject** ppObjectIntersected
					 , KG3DSceneEntity** ppEntityIntersected
					 , FLOAT fTestRange = EM_RAY_INTERSECTION_MAX_RANGE
					 , KG3DIntersectionFilter* pFilter = NULL
					 );//ֱ�߽��㣬ֱ��Ϊ��ʽP(t) = vSrc + vDir * t������������������Ķ������ײ�������򲻷��ض���

//ֻ������,��������ǽ��㣬Ҳ�����ǵ����ߵ�ԭ��ľ���, dwMask��EM_RAY_INTERSECTION_MASK�����
BOOL RayIntersectionTerrain(KG3DScene& Scene, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir
							, FLOAT* pfDist
							, FLOAT fTestRange = EM_RAY_INTERSECTION_MAX_RANGE
							, DWORD dwMask = EM_RAY_INTERSECTION_MASK_TERRAIN);

//ֻ�������Ļ����������һ��
BOOL RayIntersectionEntity(KG3DScene& Scene, const D3DXVECTOR3& vSrc
						   , const D3DXVECTOR3& vNormalizedDir, FLOAT* pDistance
						   , KG3DRepresentObject** ppObj, KG3DSceneEntity** pEntityIntersected
						   ,  FLOAT fTestRange = EM_RAY_INTERSECTION_MAX_RANGE
						   , KG3DIntersectionFilter* pFilter = NULL);	//�������ײ�������

//����汾����ֱ�����
BOOL RayIntersectionVertical(KG3DScene& sceneIn, const D3DXVECTOR3& vSrc
									, FLOAT* pfIntersectionHeight, KG3DRepresentObject** ppObject, DWORD dwMask
									, KG3DIntersectionFilter* pFiler = NULL);//dwMask��EM_RAY_INTERSECTION_MASK����ϣ����������ppObject��Maskָ����Object��ײ���᷵����ײ��Objectָ�룬�������ü�����pfIntersectionHeight����ΪNULL�����ص�����ײ��ĸ߶ȣ������������е�����

//����Ҫ��ʽʹ��������Щ�����汾���������RayIntersectionEntity��Scene�İ汾�����ˣ�������Ƿ��¸�ʽ������
namespace Private
{
	BOOL RayIntersectionEntity(KG3DSceneOutDoorSpaceMgr& OutDoor
		, const D3DXVECTOR3& vSrc
		, const D3DXVECTOR3& vNormalizedDir
		, FLOAT* pDistance
		, KG3DRepresentObject** ppObj
		, FLOAT fTestRange = EM_RAY_INTERSECTION_MAX_RANGE
		, KG3DIntersectionFilter* pFilter = NULL);//��������ͬ��

	BOOL RayIntersectionEntity(KG3DSceneEntityContainer& Container
		, const D3DXVECTOR3& vSrc
		, const D3DXVECTOR3& vNormalizedDir
		, FLOAT* pDistance
		, KG3DRepresentObject** ppObj
		, FLOAT fTestRange
		, KG3DIntersectionFilter* pFilter
		, BOOL bNewFormatOrOldFormat);//������ʽ��Container��ײ���õ�����Ľ���������Obj�����Ϊ��ʽP(t) = vSrc + vDir * t�е�t�������������ѡ�����龲̬���塣���Է�Χ��ȫContainer

	BOOL RayIntersectionEntity(KG3DSceneEntity& Entity
		, const D3DXVECTOR3& vSrc
		, const D3DXVECTOR3& vNormalizedDir
		, FLOAT* pRet
		, KG3DIntersectionFilter* pFilter = NULL);

/*
	//�����Ƕ��ߵİ汾������Intersector������İ汾����������İ汾ʵ��
	BOOL RayIntersectionEntityMulti(KG3DSceneEntity& Entity
		, const D3DXVECTOR3* pvSrcArray
		, const D3DXVECTOR3* pvNormalizedDirArray
		, FLOAT* pRetArray
		, size_t Count
		, KG3DIntersectionFilter* pFilter = NULL);//pRetArray��ѡ

	BOOL RayIntersectionEntityNoOutDoorMulti(KG3DScene& Scene
		, const D3DXVECTOR3* pvSrcArray
		, const D3DXVECTOR3* pvNormalizedDirArray
		, FLOAT* pRetArray
		, KG3DSceneEntity** ppEntityIntersectedArray	//ÿ���߷�������ģ������ظ�
		, FLOAT* fTestRangeArray
		, size_t Count;
		, KG3DIntersectionFilter* pFilter = NULL);//ppEntityIntersected��pRetArray��ѡ

	BOOL RayIntersectionEntityMulti(KG3DSceneEntityContainer& Container
		, const D3DXVECTOR3* pvSrcArray
		, const D3DXVECTOR3* pvNormalizedDirArray
		, FLOAT* pDistanceArray
		, KG3DRepresentObject** ppObjArray
		, FLOAT* fTestRangeArray
		, size_t Count;
		, KG3DIntersectionFilter* pFilter
		, BOOL bNewFormatOrOldFormat);
*/
};

#endif //_INCLUDE_KG3DINTERSECTION_H_
