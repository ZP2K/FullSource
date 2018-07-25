// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


//#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include "Engine.h"
#include "Engine/KGLog.h"
#include "Engine/KG_Time.h"
#include <windows.h>
#include <dsound.h>
#include <math.h>

//��0.0-1.0����������ת��Ϊdsoud�õ�����
inline long LinearToLogVol(float fLevel)
{
	// Clamp the value

	if(fLevel <= FLT_MIN)
		return DSBVOLUME_MIN;
	if(fLevel >= 1.0f)
		return DSBVOLUME_MAX;

	long lResult = (long)(-2000.0 * log10(1.0f / (double)fLevel));
	if (lResult < DSBVOLUME_MIN)
		lResult = DSBVOLUME_MIN;

	return lResult;
}

//��dsoud�õ�����ת��Ϊ0.0-1.0����������
inline float LogToLinearVol(long lLevel)
{
	// Clamp the value
	if(lLevel <= DSBVOLUME_MIN + 400)
		return 0.0f;
	else if(lLevel >= DSBVOLUME_MAX)
		return 1.0f;
	float fResult = (float)(pow(10, double(lLevel + 2000) / 2000.0f) / 10.0f);
	if (fResult < 0.0f)
		fResult = 0.0f;
	if (fResult > 1.0f)
		fResult = 1.0f;
	return fResult;
}

//��0.0-1.0����������ת��Ϊ�ֱ�
inline long VolumeToDecibels(float vol) 
{
	if (vol>=1.0F) 
		return DSBVOLUME_MAX;
	if (vol<=0.0F) 
		return DSBVOLUME_MIN;
	long lResult = long(float(log10(vol)) * 3321.928094887F);
	if (lResult < DSBVOLUME_MIN)
		lResult = DSBVOLUME_MIN;
	return lResult;
}

#define NUM_PLAY_NOTIFICATIONS 2//NOtification�ĸ���
#define BUFFER_LENGTH 4//Buffer���ų���3��