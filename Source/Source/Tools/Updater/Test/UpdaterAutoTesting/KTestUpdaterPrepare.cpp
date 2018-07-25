#include "stdafx.h"
#include "KTestUpdaterPrepare.h"
#include "KRegInfo.h"
#include "KConfig.h" 

//��������
const TCHAR CFG_HIGH_VERSION[]		= _T("1-0-8-1341");			//�汾��Ϣ�ļ���version.cfg�������ð汾��
const TCHAR CFG_LOW_VERSION[]		= _T("1-0-8-1300");
const TCHAR REG_HIGH_VERSION[]		= _T("1.0.8.1341");			//ע�������ð汾��
const TCHAR REG_LOW_VERSION[]		= _T("1.0.8.1300");
const TCHAR VALIDATE_HIGH_VERSION[]	= _T("1.0.8.1341");			//��֤����汾��
const TCHAR VALIDATE_LOW_VERSION[]	= _T("1.0.8.1300");
const TCHAR GU_CONFIG_SECTION[]		= _T("server_1");			//�����ļ���gameupdater.ini���з�������Section���� 
const TCHAR GU_CONFIG_KEY[]			= _T("ip");					//�����ļ���gameupdater.ini����ip��Key����

void KTestUpdaterPrepare::setUp()
{
	int nRetCode = false;
	int nResult  = false;
	m_bUpdateDirCreated = false;
	m_szAppDir[0]		 = _T('\0');
	m_szGUconfig[0]		 = _T('\0');
	m_szVersionFile[0]	 = _T('\0');
	m_szUpdateTempDir[0] = _T('\0');
	//������֤�õ���ʱ����Ŀ¼
	nRetCode = _sntprintf(m_szUpdateTempDir, MAX_PATH, _T("%s\\%s\\"), g_szAppPath, TEMP_UPDATE_DIR);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	m_szUpdateTempDir[MAX_PATH - 1] = _T('\0');
	nRetCode = ::CreateDirectory(m_szUpdateTempDir, NULL);
	KGLOG_PROCESS_ERROR(nRetCode || GetLastError() == ERROR_FILE_EXISTS);
	m_bUpdateDirCreated = true;
	//������֤�õĳ�������Ŀ¼����ȫ�ֵĳ�������Ŀ¼��g_szAppPath��������һ��'\'
	nRetCode = _sntprintf(m_szAppDir, MAX_PATH, _T("%s\\"), g_szAppPath);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	m_szAppDir[MAX_PATH - 1] = _T('\0');

	nResult = true;
Exit0:
	if (!nResult && m_bUpdateDirCreated)
	{
		nRetCode = ::RemoveDirectory(m_szUpdateTempDir);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Remove Update Directory Failed");
		}
	}
	CPPUNIT_ASSERT(nResult);
}
void KTestUpdaterPrepare::tearDown()
{
	int nRetCode = false;
	int nResult  = false;

	if (m_bUpdateDirCreated)
	{
		nRetCode = ::RemoveDirectory(m_szUpdateTempDir);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Remove Update Directory Failed");
		}
	}

	nResult = true;
//Exit0:
	CPPUNIT_ASSERT(nResult);
}

int KTestUpdaterPrepare::PrepareVersionFile(const TCHAR cszVersion[])
{
	int nResult  = false;
	int nRetCode = false;
	BOOL bPrepareVersionFile = false;
	TCHAR szTestResFile[MAX_PATH];

	ASSERT(cszVersion);

	//׼�������ð汾��Ϣ�ļ�
	nRetCode = _sntprintf(szTestResFile, MAX_PATH, _T("%s\\%s"), g_szTestingResDir, VERSION_FILE_NAME);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szTestResFile[MAX_PATH - 1] = _T('\0');
	nRetCode = _sntprintf(m_szVersionFile, MAX_PATH, _T("%s\\%s"), g_szAppPath, VERSION_FILE_NAME);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	m_szVersionFile[MAX_PATH - 1] = _T('\0');
	nRetCode = ::CopyFile(szTestResFile, m_szVersionFile, FALSE);
	KGLOG_PROCESS_ERROR(nRetCode);
	bPrepareVersionFile = true;
	//�޸İ汾�����ڲ���
	nRetCode = KTestCommon::ModifyVersionFile(m_szVersionFile, cszVersion);
	KGLOG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	if (!nResult && bPrepareVersionFile)
	{
		//ɾ���汾��Ϣ�ļ�
		nRetCode = ::DeleteFile(m_szVersionFile);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Delete version.cfg failed");
		}
	}
	return nResult;
}

int KTestUpdaterPrepare::PrepareGUconfigFile(const TCHAR cszRemotePath[])
{
	int nResult  = false;
	int nRetCode = false;
	BOOL bPrepareGUconfigFile = false;
	TCHAR szTestResFile[MAX_PATH];

	ASSERT(cszRemotePath);

	nRetCode = _sntprintf(szTestResFile, MAX_PATH, _T("%s\\%s"), g_szTestingResDir, CONFIG_FILE_NAME);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szTestResFile[MAX_PATH - 1] = _T('\0');
	nRetCode = _sntprintf(m_szGUconfig, MAX_PATH, _T("%s\\%s"), g_szAppPath, CONFIG_FILE_NAME);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	m_szGUconfig[MAX_PATH - 1] = _T('\0');
	nRetCode = ::CopyFile(szTestResFile, m_szGUconfig, FALSE);
	KGLOG_PROCESS_ERROR(nRetCode);
	bPrepareGUconfigFile = true;
	//�޸Ĳ����������ļ��ķ�����ip��ַ�����ڲ���
	nRetCode = ::WritePrivateProfileString(
		GU_CONFIG_SECTION, 
		GU_CONFIG_KEY, 
		cszRemotePath, 
		m_szGUconfig);
	KGLOG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	if (!nResult && bPrepareGUconfigFile)
	{
		//ɾ�������ļ�
		nRetCode = ::DeleteFile(m_szGUconfig);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Delete gameupdater.ini failed");
		}
	}
	return nResult;
}

void KTestUpdaterPrepare::TestPrepareSuccess()
{
	int nResult  = false;
	int nRetCode = false;
	BOOL bPrepareVersionFile  = false;
	BOOL bPrepareGUconfigFile = false;
	KConfig Config;
	//׼���������Ի���
	nRetCode = PrepareVersionFile(CFG_HIGH_VERSION);//1-0-8-1341
	KGLOG_PROCESS_ERROR(nRetCode);
	bPrepareVersionFile = true;
	nRetCode = PrepareGUconfigFile(REMOTE_PATH);
	KGLOG_PROCESS_ERROR(nRetCode);
	bPrepareGUconfigFile = true;
	//ִ�м���
	nRetCode = Config.LoadConfig();
	KGLOG_PROCESS_ERROR(nRetCode);
	//��֤���
	nRetCode = _tcsncmp(Config.GetCurrentVersion(), VALIDATE_HIGH_VERSION, MAX_VERSION_LENGTH);//1.0.8.1341
	KGLOG_PROCESS_ERROR(nRetCode == 0);
	nRetCode = _tcsncmp(Config.GetVersionLineName(), VERSION_LINE_NAME, MAX_PATH);
	KGLOG_PROCESS_ERROR(nRetCode == 0);
	nRetCode = _tcsncmp(Config.GetAppPath(), m_szAppDir, MAX_PATH);
	KGLOG_PROCESS_ERROR(nRetCode == 0);
	nRetCode = _tcsncmp(Config.GetUpdatePath(), m_szUpdateTempDir, MAX_PATH);
	KGLOG_PROCESS_ERROR(nRetCode == 0);
	nRetCode = _tcsncmp(Config.GetRemotePath(), DOWNLOAD_URL, MAX_PATH);
	KGLOG_PROCESS_ERROR(nRetCode == 0);

	nResult = true;
Exit0:
	if (bPrepareVersionFile)
	{
		//ɾ���汾��Ϣ�ļ�
		nRetCode = ::DeleteFile(m_szVersionFile);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Delete version.cfg failed");
		}
	}
	if (bPrepareGUconfigFile)
	{
		//ɾ�������ļ�
		nRetCode = ::DeleteFile(m_szGUconfig);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Delete gameupdater.ini failed");
		}
	}
	
	CPPUNIT_ASSERT(nResult);
}

void KTestUpdaterPrepare::TestLostVersionFile()
{
	int nResult  = false;
	int nRetCode = false;
	BOOL bPrepareGUconfigFile = false;
	size_t uLength = 0;
	KConfig Config;

	nRetCode = PrepareGUconfigFile(REMOTE_PATH);
	KGLOG_PROCESS_ERROR(nRetCode);
	bPrepareGUconfigFile = true;

	nRetCode = Config.LoadConfig();
	KGLOG_PROCESS_ERROR(nRetCode == false);
	uLength = _tcslen(Config.GetCurrentVersion());
	KGLOG_PROCESS_ERROR(uLength == 0);
	KGLogPrintf(KGLOG_INFO, "�ɹ���֤\"�޷���ȡ�汾��Ϣ�ļ��еİ汾\"");

	nResult = true;
Exit0:
	if (bPrepareGUconfigFile)
	{
		//ɾ�������ļ�
		nRetCode = ::DeleteFile(m_szGUconfig);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Delete gameupdater.ini failed");
		}
	}
	CPPUNIT_ASSERT(nResult);
}

void KTestUpdaterPrepare::TestLostConfigFile()
{
	int nResult  = false;
	int nRetCode = false;
	BOOL bPrepareVersionFile = false;
	size_t uLength = 0;
	KConfig Config;

	nRetCode = PrepareVersionFile(CFG_HIGH_VERSION);//1-0-8-1341
	KGLOG_PROCESS_ERROR(nRetCode);
	bPrepareVersionFile = true;

	nRetCode = Config.LoadConfig();
	KGLOG_PROCESS_ERROR(nRetCode == false);
	uLength = _tcslen(Config.GetRemotePath());
	KGLOG_PROCESS_ERROR(uLength == 0);
	KGLogPrintf(KGLOG_INFO, "�ɹ���֤\"�޷���ȡ�����ļ��еķ�����ip��ַ\"");

	nResult = true;
Exit0:
	if (bPrepareVersionFile)
	{
		//ɾ���汾��Ϣ�ļ�
		nRetCode = ::DeleteFile(m_szVersionFile);
		if (!nRetCode)
		{
			KGLogPrintf(KGLOG_ERR, "Delete version.cfg failed");
		}
	}
	CPPUNIT_ASSERT(nResult);
}
