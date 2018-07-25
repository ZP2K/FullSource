/***************************************************
Filename:wndRichText_Test.cpp
CreatedBy: YangWeidong
CreatedOn: 2005��8��15�� 15:04:50.7491037
Description:
**************************************************/

#include "stdafx.h"

#ifdef _UNIT_TEST

#include <vector>
#include <string>
#include <Windows.h>
#include "../ui/common/tblock.h"
#include <fstream>

#include "wndRichText_Test.h"
#include "../ui/elem/wndRichText.h"
#include "../ui/text/ktext.h"
#include "../ui/text/ktextencoder.h"
#include "../ui/text/ktextfonttable.h"
#include "../ui/text/ktexteventtable.h"
#include "../ui/driver/kanimateimagemgr.h"

#define Max_String_Length_For_Test 256

using namespace std;
CPPUNIT_TEST_SUITE_REGISTRATION(KWndRichText_Test);

KWndRichText_Test::KWndRichText_Test()
{
}

KWndRichText_Test::~KWndRichText_Test()
{
}

void KWndRichText_Test::setUp()
{
}

void KWndRichText_Test::tearDown()
{
}

void KWndRichText_Test::WriteTestString(LPTSTR ToString, int& iWritePos, LPTSTR FromString, int iWriteLength)
{
	if (0 == iWriteLength)
	{
		iWriteLength = static_cast<int>(_tcslen(FromString));
	}

	//if ((iWritePos+iWriteLength) >= Max_String_Length_For_Test)
	//{
	//}

	for (int i=0; i<iWriteLength; i++)
	{
		ToString[iWritePos++] = FromString[i];
	}
	ToString[iWritePos] = '\0';
}

void WriteTestCtrl(LPTSTR ToString, int& iWritePos, UI::KText::KTEXT_CTRL_CODE nCtrl)
{
	ToString[iWritePos++] = nCtrl;
	ToString[iWritePos] = '\0';
}

void KWndRichText_Test::CompareTestString(LPTSTR TestEncodeString, LPTSTR ProgramEncodeString, int iCmpLength)
{
	if (0 == iCmpLength)
	{
		int iTestLength = static_cast<int>(_tcslen(TestEncodeString));
		int iProgramLength = static_cast<int>(_tcslen(ProgramEncodeString));
		iCmpLength = iTestLength>iProgramLength ? iTestLength : iProgramLength;
	}

	CPPUNIT_ASSERT(0 == memcmp(TestEncodeString,ProgramEncodeString,iCmpLength));
}

//void KWndRichText_Test::SetText_Test()
//{	
//}

//////////////////////////////////////////////////////////////////////////
//@������� :
//@����Ŀ�� : ���Զ��ַ�����ת������

//@ǰ�ó��� :
//@C1       :
//@C2       :
//@����Դ   : KTextEncode::Encode
//@�������� : LPTSTR m_sEncodeText = "����<size 30>����</size>����"
//@�������� : 
//@OP1      : ��һ���ַ��������˹�ת���Ҫ�ﵽ�Ľ��
//@OP2      : ͬ�����ַ���������KTextEncode::Encode����ת��
//@���ó��� :
//@CP1      :
//@CP2      :
//////////////////////////////////////////////////////////////////////////

void KWndRichText_Test::Encode_Test_FontSize()
{
	KWndRichText m_KWndRichText;
	
	LPTSTR m_sEncodeText = "����<size 30>����</size>����";
	TBlock<TCHAR> aBlock;
	aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	LPTSTR m_sNeedWriteString = "����";
	TCHAR  m_vcTestString[256];
	m_vcTestString[0] = '\0';
	int iWritePos(0);

	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_SIZE));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(30));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_SIZE_RESTORE));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}

void KWndRichText_Test::Encode_Test_FontType()
{
	KWndRichText m_KWndRichText;

	LPTSTR m_sEncodeText = "����<font fonttype1>����</font>����";
	TBlock<TCHAR> aBlock;
	aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	LPTSTR m_sNeedWriteString = "����";
	TCHAR  m_vcTestString[256];
	m_vcTestString[0] = '\0';
	int iWritePos(0);
	LPCTSTR FontTypeName = "fonttype1";
	int iFontTypeIndex = KTextFontTable::GetSelf().FindFont(FontTypeName);

	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_TYPE));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(iFontTypeIndex));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_TYPE_RESTORE));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}

void KWndRichText_Test::Encode_Test_FontColor()
{
	KWndRichText m_KWndRichText;

	LPTSTR m_sEncodeText = "����<color Red>����</color>����";
	TBlock<TCHAR> aBlock;
	aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	LPTSTR m_sNeedWriteString = "����";
	TCHAR  m_vcTestString[256];
	m_vcTestString[0] = '\0';
	int iWritePos(0);

	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_COLOR));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(255));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(0));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(0));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_COLOR_RESTORE));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}

void KWndRichText_Test::Encode_Test_FontBorderSize()
{
	//KWndRichText m_KWndRichText;

	//LPTSTR m_sEncodeText = "����<bordersize 30>����</bordersize>����";
	//TBlock<TCHAR> aBlock;
	//aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	//memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	//int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	//LPTSTR m_sNeedWriteString = "����";
	//TCHAR  m_vcTestString[256];
	//m_vcTestString[0] = '\0';
	//int iWritePos(0);

	//WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	//WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_BORDER_SIZE));
	//WriteTestOther((LPTSTR)m_vcTestString, iWritePos, 30);
	//WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	//WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_BORDER_SIZE_RESTORE));
	//WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	//CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}

void KWndRichText_Test::Encode_Test_FontBorderColor()
{
	KWndRichText m_KWndRichText;

	LPTSTR m_sEncodeText = "����<border red>����</border>����";
	TBlock<TCHAR> aBlock;
	aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	LPTSTR m_sNeedWriteString = "����";
	TCHAR  m_vcTestString[256];
	m_vcTestString[0] = '\0';
	int iWritePos(0);

	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_BORDER_COLOR));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(255));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(0));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(0));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_FONT_BORDER_COLOR_RESTORE));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}

void KWndRichText_Test::Encode_Test_Event()
{
	KWndRichText m_KWndRichText;

	LPTSTR m_sEncodeText = "����<event name=event1 file=ui/script/text/testtext.lua>����</event>����";
	TBlock<TCHAR> aBlock;
	aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	LPTSTR m_sNeedWriteString = "����";
	TCHAR  m_vcTestString[256];
	m_vcTestString[0] = '\0';
	int iWritePos(0);
	LPCTSTR EventName = "event1";
	WORD wEventIndex = UI::KTextEventTable::GetSelf().GetEventIndex(EventName);

	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_EVENT));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, wEventIndex);
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_EVENT_RESTORE));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}

void KWndRichText_Test::Encode_Test_Pic()
{
	KWndRichText m_KWndRichText;

	LPTSTR m_sEncodeText = "����<pic name=:) file=ui/image/face/faces.uitex width=10 height=20 align=right|top group=0 frame=0>����";
	TBlock<TCHAR> aBlock;
	aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	LPTSTR m_sNeedWriteString = "����";
	TCHAR  m_vcTestString[256];
	m_vcTestString[0] = '\0';
	int iWritePos(0);
	LPCTSTR PicName = ":)";
	int iPicIndex = UI::KAnimateImageMgr::GetSelf().GetImageIndex(PicName);
	SIZE sizeTest;
	sizeTest.cx = 10;
	sizeTest.cy = 20;
	WORD wFlag = UI::KText::KImage::FLAG_CENTER;
	wFlag = wFlag | UI::KText::KImage::FLAG_ALIGN_RIGHT | UI::KText::KImage::FLAG_ALIGN_TOP;

	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_IMAGE));
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, sizeTest);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, iPicIndex);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, wFlag);
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}

void KWndRichText_Test::Encode_Test_Newline()
{
	KWndRichText m_KWndRichText;

	LPTSTR m_sEncodeText = "����<n>����";
	TBlock<TCHAR> aBlock;
	aBlock.Alloc(static_cast<int>(_tcslen(m_sEncodeText)));
	memcpy(&aBlock[0], m_sEncodeText, aBlock.GetSize());
	int nEncodedLength = KTextEncoder::Encode(&aBlock[0], aBlock.GetSize());

	LPTSTR m_sNeedWriteString = "����";
	TCHAR  m_vcTestString[256];
	m_vcTestString[0] = '\0';
	int iWritePos(0);

	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);
	WriteTestOther((LPTSTR)m_vcTestString, iWritePos, static_cast<byte>(UI::KText::KTC_NEWLINE));
	WriteTestString((LPTSTR)m_vcTestString, iWritePos, m_sNeedWriteString);

	CompareTestString(&aBlock[0], m_vcTestString, iWritePos);
}
void KWndRichText_Test::SetFontSize_Test()
{
	KWndRichText m_KWndRichText;

	int iGetFontSize = m_KWndRichText.GetFontSize();
	CPPUNIT_ASSERT(12 == iGetFontSize);

	LPTSTR m_sSetText = _T("����<n>����");
	m_KWndRichText.SetText(m_sSetText);
	int iSetFontSize = 20;
	m_KWndRichText.SetFontSize(iSetFontSize);
	iGetFontSize = m_KWndRichText.GetFontSize();
	CPPUNIT_ASSERT(iSetFontSize == iGetFontSize);
}

void KWndRichText_Test::SetTextColor_Test()
{
	KWndRichText m_KWndRichText;

	int iGetTextColor = m_KWndRichText.GetTextColor();
	CPPUNIT_ASSERT(0 == iGetTextColor);

	LPTSTR m_sSetText = _T("����<n>����");
	m_KWndRichText.SetText(m_sSetText);
	int iSetTextColor = 20;
	m_KWndRichText.SetTextColor(iSetTextColor);
	iGetTextColor = m_KWndRichText.GetTextColor();
	CPPUNIT_ASSERT(iSetTextColor == iGetTextColor);
}

void KWndRichText_Test::SetTextBorderColor_Test()
{
	KWndRichText m_KWndRichText;

	int iGetTextBorderColor = m_KWndRichText.GetTextBorderColor();
	CPPUNIT_ASSERT(0 == iGetTextBorderColor);

	LPTSTR m_sSetText = _T("����<n>����");
	m_KWndRichText.SetText(m_sSetText);
	int iSetTextBorderColor = 20;
	m_KWndRichText.SetTextBorderColor(iSetTextBorderColor);
	iGetTextBorderColor = m_KWndRichText.GetTextBorderColor();
	CPPUNIT_ASSERT(iSetTextBorderColor == iGetTextBorderColor);
}

void KWndRichText_Test::SetTopLine_Test()
{
	KWndRichText m_KWndRichText;

	int iGetTopLine = m_KWndRichText.GetTopLine();
	CPPUNIT_ASSERT(0 == iGetTopLine);

	LPTSTR m_sSetText = _T("����<n>����");
	m_KWndRichText.SetText(m_sSetText);
	int iSetTopLine = 20;
	m_KWndRichText.SetTopLine(iSetTopLine);
	iGetTopLine = m_KWndRichText.GetTopLine();
	CPPUNIT_ASSERT(iSetTopLine == iGetTopLine);
}

void KWndRichText_Test::SetRowSpacing_Test()
{
	KWndRichText m_KWndRichText;

	int iGetRowSpacing = m_KWndRichText.GetRowSpacing();
	CPPUNIT_ASSERT(0 == iGetRowSpacing);

	LPTSTR m_sSetText = _T("����<n>����");
	m_KWndRichText.SetText(m_sSetText);
	int iSetRowSpacing = 20;
	m_KWndRichText.SetRowSpacing(iSetRowSpacing);
	iGetRowSpacing = m_KWndRichText.GetRowSpacing();
	CPPUNIT_ASSERT(iSetRowSpacing == iGetRowSpacing);
}

///*������ڴ�������Ƶ���һ�ݿ���*/
//void KWndRichText_Test::DummyClassForFirstCopy(void* pvClassBeingTest)
//{};
///*������ڴ�������Ƶ��ڶ��ݿ���*/
//void KWndRichText_Test::DummyClassForSecondCopy(void* pvClassBeingTest)
//{};
///*������ڴ�������浽ָ�����ļ�*/
//void KWndRichText_Test::DummyClassToFile( LPCSTR szFilename)
//{};
///*�Ƚ����ݿ���*/
//void KWndRichText_Test::CompareCopy(bool bAutoReleaseCopy)
//{};

#endif
