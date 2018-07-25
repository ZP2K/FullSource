/***************************************************
Filename:KWndPageSet_Test.cpp
CreatedBy: WilsonWei
CreatedOn: 2005��7��27�� 14:32:24.8338619
Test Requirement Reference by Wiki path:
http://192.168.22.107/pmwiki/pmwiki.php?n=J3Test.J3-QC-REQ-SO3Client_UI_KWndPageSet

Comment:




**************************************************/



#include "StdAfx.h"

#ifdef  _UNIT_TEST
#include "KWndPageSet_Test.h"
#include "../ui/elem/wndpageset.h"
#include "../ui/elem/wndbutton.h"
#include "../ui/elem/wndpage.h"

CPPUNIT_TEST_SUITE_REGISTRATION( KWndPageSet_Test );


KWndPageSet_Test::KWndPageSet_Test()
{
}

KWndPageSet_Test::~KWndPageSet_Test()
{
}

void KWndPageSet_Test::setUp()
{
}

void KWndPageSet_Test::tearDown()
{

}

void KWndPageSet_Test::Test_Init()
{
}
void KWndPageSet_Test::Test_Bind()
{

}
//////////////////////////////////////////////////////////////////////////
//@������� : KWndPageSet_ActivePage_1
//@����Ŀ�� : 
//��û���κ�ҳ�������³��Լ���
//
//@ǰ�ó��� :
//@C1		: KWndPageSet��û���κ�Page
//@C2		:
//@����Դ   : ���Դ���
//@�������� : KWndPageSet��
//@�������� :
//@OP1		: �½�һ��KWndPageSetʵ��
//@OP2		: ����KWndPageSet.ActivePage(0)
//@���ó��� :
//@CP1		: ActivePage()����false
//@CP2		:
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_ActivePage_1()
{
	//ȷ���������κ�ҳ��
	UI::KWndPageSet pageSet;
	//������ڵ�ҳ��
	bool nRetCode = pageSet.ActivePage(0);
	CPPUNIT_ASSERT_EQUAL(false, nRetCode );
}


//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_ActivePage_2
//@����Ŀ�� : 
//��ֻ��һ��ҳ��������,���Լ�����ڵ�ҳ��
//
//@ǰ�ó��� :
//@C1		:KWndPageSet�д�������Page
//@C2		:
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1		:�½�һ��KWndPageSetʵ��
//@OP2		:��������Page
//@OP3		:����KWndPageSet.ActivePage(0)
//@���ó��� :
//@CP1		:ActivePage()����false
//@CP2		:��һҳ������
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_ActivePage_2()
{
	bool nRetCode = false;
	UI::KWndPageSet pageSet;
	UI::KWndPage page;
	UI::KWndPage page2;
	UI::KWndButton btn;
	UI::KWndButton btn2;
	nRetCode = pageSet.AddPage( &page, &btn);
	nRetCode = pageSet.AddPage( &page2, &btn2);
	nRetCode = pageSet.ActivePage(0);
	CPPUNIT_ASSERT( nRetCode == false);
	CPPUNIT_ASSERT_EQUAL( 0,pageSet.m_nAcitvePage);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_ActivePage_3
//@����Ŀ�� :
//��������ҳ��������,����ڶ���ҳ��
//
//@ǰ�ó��� :
//@C1       :KWndPageSet�д�������Page
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :�½�һ��KWndPageSetʵ��
//@OP2      :��������Page
//@OP3		:����KWndPageSet.ActivePage(1)
//@���ó��� :
//@CP1      :ActivePage()����true
//@CP2      :�ڶ�ҳ������
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_ActivePage_3()
{
	UI::KWndPageSet pageSet;
	UI::KWndPage page;
	UI::KWndPage page2;
	UI::KWndButton btn;
	UI::KWndButton btn2;
	pageSet.AddPage( &page, &btn);
	pageSet.AddPage( &page2, &btn2);
	bool nRetCode = pageSet.ActivePage(1);
	CPPUNIT_ASSERT_EQUAL( true, nRetCode);
	CPPUNIT_ASSERT_EQUAL( 1, pageSet.m_nAcitvePage);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_ActivePage_4
//@����Ŀ�� : 
//��������ҳ��������,������ڵ�ҳ��(����)
//
//@ǰ�ó��� :
//@C1       :KWndPageSet�д�������Page
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :�½�һ��KWndPageSetʵ��
//@OP2      :��������Page
//@OP3		:����KWndPageSet.ActivePage(-1)
//@���ó��� :
//@CP1      :ActivePage()����false
//@CP2      :m_nAcitvePage = 0
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_ActivePage_4()
{
	UI::KWndPageSet pageSet;
	UI::KWndPage page;
	UI::KWndPage page2;
	UI::KWndButton btn;
	UI::KWndButton btn2;
	pageSet.AddPage( &page, &btn);
	pageSet.AddPage( &page2, &btn2);
	bool nRetCode = pageSet.ActivePage( -1 );
	CPPUNIT_ASSERT_EQUAL( false, nRetCode );
	CPPUNIT_ASSERT_EQUAL( 0, pageSet.m_nAcitvePage);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_ActivePage_5
//@����Ŀ�� :
//��������ҳ��������,������ڵ�ҳ��(������)
//
//@ǰ�ó��� :
//@C1       :KWndPageSet�д�������Page
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :�½�һ��KWndPageSetʵ��
//@OP2      :��������Page
//@OP3		:����KWndPageSet.ActivePage(1000)
//@���ó��� :
//@CP1      :ActivePage()����false
//@CP2      :m_nAcitvePage=0
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_ActivePage_5()
{
	UI::KWndPageSet pageSet;
	UI::KWndPage page;
	UI::KWndPage page2;
	UI::KWndButton btn;
	UI::KWndButton btn2;
	pageSet.AddPage( &page, &btn);
	pageSet.AddPage( &page2, &btn2);
	bool nRetCode = pageSet.ActivePage( 10000 );
	CPPUNIT_ASSERT_EQUAL( false, nRetCode );
	CPPUNIT_ASSERT_EQUAL( 0, pageSet.m_nAcitvePage);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetActivePageIndex_1
//@����Ŀ�� : 
//�������κ�ҳ,��ȡ��ǰ����������
//
//@ǰ�ó��� : 
//@C1		: KWndPageSet�в������κ�ҳ
//@C2		:
//@����Դ   : ���Դ���
//@�������� : KWndPageSet
//@�������� :
//@OP1		:����һ��KWndPageSetʵ��
//@OP2		:����GetActivePageIndex()����
//@���ó��� :
//@CP1		: GetActivePageIndex���� -1
//@CP2		:
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_GetActivePageIndex_1()
{
    UI::KWndPageSet pageSet;
    int nPageIndex = pageSet.GetActivePageIndex();
    CPPUNIT_ASSERT_EQUAL( -1, nPageIndex);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetActivePageIndex_2
//@����Ŀ�� : 
//������ֻ��һ��ҳ�������,��ȡ�����ҳ����
//
//@ǰ�ó��� :
//@C1       :KWndPageSet�д���һ��ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :���һ��ҳ
//@OP3      :��ȡ��ǰ�������ҳ������
//@���ó��� :
//@CP1      :GetActivePageIndex����0
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_GetActivePageIndex_2()
{
    UI::KWndPageSet pageSet;
    UI::KWndPage page;
    UI::KWndButton btn;
    pageSet.AddPage( &page, &btn);
    int nPageIndex = pageSet.GetActivePageIndex();
    CPPUNIT_ASSERT_EQUAL( 0, nPageIndex);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetActivePageIndex_3
//@����Ŀ�� :
//�������κ�ҳ,��ȡ��ǰ����������
//@ǰ�ó��� :
//@C1       :KWndPageSet�в������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :�½�һ��KWndPageSetʵ��
//@OP2      :����GetActivePageIndex()
//@���ó��� :
//@CP1      :GetActivePageIndex()����-1
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_GetActivePageIndex_3()
{
	UI::KWndPageSet pageSet;
	UI::KWndPage page;
	int nPageIndex = pageSet.GetActivePageIndex();
	CPPUNIT_ASSERT_EQUAL( -1, nPageIndex);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_AddPage_1
//@����Ŀ�� :�������κ�ҳ,����һҳ,��鹦���Ƿ�����
//
//@ǰ�ó��� :
//@C1       :KWndPageSet�в������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :����һҳ
//@OP3		:��ȡҳ��Ŀ
//@���ó��� :
//@CP1      :m_nPageNum = 1
//@CP2      :
//////////////////////////////////////////////////////////////////////////
void KWndPageSet_Test::Test_AddPage_1()
{
    UI::KWndPageSet pageSet;
    UI::KWndPage page;
	UI::KWndButton btn;
	pageSet.AddPage( &page, &btn);
	int nPageNum = pageSet.m_nNumPage;
	CPPUNIT_ASSERT_EQUAL( 1, nPageNum);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_AddPage_2
//@����Ŀ�� :�������κ�ҳ,��������100ҳ,��鹦���Ƿ�����

//@ǰ�ó��� :
//@C1       :KWndPageSet�������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :��������100ҳ
//@OP3		:��ȡҳ��Ŀ
//@���ó��� :
//@CP1      :m_nPageNum = 100
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_AddPage_2()
{
	UI::KWndPageSet pageSet;
	for( int i=0; i<100 ; i++)
	{
		UI::KWndPage page;
		UI::KWndButton btn;
		pageSet.AddPage( &page, &btn);
	}
	int nPageNum = pageSet.m_nNumPage;
	CPPUNIT_ASSERT_EQUAL( 100, nPageNum);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetActivePage_1
//@����Ŀ�� :�������κ�ҳ,��ȡ��ǰ����ҳ

//@ǰ�ó��� :
//@C1       :KWndPageSet�в������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :����GetActivePage
//@���ó��� :
//@CP1      :GetActivePage()���� NULL
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_GetActivePage_1()
{
	UI::KWndPageSet pageSet;
	UI::KWndPage* pRetPage = pageSet.GetActivePage();
	CPPUNIT_ASSERT_MESSAGE("ֻҪ����NULL,����ȷ!", NULL==pRetPage);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetActivePage_2
//@����Ŀ�� :�������κ�ҳ,����һ����ҳ��,��ȡ��ǰ����ҳ

//@ǰ�ó��� :
//@C1       :KWndPageSet�в������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :����һ����ҳ
//@OP3		:����GetActivePage()
//@���ó��� :
//@CP1      :GetActivePage()���������ӵ�ҳ�����ָ��
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_GetActivePage_2()
{
	UI::KWndPageSet pageSet;
	UI::KWndPage page;
	UI::KWndButton btn;
	pageSet.AddPage( &page, &btn);
	UI::KWndPage* pRetPage = pageSet.GetActivePage();
	CPPUNIT_ASSERT( pRetPage);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetActivePage_3
//@����Ŀ�� :�������κ�ҳ,������100ҳ,��ȡ��ǰ����ҳ

//@ǰ�ó��� :
//@C1       :KWndPageSet�в������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :������100ҳ
//@OP3		:����GetActivePage()
//@���ó��� :
//@CP1      :GetActivePage()���ص�100ҳ��ָ��
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_GetActivePage_3()
{
	UI::KWndPageSet pageSet;
	for(int i = 0; i<100; i++)
	{
		UI::KWndPage page;
		UI::KWndButton btn;
		pageSet.AddPage( &page, &btn);
	}
	UI::KWndPage* pRetPage = pageSet.GetActivePage();
	CPPUNIT_ASSERT( pRetPage );
	CPPUNIT_ASSERT( !(pRetPage->GetNextWnd()));
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetActivePage_4
//@����Ŀ�� :�������κ�ҳ,������10ҳ,��ȡ��ǰ����ҳ

//@ǰ�ó��� :
//@C1       :KWndPageSet�в������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :������10ҳ
//@OP3		:����8ҳ����
//@OP4		:����GetActivePage()
//@���ó��� :
//@CP1      :GetActivePage()���ص�8ҳ��ָ��
//@CP2      :
//////////////////////////////////////////////////////////////////////////
void KWndPageSet_Test::Test_GetActivePage_4()
{
	UI::KWndPageSet pageSet;
	for(int i=0; i<10; i++)
	{
		UI::KWndPage page;
		UI::KWndButton btn;
		pageSet.AddPage( &page, &btn);
	}
	bool nRetCode = pageSet.ActivePage(7);
	CPPUNIT_ASSERT_EQUAL( true, nRetCode);
	UI::KWndPage* pActivePage = pageSet.GetActivePage();
	CPPUNIT_ASSERT( pActivePage);
	CPPUNIT_ASSERT_EQUAL( 7, pageSet.m_nAcitvePage);
}
//////////////////////////////////////////////////////////////////////////
//@������� :KWndPageSet_GetTypeInfo_1
//@����Ŀ�� :�������κ�ҳ,��ȡ������Ϣ

//@ǰ�ó��� :
//@C1       :KWndPageSet�в������κ�ҳ
//@C2       :
//@����Դ   :���Դ���
//@�������� :KWndPageSet��
//@�������� :
//@OP1      :����һ��KWndPageSetʵ��
//@OP2      :����GetTypeInfo
//@���ó��� :
//@CP1      :GetTypeInfo���س���ָ��(��ΪNULL)
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndPageSet_Test::Test_GetTypeInfo_1()
{
	UI::KWndPageSet pageSet;
	UI::KWndTypeInfo const* pTypeInfo = pageSet.GetTypeInfo();
	CPPUNIT_ASSERT( pTypeInfo );
}
void KWndPageSet_Test::Test_StaticMember()
{
}
void KWndPageSet_Test::DummyClassForFirstCopy(void* pvClassBeingTest)
{
    _testdata.m_nAcitvePage = ((UI::KWndPageSet*) pvClassBeingTest)->m_nAcitvePage;
    _testdata.m_nNumPage    = ((UI::KWndPageSet*) pvClassBeingTest)->m_nNumPage;
    //_testdata.ms_sTypeInfo = ((UI::KWndPageSet*) pvTestData)->ms_sTypeInfo;
    SetFirstCopy( &_testdata, sizeof( _TestData ));
}
void KWndPageSet_Test::DummyClassForSecondCopy(void* pvClassBeingTest)
{
    _testdata.m_nAcitvePage = ((UI::KWndPageSet*) pvClassBeingTest)->m_nAcitvePage;
    _testdata.m_nNumPage    = ((UI::KWndPageSet*) pvClassBeingTest)->m_nNumPage;
    //_testdata.ms_sTypeInfo = ((UI::KWndPageSet*) pvClass)->ms_sTypeInfo;
    SetSecondCopy( &_testdata, sizeof( _TestData ));
}
void KWndPageSet_Test::DummyClassToFile( LPCSTR szFilename)
{
}
void KWndPageSet_Test::CompareCopy(bool bAutoReleaseCopy)
{
    if( NULL != _FirstCopy && NULL != _SecondCopy)
    {
        _TestData* pFirstTestdata = (_TestData*) _FirstCopy;
        _TestData* pSecondTestData = (_TestData*) _SecondCopy;
        CPPUNIT_ASSERT( pFirstTestdata->m_nAcitvePage == pSecondTestData->m_nAcitvePage);
        CPPUNIT_ASSERT( pFirstTestdata->m_nNumPage == pSecondTestData->m_nNumPage);
        if( bAutoReleaseCopy)
            ReleaseAllCopy();
    }
}

#endif //End of #ifdef _UNIT_TEST