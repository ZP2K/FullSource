#pragma once

enum Frustum {NEAR_LEFT_BOTTOM = 0,NEAR_RIGHT_BOTTOM,NEAR_LEFT_TOP,NEAR_RIGHT_TOP,
			  FAR_LEFT_BOTTOM,     FAR_RIGHT_BOTTOM, FAR_LEFT_TOP, FAR_RIGHT_TOP,};

enum FrustumPlane {NEARPLANE = 0,FARPLANE,LEFTPLANE,RIGHTPLANE,TOPPLANE,BOTTOMPLANE};

enum CULLSTATE
{
	CS_UNKNOWN = 0,      // cull state not yet computed
	CS_INSIDE = 1,       // object bounding box is at least partly inside the frustum
	CS_OUTSIDE = 2,      // object bounding box is outside the frustum
	CS_INSIDE_SLOW = 3,  // OBB is inside frustum, but it took extensive testing to determine this
	CS_OUTSIDE_SLOW = 4, // OBB is outside frustum, but it took extensive testing to determine this
};

class KG3DCamera;
class KG3DPvsPortal;

/*
����Ҫ�ĺ�����Createϵ�з���
���к�����û���ж�Camera��UP�����Ǻ���Ҫ���пյ�ʱ����ϡ�
������portal������ʱ��û���ж��Ƿ��ܹ��������пռ���
*/

class KG3DFrustum
{
	KG_TEST_DECLARE_CLASS_TEST(KG3DFrustum)
public:
	//<��ײ>
	//���԰���ײϸ��һ�£������Ҫ���Ԫ����ײ����Ļ���INTERSECT��INSIDE���Թ���ΪTRUE
	//�����Ҫ����ϸ�Ľ���Ϳ����жϷ��ص�����������
	enum INTERSECT_TYPE
	{
		OUTSIDE = 0,
		INTERSECT = 1,
		INSIDE = 2,
	};

    BOOL PortalVisable(const D3DXVECTOR3 vp[], DWORD dwLookMode) const;

	BOOL IsOBBoxVisible(const D3DXVECTOR3 BoxPos[]) const;
	BOOL IsAABBoxVisible(const AABBOX& BBox) const;

    BOOL	IsNegativeSide(const D3DXPLANE& plane, const D3DXVECTOR3& vCenter, const D3DXVECTOR3& vHalf);
	INT		Intersect(const AABBOX& BBox) const;	//��������enum�е�һ��
	//</��ײ>

	HRESULT GetFrustumXZRect(D3DXVECTOR2& A,D3DXVECTOR2& C);

	//<����>//��Щ���������ʧ�ܵĻ���Frustum����ʹ�ã�����Ĳ����Ѿ�����
	//HRESULT			CalculateViewFrustum(BOOL bGetTransform = TRUE);
	HRESULT			CreateByDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT			CreateByCamera(KG3DCamera& Camera);
	HRESULT			CreateBy2Rays(const D3DXVECTOR3& vSrcA, const D3DXVECTOR3& vDirA,
								const D3DXVECTOR3&	vSrcC, const D3DXVECTOR3& vDirC,FLOAT fDistance);	//����DirӦ���Ѿ���λ��,����Src���̫���ڽӽ����޷����Near��

    //HRESULT         CalcDirection();    
	HRESULT         CreateByObjectPVS(KG3DCamera& Camera, D3DXVECTOR3 ProtalPos[4]);//������������ӿڴ�����׶
	HRESULT			CreateByPointGate(KG3DCamera& Camera
									, const D3DXVECTOR3& BottomLeft
									, const D3DXVECTOR3& BottomRight
									, const D3DXVECTOR3& TopLeft
									, const D3DXVECTOR3& TopRight);//�������ʧ�ܣ����frustum�ǲ����õġ����ĸ��㱣֤�������

	BOOL           CullByPortal(const KG3DPvsPortal& Portal, DWORD dwLookMode);//������������ӿ��޸���׶
	//</����>
	
	HRESULT			DrawFrustumByLine();

	const D3DXVECTOR3&	GetOrigin()const {return m_CameraData.m_vOrigin;}
	const D3DXVECTOR3&	GetPoint(INT nIndex)const;
	INT					GetPointCount()const;
	const D3DXVECTOR3&	GetDirection()const{return m_CameraData.m_vDirection;}
	const D3DXPLANE&	GetPlane(INT nIndex)const;
	INT					GetPlaneCount()const;


	BOOL            IsOBBoxVisibleOptimize(D3DXVECTOR3 BoxPos[]);
	BOOL			IsAABBoxInside(AABBOX& BBox);

	KG3DFrustum();
	~KG3DFrustum();
private:
	

	CULLSTATE KG3DFrustum::CullObjectInside(const D3DXVECTOR3 *ViewFrustumPoint,const D3DXPLANE *ViewFrustumPlane,
		const D3DXVECTOR3 *ObjectBoundPoint,const D3DXPLANE *ObjectBoundPlane);

	VOID	ComputePlanesFromPoints();

//����Ķ�������Private���ã�DEBUGʱд��protected��Ϊ��Ҫ���໯����TDD
#if defined(DEBUG) | defined(_DEBUG)
protected:
#else
private:
#endif
	struct CameraDataStruct;
	HRESULT CreateByGatePrivate(CameraDataStruct& CameraData
					, const D3DXVECTOR3& BottomLeft
					, const D3DXVECTOR3& BottomRight 
					, const D3DXVECTOR3& TopLeft
					, const D3DXVECTOR3& TopRight );
	VOID	ComputeFrustum(const D3DXMATRIX& matView, const D3DXMATRIX& matProj);

	enum{
		em_point_count = 8,
		em_plane_count = 6,
		em_edge_count = 12,
	};

	DWORD			m_dwState;	//��������һЩBOOLֵ�ģ���֤�����ǰ��Ļ�����ʼ����ʱ�������ZeroMemory
	D3DXVECTOR3		m_vPoints[em_point_count];
	static D3DXVECTOR3	m_stVerticesInPPSpace[em_point_count];//��׶����PostProjection�ռ䣬��߶���{1,-1,0}�ģ����Կ��Ա�ɹ���ľ�̬���ݣ����Ը��ݾ��󷴼����ԭ���ĵ㡣˳��������NEAR_LEFT_BOTTOM��enum��˳��һ��
	D3DXPLANE		m_Planes[em_plane_count];//ȫ������

	//D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;
	D3DXMATRIX		m_matViewProj;
	//D3DXMATRIX      m_matVPInv;	//2009-1-20���������������Ҫ���ڣ���ʱ�����Դ�m_matViewProj�����������Щ�ط���û��ά���������������ֱ�Ӵ��	<m_matVPInv Refactor>

	//������Щ�������ô�Camera������Create���������ģ����򲻱�֤����
	struct CameraDataStruct
	{
		D3DXVECTOR3     m_vOrigin;
		FLOAT           m_fZFar;
		FLOAT			m_fZNear;	
		D3DXVECTOR3     m_vDirection;
	};
	CameraDataStruct m_CameraData;

	static HRESULT			ConstructEdges(const D3DXVECTOR3 (&vPoints)[em_point_count], KG3DLine (&edges)[em_edge_count]);//����������һ��KG3DLine���������
};


struct KG3DEntityObjectShell;

struct IKG3DSceneEntityContainer
{
	virtual HRESULT RemoveEntityObjectShell(KG3DEntityObjectShell* pShell) = 0;
	virtual HRESULT InputObjectShell(KG3DEntityObjectShell* pShell, BOOL bCheck2Pvs) = 0;
};
