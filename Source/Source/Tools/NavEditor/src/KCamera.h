#pragma once

struct KCamera
{
    KCamera(void);
    ~KCamera(void);

    void FrameUpdate();

    void OnMouseWheel(int nDelta);
    void OnPitchingGrag(int nDelta); // ������ק
    void OnRotatingGrag(int nDelta); // ��ת��ק

    D3DXVECTOR3  m_vUp;
    D3DXVECTOR3  m_vRight;
    D3DXVECTOR3  m_vLook;
};

extern KCamera g_Camera;
