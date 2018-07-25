// File: DateTimeSystem.h
// Desc:

#ifndef DATETIMESYSTEM_H
#define DATETIMESYSTEM_H

#include"Singleton.h"
#include"Date.h"
#include"Time.h"  // Ours

#include<time.h>  // System's
#include<windows.h>

namespace Common
{

class DLLAPI DateTimeSystem
{
private:
	bool m_bIsSupportHighResolutionCounter;
	__int64 m_Freq;  // ��ʱ��Ƶ��

private:
	DateTimeSystem(const DateTimeSystem& dts) {}
	DateTimeSystem& operator = (const DateTimeSystem& dts) { return *this; }

public:
	DateTimeSystem();
	~DateTimeSystem() {}
	
	void GetLocalDate(Date& d) const;
	void GetLocalTime(Time& t) const;
	void GetGMTDate(Date& d) const;
	void GetGMTTime(Time& t) const;

	unsigned int GetTimeValueMS() const;
	unsigned int GetTimeValueS() const;
	unsigned int GetTimeValueMS_LR() const { return GetTickCount(); }  // ���ص�ǰʱ��ֵ(����)���;���
	unsigned int GetTimeValueS_LR() const { return GetTickCount() / 1000; }  // ���ص�ǰʱ��ֵ(��)���;���
	unsigned int GetTimeValueMS_HR() const;  // ���ص�ǰʱ��ֵ(����)���߾���
	unsigned int GetTimeValueS_HR() const;  // ���ص�ǰʱ��ֵ(��)���߾���
};

inline unsigned int DateTimeSystem::GetTimeValueMS() const
{
	unsigned int ret = 0;

	if(m_bIsSupportHighResolutionCounter)
	{
		__int64 temp;
		QueryPerformanceCounter((LARGE_INTEGER*)&temp);

		ret = static_cast<unsigned int>(temp / m_Freq) * 1000 + static_cast<unsigned int>(temp % m_Freq) * 1000 / m_Freq;
	}
	else
	{
		ret = GetTickCount();
	}

	return ret;
}

inline unsigned int DateTimeSystem::GetTimeValueS() const
{
	unsigned int ret = 0;

	if(m_bIsSupportHighResolutionCounter)
	{
		__int64 temp;
		QueryPerformanceCounter((LARGE_INTEGER*)&temp);

		ret = static_cast<unsigned int>(temp / m_Freq) + static_cast<unsigned int>(temp % m_Freq) / m_Freq;
	}
	else
	{
		ret = GetTickCount() / 1000;
	}

	return ret;
}

inline unsigned int DateTimeSystem::GetTimeValueMS_HR() const  // ���ص�ǰʱ��ֵ(����)���߾���
{
	unsigned int ret;

	if(m_bIsSupportHighResolutionCounter)
	{
		__int64 temp;
		QueryPerformanceCounter((LARGE_INTEGER*)&temp);

		ret = static_cast<unsigned int>(temp / m_Freq) * 1000+ static_cast<unsigned int>(temp % m_Freq) * 1000 / m_Freq;
	}
	else
	{
		ret = GetTickCount();
	}

	return ret;
}

inline unsigned int DateTimeSystem::GetTimeValueS_HR() const  // ���ص�ǰʱ��ֵ(��)���߾���
{
	unsigned int ret;

	if(m_bIsSupportHighResolutionCounter)
	{
		__int64 temp;
		QueryPerformanceCounter((LARGE_INTEGER*)&temp);

		ret = static_cast<unsigned int>(temp / m_Freq) + static_cast<unsigned int>(temp % m_Freq) / m_Freq;
	}
	else
	{
		ret = GetTickCount() / 1000;
	}

	return ret;
}

}  // namespace

#endif