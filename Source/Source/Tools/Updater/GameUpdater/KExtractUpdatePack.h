//////////////////////////////////////////////////////////////////////////////////////
//  Copyright(c) Kingsoft
//
//  FileName    :   KExtractUpdatePack.h
//  Creater     :   ZhongGuangZe
//  Date        :   2008-06-10
//  Comment     :   ��ѹ���°������¿ͻ���
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KEXTRACT_PACKAGE_H_
#define _KEXTRACT_PACKAGE_H_ 

#include <vector>
#include "unzip.h"

#define PAK_LIST_LMIT 128

using std::vector;

enum EXTRACT_PACK_RESULT
{
	EXTRACT_OK,
	EXTRACT_FAILD,
	EXTRACT_FINISH
};

class KExtractUpdatePack
{
public:
	KExtractUpdatePack();
	~KExtractUpdatePack() {UnInit();}

public:
	int          Init(const TCHAR szPackFileName[], const TCHAR szExtractPath[]);
	void         UnInit();

	EXTRACT_PACK_RESULT  ExtractNext();
	int          ExtractSingleFile(const TCHAR szZipName[], const TCHAR szFileName[], const TCHAR szOutPath[]);

	const TCHAR* GetCurrentFileName() {return m_szCurrentFile;}
	int          GetFileCount() {return m_nZipTotalFiles;}
	void         SetPackSize(unsigned int uSize) {m_uPackSize = uSize;} //�Զ���������

private:
	int _ExtractFileToPathFile(int nItemIndex, const TCHAR cszFullFileName[]);
	int _DisposePackFile(int nItemIndex);
	int _IsPackFileSuffix(const TCHAR szFileName[]);
	int _ReadPackageini();
	int _SavePackageList();
	int _AddUpdatePack();
	int _CheckPack(const TCHAR szDesPack[], const TCHAR szAddPack[]);  //���PACK���Ƿ����2G������2��PACK�����ļ�������������20W
	int _ExceptFile(const TCHAR szFileName[]);
	int _MargePackFile(const TCHAR szSrcPack[], const TCHAR szDesPack[]);

	int _GetFileNameLoaction(const TCHAR szFile[], int* nLocation);
	int _FileIsExist(const TCHAR szFilePath[]);
	int _GetFileSize(const TCHAR szFileName[], DWORD* dwRetSize);

    int _CreateEmptyFile(const TCHAR cszFullFileName[]);
    int _GetParentDirectory(const TCHAR cszFullName[], tstring *pstrRetParentDirectory);
    int _EnsureDirectory(const TCHAR cszDirectoryName[]);
private:	
	TCHAR*    m_pszPackageList[PAK_LIST_LMIT];
	int       m_nPackageCount;
	int		  m_nZipTotalFiles;				//�ļ�����
	int		  m_nZipItemIndex;				//��ǰ���ڽ�ѹ�����ļ�������
	int		  m_nVersionCfgIndex;
	HZIP	  m_pZip;
	unsigned  m_uPackSize;                  //�ְ����ļ���С

	TCHAR	  m_szPackFileName[MAX_PATH];	//ѹ����·��
	TCHAR	  m_szExtractPath[MAX_PATH];	//��ѹ��·��
	TCHAR     m_szCurrentFile[MAX_PATH];    //��ѹ�ĵ�ǰ�ļ���
	TCHAR     m_szPakPath[MAX_PATH];
};

#endif /* _KEXTRACT_PACKAGE_H_ */