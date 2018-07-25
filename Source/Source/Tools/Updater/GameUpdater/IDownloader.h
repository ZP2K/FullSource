#ifndef _IHTTPDOWNLOADER_H_
#define _IHTTPDOWNLOADER_H_

#include <Windows.h>

enum DOWNLOADER_ERROR_CODE
{	
	dec_err_success,		   //�ɹ�
	dec_err_notstart,		   //δ��ʼ����
	dec_err_stop,			   //�û�ֹͣ����
	dec_err_url,			   //�ļ����ص�ַ����
	dec_err_cannotconnect,     //�޷����ӷ�����
	dec_err_disconnection,     //�������
	dec_err_noenoughdiskspace, //���̿ռ䲻��
	dec_err_writefileerror,    //д�ļ�����
	dec_err_cannotdownload     //���������ļ����п����ļ�������		
};

class IDownloader
{
public:
	virtual __int64 GetTotalFileSize() = 0;
	virtual __int64 GetTotalDownloadedSize() = 0;
};

class IHttpFile
{
public:
	virtual const TCHAR *FileName() = 0;
	virtual __int64		 FileSize() = 0;
	virtual __int64		 DownloadedSize() = 0;
	virtual int			 Error() = 0;
};

class IDownloaderNotification
{
public:
	virtual void OnProgress(IDownloader *pDownloader, IHttpFile *pHttpFile, int nRecvdSize) = 0;
};

 class KSyncLock
 {
 public:
 	KSyncLock()    { InitializeCriticalSection(&m_CriticalSection);            }
 	~KSyncLock()   { DeleteCriticalSection(&m_CriticalSection);                }
 	int Lock()      { EnterCriticalSection(&m_CriticalSection); return true;    }
 	int Unlock()    { LeaveCriticalSection(&m_CriticalSection); return true;    }
 private:
 	CRITICAL_SECTION m_CriticalSection;
 };

#endif 