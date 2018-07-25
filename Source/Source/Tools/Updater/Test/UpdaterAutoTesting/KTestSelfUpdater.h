//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestSelfUpdater.h    
//  Version     :   1.0
//  Creater     :   linfuyong
//  Date        :   2009-7-15 11:00
//  Comment     :   ��֤�Ը���
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestSelfUpdater_H__
#define _KTestSelfUpdater_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestSelfUpdater : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestSelfUpdater);
	CPPUNIT_TEST(TestNoUpdatePath);
	CPPUNIT_TEST(TestNoIniFile);
	//CPPUNIT_TEST(TestReadOnlyFiles);
	CPPUNIT_TEST(TestGameUpdaterIniURL);
	CPPUNIT_TEST(TestSelfUpdater_Normal);
	CPPUNIT_TEST(TestSelfUpdater_NoIni);
	CPPUNIT_TEST(TestSelfUpdater_ErrParam);
	CPPUNIT_TEST(TestSelfUpdater_ErrServerURL);
	CPPUNIT_TEST(TestVersionHigher);
	CPPUNIT_TEST(TestVersionEqual);

	//CPPUNIT_TEST(TestStartSeveralUpdateProc);	//�ֶ�����
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// ���³�������Ҹ��£�updateĿ¼�����ڵ����
	void TestNoUpdatePath();

	// ���³�������Ҹ��£�gameupdater.ini�����ڵ����
	void TestNoIniFile();

	// ���³�������Ҹ��£��ȴ��Ľ��̳����쳣
	void TestReadOnlyFiles();

	// ���³�������Ҹ��£�gameupdater.ini�����ļ���URL��
	void TestGameUpdaterIniURL();

	// KGU_SelfUpdater��Ԫ����, �������У���ִ��Init()����ִ��Run()��
	void TestSelfUpdater_Normal();

	// KGU_SelfUpdater��Ԫ����, û��gameupdater.ini�ļ�
	void TestSelfUpdater_NoIni();

	// KGU_SelfUpdater��Ԫ����, Init()������Ч��·��������
	void TestSelfUpdater_ErrParam();

	// KGU_SelfUpdater��Ԫ����, gameupdater.ini�ļ��ķ�����ip����ȷ
	void TestSelfUpdater_ErrServerURL();

	// ���ذ汾�űȷ������İ汾�Ÿ�
	void TestVersionHigher();

	// ���ذ汾����������İ汾�����
	void TestVersionEqual();

	// ���³�������Ҹ��£�ͬʱ�����������½���(�ֶ�����)
	void TestStartSeveralUpdateProc();

private:
	int PrepareExistFiles();
	int PrepareTestSpace(const TCHAR cszTestVersion[]);
	int StartProc();
	int WaitAndCloseProc(const TCHAR szLatestVersion[], UINT uMaxTimeOut);
	int ValidateFileData();
	int SetServerVersion(const TCHAR* pszVersion);
	int GetServerVersion(OUT TCHAR* pszRetVersion);

private:
	STARTUPINFO			m_ServerStartupinfo;		//http������������Ϣ
	STARTUPINFO			m_GameUpdaterStartupinfo;	//GameUpdater����������Ϣ
	PROCESS_INFORMATION m_ServerProcessInfo;		//http������������Ϣ�ṹ��
	PROCESS_INFORMATION m_GameUpdaterProcessInfo;	//GameUpdater������Ϣ�ṹ��
	BOOL				m_bRegSaved;				//��¼ԭʼ��ע���ı�־
	TCHAR				m_szServerVersion[32];		//�汾�ų��Ȳ�����32���ַ�,�磺"1-0-0-1505"
};
#endif ///_KTestSelfUpdater_H__