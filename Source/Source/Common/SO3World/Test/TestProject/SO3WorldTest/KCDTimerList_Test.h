/**@file 
 * @brief Test class KCDTimerList
 * @author Li Qing
 * @date 2005.12.28
 */
#ifndef _TEST_KCDTIMERLIST_H_
#define _TEST_KCDTIMERLIST_H_
#include <vector>
#include <algorithm>

#include "KSUnitTest/KSCppUnit_Helper.h"
#include "KSO3World.h"
#include "KCDTimerList.h"

/**
 * KCDTimerList_Test
 */
class KCDTimerList_Test : public CppUnit::TestFixture
{
#ifndef TEST_DEBUG
	CPPUNIT_TEST_SUITE(KCDTimerList_Test);
	CPPUNIT_TEST(testLoad_TC1);		/*������ʱ����*/
	CPPUNIT_TEST(testLoad_TC2);		/*��ʱ���������ظ�ID*/
	CPPUNIT_TEST(testSave_TC1);		/*������ʱ��д*/
	CPPUNIT_TEST(testSave_TC2);		/*ֻ�в��ּ�ʱ���ᱣ��*/
	CPPUNIT_TEST(testResetTimer);
	CPPUNIT_TEST(testCheckTimer);
	CPPUNIT_TEST(testClearTimer);
	CPPUNIT_TEST_SUITE_END();
#endif
public:
	static const int TIMER_BUF_SIZE = 256;
	void setUp();
	void tearDown();
	
	void testLoad_TC1();
	void testLoad_TC2();
	void testSave_TC1();	
	void testSave_TC2();
	void testResetTimer();
	void testCheckTimer();
	void testClearTimer();
protected:
	KCDTimerList *m_TimerList1;
	KCDTimerList *m_TimerList2;
	KCDTimerList *m_TimerList3;
	KCDTimerList *m_TimerList4;
	KCDTimerList *m_TimerList5;
	KCDTimerList *m_NullTimerList;	//��KCDTimerList�����治����κ�����

	time_t m_StartTime;
	/**
	 * ԭʼtimer char������Ѻñ��淽ʽ��������<DWORD, int>��ʽ��
	 * ��ʱ����Ϣ������vector�С�
	 */
	typedef vector<pair<DWORD, int> > TIMER_VECTOR_T;
	char m_TimerBuf1[TIMER_BUF_SIZE];		//��������1��ԭʼ��ʽ
	char m_TimerBuf2[TIMER_BUF_SIZE];		//��������2��ԭʼ��ʽ
	char m_TimerBuf3[TIMER_BUF_SIZE];		//��������3��ԭʼ��ʽ
	char m_TimerBuf4[TIMER_BUF_SIZE];		//��������4�����ظ�ID



	void initTimerBuf1();	
	void initTimerBuf2();
	void initTimerBuf3();
	void initTimerBuf4();
	/** 
	 * ��char*�ж�timer��Ϣ�����ڲ�����ʹ�á�
	 * @param timerBuf	Ҫ��ȡ��char����
	 * @return timerVec	��timerBuf�еļ�ʱ�����浽timerVec�С�
	 */
	void readTimerBuf(char *timerBuf, TIMER_VECTOR_T &timerVec);
	/** 
	 * ��һ��������д��char������
	 * @param timerBuf	����
	 * @param timerNO	�ڼ�����ʱ����
	 * @param timerID	timer ID
	 * @param interval	timer interval
	 */
	void writeTimer(char *timerBuf, int timerNO, DWORD timerID, 
		int interval);
	/**
	 * �Ѽ�ʱ������д������
	 */
	void writeTimerCount(char *timerBuf, WORD timerCount);
	/** 
	 * ��ȡ���������һ����ʱ����Ϣ����һ��λ�ã��������һ����ʱ������
	 * ���20���ֽڣ���ô�ͷ���ָ���21�ֽڵ�ָ�롣
	 */
	char *getTimerBufEnd(char *timerBuf);
};

#endif //_TEST_KCDTIMERLIST_H_

