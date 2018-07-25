//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestCommon.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-23 15:00
//  Comment     :   ���Թ��̵Ĺ���ͷ�ļ�,��Ҫ����һЩ���õ��ַ���������ͽӿ�
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestCommon_
#define _KTestCommon_

//#include <Windows.h>
#include "stdafx.h"
#include <tchar.h>
#include "kglog.h"
#include "UpdateDefine.h"
#include "KFileMD5Maker.h"

//ע�����
const HKEY REG_ROOT_KEY					= HKEY_LOCAL_MACHINE;					//����
const TCHAR REG_SUB_KEY[]				= _T("SOFTWARE\\Kingsoft\\jx3\\zhcn");	//�Ӽ�
const TCHAR REG_INSTALL_PATH_NAME[]		= _T("installPath");					//ֵ����ڼ�¼��װ·��
const TCHAR REG_CURRENT_VERSION_NAME[]  = _T("version");						//ֵ����ڼ�¼�汾��Ϣ

const TCHAR GAMEUPDATER_PROC_NAME[]	 = _T("gameupdater.exe");//���³���Ľ�����

const TCHAR PACKAGE_CONFIG[]		 = _T("package.ini");	//�������ļ�����
const TCHAR PACKAGE_CONFIG_SECTION[] = _T("SO3Client");		//�������ļ�section
const TCHAR PACKAGE_CONFIG_KEY0[]	 = _T("0");				//��¼pak�����Ƶ�key
const TCHAR PACKAGE_CONFIG_KEY1[]	 = _T("1");				//��¼pak�����Ƶ�key

const TCHAR UPDATE_DLL[]  = _T("update.dll");				//���½ӿڶ�̬��
const TCHAR UPDATER_SWF[] = _T("update.swf");				//Flash��Դ�ļ�
const TCHAR TEST_PACK_1[] = _T("update_1.pak");				//�ϲ�Ŀ�������
const TCHAR TEST_PACK_2[] = _T("update_2.pak");				//���Ժϲ�������
const TCHAR TEST_PACK_3[] = _T("update_3.pak");				//���Էְ�����(2G��С)

const TCHAR TEST_RESOURCE_DIR[] = _T("TestingRes");			//������ԴĿ¼����
const TCHAR TEST_SPACE_DIR[]	= _T("TestingSpace");		//���Կռ�Ŀ¼����
const TCHAR TEMP_UPDATE_DIR[]	= _T("update");				//����ʱ����ʱĿ¼
const TCHAR PAK_DIR[]			= _T("pak");				//����ʱ��pak��Ŀ¼

const TCHAR SERVER_NAME[]		= _T("httpsrv.exe");			//�Խ�Http������������
const TCHAR REMOTE_PATH[]		= _T("http://127.0.0.1/jx3/");	//�Խ�Http��������Ŀ¼
const TCHAR VERSION_LINE_NAME[]	= _T("zhcn");					//�汾������
const TCHAR DOWNLOAD_URL[]		= _T("http://127.0.0.1/jx3/zhcn/");

const int MAX_TIMEOUT	 = 1000 * 60 * 30;					//�ȴ���ʱʱ��
const int MAX_URL_LANGTH = 255;								//URL�������ֵ

extern TCHAR g_szAppPath[MAX_PATH];							//��������·��
extern TCHAR g_szTestingResDir[MAX_PATH];					//������Դ·��
extern TCHAR g_szTestingSpaceDir[MAX_PATH];					//���Կռ�·��
extern TCHAR g_szRegInstallPath[MAX_PATH];					//ע����м�¼��ԭʼ��װĿ¼
extern TCHAR g_szRegVersion[MAX_VERSION_LENGTH];			//ע����м�¼��ԭʼ�汾��Ϣ

static const int UPDATE_FILE_COUNT = 10;
//���µ��ļ�����
static const TCHAR* UPDATE_FILE_NAME[] = 
{
	_T("Dumper.dll"),
	_T("GameConfig.exe"),
	_T("IPPVideo.dll"),
	_T("KG3DEngine.dll"),
	_T("KG3DSound.dll"),
	_T("KGUI.dll"),
	_T("JX3Client.exe"),
	_T("JX3Interaction.dll"),
	_T("JX3Represent.dll"),
	_T("JX3UI.dll")
};
//���µ��ļ����ƶ�Ӧ��MD5ֵ
static const TCHAR* UPDATE_FILE_MD5[] = 
{
	_T("f242e43d6e75f918bed7158977b944d7"),
	_T("b6dbcd1ce99205d6be08a9732466a0cf"),
	_T("b96d3bb1c4a63b018d16541119c277cf"),
	_T("c1dddb6d05f0fbc69884dea8cd253275"),
	_T("9e6eb29d0ea5489c8dc68ff376b75b91"),
	_T("0c986896999eeeb404b220103fb0eb4d"),
	_T("51795e8b0131f3aa760548132d079723"),
	_T("06d212204fb7736ea1567fd91bfa79ef"),
	_T("abfca18725ec5eb68b15e3c35801e7a2"),
	_T("b639db83001bccdcaf16fe185d60134c")
};

namespace KTestCommon
{
	//�޸İ汾��Ϣ�ļ�version.cfg�еİ汾�ţ���Sword3.version���ֵ
	int ModifyVersionFile(const TCHAR cszFilePath[], const TCHAR cszVersion[]);
	// ��ȡ��ǰ�İ汾��
	int GetCurVersion(const TCHAR cszFilePath[], OUT TCHAR* pRetCurVersion);

	//������Ĳ�����Դ����ԴĿ¼���Ƶ����Կռ�ָ����Ŀ¼��
	//cszResName ������Դ�ļ�����Դ�ļ����µ�����
	//cszTestName ���ƺ�Ĳ�����Դ�ļ������ɰ���Ŀ¼����"pak\\update_1"�������Ϊ�մ�������ԭ��Դ�ļ������Ƶ����Կռ��Ŀ¼��
	int PrepareTestResoure(const TCHAR cszResName[], const TCHAR cszTestName[] = "");

	//���Ŀ¼
	int CleanPath(const TCHAR szPath[]);
	int CleanPathExceptLogAndPak(const TCHAR cszPath[]);

	//��Ŀ¼�������ļ������޸�Ϊֻ��
	//dwFileAttributes : FILE_ATTRIBUTE_HIDDEN ,FILE_ATTRIBUTE_NORMAL ,FILE_ATTRIBUTE_READONLY, 
	//					 FILE_ATTRIBUTE_SYSTEM,FILE_ATTRIBUTE_TEMPORARY
	int SetAllFilesAttributes(const TCHAR cszPath[], DWORD dwFileAttributes);

	//ע������
	int SaveRegData();					//����汾��Ϣ�Ͱ�װĿ¼
	int RestoreRegData();				//��ԭ�汾��Ϣ�Ͱ�װĿ¼
	int ModifyRegKey(const TCHAR cszValueName[], const TCHAR cszValue[]);
	int DelRegValue(const TCHAR cszValueName[]);

	//�ȴ������������
	int WaitForProcessStarted(HANDLE hProcess);

	//�رս��̲��ȴ������
	int CloseAndWaitForProcess(HANDLE hProcess);

	//��֤�ļ���MD5ֵ
	int ValidateFileMD5(const TCHAR cszFilePath[], const TCHAR cszMD5[MD5_LENGTH]);

	//��֤�ְ����package.ini���޸�
	int ValidatePackageFile();

	//�ж�һ���ļ��Ƿ����
	int IsFileExists(const TCHAR* cpszFileName);

	//�رս���
	enum enumTERMINATEPROCESS
	{
		enumSuccessful = 0,
		enumProcessNameIsEmpty = 1,
		enumGetProccessIDByNameFail = 2,
		enumTerminateProcessFail = 3
	};
	int CloseProcess(const TCHAR* lpProcName);
	bool _GetPidByName(IN const TCHAR* lpName, OUT DWORD& dwPid);
	bool _ExitOtherProcess(IN DWORD dwPid);
}

#endif ///_KTestCommon_