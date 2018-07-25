#include "KGTestLoadMaterial.h"
#include "KG3DMaterial.h"
#include "KG3DMaterialTable.h"

const char RESULT_LOAD_MTL[]    = "Result_Load_Mtl.txt";	//�����ļ�����ʧ�ܵļ�¼����ļ�
const char RESULT_LOAD_TEX[]    = "Result_Load_Tex.txt";	//�����ļ���Ӧ����ͼ�ļ�����ʧ�ܵļ�¼����ļ�
const char RESULT_FILTER_FILE[] = "Result_Filter_File.txt";	//�����˵ļ�¼����ļ�

const string STR_LOAD_RESULT[] =
{
	"Can't Find Texture File", "Loading Texture Error", "No any Texture File"
};

KGTestLoadMaterial::KGTestLoadMaterial()
{
	m_pTestInterface = NULL;
	m_pMtlTable		 = NULL;

	m_szApplicationPath[0] = '\0';
	m_szLogFileName[0]	   = '\0';
	m_szResultPath[0]	   = '\0';
	m_mapErrorMtlFileInfo.clear();
	m_vecLoadFailedMtl.clear();
	m_vecFilterMtl.clear();
}

KGTestLoadMaterial::~KGTestLoadMaterial()
{
}

int KGTestLoadMaterial::Init(IKG3DAutoTestInterface* pTestInterface,
							 const char cszResultPath[])
{
	int nResult  = false;
	int nRetCode = false;
	char* pszResult = NULL;

	KG_ASSERT_EXIT(pTestInterface);
	KG_ASSERT_EXIT(cszResultPath);
	KGLOG_PROCESS_ERROR(cszResultPath[0] != '\0');

	m_pTestInterface = pTestInterface;
	m_pTestInterface->AddRef();
	m_pMtlTable = m_pTestInterface->GetMaterialTable();
	KGLOG_PROCESS_ERROR(m_pMtlTable);

	nRetCode = strncpy_s(m_szResultPath, sizeof(m_szResultPath), cszResultPath, strlen(cszResultPath));
	KGLOG_PROCESS_ERROR(nRetCode == 0);

	KGLogPrintf(KGLOG_INFO, "Ready to Test Load Material");

	nResult = true;
Exit0:
	if (!nResult)
	{
		UnInit();
	}
	return nResult;
}

void KGTestLoadMaterial::UnInit()
{
	if (m_pTestInterface)
	{
		m_pTestInterface->Release();
		m_pTestInterface = NULL;
	}
	m_pMtlTable = NULL;
	m_mapErrorMtlFileInfo.clear();
	m_vecLoadFailedMtl.clear();
	m_vecFilterMtl.clear();
}

void KGTestLoadMaterial::TestLoadOneMtlFile(const char cszFileName[])
{
	HRESULT hrRetCode = E_FAIL;
	KG3DMaterial* pMaterial = NULL;

	_ASSERTE(cszFileName);

	KGLogPrintf(KGLOG_INFO, "Test Load Mtl: %s", cszFileName);

	hrRetCode = m_pMtlTable->LoadResourceFromFile(cszFileName, 0, 0, (IKG3DResourceBase**)&pMaterial);
	if (FAILED(hrRetCode) || pMaterial == NULL)
	{
		m_vecLoadFailedMtl.push_back(cszFileName);
	}
	else
	{
		KG3DMaterial::KG3DMaterialSubset* pSubset = NULL;
		bool bNoAnyTex = false;
		for (size_t i = 0; i < pMaterial->m_dwNumMaterials; i++)
		{
			pMaterial->GetSubMaterial((DWORD)i, (PVOID*)&pSubset);
			if (pSubset->m_dwNumUsedTexture != 0)
			{
				bNoAnyTex = true;
			}
		}
		if (!bNoAnyTex)
		{
			TextureInfoMap TexNames;
			m_mapErrorMtlFileInfo.insert(make_pair(cszFileName, TexNames));
			m_mapErrorMtlFileInfo[cszFileName].insert(make_pair("", NO_ANY_TEX));
		}
	}
	if (pMaterial)
	{
		//����Ҫע��Ӳ���RELEASE_ATONCE���Ա������ͷ���Դ��Ĭ���Ƿ����ͷŶ��У�RELEASE_INQUEUE��
		//�����ڴ��л�һֱռ����һ����������Դ�����������ͷ���Դ���еĴ�С
		hrRetCode = m_pMtlTable->ReleaseResource((KG3DMaterial*)pMaterial, RELEASE_ATONCE);
		if (FAILED(hrRetCode))
		{
			KGLogPrintf(KGLOG_ERR, "Release Mtl Failed!");
		}
		else
		{
			pMaterial = NULL;
		}
	}
	KGLogPrintf(KGLOG_INFO, "End Load Test");
}

int KGTestLoadMaterial::AnalyseLog()
{
	int nResult  = false;
	int nRetCode = false;
	FILE* pLogFile = NULL;
	char* pResult = NULL;
	char szLine[MAX_LOG_LINE]	 = {0};
	char szNewLogFile[MAX_PATH]	 = {0};
	char szMtlFileName[MAX_PATH] = {0};
	char szTexFileName[MAX_PATH] = {0};
	TextureInfoMap TexNames;

	nRetCode = KGTestCommon::FindLogFile(g_szLogPath, m_szLogFileName, sizeof(m_szLogFileName));
	KGLOG_PROCESS_ERROR(nRetCode);

	nRetCode = _snprintf_s(
		szNewLogFile,
		sizeof(szNewLogFile),
		sizeof(szNewLogFile) - 1,
		"%s.log",
		m_szLogFileName
	);	//*****.log -> *****.log.log
	KGLOG_PROCESS_ERROR(nRetCode > 0);

	nRetCode = ::CopyFile(m_szLogFileName, szNewLogFile, TRUE);
	KGLOG_PROCESS_ERROR(nRetCode);

	pLogFile = fopen(szNewLogFile, "r");
	KGLOG_PROCESS_ERROR(pLogFile);

	while (true)
	{
		pResult = fgets(szLine, sizeof(szLine), pLogFile);
		if (!pResult)
		{
			nRetCode = feof(pLogFile);
			KG_PROCESS_SUCCESS(nRetCode);
			KGLOG_PROCESS_ERROR(nRetCode);
		}
		//MTL:D:\JZ_���˷���002sn.Mtl, Load Texture d:\texture\mt_zˮկ001.tga failed
		nRetCode = KGTestCommon::GetSubStr(szLine, "MTL:", ",", szMtlFileName, sizeof(szMtlFileName));
		if (!nRetCode)
		{
			continue;
		}
		nRetCode = KGTestCommon::GetSubStr(szLine, "Load Texture ", " failed", szTexFileName, sizeof(szTexFileName));
		if (!nRetCode)
		{
			continue;
		}
//����ȡ������
// 		nRetCode = CheckFilter(szMtlFileName, szTexFileName);
// 		if (nRetCode)
 		{
			m_mapErrorMtlFileInfo.insert(make_pair(szMtlFileName, TexNames));
			nRetCode = _access(szTexFileName, 0);
			if (nRetCode == -1)
			{
				m_mapErrorMtlFileInfo[szMtlFileName].insert(make_pair(szTexFileName, NO_EXIST));
			}
			else
			{
				m_mapErrorMtlFileInfo[szMtlFileName].insert(make_pair(szTexFileName, LOADING_ERROR));
			}
		}		
	}

Exit1:
	nResult = true;
Exit0:
	if (pLogFile)
	{
		fclose(pLogFile);
		pLogFile = NULL;
	}
	nRetCode = remove(szNewLogFile);
	if (nRetCode == -1 && szNewLogFile)
	{
		KGLogPrintf(KGLOG_ERR, "remove file faild: %s", szNewLogFile);
	}
	return nResult;
}

int KGTestLoadMaterial::OutputResult()
{
	int nResult  = false;
	int nRetCode = false;
	char szResultFile[MAX_PATH] = {0};
	char* pPath = NULL;
	FILE* pResultFile = NULL;
	string strWrite;

	{//�������ʧ�ܵĲ��ʼ�¼���ļ�
		nRetCode = _snprintf_s(
			szResultFile,
			sizeof(szResultFile) - 1,        
			"%s%s",
			m_szResultPath,
			RESULT_LOAD_MTL
		);
		KGLOG_PROCESS_ERROR(nRetCode > 0);
		
		pResultFile = fopen(szResultFile, "w");
		KGLOG_PROCESS_ERROR(pResultFile);

		strWrite = "��Load Failed Mtl File Name��\n\n";
		fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
		for (size_t i = 0; i < m_vecLoadFailedMtl.size(); i++)
		{
			strWrite = "\t" + m_vecLoadFailedMtl[i] + "\n";
			fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
		}
		strWrite = string("\n��Total Num��\t") +
				   KGTestCommon::IntToString((int)m_vecLoadFailedMtl.size());
		fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);

		fclose(pResultFile);
		pResultFile = NULL;
		szResultFile[0] = '\0';
	}

	{//������ʼ�����ͼʧ�ܵĲ��ʺ���ͼ��¼���ļ�
		MtlFileInfoMap::iterator posMtl;
		TextureInfoMap::iterator posTex;
		nRetCode = _snprintf_s(
			szResultFile,
			sizeof(szResultFile) - 1,
			"%s%s",
			m_szResultPath,
			RESULT_LOAD_TEX
		);
		KGLOG_PROCESS_ERROR(nRetCode > 0);

		pResultFile = fopen(szResultFile, "w");
		KGLOG_PROCESS_ERROR(pResultFile);

		strWrite = string("��Analyse Log File Name��    ") + m_szLogFileName + "\n";
		fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);

		for (posMtl = m_mapErrorMtlFileInfo.begin(); posMtl != m_mapErrorMtlFileInfo.end(); ++posMtl)
		{
			strWrite = "\n��Mtl File Name��    " + posMtl->first + "\n";
			fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
			strWrite = "��Texture Error��\n";
			fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
			for (posTex = posMtl->second.begin(); posTex != posMtl->second.end(); ++posTex)
			{
				strWrite = "        " + STR_LOAD_RESULT[posTex->second] + "  " + posTex->first + "\n";
				fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
			}
		}
		strWrite = string("\n��Total Num��\t") +
				   KGTestCommon::IntToString((int)m_mapErrorMtlFileInfo.size());
		fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);

		fclose(pResultFile);
		pResultFile = NULL;
		szResultFile[0] = '\0';
	}
// 	{//��������˵ļ�¼���ļ�
// 		nRetCode = _snprintf_s(
// 			szResultFile,
// 			sizeof(szResultFile) - 1,
// 			"%s%s",
// 			m_szResultPath,
// 			RESULT_FILTER_FILE
// 			);
// 		KGLOG_PROCESS_ERROR(nRetCode > 0);
// 
// 		pResultFile = fopen(szResultFile, "w");
// 		KGLOG_PROCESS_ERROR(pResultFile);
// 
// 		strWrite = "��Filter File Name��\n\n";
// 		fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
// 		for (size_t i = 0; i < m_vecFilterMtl.size(); i++)
// 		{
// 			strWrite = "\t" + m_vecFilterMtl[i] + "\n";
// 			fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
// 		}
// 		strWrite = string("\n��Total File Num��\t") +
// 			KGTestCommon::IntToString((int)m_vecFilterMtl.size());
// 		fwrite(strWrite.c_str(), strWrite.length(), 1, pResultFile);
// 
// 		fclose(pResultFile);
// 		pResultFile = NULL;
// 		szResultFile[0] = '\0';
// 	}

	nResult = true;
Exit0:
	if (pResultFile)
	{
		fclose(pResultFile);
		pResultFile = NULL;
	}
	return nResult;
}


int KGTestLoadMaterial::CheckFilter(const char cszMtlFile[], const char cszTexFile[])
{
	int nRetCode = false;
	int nResult  = false;
	char szMtlName[MAX_PATH] = {0};
	char szTexFile[MAX_PATH] = {0};
	char* pszResult = NULL;

	_ASSERTE(cszMtlFile);
	_ASSERTE(cszTexFile);

	//���˲����ļ�F1��F2��M2��ص���ͼ���ش���
	nRetCode = _splitpath_s(cszMtlFile, NULL, 0, NULL, 0, szMtlName, sizeof(szMtlName), NULL, 0);
	KG_PROCESS_ERROR(nRetCode == 0);

	nRetCode = _strlwr_s(szMtlName, sizeof(szMtlName));
	KG_PROCESS_ERROR(nRetCode == 0);

	pszResult = strstr(szMtlName, "f1");
	KG_PROCESS_ERROR(pszResult == NULL);

	pszResult = strstr(szMtlName, "f2");
	KG_PROCESS_ERROR(pszResult == NULL);

	pszResult = strstr(szMtlName, "m2");
	KG_PROCESS_ERROR(pszResult == NULL);

	//���˹���ͼ��ص���ͼ���ش���
	
	nRetCode = strncpy_s(szTexFile, sizeof(szTexFile), cszTexFile, strlen(cszTexFile));
	KG_PROCESS_ERROR(nRetCode == 0);
	pszResult = strstr(szTexFile, "����ͼ");
	KG_PROCESS_ERROR(pszResult == NULL);

	nResult = true;
Exit0:
	if (!nResult)
	{
		m_vecFilterMtl.push_back(cszMtlFile);
	}
	return nResult;	
}