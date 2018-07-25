////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGPerformerce.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-12-16 10:23:09
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGPERFORMERCE_H_
#define _INCLUDE_KGPERFORMERCE_H_

////////////////////////////////////////////////////////////////////////////////
#include <vector>

struct KGPerformerceSystemInfo
{
	double	frequency;	//����Ӳ����������0,��λper MiliSecond
	
	KGPerformerceSystemInfo();
	
	static BOOL QueryFreq(LARGE_INTEGER& freq);	//��λ��per ��
	static BOOL QueryCounter(LARGE_INTEGER& counter);	//��ͬ��������ͬ��������SetThreadAffinityMask����Ȼ�ò������������ģ�����BIOS�ģ�BIOSҲ���ܳ���
};

class KGPerformercenRecorder
{
public:
	KGPerformercenRecorder();
	~KGPerformercenRecorder();

	VOID Enable(BOOL enable);
	VOID Start();
	VOID Sample();
	VOID Stop();
	VOID PrintToOut();
private:
	std::vector<LARGE_INTEGER>	m_samplers;
	LARGE_INTEGER	m_startCounter;
	KGPerformerceSystemInfo	m_systemInfo;
	SHORT m_bIsEnable;
	SHORT m_bStarted;
};

#if defined(DEBUG) | defined(_DEBUG)
#define KG_PERF_START(bEnable)	KGPerformercenRecorder perfRecorder; {perfRecorder.Start(); perfRecorder.Enable(bEnable);}NULL
#define KG_PERF_SAMPLE() {perfRecorder.Sample();} NULL
#define KG_PERF_STOP()	{perfRecorder.Stop(); perfRecorder.PrintToOut();} NULL
#else
#define KG_PERF_START(bEnable)
#define KG_PERF_SAMPLE()
#define KG_PERF_STOP()
#endif





#endif //_INCLUDE_KGPERFORMERCE_H_
