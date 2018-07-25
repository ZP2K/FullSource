#ifndef _UPDATE_PAK_SFX_H_
#define _UPDATE_PAK_SFX_H_


const TCHAR GAMEUPDATER_FILE_LIST[][MAX_PATH] = {
	_T("gameupdater.exe"), 
	_T("update.dll"),
	_T("gameupdater.ini")
};
const TCHAR GAMEUPDATER_PROCESS[] = _T("gameupdater.exe");
const TCHAR GAME_VERSION_FILE[] = _T("version.cfg");
const TCHAR PACK_UPDATE_FILE_PATH[] = _T("updatepack");

const TCHAR PRODUCT_VERSION_KEY[] = _T("Sword3.version");
const TCHAR PRODUCT_VERSION_LINE_NAME_KEY[] = _T("Sword3.VersionLineName");
const TCHAR PRODUCT_VERSION_FULL_NAME_KEY[] = _T("Sword3.VersionLineFullName");
const TCHAR PRODUCT_VERSION_EX_KEY[] = _T("Sword3.VersionEx");


TCHAR g_szUpdatePakFile[MAX_PATH] = {0};
TCHAR g_szGameInstallPath[MAX_PATH] = {0};
TCHAR g_szGameupdaterFile[MAX_PATH] = {0};
TCHAR g_szPackVersionLineName[MAX_PATH] = {0};
TCHAR g_szPackVersionEx[MAX_PATH] = {0};

//�Ӹ��°��н�ѹ updatepack/version.cfg,��ȷ�������°������õİ汾
int GetPackVersionInfo();
//��ע����в�����Ϸ��װĿ¼
int LocateInstallPath();
//��鱾���°��Ƿ��ʺ��԰�װ����Ϸ�汾
int CheckVersion();
//��ѹgameupdater
int ExtractGameupdater();
//��������
int Update();
int ShowPackInfo();

int ParseVersionInfo(const TCHAR szVersionFile[], TCHAR *pszVersionLineName, TCHAR *pszVersionEx);
void StringTrim(TCHAR szString[]);


#endif