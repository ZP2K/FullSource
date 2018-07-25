#include "stdafx.h"
#include "KG_Bip.h"
#include "KG_Clip.h"

const char *HELPMSG =
"\n"
"����ģ�͹Ǽܼ�鹤�ߣ�\n"
"�÷�1: ָ�������ͻ���·�� \n"
"����:  -client \n"
"����:  i>���ͻ������Ƿ�����ͬ�ĹǼ��ļ�, ���д��BipCheckResult.txt \n"
"        ii>������Ǽ��ļ���ƥ��Ķ����ļ�, ���д��AniCheckResult.txt \n"
"ʾ��:  KG_BoneChecker.exe -client \"D:\\Kingsoft\\Game\\Sword3-products\\trunk\\client\" \n"
"\n"
"�÷�2: ָ��һ���Ǽ��ļ� \n"
"����:  -bip \n"
"����:  ������Ǽ��ļ���ƥ��Ķ����ļ�, ���д��AniCheckResult.txt \n"
"ʾ��1: �����ļ��ڹǼ��ļ�����ͬĿ¼�� \n"
"        KG_BoneChecker.exe -bip \"d:\\model\\model.txt\"  (��d:\\model\\������model_st01.ani, model_st02.ani�ȶ����ļ�) \n"
"ʾ��2: �����ļ���NPC_Source��player�Ķ�ӦĿ¼�� \n"
"        KG_BoneChecker.exe -bip \"d:\\client\\data\\source\\NPC_source\\A001\\ģ��\\A001.txt\" \n"
"        KG_BoneChecker.exe -bip \"d:\\client\\data\\source\\player\\M2\\����\\Mdl\\M2.txt\" \n";

int GenerateJX3BipFileList(char szPath[], vector<string> &vecBipList)
{
    int nResult  = false;
    int nRetCode = false;
    FILE *fpFile = NULL;
    char szBipName[MAX_PATH];
    char szCmd[MAX_PATH];
    size_t uLen = 0;

    ASSERT(szPath);

    uLen = strlen(szPath);
    if (szPath[uLen - 1] == '\\')
        szPath[uLen - 1] = '\0';

    nRetCode = snprintf(szCmd, sizeof(szCmd) - 1, "dir /s /b \"%s\\data\\source\\player\\*.txt\" > temp_biplist.dat", szPath);
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szCmd) - 1));
    szCmd[sizeof(szCmd) - 1] = '\0';

    nRetCode = system(szCmd);
    KG_PROCESS_ERROR(nRetCode == 0);

    nRetCode = snprintf(szCmd, sizeof(szCmd) - 1, "dir /s /b \"%s\\data\\source\\NPC_source\\*.txt\" >> temp_biplist.dat", szPath);
    ASSERT((nRetCode > 0) && (nRetCode <= sizeof(szCmd) - 1));
    szCmd[sizeof(szCmd) - 1] = '\0';

    nRetCode = system(szCmd);
    KG_PROCESS_ERROR(nRetCode == 0);

    fpFile = fopen("temp_biplist.dat", "r");
    KGLOG_PROCESS_ERROR(fpFile);

    while (true)
    {
        char *pRetCode = fgets(szBipName, sizeof(szBipName) - 1, fpFile);
        if (!pRetCode)
            break;
        szBipName[sizeof(szBipName) - 1] = '\0';

        size_t uLen = strlen(szBipName);
        ASSERT(uLen > 1);
        size_t i = uLen - 1;
        while (szBipName[i] == '\r' || szBipName[i] == '\n')
        {
            ASSERT(i > 0);
            --i;
        }
        szBipName[i + 1] = '\0';

        vecBipList.push_back(szBipName);
    }

    nResult = true;
Exit0:
    if (!nResult)
    {
        KGLogPrintf(KGLOG_INFO, "can't find JX3 Client Path!! \"%s\"", szPath);
    }
    if (fpFile)
    {
        fclose(fpFile);
        fpFile = NULL;
    }
    remove("temp_biplist.dat");
    return nResult;
}

int IsBipFileExist(const char cszBipName[])
{
    int nResult  = false;
    int nRetCode = false;

    ASSERT(cszBipName);

    nRetCode = g_IsFileExist(cszBipName);
    KG_PROCESS_ERROR(nRetCode);

    nResult = true;
Exit0:
    if (!nResult)
    {
        KGLogPrintf(KGLOG_INFO, "can't find bip file!! \"%s\"", cszBipName);
    }
    return nResult;
}

int LoadConfig(BOOL *pnIsClient, char *pszDir, unsigned uSize)
{
    int nResult = false;
    int nRetCode = false;
    const char CONFIGFILE[] = "KG_BoneChecker.ini";
    IIniFile *piFile = NULL;

    *pnIsClient = false;

    piFile = g_OpenIniFile(CONFIGFILE);
    KGLOG_PROCESS_ERROR(piFile);
    
    nRetCode = piFile->GetString("BoneChecker", "SO3ClientDir", "", pszDir, uSize);
    KGLOG_PROCESS_ERROR(nRetCode);

    if (strcmp(pszDir, ""))
    {
        *pnIsClient = true;
        KG_PROCESS_SUCCESS(true);
    }

    nRetCode = piFile->GetString("BoneChecker", "bipdir", "", pszDir, uSize);
    KGLOG_PROCESS_ERROR(nRetCode);

Exit1:
    nResult = true;
Exit0:
    if (!nResult)
        puts("!!!Read Config File Failed.");
    KG_COM_RELEASE(piFile);
    return nResult;
}

int main(int argc, char *argv[])
{
    int nResult  = false;
    int nRetCode = false;
    KGLOG_PARAM LogParam  = {"Logs", "KG_BoneCheker", KGLOG_OPTION_FILE, 65535};
    vector<string>vecBipList;
    char szDir[MAX_PATH];
    int nIsCheckClientAni = false;

    g_SetRootPath();

    nRetCode = KGLogInit(LogParam, NULL);
    KG_PROCESS_ERROR(nRetCode);

    KGLogPrintf(KGLOG_INFO, "%s\n", "BoneChecker Build at " __TIME__ " " __DATE__);

    KGLogPrintf(KGLOG_INFO, "Begin...\n");

    szDir[0] = '\0';

    if (argc == 3)
    {
        if (!stricmp(argv[1], "-client"))
            nIsCheckClientAni = true;
        else if (!stricmp(argv[1], "-bip"))
            nIsCheckClientAni = false;
        else
            KGLOG_PROCESS_ERROR(false && "wrong argument");

        memcpy(szDir, argv[2], _countof(szDir) - 1);
        szDir[_countof(szDir) - 1] = '\0';
    }
    else
    {
        nRetCode = LoadConfig(&nIsCheckClientAni, szDir, _countof(szDir));
        KGLOG_PROCESS_ERROR(nRetCode && "Load Config Failed");
    }

    if (nIsCheckClientAni)
    {
        KGLogPrintf(
            KGLOG_INFO, 
            "Check all SO3Client bip && clip...\n"
            "SO3ClientDir=\"%s\"\n",
            szDir
        );

        nRetCode = GenerateJX3BipFileList(szDir, vecBipList);
        KG_PROCESS_ERROR(nRetCode);

        nRetCode = AnalyseBip(vecBipList);
        KG_PROCESS_ERROR(nRetCode);
    }
    else
    {
        KGLogPrintf(KGLOG_INFO, "Check Ani file with bip \"%s\"\n", szDir);

        nRetCode = IsBipFileExist(szDir);
        KG_PROCESS_ERROR(nRetCode);

        vecBipList.push_back(szDir);
    }

    nRetCode = AnalyseAni(vecBipList);
    KG_PROCESS_ERROR(nRetCode);

    nResult = true;
Exit0:
    if (nResult)
        KGLogPrintf(KGLOG_INFO, "done successful.");
    else
        KGLogPrintf(KGLOG_INFO, "!!!done with some error, please check log file.");
    KGLogUnInit(NULL);
    return nResult ? 0 : 1;
}
