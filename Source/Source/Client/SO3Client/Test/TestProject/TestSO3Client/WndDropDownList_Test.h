/***************************************************
Filename:WndDropDownList_Test.h
CreatedBy: HouRui
CreatedOn: 2005��7��31�� 21:00:00
Test Requirement Reference by Wiki path:
http://192.168.22.107/pmwiki/pmwiki.php?n=J3Test.J3-QC-REQ-SO3Client_UI_dropdownlist
Comment:

**************************************************/

#ifndef WNDDROPDOWNLIST_TEST_H
#define WNDDROPDOWNLIST_TEST_H

#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/TestFixture.h"
#include "../ui/script/kwndscripttable.h"
#include "KSUnitTest/KingsoftUnitTest.h"
#include "KSUnitTest/ClassDataCopyBuffer.H"
//#include "../ui/elem/wnddropdownlist.h"

#include "../ui/elem/wndedit.h"
#include "../ui/elem/wndimage.h"
#include "../ui/elem/wndlist2.h"

class WndDropDownList_Test : 
        public CppUnit::TestFixture,
        public KingsoftUnitTest::ClassDataCopyBuffer
        //public KingsoftUnitTest::IClassUnitTest
{
    CPPUNIT_TEST_SUITE(WndDropDownList_Test);

    CPPUNIT_TEST(Test_InsertItem_1);
    CPPUNIT_TEST(Test_InsertItem_2);
    CPPUNIT_TEST(Test_InsertItem_3);
    CPPUNIT_TEST(Test_InsertItem_4);
    CPPUNIT_TEST(Test_InsertItem_5);
    CPPUNIT_TEST(Test_InsertItem_6);
    CPPUNIT_TEST(Test_InsertItem_7);
    CPPUNIT_TEST(Test_InsertItem_8);
    CPPUNIT_TEST(Test_InsertItem_9);
    CPPUNIT_TEST(Test_InsertItem_10);
    CPPUNIT_TEST(Test_InsertItem_11);
    CPPUNIT_TEST(Test_InsertItem_12);
	CPPUNIT_TEST(Test_InsertItem_13);
    CPPUNIT_TEST(Test_InsertItem_14);
	CPPUNIT_TEST(Test_InsertItem_15);
	CPPUNIT_TEST(Test_InsertItem_16);
	CPPUNIT_TEST(Test_InsertItem_17);
	CPPUNIT_TEST(Test_InsertItem_18);
	CPPUNIT_TEST(Test_InsertItem_19);
	CPPUNIT_TEST(Test_InsertItem_20);
	CPPUNIT_TEST(Test_InsertItem_21);

    CPPUNIT_TEST(Test_DeleteItem_1);
    CPPUNIT_TEST(Test_DeleteItem_2);
    CPPUNIT_TEST(Test_DeleteItem_3);
    CPPUNIT_TEST(Test_DeleteItem_4);
    CPPUNIT_TEST(Test_DeleteItem_5);

    CPPUNIT_TEST(Test_SelectItem_1);
    CPPUNIT_TEST(Test_SelectItem_2);
    CPPUNIT_TEST(Test_SelectItem_3);
	CPPUNIT_TEST(Test_SelectItem_4);

    CPPUNIT_TEST(Test_SearchItem_1);
    CPPUNIT_TEST(Test_SearchItem_2);
    CPPUNIT_TEST(Test_SearchItem_3);
    CPPUNIT_TEST(Test_SearchItem_4);
    CPPUNIT_TEST(Test_SearchItem_5);

    CPPUNIT_TEST(Test_SetFontSize_1); 
    CPPUNIT_TEST(Test_SetFontSize_2);

    CPPUNIT_TEST(Test_SetTipColor_1);
    CPPUNIT_TEST(Test_SetTipColor_2);

    CPPUNIT_TEST(Test_SetListBGColor_1); 
    CPPUNIT_TEST(Test_SetListBGColor_2); 

    CPPUNIT_TEST(Test_SetListSLColor_1);
    CPPUNIT_TEST(Test_SetListSLColor_2);

    CPPUNIT_TEST(Test_SetListFLColor_1);
    CPPUNIT_TEST(Test_SetListFLColor_2);

    CPPUNIT_TEST_SUITE_END();

//public:
//    WndDropDownList_Test(void);
//    ~WndDropDownList_Test(void);

//private:
//    struct _TestData
//    {
//        _TestData(){};
//        ~_TestData(){};
//        BOOL			m_bEditable;		// �����ܷ񱻱༭
//        BOOL			m_bEnableTip;		// ��ֹ��������ʾTip
//        int		    m_nFontSize;		// �����С
//        int			m_nMaxItemNum;		// �����¼������¼����Ŀ
//        char			m_szTipInfo[128];	// Tip��ʾ
//        DWORD			m_dwTipColor;		// Tip������ɫ
//        DWORD			m_dwListBGColor;	// �����б�ı�����ɫ
//        DWORD			m_dwListSLColor;	// �����б�ķָ�����ɫ
//        DWORD			m_dwListFLColor;	// �����б�ı߿���ɫ
//        BOOL			m_bListExtend;		// �Ƿ���չ��״̬
//    };
//    _TestData _testdata;

public:
    void setUp();           
    void tearDown();

public:
    //����������ԣ�����2�����⺯����ָ��λ�ò�������б�ĩβ���룩
    void Test_InsertItem_1();
    void Test_InsertItem_2();
    void Test_InsertItem_3();
    void Test_InsertItem_4();
    void Test_InsertItem_5();
    void Test_InsertItem_6();
    void Test_InsertItem_7();
    void Test_InsertItem_8();
    void Test_InsertItem_9();
    void Test_InsertItem_10();
    void Test_InsertItem_11();
    void Test_InsertItem_12();
	void Test_InsertItem_13();
	void Test_InsertItem_14();
	void Test_InsertItem_15();
	void Test_InsertItem_16();
	void Test_InsertItem_17();
	void Test_InsertItem_18();
	void Test_InsertItem_19();
	void Test_InsertItem_20();
	void Test_InsertItem_21();

    //ɾ������ԣ�����2�����⺯����ָ��λ��ɾ�������б�ĩβɾ����
    void Test_DeleteItem_1();
    void Test_DeleteItem_2();
    void Test_DeleteItem_3();
    void Test_DeleteItem_4();
    void Test_DeleteItem_5();

    //���õ�ǰѡ�������
    void Test_SelectItem_1();
    void Test_SelectItem_2();
    void Test_SelectItem_3();
    void Test_SelectItem_4();

    //����ָ����Ĳ��ԣ�����2�����⺯��������ָ���ַ���ֵ���������ָ������ֵ���
    void Test_SearchItem_1();
    void Test_SearchItem_2();
    void Test_SearchItem_3();
    void Test_SearchItem_4();
    void Test_SearchItem_5();

    //���������С����
    void Test_SetFontSize_1();
    void Test_SetFontSize_2();

    //����Tip������ɫ����
    void Test_SetTipColor_1();
    void Test_SetTipColor_2();

    //���������б�ı�����ɫ����
    void Test_SetListBGColor_1();
    void Test_SetListBGColor_2();

    //���������б�ķָ�����ɫ����
    void Test_SetListSLColor_1();
    void Test_SetListSLColor_2();

    //���������б�ı߿���ɫ����
    void Test_SetListFLColor_1();
    void Test_SetListFLColor_2();

private:
    UI::KWndEdit          *m_pEditTextBox1;
    UI::KWndImage		  *m_pButtonImage1;
    UI::KWndList2		  *m_pItemList1;
    //UI::KWndDropDownList  *m_dropDownList;
//public:
//    /*������ڴ�������Ƶ���һ�ݿ���*/
//    void DummyClassForFirstCopy(void* pvClassBeingTest);
//    /*������ڴ�������Ƶ��ڶ��ݿ���*/
//    void DummyClassForSecondCopy(void* pvClassBeingTest);
//    /*������ڴ�������浽ָ�����ļ�*/
//    void DummyClassToFile( LPCSTR szFilename);
//    /*�Ƚ����ݿ���*/
//    void CompareCopy(bool bAutoReleaseCopy);
};

#endif //WNDDROPDOWNLIST_TEST_H