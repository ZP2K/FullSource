//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMp3Music.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	MP3 Stream Music Class
//---------------------------------------------------------------------------
#include "stdafx.h" 
#include "KMp3Music.h"

//---------------------------------------------------------------------------
#define MP3_BUFSIZE 60000 // mp3 data buffer
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	Open
// ����:	�������ļ�
// ����:	FileName	�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------

KMp3Music::KMp3Music()
{
	m_mp3buffer=NULL;
	m_mp3bufptr=NULL;
	m_pMp3File=NULL;
	m_mp3bufbytes=0;
	m_mp3frmbytes=0;
	m_szMp3FilePath[0] = '\0';
}

BOOL KMp3Music::Open(LPSTR FileName)
{
	if (!FileName || !FileName[0])
		return FALSE;

	if (m_mp3buffer || m_pMp3File)
		Close();

	if (!Mp3FileOpen(FileName))
		return FALSE;

	if (!Mp3Init())
		return FALSE;

	if (!Init())
		return FALSE;

	return TRUE;
}

void KMp3Music::Close()
{
	KMusic::Close();
	Clear();
}

//---------------------------------------------------------------------------
// ����:	InitSoundFormat
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp3Music::InitSoundFormat()
{
	// align buffer size with mp3 frame 4608
	m_dwBufferSize = m_WaveFormat.nAvgBytesPerSec;
	m_dwBufferSize = (m_dwBufferSize + 4607) / 4608 * 4608;

	// 2 second sound buffer
	m_dwBufferSize *= 2;
}
//---------------------------------------------------------------------------
// ����:	ReadWaveData
// ����:	��ȡ��������
// ����:	lpBuf	����
//			dwLen	����	
// ����:	DWORD	ʵ�ʶ�ȡ�ĳ���
//---------------------------------------------------------------------------
DWORD KMp3Music::ReadWaveData(LPBYTE lpBuf, DWORD dwLen)
{
	return Mp3Decode(lpBuf, dwLen);
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp3Music::Seek(int nPercent)
{
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	��ͷ��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMp3Music::Rewind()
{
	//---07.8.29---Vonbo---
	Mp3FileSeekDataBegin();
	//----------end of 8.29
	m_mp3bufbytes = 0;
}
//---------------------------------------------------------------------------
// ����:	Mp3FileOpen
// ����:	
// ����:	FileName
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KMp3Music::Mp3FileOpen(LPSTR FileName)
{
    ASSERT(m_pMp3File == NULL);

	m_pMp3File = g_OpenFile(FileName, false, false);

	if (m_pMp3File==NULL)
	{
		m_szMp3FilePath[0] = '\0';
		return FALSE;
	}
	else
	{
		strcpy(m_szMp3FilePath, FileName);
		return TRUE;
	}
}
//---------------------------------------------------------------------------
// ����:	Mp3FileRead
// ����:	
// ����:	pBuf
//			dwlen
// ����:	void
//---------------------------------------------------------------------------
DWORD KMp3Music::Mp3FileRead(PBYTE pBuf, DWORD dwLen)
{
	return m_pMp3File->Read(pBuf, dwLen);
}
//---------------------------------------------------------------------------
// ����:	Mp3FileSeek
// ����:	
// ����:	lOffset
// ����:	void
//---------------------------------------------------------------------------
DWORD KMp3Music::Mp3FileSeek(LONG lOffset, int nPosition /*= SEEK_SET*/ )
{
	return m_pMp3File->Seek(lOffset, nPosition);
}

//---------------------------------------------------------------------------
// ����:	Mp3FileSeekDataBegin()
// ����:	��Mp3�ļ������ݶ�λ����Ƶ��ʼ��(������ʼ��ID3��ǩ)
// ����:	lOffset
// ����:	void
//---------------------------------------------------------------------------
void KMp3Music::Mp3FileSeekDataBegin()
{	
	LONG longDataPos = 0;
	int nOffSet = SEEK_SET;
	char szHeader[3];

	Mp3FileSeek(0);
	Mp3FileRead((PBYTE)szHeader, 3);	// Read the First Byte
	if (szHeader[0] == 'I' && szHeader[1] == 'D' && szHeader[2] == '3') //ID3 v2
	{
		BYTE bySize[4] = {0};
		Mp3FileSeek(6);
		Mp3FileRead((PBYTE)bySize, 4);	// Read the tag header
		longDataPos = (bySize[0] & 0x7F)<<21 | (bySize[1] & 0x7F)<<14 
			| (bySize[2] & 0x7F)<<7 | (bySize[3] & 0x7F);
		nOffSet = SEEK_CUR;
	}
	Mp3FileSeek(longDataPos, nOffSet);
}
//---------------------------------------------------------------------------
// ����:	Mp3Init
// ����:	��ʼ��MP3�ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KMp3Music::Mp3Init()
{
	MPEG_HEAD mpeg_head;
	DEC_INFO  dec_info;
	
	// load mp3lib.dll
	//if (!LoadMp3Lib())
	//	return FALSE;

	// reduction_code:	sample rate reduction code
	//		0 = full rate
	//		1 = half rate
	//		2 = quarter rate
	int reduction_code = 0;
	
	// transform_code:	ignored
	int transform_code = 0;

	// convert_code:	channel conversion
	//		0 = two chan output
	//		1 = convert two chan to mono
	//		2 = convert two chan to left chan
	//		3 = convert two chan to right chan
	int convert_code = 0;
	
	// freq_limit:	limits bandwidth of pcm output to specified
	//		frequency. Special use. Set to 24000 for normal use.
	int freq_limit = 24000;

	// alloc mp3 stream buffer
//	if (!m_Mp3Buffer.Alloc(MP3_BUFSIZE))
//		return FALSE;
//	m_mp3buffer = (PBYTE)m_Mp3Buffer.GetMemPtr();

	if (m_mp3buffer)
	{
		free(m_mp3buffer);
		m_mp3buffer=NULL;
	}
	m_mp3buffer=(PBYTE) malloc(MP3_BUFSIZE*sizeof(BYTE));
	if (m_mp3buffer==NULL)
	{
		KGLogPrintf(KGLOG_ERR, "%s", "SoundShell\tKMP3Music::m_mp3buffer Fail To Alloc Memory");
		return FALSE;
	}

	// init mp3 buffer
	m_mp3bufptr = m_mp3buffer;

	// seek to begin of mp3 file
	//---07.8.29---Add By Vonbo---
	Mp3FileSeekDataBegin();
	//--------------end of 07.8.29

	// fill mp3 buffer
	if (!Mp3FillBuffer())
	{
		free(m_mp3buffer);
		m_mp3buffer=NULL;
		return FALSE;
	}

	// parse mpeg header
	m_mp3frmbytes = m_Decoder.DecodeHead(m_mp3buffer, &mpeg_head);
	if (m_mp3frmbytes == 0)
	{
		KGLogPrintf(KGLOG_ERR, "%s : %s", "SoundShell\tKMp3Music::mp3 decode head fail", m_szMp3FilePath);
		free(m_mp3buffer);
		m_mp3buffer=NULL;
		return FALSE;
	}

	// init mp3 decoder
	if (!m_Decoder.Init(&mpeg_head, m_mp3frmbytes,
		reduction_code, transform_code, convert_code, freq_limit))
	{
		KGLogPrintf(KGLOG_ERR, "%s : %s", "SoundShell\tKMp3Music::mp3 decode init fail", m_szMp3FilePath);
		free(m_mp3buffer);
		m_mp3buffer=NULL;
		return FALSE;
	}

	// get mp3 info
	m_Decoder.DecodeInfo(&dec_info);

	// set pcm wave format
	m_WaveFormat.wFormatTag		 = WAVE_FORMAT_PCM;
	m_WaveFormat.nChannels		 = dec_info.channels;
	m_WaveFormat.nSamplesPerSec	 = dec_info.samprate;
	m_WaveFormat.nAvgBytesPerSec = (dec_info.channels * dec_info.samprate * dec_info.bits + 7) / 8;
	m_WaveFormat.nBlockAlign	 = (dec_info.channels * dec_info.bits + 7) / 8;
	m_WaveFormat.wBitsPerSample	 = dec_info.bits;
	m_WaveFormat.cbSize			 = 0;

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Mp3Decode
// ����:	����MP3
// ����:	lpPcmBuf		PCM buffer pointer
//			dwBufLen		buffer size in bytes
// ����:	void
//---------------------------------------------------------------------------
DWORD KMp3Music::Mp3Decode(PBYTE lpPcmBuf, DWORD dwBufLen)
{
	PBYTE	pcm_buffer = lpPcmBuf;
	int		pcm_trigger = dwBufLen;
	int		pcm_bufbytes = 0;
	int		mp3_size = 0;
	int		pcm_size = 0;
	IN_OUT	res = {0, 0};
		
	while (TRUE)
	{
		// fill mp3 buffer
		if (!Mp3FillBuffer())
			break;
		
		// end of mp3 file
		if (m_mp3bufbytes < m_mp3frmbytes)
			break;

		// decode one frame to pcm buffer
		res = m_Decoder.DecodeFrame(m_mp3bufptr, pcm_buffer);
		if (res.in_bytes <= 0)
		{
			g_DebugLog("KMp3Music: bad sync in mp3 file");
			break;
		}

		// update buffer pointer and buffer size
		m_mp3bufptr += res.in_bytes;
		m_mp3bufbytes -= res.in_bytes;
		pcm_buffer += res.out_bytes;
		pcm_bufbytes += res.out_bytes;
		
		// is pcm buffer full
		if (pcm_bufbytes >= pcm_trigger)
			break;
	}
	return pcm_bufbytes;
}
//---------------------------------------------------------------------------
// ����:	Mp3FillBuffer
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KMp3Music::Mp3FillBuffer()
{
	static int mp3_trigger = 2500;
	DWORD dwRead;
	
	// signed var could be negative
	if (m_mp3bufbytes < 0)
		m_mp3bufbytes = 0;
	
	// read mp3 data to buffer
	if (m_mp3bufbytes < mp3_trigger)
	{
		memmove(m_mp3buffer, m_mp3bufptr, m_mp3bufbytes);
		dwRead = Mp3FileRead(m_mp3buffer + m_mp3bufbytes, 
			MP3_BUFSIZE - m_mp3bufbytes);
		m_mp3bufbytes += dwRead;
		m_mp3bufptr = m_mp3buffer;
	}
	
	return TRUE;
}
//---------------------------------------------------------------------------
KMp3Music::~KMp3Music()
{
	Close();
}

void KMp3Music::Clear()
{
	if (m_mp3buffer!=NULL)
	{
		free(m_mp3buffer);
		m_mp3buffer=NULL;
	}
	m_mp3bufptr = NULL;
	m_mp3bufbytes = 0;
	m_mp3frmbytes = 0;
	if (m_pMp3File)
	{
		m_pMp3File->Release();
		m_pMp3File = NULL;
	}
	m_szMp3FilePath[0] = '\0';
}

void KMp3Music::Stop()
{
	KMusic::Stop();
//	m_mp3bufbytes=0;
//	m_mp3frmbytes=0;
}