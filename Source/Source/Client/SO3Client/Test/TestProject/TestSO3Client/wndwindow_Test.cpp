

#include "stdafx.h"

#ifdef _UNIT_TEST

#include <vector>
#include <string>
#include "wndwindow_Test.h"
#include "../ui/elem/wndwindow.h"

using namespace std;
CPPUNIT_TEST_SUITE_REGISTRATION(KWndWindow_Test);


KWndWindow_Test::KWndWindow_Test()
{

}

KWndWindow_Test::~KWndWindow_Test()
{

}


void KWndWindow_Test::setUp()
{


}

void KWndWindow_Test::tearDown()
{

}


//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :�����Ӵ���

//@ǰ�ó��� :m_KWndWindow���κδ��ڶ��޹���, 4��ָ�붼Ϊ��ָ��	
//@C1       : 
//@C2       :
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :�����Ӵ���, ˫������
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      :�����ָ��
//@CP2      :��������100�����κδ����޹������Ӵ���
//////////////////////////////////////////////////////////////////////////

void KWndWindow_Test::AddChild_Test(void)
{
	//����NULL
	KWndWindow m_KWndWindow; //
	m_KWndWindow.AddChild(NULL);
	//SWndWindow_Data *s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndWindow), sizeof(KWndWindow)));
	//û���Ӵ��� ����ֵ��NULL
	CPPUNIT_ASSERT(m_KWndWindow.GetFirstChild() == NULL);

	//����100���Ӵ���
	//����100����Ч��KWndWindowָ��
	KWndWindow window[100];
	for (int i=0; i<100; i++)
	{
		m_KWndWindow.AddChild(&window[i]);	
	}

	//SWndWindow_Data *s_data2 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndWindow), sizeof(KWndWindow)));

	CPPUNIT_ASSERT(m_KWndWindow.GetFirstChild() == &window[0]);
	KWndWindow *ChildWindow = m_KWndWindow.GetFirstChild();
	
	//for (int i=1; i<100; i++)
	//{
	//	CPPUNIT_ASSERT(ChildWindow->m_pNextWnd == &window[i]);
	//	ChildWindow = (SWndWindow_Data *)ChildWindow->m_pNextWnd;
	//}
	//CPPUNIT_ASSERT(ChildWindow->m_pNextWnd == NULL);

	int j = 1;
	ChildWindow = m_KWndWindow.GetFirstChild();
	while (ChildWindow->GetNextWnd() != NULL)
	{
		//������ӵ�100���Ӵ���˳���Ƿ���ȷ
		//ÿ�������ӵ��Ӵ�����˫������ ��β
		CPPUNIT_ASSERT(ChildWindow->GetNextWnd() == &window[j]);  
		//���ÿ�������ӵ��Ӵ��ڵĸ�����
		CPPUNIT_ASSERT(ChildWindow->GetParent() == &m_KWndWindow); 
		ChildWindow = ChildWindow->GetNextWnd();
		j++;
	}

	//����Ƿ�100���Ӵ���
	CPPUNIT_ASSERT(j == 100);  
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :�����ֵܴ���

//@ǰ�ó��� :m_KWndWindow���κδ��ڶ��޹���, 4��ָ�붼Ϊ��ָ��	
//@C1       : 
//@C2       :
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :�����ֵܴ���, ˫������
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      :�����ָ��
//@CP2      :�޸����� ��������100�����κδ����޹������ֵܴ���
//@CP3      :�и����� ��������100���ֵܴ���(��������������ϵ)
//////////////////////////////////////////////////////////////////////////
void KWndWindow_Test::AddBrother_Test(void)
{
	//����NULL
	KWndWindow m_KWndWindow;
	m_KWndWindow.AddBrother(NULL);
	CPPUNIT_ASSERT(m_KWndWindow.GetNextWnd() == NULL);


	//�޸����� ����100����Ч��KWndWindowָ��
	KWndWindow window[100];
	for (int i=0; i<100; i++)
	{
		m_KWndWindow.AddBrother(&window[i]);
	}

	//s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndWindow), sizeof(KWndWindow)));
	int j = 0;
	KWndWindow *BroWindow = &m_KWndWindow;
	while (BroWindow->GetNextWnd() != NULL)
	{
		//ÿ�������ӵ��ֵܴ�����˫������ ��β
		//������ӵ�100���ֵܴ���˳���Ƿ���ȷ �����ӵĴ����ڱ�β��
		CPPUNIT_ASSERT(BroWindow->GetNextWnd() == &window[j]);  
		BroWindow = BroWindow->GetNextWnd();
		j++;
	}
	CPPUNIT_ASSERT(j == 100);  //�Ƿ�Ϊ100���ֵܴ���
	

	//�и����� ��������100���ֵܴ���(��Щ���ڶ����ֵܴ���)
	KWndWindow MyParent;
	KWndWindow MyChild;
	MyParent.AddChild(&MyChild);
	for (int i=0; i<100; i++)
	{
		MyChild.AddBrother(&window[i]);
	}	
	//s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&MyChild), sizeof(KWndWindow)));	
	j = 0;
	BroWindow = &MyChild;
	//��һ���Ӵ��ڵĸ���������ȷ����
	CPPUNIT_ASSERT(BroWindow->GetParent() == &MyParent);  
	while (BroWindow->GetNextWnd() != NULL)
	{
		//ÿ�������ӵ��ֵܴ�����˫������ ��β
		//������ӵ�100���ֵܴ���˳���Ƿ���ȷ �����ӵĴ����ڱ�β��
		CPPUNIT_ASSERT(BroWindow->GetNextWnd() == &window[j]);  
		CPPUNIT_ASSERT(BroWindow->GetParent() == &MyParent);  
		BroWindow = BroWindow->GetNextWnd();
		j++;
	}
	CPPUNIT_ASSERT(j == 100);  //�Ƿ�Ϊ100���ֵܴ���

}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :�Ѵ����ƶ�����ǰ��
//@ǰ�ó��� : 
//@C1        :�������������κ���ϵ
//@C2        :��m_KWndWindow����100���ֵܴ���,
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      :���κ��ֵܴ��� ��m_KWndWindow�����ƶ�����ǰ��
//@CP2      :100���ֵܴ��� ��m_KWndWindow�����ƶ�����ǰ��
//@CP3      :100���ֵܴ��� ��Window[1]�����ƶ�����ǰ��
//@CP4      :100���ֵܴ��� ��Window[50]�����ƶ�����ǰ��
//@CP5      :100���ֵܴ��� ��Window[99]�����ƶ�����ǰ��
//////////////////////////////////////////////////////////////////////////

void KWndWindow_Test::BringToTop_Test()
{
	//���κ��ֵܴ��� ��m_KWndWindow�Ѵ����ƶ�����ǰ��
	KWndWindow m_KWndWindow;
	m_KWndWindow.BringToTop();
	KWndWindow *BroWindow = &m_KWndWindow;
	while (BroWindow->GetNextWnd() != NULL)
	{
		BroWindow = BroWindow->GetNextWnd();
	}
	//��m_KWndWindow�Ѵ����ƶ�����ǰ��
	CPPUNIT_ASSERT(BroWindow == &m_KWndWindow);  


	//����100���ֵܴ���
	KWndWindow window[100];
	for (int i=0; i<100; i++)
	{
		m_KWndWindow.AddBrother(&window[i]);
	}
	
	//SWndWindow_Data *s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndWindow), sizeof(KWndWindow)));
	//��m_KWndWindow�Ѵ����ƶ�����ǰ��
	m_KWndWindow.BringToTop();
	BroWindow = &m_KWndWindow;
	while (BroWindow->GetNextWnd() != NULL)
	{
		BroWindow = BroWindow->GetNextWnd();
	}
	//��ǰ��Ĵ�����m_KWndWindow��
	CPPUNIT_ASSERT(BroWindow == &m_KWndWindow);  

	//s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&window[1]), sizeof(KWndWindow)));
	//��window[1]�Ѵ����ƶ�����ǰ��
	window[1].BringToTop();   
	BroWindow = &window[1];
	while (BroWindow->GetNextWnd() != NULL)
	{
		BroWindow = BroWindow->GetNextWnd();
	}
	//��ǰ��Ĵ�����window[1]��
	CPPUNIT_ASSERT(BroWindow == &window[1]);   


	//s_data1 = reinterpret_cast<SWndWindow_Data *>
			//(CopyClassData(reinterpret_cast<void *>(&window[50]), sizeof(KWndWindow)));
	//��window[50]�Ѵ����ƶ�����ǰ��
	window[50].BringToTop();  
	BroWindow = &window[50];
	while (BroWindow->GetNextWnd() != NULL)
	{
		BroWindow = BroWindow->GetNextWnd();
	}
	//��ǰ��Ĵ�����window[50]��
	CPPUNIT_ASSERT(BroWindow == &window[50]);  

	//s_data1 = reinterpret_cast<SWndWindow_Data *>
			//(CopyClassData(reinterpret_cast<void *>(&window[99]), sizeof(KWndWindow)));

	//��window[99]�Ѵ����ƶ�����ǰ��
	window[99].BringToTop();  
	BroWindow = &window[99];
	while (BroWindow->GetNextWnd()!= NULL)
	{
		BroWindow = BroWindow->GetNextWnd();
	}
	//��ǰ��Ĵ�����window[99]��
	CPPUNIT_ASSERT(BroWindow == &window[99]);  
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :���ô���λ�ã����Ͻ�����ڸ��������Ͻ�λ�õ�λ�ƣ�
//@ǰ�ó��� :
//@C1       : 
//@C2       :
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      :���ô���λ��(���ڸ�����0, 0)
//@CP2      :���ô���λ��(���ڸ�����10, 10)
//@CP3      :���ô���λ��(���ڸ�����1024, 1024)
//////////////////////////////////////////////////////////////////////////
void KWndWindow_Test::SetPosition_Test(void)
{
	KWndWindow m_KWndWindow[3];
	
	//SWndWindow_Data *s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndWindow[0]), sizeof(KWndWindow)));
	float left = -1, top = -1;
	m_KWndWindow[0].SetPosition(0, 0);
	m_KWndWindow[0].GetPosition(&left, &top);
	CPPUNIT_ASSERT(left == 0.0); 
	CPPUNIT_ASSERT(top == 0.0); 

	m_KWndWindow[1].SetPosition(10, 10);
	//s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndWindow[1]), sizeof(KWndWindow)));
	m_KWndWindow[1].GetPosition(&left, &top);
	CPPUNIT_ASSERT(left == 10.0); 
	CPPUNIT_ASSERT(top == 10.0); 

	
	//s_data1 = reinterpret_cast<SWndWindow_Data *>
				//(CopyClassData(reinterpret_cast<void *>(&m_KWndWindow[2]), sizeof(KWndWindow)));
	m_KWndWindow[2].SetPosition(1024, 1024);
	m_KWndWindow[2].GetPosition(&left, &top);
	CPPUNIT_ASSERT(left == 1024.0); 
	CPPUNIT_ASSERT(top == 1024.0); 
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :��ȡ����λ�� �������
//@ǰ�ó��� :
//@C1       : ���ô���λ��(���ڸ�����-10, -10)
//@C2       : ���ô���λ��(���ڸ�����0, 0)
//@C3       : ���ô���λ��(���ڸ�����10, 10)
//@C4       : ���ô���λ��(���ڸ�����1024, 1024)
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      : ͨ��������� ��鴰��λ��(���ڸ�����-10, -10)
//@CP2      : ͨ��������� ������ô���λ��(���ڸ�����0, 0)
//@CP3      : ͨ��������� ������ô���λ��(���ڸ�����10, 10)
//@CP4      : ͨ��������� ������ô���λ��(���ڸ�����1024, 1024)
//////////////////////////////////////////////////////////////////////////
void KWndWindow_Test::GetPosition_Test(void)
{
	//���GetPosition����һ��������NULL  ��ֻ��һ��ֵ����ȷ�� Ӧ����
	//����ֵ���ж�
	KWndWindow m_KWndWindow[4];
	float  point[4] = {-10, 0, 10, 1024};

	for (int i=0; i<4; i++)
	{
		m_KWndWindow[i].SetPosition(point[i], point[i]);	
	}

	float left, top;
	for (int i=0; i<4; i++)
	{
		m_KWndWindow[i].GetPosition(&left, &top);
		CPPUNIT_ASSERT(left == point[i]); 
		CPPUNIT_ASSERT(top  == point[i]); 	
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :���ô��ڴ�С
//@ǰ�ó��� :���Ӵ��ں����Ӵ���
//@C1       : ���Ӵ��� ���ô��ڴ�С(10, 0)
//@C2       : ���Ӵ��� ���ô��ڴ�С(1024, 1024)
//@C3       : ���Ӵ��� ���ô��ڴ�С(10, 0)
//@C4       : ���Ӵ��� ���ô��ڴ�С(1024, 1024)
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      : ��鸸���ں��Ӵ��ڴ�С(0, 0)
//@CP2      : ��鸸���ں��Ӵ��ڴ�С(1024, 1024)
//////////////////////////////////////////////////////////////////////////
void KWndWindow_Test::SetSize_Test(void)
{
	float size[2] = {10.0, 1024.0};
	KWndWindow NoChild;
	KWndWindow window[6];
	
	window[0].SetStyle(0xFFFFFFFF);
	//�����Ӵ���
	for (int i=1; i<6; i++)
	{
		window[0].AddChild(&window[i]);
		window[i].SetStyle(0xFFFFFFFF);   //���ô������� 
	}

	//���Ӵ��� ���ô��ڴ�С(10, 0)
	//���Ӵ��� ���ô��ڴ�С(1024, 1024)
	for (int j=0; j<2; j++)
	{
		window[0].SetSize(size[j], size[j]);

		{
			//��鸸���ڵĴ�С
			float Width = -1, Height = -1;
			window[0].GetSize(&Width, &Height);
			CPPUNIT_ASSERT(Width  == size[j]);      //�����ڵĸ߶ȺͿ�ȶ��ı���
			CPPUNIT_ASSERT(Height  == size[j]);   
		}
		//��������Ӵ��ڵĴ�С
		for (int i=1; i<6; i++)
		{
			float Width = -1, Height = -1;
			window[i].GetSize(&Width, &Height);
			CPPUNIT_ASSERT(Width  == 0.0);       //ֻ�ı����Ӵ��ڵĸ߶�, �������ڵĸ߶ȺͿ�ȶ��ı���
			CPPUNIT_ASSERT(Height  == size[j]);   //�봰�ڵ������й� ��������WND_S_SIZE_WITH_B_EDGE
		}
	}

	//���Ӵ���
	for (int j=0; j<2; j++)
	{
		NoChild.SetSize(size[j], size[j]);
		//��鴰�ڵĴ�С		
		float Width = -1, Height = -1;
		NoChild.GetSize(&Width, &Height);
		CPPUNIT_ASSERT(Width ==size[j]); 
		CPPUNIT_ASSERT(Height  == size[j]); 	
	}

}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :��ȡ���ڴ�С
//@ǰ�ó��� :���Ӵ���
//@C1       : ���Ӵ��� ���ô��ڴ�С(0, 0)
//@C2       : ���Ӵ��� ���ô��ڴ�С(10, 10)
//@C3       : ���Ӵ��� ���ô��ڴ�С(1024, 1024)
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      : ��鴰�ڴ�С(0, 0)
//@CP2      : ��鴰�ڴ�С(1024, 1024)

//////////////////////////////////////////////////////////////////////////
void KWndWindow_Test::GetSize_Test(void)
{
	//��һ������ΪNULL
	KWndWindow m_KWndWindow[4];
	float  point[3] = {0.0, 10.0, 1024.0};
	for (int i=0; i<3; i++)
	{
		float left = -1, top = -1;
		m_KWndWindow[i].SetSize(point[i], point[i]);	
		m_KWndWindow[i].GetSize(&left, &top);
		CPPUNIT_ASSERT(left == point[i]); 
		CPPUNIT_ASSERT(top  == point[i]); 	
	}
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :��������ĵ���
//@ǰ�ó��� :���Ӵ��ں����Ӵ���
//@C1       : ���Ӵ��� ���ô��ھ�������(0, 0)
//@C2       : ���Ӵ��� ���ô��ھ�������(1024, 1024)
//@C3       : ���Ӵ��� ���ô��ھ�������(0, 0)
//@C4       : ���Ӵ��� ���ô��ھ�������(1024, 1024)
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      : ��鴰�ھ�������(0, 0)
//@CP2      : ��鴰�ھ�������(1024, 1024)

//////////////////////////////////////////////////////////////////////////
void KWndWindow_Test::AbsoluteMove_Test(void)
{
	
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :��þ�������
//@ǰ�ó��� :��ʼ����������Ϊ(0, 0)
//@C1       : ���Ӵ��� ���ô��ھ�������(-10, -10)
//@C2       : ���Ӵ��� ���ô��ھ�������(20, 20)
//@C3       : ���Ӵ��� ���ô��ھ�������(50, 50)
//@C4       : ���Ӵ��� ���ô��ھ�������(1024, 1024)
//@C5       : ���Ӵ��� ���ô��ھ�������(-10, -10)
//@C6       : ���Ӵ��� ���ô��ھ�������(20, 30)
//@C7       : ���Ӵ��� ���ô��ھ�������(50, 50)
//@C8       : ���Ӵ��� ���ô��ھ�������(1024, 1024)
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      : ͨ��������� ��鴰�ھ�������(-10, -10)
//@CP2      : ͨ��������� ��鴰�ھ�������(20, 20)
//@CP3      : ͨ��������� ��鴰�ھ�������(50, 50)
//@CP4      : ͨ��������� ��鴰�ھ�������(1024, 1024)
//////////////////////////////////////////////////////////////////////////
void KWndWindow_Test::GetAbsolutePos_Test(void)
{
	float AbsolutePos = 0.0f;
	float Pos[4] = {-10, 20, 50, 1024};
	KWndWindow NoChild;
	KWndWindow window[10];
	for (int i=1; i<10; i++)
	{
		window[0].AddChild(&window[i]);    //�����Ӵ���
	}
	
    //-----------------------------------�����������С��0 Ҳ������?
	for (int j=0; j<4; j++)
	{
		//��ȷ�ľ�������
		AbsolutePos += Pos[j];
		window[0].AbsoluteMove(Pos[j], Pos[j]);
		//���д��ڵľ�����������ȷ����(�����Ӵ���)
		for (int i=0; i<10; i++)
		{
			float ax = -1, ay = -1;
			window[i].GetAbsolutePos(&ax, &ay);
			CPPUNIT_ASSERT(ax  == AbsolutePos); 
			CPPUNIT_ASSERT(ay  == AbsolutePos); 
		}
	}	

	AbsolutePos = 0.0f;
	for (int j=0; j<4; j++)
	{
		//��ȷ�ľ�������
		AbsolutePos += Pos[j];
		NoChild.AbsoluteMove(Pos[j], Pos[j]);
		//���ڵľ�����������ȷ����
		float ax = -1, ay = -1;
		NoChild.GetAbsolutePos(&ax, &ay);
		CPPUNIT_ASSERT(ax  == AbsolutePos); 
		CPPUNIT_ASSERT(ay  == AbsolutePos); 	
	}
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :��þ�������
//@ǰ�ó��� :��ʼ����������Ϊ(0, 0)  ���ڿ���
//@C1       : ���Ӵ��� ���ô��ھ�������(0, 0) ��С(0, 0)
//@C2       : ���Ӵ��� ���ô��ھ�������(50, 50) ��С(20, 20)
//@C3       : ���Ӵ��� ���ô��ھ�������(50, 50) ��С(1024, 1024)

//@C4       : ���Ӵ��� ���ô��ھ�������(0, 0) ��С(0, 0) �Ӵ��� ����(0, 0) ��С(0, 0)
//@C5       : ���Ӵ��� ���ô��ھ�������(50, 50) ��С(20, 20) �Ӵ�������(50, 50) ��С(100, 100)
//@C6       : ���Ӵ��� ���ô��ھ�������(50, 50) ��С(1024, 1024) �Ӵ�������(10, 10) ��С(10, 10) 
//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      : ͨ��������� ��������Ƿ��ڴ�����
//////////////////////////////////////////////////////////////////////////

void KWndWindow_Test::PtInWindow_Test(void)
{
	//PtInWindow �Ĳ�����Ȼ��int������float
	typedef struct 
	{
		int x;
		int y;
	}POINT, *LPPOINT;

	POINT point[3] = { {10, 10}, {60, 60}, {100, 100} };
	float size[3] = {0, 20, 1024};
	float Pos[3] = {0, 50, 50};

	
	//�ȼ�����Ӵ��ڵ������Ƿ��ڴ�����
	KWndWindow Parent[3];
	for (int i=0; i<3; i++)
	{
		//���ô��ڿ���
		Parent[i].SetStyle(WND_S_VISIBLE);  
		Parent[i].AbsoluteMove(Pos[i], Pos[i]);
		Parent[i].SetSize(size[i], size[i]);
	}
	
	for (int i=0; i<3; i++)
	{
		int res = Parent[0].PtInWindow(point[i].x, point[i].y);
		CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
	}

	int res = -1;
	res = Parent[0].PtInWindow(point[0].x, point[0].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
	res = Parent[0].PtInWindow(point[1].x, point[1].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
	res = Parent[0].PtInWindow(point[2].x, point[2].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����

	res = Parent[1].PtInWindow(point[0].x, point[0].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
	res = Parent[1].PtInWindow(point[1].x, point[1].y);
	CPPUNIT_ASSERT(res  == 1);  //�ڴ�����
	res = Parent[1].PtInWindow(point[2].x, point[2].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����

	res = Parent[2].PtInWindow(point[0].x, point[0].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
	res = Parent[2].PtInWindow(point[1].x, point[1].y);
	CPPUNIT_ASSERT(res  == 1);  //�ڴ�����
	res = Parent[2].PtInWindow(point[2].x, point[2].y);
	CPPUNIT_ASSERT(res  == 1);  //���ڴ�����

	//���Ӵ��ڵ������Ƿ��ڴ�����
	KWndWindow Child[3];
	//����Ӵ���
	for (int i=0; i<3; i++)
	{
		//����Ӵ���ʱ���޸��Ӵ��ڵľ�������
		Parent[i].SetStyle(0);
		Parent[i].AddChild(&Child[i]);
	}
	Child[0].SetSize(0, 0);
	res = Parent[0].PtInWindow(point[0].x, point[0].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
	res = Parent[0].PtInWindow(point[1].x, point[1].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
	res = Parent[0].PtInWindow(point[2].x, point[2].y);
	CPPUNIT_ASSERT(res  == 0);  //���ڴ�����
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :�Ƿ�Ϊ���ϲ㴰��
//@ǰ�ó��� :��ʼ����������Ϊ(0, 0)  ���ڿ���
//@C1       : ���Ӵ��� ���ô��ھ�������(0, 0) ��С(0, 0)
//@C2       : ���Ӵ��� ���ô��ھ�������(50, 50) ��С(20, 20)
//@C3       : ���Ӵ��� ���ô��ھ�������(50, 50) ��С(1024, 1024)

//@����Դ   :
//@�������� :m_KWndWindow
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      : ͨ��������� �������Ƿ�Ϊ���ϲ㴰��
//////////////////////////////////////////////////////////////////////////

void KWndWindow_Test::TopChildFromPoint_Test(void)
{
	//PtInWindow �Ĳ�����Ȼ��int������float
	typedef struct 
	{
		int x;
		int y;
	}POINT, *LPPOINT;

	POINT point[3] = { {10, 10}, {60, 60}, {100, 100} };
	float size[3] = {0, 20, 1024};
	float Pos[3] = {0, 50, 50};

	
	//�ȼ�����Ӵ��ڵ������Ƿ��ڴ�����
	KWndWindow Parent[3];
	for (int i=0; i<3; i++)
	{
		//���ô��ڿ���
		Parent[i].SetStyle(WND_S_VISIBLE);  
		Parent[i].AbsoluteMove(Pos[i], Pos[i]);
		Parent[i].SetSize(size[i], size[i]);
	}	
	KWndWindow *TopChild = Parent[0].TopChildFromPoint(point[0].x, point[0].y);
	//�����κδ�����
	CPPUNIT_ASSERT(TopChild  == NULL);  
	TopChild = Parent[0].TopChildFromPoint(point[1].x, point[1].y);
	//�����κδ�����
	CPPUNIT_ASSERT(TopChild  == NULL); 
	TopChild = Parent[0].TopChildFromPoint(point[2].x, point[2].y);
	//�����κδ�����
	CPPUNIT_ASSERT(TopChild  == NULL); 

	//��鴰��1
	TopChild = Parent[1].TopChildFromPoint(point[0].x, point[0].y);
	//���ϲ㴰��
	CPPUNIT_ASSERT(TopChild  == NULL);
	TopChild = Parent[1].TopChildFromPoint(point[1].x, point[1].y);
	//���ϲ㴰����Parent[1]
	CPPUNIT_ASSERT(TopChild  == &Parent[1]);
	TopChild = Parent[1].TopChildFromPoint(point[2].x, point[2].y);
	//���ϲ㴰��
	CPPUNIT_ASSERT(TopChild  == NULL);

	//��鴰��2
	TopChild = Parent[2].TopChildFromPoint(point[0].x, point[0].y);
	//���ϲ㴰��
	CPPUNIT_ASSERT(TopChild  == NULL);
	TopChild = Parent[2].TopChildFromPoint(point[1].x, point[1].y);
	//���ϲ㴰����Parent[2]
	CPPUNIT_ASSERT(TopChild  == &Parent[2]);
	TopChild = Parent[2].TopChildFromPoint(point[2].x, point[2].y);
	//���ϲ㴰����Parent[2]
	CPPUNIT_ASSERT(TopChild  == &Parent[2]);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :�Լ������Ӵ��ڣ��Ӵ�������������������

//@ǰ�ó��� :
//@C1       :�޸�����
//@C2       :�и����� 
//@C3       :�и����� , ��һ��next�ֵܴ���
//@C3       :�и����� , ��һ��next�ֵܴ���, ��һ��Previous����
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :�޸����� SplitSmaleFamily
//@OP2      :�Ӵ��� SplitSmaleFamily
//@OP3      :�ֵܴ��� SplitSmaleFamily
//@���ó��� :
//@CP1      :
//@CP2      :��鴰�����Ƿ���ȷ
//////////////////////////////////////////////////////////////////////////

void KWndWindow_Test::SplitSmaleFamily_Test(void)
{
	//�޸�����
	KWndWindow wnd;
	wnd.SplitSmaleFamily();
	CPPUNIT_ASSERT(wnd.GetNextWnd() == NULL);
	CPPUNIT_ASSERT(wnd.GetPrevWnd() == NULL);
	CPPUNIT_ASSERT(wnd.GetParent() == NULL);
	
	//�и����� 
	KWndWindow Child;
	wnd.AddChild(&Child);
	Child.SplitSmaleFamily();
	CPPUNIT_ASSERT(Child.GetNextWnd() == NULL);
	CPPUNIT_ASSERT(Child.GetPrevWnd() == NULL);
	//������ָ��ΪNULL
	CPPUNIT_ASSERT(Child.GetParent() == NULL);
	
	//�и����� , ��һ��next�ֵܴ���
	KWndWindow parent1, child1, brothor1;
	parent1.AddChild(&child1);
	child1.AddBrother(&brothor1);
	child1.SplitSmaleFamily();
	//�Ӵ��ڵ�ָ��ΪNULL
	CPPUNIT_ASSERT(Child.GetNextWnd() == NULL);
	CPPUNIT_ASSERT(Child.GetPrevWnd() == NULL);
	CPPUNIT_ASSERT(Child.GetParent() == NULL);
	//�����ڵ�pChildָ��ָ��brothor1;
	CPPUNIT_ASSERT(parent1.GetFirstChild() == &brothor1);

	//�и����� , ��һ��next�ֵܴ���, ��һ��Previous����
	KWndWindow parent2, child2, brothor2, brothor3;
	parent2.AddChild(&child2);
	parent2.AddChild(&brothor2);
	parent2.AddChild(&brothor3);
	brothor2.SplitSmaleFamily();
	//�Ӵ��ڵ�ָ��ΪNULL
	CPPUNIT_ASSERT(brothor2.GetNextWnd() == NULL);
	CPPUNIT_ASSERT(brothor2.GetPrevWnd() == NULL);
	CPPUNIT_ASSERT(brothor2.GetParent() == NULL);
	//�����ڵ�pChildָ��ָ��child2;
	CPPUNIT_ASSERT(parent2.GetFirstChild() == &child2);
	//brothor3�ĸ�����ָ��
	CPPUNIT_ASSERT(brothor3.GetParent() == &parent2);
	//brothor3���ֵܴ���ָ��
	CPPUNIT_ASSERT(brothor3.GetPrevWnd() == &child2);

	//�ѵ�һ���Ӵ��ڷ������
	child2.SplitSmaleFamily();
	//�����ڵ�pChildָ��ָ��borthor3;
	CPPUNIT_ASSERT(parent2.GetFirstChild() == &brothor3);
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :ȡ�ð��������Ӵ��ڷֲ��������С����

//@ǰ�ó��� :
//@C1       :
//@C2       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :
//@OP2      :
//@���ó��� :
//@CP1      :
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndWindow_Test::GetAllChildLayoutRect_Test()
{




}
#endif
