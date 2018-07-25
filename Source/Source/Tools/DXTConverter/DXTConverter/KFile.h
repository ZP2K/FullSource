//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KFile.h    
//  Version     :   1.0
//  Creater     :   dengwenfeng
//  Date        :   2007-09-21 
//  Comment     :   �����ݱ��浽�ļ�
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KFILE_H_
#define _KFILE_H_

class KFile
{
public:
    KFile();
    ~KFile();
public:
    int OpenFile(const char* pcszFilePath);
    void CloseFile();
    int SaveFile(const char *cpszInfo);
private:
    FILE *m_pFile;
};

#endif