#ifndef _KRANDGENERATOR_TEST_H_
#define _KRANDGENERATOR_TEST_H_

#include <vector>
#include <windows.h>
#include <climits>
#include <ctime>
#include "KSUnitTest/KSCppUnit_Helper.h"
#include "KRandGenerator.h"

class KRandGenerator_Test : public CppUnit::TestFixture
{

	CPPUNIT_TEST_SUITE(KRandGenerator_Test);
	CPPUNIT_TEST(testLoad_TC1);	//���뻺��Ϊ��, null pointer
	CPPUNIT_TEST(testLoad_TC2);	//RAND_LIST����Ϊ0, Global RPֵΪ0
	CPPUNIT_TEST(testLoad_TC3);	//RAND_LIST����ΪMAX_RAND_COUNT,
								//Global RPֵΪSHRT_MAX
	CPPUNIT_TEST(testLoad_TC4);	//RAND_LIST����Ϊ1, Global RPֵΪ1
	CPPUNIT_TEST(testSave_TC1);	//�������ָ��Ϊ��
	CPPUNIT_TEST(testSave_TC2);	//�ı�Global RP�󱣴�
	CPPUNIT_TEST_SUITE_END();

public:
	//max necessary size of input buffer
	static const int RAND_BUF_SIZE = sizeof(BYTE) + 
		(sizeof(WORD) + sizeof(time_t)) * MAX_RAND_COUNT + 
		sizeof(WORD) +
		sizeof(WORD) * MAX_RP_GROUP_COUNT;
	void setUp();
	void tearDown();

	void testLoad_TC1();	
	void testLoad_TC2();	
	void testLoad_TC3();	
	void testLoad_TC4();
	void testSave_TC1();
	void testSave_TC2();
private:
	typedef vector<pair<WORD, time_t> > RAND_TIMER_VECTOR;

	KRandGenerator *m_pKRandGenerator1;
	KRandGenerator *m_pKRandGenerator2;
	KRandGenerator *m_pKRandGenerator3;
	KRandGenerator *m_pKRandGenerator4;
	KRandGenerator *m_pKRandGenerator5;
	//RAND_LIST����Ϊ0, Global RPֵΪ0
	char m_RandBuf1[RAND_BUF_SIZE];
	//RAND_LIST����ΪMAX_RAND_COUNT, Global RPֵΪSHRT_MAX
	char m_RandBuf2[RAND_BUF_SIZE];	
	//RAND_LIST����Ϊ1, Global RPֵΪ1
	char m_RandBuf3[RAND_BUF_SIZE];
	char m_RandBuf4[RAND_BUF_SIZE];
	time_t startTime;

	//use different scheme to init a buffer
	void initRandBuf1(char *);
	void initRandBuf2(char *);
	void initRandBuf3(char *);
	void initRandBuf4(char *);

	void writeRandCount(char *buf, BYTE randCount);
	void writeRandIndex(char *buf, int randNO, WORD index);
	void writeRandTime(char *buf, int randNO, time_t t);
	void writeGlobbalRP(char *buf, int randCount, WORD globalRP);
	void writeRP(char *buf, int randCount, int rpNO, WORD rp);
	//find the end of the rand_info_buffer
	char *getRandBufEnd(char *buf);
	//load <randId, randTime> info to a vector
	void loadRandTimer(char *buf, RAND_TIMER_VECTOR &vec);
	//find the position where Global RP lies
	char *findGlobalRPPos(char *buf);
	//calculate how many bytes which are occupied by RP info
	size_t getRPSize();
};

#endif