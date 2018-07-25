////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DTrackCamera.h
//  Version     : 1.0
//  Author      : ZhaoXinyu (Chiyer)
//  CopyRight   : Kingsoft Corporation All rights Reserved
//  History     : 2006-5-18 15:24:45
//  Comment     : 
//  Update      : 2006-12-12 14:17:59 (Chiyer)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DTRACKCAMERA_H_
#define _INCLUDE_KG3DTRACKCAMERA_H_

#include "KG3DCamera.h"
#include "IEOther.h"	//by dengzhihui Refactor 2006��11��6��
#include "KG3DGraphicsStructEx.h"

class KG3DTrackCamera : public KG3DCamera, public IEKG3DTrackCamera //by dengzhihui Refactor 2006��11��6�� �������ָ���Ա��������Ϊ����Ĭ�ϸ�ֵ���� by liumingod
{
public :
	virtual HRESULT GetStation(IEKG3DCameraStation** pStation);//by dengzhihui Refactor 2006��11��6��

    // �󶨵�����(�����ڵ����ʱ��Ҫ�ӵ�ǰ������ȡ�õر���Ϣ)
    virtual HRESULT BindScene(KG3DScene* lpCurScene);

    // ���ڵ����͸���Camera��Ϣ(View Matrix)
    virtual HRESULT FrameMove();

     // ����ӿ� : �������λ��, ��Ҫ�������λ��ʱ����
    virtual HRESULT UpdateCamera(                  
        D3DXVECTOR3 vecPosition,             // -- ���ڴ��ĸ���λ��
        D3DXVECTOR3 vecTargetPos,              // -- ����λ��
        float       fLookAtOffset = 100.0f,  // -- LookAt�㵽����λ�õ�ƫ��ֵ
        float       fTargetSpeed  = 800.f    // -- Ŀ����ƶ��ٶ�
        );

    // ���� / �õ� Camera �ĵ�ǰ���� 
    virtual HRESULT SetTrackInfo(TrackCameraInfo  trackInfo);
    virtual HRESULT GetTrackInfo(TrackCameraInfo* pTrackInof);

    // �õ���ǰ Camera ��״̬���鿴 ENUM_CAMERA_STATE
    virtual DWORD GetCurrentState();

	// ģ��ͻ��˲���
	HRESULT UpdateAsClient(D3DXVECTOR3 vecTargetPos, D3DXVECTOR3 vecTargetDir
		, float fTargerSpeed, DWORD dwTargetMoveType);
	
	virtual IKG3DCamera* GetNew();
	virtual HRESULT Copy(IKG3DCamera* pCamera);
	
	virtual KG3DTrackCamera* GetTrackCameraInterface(){return this;}	
	LRESULT	ProcessMouseWeel(WPARAM wParam, LPARAM lParam);	

    KG3DTrackCamera(void);
    virtual ~KG3DTrackCamera(void);
private:
	// �ڵ�����
	HRESULT ObstructdAvert(D3DXVECTOR3* pVecCameraPos, D3DXVECTOR3* pVecTargerPos);

	/*
	Ϊ�˷�������ʹ�ò�����������ģ������ڣ���ײ��::RayIntersection
    ���VS��VD���߶��볡���ཻ�Ľ��������VS��һ��
	*/
    //BOOL GetCrossPosition(const D3DXVECTOR3& VS, const D3DXVECTOR3& VD, D3DXVECTOR3* pVecCrossPos);

    virtual BOOL GetCrossPosition(
        const D3DXVECTOR3 vSrc[], 
        const D3DXVECTOR3 vDst[],
        int   nCount,
        D3DXVECTOR3* pVecCrossPos
        ); //���Ľ�����õ�һ���Src��vDes������Ľ������Src�ľ���õ�

	LRESULT	ProcessMouseMove(POINT& prevPoint, POINT& currPoint);		
	//HRESULT ProcessLButtonDn(WPARAM wParam, LPARAM lParam);
	//HRESULT ProcessLButtonUp(WPARAM wParam, LPARAM lParam);
	HRESULT SetDistance(float fDistance);
	HRESULT SetAngleHor(float fAngleHor);
	HRESULT SetAngleVel(float fAngleVel);

	float   GetDistance();
	float   GetAngleHor();
	float   GetAngleVel();

	HRESULT AddDistance(float fDistance);
	HRESULT AddAngleHor(float fAngleHor);
	HRESULT AddAngleVel(float fAngleVel);
	HRESULT ZoomDistance(float fMultiple);
private:
	D3DXVECTOR3		m_vecCurPostion;        // ��ǰλ��
	D3DXVECTOR3		m_vecPerfectPos;        // ����λ��  
	D3DXVECTOR3		m_vecSavedPePos;        // ǰһ֡������λ��

	D3DXVECTOR3		m_vecTagPostion;        // ����λ��
	D3DXVECTOR3     m_vecLookAtPos;         // �۲�λ��
	D3DXVECTOR3		m_vecPrevTagPos;        // ǰһ֡������λ��

	D3DXVECTOR3     m_vecCurVelocity;       // ��ǰ�������˶����ٶ�

	int             m_nUseFlexFlag;         // �Ƿ�Ҫ�������˶�(�ڳ����ڵ��󣬴˱�־��ΪTrue)
	int             m_nUpdateFlag;          // ��Ҫ�������λ�ñ�־(UpdateCamera���������ú�����ΪTrue)
	int				m_nObstrucFlag;         // �ڵ���־(�����ڵ�������ΪTrue)

	float           m_fDisPreTarger;        // ����λ�õ�����λ�õĳ���
	float           m_fFlexOffset;          // ��������ĵĳ��������˵ĳ���
	float			m_fStartVelocity;		// ��ʼ�ٶ�

	DWORD           m_dwState;              // ��ǰ��״̬, �鿴 ENUM_CAMERA_STATE

	float           m_fZNearSave;           

	//
	D3DXVECTOR3     m_vecPositionSave;               
	D3DXVECTOR3     m_vecAxesPosSave;           
	//

	TrackCameraInfo m_trackInfo;            


	KG3DCameraSphere            m_BoundSphere;
private :
    D3DXVECTOR3		m_vecTagDirectn;

	float			m_fDistance;
	float			m_fAngleHor;
	float			m_fAngleVel;
	float			m_fAnVarHor;
	float			m_fAnVarVel;

	float			m_fUpOffset;
    float           m_fVelGlide;

	float			m_fAnglVelocity;
    float           m_nFlexLength;  

	int				m_nRegressFlag;
	int				m_nObTerrainFg;

public:
	struct _Station
	{
        KG3DTrackCamera* pCamera;

        float       fFlexCoefficient;   // ����ϵ��  
        float       fDampCoefficient;   // ����ϵ��

        int			bObstructdAvert;	// �Ƿ��ڵ���⡡
        int         bUseFlexibilitySys; // ʹ�õ���ϵͳ

		int			nChaseType;		    // ׷β���� 0:�Ӳ� 1:���� 2:����
        float       fChaseRate;         // ׷β�ٶ�

		float		fMaxDistance;		// �����ǵ�������
		float		fMinDistance;		// �����ǵ��������

		float		fMaxAngelVel;		// ��ֵ��ת���Ƕ�
		float		fMinAngelVel;		// ��ֵ��ת��С�Ƕ�

		float		fAngelRateHor;		// ˮƽ��ת���ݶ�
		float		fAngelRateVel;		// ��ֵ��ת���ݶ�

		float		fDisZoomRate;		// �������ݶ�
        float       fFlexRate;          // 
       
		_Station();
		_Station& operator=(_Station& temp);

		int IniByConfigFile(const char pcszFileName[]);
		int SaveToConfigFile(const char pcszFileName[]);
	};

	//(by dengzhihui Refactor 2006��11��6��
	class CStation : public IEKG3DCameraStation
	{
	public:
		void Init(_Station* pStation){m_station = pStation;}
	private:
		_Station* m_station;

		virtual HRESULT IniByConfigFile(LPCTSTR pcszFileName);
		virtual HRESULT SaveToConfigFile(LPCTSTR pcszFileName);

		virtual HRESULT GetChaseType(int* pChaseType);
		virtual HRESULT SetChaseType(int nChaseType);
		virtual HRESULT GetChaseRate(float* pChaseRate);
		virtual HRESULT SetChaseRate(float nChaseRate);
		virtual HRESULT GetObstructdAvert(int* pObstructdAvert);
		virtual HRESULT SetObstructdAvert(int nObstructdAvert);
		virtual HRESULT GetFlexCoefficient(float* FlexCoefficient);
		virtual HRESULT SetFlexCoefficient(float FlexCoefficient);
		virtual HRESULT GetDampCoefficient(float* DampCoefficient);
		virtual HRESULT SetDampCoefficient(float DampCoefficient);
		virtual HRESULT GetMaxAngelVel(float* MaxAngelVel);
		virtual HRESULT SetMaxAngelVel(float MaxAngelVel);
		virtual HRESULT GetMinAngelVel(float* MinAngelVel);
		virtual HRESULT SetMinAngelVel(float MinAngelVel);
		virtual HRESULT GetAngelRateHor(float* AngelRateHor);
		virtual HRESULT SetAngelRateHor(float AngelRateHor);
		virtual HRESULT GetAngelRateVel(float* AngelRateVel);
		virtual HRESULT SetAngelRateVel(float AngelRateVel);
		virtual HRESULT GetMaxDistance(float* MaxDistance);
		virtual HRESULT SetMaxDistance(float MaxDistance);
		virtual HRESULT GetDisZoomRate(float *fRate);
		virtual HRESULT SetDisZoomRate(float fRate);
	};
private:
	_Station m_cameraStation;
	CStation m_StationHelper;


	// Inherited via KG3DCamera
	virtual HRESULT GetUp(D3DXVECTOR3 * pUp) override;

	virtual HRESULT EnableHighPrecise(BOOL bEnable) override;

};

#endif //_INCLUDE_KG3DTRACKCAMERA_H_
