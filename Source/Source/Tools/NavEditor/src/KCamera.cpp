#define _WIN32_WINNT 0x0500 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <Windowsx.h>
#include <zmouse.h>
#include <WinUser.h>
#include "KApplication.h"
#include "KScene.h"
#include "KCamera.h"

KCamera g_Camera;

KCamera::KCamera(void)
{
    m_vUp       = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_vRight    = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    m_vLook     = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

KCamera::~KCamera(void)
{
}

void KCamera::FrameUpdate()
{

}

void KCamera::OnMouseWheel(int nDelta)
{
    float fValue = 0.0f;

    if (nDelta < 0)
    {
        fValue = 0.1f;
    }
    else
    {
        fValue = -0.1f;
    }

    m_vLook *= (1.0f + fValue);
}

// ����
void KCamera::OnPitchingGrag(int nDelta)
{
    float fAngle = 0.0f;

    if (nDelta > 0)
    {
        // ̧�߾�ͷ,��߿���̧�����ӽǶ�
        fAngle = D3DX_PI / 200;
    }
    else
    {
        // ѹ�;�ͷ,�������ѹ��ˮƽ�Ͳ���������
        fAngle = -D3DX_PI / 200;
    }

    // ��һ���̶ȾͲ�Ҫ��������
    if (m_vUp.y <= 0.0f && fAngle < 0.0f)
        return;

        // ��һ���̶ȾͲ�Ҫ��������
    if (m_vLook.y >= 0.0f && fAngle < 0.0f)
        return;

    D3DXMATRIX Matrix;

    D3DXMatrixRotationAxis(&Matrix, &m_vRight, fAngle);

    D3DXVec3TransformCoord(&m_vUp, &m_vUp, &Matrix);
    D3DXVec3TransformCoord(&m_vLook, &m_vLook, &Matrix);
    
    D3DXVec3Normalize(&m_vUp, &m_vUp);
}

// ��ת
void KCamera::OnRotatingGrag(int nDelta)
{
    float fAngle = 0.0f;

    if (nDelta > 0)
    {
        fAngle = D3DX_PI / 120;
    }
    else
    {
        fAngle = -D3DX_PI / 120;
    }

    D3DXVECTOR3 vVertical(0.0f, 1.0f, 0.0f);
    D3DXMATRIX  Matrix;

    D3DXMatrixRotationAxis(&Matrix, &vVertical, fAngle);

    D3DXVec3TransformCoord(&m_vUp, &m_vUp, &Matrix);
    D3DXVec3TransformCoord(&m_vLook, &m_vLook, &Matrix);
    D3DXVec3TransformCoord(&m_vRight, &m_vRight, &Matrix);

    D3DXVec3Normalize(&m_vUp, &m_vUp);
    D3DXVec3Normalize(&m_vRight, &m_vRight);
}
