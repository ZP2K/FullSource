//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   TestClientReporter.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-3-13 14:30
//  Comment     :   ��֤�ռ���Ϸ��Ϣ
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _TestClientReporter_H__
#define _TestClientReporter_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class TestClientReporter : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestClientReporter);
	CPPUNIT_TEST(KTestFullRun_01);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void KTestFullRun_01();

};

#endif ///_TestClientReporter_H__
