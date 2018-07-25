// TabToXML.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileType.h"
#include "file.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include<conio.h>  
#include <direct.h>
using namespace std;
char g_szstr[2048] = {0};
int  g_nstrpos = 0;
char* ChangeForstr(char *pszstr)// <  ��"<"�滻��"&lt";
{
	char *pStart = NULL;
	int nLenstr = 0;
	pStart = (char*) memchr(pszstr,'<',strlen(pszstr));

	if (pStart)
	{
		memcpy(&g_szstr[g_nstrpos],pszstr,pStart - pszstr );
		g_nstrpos += (pStart - pszstr);
		g_szstr[g_nstrpos++] = '&';
		g_szstr[g_nstrpos++] = 'l';
		g_szstr[g_nstrpos++] = 't';
		g_szstr[g_nstrpos++] = ';';
		return ChangeForstr(++pStart);
	}
	else
	{
		nLenstr = strlen(pszstr);
		memcpy(&g_szstr[g_nstrpos],pszstr,nLenstr);
		g_nstrpos += nLenstr;
		g_szstr[g_nstrpos] = '\0';
		return g_szstr;
	}
	
}
char* ChangeForstr1(char *pszstr)// >  ��"<"�滻��"&gt";
{
	char *pStart = NULL;
	int nLenstr = 0;
	
	pStart = (char*) memchr(pszstr,'>',strlen(pszstr));

	if (pStart)
	{
		memcpy(&g_szstr[g_nstrpos],pszstr,pStart - pszstr );
		
		g_nstrpos += (pStart - pszstr);
		g_szstr[g_nstrpos++] = '&';
		g_szstr[g_nstrpos++] = 'g';
		g_szstr[g_nstrpos++] = 't';
		g_szstr[g_nstrpos++] = ';';
		return ChangeForstr1(++pStart);
	}
	else
	{
		nLenstr = strlen(pszstr);
		memcpy(&g_szstr[g_nstrpos],pszstr,nLenstr);
		g_nstrpos += nLenstr;
		g_szstr[g_nstrpos] = '\0';
		return g_szstr;
	}

}

char* ChangeForstr2(char *pszstr)// &  ��"&"�滻��"&amp;";
{
	char *pStart = NULL;
    int nLenstr = 0;

	pStart = (char*) memchr(pszstr,'&',strlen(pszstr));

	if (pStart)
	{
		memcpy(&g_szstr[g_nstrpos],pszstr,pStart - pszstr );

		g_nstrpos += (pStart - pszstr);
		g_szstr[g_nstrpos++] = '&';
		g_szstr[g_nstrpos++] = 'a';
		g_szstr[g_nstrpos++] = 'm';
		g_szstr[g_nstrpos++] = 'p';
		g_szstr[g_nstrpos++] = ';';
		return ChangeForstr2(++pStart);
	}
	else
	{
		nLenstr = strlen(pszstr);
		memcpy(&g_szstr[g_nstrpos],pszstr,nLenstr);
		g_nstrpos += nLenstr;
		g_szstr[g_nstrpos] = '\0';
		return g_szstr;
	}

}

char* ChangeForstr3(char *pszstr)// "  ��"""�滻��"&quot;";
{
	char *pStart = NULL;
    int nLenstr = 0;

	pStart = (char*) memchr(pszstr,'\"',strlen(pszstr));

	if (pStart)
	{
		memcpy(&g_szstr[g_nstrpos],pszstr,pStart - pszstr );

		g_nstrpos += (pStart - pszstr);
		g_szstr[g_nstrpos++] = '&';
		g_szstr[g_nstrpos++] = 'q';
		g_szstr[g_nstrpos++] = 'u';
		g_szstr[g_nstrpos++] = 'o';
		g_szstr[g_nstrpos++] = 't';
		g_szstr[g_nstrpos++] = ';';
		return ChangeForstr3(++pStart);
	}
	else
	{
		nLenstr = strlen(pszstr);
		memcpy(&g_szstr[g_nstrpos],pszstr,nLenstr);
		g_nstrpos += nLenstr;
		g_szstr[g_nstrpos] = '\0';
		return g_szstr;
	}

}

char* ChangeForstr4(char *pszstr)// '  ��"'"�滻��"&qpos;";
{
	char *pStart = NULL;
	int nLenstr = 0;

	pStart = (char*) memchr(pszstr, '\'', strlen(pszstr));

	if (pStart)
	{
		memcpy(&g_szstr[g_nstrpos], pszstr, pStart - pszstr );

		g_nstrpos += (pStart - pszstr);
		g_szstr[g_nstrpos++] = '&';
		g_szstr[g_nstrpos++] = 'a';
		g_szstr[g_nstrpos++] = 'p';
		g_szstr[g_nstrpos++] = 'o';
		g_szstr[g_nstrpos++] = 's';
		g_szstr[g_nstrpos++] = ';';
		return ChangeForstr4(++pStart);
	}
	else
	{
		nLenstr = strlen(pszstr);
		memcpy(&g_szstr[g_nstrpos], pszstr, nLenstr);
		g_nstrpos += nLenstr;
		g_szstr[g_nstrpos] = '\0';
		return g_szstr;
	}

}

int Clearglobal()
{
	memset(g_szstr,0,sizeof(g_szstr));
	g_nstrpos = 0;
	return 1;

}

char *ChangstrForXML(char *psztab)//������������ַ���ת������<����>����&����"����'��
{
	char *pszTem = NULL;
	char szTemstr[2048] = {0};
	int nstrLen = 0;
	Clearglobal();

	pszTem = ChangeForstr2(psztab);//�ȼ�顰&��;
	nstrLen = strlen(pszTem);
	memset(szTemstr, 0, sizeof(szTemstr));
	memcpy(szTemstr, pszTem, nstrLen);
	szTemstr[nstrLen] = '\0';
	Clearglobal();

	pszTem = ChangeForstr(szTemstr);
	nstrLen = strlen(pszTem);
	memcpy(szTemstr, pszTem, nstrLen);
	szTemstr[nstrLen] = '\0';
	Clearglobal();

	pszTem = ChangeForstr1(szTemstr);
	nstrLen = strlen(pszTem);
	memset(szTemstr, 0, sizeof(szTemstr));
	memcpy(szTemstr, pszTem, nstrLen);
	szTemstr[nstrLen] = '\0';
	Clearglobal();

	pszTem = ChangeForstr3(szTemstr);
	nstrLen = strlen(pszTem);
	memset(szTemstr, 0, sizeof(szTemstr));
	memcpy(szTemstr, pszTem, nstrLen);
	szTemstr[nstrLen] = '\0';
	Clearglobal();

	pszTem = ChangeForstr4(szTemstr);

	return pszTem;

}

char* FindDir(char *pszstr)// ���ļ���Ŀ¼���������һ��"\"����һ��λ��
{
	char *pStart = NULL;
	if (pszstr == NULL)
	{
		return NULL;
	}
	pStart = (char*) memchr(pszstr, '\\', strlen(pszstr));

	if (pStart)
	{
		return FindDir(++pStart);
	}
	else
	{
		return pszstr;
	}

}
bool CheckIsAbsoluteDir(char *pszDir)
{
	char *pStart = NULL;
	if (pszDir == NULL)
	{
		return false;
	}
	pszDir++;
	pStart = (char*) memchr(pszDir, ':', strlen(pszDir));
	if (pStart)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int _tmain(int argc, char * argv[])
{
	
	if (argc != 3)
	{
		cout << "tab�ļ�������,ע���׺��.tab�Ĵ�Сд��·���Ǿ���·��" << endl;
		cout << "xml�ļ�������,ע���׺��.xml�Ĵ�Сд��·���Ǿ���·��" << endl;
		cout << "�������£�" << endl;
		cout << "D:\src.tab D:\des.xml" << endl;
		cout << "�����������ԣ���������Ϊ3" << endl;
		return 1;
	}
	ITabFile*   piTabFile   = NULL;
    int nResult = 0;
	int nRecode = 0;
	bool bRecode = false;
	char szKeyName[1024] = {0};
	char szTabFileName[1024] = {0};
	char szXmlMainKey[1024] = {0};
	char szTemValue[2048] = {0};
	int nNameLen = 0;
	int nWidth = 0;
	int nHeight = 0;
	char szInTabFileName[1024] = {0};
	char szXmlOutFileName [1024] = {0};
	ofstream outfile;
	char *pszTem = NULL;
	char szXMLDir[1024] = {0};
	char *pszstrpos = NULL;
	char *pszXMLmainKey = NULL;
	char *pRelativeTabFileName = NULL;

	strcpy(szInTabFileName, argv[1]);

	bRecode = CheckIsAbsoluteDir(argv[1]);
	if (bRecode == false)
	{
		cout<< argv[1] << "���Ǿ���·��" << endl;
		KG_PROCESS_ERROR(bRecode);
	}

	bRecode = CheckIsAbsoluteDir(argv[2]);
	if (bRecode == false)
	{
		cout<< argv[2] << "���Ǿ���·��" << endl;
		KG_PROCESS_ERROR(bRecode);
	}

    nNameLen = strlen(szInTabFileName);
	if (nNameLen < 5)
	{
		cout << "tab,�����ļ�������\n" << endl;
		KG_PROCESS_ERROR(nRecode);
		
	}
	
	nResult = memcmp((const void*)&szInTabFileName[nNameLen-4], ".tab", 4);
	if (nResult != 0)
	{
		cout << "tab,�����ļ�����׺����,Ӧ��.tab" << endl;
		KG_PROCESS_ERROR(nRecode);
	}

	strcpy(szXmlOutFileName, argv[2]);
	nNameLen = strlen(szXmlOutFileName);
	if (nNameLen < 5)
	{
		cout << "Xml,�����ļ�������\n" << endl;
		KG_PROCESS_ERROR(nRecode);

	}

	nResult = memcmp((const void*)&szXmlOutFileName[nNameLen-4], ".xml", 4);

	if (nResult != 0)
	{
		cout << "Xml,�����ļ�����׺����Ӧ��.xml" << endl;
		KG_PROCESS_ERROR(nRecode);
	}

	memcpy(szTabFileName, szInTabFileName, sizeof(szTabFileName));

	nNameLen = strlen(szTabFileName);
	memcpy(szXmlMainKey, szTabFileName, nNameLen - 4);
	szXmlMainKey[nNameLen - 4] = '\0';
	pszXMLmainKey = FindDir(szXmlMainKey);

//	g_SetRootPath();
	pRelativeTabFileName = FindDir(szInTabFileName);

	piTabFile = g_OpenTabFile(szTabFileName);
	if (piTabFile == NULL)
	{
		cout << "tab�ļ���ʧ�ܣ������Ƿ���ڸ��ļ�" << szTabFileName << "�����Ǿ���·��д��" << endl;
		KG_PROCESS_ERROR(piTabFile);
	}
	
	pszstrpos = FindDir(szXmlOutFileName);
	if (pszstrpos == szXmlOutFileName)
	{
		memcpy(szXMLDir, szXmlOutFileName, strlen(szXmlOutFileName));
		nNameLen = strlen(szXmlOutFileName);
		szXMLDir[nNameLen] = '\0';
	}
	else
	{
		memcpy(szXMLDir, szXmlOutFileName, pszstrpos - szXmlOutFileName);
		nNameLen = strlen(szXmlOutFileName);
		szXMLDir[nNameLen] = '\0';

	}

	outfile.open(szXmlOutFileName);
	nResult = outfile.is_open();
	if (nResult == 0)
	{
		cout << "XML�ļ���Ŀ¼�ļ���" << szXMLDir << "������,�����´�������·��д������" <<  endl;
		KG_PROCESS_ERROR(nResult);
	}
	

	nWidth = piTabFile->GetWidth();
	nHeight = piTabFile->GetHeight();

	outfile << "<?xml version=\"1.0\" encoding=\"gbk\"?>" << '\n';
	outfile << '<' << strupr(pszXMLmainKey) << 'S' << '>' <<'\n';
	if (0 == strncmp("RelationForce.tab", pRelativeTabFileName, sizeof("RelationForce.tab")))
	{
		for ( int i = 2; i <= nHeight; i++)
		{
			outfile<< '\t' << '<' << strupr(pszXMLmainKey)  << '\n';
			for (int j = 1; j <= 2; j++)
			{
				piTabFile->GetString(1,j,"",szKeyName,sizeof(szKeyName));

				if (szKeyName[0] == NULL)
				{
					continue;
				}

				piTabFile->GetString(i,j,"",szTemValue,sizeof(szTemValue));

				pszTem = ChangstrForXML(szTemValue);

				if ( j == 1)
				{
					outfile << '\t' << '\t' <<  szKeyName << '='<< '\"' << pszTem << '\"'; //<< '\n';
				}
				else
				{
					outfile << ' ' <<  szKeyName << '='<< '\"' << pszTem << '\"';
				}
				Clearglobal();

			}
			outfile << "/>" << '\n';
		}
	}
	else
	{
		for ( int i = 2; i <= nHeight; i++)
		{
			outfile<< '\t' << '<' << strupr(pszXMLmainKey)  << '\n';
			for (int j = 1; j <= nWidth; j++)
			{
				piTabFile->GetString(1,j,"",szKeyName,sizeof(szKeyName));

				if (szKeyName[0] == NULL)
				{
					continue;
				}

				piTabFile->GetString(i,j,"",szTemValue,sizeof(szTemValue));

				pszTem = ChangstrForXML(szTemValue);

				if ( j == 1)
				{
					outfile << '\t' << '\t' <<  szKeyName << '='<< '\"' << pszTem << '\"'; //<< '\n';
				}
				else
				{
					outfile << ' ' <<  szKeyName << '='<< '\"' << pszTem << '\"';
				}
				Clearglobal();

			}
			outfile << "/>" << '\n';
		}
	}
	outfile << "</" << strupr(pszXMLmainKey) << 'S' << '>' <<'\n';

	nRecode = 1;

Exit0:
	if (nRecode == 0)
	{
		cout << "tab�ļ�ת����XMLʧ��" << endl;
	}
	else
	{
		outfile.close();
		cout << "ת���ɹ�" << endl;
	}
	
	return 0;
}

