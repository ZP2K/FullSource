//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestRegOperation.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-23 15:00
//  Comment     :   ��֤��дע���ӿڵ���ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestRegOperation_H__
#define _KTestRegOperation_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestRegOperation : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestRegOperation);
	CPPUNIT_TEST(TestInstallDirectory);
	CPPUNIT_TEST(TestVersionInfo);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//��֤��дע���װ��Ϣ
	void TestInstallDirectory();
	//��֤��дע���汾��Ϣ
	void TestVersionInfo();

private:
	BOOL m_bRegSaved;								//��¼ԭʼ��ע���ı�־
};

#endif ///_KTestRegOperation_H__