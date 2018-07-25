#include "stdafx.h"
#include "toolhelp.h"
#include "KClientReport.h"
#include "md5.h"
#include "KThread.h"

#include <io.h>
#include <shlobj.h>
#include <shobjidl.h>

#define GAME_CLIENT_INFO(nID, pExeName, pRegKey, pRegSubKey, pRegValue, pTestVaule)   \
		m_ClientFlags[(nID)].GameExeName.push_back((pExeName));             \
		m_ClientFlags[(nID)].GameRegisterInfo.RegKey     = (pRegKey);    \
		m_ClientFlags[(nID)].GameRegisterInfo.RegSubKey  = (pRegSubKey); \
		m_ClientFlags[(nID)].GameRegisterInfo.RegValue   = (pRegValue);  \
		m_ClientFlags[(nID)].GameRegisterInfo.TestVaule  = (pTestVaule);

KClientReport::KClientReport(void)
{
	GAME_CLIENT_INFO(GAME_NAME_SD_GAN_DA_OL, "GUpdate_CHINA.exe", HKEY_LOCAL_MACHINE,  //SD�Ҵ�
		_T("SOFTWARE\\9you\\SD�Ҵ�Online"), NULL, NULL);

	GAME_CLIENT_INFO(GAME_NAME_CHU_QIU_Q_ZHUAN_OL, "CQOnline.exe", HKEY_LOCAL_MACHINE,   //����Q��
		_T("SOFTWARE\\Kingsoft\\cqonline"), _T("Version"), NULL);

	GAME_CLIENT_INFO(GAME_NAME_SWORLD1, "jxonline.exe", HKEY_LOCAL_MACHINE,   //JX1
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\������Ե�����ͻ���"), _T("URLInfoAbout"), _T("http://www.kingsoft.com"));

	GAME_CLIENT_INFO(GAME_NAME_SWORLD2, "Jxonline2.exe", HKEY_LOCAL_MACHINE,   //JX2
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\������Ե�����2"), _T("DisplayName"), _T("������Ե�����2"));

	GAME_CLIENT_INFO(GAME_NAME_JXSJ, "jxonline.exe", HKEY_LOCAL_MACHINE,      //��������
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\��������"), _T("DisplayName"), _T("��������"));

	GAME_CLIENT_INFO(GAME_NAME_MENG_HUAN_XI_YOU, "my.exe", HKEY_LOCAL_MACHINE,   //�λ�����
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\�λ�����_is1"), _T("URLInfoAbout"), _T("http://xyq.163.com/"));

	GAME_CLIENT_INFO(GAME_NAME_WOW, "WoW.exe", HKEY_LOCAL_MACHINE,   //ħ������
		_T("SOFTWARE\\Blizzard Entertainment\\World of Warcraft"), _T("GamePath"), NULL);

	GAME_CLIENT_INFO(GAME_NAME_WOW, "Launcher.exe", HKEY_LOCAL_MACHINE,  //ħ��������2��EXE
		_T("SOFTWARE\\Blizzard Entertainment\\World of Warcraft"), _T("GamePath"), NULL);

	GAME_CLIENT_INFO(GAME_NAME_MO_YU, "Play.exe", HKEY_LOCAL_MACHINE,  //ħ��
		_T("SOFTWARE\\Tqdigital\\Soul"), _T("Path"), NULL);

	GAME_CLIENT_INFO(GAME_NAME_QI_JI_SHI_JIE, "SUN.exe", HKEY_USERS,  //�漣����
		_T("S-1-5-21-3755760448-3218920860-3279771573-500\\Software\\Ahnlab\\HShield"), _T("packver"), NULL);

	GAME_CLIENT_INFO(GAME_NAME_TIAN_XIA2, "tw2launch.exe", HKEY_LOCAL_MACHINE,  //���·�
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\���·�"), _T("URLInfoAbout"), _T("http://tx2.163.com"));

	GAME_CLIENT_INFO(GAME_NAME_XIN_LONG_QI_SHI_OL, "������ʿonline.exe", HKEY_LOCAL_MACHINE,  //������ʿonline
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\������ʿOnline"), _T("URLInfoAbout"), _T("http://www.lqsonline.com.cn"));

	GAME_CLIENT_INFO(GAME_NAME_AION, "PlayNCLauncher.exe", HKEY_LOCAL_MACHINE, //����֮��
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\AION"), _T("URLInfoAbout"), _T("http://aion.sdo.com"));

	GAME_CLIENT_INFO(GAME_NAME_ZHU_XIAN, "patcher.exe", HKEY_LOCAL_MACHINE, //����
		_T("SOFTWARE\\Classes\\ZPWUpdatePack"), _T(""), _T("����"));

	GAME_CLIENT_INFO(GAME_NAME_DNF, "DNFchina.exe", HKEY_LOCAL_MACHINE, //DNF
		_T("SOFTWARE\\Tencent\\DNF"), _T("Path"), NULL);

	GAME_CLIENT_INFO(GAME_NAME_XIAN_JIAN_OL, "palonline.exe", HKEY_LOCAL_MACHINE,  //�ɽ�
		_T("SOFTWARE\\Microsoft\\WINDOWS\\CURRENTVERSION\\Uninstall\\{0650D9A9-07CC-4297-9040-140BA1084526}"), NULL, NULL);

	GAME_CLIENT_INFO(GAME_NAME_RE_XUE_CHUAN_QI, "Mir.exe", HKEY_LOCAL_MACHINE, //��Ѫ����
		_T("SOFTWARE\\snda\\Legend of mir"), _T("Path"), NULL);

	GAME_CLIENT_INFO(GAME_NAME_TIAN_LONG_BA_BU, "Launch.exe", HKEY_LOCAL_MACHINE,  //�����˲�
		_T("tianlongbabu"), NULL, NULL);

	GAME_CLIENT_INFO(GAME_NAME_JX2_WZ, "Jx2wzOnline.exe", HKEY_LOCAL_MACHINE,  //����2�⴫
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Kingsoft\\Jx2wzOnline\\Jx2wzOnline.exe"), NULL, NULL);

	m_nInstallMode = imUnknown;
	m_nIsFirstRun = false;
}

KClientReport::~KClientReport(void)
{
    std::vector<THREAD_DATA *>::iterator it;
    THREAD_DATA *pDeleteTarget = NULL;

    for (it = m_Threads.begin(); it != m_Threads.end(); ++it)
    {
        pDeleteTarget = *it;

        if (pDeleteTarget)
        {
            delete pDeleteTarget;
            pDeleteTarget = NULL;
        }
    }
}

int KClientReport::ReportClientInfo(const TCHAR* pszCurrentVersion)
{
	int nResult  = false;
	int nRetCode = false;
	HRESULT hRetCode = S_FALSE;
	CString szFullURL;

	KGLOG_PROCESS_ERROR(pszCurrentVersion);

	nRetCode = _GetInstallMode();
	KGLOG_PROCESS_ERROR(nRetCode);

	KG_PROCESS_SUCCESS(!m_nIsFirstRun);

	hRetCode = m_SystemInfo.GetHardDiskId(m_strUUID); //ȡ��UUID
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	nRetCode = _WriteUUIDToRegistry();
	KGLOG_PROCESS_ERROR(nRetCode);

	nRetCode = _GetDesktopAndStartmenuShortcutsPoint();
	KGLOG_PROCESS_ERROR(nRetCode);

	nRetCode = _SearchInstalledGames();
	KGLOG_PROCESS_ERROR(nRetCode);

	hRetCode = m_SystemInfo.GetDisplayInfo(m_strVideoCard);
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	szFullURL.Format(_NEW_INSTALL, m_strUUID, m_nInstallMode, pszCurrentVersion, m_strVideoCard, m_strOtherGame);

	DoReport(szFullURL);

Exit1:
	nResult = true;
Exit0:
	return nResult;
}

int KClientReport::ReportUpdateInfo(const TCHAR* pszCurrentVersion)
{
	int nResult  = false;
	HRESULT hRetCode = S_FALSE;
	CString szFullURL;

	KGLOG_PROCESS_ERROR(pszCurrentVersion);

	hRetCode = m_SystemInfo.GetHardDiskId(m_strUUID); //ȡ��UUID
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	szFullURL.Format(_CHANGE_VERSION, m_strUUID, pszCurrentVersion);

	DoReport(szFullURL);

	nResult = true;
Exit0:
	return nResult;
}

int KClientReport::_SearchInstalledGames()
{
	int nExistFlags = false;
	std::set<std::string>::const_iterator itor;
	std::vector<std::string>::const_iterator ExeNameItor;
	TCHAR szTempFile[MAX_PATH];

	for (int i = GAME_NAME_BEGIN; i < GAME_NAME_END; ++i)
	{
		nExistFlags = _CheckRegValueExist(&m_ClientFlags[i].GameRegisterInfo);
		if (!nExistFlags)
		{
			for (
				ExeNameItor = m_ClientFlags[i].GameExeName.begin(); 
				ExeNameItor != m_ClientFlags[i].GameExeName.end();
				++ExeNameItor
			)
			{
				_tcscpy(szTempFile, (*ExeNameItor).c_str());
				itor = m_ShortcutsPointList.find(_tcsupr(szTempFile));
				if (itor != m_ShortcutsPointList.end())
				{
					nExistFlags = true;
					break;
				}
			}
		}

		m_strOtherGame.AppendChar(nExistFlags + _T('0'));
	}

	KGLogPrintf(KGLOG_INFO, "ID: %s \n", m_strOtherGame);
	return true;
}

int KClientReport::_CheckRegValueExist(PGAMECLIENTREGINFO pRegInfo)
{ 
	int   nRetCode  = false;
	int   nResult   = false;
	HKEY  hKey      = NULL;
	DWORD dwSizeReaded = 0;
	TCHAR szValue[_PATH_MAX];

	KGLOG_PROCESS_ERROR(pRegInfo);
	
	nRetCode = RegOpenKeyEx(pRegInfo->RegKey, pRegInfo->RegSubKey, 0, KEY_READ, &hKey);
	KG_PROCESS_ERROR(nRetCode == ERROR_SUCCESS);

	if (pRegInfo->RegValue)
	{
		dwSizeReaded = _PATH_MAX * sizeof(TCHAR);
		nRetCode = RegQueryValueEx(hKey, pRegInfo->RegValue, NULL, NULL, (LPBYTE)szValue, &dwSizeReaded);
		KG_PROCESS_ERROR(nRetCode == ERROR_SUCCESS);
	}

	if (pRegInfo->TestVaule)
	{
		nRetCode = _tcsnicmp(szValue, pRegInfo->TestVaule, _tcslen(pRegInfo->TestVaule));
		KG_PROCESS_ERROR(nRetCode == 0);
	}

	nResult = true;
Exit0:
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}	
	return nResult;
}

int KClientReport::_GetInstallMode()
{
	int nRetCode = false;

	//����ǰ�װ����װ�ģ�����copy��װ��
	GAMECLIENTREGINFO ClientInfo = {HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Kingsoft\\JX3"), NULL, NULL};	
	nRetCode = _CheckRegValueExist(&ClientInfo);
	if (nRetCode)
		m_nInstallMode = imInstall;
	else
		m_nInstallMode = imXCopy;

	//����Ƿ����״�����
	GAMECLIENTREGINFO ClientInfo2 = {HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Kingsoft\\JX3"), _T("TESTVERSION"), NULL};
	nRetCode = _CheckRegValueExist(&ClientInfo2);
	if (nRetCode)
		m_nIsFirstRun = false;
	else
		m_nIsFirstRun = true;

	return true;
}


int KClientReport::_WriteUUIDToRegistry()
{
	int   nRetCode = false;
	int   nResult  = false;
	HKEY  hKey     = NULL;
	DWORD dwDisp   = 0;
	
	nRetCode = RegCreateKeyEx(
		HKEY_LOCAL_MACHINE, 
		"SOFTWARE\\Kingsoft\\JX3", 
		0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE, NULL, &hKey, &dwDisp
	);
	KGLOG_PROCESS_ERROR(nRetCode == ERROR_SUCCESS);

	nRetCode = RegSetValueEx(hKey, "UUID", 0, REG_SZ, (BYTE *)m_strUUID.GetBuffer(), m_strUUID.GetLength());
	KGLOG_PROCESS_ERROR(nRetCode == ERROR_SUCCESS);

	nRetCode = RegSetValueEx(hKey, "TESTVERSION", 0, REG_SZ, (BYTE *)"1", 1);
	KGLOG_PROCESS_ERROR(nRetCode == ERROR_SUCCESS);

	nResult = true;
Exit0:
	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}	
	return nResult;
}

int KClientReport::_GetDesktopAndStartmenuShortcutsPoint()
{
	int nRetCode = false;
	int nResult  = false;
	LPITEMIDLIST ppidl;
	unsigned  uLen     = 0;
	HRESULT   hRetCode = S_FALSE;

	//����ǰ�û��Ŀ�ʼ�˵�
	hRetCode = SHGetSpecialFolderLocation(NULL, CSIDL_STARTMENU, &ppidl);
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	nRetCode = SHGetPathFromIDList(ppidl, m_szCurrentPath);
	KGLOG_PROCESS_ERROR(nRetCode);

	uLen = (int)_tcslen(m_szCurrentPath);
	nRetCode = _GetPathShortcuts(uLen);
	KGLOG_PROCESS_ERROR(nRetCode);

	//����ȫ���û��Ŀ�ʼ�˵�
	hRetCode = SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_STARTMENU, &ppidl);
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);
	
	nRetCode = SHGetPathFromIDList(ppidl, m_szCurrentPath);
	KGLOG_PROCESS_ERROR(nRetCode);

	uLen = (int)_tcslen(m_szCurrentPath);
	nRetCode = _GetPathShortcuts(uLen);
	KGLOG_PROCESS_ERROR(nRetCode);

	//����ǰ�û�������
	hRetCode = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl);
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	nRetCode = SHGetPathFromIDList(ppidl, m_szCurrentPath);
	KGLOG_PROCESS_ERROR(nRetCode);

	uLen = (int)_tcslen(m_szCurrentPath);
	nRetCode = _GetPathShortcuts(uLen);
	KGLOG_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	return nResult;
}

int KClientReport::_GetPathShortcuts(unsigned uFullPathFileNameLen)
{
	int          nRetCode    = false;
	int          nResult     = false;
	HANDLE       hFindHandle = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA FindData;

	KGLOG_PROCESS_ERROR(uFullPathFileNameLen + 5 < sizeof(m_szCurrentPath));

	if (
		m_szCurrentPath[uFullPathFileNameLen - 1] != _T('\\') &&
		m_szCurrentPath[uFullPathFileNameLen - 1] != _T('/')
	)
	{
		m_szCurrentPath[uFullPathFileNameLen++] = _T('\\');
		m_szCurrentPath[uFullPathFileNameLen]   = _T('\0');	
	}

	_tcscpy(m_szCurrentPath + uFullPathFileNameLen, _T("*.*"));

	hFindHandle = ::FindFirstFile(m_szCurrentPath, &FindData);
	if (hFindHandle == INVALID_HANDLE_VALUE)
		goto Exit1;

	while (true)
	{
		unsigned uNewLength = 0;

		if (FindData.cFileName[0] != '.')
		{
			uNewLength = (unsigned)_tcslen(FindData.cFileName);
			
			KGLOG_PROCESS_ERROR((uFullPathFileNameLen + uNewLength) < sizeof(m_szCurrentPath));

			_tcscpy(m_szCurrentPath + uFullPathFileNameLen, FindData.cFileName);
			uNewLength += uFullPathFileNameLen;
			m_szCurrentPath[uNewLength] = _T('\0');

			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				nRetCode = _GetPathShortcuts(uNewLength);
				KGLOG_PROCESS_ERROR(nRetCode);
			}
			else
			{
				m_szCurrentPath[uNewLength] = _T('\0');

				nRetCode = _tcsicmp(m_szCurrentPath + (uNewLength - 4), _T(".lnk"));
				if (nRetCode == 0)
				{
					nRetCode = _GetShortcutsPoint(m_szCurrentPath);
					//KGLOG_PROCESS_ERROR(nRetCode);
				}
			}
		}

		nRetCode = ::FindNextFile(hFindHandle, &FindData);
		if (!nRetCode)
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
				break;
			else
			{
				KGLogPrintf(KGLOG_INFO, "errid = %d", GetLastError());
				KGLOG_PROCESS_ERROR(nRetCode && "FindNextFile");
			}
		}
	}
	
Exit1:
	nResult = true; 
Exit0:
	if (hFindHandle == INVALID_HANDLE_VALUE)
	{
		::FindClose(hFindHandle);
		hFindHandle = INVALID_HANDLE_VALUE;
	}

	return nResult;
}
int KClientReport::_GetShortcutsPoint(TCHAR szShortcutsName[])
{
	int           nResult       = false;
	HRESULT       hRetCode      = S_FALSE; 
	IShellLink*   pShellLink    = NULL; 
	IPersistFile* pPersistFile  = NULL;
	WCHAR         wszPath[MAX_PATH + 40];
	TCHAR         szTempPath[MAX_PATH + 40];
	WIN32_FIND_DATA wfd; 

	KGLOG_PROCESS_ERROR(szShortcutsName);

	_tcscpy(szTempPath, szShortcutsName);

	hRetCode = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,   
		IID_IShellLink, (LPVOID*) &pShellLink);	
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);
	  
	hRetCode = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*) &pPersistFile);
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szTempPath, -1, wszPath, MAX_PATH);

	hRetCode = pPersistFile->Load(wszPath, STGM_READ);
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	hRetCode = pShellLink->GetPath((LPSTR)szTempPath, MAX_PATH, (WIN32_FIND_DATA*) &wfd, SLGP_RAWPATH);
	KGLOG_PROCESS_ERROR(hRetCode == S_OK);

	//save to list
	m_ShortcutsPointList.insert(_tcsupr(wfd.cFileName));

	nResult = true; 
Exit0:

	if (pPersistFile)
	{
		pPersistFile->Release();
		pPersistFile = NULL;
	}

	if (pShellLink)
	{
		pShellLink->Release();
		pShellLink = NULL;
	}
	return nResult;
}


int KClientReport::DoReport(CString &strUrl)
{
	THREAD_DATA *pTData = new THREAD_DATA;
	if (pTData)
	{
		m_Threads.push_back(pTData);
		pTData->strParam = strUrl;
		pTData->Thread.Create(DoReportFunction, (void *)pTData->strParam.GetString());
	}

	return true;
}

void KClientReport::DoReportFunction(void *pvParam)
{
	if (pvParam)
	{
		URLDownloadToFile(NULL, (TCHAR *)pvParam, _T("a.tmp"), 0, 0);
		_tremove(_T("a.tmp"));
	}
}

