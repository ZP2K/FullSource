/***************************************************
Filename:  testDropdownlist.cpp
CreatedBy: HouRui 
CreatedOn: 2005��7��31�� 21:00:00
Test Requirement Reference by Wiki path:
http://192.168.22.107/pmwiki/pmwiki.php?n=J3Test.J3-QC-REQ-SO3Client_UI_dropdownlist
Comment:

**************************************************/

#include "stdafx.h"
#include "WndDropDownList_Test.h"
#include "../ui/elem/wnddropdownlist.h"


#ifdef _UNIT_TEST

#include <iostream>
using namespace std;
UI::KWndDropDownList  *m_dropDownList;
CPPUNIT_TEST_SUITE_REGISTRATION(WndDropDownList_Test);



void WndDropDownList_Test::setUp()
{
	m_pEditTextBox1 = new UI::KWndEdit;
    m_pButtonImage1 = new UI::KWndButton;
    m_pItemList1    = new UI::KWndList2;
    m_dropDownList  = new UI::KWndDropDownList;
    m_dropDownList->BindEdit(m_pEditTextBox1);
    m_dropDownList->BindImage(m_pButtonImage1);
    m_dropDownList->BindList2(m_pItemList1);
}

void WndDropDownList_Test::tearDown()
{
    if (m_pEditTextBox1)
    {
        delete m_pEditTextBox1;
        m_pEditTextBox1 = NULL;
    }
    if (m_pButtonImage1)
    {
        delete m_pButtonImage1;
        m_pButtonImage1 = NULL;
    }
    if (m_pItemList1)
    {
        delete m_pItemList1;
        m_pItemList1 = NULL;
    }
    if (m_dropDownList)
    {
        delete m_dropDownList;
        m_dropDownList = NULL;
    }   
}

//WndDropDownList_Test::WndDropDownList_Test()
//{
//}
//
//WndDropDownList_Test::~WndDropDownList_Test()
//{
//}

//////////////////////////////////////////////////////////////////////////
//@������� :1.1 
//@����Ŀ�� :����InsertItem�Ƿ������б�Ϊ�յ�ʱ���������
//@ǰ�ó��� :
//@C1       :�б�Ϊ�գ���InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :��������
//@���ó��� :
//@CP1      :���Բ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_1()
{
    int nRetCode		    = false;
    const char cszStrName[] = "item1";
    char szStr[10];

	nRetCode = m_dropDownList->InsertItem(0, cszStrName);
    CPPUNIT_ASSERT(nRetCode == 0);
	nRetCode = m_dropDownList->GetItemCount();
    CPPUNIT_ASSERT(nRetCode == 1);
	nRetCode = m_dropDownList->GetItemText(0, szStr, sizeof(szStr));
	CPPUNIT_ASSERT(nRetCode);
	CPPUNIT_ASSERT(strcmp(szStr, cszStrName) == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.2
//@����Ŀ�� :��֤�ܹ����б�����������ڱ�ͷ�����У���β��������
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :�ڱ�ͷ�����У���β�ֱ��������
//@���ó��� :
//@CP1      :�����ڱ�ͷ�����У���β��������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_2()
{
    int nRetCode				= false;
    int i						= 0;
    const char *cszStrName[]    = {"item2", "item4", "item1", "item3", "item5"};
    const char *cszStrNameNew[] = {"item1", "item2", "item3", "item4", "item5"};
    char szStr[10]				= {0};  

    for (i = 0; i < 2; i++)
	{
		nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == i);
	}
	//�ڱ�ͷ����
	nRetCode = m_dropDownList->InsertItem(0, cszStrName[2]);
	CPPUNIT_ASSERT(nRetCode == 0);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
	
	//�ڱ��в���
	nRetCode = m_dropDownList->InsertItem(2, cszStrName[3]);
	CPPUNIT_ASSERT(nRetCode == 2);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 4);

	//�ڱ�β����
	nRetCode = m_dropDownList->InsertItem(4, cszStrName[4]);
	CPPUNIT_ASSERT(nRetCode == 4);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 5);

    for (i = 0; i < 5; i++)
    {
        nRetCode = m_dropDownList->GetItemText(i, szStr, sizeof(szStr));
        CPPUNIT_ASSERT(nRetCode > 0);
        CPPUNIT_ASSERT(strcmp(szStr, cszStrNameNew[i]) == 0);    
    }   
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.3
//@����Ŀ�� :��֤�ܹ����б��ĩβ������������ַ�������
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������ַ�
//@���ó��� :
//@CP1      :���Բ�����������ַ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_3()
{
    int nRetCode			 = false;
    int i					 = 0;
    const char *cszStrName[] = {"item1", "item2", "item3", "���šơǡȡΡޡ�"};
    char szStr[20]			 = {0};  

    for (i = 0; i < 4; i++)
    {
        nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
        CPPUNIT_ASSERT(nRetCode == i);
		if (i == 3)
		{
			nRetCode = m_dropDownList->GetItemCount();
			CPPUNIT_ASSERT(nRetCode == 4);
			nRetCode = m_dropDownList->GetItemText(3, szStr, sizeof(szStr));
			CPPUNIT_ASSERT(strcmp(szStr, cszStrName[3]) == 0);
		}
    }
    
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.4
//@����Ŀ�� :��֤�ַ���ָ��Ϊ�յ�ʱ���ܲ���
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ʱ�����ָ��
//@���ó��� :���ܲ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_4()
{
    int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
    char *szStr		   = NULL;

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == i);
	}

	nRetCode = m_dropDownList->InsertItem(3, szStr);
	CPPUNIT_ASSERT(nRetCode == -1);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);

}

//////////////////////////////////////////////////////////////////////////
//@������� :1.5
//@����Ŀ�� :��֤���ܲ�����ַ���
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���б��ĩβ���������ַ���
//@���ó��� :
//@CP1      :���ܲ�����ַ���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_5()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char *szStr		   = "";

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == i);
	}

	nRetCode = m_dropDownList->InsertItem(3, szStr);
	CPPUNIT_ASSERT(nRetCode == -1);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.6
//@����Ŀ�� :��֤���ܲ���ո�
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ո�
//@���ó��� :
//@CP1      :���ܲ���ո�
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_6()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char *szStr		   = " ";

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == i);
	}

	nRetCode = m_dropDownList->InsertItem(3, szStr);
	CPPUNIT_ASSERT(nRetCode == -1);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.7
//@����Ŀ�� :��֤���ܲ��볬���ַ���260��a
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���볬���ַ���
//@���ó��� :
//@CP1      :���Բ��볬���ַ���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_7()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char szStr[]	   = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	char szOutStr[270];
	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == i);
	}

	nRetCode = m_dropDownList->InsertItem(3, szStr);
	CPPUNIT_ASSERT(nRetCode == 3);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 4);
	nRetCode = m_dropDownList->GetItemTextLength(3);
	CPPUNIT_ASSERT(nRetCode == 260);
	nRetCode = m_dropDownList->GetItemText(3, szOutStr, sizeof(szOutStr));
	CPPUNIT_ASSERT(nRetCode);
	CPPUNIT_ASSERT(strcmp(szOutStr, szStr) == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.8
//@����Ŀ�� :��֤���ܲ��뵽������λ��
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :Ҫ�����λ�ò�����
//@���ó��� :
//@CP1      :���ܲ��뵽������λ��
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_8()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char *szStr		   = "item";
	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == i);
	}

	nRetCode = m_dropDownList->InsertItem(5, szStr);
	CPPUNIT_ASSERT(nRetCode == -1);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
}
//////////////////////////////////////////////////////////////////////////
//@������� :1.9
//@����Ŀ�� :��֤���ܲ����ظ�����
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :�����ظ�����
//@���ó��� :
//@CP1      :���ܲ����ظ�����
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_9()
{
	int nRetCode		    = false;
	const char cszStrName[] = "item1";

	nRetCode = m_dropDownList->InsertItem(0, cszStrName);
	CPPUNIT_ASSERT(nRetCode == 0);
	nRetCode = m_dropDownList->InsertItem(1, cszStrName);
	CPPUNIT_ASSERT(nRetCode == -1);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 1);
}
//////////////////////////////////////////////////////////////////////////
//@������� :1.10
//@����Ŀ�� :��֤���ܲ���û�н��������ַ���
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����û�н��������ַ���
//@���ó��� :
//@CP1      :���ܲ���û�н��������ַ���
//////////////////////////////////////////////////////////////////////////

void WndDropDownList_Test::Test_InsertItem_10()
{
	int nRetCode		   = false;
	int i				   = 0;
	char szStr[20]		   = {0};
	const char cszCmpStr[] = "aa";

	char szName[2] = "";
	szName[0]	   = 'a';
	szName[1]	   = 'a';

	nRetCode = m_dropDownList->InsertItem(0, szName);
	CPPUNIT_ASSERT(nRetCode == 0);
	nRetCode = m_dropDownList->GetItemTextLength(0);
	CPPUNIT_ASSERT(nRetCode == 2);
	nRetCode = m_dropDownList->GetItemText(0, szStr, sizeof(szStr));
	CPPUNIT_ASSERT(nRetCode == 2);
	CPPUNIT_ASSERT(strcmp(szStr, cszCmpStr) == 0);
}
//////////////////////////////////////////////////////////////////////////
//@������� :1.11
//@����Ŀ�� :��֤���б�Ϊ�յ�ʱ����Բ�������
//@ǰ�ó��� :
//@C1       :�б�Ϊ�գ���AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :��������
//@���ó��� :
//@CP1      :���Բ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_11()
{
	int nRetCode		    = false;
	const char cszStrName[] = "item1";
	char szStr[10];

	nRetCode = m_dropDownList->AddItem(cszStrName);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 1);
	nRetCode = m_dropDownList->GetItemText(0, szStr, sizeof(szStr));
	CPPUNIT_ASSERT(nRetCode);
	CPPUNIT_ASSERT(strcmp(szStr, cszStrName) == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.12
//@����Ŀ�� :��֤�б���������¿��Բ�������
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :��������
//@���ó��� :
//@CP1      :���Բ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_12()
{
	int nRetCode		    = false;
	const char *cszStrName[] = {"item1", "item2"};
	char szStr[10];
    
	nRetCode = m_dropDownList->AddItem(cszStrName[0]);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->AddItem(cszStrName[1]);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 2);
	nRetCode = m_dropDownList->GetItemText(1, szStr, sizeof(szStr));
	CPPUNIT_ASSERT(nRetCode);
	CPPUNIT_ASSERT(strcmp(szStr, cszStrName[1]) == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.13
//@����Ŀ�� :��֤���Բ��������ַ�
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������ַ�
//@���ó��� :
//@CP1      :���Բ��������ַ�
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_13()
{
	int nRetCode			 = false;
	int i					 = 0;
	const char *cszStrName[] = {"item1", "item2", "item3", "���šơǡȡΡޡ�"};
	char szStr[20]			 = {0};  

	for (i = 0; i < 4; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
		if (i == 3)
		{
			nRetCode = m_dropDownList->GetItemCount();
			CPPUNIT_ASSERT(nRetCode == 4);
			nRetCode = m_dropDownList->GetItemText(3, szStr, sizeof(szStr));
			CPPUNIT_ASSERT(strcmp(szStr, cszStrName[3]) == 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.14
//@����Ŀ�� :��֤����ʱ�����ָ�벻�ܲ���ɹ�
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ʱ�����ָ��
//@���ó��� :
//@CP1      :����ʱ�����ָ�벻�ܲ���ɹ�
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_14()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char *szStr		   = NULL;

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}

	nRetCode = m_dropDownList->AddItem(szStr);
	CPPUNIT_ASSERT(!nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.15
//@����Ŀ�� :��֤�����Բ�����ַ���
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :������ַ���
//@���ó��� :
//@CP1      :�����Բ�����ַ���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_15()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char *szStr		   = "";

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}

	nRetCode = m_dropDownList->AddItem(szStr);
	CPPUNIT_ASSERT(!nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.16
//@����Ŀ�� :��֤�����Բ���ո�
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ո�
//@���ó��� :
//@CP1      :�����Բ���ո�
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_16()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char *szStr		   = " ";

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}

	nRetCode = m_dropDownList->AddItem(szStr);
	CPPUNIT_ASSERT(!nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.17
//@����Ŀ�� :��֤�����Բ��볬���ַ���
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���볬���ַ���
//@���ó��� :
//@CP1      :�����Բ��볬���ַ���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_17()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
	char szStr[]	   = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	char szOutStr[270];
	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}

	nRetCode = m_dropDownList->AddItem(szStr);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 4);
	nRetCode = m_dropDownList->GetItemTextLength(3);
	CPPUNIT_ASSERT(nRetCode == 260);
	nRetCode = m_dropDownList->GetItemText(3, szOutStr, sizeof(szOutStr));
	CPPUNIT_ASSERT(nRetCode);
	CPPUNIT_ASSERT(strcmp(szOutStr, szStr) == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.18
//@����Ŀ�� :��֤�����Բ����ظ�����
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :��������
//@���ó��� :
//@CP1      :���Բ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_18()
{
	int nRetCode		    = false;
	const char cszStrName[] = "item1";

	nRetCode = m_dropDownList->AddItem(cszStrName);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->AddItem(cszStrName);
	CPPUNIT_ASSERT(!nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 1);
}
//////////////////////////////////////////////////////////////////////////
//@������� :1.19
//@����Ŀ�� :��֤�����Բ���û�н��������ַ���
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����û�н��������ַ���
//@���ó��� :
//@CP1      :�����Բ���û�н��������ַ���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_19()
{
	int nRetCode		   = false;
	int i				   = 0;
	char szStr[20]		   = {0};
	const char cszCmpStr[] = "aa";

	char szName[2] = {0};
	szName[0]	   = 'a';
	szName[1]	   = 'a';

	nRetCode = m_dropDownList->AddItem(szName);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->GetItemTextLength(0);
	CPPUNIT_ASSERT(nRetCode == 2);
	nRetCode = m_dropDownList->GetItemText(0, szStr, sizeof(szStr));
	CPPUNIT_ASSERT(nRetCode == 2);
	CPPUNIT_ASSERT(strcmp(szStr, cszCmpStr) == 0);
}
//////////////////////////////////////////////////////////////////////////
//@������� :1.20
//@����Ŀ�� :��֤���뿪ͷ���м���β�пո���ַ�����ʱ��ʵ�ʲ����ַ����Ƿ�һ��
//@ǰ�ó��� :
//@C1       :�б���������InsertItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���뿪ͷ���м���β�пո���ַ���
//@���ó��� :
//@CP1      :���ʵ�ʲ����ַ����Ƿ�һ��
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_20()
{
	int nRetCode = false;
	int i = 0;
	char *cszStrName[] = {"item1", "item2", "item3", "  item4", "item    5", "item6    "};
	char szStr[20];

    for (i = 0; i < 6; i++)
	{
		nRetCode = m_dropDownList->InsertItem(i, cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == i);
	}
    nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 6);
	for (i = 3; i < 6; i++)
	{
		nRetCode = m_dropDownList->GetItemText(i, szStr, sizeof(szStr));
		CPPUNIT_ASSERT(nRetCode);
		nRetCode = strcmp(szStr,cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
//@������� :1.21
//@����Ŀ�� :��֤���뿪ͷ���м���β�пո���ַ�����ʱ��ʵ�ʲ����ַ����Ƿ�һ��
//@ǰ�ó��� :
//@C1       :�б���������AddItem����
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���뿪ͷ���м���β�пո���ַ���
//@���ó��� :
//@CP1      :���ʵ�ʲ����ַ����Ƿ�һ��
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_InsertItem_21()
{
	int nRetCode = false;
	int i = 0;
	char *cszStrName[] = {"item1", "item2", "item3", "  item4", "item    5", "item6    "};
	char szStr[20];

	for (i = 0; i < 6; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 6);
	for (i = 3; i < 6; i++)
	{
		nRetCode = m_dropDownList->GetItemText(i, szStr, sizeof(szStr));
		CPPUNIT_ASSERT(nRetCode);
		nRetCode = strcmp(szStr,cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
//@������� :2.1
//@����Ŀ�� :��֤���б���ɾ����
//@ǰ�ó��� :
//@C1       :�б�Ϊ�գ���DeleteItemɾ��
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :ɾ���б���
//@���ó��� :
//@CP1      :������ɾ���б���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_DeleteItem_1()
{
 int nRetCode = true;
 
 nRetCode = m_dropDownList->DeleteItem(2);
 CPPUNIT_ASSERT(!nRetCode);
}

//////////////////////////////////////////////////////////////////////////
//@������� :2.2
//@����Ŀ�� :��֤����ɾ����ͷ�����У���β����
//@ǰ�ó��� :
//@C1       :�б���������DeleteItemɾ��
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :ɾ����ͷ�����У���β����
//@���ó��� :
//@CP1      :����ɾ����ͷ�����У���β����
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_DeleteItem_2()
{
	int nRetCode				 = false;
	int i						 = 0;
	char *cszStrName[]			 = {"item1", "item2", "item3", "item4", "item5", "item6"};
	const char *cszStrNameRest[] = {"item2", "item3", "item5"};
	char szstr[10];

	for (i = 0; i < 6; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}
	//ɾ����ͷ��
	nRetCode = m_dropDownList->DeleteItem(0);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 5);
	//ɾ��������
    nRetCode = m_dropDownList->DeleteItem(2);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 4);
	//ɾ����β��
	nRetCode = m_dropDownList->DeleteItem(3);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->GetItemText(i, szstr, sizeof(szstr));
		CPPUNIT_ASSERT(nRetCode);
		CPPUNIT_ASSERT(strcmp(szstr, cszStrNameRest[i]) == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
//@������� :2.3
//@����Ŀ�� :��֤������ɾ�������ڵ���
//@ǰ�ó��� :
//@C1       :�б���������DeleteItemɾ��
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :ɾ�������ڵ���
//@���ó��� :
//@CP1      :������ɾ�������ڵ���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_DeleteItem_3()
{
	int nRetCode	   = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};
    
	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}
	nRetCode = m_dropDownList->DeleteItem(5);
	CPPUNIT_ASSERT(!nRetCode);
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
}

//////////////////////////////////////////////////////////////////////////
//@������� :2.4
//@����Ŀ�� :��֤��ǰ�б�Ϊ�յ�����²���ɾ��
//@ǰ�ó��� :
//@C1       :�б�Ϊ�գ���DeleteAllItemɾ��
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :ɾ���б�������
//@���ó��� :
//@CP1      :����ɾ��
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_DeleteItem_4()
{
	int nRetCode = true;

	m_dropDownList->DeleteAllItem();
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :2.5
//@����Ŀ�� :��֤�б���������¿���ɾ��������
//@ǰ�ó��� :
//@C1       :�б���������DeleteAllItemɾ��
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :ɾ���б�������
//@���ó��� :
//@CP1      :����ɾ���б����
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_DeleteItem_5()
{
	int nRetCode       = false;
	int i			   = 0;
	char *cszStrName[] = {"item1", "item2", "item3"};

	for (i = 0; i < 3; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszStrName[i]);
		CPPUNIT_ASSERT(nRetCode);
	}
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 3);
	m_dropDownList->DeleteAllItem();
	nRetCode = m_dropDownList->GetItemCount();
	CPPUNIT_ASSERT(nRetCode == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :3.1
//@����Ŀ�� :��֤�����б�Ϊ�յ���������õ�ǰѡ����
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���õ�ǰѡ����
//@���ó��� :
//@CP1      :�������õ�ǰѡ����
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SelectItem_1()
{
	int nRetCode = false;

	nRetCode = m_dropDownList->SelectItem(1);
	CPPUNIT_ASSERT(nRetCode == -1);
}

//////////////////////////////////////////////////////////////////////////
//@������� :3.2 
//@����Ŀ�� :��֤�ܹ�����������������õ�ǰѡ����
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���õ�ǰѡ����
//@���ó��� :
//@CP1      :�������õ�ǰѡ����
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SelectItem_2()
{
    int i					  = 0;
    int nRetCode			  = false;
    const char *cszitemname[] = {"item1", "item2", "item3"};

    for (i = 0; i < 3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    nRetCode = m_dropDownList->SelectItem(1);
    CPPUNIT_ASSERT(nRetCode == 1);
    nRetCode = m_dropDownList->GetSelectedItemIndex();
    CPPUNIT_ASSERT(nRetCode == 1);
}



//////////////////////////////////////////////////////////////////////////
//@������� :3.3
//@����Ŀ�� :��֤�������õ�ǰѡ����Ϊ�������б���
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���õ�ǰѡ����Ϊ�������б���
//@���ó��� :
//@CP1      :�������õ�ǰѡ����Ϊ�������б���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SelectItem_3()
{
    int i					  = 0;
    int nRetCode			  = false;
    const char *cszitemname[] = {"item1", "item2", "item3"};

    for (i = 0; i< 3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    nRetCode = m_dropDownList->SelectItem(5);
    CPPUNIT_ASSERT(nRetCode == -1);
	nRetCode = m_dropDownList->SelectItem(-1);
	CPPUNIT_ASSERT(nRetCode == -1);
	nRetCode = m_dropDownList->SelectItem(-10);
	CPPUNIT_ASSERT(nRetCode == -1);
}

//////////////////////////////////////////////////////////////////////////
//@������� :3.4
//@����Ŀ�� :��֤���������Ѿ����ù�����
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����λ�ú��ϴε��ظ�
//@���ó��� :
//@CP1      :���������Ѿ����ù�����
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SelectItem_4()
{
    int i					  = 0;
    int nRetCode			  = false;
    const char *cszItemName[] = {"item1", "item2", "item3"};

    for (i = 0; i < 3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszItemName[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    nRetCode = m_dropDownList->SelectItem(1);
    CPPUNIT_ASSERT(nRetCode == 1);
    nRetCode = m_dropDownList->SelectItem(1);
    CPPUNIT_ASSERT(nRetCode == 1);
}

//////////////////////////////////////////////////////////////////////////
//@������� :4.1 
//@����Ŀ�� :��֤�������б�Ϊ��ʱ����ָ���ַ�������
//@ǰ�ó��� :
//@C1       :�б�Ϊ�յ�Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ָ���ַ�������
//@���ó��� :
//@CP1      :��������ָ���ַ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SearchItem_1()
{
	int nRetCode                    = false;
	const char cszsearch[]			= "item2";	

	nRetCode = m_dropDownList->SearchItem(cszsearch, 0);
	CPPUNIT_ASSERT(nRetCode == -1);
}
//////////////////////////////////////////////////////////////////////////
//@������� :4.2 
//@����Ŀ�� :��֤�ܹ������������������ָ���ַ�������
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ָ���ַ�������
//@���ó��� :
//@CP1      :��������ָ���ַ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SearchItem_2()
{
    int i                     = 0;
    int nRetCode              = false;
    char cszout[10]           = {0};
    const char *cszitemname[] = {"item1", "item2", "item3"};	

    for (i = 0; i < 3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    nRetCode = m_dropDownList->SearchItem(cszitemname[1], 0);
    CPPUNIT_ASSERT(nRetCode == 1);
    nRetCode = m_dropDownList->GetItemText(nRetCode, cszout, sizeof(cszout));
    CPPUNIT_ASSERT(nRetCode);
    CPPUNIT_ASSERT(strcmp(cszout, cszitemname[1]) == 0);
}


//////////////////////////////////////////////////////////////////////////
//@������� :4.3
//@����Ŀ�� :���Բ�������û��ƥ����ַ������λ�ò��ԣ�û�и��ַ�����
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ָ���ַ�������
//@���ó��� :
//@CP1      :��������ָ���ַ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SearchItem_3()
{
    int i					   = false;
    int nRetCode               = false;
    char cszout[10]			   = {0};
    const char *cszsearch[]    ={"item1", "item4"};
    const char *cszitemname[ ] ={"item1", "item2", "item3"};	

    for (i = 0; i < 3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    //λ�ò���
    nRetCode = m_dropDownList->SearchItem(cszsearch[0], 1);
    CPPUNIT_ASSERT(nRetCode == -1);
    //û�ж�Ӧ����
    nRetCode = m_dropDownList->SearchItem(cszsearch[1], 0);
    CPPUNIT_ASSERT(nRetCode == -1);
}

//////////////////////////////////////////////////////////////////////////
//@������� :4.4
//@����Ŀ�� :������������ܴ����쳣�ַ���
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :�����쳣�ַ�������
//@���ó��� :
//@CP1      :���������쳣�ַ�������
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SearchItem_4()
{
    int i                     = 0;
    int nRetCode              = false;
    char cszout[10]           = {0};
    const char *cszsearch[]   = {
                                    NULL, 
                                    "",
                                    " ",
                                    "���šơǡȡΡޡ�",
                                    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                                    };
    const char *cszitemname[] = {"item1", "item2", "item3"};	

    for (i = 0; i < 3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    for (i = 0; i < 5; i++)
    {
        nRetCode = m_dropDownList->SearchItem(cszsearch[i], 0);
        CPPUNIT_ASSERT(nRetCode == -1);
    }
}

//////////////////////////////////////////////////////////////////////////
//@������� :4.5
//@����Ŀ�� :�����б������ַ���ǰ׺�ظ����������µ�����
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :����ָ���ַ�������
//@���ó��� :
//@CP1      :����������Ψһƥ��������б���
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SearchItem_5()
{
    int i                     = 0;
    int nRetCode              = false;
    char cszout[300]          = {0};
    const char *cszitemname[] = {"item1", "item2", "item113"};	
   
    for (i = 0; i < 3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
	nRetCode = m_dropDownList->SearchItem(cszitemname[0], 0);
	CPPUNIT_ASSERT(nRetCode == 0);
	nRetCode = m_dropDownList->GetItemText(nRetCode, cszout, sizeof(cszout));
	CPPUNIT_ASSERT(nRetCode);
	CPPUNIT_ASSERT(strcmp(cszout, cszitemname[0]) == 0);

    nRetCode = m_dropDownList->SearchItem(cszitemname[2], 0);
    CPPUNIT_ASSERT(nRetCode == 2);
    nRetCode = m_dropDownList->GetItemText(nRetCode, cszout, sizeof(cszout));
    CPPUNIT_ASSERT(nRetCode);
    CPPUNIT_ASSERT(strcmp(cszout, cszitemname[2]) == 0);
}

//////////////////////////////////////////////////////////////////////////
//@������� :5.1 
//@����Ŀ�� :��֤�ܹ����б�Ϊ�յ���������������С
//@ǰ�ó��� :
//@C1       :�б�Ϊ�յ�Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������С
//@���ó��� :
//@CP1      :�������������С
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetFontSize_1()
{
	int nRetCode			  = false;
	int dwSize				  = 0;
	const int cszsize		  = 14;

	m_dropDownList->SetFontSize(cszsize);
	dwSize = m_dropDownList->GetFontSize();
	CPPUNIT_ASSERT_EQUAL(dwSize, cszsize);
}
//////////////////////////////////////////////////////////////////////////
//@������� :5.2 
//@����Ŀ�� :��֤�ܹ�����������������������С
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������С
//@���ó��� :
//@CP1      :�������������С
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetFontSize_2()
{
    int i                     = 0;
    int KWndDropDownLi        = 0;
    int nRetCode              = false;
    int dwSize                = 0;
    const int cszsize         = 12;
    const char *cszitemname[] = {"item1", "item2", "item3"};

    for (i = 0; i<3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    m_dropDownList->SetFontSize(cszsize);
    dwSize = m_dropDownList->GetFontSize();
    CPPUNIT_ASSERT_EQUAL(dwSize, cszsize);
}


//////////////////////////////////////////////////////////////////////////
//@������� :6.1 
//@����Ŀ�� :��֤�ܹ����б�Ϊ�������������ʾ������ɫ
//@ǰ�ó��� :
//@C1       :�б�Ϊ�յ�Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :������ʾ������ɫ
//@���ó��� :
//@CP1      :����������ʾ������ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetTipColor_1()
{
	int nRetCode         = false;
	DWORD dwColor        = 0;
	const DWORD cszcolor = 155;   

	m_dropDownList->SetTipColor(cszcolor);
	dwColor = m_dropDownList->GetTipColor();
	CPPUNIT_ASSERT_EQUAL(dwColor, cszcolor);
}
//////////////////////////////////////////////////////////////////////////
//@������� :6.2 
//@����Ŀ�� :��֤�ܹ������������������ʾ������ɫ�����������ʾ��
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :������ʾ������ɫ
//@���ó��� :
//@CP1      :����������ʾ������ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetTipColor_2()
{
    int i                     = 0;
    int nRetCode              = false;
    DWORD dwColor             = 0;
    const DWORD cszcolor      = 255;
    const char *cszitemname[] = {"item1", "item2", "item3"};

    for (i = 0; i<3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    m_dropDownList->SetTipColor(cszcolor);
    dwColor = m_dropDownList->GetTipColor();
    CPPUNIT_ASSERT_EQUAL(dwColor, cszcolor);
}

//////////////////////////////////////////////////////////////////////////
//@������� :7.1
//@����Ŀ�� :��֤�ܹ����б���������������б�����ɫ
//@ǰ�ó��� :
//@C1       :�б�Ϊ�յ�Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������б�����ɫ
//@���ó��� :
//@CP1      :�������������б�����ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetListBGColor_1()
{
	int nRetCode	    	= false;
	DWORD dwColor	    	= 0;
	const DWORD cszcolor    = 123;

	m_dropDownList->SetListBGColor(cszcolor);
	dwColor = m_dropDownList->GetListBGColor();
	CPPUNIT_ASSERT_EQUAL(dwColor, cszcolor);
}
//////////////////////////////////////////////////////////////////////////
//@������� :7.2 
//@����Ŀ�� :��֤�ܹ�������������������б�����ɫ
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������б�����ɫ
//@���ó��� :
//@CP1      :�������������б�����ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetListBGColor_2()
{
    int i					  = 0;
    int nRetCode			  = false;
    DWORD dwColor			  = 0;
    const DWORD cszcolor	  = 451;
    const char *cszitemname[] = {"item1", "item2", "item3"};

    for (i = 0; i<3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    m_dropDownList->SetListBGColor(cszcolor);
    dwColor = m_dropDownList->GetListBGColor();
    CPPUNIT_ASSERT_EQUAL(dwColor, cszcolor);
}


//////////////////////////////////////////////////////////////////////////
//@������� :8.1
//@����Ŀ�� :��֤�ܹ����б�Ϊ�յ���������÷ָ�����ɫ
//@ǰ�ó��� :
//@C1       :�б�Ϊ�յ�Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���÷ָ�����ɫ
//@���ó��� :
//@CP1      :�������÷ָ�����ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetListSLColor_1()
{
	int nRetCode		 = false;
	DWORD dwColor		 = 0;
	const DWORD cszcolor = 233;

	m_dropDownList->SetListSLColor(cszcolor);
	dwColor = m_dropDownList->GetListSLColor();
	CPPUNIT_ASSERT(dwColor == cszcolor);
}
//////////////////////////////////////////////////////////////////////////
//@������� :8.2 
//@����Ŀ�� :��֤�ܹ������б���ͷָ��ߵ���������÷ָ�����ɫ
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���÷ָ�����ɫ
//@���ó��� :
//@CP1      :�������÷ָ�����ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetListSLColor_2()
{	
	int nRetCode			  = false;
	int i                     = 0;
	DWORD dwColor		      = 0;
	const DWORD cszcolor	  = 233;
	const char *cszitemname[] = {"item1", "item2", "item3"};

	for (i = 0; i<3; i++)
	{
		nRetCode = m_dropDownList->AddItem(cszitemname[i]);
		CPPUNIT_ASSERT(nRetCode);
	}
	m_dropDownList->SetListSLColor(cszcolor);
	dwColor = m_dropDownList->GetListSLColor();
	CPPUNIT_ASSERT(dwColor == cszcolor);
}


//////////////////////////////////////////////////////////////////////////
//@������� :9.1
//@����Ŀ�� :��֤�ܹ����б�Ϊ����������������б�߿���ɫ
//@ǰ�ó��� :
//@C1       :�б�Ϊ�յ�Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������б�߿���ɫ
//@���ó��� :
//@CP1      :�������������б�߿���ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetListFLColor_1()
{
	int nRetCode         = false;
	DWORD dwColor        = 0;
	const DWORD cszcolor =125;

	m_dropDownList->SetListFLColor(cszcolor);
	dwColor = m_dropDownList->GetListFLColor();
	CPPUNIT_ASSERT(dwColor == cszcolor);
}

//////////////////////////////////////////////////////////////////////////
//@������� :9.2 
//@����Ŀ�� :��֤�ܹ����б�������������������б�߿���ɫ
//@ǰ�ó��� :
//@C1       :�б�������Dropdownlist
//@����Դ   :Dropdownlist
//@�������� :
//@�������� :
//@OP1      :���������б�߿���ɫ
//@���ó��� :
//@CP1      :�������������б�߿���ɫ
//////////////////////////////////////////////////////////////////////////
void WndDropDownList_Test::Test_SetListFLColor_2()
{
    int i                     = 0;
    int nRetCode              = false;
    DWORD dwColor             = 0;
    const DWORD cszcolor      = 255;
    const char *cszitemname[] = {"item1", "item2", "item3"};

    for(i = 0; i<3; i++)
    {
        nRetCode = m_dropDownList->AddItem(cszitemname[i]);
        CPPUNIT_ASSERT(nRetCode);
    }
    m_dropDownList->SetListFLColor(cszcolor);
    dwColor = m_dropDownList->GetListFLColor();
    CPPUNIT_ASSERT(dwColor == cszcolor);
}





//void WndDropDownList_Test::DummyClassForFirstCopy(void* pvClassBeingTest)
//{
//    _testdata.m_bEditable     = ((UI::KWndDropDownList*)pvClassBeingTest)->m_bEditable;
//    _testdata.m_bEnableTip    = ((UI::KWndDropDownList*)pvClassBeingTest)->m_bEnableTip;
//    _testdata.m_bListExtend   = ((UI::KWndDropDownList*)pvClassBeingTest)->m_bListExtend;
//    _testdata.m_dwListBGColor = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwListBGColor;
//    _testdata.m_dwListFLColor = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwListFLColor;
//    _testdata.m_dwListSLColor = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwListSLColor;
//    _testdata.m_dwTipColor    = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwTipColor;
//    _testdata.m_nFontSize     = ((UI::KWndDropDownList*)pvClassBeingTest)->m_nFontSize;
//    _testdata.m_nMaxItemNum   = ((UI::KWndDropDownList*)pvClassBeingTest)->m_nMaxItemNum;
//    //_testdata.m_szTipInfo     = ((UI::KWndDropDownList*)pvClassBeingTest)->m_szTipInfo;
//    SetFirstCopy( &_testdata, sizeof( _TestData ));
//}
//void WndDropDownList_Test::DummyClassForSecondCopy(void* pvClassBeingTest)
//{
//    _testdata.m_bEditable     = ((UI::KWndDropDownList*)pvClassBeingTest)->m_bEditable;
//    _testdata.m_bEnableTip    = ((UI::KWndDropDownList*)pvClassBeingTest)->m_bEnableTip;
//    _testdata.m_bListExtend   = ((UI::KWndDropDownList*)pvClassBeingTest)->m_bListExtend;
//    _testdata.m_dwListBGColor = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwListBGColor;
//    _testdata.m_dwListFLColor = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwListFLColor;
//    _testdata.m_dwListSLColor = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwListSLColor;
//    _testdata.m_dwTipColor    = ((UI::KWndDropDownList*)pvClassBeingTest)->m_dwTipColor;
//    _testdata.m_nFontSize     = ((UI::KWndDropDownList*)pvClassBeingTest)->m_nFontSize;
//    _testdata.m_nMaxItemNum   = ((UI::KWndDropDownList*)pvClassBeingTest)->m_nMaxItemNum;
//    //_testdata.m_szTipInfo     = ((UI::KWndDropDownList*)pvClassBeingTest)->m_szTipInfo;
//    SetSecondCopy( &_testdata, sizeof( _TestData ));
//}
//void WndDropDownList_Test::DummyClassToFile( LPCSTR szFilename)
//{
//}
//void WndDropDownList_Test::CompareCopy(bool bAutoReleaseCopy)
//{
//    if( NULL != _FirstCopy && NULL != _SecondCopy)
//    {
//        _TestData* pFirstTestdata  = (_TestData*) _FirstCopy;
//        _TestData* pSecondTestData = (_TestData*) _SecondCopy;
//
//        CPPUNIT_ASSERT(pFirstTestdata->m_bEditable     == pSecondTestData->m_bEditable);
//        CPPUNIT_ASSERT(pFirstTestdata->m_bEnableTip    == pSecondTestData->m_bEnableTip);
//        CPPUNIT_ASSERT(pFirstTestdata->m_bListExtend   == pSecondTestData->m_bListExtend);
//        CPPUNIT_ASSERT(pFirstTestdata->m_dwListBGColor == pSecondTestData->m_dwListBGColor);
//        CPPUNIT_ASSERT(pFirstTestdata->m_dwListFLColor == pSecondTestData->m_dwListFLColor);
//        CPPUNIT_ASSERT(pFirstTestdata->m_dwListSLColor == pSecondTestData->m_dwListSLColor);
//        CPPUNIT_ASSERT(pFirstTestdata->m_dwTipColor    == pSecondTestData->m_dwTipColor);
//        CPPUNIT_ASSERT(pFirstTestdata->m_nFontSize     == pSecondTestData->m_nFontSize);
//        CPPUNIT_ASSERT(pFirstTestdata->m_nMaxItemNum   == pSecondTestData->m_nMaxItemNum);
//        CPPUNIT_ASSERT(pFirstTestdata->m_szTipInfo     == pSecondTestData->m_szTipInfo);
//        if( bAutoReleaseCopy)
//            ReleaseAllCopy();
//    }
//}
#endif //End of #ifdef _UNIT_TEST