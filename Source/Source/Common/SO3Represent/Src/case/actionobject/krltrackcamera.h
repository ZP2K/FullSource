#ifndef CASE_ACTIONOBJECT_KRLTRACKCAMERA_H
#define CASE_ACTIONOBJECT_KRLTRACKCAMERA_H

struct KRLCameraParam;
class IKG3DCamera;

struct KRLSpringParam
{
    float fDistance;
    float fTime;
    float fVelocity;
    float fEpsilon;
};

struct KRLSpringResult
{
    int nFinished;
    float fDistance;
    float fVelocity;
};

class KRLTrackCamera
{
public:
    KRLTrackCamera();
    ~KRLTrackCamera();

    HRESULT UpdateCamera(D3DXVECTOR3 vTarget, D3DXVECTOR3 vPosition, D3DXVECTOR3 vFront, float fWheelOffset, float fLookAtOffset);

    HRESULT FrameMove(KRLCameraParam* pResultParam, float fDelayTime);


    // �����߼�⣬����н��㣬������Ŀ�������Ľ���
    int GetCollisionPosition(
        const D3DXVECTOR3& crvCameraPos,
        const D3DXVECTOR3& crvTargetPos,
        IKG3DCamera* p3DCamera,
        D3DXVECTOR3* pvCrossPos
    );

    void SetCollisionFlag(int nFlag);

private:
    void DetectObstruct(float fExpectDistance);
    void StopSpringSystem(float fDistance);

private:
    D3DXVECTOR3 m_vTarget;
    D3DXVECTOR3 m_vExpectTarget;

    D3DXVECTOR3 m_vPosition;
    D3DXVECTOR3 m_vExpectFront;
    float m_fExpectDistance;

    D3DXVECTOR3 m_vLookAt;

    float m_fDistance;

    float m_fCurrentVelocity;     // ��ǰ�������˶����ٶ�

    float m_fWheelOffset;          // ��������ĵĳ���ͨ����껬�������˵ĳ���

    int m_nUseFlexFlag;         // �Ƿ�Ҫ�������˶�(�ڳ����ڵ��󣬴˱�־��ΪTrue)
    int m_nUpdateFlag;          // ��Ҫ�������λ�ñ�־(UpdateCamera���������ú�����ΪTrue)
/*    int m_nObstrucFlag;         // �ڵ���־(�����ڵ�������ΪTrue)*/
    int m_nCollisionFlag;       // ��ͷ���������ϰ��﷢����ײ
};

int RunSpringSystem(KRLSpringParam RLSpringInParam, KRLSpringResult* pRLSpringOutParam);

#endif // CASE_ACTIONOBJECT_KRLTRACKCAMERA_H