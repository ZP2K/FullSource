///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
// FileName : KHttpDownloader.h
// Creator  : chengzhaohui
// Date     : 2009-1-6
// Comment  : �����ļ�����
//
///////////////////////////////////////////////////////////////
#ifndef _KHTTP_DOWNLOADER_H_
#define _KHTTP_DOWNLOADER_H_

#include "IDownloader.h"
#include <vector>
#include <map>
#include "KHttpFile.h"

class KHttpDownloader : public IDownloader
{
public:
	KHttpDownloader(void);
	virtual ~KHttpDownloader(void);

	//�����߳�������Ĭ��Ϊ5����С1�����10
	void SetThreadCount(int nThreadCount);

	void SetNotification(IDownloaderNotification *pNotify);

	//************************************
	// Method:    �����Ҫ�������ص��ļ���ַ��һ�����һ����ַ
	// Parameter: const TCHAR szFileUrl[] �ļ���ַ
	// Parameter: const TCHAR szSavePath[] �ļ�����·��
	// Parameter: int nResumeFlag �Ƿ���Ҫ�ϵ�����
	//************************************
	int AddFile(
		const TCHAR szFileUrl[], 
		const TCHAR szSavePath[],
		int nResumeFlag = true
	);

	//************************************
	// ��ȡ�����ļ����ܴ�С
	//************************************
	__int64 GetTotalFileSize();
	//************************************
	// ��ȡ�����ļ��Ѿ����ص��ܴ�С
	//************************************
	__int64 GetTotalDownloadedSize();
	
	//************************************	
	// ��ʼ���ز��ȴ�һ�������ļ�������ɡ�
	// ��ĳ�ļ�������ɣ��������ش��ļ���Ϣָ�룬�������NULL����ʾ�����ļ�������ɡ�����ֹ�����
	// �����Ҫѭ������ֱ������NULL��
	//************************************
	IHttpFile *StartAndWait();

	void Abort();
	
private:
	int BatchDownload();
	static unsigned __stdcall ThreadFunction(void *pvParam);

private:
	IDownloaderNotification *m_pNotify;	

	enum{MIN_THREAD = 1, MAX_THREAD = 10, DEFAULT_THREAD = 5};
	int m_nThreadCount; //�����������߳�������һ���߳�����һ���ļ�

	typedef std::vector<IHttpFile *> READY_DOWNLOADER_VECT;
	READY_DOWNLOADER_VECT m_ReadyFiles; //��Ҫ���ص��ļ��б�
	int m_nNextFile;//ָ���¸���Ҫ���ص��ļ�

	typedef std::map<HANDLE, IHttpFile *> RUN_DOWNLOADER_MAP;
	RUN_DOWNLOADER_MAP m_DownloadingFiles; //���ض���

private:
	class KHttpFileCallback : public IHttpFileAsyncCallback
	{
	public:	
		KHttpFileCallback(){m_pDownloader = NULL; m_pNotify = NULL;}
		void OnProgress(IHttpFile *pHttpFile, int nRecvdSize);
		IDownloader *m_pDownloader;
		IDownloaderNotification *m_pNotify;
	private:
		KSyncLock m_Lock;
	}m_HttpFileCallback;
};

#endif