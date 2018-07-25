
// HttpDownload.h: interface for the CHttpDownload class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPDOWNLOAD_H__4E55FDB1_0DE8_4ACD_94F4_CD097B5EAED0__INCLUDED_)
#define AFX_HTTPDOWNLOAD_H__4E55FDB1_0DE8_4ACD_94F4_CD097B5EAED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bufsocket.h"
class KHttpFile;

////////////////////////////////////////////////////////////////////////////////
// �ṹ����

#pragma pack(push, 1)

typedef struct _tagHttpDownloadStatus
{
	INT		nStatusType;				// ״̬����
	DWORD	dwFileSize;					// �ļ���С
	DWORD	dwFileDownloadedSize;		// �Ѿ����ش�С
	CString	strFileName;				// �ļ���
	INT		nErrorCount;				// �������
	CString strError;					// ����ԭ��
	DWORD	dwErrorCode;				// �������	
} HTTPDOWNLOADSTATUS,*PHTTPDOWNLOADSTATUS;

#pragma pack(pop)


////////////////////////////////////////////////////////////////////////////////
//					��������
////////////////////////////////////////////////////////////////////////////////
#undef	IN	
#define IN	// �������
#undef	OUT
#define OUT	// �������

////////////////////////////////////////////////////////////////////////////////
//					�ⲿ����
////////////////////////////////////////////////////////////////////////////////
// ���ؽ��
CONST INT	HTTP_RESULT_SUCCESS			= 0;	// �ɹ�
CONST INT	HTTP_RESULT_SAMEAS			= 1;	// Ҫ���ص��ļ��Ѿ����ڲ�����Զ���ļ�һ�£���������
CONST INT	HTTP_RESULT_STOP			= 2;	// ��;ֹͣ(�û��ж�)
CONST INT	HTTP_RESULT_FAIL			= 3;	// ����ʧ��
CONST INT	HTTP_RESULT_REDIRECT_FTP	= 4;	// �ض���FTP

// Added by Linsuyi  2002/02/01
CONST INT   HTTP_RESULT_REDIRECT_HTTP   = 5;    // �ض���HTTP

CONST INT   HTTP_RESULT_DISK_FULL       = 6;	//


// ��Ϣ
CONST WPARAM	MSG_HTTPDOWNLOAD_NETWORK	= (WPARAM)1; // ����״̬
CONST WPARAM	MSG_HTTPDOWNLOAD_STATUS		= (WPARAM)2; // ����״̬
CONST WPARAM	MSG_HTTPDOWNLOAD_MAX		= (WPARAM)32; //�������ɹ�����


// �������
CONST INT	HTTP_RETRY_NONE			= 0;	// ������
CONST INT	HTTP_RETRY_TIMES		= 1;	// ����һ������
CONST INT	HTTP_RETRY_ALWAYS		= 2;	// ��������(�п�����ѭ��)
CONST INT	DEFAULT_HTTP_RETRY_MAX	= 10;	//ȱʡ�����Դ���

// PROXY������
CONST INT	HTTP_PROXY_NONE			= 0;	// û�д���
CONST INT	HTTP_PROXY_HTTPGET		= 1;	// ͨ����HTTP��GET�ʹ���
CONST INT	HTTP_PROXY_HTTPCONNECT	= 2;	// HTTP CONNECT����
CONST INT	HTTP_PROXY_SOCKS4		= 3;	// SOCKS4 ����	
CONST INT	HTTP_PROXY_SOCKS4A		= 4;	// SOCKS4A����
CONST INT	HTTP_PROXY_SOCKS5		= 5;	// SOCKS 5����
CONST INT	HTTP_PROXY_USEIE		= 6;	// ʹ��IE���õĴ������������

//����״̬����
CONST INT	HTTP_STATUS_FILENAME			= 1;	// �ļ���
CONST INT	HTTP_STATUS_FILESIZE			= 2;	// �ļ���С
CONST INT	HTTP_STATUS_FILEDOWNLOADEDSIZE	= 3;	// �ļ��Ѿ����ش�С
CONST INT	HTTP_STATUS_ERRORCOUNT			= 4;	// �����������
CONST INT	HTTP_STATUS_ERRORCODE			= 5;	// �������
CONST INT	HTTP_STATUS_ERRORSTRING			= 6;	// ����ԭ��

// HTTP֧�ֵ�����
CONST INT	HTTP_VERB_MIN		= 0;
CONST INT	HTTP_VERB_POST      = 0;
CONST INT	HTTP_VERB_GET       = 1;
CONST INT	HTTP_VERB_HEAD      = 2;
CONST INT	HTTP_VERB_PUT       = 3;
CONST INT	HTTP_VERB_OPTIONS   = 4;
CONST INT	HTTP_VERB_DELETE    = 5;
CONST INT	HTTP_VERB_TRACE	    = 6;
CONST INT	HTTP_VERB_CONNECT   = 7;
CONST INT	HTTP_VERB_MAX		= 7;

////////////////////////////////////////////////////////////////////////////////
//							�ڲ�˽��
////////////////////////////////////////////////////////////////////////////////
// ȱʡ��ʱ����
static CONST DWORD HTTP_CONN_TIMEOUT	= 60*1000;	// 60��
static CONST DWORD HTTP_SEND_TIMEOUT	= 60*1000;	// 60��
static CONST DWORD HTTP_RECV_TIMEOUT	= 60*1000;	// 60��

// ��������Ľ��
static CONST INT	HTTP_REQUEST_SUCCESS		= 0;	// �ɹ�
static CONST INT	HTTP_REQUEST_ERROR			= 1;	// һ��������󣬿�������
static CONST INT	HTTP_REQUEST_STOP			= 2;	// ��;ֹͣ(�û��ж�) (��������)
static CONST INT	HTTP_REQUEST_FAIL			= 3;	// ʧ�� (��������)
static CONST INT	HTTP_REQUEST_REDIRECT_FTP	= 4;	// �ض���FTP

// HTTP STATUS CODE����
static CONST INT	HTTP_SUCCESS		= 0;	// �ɹ�		
static CONST INT	HTTP_REDIRECT		= 1;	// �ض���
static CONST INT	HTTP_FAIL			= 2;	// ʧ��
static CONST INT	HTTP_REDIRECT_FTP	= 3;	// �ض���FTP

// ȱʡ�˿ں�
static CONST INT	DEFAULT_HTTP_PORT	= 80;

// HTTP����
static LPCSTR	HTTP_VERB_STR[] = 
{
	_T("POST "),
	_T("GET "),
	_T("HEAD "),
	_T("PUT "),
	_T("OPTIONS "),
	_T("DELETE "),
	_T("TRACE "),
	_T("CONNECT ")
};


////////////////////////////////////////////////////////////////////////////////
//						�ඨ��
////////////////////////////////////////////////////////////////////////////////
class CHttpDownload  
{
public:
	CHttpDownload();
	virtual ~CHttpDownload();

public:
	BOOL IsUserStop();
	// ��̬����������64���롢����
	static INT Base64Encode(IN LPCTSTR lpszEncoding,OUT CString& strEncoded );
	static INT Base64Decode(IN LPCTSTR lpszDecoding,OUT CString& strDecoded );

	VOID SetUserAgent(LPCTSTR lpszUserAgent);
	VOID SetTimeout(DWORD dwSendTimeout,DWORD dwRecvTimeout,DWORD dwConnTimeout);
	VOID SetRetry(INT nRetryType,INT nRetryDelay=0,INT nRetryMax = 0);
	VOID SetNotifyWnd(KHttpFile* pNotify);
	VOID StopDownload();
	BOOL ParseURL(IN LPCTSTR lpszURL,OUT CString& strServer,OUT CString& strObject,OUT INT& nPort);
	BOOL GetDownloadFileStatus(IN LPCTSTR lpszDownloadUrl, LPCTSTR lpszSavePath, OUT DWORD &dwFileSize, /*OUT CTime &FileTime*/ OUT DWORD &dwDownloadedSize);
	INT  Download(LPCTSTR lpszDownloadUrl,LPCTSTR lpszSavePath = NULL,BOOL bForceDownload = FALSE,BOOL bSaveResponse = TRUE,INT nVerb = HTTP_VERB_GET,LPCTSTR lpszData = NULL,LPTSTR lpszFtpURL = NULL );
	
	//��ʼ��Socket by zhongguangze
	BOOL InitSocket(INT nMajorVer /*= 2*/ ,INT nMinorVer /*= 2*/);
	VOID UnInitSocket();

	BOOL		m_bStopDownload;	// ֹͣ����	
private:
	BOOL	CreateSocket();
	VOID	CloseSocket();
	INT		SendRequest(INT nVerb = HTTP_VERB_GET,LPTSTR lpszFtpURL = NULL);
	INT		GetInfo(IN LPCTSTR lpszHeader,OUT DWORD& dwContentLength,OUT DWORD& dwStatusCode,OUT CTime& TimeLastModified,OUT CString& strCookie,LPTSTR lpszFtpURL);
	CTime	GetTime(LPCTSTR lpszTime);
	VOID	GetFileName();
	INT		MakeConnection(CBufSocket* pBufSocket,CString strServer,INT nPort);
	BOOL	IsValidFileName(LPCTSTR lpszFileName);
	
private:	
	// ���ز���
	// ������URL�ͱ��ر���·��
	CString		m_strDownloadUrl;
	CString		m_strSavePath;		// ������ȫ·���򱣴�Ŀ¼	
	CString		m_strTempSavePath;	//��ʱ�ļ���·��
	BOOL		m_bSaveResponse;

	// ֹͣ����

	HANDLE		m_hStopEvent;		// ֹͣ�����¼�

	// ǿ����������(�������е��ļ��Ƿ���Զ���ļ���ͬ)
	BOOL		m_bForceDownload;

	// �Ƿ�֧�ֶϵ�����
	BOOL		m_bSupportResume;

	// �ļ��Լ����ش�С
	DWORD		m_dwFileSize;				// �ļ��ܵĴ�С	
	DWORD		m_dwFileDownloadedSize;		// �ļ��ܹ��Ѿ����صĴ�С

	DWORD		m_dwDownloadSize;			// ������Ҫ���صĴ�С
	DWORD		m_dwDownloadedSize;			// �����Ѿ����صĴ�С

	DWORD		m_dwHeaderSize;				// ����ͷ�Ĵ�С (��ʱû������)
	CString		m_strHeader;				// ����ͷ����Ϣ (��ʱû������)

	// �ļ�����(Զ���ļ�����Ϣ)
	CTime		m_TimeLastModified;

	// Referer
	CString		m_strReferer;
	
	// UserAgent
	CString		m_strUserAgent;

	// ��ʱTIMEOUT	���ӳ�ʱ�����ͳ�ʱ�����ճ�ʱ(��λ������)
	DWORD		m_dwConnTimeout;	
	DWORD		m_dwRecvTimeout;
	DWORD		m_dwSendTimeout;

	// ���Ի���
	INT			m_nRetryType;	//��������(0:������ 1:����һ������ 2:��������)
	INT			m_nRetryTimes;	//���Դ���
	INT			m_nRetryDelay;	//�����ӳ�(��λ������)
	INT			m_nRetryMax;    //���Ե�������

	// ������
	INT			m_nErrorCount;	//������� 
	CString		m_strError;		//������Ϣ 
	DWORD		m_dwErrorCode;	//�������

	// ���������ڷ�����Ϣ
	BOOL		m_bNotify;			// �Ƿ����ⷢ��֪ͨ��Ϣ	
	HWND		m_hNotifyWnd;		// ��֪ͨ�Ĵ���
	INT			m_nNotifyMessage;	// ��֪ͨ����Ϣ

	KHttpFile* m_pNotify;

	// �Ƿ������֤ : Request-Header: Authorization
	BOOL		m_bAuthorization;
	CString		m_strUsername;
	CString		m_strPassword;

	// �Ƿ�ʹ�ô��� 
	BOOL		m_bProxy;
	CString		m_strProxyServer;
	INT			m_nProxyPort;
	INT			m_nProxyType;
	
	// �����Ƿ���Ҫ��֤: Request-Header: Proxy-Authorization
	BOOL		m_bProxyAuthorization;
	CString		m_strProxyUsername;
	CString		m_strProxyPassword;

	// ���ع��������õı���
	CString		m_strServer;
	CString		m_strObject;
	CString		m_strFileName;
	INT			m_nPort;
	INT			m_nVerb;
	CString		m_strData;// ����ʱ���͵�����

	// �Ƿ�ʹ��Cookie
	CString		m_strCookie;
	BOOL		m_bUseIECookie;

	CBufSocket	m_cBufSocket;// �������ӵ�SOCKET
	
	// ����BASE64���롢����
	static INT		m_nBase64Mask[];
	static CString	m_strBase64TAB;
};

#endif // !defined(AFX_HTTPDOWNLOAD_H__4E55FDB1_0DE8_4ACD_94F4_CD097B5EAED0__INCLUDED_)
