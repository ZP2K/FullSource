/***************************************************
Filename:wndscrollbar_Test.cpp
CreatedBy: Yan Chunwa
CreatedOn: 2005��8��30�� 10:20:03.0083581
Test Requirement Reference by Wiki path:
http://sword-test/qcweb/pmwiki.php?n=XSJQC-DEV.JX3-QC-REQ-SO3Client_UI_Scrollbar
Comment:
**************************************************/
#include "stdafx.h"

#ifdef _UNIT_TEST

#include "wndscrollbar_Test.h"
#include "../ui/elem/wndscrollbar.h"
#include "../ui/elem/wndbutton.h"
#include "../ui/elem/wndimage.h"

using namespace std;
#include "../../DevEnv/Include/Lua/llimits.h"

CPPUNIT_TEST_SUITE_REGISTRATION(KWndScrollBar_Test);

UI::KWndButton* btnNext;
UI::KWndButton* btnPage;
UI::KWndButton* btnSlide;
UI::KWndScrollBar* scrollbar;

void KWndScrollBar_Test::setUp()
{  
	scrollbar = new UI::KWndScrollBar;
	btnNext = new UI::KWndButton;
	btnPage = new UI::KWndButton;
	btnSlide = new UI::KWndButton;
	scrollbar->BindNextPageButton(btnNext);
	scrollbar->BindPrePageButton(btnPage);
	scrollbar->BindSlideButton(btnSlide);
}

void KWndScrollBar_Test::tearDown()
{
	delete scrollbar;
	delete btnNext;
	delete btnPage;
	delete btnSlide;
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :��֤SetScrollPos,GetScrollPos����ȷ��
//@ǰ�ó��� :Ҫ�����ú�λ��ȡֵ��ΧSetValueRange
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :�ֱ���-1��0��1��200��MAX_INT
//@���ó��� :
//@CP1      :����õĵ�ǰλ��ֵ�Ƿ���Ԥ����ͬ
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndScrollBar_Test::SetScrollPostest()
{
	int nRetCode = false;
	int i = 0;
	int j = 0;
	const pos[5] = {-1, 0, 1, 200, MAX_INT};
	
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == 0);

	//δ����ʼ��λ��ֵ��Χ
	for (i = 0; i < 5; i++)
	{
		scrollbar->SetScrollPos(pos[i]);
		nRetCode = scrollbar->GetScrollPos();
		CPPUNIT_ASSERT(nRetCode == MinPos);
	}
	//�Ѿ���ʼ��λ��ֵ��Χ
	scrollbar->SetValueRange(MinPos, MaxPos);
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == MaxPos);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == MinPos);
		

	for (i = 0; i < 5; i++)
	{
		scrollbar->SetScrollPos(pos[i]);
		nRetCode = scrollbar->GetScrollPos();
		switch(i) 
		{
		case 0:
			CPPUNIT_ASSERT(nRetCode == MinPos);
			break;
		case 3:
			CPPUNIT_ASSERT(nRetCode == MaxPos - 1);
			break;
		case 4:
			CPPUNIT_ASSERT(nRetCode == MaxPos - 1);
			break;
		default:
			CPPUNIT_ASSERT(nRetCode == pos[i]);
		}//switch
		
	}//for i
	
}

//////////////////////////////////////////////////////////////////////////
//@������� :2
//@����Ŀ�� :��֤ScrollLine����ȷ��
//@ǰ�ó��� :Ҫ�����ú�λ��ȡֵ��ΧSetValueRange
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :����0��1��NULL�������
//@OP2      :������λ���׶�/ĩ�ˣ����/��ǰ����
//@���ó��� :
//@CP1      :����õĵ�ǰλ��ֵ�Ƿ���Ԥ��ֵ��ͬ
//@CP2      :�������ɹ�
//////////////////////////////////////////////////////////////////////////
void KWndScrollBar_Test::ScrollLinetest()
{
	int nRetCode = false;
	int i = 0;
	int curPos = 10;
	bool bpre[3] = {0, 1, NULL};

	scrollbar->SetValueRange(MinPos, MaxPos);
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == MaxPos);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == MinPos);
	scrollbar->SetScrollPos(curPos);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == curPos);
    //����
    nRetCode = scrollbar->ScrollLine(bpre[0]);
	CPPUNIT_ASSERT(nRetCode == curPos + 1);
	curPos = nRetCode;
	//��ǰ��
	nRetCode = scrollbar->ScrollLine(bpre[1]);
	CPPUNIT_ASSERT(nRetCode == curPos - 1);
	curPos = nRetCode;
	//�������
	nRetCode = scrollbar->ScrollLine(bpre[2]);
	//CPPUNIT_ASSERT(!nRetCode);

	//��ǰλ��Ϊ��һ����������ǰ��
	scrollbar->SetScrollPos(MinPos);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == MinPos);
	nRetCode = scrollbar->ScrollLine(bpre[1]);
	CPPUNIT_ASSERT(nRetCode == MinPos);
	//��ǰλ��Ϊ���һ������������
	scrollbar->SetScrollPos(MaxPos);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == MaxPos - 1);
	nRetCode = scrollbar->ScrollLine(bpre[0]);
	CPPUNIT_ASSERT(nRetCode == MaxPos - 1);
	
}

//////////////////////////////////////////////////////////////////////////
//@������� :3
//@����Ŀ�� :��֤ScrollPage����ȷ��
//@ǰ�ó��� :Ҫ�����ú�λ��ȡֵ��ΧSetValueRange,���ú�ҳ���С
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :����0��1��NULL�������  
//@OP2      :������λ���׶�(0,2,10)/ĩ��(190,198,200)�����/��ǰ��ҳ
//@���ó��� :
//@CP1      :����õĵ�ǰλ��ֵ�Ƿ���Ԥ��ֵ��ͬ
//@CP2      :��ҳ���ɹ�
//////////////////////////////////////////////////////////////////////////
void KWndScrollBar_Test::ScrollPagetest()
{
	int nRetCode = false;
	int curPos = 25;
	int PageSize = 10;
	bool bpre[3] = {0, 1, NULL};

	scrollbar->SetValueRange(MinPos, MaxPos);
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == MaxPos);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == MinPos);

	scrollbar->SetPageSize(PageSize);
	nRetCode = scrollbar->GetPageSize();
	CPPUNIT_ASSERT(nRetCode == PageSize);

	scrollbar->SetScrollPos(curPos);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == curPos);

	//���ҳ
    nRetCode = scrollbar->ScrollPage(bpre[0]);
	CPPUNIT_ASSERT(nRetCode == curPos + PageSize);
	curPos = nRetCode;
	//��ǰ��ҳ
	nRetCode = scrollbar->ScrollPage(bpre[1]);
	CPPUNIT_ASSERT(nRetCode == curPos - PageSize);
	curPos = nRetCode;
	//�������
	nRetCode = scrollbar->ScrollPage(bpre[2]);
	//CPPUNIT_ASSERT(!nRetCode);

	//��ǰҳΪ��һҳ��������ǰ��ҳ
	scrollbar->SetScrollPos(MinPos);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == MinPos);
	nRetCode = scrollbar->ScrollPage(bpre[1]);
	CPPUNIT_ASSERT(nRetCode == MinPos);

	scrollbar->SetScrollPos(2);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == 2);
	nRetCode = scrollbar->ScrollPage(bpre[1]);
	CPPUNIT_ASSERT(nRetCode == MinPos);

	scrollbar->SetScrollPos(10);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == 10);
	nRetCode = scrollbar->ScrollPage(bpre[1]);
	CPPUNIT_ASSERT(nRetCode == MinPos);

	//��ǰҳΪ���һҳ���������ҳ
	scrollbar->SetScrollPos(MaxPos - 1);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == MaxPos - 1);
	nRetCode = scrollbar->ScrollPage(bpre[0]);
	CPPUNIT_ASSERT(nRetCode == MaxPos - 1);

	scrollbar->SetScrollPos(198);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == 198);
	nRetCode = scrollbar->ScrollPage(bpre[0]);
	CPPUNIT_ASSERT(nRetCode == MaxPos - 1);

	scrollbar->SetScrollPos(190);
	nRetCode = scrollbar->GetScrollPos();
	CPPUNIT_ASSERT(nRetCode == 190);
	nRetCode = scrollbar->ScrollPage(bpre[0]);
	CPPUNIT_ASSERT(nRetCode == MaxPos - 1);
}

//////////////////////////////////////////////////////////////////////////
//@������� :4
//@����Ŀ�� :��֤SetValueRange,GetMinValue,GetMaxValue����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :���루3��2������1��1������-4��-1����(-4, 1)��1��100��������� 
//@OP2      :
//@���ó��� :
//@CP1      :���λ�����ֵ����Сֵ
//@CP2      :
//////////////////////////////////////////////////////////////////////////
void KWndScrollBar_Test::SetValueRangetest()
{
	int nRetCode = false;
	int minVal = 0;
	int maxVal = 0;
    
	minVal = scrollbar->m_nMinValue;
	maxVal = scrollbar->m_nMaxValue;
	//��Сֵ > ���ֵ 
	scrollbar->SetValueRange(3, 2);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == minVal);
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == maxVal);
	//��Сֵ �� ���ֵ
	scrollbar->SetValueRange(1, 1);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == 1);
	minVal = nRetCode;
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == 1);
	maxVal = nRetCode;
	//��Сֵ < ���ֵ��������Ϊ����
	scrollbar->SetValueRange(-4, -1);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == -4);
	minVal = nRetCode;
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == -1);
	maxVal = nRetCode;
	//��Сֵ < ���ֵ�����ֵΪ����
	scrollbar->SetValueRange(-4, 1);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == -4);
	minVal = nRetCode;
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == 1);
	maxVal = nRetCode;
	//�������
	scrollbar->SetValueRange(1, 100);
	nRetCode = scrollbar->GetMinValue();
	CPPUNIT_ASSERT(nRetCode == 1);
	nRetCode = scrollbar->GetMaxValue();
	CPPUNIT_ASSERT(nRetCode == 100);

}
//////////////////////////////////////////////////////////////////////////
//@������� :5
//@����Ŀ�� :��֤SetPageSize,GetPageSize����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :����-1��0��1��50��MAX_INT 
//@OP2      :
//@���ó��� :
//@CP1      :����õĵ�ǰҳ���С�Ƿ���Ԥ��ֵ��ͬ
//@CP2      :
//////////////////////////////////////////////////////////////////////////
void KWndScrollBar_Test::SetPageSizetest()
{
	int nRetCode = false;
	int i = 0;
	const int nSize[5] = {-1, 0, 1, 50, MAX_INT};

	for(i = 0; i < 5; i++)
	{
		nRetCode = scrollbar->m_nPageSize;
		scrollbar->SetPageSize(nSize[i]);		
		if(i < 2)
			CPPUNIT_ASSERT(nRetCode == scrollbar->GetPageSize());
		else
		{
		CPPUNIT_ASSERT(nRetCode != scrollbar->GetPageSize() && scrollbar->GetPageSize() == nSize[i]);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//@������� :6
//@����Ŀ�� :��֤SetTotalSize����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :����-1��0��1��50��MAX_INT 
//@OP2      :
//@���ó��� :
//@CP1      :����õĵ�ǰλ��ֵ�Ƿ�������ֵ��ͬ
//@CP2      :
//////////////////////////////////////////////////////////////////////////
void KWndScrollBar_Test::SetTotalSizetest()
{
	int nRetCode = false;
	int i = 0;
	const int nSize[5] = {-1, 0, 1, 50, MAX_INT};

	for(i = 0; i < 5; i++)
	{
		nRetCode = scrollbar->m_nTotalSize;
		scrollbar->SetTotalSize(nSize[i]);
		
		if(i < 2)
			CPPUNIT_ASSERT(nRetCode == scrollbar->m_nTotalSize);
		else
			CPPUNIT_ASSERT(nRetCode != scrollbar->m_nTotalSize && scrollbar->m_nTotalSize == nSize[i]);
	}
	
	
}
//////////////////////////////////////////////////////////////////////////
//@������� :7
//@����Ŀ�� :��֤SetSize����ȷ��
//@ǰ�ó��� :������Ϊˮƽ�͵�
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :���루-1��-8),��0��0),(0, 50),(50, 0)��50��100),(MAX_INT��MAX_INT��
//@OP2      :
//@���ó��� :
//@CP1      :����õĵ�ǰ���ڴ�С�Ƿ�������ֵ��ͬ
//@CP2      :
//////////////////////////////////////////////////////////////////////////
void KWndScrollBar_Test::SetSizetest()
{
	int nRetCode = false;
	float pWidth1 = 0;
	float pHeight1 = 0;
	float pWidth2 = 0;
	float pHeight2 = 0;

    
	//(-1, -8)	
	scrollbar->GetSize(&pWidth1, &pHeight1);
	scrollbar->SetSize(-1, -8);  
	scrollbar->GetSize(&pWidth2, &pHeight2);
	CPPUNIT_ASSERT(pWidth2 == 0 && pHeight2 == 5);
	//(0, 0)
	scrollbar->GetSize(&pWidth1, &pHeight1);
	scrollbar->SetSize(0, 0);  
	scrollbar->GetSize(&pWidth2, &pHeight2);
	CPPUNIT_ASSERT(pWidth2 == pWidth1 && pHeight2 == pHeight1);
	//(0, 50)
	scrollbar->GetSize(&pWidth1, &pHeight1);
	scrollbar->SetSize(0, 50);  
	scrollbar->GetSize(&pWidth2, &pHeight2);
	CPPUNIT_ASSERT(pWidth2 == pWidth1 && pHeight2 == 50);
	//(50, 0)
	scrollbar->GetSize(&pWidth1, &pHeight1);
	scrollbar->SetSize(50, 0);  
	scrollbar->GetSize(&pWidth2, &pHeight2);
	CPPUNIT_ASSERT(pWidth2 == 50 && pHeight2 == 5);
	//(50, 100)
	//scrollbar->GetSize(pWidth1, pHeight1);
	scrollbar->SetSize(50, 100);  
	scrollbar->GetSize(&pWidth2, &pHeight2);
	CPPUNIT_ASSERT(pWidth2 == 50 && pHeight2 == 100);
	//��MAX_INT��MAX_INT��
	scrollbar->SetSize(MAX_INT, MAX_INT);  
	scrollbar->GetSize(&pWidth2, &pHeight2);
	CPPUNIT_ASSERT(pWidth2 == MAX_INT && pHeight2 == MAX_INT);
}

#endif


