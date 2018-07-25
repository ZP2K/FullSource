//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestExtractPack.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-25 11:00
//  Comment     :   ��֤��pak������ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestExtractPack_H__
#define _KTestExtractPack_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestExtractPack : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestExtractPack);
	CPPUNIT_TEST(TestExtractPackSuccess);
	CPPUNIT_TEST(TestExtractPack_NoVersionFile);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//��ѹ��Ŀ���ļ��в�����
	//��ѹ�ĵ�һ���ļ�Ϊ0kb��С
	//���°����ڲ�ͬ��Ŀ¼��������version.cfg
	//���°�updatepackĿ¼֮������ļ����ܱ���ѹ����
	//���°�updatepackĿ¼֮�´��ڶ��Ŀ¼
	//�������µ��ļ�����ѹ
	//updatepack�ļ���֮����ļ�����Ҫ��ѹ
	void TestExtractPackSuccess();

	//���°���û��version.cfg
	void TestExtractPack_NoVersionFile();
};

#endif ///_KTestNewPack_H__