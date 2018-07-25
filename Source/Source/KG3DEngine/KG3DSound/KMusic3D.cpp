//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMusic3D.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Wave Stream Music Class
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "KDSound.h"
#include "KMusic3D.h"

//---------------------------------------------------------------------------
// ����:	KMusic3D
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMusic3D::KMusic3D()
{
	m_bEnbale3DSound = TRUE;
}
//---------------------------------------------------------------------------
// ����:	~KMusic3D
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMusic3D::~KMusic3D()
{
	Close();
}

//---------------------------------------------------------------------------
// ����:	InitSoundBuffer
// ����:	��������Buffer
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KMusic3D::InitSoundBuffer()
{
	DSBUFFERDESC		dsbdesc;
	HRESULT				hres;
	LPDIRECTSOUND8		pDirectSound;

	// get direct sound
	pDirectSound = g_pDirectSound->GetDirectSound();
	if (pDirectSound == NULL)
		return FALSE;

	// close any open file and release interfaces
	if (m_pSoundBuffer)
		FreeSoundBuffer();

	// set streaming sound buffer desc
	memset(&dsbdesc,0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_CTRL3D //3D��Ч
		| DSBCAPS_CTRLPOSITIONNOTIFY // ����λ��֪ͨ
		| DSBCAPS_GETCURRENTPOSITION2 // ����ȡ�õ�ǰλ��
		| DSBCAPS_CTRLVOLUME // ʹ�������Կ���
		| DSBCAPS_LOCSOFTWARE // ʹ���������
		| DSBCAPS_STICKYFOCUS; // �����ں�̨����

	// The size of the buffer is arbitrary, but should be at least
	// two seconds, to keep data writes well ahead of the play position.
	dsbdesc.dwBufferBytes = m_dwBufferSize;
	dsbdesc.lpwfxFormat = &m_WaveFormat;

	if (m_WaveFormat.nChannels != 1)
	{
		KGLogPrintf(KGLOG_ERR, "%s", "SoundShell\tKMusic3D : 3d sound only support one channel wave!");
		return FALSE;
	}

	// create streaming sound buffer
	hres = pDirectSound->CreateSoundBuffer(&dsbdesc, &m_pSoundBuffer, NULL);
	if (hres != DS_OK)
	{
		KGLogPrintf(KGLOG_ERR, "%s", "SoundShell\tKWavMusic : Create sound buffer failed");
		return FALSE;
	}

	//ͨ������������������ȡ3D buffer��ָ�롣
	hres = m_pSoundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (VOID**)&m_p3DBuffer);
	if (hres != DS_OK)
	{
		KGLogPrintf(KGLOG_ERR, "%s", "SoundShell\tKWavMusic : get 3d sound buffer failed");
		return FALSE;
	}

	m_dsBufferParams.dwSize = sizeof(DS3DBUFFER);
	hres = m_p3DBuffer->GetAllParameters(&m_dsBufferParams);
	if (hres != DS_OK)
	{
		KGLogPrintf(KGLOG_ERR, "%s", "SoundShell\tKWavMusic : get 3d parameters failed");
		return FALSE;
	}

	if (!m_bEnbale3DSound)
	{
		m_dsBufferParams.dwMode = DS3DMODE_DISABLE;
		hres = m_p3DBuffer->SetAllParameters(&m_dsBufferParams, DS3D_IMMEDIATE);
		if (hres != DS_OK)
		{
			KGLogPrintf(KGLOG_ERR, "%s", "SoundShell\tKWavMusic : set 3d parameters failed");
			return FALSE;
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	FreeSoundBuffer
// ����:	�ͷ�����Buffer
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMusic3D::FreeSoundBuffer()
{
	if (g_pDirectSound->GetDirectSound() == NULL)
		return;

	KG_COM_RELEASE(m_p3DBuffer);
	KMusic::FreeSoundBuffer();
}

HRESULT	KMusic3D::SetPosition(D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity)
{
	if (pvPosition)
	{
		memcpy(&m_dsBufferParams.vPosition, pvPosition, sizeof(D3DVECTOR));
		if (m_p3DBuffer)
			m_p3DBuffer->SetPosition(pvPosition->x, pvPosition->y, pvPosition->z, DS3D_IMMEDIATE);
	}
	if (pvVelocity)
	{
		memcpy(&m_dsBufferParams.vVelocity, pvVelocity, sizeof(D3DVECTOR));
		if (m_p3DBuffer)
			m_p3DBuffer->SetVelocity(pvVelocity->x, pvVelocity->y, pvVelocity->z, DS3D_IMMEDIATE);
	}

	return S_OK;
}

HRESULT	KMusic3D::SetParameters(FLOAT fMinDistance, FLOAT fMaxDistance)
{
	m_dsBufferParams.flMinDistance = fMinDistance;
	m_dsBufferParams.flMaxDistance = fMaxDistance;
	if (m_p3DBuffer)
		m_p3DBuffer->SetAllParameters(&m_dsBufferParams, DS3D_IMMEDIATE);
	return S_OK;
}

void KMusic3D::Enable3DSound(BOOL bEnbale)
{
	if (m_p3DBuffer)
	{
		if (bEnbale)
		{
			if (!m_bEnbale3DSound)
			{
				m_dsBufferParams.dwMode = DS3DMODE_NORMAL;
				m_p3DBuffer->SetAllParameters(&m_dsBufferParams, DS3D_IMMEDIATE);
			}
		}
		else
		{
			if (m_bEnbale3DSound)
			{
				m_dsBufferParams.dwMode = DS3DMODE_DISABLE;
				m_p3DBuffer->SetAllParameters(&m_dsBufferParams, DS3D_IMMEDIATE);
			}
		}

	}
	m_bEnbale3DSound = bEnbale;
}

