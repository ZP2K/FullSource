//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestCombinePack.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-25 11:00
//  Comment     :   ��֤�ϲ�pak������ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestCombinePack_H__
#define _KTestCombinePack_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestCombinePack : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestCombinePack);
	CPPUNIT_TEST(TestCombineOnce);
	CPPUNIT_TEST(TestCombineSeveralTimes);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//�ϲ�1���������������н����֤
	void TestCombineOnce();
	//�ϲ�����������������н����֤��������Ժ�10�ΰ�
	void TestCombineSeveralTimes();

private:
	TCHAR m_szTestPack1[MAX_PATH];		//�ϲ�Ŀ¼��
	TCHAR m_szTestPack2[MAX_PATH];		//���Ժϲ���
};

#endif ///_KTestCombinePack_H__