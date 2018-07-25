#include "stdafx.h"
#include "KRandGenerator.h"
#include "Engine/Random.h"
#include "KSO3World.h"

#define	RAND_INTERVAL	(86400)

KRandGenerator::KRandGenerator(void)
{
}

KRandGenerator::~KRandGenerator(void)
{
}

char* KRandGenerator::Load(char* pData)
{
    BYTE* pbyCount          = NULL;
    WORD* pwGlobalRPValue   = NULL;
	std::pair<RAND_LIST::iterator, bool> InsRet;

	KGLOG_PROCESS_ERROR(pData);

	//���ԭ������
	m_RandList.clear();

	//��ȡ��ʱ������
	pbyCount = (BYTE*)pData;
	pData += sizeof(BYTE);

	KGLOG_PROCESS_ERROR(*pbyCount <= MAX_RAND_COUNT);

	//��ȡ��ʱ��
	for (int nIndex = 0; nIndex < *pbyCount; nIndex++)
	{
		WORD*	pwIndex = (WORD*)pData;
		pData += sizeof(WORD);

		time_t*	pTime = (time_t*)pData;
		pData += sizeof(time_t);

		InsRet = m_RandList.insert(std::make_pair(*pwIndex, *pTime));
		KGLOG_PROCESS_ERROR(InsRet.second);
	}

	//��ȡRP��¼
	pwGlobalRPValue = (WORD*)pData;
	pData += sizeof(WORD);

	m_wGlobalRPValue = *pwGlobalRPValue;

	for (int nIndex = 0; nIndex < MAX_RP_GROUP_COUNT; nIndex++)
	{
		WORD* pwRPValue = (WORD*)pData;
		pData += sizeof(WORD);

		m_wRPList[nIndex] = *pwRPValue;
	}

	return pData;

Exit0:

	return NULL;
}

char* KRandGenerator::Save(char* pData)
{
    BYTE* pbyCount        = NULL;
    WORD* pwGlobalRPValue = NULL;
	KGLOG_PROCESS_ERROR(pData);

	//д���ʱ������
	pbyCount = (BYTE*)pData;
	pData += sizeof(BYTE);

	_ASSERT(m_RandList.size() < 256 && MAX_RAND_COUNT < 256);
	*pbyCount = (BYTE)m_RandList.size();

	//д���ʱ��
	for (RAND_LIST::iterator it = m_RandList.begin();
		it != m_RandList.end(); ++it)
	{
		WORD*	pwIndex = (WORD*)pData;
		pData += sizeof(WORD);

		time_t*	pTime = (time_t*)pData;
		pData += sizeof(time_t);

		*pwIndex = it->first;
		*pTime = it->second;
	}

	//д��RP��¼
	pwGlobalRPValue = (WORD*)pData;
	pData += sizeof(WORD);

	*pwGlobalRPValue = m_wGlobalRPValue;

	for (int nIndex = 0; nIndex < MAX_RP_GROUP_COUNT; nIndex++)
	{
		WORD* pwRPValue = (WORD*)pData;
		pData += sizeof(WORD);

		*pwRPValue = m_wRPList[nIndex];
	}

	return pData;

Exit0:

	return NULL;
}

BOOL KRandGenerator::Generate(int nProbability, int nMaxNumber, WORD wRandIndex, 
							  DWORD dwRPGroupIndex, int nRPBonus)
{
	BOOL bResult        = false;
	int nRand           = 0;
    time_t CurrentTime  = 0;
	RAND_LIST::iterator it;
	std::pair<RAND_LIST::iterator, bool> InsRet;

	KGLOG_PROCESS_ERROR(dwRPGroupIndex < MAX_RP_GROUP_COUNT);

	//ȡ��ǰʱ��
	CurrentTime = g_pSO3World->m_nCurrentTime;

	//��ʱ����в���
	it = m_RandList.find(wRandIndex);

	//���û�鵽,����Ҫ����
	if (it == m_RandList.end())
	{
		if (m_RandList.size() >= MAX_RAND_COUNT)	//���ʱ�������
		{
			//�����ѡһ��ɾ��
			nRand = g_Random((unsigned int)m_RandList.size());
			it = m_RandList.begin();
			while (nRand > 0)
			{
				nRand--;
				it++;
			}
			m_RandList.erase(it);
		}

		//�����µļ�ʱ��
		InsRet = m_RandList.insert(std::make_pair(wRandIndex, CurrentTime));
		KGLOG_PROCESS_ERROR(InsRet.second);

		it = InsRet.first;
	}

	KGLOG_PROCESS_ERROR(it != m_RandList.end());

	//����Ƿ����´μ�����ʵ�ʱ��
	KG_PROCESS_ERROR(it->second <= CurrentTime);

	//�������
	nRand = g_Random(nMaxNumber);

	//��ȡRP�ĸ��ʼӳ�
	nRand = GetRPRand(nRand, m_wRPList[dwRPGroupIndex]);

	//�Ƿ�ɹ�
	if (nRand < nProbability)
	{
		//���RP
		m_wGlobalRPValue = 0;
		m_wRPList[dwRPGroupIndex] = 0;

		bResult = true;
	}
	else
	{
		//�ӳ�RP
		int nCurrentRP = m_wRPList[dwRPGroupIndex];
		nCurrentRP += nRPBonus;
		if (nCurrentRP > 0)
			m_wRPList[dwRPGroupIndex] = (WORD)nCurrentRP;
		else
			m_wRPList[dwRPGroupIndex] = 0;
	}

	//�����´μ�����ʵ�ʱ��
	it->second = CurrentTime + RAND_INTERVAL;

Exit0:

	return bResult;
}

int	KRandGenerator::GetRPRand(int nRand, WORD wRPValue)
{
	return nRand - wRPValue - m_wGlobalRPValue;
}
