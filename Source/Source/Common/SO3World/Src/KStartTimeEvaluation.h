#ifndef KSTARTTIMEEVALUATION_H
#define KSTARTTIMEEVALUATION_H

#if defined(_CLIENT)

// ��ʼ֡ʱ���������

struct KStartTimeEvaluation
{
    DWORD dwFirstStartTime;
    DWORD dwEvaluationCount;
    double lfFirstStartTime;
    double lfNetStartTime;
};

int InitStartTimeEvaluation(KStartTimeEvaluation* pStartTimeEvaluation);
int UpdateStartTimeEvaluation(KStartTimeEvaluation* pStartTimeEvaluation, DWORD dwTimeNow);

#endif	//_CLIENT

#endif // KSTARTTIMEEVALUATION_H
