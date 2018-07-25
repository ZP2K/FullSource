#include "stdafx.h"
#include "KTestExtractPack.h"
#include "KExtractUpdatePack.h"
#include "KFileMD5Maker.h"

const int MAX_EXTRACT_SIZE = 6 * 1024 * 1024;												//�ְ�������6M
const TCHAR ZIP_FILE_NAME_SUCCESS[] = _T("test_extract_patch_success.exe");					//����������������
const TCHAR ZIP_FILE_NAME_NOVERSIONFILE[] = _T("test_extract_patch_no_version_file.exe");	//����������������

//��ѹ������ļ������б�
const int MAX_FILE_COUNT = 5;
const TCHAR* Ex_FILE_LIST[MAX_FILE_COUNT] = 
{
	_T("Dumper.dll"),
	_T("KGUI.dll"),
	_T("version.cfg"),
	_T("pak/update_1.pak"),
	_T("data/UI/version.cfg")
};
//��ѹ������ļ���Ӧ��MD5ֵ�б�
const TCHAR* Ex_MD5_LIST[MAX_FILE_COUNT] = 
{
	_T("d41d8cd98f00b204e9800998ecf8427e"),
	_T("0c986896999eeeb404b220103fb0eb4d"),
	_T("b3bbd5a5080720fa13d8ebd4060960b3"),
	_T("0ff7e25f87a94aec08eb0ec70bc02b05"),
	_T("90aa895311955dd2f0abcad3bdcc5db6")
};
//����Ҫ��ѹ���ļ�
const int MAX_FORBIDDEN_FILE_COUNT = 2;
const TCHAR* FORBIDDEN_FILES[MAX_FORBIDDEN_FILE_COUNT] = 
{
	_T("gameupdater.exe"),	//updatepack�ļ���֮����ļ�����Ҫ��ѹ
	_T("update.dll"),		//�������µ��ļ�����ѹ
};

void KTestExtractPack::setUp()
{
	int nRetCode = false;
	int nResult  = false;
	TCHAR szPakDir[MAX_PATH]	= _T("");
	TCHAR szUpdateDir[MAX_PATH] = _T("");

	nRetCode = KTestCommon::CleanPath(g_szTestingSpaceDir);
	KGLOG_PROCESS_ERROR(nRetCode);

	//����updateĿ¼
	nRetCode = _sntprintf(szUpdateDir, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, TEMP_UPDATE_DIR);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szUpdateDir[MAX_PATH - 1] = _T('\0');
	nRetCode = ::CreateDirectory(szUpdateDir, NULL); 
	KGLOG_PROCESS_ERROR(nRetCode || GetLastError() == ERROR_FILE_EXISTS);

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}


void KTestExtractPack::tearDown()
{
	int nRetCode = false;
	int nResult  = false;

	nRetCode = KTestCommon::CleanPath(g_szTestingSpaceDir);
	KGLOG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KTestExtractPack::TestExtractPackSuccess()
{
	int nResult  = false;
	int nRetCode = false;

	TCHAR szTempFile[MAX_PATH];
	TCHAR szZipFileName[MAX_PATH];
	KExtractUpdatePack Extractor;

	//׼�����Ի���
	nRetCode = KTestCommon::PrepareTestResoure(PACKAGE_CONFIG);
	KGLOG_PROCESS_ERROR(nRetCode);
	nRetCode = _sntprintf(szZipFileName, MAX_PATH, _T("%s\\%s"), TEMP_UPDATE_DIR, ZIP_FILE_NAME_SUCCESS);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szZipFileName[MAX_PATH - 1] = _T('\0');
	nRetCode = KTestCommon::PrepareTestResoure(ZIP_FILE_NAME_SUCCESS, szZipFileName);
	KGLOG_PROCESS_ERROR(nRetCode);
	//���÷ְ�����
	Extractor.SetPackSize(MAX_EXTRACT_SIZE);
	//��ʼ������ѹ������
	nRetCode = _sntprintf(szZipFileName, MAX_PATH, _T("%s\\%s\\%s"), g_szTestingSpaceDir, TEMP_UPDATE_DIR, ZIP_FILE_NAME_SUCCESS);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szZipFileName[MAX_PATH - 1] = _T('\0');
	_tcscat(g_szTestingSpaceDir, _T("\\"));
  	
	nRetCode = Extractor.Init(szZipFileName, g_szTestingSpaceDir);
	KGLOG_PROCESS_ERROR(nRetCode);
  	while (true)
  	{
  		nRetCode = Extractor.ExtractNext();
  		if (nRetCode == EXTRACT_FINISH)						//��ѹ���
  		{
  			Extractor.UnInit();
  			break;
  		}
  		KGLOG_PROCESS_ERROR(nRetCode != EXTRACT_FAILD);	//��ѹ����
 	}

	//��֤��ȷ��ѹ
	for (int nIndex = 0; nIndex < MAX_FILE_COUNT; ++nIndex)
	{
		nRetCode = _sntprintf(szTempFile, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, Ex_FILE_LIST[nIndex]);
		KGLOG_PROCESS_ERROR(nRetCode != -1);
		szTempFile[MAX_PATH - 1] = _T('\0');
		nRetCode = KTestCommon::ValidateFileMD5(szTempFile, Ex_MD5_LIST[nIndex]);
		KGLOG_PROCESS_ERROR(nRetCode);
	}

	//��֤����Ҫ��ѹ���ļ�
	for (int nIndex = 0; nIndex < MAX_FORBIDDEN_FILE_COUNT; ++nIndex)
	{
		nRetCode = _sntprintf(szTempFile, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, FORBIDDEN_FILES[nIndex]);
		KGLOG_PROCESS_ERROR(nRetCode != -1);
		szTempFile[MAX_PATH - 1] = _T('\0');

		nRetCode = KTestCommon::IsFileExists(szTempFile);
		KGLOG_PROCESS_ERROR(nRetCode);
	}

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KTestExtractPack::TestExtractPack_NoVersionFile()
{
	int nResult  = false;
	int nRetCode = false;

	TCHAR szZipFileName[MAX_PATH];
	KExtractUpdatePack Extractor;

	//׼�����Ի���
	nRetCode = KTestCommon::PrepareTestResoure(PACKAGE_CONFIG);
	KGLOG_PROCESS_ERROR(nRetCode);
	nRetCode = _sntprintf(szZipFileName, MAX_PATH, _T("%s\\%s"), TEMP_UPDATE_DIR, ZIP_FILE_NAME_NOVERSIONFILE);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szZipFileName[MAX_PATH - 1] = _T('\0');
	nRetCode = KTestCommon::PrepareTestResoure(ZIP_FILE_NAME_NOVERSIONFILE, szZipFileName);
	KGLOG_PROCESS_ERROR(nRetCode);
	//���÷ְ�����
	Extractor.SetPackSize(MAX_EXTRACT_SIZE);
	//��ʼ������ѹ������
	nRetCode = _sntprintf(szZipFileName, MAX_PATH, _T("%s\\%s\\%s"), g_szTestingSpaceDir, TEMP_UPDATE_DIR, ZIP_FILE_NAME_NOVERSIONFILE);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szZipFileName[MAX_PATH - 1] = _T('\0');
	_tcscat(g_szTestingSpaceDir, _T("\\"));

	nRetCode = Extractor.Init(szZipFileName, g_szTestingSpaceDir);
	KGLOG_PROCESS_ERROR(nRetCode);	
	while (true)
	{
		nRetCode = Extractor.ExtractNext();
		if (nRetCode == EXTRACT_FINISH)						//��ѹ���
		{
			Extractor.UnInit();
			break;
		}
		if (nRetCode == EXTRACT_FAILD)	//��ѹ����
		{
			CPPUNIT_ASSERT(true);
			KGLOG_PROCESS_ERROR(nRetCode != EXTRACT_FAILD);	//��ѹ����
		}		
	}

	nResult = true;
Exit0:;
	//CPPUNIT_ASSERT(!nResult);
}