//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestWinInetDownloader.h    
//  Version     :   1.0
//  Creater     :   linfuyong
//  Date        :   2009-5-22 11:00
//  Comment     :   ��֤���ؽ������ȷ��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestWinInetDownloader_H__
#define _KTestWinInetDownloader_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"
#include "KGU_Public.h"
#include <string>

#if defined _UNICODE || defined UNICODE
typedef std::wstring _tstring;
#else
typedef std::string _tstring;
#endif

class KTestWinInetDownloader : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestWinInetDownloader);
    CPPUNIT_TEST(TestDownloadNormal);
    CPPUNIT_TEST(TestDownloadConver);
    CPPUNIT_TEST(TestDownloadNoSavePath);
    CPPUNIT_TEST(TestDownloadErrorURL);
    CPPUNIT_TEST(TestDownloadServerClosed);
	CPPUNIT_TEST(TestDownloadBadNetwork);
	CPPUNIT_TEST(TestDownloadCloseServer);

	//CPPUNIT_TEST(TestDownload);//�ֶ�����
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//��������һ���ļ�
	void TestDownloadNormal();

	//�������أ��Ѿ�������ͬ���ֵ��ļ���
	void TestDownloadConver();

	//���Ա���·�������ڵ����
	void TestDownloadNoSavePath();

	//�������ص�ַ�쳣,URL���ɴ�
	void TestDownloadErrorURL();

	//���Է�����û�����������
	void TestDownloadServerClosed();

	//�������粻��������·�ɵ���������ع�����ַ���ļ���
	void TestDownloadBadNetwork();
	
	//û��������͹رշ�����
	void TestDownloadCloseServer();

	//��������һ���ļ�
	void TestDownload();

private:
	int _OpenServer();
	int _CloseServer();

private:
	STARTUPINFO m_ServerStartupInfo;					//http������������Ϣ
	STARTUPINFO m_WinInetDownloaderStartupInfo;			//TestWinInetDownloader����������Ϣ
	PROCESS_INFORMATION m_ServerProcessInfo;			//http������������Ϣ�ṹ��
	PROCESS_INFORMATION m_WinInetDownloaderProcessInfo;	//GameUpdater������Ϣ�ṹ��
};
#endif ///_KTestWinInetDownloader_H__