#pragma once
#include "../../../Include/kg3dengine/kg3dinterface.h"
#include "../EditInterface/IEOther.h"	//by dengzhihui Refactor 2006��11��2��
#include "KG3DRecorder.h"
class KG3DScene;
class KG3DTrackCamera;
class KG3DCamera :
	public IEKG3DCamera //by dengzhihui Refactor 2006��11��2��
{
protected :
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	BOOL        m_bPerspective;         //  ��ǰ��ͶӰģʽ��ͬʱ������֤�ͷţ���Ҫ�ĳ�bool�ͺ�
    BOOL        m_bGlobalPerspective;   //  ��ͶӰģʽ
    BOOL        m_bPerspectiveSave;
	D3DXVECTOR3 m_DefaultUp;

	KG3DCameraPersepectiveData m_PerspectiveData;
	KG3DCameraPersepectiveData m_GlobalPerspectiveData;
	KG3DCameraPersepectiveData m_PerspectiveDataSave;

	KG3DCameraOrthogonalData m_OrthogonalData;
	KG3DCameraOrthogonalData m_GlobalOrthogonalData;
	KG3DCameraOrthogonalData m_OrthogonalDataSave;

	D3DXMATRIX m_MatrixView;
	D3DXMATRIX m_MatrixProject;

	KG3DScene*      m_lpCurScene;           // ��ǰ��������ΪScene�϶��Ǳ�Camera���ͷŵģ�����Camera��Ч��Scene����Ч�����õ�����������

    D3DXVECTOR3     m_vOffset;

    D3DXVECTOR3     m_vRight;
    D3DXVECTOR3     m_vFront;
    D3DXVECTOR3     m_vUp;

public:
	UINT m_uSceneSpaceNode;
	DWORD m_dwRecordID;		

    virtual HRESULT ClearOffset() { m_vOffset = D3DXVECTOR3(0.f, 0.f, 0.f);  return S_OK; }
	virtual HRESULT ComputeMatrix();	
    virtual HRESULT FrameMove() {return E_NOTIMPL;}
	virtual KG3DTrackCamera* GetTrackCameraInterface(){return NULL;}
private:
	//��ײ��::RayIntersection��ת����Scene�������Ϊ�˷����ⲿ�õģ��ڲ���Ҫ�ã����Է���private����
	virtual BOOL GetCrossPosition(const D3DXVECTOR3& VS, const D3DXVECTOR3& VD, D3DXVECTOR3* pVecCrossPos);
	void ComputeViewMatrix();
	void ComputePerspectiveMatrix();
protected:
	virtual BOOL RayIntersectionCamera(const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vDir, FLOAT fTestRange, FLOAT* pfRet);

	//������Щ������TrackCamera�õģ���Camera����ʵ����Щ����
public:
	virtual HRESULT GetStation(IEKG3DCameraStation** pStation){return E_NOTIMPL;}
	virtual HRESULT SetTrackInfo(TrackCameraInfo  trackInfo){return E_NOTIMPL;}
	virtual HRESULT GetTrackInfo(TrackCameraInfo* pTrackInof){return E_NOTIMPL;}
	virtual DWORD GetCurrentState(){return 0;}
public:
	const KG3DCameraPersepectiveData&	GetPerspectiveData()	{return m_PerspectiveData;}
	const KG3DCameraOrthogonalData&		GetOrthogonalData()		{return m_OrthogonalData;}

	const D3DXMATRIX&					GetViewMatrix(){return m_MatrixView;}
	const D3DXMATRIX&					GetProjectionMatrix(){return m_MatrixProject;}

    HRESULT Capture();			//���浱ǰ��Presepective����
    HRESULT RestoreCapture();	//���»ָ���ǰ��Presepective����
    HRESULT RestoreGlobalProject();
    HRESULT GetGlobalPerspective(float* pfFovy, float* pfAspect, float* pfzNear, float* pfzFar);
    HRESULT SetGlobalPerspective(float fFovy, float fAspect, float fzNear, float fzFar);
    HRESULT GetGlobalOrthogonal(float* pfWidth, float* pfHeight, float* pfzNear, float* pfzFar);
    HRESULT SetGlobalOrthogonal(float fWidth, float fHeight, float fzNear, float fzFar);
    HRESULT GetNearPlaneRect(D3DXVECTOR3& A, D3DXVECTOR3& B, D3DXVECTOR3& C, D3DXVECTOR3& D, D3DXVECTOR3& Center);

	//Camera��Set��UpdateCamera��
    inline D3DXVECTOR3 GetCameraRight(){ return m_vRight; }
    inline D3DXVECTOR3 GetCameraFront(){ return m_vFront; }
    inline D3DXVECTOR3 GetCameraUp()   { return m_vUp; }

	inline D3DXVECTOR3 GetPosition(){return m_Position;}
    inline D3DXVECTOR3 GetLookAtPosition() { return m_LookAt; }
    inline void SetSwingOffset(const D3DXVECTOR3& vOffset) { m_vOffset = vOffset; }

	HRESULT SetCamera();//�趨View��Projection����
	HRESULT	PushMatrixToDevice();	//��Matrix�赽Device���棬�����õĻ�������������

	virtual IKG3DCamera* GetNew();
	virtual HRESULT Copy(IKG3DCamera* pCamera);
    virtual HRESULT UpdateCamera(D3DXVECTOR3 vecPosition, D3DXVECTOR3 vecTargetPos
								, float fLookAtOffset, float fTargetSpeed);//�������TrackCamera�Ļ�������ȫ���ⲿ���ƣ�û���ڵ����͵��ɵĹ���
	
	virtual HRESULT BindScene(KG3DScene* lpCurScene);	//��һ��Scene
	virtual HRESULT UnBindScene();	//�Ժ�Ҫ���¸Ľ�����ҪCamera�����Sceneָ����
	/************************************************************************/
	/*
	Ϊ��ҪMatrix�͸���Vectorͬ����ÿSetһ�¾�Ҫ���¼���һ��Matrix���ܺĵ�
	������ĺ���һ�����ö��Vector�������ظ�����Matrix�����ֻҪ�����е�һ��
	�򼸸�Vector������������ΪNULL
	*/
	/************************************************************************/
	virtual HRESULT SetPositionLookAtUp(const D3DXVECTOR3* pPos, const D3DXVECTOR3* pLookAt, const D3DXVECTOR3* pUp);//һ����3�������Լ���
	virtual HRESULT GetPosition(D3DXVECTOR3 *pPos);
	virtual HRESULT SetPosition(D3DXVECTOR3 Pos ,BOOL bOutCall = FALSE);//������SetPositionLookAtUpһ����3��ֵ���ֿ���Ļ���ÿ�ζ�Ҫ����һ�ξ���
	virtual HRESULT SetLookAtPosition(D3DXVECTOR3 Pos,BOOL bOutCall = FALSE);//������SetPositionLookAtUpһ����3��ֵ���ֿ���Ļ���ÿ�ζ�Ҫ����һ�ξ���
	
	virtual HRESULT GetLookAtPosition(D3DXVECTOR3 *pPos);
	virtual HRESULT GetUpDirection(D3DXVECTOR3 *pUp);
	virtual HRESULT SetUpDirection(D3DXVECTOR3 Up, BOOL bOutCall = FALSE);//������SetPositionLookAtUpһ����3��ֵ���ֿ���Ļ���ÿ�ζ�Ҫ����һ�ξ���

	virtual HRESULT GetFront(D3DXVECTOR3* pFront);
	virtual HRESULT GetRight(D3DXVECTOR3* pRight);

	int				GetQuadrant();

	virtual BOOL    IsPerspective();

	virtual HRESULT GetPerspective(float *pfFovy, float *pfAspect, float *pfzNear, float *pfzFar);
	virtual HRESULT SetPerspective(float fFovy, float fAspect, float fzNear, float fzFar);

	virtual HRESULT GetOrthogonal(float *pfWidth, float *pfHeight, float *pfzNear, float *pfzFar);//��ȡ����ͶӰ����
	virtual HRESULT SetOrthogonal(float fWidth, float fHeight, float fzNear, float fzFar);

	virtual HRESULT GetMatrix(D3DXMATRIX* pMatrixView, D3DXMATRIX* pMatrixProjection);
///////////ADD BY HAUNGJINSHOU 2008-1-30/////////////////////////
public:
	float GetHeightDistance();
	void  SetHeightDistance(float& fVal);
	float m_fHeightDistance; //��ǰ�����λ����Ŀ��λ�õĸ߶Ȳ�
//////////////////////////////////////////////////////////////////
public:
	KG3DCamera(void);
	virtual ~KG3DCamera(void);
private:
	//���뱣֤�������⼸����������Ȼ�����������������
	void SetPositionPrivate(const D3DXVECTOR3& Pos);
	void SetLookAtPostionPrivate(const D3DXVECTOR3& Pos );
	void SetUpDirectionPrivate(const D3DXVECTOR3& Up );

	// Inherited via IEKG3DCamera
	virtual HRESULT GetUp(D3DXVECTOR3 * pUp) override;
	virtual HRESULT EnableHighPrecise(BOOL bEnable) override;
};
