
///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
// FileName : KSelfUpdater.H
// Creator  : chengzhaohui, zhongguangze
// Date     : 
// Comment  : gameupdater������ģ��
//
///////////////////////////////////////////////////////////////

#ifndef _KSELFUPDATER_H_
#define _KSELFUPDATER_H_

#include "UpdateDefine.h"
#include "KHttpDownloader.h"
#include <vector>

class KSelfUpdater
{
public:
    KSelfUpdater(void);
    ~KSelfUpdater(void);

	//��ʼ�Ը���
	//cpszRemotePath    Զ�̷�������URL
	//cpszUpdatePath    ��������Ҫ����ʱĿ¼
	//cpszClientVersion ���԰汾��Ŀǰֻ��zhcn
	//hWindows	        Flash�����Handle
	int Start(const TCHAR cpszRemotePath[], const TCHAR cpszUpdatePath[], 
		const TCHAR cpszClientVersion[], HANDLE hWindows = INVALID_HANDLE_VALUE);

    int Stop();
    
private:
	//���ط������ϵ� updaterversion.txt �ļ�
    int DownloadVersionFile();

	//�������ص�updaterversion.txt�ļ��������������¼��gameupdater�汾���Ƿ���ڱ��ص�
    int ParseVersionFile();

	//����updaterversion.txt�ļ�������¼���ļ�
    int DownloadUpdateFile();

	//�Ѿɵ�gameupdater�������ƶ�����ʱĿ¼��
	int _MoveOldUpdaterToTemp();
	
	int _StartupFlashWindow();
	int _CopyNewFileToClientPath();

	//���صĴ�����
	void ProcessDownloadError(const TCHAR szFile[], int nError); 

private:
    KHttpDownloader m_HttpClient;

    tstring m_strApp;
    tstring m_strPath;
    tstring m_strLocalVersionFile;
	TCHAR   m_szRemotePath[MAX_PATH];
	TCHAR   m_szUpdatePath[MAX_PATH];
	TCHAR   m_szClientVersion[MAX_VERSION_LENGTH];
	int     m_nNeedUpdateFlag;

	typedef std::vector<tstring> STRING_VECT;
	STRING_VECT m_SelfFileList;
};

#endif