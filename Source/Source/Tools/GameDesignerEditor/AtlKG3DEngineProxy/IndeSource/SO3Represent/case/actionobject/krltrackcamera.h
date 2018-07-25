#ifndef CASE_ACTIONOBJECT_KRLTRACKCAMERA_H
#define CASE_ACTIONOBJECT_KRLTRACKCAMERA_H

struct KRLCameraParam;

class KRLTrackCamera
{
public:
    KRLTrackCamera();
    ~KRLTrackCamera();

    HRESULT UpdateCamera(D3DXVECTOR3 vPosition, D3DXVECTOR3 vFront, float fFlexOffset, float fLookAtOffset);

    HRESULT FrameMove(KRLCameraParam* pResultParam, float fDelayTime);


    // �����߼�⣬����н��㣬������Ŀ�������Ľ���
    int GetCollisionPosition(
        const D3DXVECTOR3& crvCameraPos,
        const D3DXVECTOR3& crvTargetPos,
        IKG3DCamera* p3DCamera,
        D3DXVECTOR3* pvCrossPos
    );

    void SetFlexOffset(float fFlexOffset);

    void SetCollisionFlag(int nFlag);

private:
    void DetectObstruct(const D3DXVECTOR3& crvPreCameraPos);

private:
    D3DXVECTOR3		m_vPosition;
    D3DXVECTOR3		m_vExpectPosition;

    D3DXVECTOR3		m_vLookAt;
    D3DXVECTOR3		m_vExpectLookAt;

    D3DXVECTOR3     m_vCurrentVelocity;     // ��ǰ�������˶����ٶ�

    float           m_fFlexOffset;          // ��������ĵĳ���ͨ����껬�������˵ĳ���

    float           m_fSpringK;             // ����ϵ��  
    float           m_fDampingK;            // ����ϵ��  

    int             m_nUseFlexFlag;         // �Ƿ�Ҫ�������˶�(�ڳ����ڵ��󣬴˱�־��ΪTrue)
    int             m_nUpdateFlag;          // ��Ҫ�������λ�ñ�־(UpdateCamera���������ú�����ΪTrue)
    int				m_nObstrucFlag;         // �ڵ���־(�����ڵ�������ΪTrue)
    int             m_nCollisionFlag;       // ��ͷ���������ϰ��﷢����ײ
};

#endif // CASE_ACTIONOBJECT_KRLTRACKCAMERA_H