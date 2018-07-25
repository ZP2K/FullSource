//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KSoundShell.h
// Date:	2004.07.21
// Code:	ShiShuaiWei(Canon)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef _KSOUNDSHELL_H
#define _KSOUNDSHELL_H
//---------------------------------------------------------------------------
#pragma once
#include "IKG3DSoundShell.h"
#include "KMp3Music.h"
#include "KWavSound.h"
#include "KDSound.h"
#include "KMp3Music3D.h"
#include "KWavSound3D.h"
#include "KRecordSound.h"
#include "d3dx9math.h"
//---------------------------------------------------------------------------
//## ����ģ��Ķ���ӿڵ�ʵ�֡�

#define KG_2D_MUSIC_COUNT 0
#define KG_2D_SOUND_COUNT 8
#define KG_3D_MUSIC_COUNT 0
#define KG_3D_SOUND_COUNT 64
#define KG_MAX_SINGLE_2D_SOUND_COUNT 3
#define KG_MAX_SINGLE_3D_SOUND_COUNT 6

class KSoundShell :	public IKG3DSoundShell
{
public:
	virtual ULONG AddRef();
	//���ٽӿڶ���
	virtual ULONG Release();
	//��ʼ����Ч����
	virtual HRESULT	Init(HWND hWnd);

	virtual HRESULT Active();

	//�����߽ӿ�
	virtual HRESULT	SetListenerPosition(D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity);
	virtual HRESULT	GetListenerPosition(D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity);
	virtual HRESULT	SetOrientation(D3DVECTOR* pvFront, D3DVECTOR* pvTop);
	virtual HRESULT	SetListenerParameters(FLOAT fDopplerFactor, FLOAT fRolloffFactor);

	virtual HRESULT	SetVolume(float fVolume); //������������ 0 - 1
	virtual float	GetVolume(); //ȡ����������
	virtual HRESULT	SetVolume(int nType, float fVolume); //������һ���͵����ֵ����� 0 - 1
	virtual float	GetVolume(int nType); //ȡ����һ���͵�����

	//----------------����������ڶ�ʧ����֮�����һص�ʱ����������ȷ�����⡣
	virtual HRESULT	Lock(); //���������ʱ��ֹͣ��������
	virtual HRESULT	Unlock(); //������������ʱ��Enable������Ƿ񲥷�
	virtual BOOL	IsLocked(); //�Ƿ�����

	virtual HRESULT	Enable(BOOL bEnable); //����������
	virtual BOOL	IsEnable();
	virtual HRESULT	Enable(int nType, BOOL bEnable); //��������һ��������
	virtual BOOL	IsEnable(int nType);

	virtual HRESULT	Enable3DSound(BOOL bEnable); //����3D��ʽ������Ч
	virtual BOOL	IsEnable3DSound();

	//----------2D���ֽӿ�-----------------
	//���ű������֣�ͬʱֻ�ܲ���һ���������֣�mp3��˫ͨ��
	virtual HRESULT PlayBgMusic(int nType, LPCSTR FileName, INT nPan, float fVolume);
	virtual HRESULT StopBgMusic(BOOL bimmediately);
	virtual HRESULT SetBgMusicVolume(float fVolume);
	virtual BOOL	IsPlayingBgMusic();
	virtual BOOL	IsPlayingBgMusic(LPCSTR FileName);
	virtual BOOL	IsBgMusicLoop();
	virtual HRESULT	SetBgMusicLoop(BOOL bLoop);

	//�����������֣������������Ķ԰׵ȣ�����ͬʱ���ڶ����mp3��˫ͨ��
	virtual HRESULT PlayMusic(int nType, LPCSTR FileName, INT nPan, float fVolume, BOOL bLoop, DWORD *pdwID);
	virtual HRESULT StopMusic(DWORD dwID);
	virtual HRESULT SetMusicVolume(DWORD dwID, float fVolume);
	virtual BOOL	IsPlayingMusic(DWORD dwID);
	virtual BOOL	IsPlayingMusic(LPCSTR FileName);
	virtual HRESULT StopAllMusic();

	//������Ч�����������ʾ��������ͬʱ���ڶ����wav,˫ͨ��
	virtual HRESULT PlaySound(int nType, LPCSTR FileName, INT nPan, float fVolume, BOOL bLoop, DWORD *pdwID);
	virtual HRESULT StopSound(DWORD dwID);
	virtual HRESULT StopSoundAfterThisTime(DWORD dwID);
	virtual HRESULT SetSoundVolume(DWORD dwID, float fVolume);
	virtual BOOL	IsPlayingSound(DWORD dwID);
	virtual BOOL	IsPlayingSound(LPCSTR FileName);
	virtual HRESULT StopAllSound();


	//----------3D���ֽӿ�-----------------
	//�����������֣���������Ի�������ͬʱ���ڶ����mp3����ͨ��
	virtual HRESULT Play3DMusic(int nType, LPCSTR FileName, float fVolume, BOOL bLoop, D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity, FLOAT fMinDistance, FLOAT fMaxDistance, DWORD *pdwID);
	virtual HRESULT Stop3DMusic(DWORD dwID);
	virtual HRESULT Set3DMusicVolume(DWORD dwID, float fVolume);
	virtual BOOL	IsPlaying3DMusic(DWORD dwID);
	virtual BOOL	IsPlaying3DMusic(LPCSTR FileName);
	virtual HRESULT	Set3DMusicPosition(DWORD dwID, D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity);
	virtual HRESULT	Set3DMusicParameters(DWORD dwID, FLOAT fMinDistance, FLOAT fMaxDistance);
	virtual HRESULT StopAll3DMusic();

	//������Ч��������������������Ч����������ͬʱ���ڶ����wav����ͨ��
	virtual HRESULT Play3DSound(int nType, LPCSTR FileName, float fVolume, BOOL bLoop, D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity, FLOAT fMinDistance, FLOAT fMaxDistance, DWORD *pdwID);
	virtual HRESULT Stop3DSound(DWORD dwID);
	virtual HRESULT Stop3DSoundAfterThisTime(DWORD dwID);
	virtual HRESULT Set3DSoundVolume(DWORD dwID, float fVolume);
	virtual BOOL	IsPlaying3DSound(DWORD dwID);
	virtual BOOL	IsPlaying3DSound(LPCSTR FileName);
	virtual HRESULT	Set3DSoundPosition(DWORD dwID, D3DVECTOR* pvPosition, D3DVECTOR* pvVelocity);
	virtual HRESULT	Set3DSoundParameters(DWORD dwID, FLOAT fMinDistance, FLOAT fMaxDistance);
	virtual HRESULT StopAll3DSound();

	//¼���ӿ�
	virtual HRESULT StartRecordSound();
	virtual HRESULT StopRecordSound();
	virtual HRESULT LockRecordSoundBuffer(void **ppBuffer, DWORD *pSize);
	virtual HRESULT ClearRecordSoundBuffer();
	virtual HRESULT UnlockRecordSoundBuffer();
	virtual void * GetRecordSoundWaveFormatEx();

	//¼�ƽӿ����
	virtual HRESULT SetEventRecordFuc(RecordSoundEventFuc pFunc);

	KSoundShell();

protected:
	virtual ~KSoundShell();

private:
	HRESULT ClearBuffer();
	HRESULT InternalInit();
	HRESULT Clear();
	float GetFinalVolume(int nType, float fVolume);
	HRESULT Stop();
	HRESULT UpdatePlayState();

private:
	struct KMp3MusicInfo
	{
		string strFile;
		KMp3Music mp3Music;
		KMp3MusicInfo *pNext;
	};
	KMp3MusicInfo * m_pMusic;

	struct KWavSoundInfo
	{
		UINT uFileHash;
		string strFile;
		KWavSound wavSound;
		DWORD dwTime;
		KWavSoundInfo *pNext;
	};
	KWavSoundInfo * m_pSound;

	struct KMp3Music3DInfo
	{
		string strFile;
		KMp3Music3D mp3Music3D;
		KMp3Music3DInfo *pNext;
	};
	KMp3Music3DInfo * m_p3DMusic;

	struct KWavSound3DInfo
	{
		UINT uFileHash;
		string strFile;
		BOOL bUpdatePosition;
		//D3DXVECTOR3 vec3CurrentPosition;
		D3DXVECTOR3 vec3NextPosition;
		KWavSound3D wavSound3D;
		DWORD dwTime;
		KWavSound3DInfo *pNext;
	};
	KWavSound3DInfo * m_p3DSound;

	struct KSoundTypeInfo
	{
		float fVolume;
		BOOL bEnable;
		KSoundTypeInfo()
		{
			fVolume = 1.0f;
			bEnable = TRUE;
		}
	};
	typedef std::map<int, KSoundTypeInfo> KTypeToInfoMap;

	KMp3Music			m_BgMusic;			//���ڲ��ű�������
	char				m_szNextMusic[MAX_PATH];

	KDirectSound	m_DirectSound;  //���ڳ�ʼ��DirectSound

	KRecordSound	m_RecordSound;

	DWORD m_dwID; //��ǰID

	ULONG m_ulRefCount; //ʵ�������ü���
	BOOL m_bInit; //��û�б���ʼ����

	float m_fVolume;
	KTypeToInfoMap m_Volume;

	BOOL m_bEnable;
	BOOL m_bLoopBgMusic;

	BOOL m_bEnbale3DSound;

	BOOL m_bLock;
	D3DXVECTOR3 m_vec3ListenerPosition;
	BOOL m_bListenerUpdated;
	RecordSoundEventFuc m_pRecordEventFuc;
};
//---------------------------------------------------------------------------
#endif