#include "TestClientReporter.h"
#include "KClientReport.h"
#include "atlstr.h"

const int GAME_NUM = 16;
const TCHAR* GAME_NAME[] = 
{
	"SD�Ҵ�Online", "����Q��online", "��1", "��2", "��������", "�λ�����", "ħ������", "ħ��",
	"�漣����", "����2", "������ʿOnline", "����֮��", "����", "���³�����ʿ", "��Ѫ����", "�ɽ�online"
};


void TestClientReporter::setUp()
{

}

void TestClientReporter::tearDown()
{

}

void TestClientReporter::KTestFullRun_01()
{
	int nResult  = false;
	int nRetCode = false; 
	CString strOtherGameFlag;
	
	KClientReport reporter;
	reporter.SerchClient();
	strOtherGameFlag = reporter.GetOtherGameFlag();
	for (int i = 0; i < GAME_NUM; i++)
	{
		KGLogPrintf(KGLOG_INFO, "%s: %c", GAME_NAME[i], strOtherGameFlag.GetAt(i));
	}
	//printf(strOtherGameFlag);



	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}