//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestFullRun.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-4-23 16:00
//  Comment     :   ��֤��������TGAתDDS����ļ��Ƿ���ȷ��
//					��������ʱ��ȡָ��Ŀ¼�µ������ļ����ļ�����Ӧ���빤����ͬ�Ĺ�����ļ����з��࣬
//					����¼�ļ��������������ڹ���ת����ɺ����֤��
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestFullRun_H__
#define _KTestFullRun_H__

#include "stdafx.h"
#include "cppunit\extensions\HelperMacros.h"

class KTestFullRun : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestFullRun);
	CPPUNIT_TEST(TestConvertedFiles);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void TestConvertedFiles();

private:
	//���������ļ�
	int LoadConfigFile();
	//�����ļ����ݣ����ڷ���ɸѡ�ͽ����֤
	int LoadFileData();
	//���ļ��л�ȡ���˻���ʽ��Ҫ��
	int LoadMapNames();
	int LoadSpecialConvertorFiles();
	int LoadFilterFiles();

	//ѭ���������ļ���CheckPath���Ŀ¼���б���TGA��DDS��ͼ�ļ�
	int FindCheckFiles();
	//����һ��Ŀ¼�µ�TGA��DDS��ͼ�ļ�������ÿ���ļ��������͹���
	int FindTextureFile(const char cszFilePath[]);
	
	//�����ļ�
	int AnalyseTGAFile(const char cszFilePath[], size_t uLen);
	int AnalyseDDSFile(const char cszFilePath[], size_t uLen);
	//�ļ���ɸѡ�������
	int IsSpecialConvertFile(const UINT uIndex);
	int IsFilterMapFile(const UINT uIndex);
	int IsFilterFile(const UINT uIndex);
	int IsTexture(const UINT uIndex);
	
	//����ת�����߽���ת��
	int RunDXTConverter();

	//��֤ת������ļ�
	int ValidateFiles();
	int ValidateNoConvertedFiles();
	int ValidateConvertedFiles();
	int ValidateDDSFiles();

	/////////////////////////////////////////////////////////////////////////
	//��������
	//�����Ӵ�
	int SearchSubStr(
		const char cszSource[], size_t uSourceLen,
		const char cszSearch[], size_t uSearchSLen
		);
	//��ʽ��Ŀ¼
	int FormatPath(char* pszPath, size_t uSize);
	//���
	int Output();
	/////////////////////////////////////////////////////////////////////////
private:
	//����ת���ļ�ConvertorFiles.tab�е���������
	typedef struct _SpecialConvertFile 
	{
		char szFileName[MAX_PATH];
		UINT nDxt;
	}SpecialConvertFile;

	//������֤����ת�����ת�����
	typedef struct _SpecialConvertFileForValidate
	{
		UINT nFileIndex;
		UINT nDxt;
	}SpecialConvertFileForValidate;

	//�ļ�·��
	char m_szMapListPath[MAX_PATH];
	char m_szFilterTableName[MAX_PATH];
	char m_szSpecialConvertTableName[MAX_PATH];
	char m_szApplicationPath[MAX_PATH];
	char m_szConfigFile[MAX_PATH];
	//�����ļ�
	vector<char*> m_vAllFileList;
	//Դ���ݣ����ɸѡ�������
	vector<char*> m_vCheckPathList;
	vector<char*> m_vMapName;
	vector<char*> m_vFilterFileAndPath;
	vector<SpecialConvertFile> m_vSpecialConvertFiles;
	//����������ݣ�������֤ת�����
	vector<SpecialConvertFileForValidate> m_vSpecialConvertFilesForValidate;
	vector<UINT> m_vDefaultConvertFileList;
	vector<UINT> m_vFilterFileList;
	vector<UINT> m_vErrorFileList;
	vector<UINT> m_vTGAFileList;
	vector<UINT> m_vDDSFileList;
};

#endif ///KTestFullRun
