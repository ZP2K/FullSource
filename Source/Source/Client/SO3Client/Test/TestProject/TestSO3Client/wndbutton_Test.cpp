
#include "stdafx.h"

#ifdef _UNIT_TEST

#include <vector>
#include <string>
#include "wndbutton_test.h"
#include "../ui/elem/wndbutton.h"
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(KWndButton_Test);


KWndButton_Test::KWndButton_Test()
{

}

KWndButton_Test::~KWndButton_Test()
{

}


void KWndButton_Test::setUp()
{


}

void KWndButton_Test::tearDown()
{

}

//////////////////////////////////////////////////////////////////////////
//@������� : 1
//@ǰ�ó��� : KWndButtonû���κ���Ϣ
//@C1:
//@C2:
//@����Դ   : ���Դ���
//@�������� : KWndButton��
//@�������� :
//@OP1: �½�һ��KWndButtonʵ��
//@OP2: ����KWndButton.SetToolTipInfo()
//@���ó��� :
//@CP1: �����ָ��
//@CP2: ����һ���ַ�'a'
//@CP3: ����4��ANSI�ַ�
//@CP4: �������� 2���ֽ�
//@CP5: �������� �� ����  20���ֽ�
//@CP6: ���볤ANSI�ַ��� 256���ַ�'a'
//////////////////////////////////////////////////////////////////////////

void KWndButton_Test::SetToolTipInfo_test(void)
{
	int nRetCode = 0;
	string str_ANSITest(256, 'a');   //256���ַ�'a'
	vector<string> cs(100);
	//1��ANSI�ַ�
	cs[0] = " ";
	//4��ANSI�ַ�
	cs[1] = "   1";    
	//һ�������ַ�
	cs[2] = "��";   
	//������� ����:20
	cs[3] = "���ġ�����123456";  
	//��ANSI�ַ��� ����:256 ���ַ�'a'
	cs[4] = str_ANSITest;
	

	//δ��鴫���ָ��
	//KWndButton m_KWndButton;
	//nRetCode = m_KWndButton.SetToolTipInfo(NULL, 20);
	//CPPUNIT_ASSERT(nRetCode);

	for (int i=0; i<5; i++)   //���齫SetToolTipInfo�ĵ�һ��������Ϊconst char*
	{
		KWndButton m_KWndButton2;
	
		const char *data = cs[i].c_str();
		int nRetCode = m_KWndButton2.SetToolTipInfo(const_cast<char *>(data),
												(int)cs[i].size());	

		//ClassDataCopyBuffer copy;
		//copy.SetFirstCopy(reinterpret_cast<void *>(&m_KWndButton2), sizeof(m_KWndButton2));
		//SWndButton_Data *s_data = reinterpret_cast<SWndButton_Data *>(copy.GetFirstCopy());

		//SWndButton_Data *s_data = 
		//	reinterpret_cast<SWndButton_Data *>
		//	(CopyClassData(reinterpret_cast<void *>(&m_KWndButton2), sizeof(m_KWndButton2)));
		char *p = m_KWndButton2.m_szTip;
		CPPUNIT_ASSERT(strncmp(cs[i].c_str(), p, nRetCode) == 0);
	}
}


//////////////////////////////////////////////////////////////////////////
//@������� : 1
//@ǰ�ó��� : ����KWndButton��Tip
//@C1:
//@C2:
//@����Դ   : ���Դ���
//@�������� : KWndButton��
//@�������� :
//@OP1: �½�һ��KWndButtonʵ��
//@OP2: ����KWndButton.GetToolTipInfo()
//@���ó��� :GetToolTipInfo��������Ϣ�洢�ڻ�����
//@CP1: ������ΪNULL
//@CP2:����1���ֽڵĻ�����
//@CP3:����63���ֽڵĻ�����
//@CP4:����256���ֽڵĻ�����
//////////////////////////////////////////////////////////////////////////

void KWndButton_Test::GetToolTipInfo_Test(void)
{
	KWndButton m_KWndButton;
	char *szBuffer = NULL;
	char szBuffer_1[1];
	char szBuffer_2[63];
	char szBuffer_3[256];
	
	m_KWndButton.SetToolTipInfo("l.l.l", 4);
	//ClassDataCopyBuffer copy;
	//copy.SetFirstCopy(reinterpret_cast<void *>(&m_KWndButton), sizeof(KWndButton));
	//SWndButton_Data *s_data = reinterpret_cast<SWndButton_Data *>(copy.GetFirstCopy());

	//SWndButton_Data *s_data = reinterpret_cast<SWndButton_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndButton), sizeof(m_KWndButton)));
	char *p = m_KWndButton.m_szTip;

	//δ������ΪNULL
	int retCode = -1;
	//retCode = m_KWndButton.GetToolTipInfo(szBuffer, 4);
    //CPPUNIT_ASSERT
	//����1���ֽڵĻ����� �ַ������δ��ֵΪ'\0'
	retCode = m_KWndButton.GetToolTipInfo(szBuffer_1, 1);
	CPPUNIT_ASSERT(strncmp(szBuffer_1, p, retCode) == 0);
	//����63���ֽڵĻ����� �ַ������δ��ֵΪ'\0'
	retCode = m_KWndButton.GetToolTipInfo(szBuffer_2, 63);
	CPPUNIT_ASSERT(strncmp(szBuffer_2, p, retCode) == 0);
	//����256���ֽڵĻ�����
	//�ַ������δ��ֵΪ'\0'
	retCode = m_KWndButton.GetToolTipInfo(szBuffer_3, 256);
	CPPUNIT_ASSERT(strncmp(szBuffer_3, p, retCode) == 0);
}

#endif
