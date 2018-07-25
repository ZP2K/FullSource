/************************************************************************/
/* ��װ�͵�ͼ�����ļ���ض�ȡ����		                                */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.18	Create												*/
/************************************************************************/
#ifndef _KMAP_FILE_H_
#define	_KMAP_FILE_H_

#include "Global.h"

class KMapFile
{
public:
	KMapFile(void);
	~KMapFile(void);
//��ȡ���
	BOOL Open(char* pszMapName);
	BOOL GetRegionCount(int& rnRegionWidth, int& rnRegionHeight);
	BOOL GetCellTotal(DWORD& rdwCellTotal);
	BOOL GetNpcTotal(DWORD& rdwServerNpcTotal, DWORD& rdwClientNpcTotal);
	BOOL GetDoodadTotal(DWORD& rdwServerDoodadTotal, DWORD& rdwClientDoodadTotal);
	BOOL Close(void);

//д����أ�ֻ�б༭����
#ifdef _EDITOR
	BOOL Create(void);
	BOOL SetRegionCount(DWORD dwRegionWidth, DWORD dwRegionHeight);
	BOOL SetCellTotal(DWORD dwCellTotal);
	BOOL SetNpcTotal(DWORD dwServerNpcTotal, DWORD dwClientNpcTotal);
	BOOL SetDoodadTotal(DWORD dwServerDoodadTotal, DWORD dwClientDoodadTotal);
	BOOL Save(char* pszMapName);

#endif	//EDITOR

private:
	IIniFile* m_pIniFile;
};

#endif	//_KMAP_FILE_H_