//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGTestLoadMaterial.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-6-15 10:00
//  Comment     :   ���ز������ݲ��������غ����־�м�¼�Ĵ�����Ϣ������������
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KGTestLoadMaterial_H__
#define _KGTestLoadMaterial_H__

#include "KGTestCommon.h"
#include "KG3DInterface.h"
#include "KG3DAutoTestInterface.h"

using namespace std;

class KGTestLoadMaterial
{
public:
	KGTestLoadMaterial();
	~KGTestLoadMaterial();
	
public:
	int  Init(IKG3DAutoTestInterface* pTestInterface,
			  const char cszResultPath[]);
	void TestLoadOneMtlFile(const char cszFileName[]);	
	int  AnalyseLog();
	int  OutputResult();
	void UnInit();

private:
	/*�������ļ��Ƿ���Ҫ����
		return 
		true:����Ҫ����
		false:��Ҫ����
	*/
	int  CheckFilter(const char cszMtlFile[], const char cszTexFile[]);



	//��ȡ�Ӵ�
	//����GetSubStr("abcdefg", "a", "g", pszSub, cuSize); -> pszSub = "bcdef"
	int  GetSubStr(const char cszSource[],
				   const char cszStart[],
				   const char cszEnd[],
				   char* pszSub,
				   const size_t cuSize);

private:
	enum						//������ͼ�ĸ�����Ϣ
	{						
		NO_EXIST,				//�Ҳ�������ͼ�ļ�
		LOADING_ERROR,			//������ͼ�����г���
		NO_ANY_TEX,				//û���κ���ͼ
		//�ڴ�֮ǰ����µĸ�����Ϣ����
		COUNT					//������Ϣ����
	};
	typedef map<string, int> TextureInfoMap;			//string:��ͼ�ļ��� int:������ͼ�ĸ�����Ϣ
	typedef map<string, TextureInfoMap> MtlFileInfoMap; //�����ļ��Ͷ�Ӧ����ͼ�ļ���Ϣ

	MtlFileInfoMap m_mapErrorMtlFileInfo;				//��¼���ز����ļ���ص���ͼ�ļ�������Ϣ
	vector<string> m_vecLoadFailedMtl;					//��¼����ʧ�ܵĲ�����Ϣ
	vector<string> m_vecFilterMtl;						//��¼���˵Ĳ����ļ�
	IKG3DAutoTestInterface* m_pTestInterface;
	KG3DMaterialTable*		m_pMtlTable;
	char m_szApplicationPath[MAX_PATH];
	char m_szLogFileName[MAX_PATH];
	char m_szResultPath[MAX_PATH];


};
#endif