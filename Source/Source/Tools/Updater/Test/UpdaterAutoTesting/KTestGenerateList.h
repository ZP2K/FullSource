//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestGenerateList.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-23 15:00
//  Comment     :   ��֤���������б����ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestGenerateList_H__
#define _KTestGenerateList_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"
#include "KVersionParser.h"

class KTestGenerateList : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestGenerateList);
	CPPUNIT_TEST(TestGenerateSuccess);
	CPPUNIT_TEST(TestLostRecord);
	CPPUNIT_TEST(TestSeveralVersionInOnePack);
	CPPUNIT_TEST(TestIntersectionVersionInside);
	CPPUNIT_TEST(TestIntersectionVersionOutside);
	CPPUNIT_TEST(TestSubsetVersion);
	CPPUNIT_TEST(TestGenerateFail);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//�����б��еĸ��°���¼��ȷ���������������б���ȷ�Ե���֤
	void TestGenerateSuccess();
	//�����б��еĸ��°���¼ȱʧ�Ĵ��� 
	void TestLostRecord();
	//�����б��еĸ��°���¼�п��汾�Ĵ���  
	void TestSeveralVersionInOnePack();
	//�����б��еĸ��°���¼�н����汾�Ĵ������뽻���汾"��"�Ĳ��԰汾��
	void TestIntersectionVersionInside();
	//�����б��еĸ��°���¼�н����汾�Ĵ������뽻���汾"��"�Ĳ��԰汾��
	void TestIntersectionVersionOutside();
	//�����б��еĸ��°���¼���Ӽ��汾�Ĵ��� 
	void TestSubsetVersion();
	//�޷��������ĳ�汾�ŵ������б�Ĵ���
	void TestGenerateFail();

private:
	//��֤����
	int ValidateData(
		KVersionParser* pParser,
		const TCHAR** cppZipList,
		const int cnListCount,
		const TCHAR cszEntryFile[],
		const TCHAR cszCurVersion[]
	);
};
#endif ///_KTestGenerateList_H__