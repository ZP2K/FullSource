////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DOutDoorAssistantFuncs.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-7 10:56:35
/*  Comment     : ���εĸ�������,������������㷨�ģ��;�������޹أ��򵥵��滻��
DataInterface�Ϳ���Ϊ��ͬ�������

*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DOUTDOORASSISTANTFUNCS_H_
#define _INCLUDE_KG3DOUTDOORASSISTANTFUNCS_H_

////////////////////////////////////////////////////////////////////////////////
class KG3DSceneOutDoorSpaceMgr;


struct RayIntersectionTerrainAccuracyStruct 
{
	FLOAT m_fMaxRange;			//ֻ�������N�����ڵ���ײ������ײ�߱����ײ�߶�,��������Զ����ײ���
	FLOAT m_fTestRangePerLineSection;			//����ײ�߶ηֳɼ������߶�

	FLOAT m_fMinTestRange;		//���������ײ�߶ε������˵㶼�ڵ����ϣ���ײҲ�������м䷢����ҲҪ��顣������ڶ��徿��Ҫ��������м��ͻ�𵽶ྫϸ�ĳ̶�
	FLOAT m_fMaxTangent;		//��������5/1������鵽�㡣������ο����жම�ͣ��������̫������̫���͹����ô�㷨Ҫ���Ǻ��ܵ�ȡ�������򣬿��Ժ�����Щ��ײ��ǰ�����˵㶼����κܸߵĲ���
	FLOAT m_fMaxAccuracy;		//����ײ�߶�ϸ�ֵ�N����֮�ڣ���ֵ�Ľ���Ϳ��Խ�����

	FLOAT m_fMinGap;			//����õ��ĵ�Ͳ��Ե�ĸ߶Ȳ��������Χ�ڣ���ô���Խ���
	INT	  m_nMaxGetHeightCount;	//ÿ����RayIntersection�������30��

	RayIntersectionTerrainAccuracyStruct();

	VOID SetMaxRange(FLOAT fRange);//����ײ������Ӵ��ʱ�򣬾���Ҳ��Ӧ�Ŵ󣬲�Ȼ����Զ���뺯����˵���ܷ�;
};

struct KG3DOutDoorIntersectionGroundHeightGetter 
{
	virtual FLOAT GetGroundHeight(FLOAT fX, FLOAT fZ) = 0;
	virtual ~KG3DOutDoorIntersectionGroundHeightGetter() = 0{}		 
};

class KG3DOutDoorIntersection
{
	//<��ײ���>
public:
	BOOL	RayIntersectionTerrain(D3DXVECTOR3 vSrc, D3DXVECTOR3 vNormalizedDir, FLOAT* pfDist
		, RayIntersectionTerrainAccuracyStruct* pAccuracy = NULL);//���߲齻�㡣����ʹ��ֱ�ߵĲ���ʽ P(t) = vSrc + vDir * t; �����ƽ����Χ���ֵ��

private:
	struct IntersectionRay	//��ײ�ߵĽṹ��
	{
		D3DXVECTOR3 m_vSrc;
		D3DXVECTOR3 m_vNormalizedDir;
		FLOAT		m_fDirCosine;	//Dir��y=0ƽ����ͶӰ�ļнǵ����ң���������Ͳ���ʽ�����T���ܹ������׵ļ���������y=0ƽ���ϵľ���
		IntersectionRay(){}
		IntersectionRay(const IntersectionRay& Other)
		{
			memcpy_s(this, sizeof(IntersectionRay), &Other, sizeof(IntersectionRay));
		}
		IntersectionRay& operator=(const IntersectionRay& Other)
		{
			memcpy_s(this, sizeof(IntersectionRay), &Other, sizeof(IntersectionRay));
			return *this;
		}
	};
	struct IntersectionTestPoint	//��ײ��Ľṹ��
	{
		D3DXVECTOR3 m_Pos;
		FLOAT	m_fT;		//�ò���ʽʱ���T
		FLOAT	m_fDeltaToTerrain;//��һ������εľ��룬����ʾ���ڵ�������
		IntersectionTestPoint(){}
		IntersectionTestPoint(const IntersectionTestPoint& Other)
		{
			memcpy_s(this, sizeof(IntersectionTestPoint), &Other, sizeof(IntersectionTestPoint));
		}
		IntersectionTestPoint& operator=(const IntersectionTestPoint& Other)
		{
			memcpy_s(this, sizeof(IntersectionTestPoint), &Other, sizeof(IntersectionTestPoint));
			return *this;
		}
	};
	FLOAT	GetIntersectionPoint(const IntersectionRay& Ray
									, const IntersectionTestPoint& StartP
									, const IntersectionTestPoint& EndP
									, const RayIntersectionTerrainAccuracyStruct& Accuracy);
	BOOL TestIntersecionInRange(const IntersectionRay& Ray
								, const IntersectionTestPoint& StartP
								, const IntersectionTestPoint& EndP
								, IntersectionTestPoint& RetStart
								, IntersectionTestPoint& RetEnd
								, const RayIntersectionTerrainAccuracyStruct& Accuracy);	
public:
	virtual FLOAT	GetGroundHeight(FLOAT fX, FLOAT fZ);//ת��
private:
	INT	m_nGetGoundHeightCount;
	//</��ײ���>
public:
	KG3DOutDoorIntersection(KG3DOutDoorIntersectionGroundHeightGetter& DataInterface);
private:
	KG3DOutDoorIntersectionGroundHeightGetter& m_Data;
};

namespace KG3D_OUTDOOR
{
	ULONG	GetObjectCount(KG3DSceneOutDoorSpaceMgr& outDoorMgr, DWORD dwTypeFilter);//dwType=REPRESENTOBJECT_NONE��ʱ��ֱ��ͳ�ƣ�Ч�ʽϸߣ�����͹��˶�Ӧ����
};

#endif //_INCLUDE_KG3DOUTDOORASSISTANTFUNCS_H_
