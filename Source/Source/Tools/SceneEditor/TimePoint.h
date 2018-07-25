#pragma once
#include <vector>
using namespace std;

namespace ModelSFX
{

//_Time_Point��Ϊ������Ŀ�������ʹ������ֵ��ֵ
struct TimePoint
{
	float fTime;
	float fLeftVal;
	float fRightVal;
};

class KTimePoint
{
public:
	//friend class KParticleSystem;
	KTimePoint();
	~KTimePoint();
	KTimePoint(float* fTP, int iTPCount = 1);
	KTimePoint(float fTime, float fLeftVal, float fRightVal);
	KTimePoint(const TimePoint *tp, int iTPCount = 1);
	//�õ�ʱ���ĸ���
	int Size() { return static_cast<int>(m_vecTP.size()); }
	
	//����Ԥ�ȴ���õ�����
	void Insert(float *fTP, int iTPCount = 1);
	
	void Insert( const TimePoint *pTP, int iTPCount = 1);

	//��λ���Ƴ�TIMEPOINT, ��0��ʼ
	inline void Remove( int iIndex);
	
	//�õ���ĳһ��ʱ���ֵ
	float GetValue( float fTime);

	vector<TimePoint> m_vecTP;
private:
	vector<TimePoint>::iterator GetTimeSocket( float fTime);
	FILE* fpDebug;
public:
	bool SaveToFile(FILE* & fp);
	void Reset(void);
	void Output(void);
};
}