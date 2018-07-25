#pragma once

/***************************************************
Filename:Test_UI_wndpageset.h
CreatedBy: WilsonWei
CreatedOn: 2005��7��27�� 10:51:16.7778130
Test Requirement Reference by Wiki path:
http://192.168.22.107/pmwiki/pmwiki.php?n=J3Test.J3-QC-REQ-SO3Client_UI_KWndPageSet
Comment:




**************************************************/



#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/TestFixture.h"
#include "../ui/elem/wndshowanimate.h"
#include "KSUnitTest/KingsoftUnitTest.h"
#include "KSUnitTest/ClassDataCopyBuffer.H"

class KWndPageSet_Test : public CppUnit::TestFixture, public KingsoftUnitTest::ClassDataCopyBuffer, public KingsoftUnitTest::IClassUnitTest
{
public:
	KWndPageSet_Test();
	~KWndPageSet_Test();
private:
    struct _TestData
    {
        _TestData(){};
        ~_TestData(){};
        int				m_nNumPage;					//ҳ�����Ŀ
        int				m_nAcitvePage;				//��ǰ�����ҳ��
    };
    _TestData _testdata;
public: 
    void setUp();
    void tearDown();
public:
    CPPUNIT_TEST_SUITE( KWndPageSet_Test );
    CPPUNIT_TEST( Test_Init);
    CPPUNIT_TEST( Test_Bind);
    CPPUNIT_TEST( Test_ActivePage_1);
    CPPUNIT_TEST( Test_ActivePage_2);
	CPPUNIT_TEST( Test_ActivePage_3);
	CPPUNIT_TEST( Test_ActivePage_4);
	CPPUNIT_TEST( Test_ActivePage_5);
    CPPUNIT_TEST( Test_GetActivePageIndex_1);
    CPPUNIT_TEST( Test_GetActivePageIndex_2);
	CPPUNIT_TEST( Test_GetActivePageIndex_3);
    CPPUNIT_TEST( Test_AddPage_1);
    CPPUNIT_TEST( Test_AddPage_2);
    CPPUNIT_TEST( Test_GetActivePage_1);
    CPPUNIT_TEST( Test_GetActivePage_2);
	CPPUNIT_TEST( Test_GetActivePage_3);
	CPPUNIT_TEST( Test_GetActivePage_4);
    CPPUNIT_TEST( Test_GetTypeInfo_1);
    CPPUNIT_TEST( Test_StaticMember);
    CPPUNIT_TEST_SUITE_END();

private:
    void Test_Init();
    void Test_Bind();
    void Test_ActivePage_1();
    void Test_ActivePage_2();
	void Test_ActivePage_3();
	void Test_ActivePage_4();
	void Test_ActivePage_5();
    void Test_GetActivePageIndex_1();
    void Test_GetActivePageIndex_2();
	void Test_GetActivePageIndex_3();
    void Test_GetTypeInfo_1();
    void Test_AddPage_1();
    void Test_AddPage_2();
    void Test_StaticMember();
    void Test_GetActivePage_1();
    void Test_GetActivePage_2();
	void Test_GetActivePage_3();
	void Test_GetActivePage_4();

public:
    /*������ڴ�������Ƶ���һ�ݿ���*/
    void DummyClassForFirstCopy(void* pvClassBeingTest);
    /*������ڴ�������Ƶ��ڶ��ݿ���*/
    void DummyClassForSecondCopy(void* pvClassBeingTest);
    /*������ڴ�������浽ָ�����ļ�*/
    void DummyClassToFile( LPCSTR szFilename);
    /*�Ƚ����ݿ���*/
    void CompareCopy(bool bAutoReleaseCopy);
};
