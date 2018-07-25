//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGFileManager.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-9-30 10:00
//  Comment     :   �ļ������࣬��������Ŀ¼�µ��ļ����ļ��Ƚ�
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGFileManager_H__
#define _KGFileManager_H__

#include "KGTestCommon.h"

class KGFileManager
{
public:
	static KGFileManager* Instance();
private:
	KGFileManager(void);
	KGFileManager(const KGFileManager& object);
	KGFileManager& operator =(const KGFileManager& object);
	~KGFileManager(void);

public:
	//����ָ��Ŀ¼�µ�����ָ�����͵��ļ����ƣ��������Ϳ�����|��������.mesh|.pvs
	int  SearchFiles(const char cszPath[], vector<string>* pvecFileName, const char cszExt[] = NULL);
	//�Ƚ�ָ������Ŀ¼�µ�����ָ�����͵��ļ��������ƱȽϷ�ʽ���������Ϳ�����|��������.mesh|.pvs
	int  ComparePathFiles(const char cszPath_1[], const char cszPath_2[], const char cszExt[] = NULL);
	//�Ƚ�ָ�������ļ��������ƱȽϷ�ʽ
	int  CompareFileByDataStream(const char cszFileName_1[], const char cszFileName_2[]);
	//����ָ��Ŀ¼�µ������ļ����ļ��е�ĳ��Ŀ¼�£�����XCopy����
	int  CopyPathFiles(const char cszSourcePath[], const char cszDestPath[]);

private:
	int  SearchFilesFromPath(const char cszPath[]);

private:
	vector<string> m_vecFileExt;
	vector<string> m_vecFoundFilesList;
	static KGFileManager m_Instance;
};

#endif