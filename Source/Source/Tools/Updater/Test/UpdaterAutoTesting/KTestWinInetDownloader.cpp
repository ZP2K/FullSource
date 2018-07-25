#include "stdafx.h"
#include "KTestWinInetDownloader.h"
#include "KFileMD5Maker.h"
//#include "KUtil.h"
#include "KThread.h"

//�������ص������������б�
const TCHAR* FILE_LIST[] = 
{
	_T("autoupdateentry.txt"),
	_T("jx3_c_1.0.8.1443-to-1.0.8.1459_zhcn_patch.exe")
};
//�������ص��ļ���Ӧ��MD5ֵ�б�
const TCHAR* MD5_LIST[] = 
{
	_T("8239346f4d1cb73453857e18c145363a"),
	_T("c18eb667e89e95c108c59432534bac53")
};

void KTestWinInetDownloader::setUp()
{
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

void KTestWinInetDownloader::tearDown()
{
	
}

void KTestWinInetDownloader::TestDownloadNormal()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
	TCHAR szURL[MAX_PATH];

	nRetCode = _OpenServer();
	KGLOG_PROCESS_ERROR(nRetCode);

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	nRetCode = _sntprintf(szURL, MAX_PATH, _T("%s%s/%s"), REMOTE_PATH, VERSION_LINE_NAME, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//��ʼ����
	nRetCode = KGU_DownloadHttpFile(szURL, szSaveFileName);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, MD5_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
	_CloseServer();
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

void KTestWinInetDownloader::TestDownloadConver()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
	TCHAR szURL[MAX_PATH];

	//����һ���ļ������Կռ��ļ���
	nRetCode = KTestCommon::PrepareTestResoure(_T("test_conver.txt"), FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode);

	nRetCode = _OpenServer();
	KGLOG_PROCESS_ERROR(nRetCode);

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	nRetCode = _sntprintf(szURL, MAX_PATH, _T("%s%s/%s"), REMOTE_PATH, VERSION_LINE_NAME, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//��ʼ����
	nRetCode = KGU_DownloadHttpFile(szURL, szSaveFileName);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, MD5_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
	_CloseServer();
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

void KTestWinInetDownloader::TestDownloadNoSavePath()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
	TCHAR szURL[MAX_PATH];

	nRetCode = _OpenServer();
	KGLOG_PROCESS_ERROR(nRetCode);

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\NoPath\\%s"), g_szTestingSpaceDir, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	nRetCode = _sntprintf(szURL, MAX_PATH, _T("%s%s/%s"), REMOTE_PATH, VERSION_LINE_NAME, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//��ʼ����
	nRetCode = KGU_DownloadHttpFile(szURL, szSaveFileName);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, MD5_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT( !nResult );
	_CloseServer();
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

void KTestWinInetDownloader::TestDownloadErrorURL()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
	TCHAR szURL[MAX_PATH];

	nRetCode = _OpenServer();
	KGLOG_PROCESS_ERROR(nRetCode);

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	nRetCode = _sntprintf(szURL, MAX_PATH, _T("%s/%s/ErrorURL/%s"), REMOTE_PATH, VERSION_LINE_NAME, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//��ʼ����
	nRetCode = KGU_DownloadHttpFile(szURL, szSaveFileName);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, MD5_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT( !nResult );
	_CloseServer();
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

void KTestWinInetDownloader::TestDownloadServerClosed()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
	TCHAR szURL[MAX_PATH];

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	nRetCode = _sntprintf(szURL, MAX_PATH, _T("%s%s/%s"), REMOTE_PATH, VERSION_LINE_NAME, FILE_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//��ʼ����
	nRetCode = KGU_DownloadHttpFile(szURL, szSaveFileName);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, MD5_LIST[0]);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT( !nResult );
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

void ThreadDownload(void *pvArg)
{
	_tstring* pszTemp = (_tstring*)pvArg;
	KGU_DownloadHttpFile(pszTemp[0].c_str(), pszTemp[1].c_str());
}
void KTestWinInetDownloader::TestDownloadCloseServer()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
	TCHAR szURL[MAX_PATH];
	KThread ClientThread;
	_tstring vArg[2];

	nRetCode = _OpenServer();
	KGLOG_PROCESS_ERROR(nRetCode);

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, FILE_LIST[1]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	nRetCode = _sntprintf(szURL, MAX_PATH, _T("%s%s/%s"), REMOTE_PATH, VERSION_LINE_NAME, FILE_LIST[1]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//���߳̿�ʼ����һ���ϴ���ļ�(ȷ��û������͹رշ�����)
	vArg[0] = szURL;
	vArg[1] = szSaveFileName;
	nRetCode = ClientThread.Create(ThreadDownload, vArg);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��ʱ���
	Sleep(3000);

	//û������֮ǰ�ر�
	nRetCode = _CloseServer();
	KGLOG_PROCESS_ERROR(nRetCode);

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, MD5_LIST[1]);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT( !nResult );
	ClientThread.Destroy();
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

void KTestWinInetDownloader::TestDownloadBadNetwork()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
	TCHAR szURL[MAX_PATH] = _T("http://connect.creativelabs.com/openal/Downloads/oalinst.zip");
	TCHAR szMD5[] = _T("47f53b4b655a9f8124687141b0f94d92");

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, _T("oalinst.zip"));
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//��ʼ����
	nRetCode = KGU_DownloadHttpFile(szURL, szSaveFileName);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, szMD5);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

//�ֶ�����
void KTestWinInetDownloader::TestDownload()
{
	int nResult  = false;
	int nRetCode = false;
	TCHAR szSaveFileName[MAX_PATH];
//	TCHAR szURL[MAX_PATH] = _T("http://127.0.0.1/zhcn/jx3_c_1.0.8.1379-to-1.0.8.1380_zhcn_patch.exe");
	TCHAR szURL[MAX_PATH] = _T("http://10.20.80.141/jx3_c_1.0.8.1379-to-1.0.8.1380_zhcn_patch.exe");
//	TCHAR szURL[MAX_PATH] = _T("http://jx3.autoupdate.kingsoft.com/jx3/zhcn/jx3_c_1.0.8.1443-to-1.0.8.1445_zhcn_patch.exe");

// 	nRetCode = _OpenServer();
// 	KGLOG_PROCESS_ERROR(nRetCode);

	Sleep(4000);

	//�������ص�ַ�뱣���ַ
	nRetCode = _sntprintf(szSaveFileName, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, FILE_LIST[1]);
	KGLOG_PROCESS_ERROR(nRetCode != -1);

	//��ʼ����
	nRetCode = KGU_DownloadHttpFile(szURL, szSaveFileName);
	KGLOG_PROCESS_ERROR(nRetCode && "WinInet Download Error!");

	//��֤
	nRetCode = KTestCommon::ValidateFileMD5(szSaveFileName, MD5_LIST[1]);
	KGLOG_PROCESS_ERROR(nRetCode && "MD5 Error!");

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
//	_CloseServer();
//	KTestCommon::CleanPath(g_szTestingSpaceDir);
}

int KTestWinInetDownloader::_OpenServer()
{
	int nResult  = false;
	int nRetCode = false;
	BOOL bServerStarted = false;
	TCHAR szServerName[MAX_PATH];

	ZeroMemory(&m_ServerStartupInfo, sizeof(m_ServerStartupInfo));
	m_ServerStartupInfo.cb = sizeof(m_ServerStartupInfo);
	ZeroMemory(&m_ServerProcessInfo, sizeof(m_ServerProcessInfo));
	//����http����������
	nRetCode = _sntprintf(szServerName, MAX_PATH, _T("%s\\%s"), g_szAppPath, SERVER_NAME);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szServerName[MAX_PATH - 1] = _T('\0');
	nRetCode = ::CreateProcess(
		NULL,
		szServerName,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&m_ServerStartupInfo,
		&m_ServerProcessInfo
		);
	KGLOG_PROCESS_ERROR(nRetCode);
	bServerStarted = true;
	//�ȴ������������������
	nRetCode = KTestCommon::WaitForProcessStarted(m_ServerProcessInfo.hProcess);
	KGLOG_PROCESS_ERROR(nRetCode);
	Sleep(3000);//��ʱ�������ʱ��������������������ʱ���ᵼ�����Ӳ��Ϸ�����������ʧ��

	nResult = true;
Exit0:
	if (!nResult && bServerStarted)
	{
		KTestCommon::CloseAndWaitForProcess(m_ServerProcessInfo.hProcess);
		m_ServerProcessInfo.hProcess = NULL;
	}
	//CPPUNIT_ASSERT(nResult);
	return nResult;
}

int KTestWinInetDownloader::_CloseServer()
{
	int nResult  = false;
	int nRetCode = false;

	//�رշ���������
	if (m_ServerProcessInfo.hProcess != NULL)
	{
		nRetCode = KTestCommon::CloseAndWaitForProcess(m_ServerProcessInfo.hProcess);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "CloseAndWait Server Process Failed");
		}
		m_ServerProcessInfo.hProcess = NULL;
	}

	nResult = true;
//Exit0:
	//CPPUNIT_ASSERT(nResult);
	return nResult;
}
