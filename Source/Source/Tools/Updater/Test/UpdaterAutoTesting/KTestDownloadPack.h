//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestDownloadPack.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-23 15:00
//  Comment     :   ��֤���ؽ������ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestDownloadPack_H__
#define _KTestDownloadPack_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"
#include "KHttpDownloader.h"

class KTestDownloadPack : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestDownloadPack);
	CPPUNIT_TEST(TestDownloadOnePack);
	CPPUNIT_TEST(TestDownloadSeveralPack);
	CPPUNIT_TEST(TestDownloadBoundary);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//����ͬʱ����1-10���ļ�����֤����1����������Ľ����ȷ
	void TestDownloadOnePack();
	//����ͬʱ����1-10���ļ�����֤���ض����������Ľ����ȷ�������������11����
	void TestDownloadSeveralPack();
	//����ͬʱ����9���ļ�����֤����8����9����10����������Ľ����ȷ
	void TestDownloadBoundary();

private:
	//ִ������������
	int Download(KHttpDownloader* pDownloader, const int cnPackNum);
	//�������غ�����а���MD5ֵ����ÿ��֤һ��ɾ��һ��
	int CheckAndDelZip(const int cnPackNum);

private:
	PROCESS_INFORMATION m_ServerProcessInfo;	//������������Ϣ�ṹ��
	STARTUPINFO m_ServerStartupInfo;			//������������Ϣ�ṹ��
};
#endif ///_KTestDownloadPack_H__