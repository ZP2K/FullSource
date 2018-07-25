/*
FileName    :  Richtext_Test.h
Version     :  
Creater     :  YangWeidong
Date        :  2005-7-29 17:03:05
Description :  
*/

#ifndef _RICHTEXT_TEST_H_
#define _RICHTEXT_TEST_H_

#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/TestFixture.h"
#include "../ui/elem/wndrichtext.h"
#include "../ui/driver/igraphic.h"
#include "../ui/text/ktextlayout.h"

using namespace UI;

class Richtext_Test : public CppUnit::TestFixture, KingsoftUnitTest::ClassDataCopyBuffer
{
	LPCTSTR              m_RichtextContent;
	int                  m_RichtextSize;
	UI::IGraphic::KColor m_RichtextColor;
	UI::IGraphic::KColor m_RichtextBorderColor;
	int                  m_Topline;
	int                  m_RowSpaceing;

	KWndRichText &m_RichtextObj;

	CPPUNIT_TEST_SUITE(cText_UI_Richtext);

	CPPUNIT_TEST(SetText_test);
	CPPUNIT_TEST(SetTextSize_test);
	CPPUNIT_TEST(SetTextColor_test);
	CPPUNIT_TEST(SetTextBorderColor_test);
	CPPUNIT_TEST(SetTopLine_test );
	CPPUNIT_TEST(SetRowSpacing_test);

	CPPUNIT_TEST_SUITE_END();

public:
	Richtext_Test();
	~Richtext_Test();

	virtual void setUp();
	virtual void tearDown();

	/*
	��richtext�����ַ����Ĺ��ܽ��в���, ���ݰ������¼�������: 
	1. ��ʼ��richtext���ַ���(�޸�ʽ�ַ�)
	2. ����richtext�ַ���(�޸�ʽ�ַ�)
	3. ����richtext�ַ���(�и�ʽ�ַ�)
	4. ���ݸ�richtext��ͬ�����ַ�: ���ֽڡ����ַ���unicode�ַ�
	5. ����richtext���ܽ��ܵ�����ַ�������
	*/
	void SetText_test();

	/*
	��richtext���������С�Ĺ��ܽ��в���, ���ݰ������¼�������: 
	1. ������С
	2. ���������СΪ0
	3. ���������СΪ��
	4. ���������СΪ100
	*/
	void SetTextSize_test(int nSize);

	/*
	��richtext����������ɫ�Ĺ��ܽ��в���, ���ݰ������¼�������: 
	1. ����ֵ 0x334455
	2. ������ֵ 0x33445566
	*/
	void SetTextColor_test(UI::IGraphic::KColor sColor);

	/*
	��richtext��������߿���ɫ�Ĺ��ܽ��в���, ���ݰ������¼�������: 
	1. ����ֵ 0x334455
	2. ������ֵ 0x33445566
	*/
	void SetTextBorderColor_test(IGraphic::KColor sColor);

	/*
	��richtext���ÿ�ʼ��ʾ�����Ľ��в���, ���ݰ������¼�������: 
	1. ����ֵ 0, 1
	2. ������ֵ -1, 99999
	*/
	void SetTopLine_test(int nValue);

	/*
	��richtext�����м��Ĺ��ܽ��в���, ���ݰ������¼�������: 
	1. ����ֵ 0, 1
	2. ������ֵ -1, 99999
	*/
	void SetRowSpacing_test(int nValue);

};

#endif //_RICHTEXT_TEST_H_