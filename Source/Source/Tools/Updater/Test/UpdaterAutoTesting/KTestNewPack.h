//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestNewPack.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-25 11:00
//  Comment     :   ��֤��pak������ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestNewPack_H__
#define _KTestNewPack_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestNewPack : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestNewPack);
	CPPUNIT_TEST(TestNewPackSuccess);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//��֤�ְ���Ľ����ȷ
	void TestNewPackSuccess();
};

#endif ///_KTestNewPack_H__