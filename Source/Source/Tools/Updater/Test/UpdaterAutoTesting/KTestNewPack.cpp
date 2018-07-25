#include "stdafx.h"
#include "KTestNewPack.h"
#include "KExtractUpdatePack.h"
//#include "KUtil.h"
#include "KFileMD5Maker.h"

const int MAX_EXTRACT_SIZE = 6 * 1024 * 1024;			//�ְ�������6M
const TCHAR ZIP_FILE_NAME[] = _T("sword3_c_1.0.8.1331-to-1.0.8.1332_zhcn_patch.exe");	//����������������
const TCHAR DEST_PACK_MD5[] = _T("deb12820df829931492e814b9a7ed2f1");					//Ŀ��ϲ���MD5
const TCHAR NEW_PACK_MD5[]  = _T("47e4a064f5299c6af46cab2064b78521");					//�ֳ����İ�MD5

void KTestNewPack::setUp()
{
	int nRetCode = false;
	int nResult  = false;
	TCHAR szPakDir[MAX_PATH]	= _T("");
	TCHAR szUpdateDir[MAX_PATH] = _T("");

	//����pakĿ¼
	nRetCode = _sntprintf(szPakDir, MAX_PATH, _T("%s\\%s"), g_szTestingSpaceDir, PAK_DIR);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szPakDir[MAX_PATH - 1] = _T('\0');
	nRetCode = ::CreateDirectory(szPakDir, NULL); 
	KGLOG_PROCESS_ERROR(nRetCode || GetLastError() == ERROR_FILE_EXISTS);
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


void KTestNewPack::tearDown()
{
	int nRetCode = false;
	int nResult  = false;

	nRetCode = KTestCommon::CleanPath(g_szTestingSpaceDir);
	KGLOG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KTestNewPack::TestNewPackSuccess()
{
	int nResult  = false;
	int nRetCode = false;

	TCHAR szNewPackName[MAX_PATH];
	TCHAR szDestPackName[MAX_PATH];
	TCHAR szZipFileName[MAX_PATH];
	KExtractUpdatePack Extractor;

	//׼�����Ի���
	//update_1.pak
	nRetCode = _sntprintf(szDestPackName, MAX_PATH, _T("%s\\%s"), PAK_DIR, TEST_PACK_1);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szDestPackName[MAX_PATH - 1] = _T('\0');
	nRetCode = KTestCommon::PrepareTestResoure(TEST_PACK_1, szDestPackName);
	KGLOG_PROCESS_ERROR(nRetCode);
	//package.ini
	nRetCode = KTestCommon::PrepareTestResoure(PACKAGE_CONFIG);
	KGLOG_PROCESS_ERROR(nRetCode);
	//sword3_c_1.0.8.1331-to-1.0.8.1332_zhcn_patch.exe
	nRetCode = _sntprintf(szZipFileName, MAX_PATH, _T("%s\\%s"), TEMP_UPDATE_DIR, ZIP_FILE_NAME);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szZipFileName[MAX_PATH - 1] = _T('\0');
	nRetCode = KTestCommon::PrepareTestResoure(ZIP_FILE_NAME, szZipFileName);
	KGLOG_PROCESS_ERROR(nRetCode);
	//���÷ְ�����
	Extractor.SetPackSize(MAX_EXTRACT_SIZE);
	//��ʼ������ѹ������
	nRetCode = _sntprintf(szZipFileName, MAX_PATH, _T("%s\\%s\\%s"), g_szTestingSpaceDir, TEMP_UPDATE_DIR, ZIP_FILE_NAME);
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
	//��֤
	//Ŀ����ޱ仯
	nRetCode = _sntprintf(szDestPackName, MAX_PATH, _T("%s\\%s\\%s"), g_szTestingSpaceDir, PAK_DIR, TEST_PACK_1);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szDestPackName[MAX_PATH - 1] = _T('\0');
	nRetCode = KTestCommon::ValidateFileMD5(szDestPackName, DEST_PACK_MD5);
	KGLOG_PROCESS_ERROR(nRetCode);
	//�ְ���MD5ֵ��Ԥ��һ��
	nRetCode = _sntprintf(szNewPackName, MAX_PATH, _T("%s%s\\%s"), g_szTestingSpaceDir, PAK_DIR, TEST_PACK_2);
	KGLOG_PROCESS_ERROR(nRetCode != -1);
	szNewPackName[MAX_PATH - 1] = _T('\0');
	nRetCode = KTestCommon::ValidateFileMD5(szNewPackName, NEW_PACK_MD5);
	KGLOG_PROCESS_ERROR(nRetCode);
	//package.ini�޸���֤
	nRetCode = KTestCommon::ValidatePackageFile();
	KGLOG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}
