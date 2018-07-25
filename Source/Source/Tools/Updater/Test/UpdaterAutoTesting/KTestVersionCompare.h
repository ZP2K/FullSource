//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestVersionCompare.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-23 10:00
//  Comment     :   ��֤�Ƚϰ汾�ŵĽӿ���ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestVersionCompare_H__
#define _KTestVersionCompare_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestVersionCompare : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestVersionCompare);
	CPPUNIT_TEST(TestVersionEqual_01);
	CPPUNIT_TEST(TestVersionEqual_02);
	CPPUNIT_TEST(TestVersionEqual_03);
	CPPUNIT_TEST(TestVersionGreater_01);
	CPPUNIT_TEST(TestVersionGreater_02);
	CPPUNIT_TEST(TestVersionGreater_03);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//�Ƚϰ汾����ȵĹ��ܽӿ���֤
	//����A�汾�Ŵ��ڡ�С�ڡ�����B�汾�ŵ����
	//�ֱ�����ְ汾�Ÿ�ʽ������֤���磺1.0.8.1341��1,0,8,1341��1-0-8-1341
	void TestVersionEqual_01();
	void TestVersionEqual_02();
	void TestVersionEqual_03();

	//�Ƚϰ汾�Ŵ��ڵĹ��ܽӿ���֤
	//����A�汾�Ŵ��ڡ�С�ڡ�����B�汾�ŵ����
	//�ֱ�����ְ汾�Ÿ�ʽ������֤���磺1.0.8.1341��1,0,8,1341��1-0-8-1341
	void TestVersionGreater_01();
	void TestVersionGreater_02();
	void TestVersionGreater_03();
};
#endif ///KTestVersionCompare