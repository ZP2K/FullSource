
//
///////////////////////////////////////////////////////////////////////////////
// BufSocket.cpp: implementation of the CBufSocket class.
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "bufsocket.h"
#include "httpdownload.h"

#define Check(a,b) if((a)) return (b);

// ��ҪWS2_32.lib�ļ�
#pragma comment(lib,"ws2_32.lib")

CHttpDownload m_HttpDownload;
//CFtpDownload m_FtpDownload;

////////////////////////////////////////////////////////////////////////////////
//			���캯��
////////////////////////////////////////////////////////////////////////////////
CBufSocket::CBufSocket()
{
	m_hSocket		= INVALID_SOCKET;
	m_pBSD			= NULL;
	m_nErrorCode	= 0;	

	//seawind
	m_hStopEvent = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//			��������
////////////////////////////////////////////////////////////////////////////////
CBufSocket::~CBufSocket()
{
	Close(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL InitLibrary(
//				INT nMajorVer /*= 2*/,
//				INT nMinorVer /*= 2*/ ) 
//	��  ;����ʼ��WinSock2��̬���ӿ�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		nMajorVer : ���汾��
//		nMinorVer : �ΰ汾��
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::InitLibrary( INT nMajorVer /*= 2*/ ,INT nMinorVer /*= 2*/)
{

	WSADATA         WSD;
	WORD			wVersionRequired = MAKEWORD( nMajorVer, nMinorVer );
	
	ZeroMemory(&WSD,sizeof(WSADATA));

	INT nErrorCode = WSAStartup(wVersionRequired, &WSD);

	if ( SOCKET_SUCCESS != nErrorCode )
		return FALSE;

	if ( LOBYTE( WSD.wVersion ) != nMinorVer ||
		 HIBYTE( WSD.wVersion ) != nMajorVer ) 
	{
		WSACleanup( );
		return FALSE; 
	}
 
	//�ɹ���ʼ��
	return TRUE;
}
	

////////////////////////////////////////////////////////////////////////////////
//	��������VOID CleanupLibrary()
//	��  ;���ͷ�WinSock2��̬���ӿ�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ������
//	����ֵ��VOID
////////////////////////////////////////////////////////////////////////////////
VOID CBufSocket::CleanupLibrary()
{
    WSACleanup();	
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT GetLastError( VOID ) 
//	��  ;����ȡ���һ�β����Ĵ�����
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ������
//	����ֵ��INT�����һ�εĲ����Ĵ�����
////////////////////////////////////////////////////////////////////////////////
INT	CBufSocket::GetLastError(VOID)
{
	return m_nErrorCode;
}

////////////////////////////////////////////////////////////////////////////////
//	��������VOID SetLastError( INT nErrorCode ) 
//	��  ;���������һ�β����Ĵ�����
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		nErrorCode : �����õ��µĴ�����
//	����ֵ��VOID
////////////////////////////////////////////////////////////////////////////////
VOID CBufSocket::SetLastError(INT nErrorCode)
{
	m_nErrorCode = nErrorCode;
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Create(
//				INT nAddressFamily /*= AF_INET*/,
//				INT nType /*= SOCK_STREAM*/,
//				INT nProtocol /*= 0*/,
//				INT nBufferSize /*= DEFAULT_BUFFER_SIZE*/,
//				SOCKET hSocket /*= INVALID_SOCKET*/,
//				DWORD dwFlags /*= WSA_FLAG_OVERLAPPED*/ ) 
//	��  ;������SOCKET��BSD�ṹ
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		nAddressFamily : SOCKET��ַ����
//		nType          : SOCKET����
//		nProtocol      : SOCKET���õ�Э��
//		nBufferSize    : �������Ĵ�С
//		hSocket        : ��ʵ���󶨵��׽���
//		dwFlags        : �׽��ֵ�����
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL	CBufSocket::Create(	INT		nAddressFamily	/*= AF_INET*/, 
							INT		nType			/*= SOCK_STREAM*/, 
							INT		nProtocol		/*= 0*/,
							INT		nBufferSize		/*= DEFAULT_BUFFER_SIZE*/,
							SOCKET	hSocket			/*= INVALID_SOCKET*/,
							DWORD	dwFlags			/*= WSA_FLAG_OVERLAPPED*/)
{
    if (INVALID_SOCKET == hSocket)
    {
        m_hSocket = WSASocket(nAddressFamily, nType, nProtocol, NULL,0,dwFlags);
        
        if (INVALID_SOCKET == m_hSocket)
        {
            SetLastError( WSAGetLastError() ); //Ϊ�λ���˻���Ҫ����
            return FALSE;
        }
    }
    else
    {
        m_hSocket = hSocket;
    }
    
    if (SOCK_STREAM == nType)   //TCP
    {
        //�����׽���ѡ��
        if (!SetSocketOption(m_hSocket))    //��������ʧ��
        {
            Close(TRUE);
            m_hSocket = INVALID_SOCKET;
            return FALSE;
        }
        
        //����BSD
        
        //�����ڴ�
        m_pBSD = new BSD;
        
        if (NULL == m_pBSD)
        {
            Close(TRUE);
            return FALSE;
        }
        
        CHAR *pszBuffer = new CHAR[nBufferSize];
        if (NULL == pszBuffer)
        {
            Close(TRUE);
            
            delete m_pBSD;
            m_pBSD = NULL;

            return FALSE;
		}

		CHAR *pszBufferRecv = new CHAR[nBufferSize];
		if (NULL == pszBufferRecv)
		{
			Close(TRUE);

			delete m_pBSD;
			m_pBSD = NULL;

			delete pszBuffer;

			return FALSE;
		}
        
        // ���ýṹ��Ա
        ZeroMemory(m_pBSD, sizeof(BSD));
        ZeroMemory(pszBuffer, nBufferSize);
		ZeroMemory(pszBufferRecv, nBufferSize);
        
        m_pBSD->hSocket			= m_hSocket;
        m_pBSD->nBufferSize		= nBufferSize;
        m_pBSD->pszBuffer		= pszBuffer;
		m_pBSD->pszBufferRecv	= pszBufferRecv;
        m_pBSD->nBytesInBuffer	= 0;
        m_pBSD->nReadIndex		= 0;
        m_pBSD->nBufferIndex	= 0;
    }
    else	// UDP
    {
        m_pBSD = NULL;
    }
    
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	��������VOID Close( BOOL bHardClose /*= FALSE */ ) 
//	��  ;���ر�SOCKET
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		bHardClose : �Ƿ������ر�(�����Źر�)
//	����ֵ��VOID
////////////////////////////////////////////////////////////////////////////////
VOID CBufSocket::Close(	BOOL bHardClose /*= FALSE */)
{
    if (m_pBSD != NULL)
    {
        // �ͷ��ڴ�
        delete[] m_pBSD->pszBuffer;
		delete[] m_pBSD->pszBufferRecv;
        delete m_pBSD;
        m_pBSD = NULL;
    }
    
    if (INVALID_SOCKET == m_hSocket)
        return;
    
    // ����Ҫ�������
    if (!bHardClose) // ���Źر� Graceful close
    {
        // ���ٷ������ݣ�����TCP�׽��֣������е����ݶ��������֮��
        // ������һ�� FIN ��֪ͨ���շ����������Ѿ�������ϡ�
        shutdown(m_hSocket, SD_SEND);
        
        // ���ջ������п��ܻ���δ���յ����ݣ��ڹر��׽���֮ǰӦ����
        // ��ȡ���������ݡ�
        INT		nRecvResult;
        HANDLE	hSocketEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        CHAR	szBuffer[256];
        do
        {
            if (hSocketEvent != NULL)
            {
                //ע�������¼�
                WSAEventSelect(m_hSocket, (WSAEVENT) hSocketEvent, FD_READ | FD_CLOSE);
                WSAWaitForMultipleEvents(1, &hSocketEvent, TRUE,DEFAULT_SHUTDOWN_RECV_TIMEOUT, TRUE);
                //��������¼�
                WSAEventSelect(m_hSocket, (WSAEVENT) hSocketEvent, 0);
            }
            ZeroMemory(szBuffer,256);
            nRecvResult = RecvOnce(m_hSocket,szBuffer, sizeof(szBuffer));
        } while (nRecvResult > 0);
        
        if (hSocketEvent != NULL)
            CloseHandle(hSocketEvent);
        
        //����������պͷ���
        shutdown(m_hSocket, SD_BOTH);
    }
    
    // �ر��׽���
    closesocket(m_hSocket);
    m_hSocket = INVALID_SOCKET;
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL SetSocketOption( SOCKET hSocket ) 
//	��  ;������Socket������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket : �������Ե��׽���
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::SetSocketOption(SOCKET hSocket)
{
    INT	nActivate = 1;

	//�����ַ����
    if (setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (LPCSTR ) &nActivate,sizeof(nActivate)) == SOCKET_ERROR )
    {
		SetLastError( WSAGetLastError() );
        return FALSE;
    }

	//  ���֧�֣�����KEEPALIVE���� (����������������������)
	//setsockopt(hSocket, SOL_SOCKET, SO_KEEPALIVE, (LPCSTR ) &nActivate,sizeof(nActivate));
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT RecvOnce(
//				SOCKET hSocket,
//				OUT CHAR *pszBuffer,
//				INT nBufferSize ) 
//	��  ;��һ���Խ�������(�ص�IO)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : ����ȡ���ݵ��׽���
//		pszBuffer   : �������ݵĻ�����
//		nBufferSize : ��������С
//	����ֵ��INT
//		>=0			: �ɹ������ص��ǽ��յ��ֽ���
//		<0			: ʧ�ܣ����ص��Ǵ�������෴��
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::RecvOnce(SOCKET hSocket,OUT CHAR *pszBuffer, INT nBufferSize)
{

	DWORD		dwRecvBytes = 0,
				dwFlags		= 0;
	WSABUF		WSABuff;

	//��ջ���
	ZeroMemory(&WSABuff,sizeof(WSABUF));

	WSABuff.len = nBufferSize;
	WSABuff.buf = pszBuffer;

	return ((WSARecv(hSocket, &WSABuff, 1, &dwRecvBytes, &dwFlags,NULL, NULL) == SOCKET_SUCCESS) ? (INT) dwRecvBytes : -WSAGetLastError());

}


////////////////////////////////////////////////////////////////////////////////
//	��������INT RecvData(
//				SOCKET hSocket,
//				OUT CHAR *pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout ) 
//	��  ;����������(����ֱ���յ�����Ϊֹ)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : ���������ݵ��׽���
//		pszBuffer   : ������
//		nBufferSize : ��������С
//		dwTimeout   : ���ճ�ʱ
//	����ֵ��INT
//		>=0			: ���յ����ֽ���
//		<0			: ʧ��(SOCKET_FAIL: -1)
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::RecvData(SOCKET hSocket, OUT CHAR *pszBuffer, INT nBufferSize, DWORD dwTimeout)
{
	HANDLE hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hReadEvent == NULL)
	{
		SetLastError( (INT)GetLastError() );
		return ( SOCKET_FAIL );
	}

	INT		nRecvBytes = 0;
	DWORD	dwWaitResult;
	for (;;)
	{
		// ע��FD_READ | FD_CLOSE �¼� 
		// (��Ϊ�����ڵȴ�FD_READ�¼��У��Է��ر��׽��֣�����Ҫ��עFD_CLOSE)
		if( WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}
	
		// �ȵ�FD_READ | FD_CLOSE�¼��ķ���
		//dwWaitResult = WSAWaitForMultipleEvents(1, &hReadEvent, TRUE,dwTimeout, TRUE);

		//seawind
		DWORD dwBeginTime = GetTickCount();
		BOOL bDataCome = FALSE;
		
		while(GetTickCount() - dwBeginTime < dwTimeout)
		{			
			//��HttpDownload,FtpDownload�๲��һ��Event Handle
			if (m_hStopEvent != NULL && (WaitForSingleObject(m_hStopEvent, 0) == WAIT_OBJECT_0))
			{
				//Ҫ�Ͽ����ߣ��û�����Stop

				CloseHandle(hReadEvent);
				SetLastError( WSAGetLastError() );
				return (SOCKET_FAIL);
				
				//break;
			}

			dwWaitResult = WSAWaitForMultipleEvents(1, &hReadEvent, TRUE, 0, TRUE);
			
			if (dwWaitResult == WSA_WAIT_EVENT_0)
			{
				//��������
				bDataCome = TRUE;
				break;
			}

			//�ճ�CPUʱ��
			Sleep(DEFAULT_BLOCKED_SNDRCV_SLEEP);
		}

		//if (dwWaitResult != WSA_WAIT_EVENT_0)
		if (!bDataCome)
		{
			// ����¼�
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		
		////////////////////////////////////////////////////////////// 
		///	ע�⣺��ʹ dwWaitResult == WSA_WAIT_EVENT0 ��ҲӦ�� 
		///			��һ����������Ƿ�������
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hReadEvent,&NetEvent) == SOCKET_ERROR)
		{
			// ����¼�
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}

		//�жϷ�����ʲô�¼� FD_READ �� FD_CLOSE	
		if( ( NetEvent.lNetworkEvents == FD_CLOSE ) ||
			( NetEvent.lNetworkEvents	== FD_READ && 
			  NetEvent.iErrorCode[FD_READ_BIT] !=0 ) )	// ��������
		{
			// ����¼�
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError(WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		////////////////////////////////////////////////////////////////
		// ����¼�
		WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
	
		// ��������
		if ((nRecvBytes = RecvOnce(hSocket, pszBuffer, nBufferSize)) >= 0)
			break;	// ����ѭ��

		INT nErrorCode = -nRecvBytes;

		if ( nErrorCode != WSAEWOULDBLOCK )	//̫���δ����ص�����
		{
			CloseHandle(hReadEvent);
			SetLastError( nErrorCode );
			return (SOCKET_FAIL);
		}

		//����ס��
		////////////////////////////////////////////////////////////////////////
		//  �����������,�͵ȴ�һ��ʱ�������,����CPU��ѯ�˷�ʱ��
		////////////////////////////////////////////////////////////////////////
		Sleep(DEFAULT_BLOCKED_SNDRCV_SLEEP);
    }

    CloseHandle(hReadEvent);
    return (nRecvBytes);
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT Recv(
//				OUT CHAR *pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/ ) 
//	��  ;����������ֱ���ﵽ�涨�ĳ���(��������)��ʱ��û�����ݿɶ�ȡʱ
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszBuffer   : ������
//		nBufferSize : ��������С
//		dwTimeout   : ���ճ�ʱ
//	����ֵ��INT�����յ����ֽ���
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::Recv(OUT CHAR *pszBuffer, INT nBufferSize, DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/)
{
	INT	nRecvBytes		= 0,
		nRecvCurrent	= 0;

	while (nRecvBytes < nBufferSize)
	{
		nRecvCurrent = RecvData(m_hSocket, (pszBuffer + nRecvBytes),(nBufferSize - nRecvBytes), dwTimeout);

		if (nRecvCurrent < 0)	//û�����ݿɶ�ȡ
			return (nRecvBytes);

		nRecvBytes += nRecvCurrent;
	}
	return (nRecvBytes);
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT SendOnce(
//				SOCKET hSocket,
//				LPCSTR pszBuffer,
//				INT nBufferSize ) 
//	��  ;��һ���Է�������(�ص�IO)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : �����͵��׽���
//		pszBuffer   : �������ݻ�����
//		nBufferSize : ��������С
//	����ֵ��INT
//		>=0			: �ɹ����ѳɹ����͵��ֽ���
//		<0			: ʧ�ܣ���������෴��
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::SendOnce(SOCKET hSocket, LPCSTR pszBuffer, INT nBufferSize)
{
	DWORD	dwSendBytes = 0;
	WSABUF	WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = nBufferSize;
	WSABuff.buf = (CHAR *) pszBuffer;

	return ((WSASend(hSocket, &WSABuff, 1, &dwSendBytes, 0,NULL, NULL) == SOCKET_SUCCESS) ? (INT) dwSendBytes : -WSAGetLastError());

}


////////////////////////////////////////////////////////////////////////////////
//	��������INT SendData(
//				SOCKET hSocket,
//				LPCSTR pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout ) 
//	��  ;����������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : �����͵��׽���
//		pszBuffer   : �������ݻ�����
//		nBufferSize : ��������С
//		dwTimeout   : ���ͳ�ʱ
//	����ֵ��INT
//		>=0			: �ɹ����ѳɹ����͵��ֽ���
//		<0			: ʧ��(SOCKET_FAIL:-1)
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::SendData(SOCKET hSocket, LPCSTR pszBuffer, INT nBufferSize, DWORD dwTimeout)
{
	HANDLE	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hWriteEvent == NULL)
	{
		SetLastError( (INT)GetLastError() );
		return (SOCKET_FAIL);
	}

	INT nSendBytes = 0;

	for (;;)
	{
		////////////////////////////////////////////////////////////////
		// �������ݳɹ�
		if ((nSendBytes = SendOnce(hSocket, pszBuffer, nBufferSize)) >= 0)
			break;

		INT nErrorCode = -nSendBytes;

		if (nErrorCode != WSAEWOULDBLOCK)
		{
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}

		
		///////////////////////////////////////////////////////////////////////////////
		//  ˯��һ��ʱ��
		///////////////////////////////////////////////////////////////////////////////
		Sleep(DEFAULT_BLOCKED_SNDRCV_SLEEP);

		// ע��FD_WRITE | FD_CLOSE �¼�
		if( WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, FD_WRITE|FD_CLOSE) == SOCKET_ERROR)
		{
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}

		// �ȴ��¼�����
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hWriteEvent, TRUE,dwTimeout, TRUE);

		if (dwWaitResult != WSA_WAIT_EVENT_0)
		{
			// ��������¼�
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}

		////////////////////////////////////////////////////////////// 
		///	ע�⣺��ʹ dwWaitResult == WSA_WAIT_EVENT0 ��ҲӦ�� 
		///			��һ����������Ƿ�������
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hWriteEvent,&NetEvent) == SOCKET_ERROR)
		{
  			// ��������¼�
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		if( ( NetEvent.lNetworkEvents == FD_CLOSE ) ||
			( NetEvent.lNetworkEvents == FD_WRITE   &&
			  NetEvent.iErrorCode[FD_WRITE_BIT] !=0 ) )	// ��������
		{
			// ��������¼�
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		// ��������¼�
		WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
   }

    CloseHandle(hWriteEvent);
    return (nSendBytes);
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT Send(
//				LPCSTR pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout /*= DEFAULT_SEND_TIMEOUT*/ ) 
//	��  ;���������������ݻ�ʱ
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszBuffer   : �������ݻ�����
//		nBufferSize : ��������С
//		dwTimeout   : ���ͳ�ʱ
//	����ֵ��INT���Ѿ��ɹ����͵��ֽ���
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::Send(LPCSTR pszBuffer, INT nBufferSize, DWORD dwTimeout /*= DEFAULT_SEND_TIMEOUT*/)
{
	INT nSendBytes		= 0,
		nSendCurrent	= 0;

	while (nSendBytes < nBufferSize)
	{
		nSendCurrent = SendData(m_hSocket, (pszBuffer + nSendBytes),(nBufferSize - nSendBytes), dwTimeout);
		if (nSendCurrent < 0)
			return (nSendBytes);

		nSendBytes += nSendCurrent;
	}
	return (nSendBytes);
}


////////////////////////////////////////////////////////////////////////////////
//	��������INT BSDReadData(
//				PBSD pBSD,
//				DWORD dwTimeout ) 
//	��  ;���ӻ�������ȡ����
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pBSD      : BufSocketData�ṹָ��
//		dwTimeout : ���ճ�ʱ
//	����ֵ��INT
//		>=0			: ��ȡ���ֽ���
//		<0			: ʧ�� (SOCKET_FAIL: -1)	
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::BSDReadData(DWORD dwTimeout)
{
	if (m_pBSD == NULL)
		return 0;

	// ���㻺������������
	INT nMaxRead = m_pBSD->nBufferSize - m_pBSD->nBytesInBuffer;

	// ��������
	INT nReadedBytes = RecvData(m_pBSD->hSocket, m_pBSD->pszBufferRecv, nMaxRead, dwTimeout);
	if (nReadedBytes > 0)
	{
		INT nHeadSize = Min(m_pBSD->nBufferSize - m_pBSD->nBufferIndex, nReadedBytes);

		if (nHeadSize > 0)
			memcpy(m_pBSD->pszBuffer + m_pBSD->nBufferIndex, m_pBSD->pszBufferRecv, nHeadSize);

		m_pBSD->nBufferIndex += nHeadSize;
		if (m_pBSD->nBufferIndex == m_pBSD->nBufferSize)
			m_pBSD->nBufferIndex = 0;

		INT nBackSize = nReadedBytes - nHeadSize;
		if (nBackSize > 0)
			memcpy(m_pBSD->pszBuffer + m_pBSD->nBufferIndex, m_pBSD->pszBufferRecv + nHeadSize, nBackSize);

		m_pBSD->nBufferIndex += nBackSize;
		m_pBSD->nBytesInBuffer += nReadedBytes;
	}

	return nReadedBytes;
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT BSDGetChar( DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/ ) 
//	��  ;���ӻ�������ȡһ���ַ�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		dwTimeout : ���ճ�ʱ
//	����ֵ��INT��һ���ַ�(����ʱ����INVALID_CHAR)
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::BSDGetChar(DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/)
{
	if (m_pBSD == NULL)
		return (INVALID_CHAR);

	if ((m_pBSD->nBytesInBuffer == 0) && (BSDReadData(dwTimeout) <= 0))
		return (INVALID_CHAR);

	INT nChar =( (INT) m_pBSD->pszBuffer[m_pBSD->nReadIndex]) & 0x000000ff;

	m_pBSD->nReadIndex = INext(m_pBSD->nReadIndex, m_pBSD->nBufferSize);

	--m_pBSD->nBytesInBuffer;

	return (nChar);
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT BSDGetString(
//				OUT CHAR *pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/ ) 
//	��  ;���ӻ�������ȡһ��(����������0x0A)(ע�⣬��β�Ļس����в�����������)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszBuffer   : ���ջ�����
//		nBufferSize : �����С
//		dwTimeout   : ���ճ�ʱ
//	����ֵ��INT
//		 0			: �ɹ� (SOCKET_SUCCESS)
//		-1			: ʧ�� (SOCKET_FAIL)	
//		 1			: ����̫С(SOCKET_BUFFER_TOO_SMALL)
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::BSDGetString(OUT CHAR *pszBuffer, INT nBufferSize,DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/)
{
	if( m_pBSD == NULL)
		return (SOCKET_FAIL);
	
	INT	nChar,nLastChar;
	INT i = 0;
	BOOL bEnough = FALSE;

	nLastChar = INVALID_CHAR;
	for (i = 0; i < (nBufferSize - 1);)
	{
		//add by holy 2003 - 12 - 10 21:03
		
		//Check(m_FtpDownload.m_bStopDownload, 1);
		Check(m_HttpDownload.m_bStopDownload, 1);
		nChar = BSDGetChar(dwTimeout);

		if (nChar == INVALID_CHAR)
			return SOCKET_FAIL;

		if (nChar == 0x0A)	// ���з���
		{
			bEnough = TRUE;
			if ( nLastChar == 0x0D)
				i-- ;	
			break;
		}
		else
			pszBuffer[i++] = (CHAR) nChar; //pszBuffer�Ǵ�����ݵĵط�

		nLastChar = nChar;
			
	}
	pszBuffer[i] = '\0';
	
	return (bEnough)?(SOCKET_SUCCESS):(SOCKET_BUFFER_TOO_SMALL);
}

////////////////////////////////////////////////////////////////////////////////
//	��������INT BSDGetData(
//				OUT CHAR *pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/ ) 
//	��  ;���ӻ�������ȡһ������������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszBuffer   : ���ջ�����
//		nBufferSize : �����С
//		dwTimeout   : ���ճ�ʱ
//	����ֵ��INT
//		>=0			: ��ȡ���ֽ���
//		<0			: ʧ��(SOCKET_FAIL:-1)
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::BSDGetData(OUT CHAR *pszBuffer, INT nBufferSize,DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/)
{
	//add by holy 2003 - 12 -11 16:04
	//Check(m_FtpDownload.m_bStopDownload, FTP_RESULT_STOP);
	//Check(m_HttpDownload.m_bStopDownload, FTP_RESULT_STOP);

	if (m_pBSD == NULL)
		return (SOCKET_FAIL);

	if ((m_pBSD->nBytesInBuffer == 0) && (BSDReadData(dwTimeout) <= 0))
		return SOCKET_FAIL; 

	INT nHeadSize = 0;
	INT nBackSize = 0;

	if (m_pBSD->nBytesInBuffer < nBufferSize) // ���ݲ�����
	{
		nHeadSize = Min(m_pBSD->nBufferSize - m_pBSD->nReadIndex, m_pBSD->nBytesInBuffer);
		memcpy(pszBuffer, m_pBSD->pszBuffer + m_pBSD->nReadIndex, nHeadSize);
		m_pBSD->nReadIndex += nHeadSize;
		if (m_pBSD->nReadIndex == m_pBSD->nBufferSize)
			m_pBSD->nReadIndex = 0;

		nBackSize = m_pBSD->nBytesInBuffer - nHeadSize;
		if (nBackSize > 0)
		{
			memcpy(pszBuffer + nHeadSize, m_pBSD->pszBuffer + m_pBSD->nReadIndex, nBackSize);
			m_pBSD->nReadIndex += nBackSize;
		}
	}
	else // ��ε������㹻����
	{
		nHeadSize = Min(m_pBSD->nBufferSize - m_pBSD->nReadIndex, nBufferSize);
		memcpy(pszBuffer, m_pBSD->pszBuffer + m_pBSD->nReadIndex, nHeadSize);
		m_pBSD->nReadIndex += nHeadSize;
		if( m_pBSD->nReadIndex == m_pBSD->nBufferSize )
			m_pBSD->nReadIndex = 0;

		nBackSize = nBufferSize - nHeadSize;
		if (nBackSize > 0)
		{
			memcpy(pszBuffer + nHeadSize, m_pBSD->pszBuffer + m_pBSD->nReadIndex, nBackSize);
			m_pBSD->nReadIndex += nBackSize;
		}
	}

	m_pBSD->nBytesInBuffer -= (nHeadSize + nBackSize);

	return nHeadSize + nBackSize;
}


////////////////////////////////////////////////////////////////////////////////
//	��������BOOL GetSockName(
//				OUT CHAR *pszIP,
//				OUT INT& nPort ) 
//	��  ;����ȡSocket�󶨵�IP��ַ�Ͷ˿�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszIP : ���ڱ���IP��ַ
//		nPort : ���ڱ���˿�
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::GetSockName(OUT CHAR *pszIP,OUT INT& nPort)
{
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr,sizeof(sockAddr));
	INT nSockAddrLen = sizeof(sockAddr);
	
	if( getsockname(m_hSocket,(SOCKADDR*)&sockAddr, &nSockAddrLen) == SOCKET_ERROR )
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	
	LPSTR pszTemp = inet_ntoa(sockAddr.sin_addr);
	memcpy(pszIP,pszTemp,strlen(pszTemp) );
	nPort = ntohs(sockAddr.sin_port);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	��������static DWORD WINAPI DNSThread( LPVOID pParam ) 
//	��  ;�����������̣߳�����ú�����Ŀ����Ϊ�˱���NT�¶�����������CACHE��ɵ�����
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pParam : ������������
//	����ֵ��DWORD��IP��ַ��ֵ�����ִ��󷵻�INADDR_NONE
////////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI DNSThread(  LPVOID pParam )
{
	DWORD dwIP = INADDR_NONE;
	PHOSTENT pHost = gethostbyname( (CHAR *)pParam );
	if(pHost == NULL)
		return INADDR_NONE;
	dwIP = inet_addr( inet_ntoa(*(IN_ADDR *)*pHost->h_addr_list) );
	return dwIP;
}

////////////////////////////////////////////////////////////////////////////////
//	��������DWORD GetIP(
//				LPCSTR pszName,
//				BOOL bFixNtDNS /*= FALSE*/ ) 
//	��  ;����ȡIP��ַ
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszName   : ������������
//		bFixNtDNS : �Ƿ�ʹ���߳������NT������������Cache����
//	����ֵ��DWORD��IP��ַ��ֵ�����ִ��󷵻�INADDR_NONE
////////////////////////////////////////////////////////////////////////////////
DWORD CBufSocket::GetIP(LPCSTR pszName,BOOL bFixNtDNS  /*= FALSE*/)
{
	DWORD dwIP = inet_addr(pszName);
	if( dwIP != INADDR_NONE )
		return dwIP;

	if( bFixNtDNS )
	{
		OSVERSIONINFO		osVersion;
		osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if( GetVersionEx(&osVersion) )
		{
			if(osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				DWORD	dwThreadId = 0;
				HANDLE	hThread = CreateThread(NULL,0,DNSThread,(LPVOID)pszName,0,&dwThreadId);
				if( hThread != NULL)
				{
					WaitForSingleObject(hThread,INFINITE);
					if( GetExitCodeThread(hThread,&dwIP))
						return dwIP;
				}
			}
		}
	}

	PHOSTENT pHost = gethostbyname(pszName);
	if(pHost == NULL)
		return INADDR_NONE;
		
	dwIP = inet_addr( inet_ntoa(*(IN_ADDR *)*pHost->h_addr_list) );
	return dwIP;
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Connect(
//				SOCKET hSocket,
//				CONST struct sockaddr * pSocketAddr,
//				INT nAddrLen,
//				DWORD dwTimeout ) 
//	��  ;����������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : �����������ӵ��׽���
//		pSocketAddr : SOCKADDR�ṹָ�룬����Ŀ���ַ
//		nAddrLen    : ��ַ�ṹ�ĳ���
//		dwTimeout   : ���ӳ�ʱ
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::Connect(SOCKET hSocket, CONST struct sockaddr * pSocketAddr, INT nAddrLen,DWORD dwTimeout)
{
	HANDLE hConnectEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hConnectEvent == NULL)
	{
		SetLastError( (INT)GetLastError() );
		return FALSE;
	}

	// ע��FD_CONNECT�¼�
	if( WSAEventSelect(hSocket, (WSAEVENT) hConnectEvent, FD_CONNECT) == SOCKET_ERROR)
	{
		CloseHandle(hConnectEvent);
		SetLastError( WSAGetLastError() );
		return FALSE;
	}
	//��ͨѶ����������
	INT	nConnectResult = WSAConnect(hSocket, pSocketAddr, nAddrLen, NULL, NULL, NULL, NULL);
	INT	nConnectError  = WSAGetLastError();
	
	if ((nConnectResult == SOCKET_ERROR) && (nConnectError == WSAEWOULDBLOCK))
	{
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hConnectEvent, TRUE,dwTimeout, TRUE);
		
		if (dwWaitResult != WSA_WAIT_EVENT_0)
		{
			SetLastError( WSAGetLastError() );
			nConnectResult = SOCKET_FAIL;
		}
		else
		{
			////////////////////////////////////////////////////////////// 
			///	ע�⣺��ʹ dwWaitResult == WSA_WAIT_EVENT0 ��ҲӦ�� 
			///			��һ����������Ƿ�������
			///////////////////////////////////////////////////////////////
			WSANETWORKEVENTS NetEvent;
			if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hConnectEvent,&NetEvent) == SOCKET_ERROR)
			{
				SetLastError( WSAGetLastError() );
				nConnectResult = SOCKET_FAIL;
			}
			else if(NetEvent.iErrorCode[FD_CONNECT_BIT] !=0 )	// ��������
			{
				SetLastError( NetEvent.iErrorCode[FD_CONNECT_BIT] );
				nConnectResult = SOCKET_FAIL;
			}
			else
				nConnectResult = SOCKET_SUCCESS;
			////////////////////////////////////////////////////////////////
		}
	}

	// ע�������¼�
	WSAEventSelect(hSocket, (WSAEVENT) hConnectEvent, 0);
	CloseHandle(hConnectEvent);

	return (nConnectResult == SOCKET_SUCCESS)?TRUE:FALSE;
}


////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Connect(
//				LPCSTR pszServer,
//				INT nPort,
//				DWORD dwTimeout /*= DEFAULT_CONNECT_TIMEOUT*/,
//				BOOL bFixNtDNS /*= FALSE*/ ) 
//	��  ;����������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszServer : �����ӵ�Ŀ���ַ
//		nPort     : �����ӵĶ˿�
//		dwTimeout : ���ӳ�ʱ
//		bFixNtDNS : �Ƿ���NT����������CACHE����
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::Connect(LPCSTR pszServer, INT nPort,DWORD dwTimeout /*= DEFAULT_CONNECT_TIMEOUT*/,BOOL bFixNtDNS /*= FALSE*/)
{
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr,sizeof(sockAddr));//���а�SOCKADDR_IN����Ϊ��

	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_port			= htons((u_short)nPort);
	sockAddr.sin_addr.s_addr	= GetIP(pszServer,bFixNtDNS);
	
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		SetLastError( WSAGetLastError() );
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////

	return Connect(m_hSocket, (SOCKADDR *)&sockAddr,sizeof(sockAddr),dwTimeout);

}


////////////////////////////////////////////////////////////////////////////////
//	��������SOCKET GetSocket()
//	��  ;����ȡ�׽���
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ������
//	����ֵ��SOCKET
////////////////////////////////////////////////////////////////////////////////
SOCKET CBufSocket::GetSocket()
{
	return m_hSocket;
}


////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Listen( INT nConnections /* = 5 */ ) 
//	��  ;�������׽���
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		nConnections : �������еĳ���
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::Listen(INT nConnections /* = 5 */ )
{
	if(listen(m_hSocket, nConnections) == SOCKET_ERROR)
	{
		SetLastError( WSAGetLastError() );
		return FALSE;
	}
	
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Bind( INT nPort /* = 0 */ ) 
//	��  ;�����׽���
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		nPort : �󶨵Ķ˿�
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::Bind(INT nPort /* = 0 */)
{
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr,sizeof(sockAddr));

	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	sockAddr.sin_port			= htons((u_short)nPort);

	return	Bind( m_hSocket,(SOCKADDR *)&sockAddr, sizeof(sockAddr));

}

////////////////////////////////////////////////////////////////////////////////
//	��������SOCKET Accept(
//				struct sockaddr * pSocketAddr,
//				LPINT nAddrLen,
//				DWORD dwTimeout /*= DEFAULT_ACCEPT_TIMEOUT*/ ) 
//	��  ;�������׽�������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pSocketAddr : SOCKET��ַ
//		nAddrLen    : ��ַ�����ĳ���
//		dwTimeout   : ��ʱ
//	����ֵ��SOCKET
////////////////////////////////////////////////////////////////////////////////
SOCKET CBufSocket::Accept(struct sockaddr * pSocketAddr, LPINT nAddrLen,DWORD dwTimeout /*= DEFAULT_ACCEPT_TIMEOUT*/)
{
	HANDLE hAcceptEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hAcceptEvent == NULL)
	{
		SetLastError( (INT)GetLastError() );
		return (INVALID_SOCKET);
	}

	// ע��FD_ACCEPT�¼�
	if( WSAEventSelect(m_hSocket, (WSAEVENT) hAcceptEvent, FD_ACCEPT) == SOCKET_ERROR)
	{
		CloseHandle(hAcceptEvent);
		SetLastError( WSAGetLastError() );
		return (INVALID_SOCKET);
	}

	SOCKET hSocketAccept = WSAAccept(m_hSocket, pSocketAddr, nAddrLen, NULL, 0);
	INT	   nConnectError = WSAGetLastError();

	if ((hSocketAccept == INVALID_SOCKET) && (nConnectError == WSAEWOULDBLOCK))
	{

		// ����
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hAcceptEvent, TRUE,dwTimeout, TRUE);

		if (dwWaitResult == WSA_WAIT_EVENT_0)
		{
			////////////////////////////////////////////////////////////// 
			///	ע�⣺��ʹ dwWaitResult == WSA_WAIT_EVENT0 ��ҲӦ�� 
			///			��һ����������Ƿ�������
			///////////////////////////////////////////////////////////////
			WSANETWORKEVENTS NetEvent;
			if(WSAEnumNetworkEvents(m_hSocket,hAcceptEvent,&NetEvent) == SOCKET_ERROR)
				SetLastError( WSAGetLastError() );
			else if(NetEvent.iErrorCode[FD_ACCEPT_BIT] !=0 )	// ��������
				SetLastError( NetEvent.iErrorCode[FD_ACCEPT_BIT] );
			else
				hSocketAccept = WSAAccept(m_hSocket, pSocketAddr, nAddrLen, NULL, 0);
		}
		else
			SetLastError( WSAGetLastError() );
	}
	
	// ע�������¼�
	WSAEventSelect( m_hSocket, (WSAEVENT) hAcceptEvent, 0);
	CloseHandle(hAcceptEvent);

	if (hSocketAccept != INVALID_SOCKET)
	{
		// �����׽��ֵ�����Ϊ��ַ�����ò���Ϊ��������
		if ( !Block(hSocketAccept, 0) || !SetSocketOption(hSocketAccept) )
		{
			closesocket(hSocketAccept);
			return (INVALID_SOCKET);
		}
	}
	return (hSocketAccept);
}


////////////////////////////////////////////////////////////////////////////////
//	��������SOCKET Accept(
//				struct sockaddr * pSocketAddr,
//				LPINT nAddrLen,
//				HANDLE hEndEvent,
//				DWORD dwTimeout /*= DEFAULT_ACCEPT_TIMEOUT*/ ) 
//	��  ;�������׽�������(�����ж�)
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pSocketAddr : SOCKET��ַ
//		nAddrLen    : ��ַ����
//		hEndEvent   : ���������¼�
//		dwTimeout   : ��ʱ
//	����ֵ��SOCKET
////////////////////////////////////////////////////////////////////////////////
SOCKET CBufSocket::Accept(struct sockaddr * pSocketAddr, LPINT nAddrLen,HANDLE hEndEvent,DWORD dwTimeout /*= DEFAULT_ACCEPT_TIMEOUT*/)
{
	if( hEndEvent == NULL)
		return Accept(pSocketAddr,nAddrLen,dwTimeout);

	HANDLE	hAcceptEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hAcceptEvent == NULL)
	{
		SetLastError( (INT)GetLastError() );
		return (INVALID_SOCKET);
	}

	WSAEVENT hEvent[2]; 
	hEvent[0] = (WSAEVENT)hAcceptEvent;
	hEvent[1] = (WSAEVENT)hEndEvent;

	// ע��FD_ACCEPT�¼�
	if( WSAEventSelect(m_hSocket, (WSAEVENT) hAcceptEvent, FD_ACCEPT) == SOCKET_ERROR)
	{
		CloseHandle(hAcceptEvent);
		SetLastError( WSAGetLastError() );
		return (INVALID_SOCKET);
	}

	SOCKET hSocketAccept = WSAAccept(m_hSocket, pSocketAddr, nAddrLen, NULL, 0);
	INT	   nConnectError = WSAGetLastError();

	if ((hSocketAccept == INVALID_SOCKET) && (nConnectError == WSAEWOULDBLOCK))
	{
		// ����
		DWORD dwWaitResult = WSAWaitForMultipleEvents(2, hEvent, FALSE,dwTimeout, TRUE);
		if (dwWaitResult == WSA_WAIT_EVENT_0)
		{
			////////////////////////////////////////////////////////////// 
			///	ע�⣺��ʹ dwWaitResult == WSA_WAIT_EVENT0 ��ҲӦ�� 
			///			��һ����������Ƿ�������
			///////////////////////////////////////////////////////////////
			WSANETWORKEVENTS NetEvent;
			if(WSAEnumNetworkEvents(m_hSocket,hAcceptEvent,&NetEvent) == SOCKET_ERROR)
				SetLastError( WSAGetLastError() );
			else if(NetEvent.iErrorCode[FD_ACCEPT_BIT] !=0 )	// ��������
				SetLastError( NetEvent.iErrorCode[FD_ACCEPT_BIT] );
			else
				hSocketAccept = WSAAccept(m_hSocket, pSocketAddr, nAddrLen, NULL, 0);
		}
		else
			SetLastError( WSAGetLastError() );
	}
	
	// ע�������¼�
	WSAEventSelect(m_hSocket, (WSAEVENT) hAcceptEvent, 0);
	CloseHandle(hAcceptEvent);

	if (hSocketAccept != INVALID_SOCKET)
	{
		// �����׽��ֵ�����Ϊ��ַ�����ò���Ϊ��������
		if ( !Block(hSocketAccept, 0) || !SetSocketOption(hSocketAccept) )
		{
			closesocket(hSocketAccept);
			return (INVALID_SOCKET);
	
		}
	}
	return (hSocketAccept);
}


////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Bind(
//				SOCKET hSocket,
//				CONST struct sockaddr * pSocketAddr,
//				INT nAddrLen ) 
//	��  ;�����׽���
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : SOCKET
//		pSocketAddr : SOCKET��ַ
//		nAddrLen    : ��ַ����
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::Bind(SOCKET hSocket, CONST struct sockaddr * pSocketAddr, INT nAddrLen)
{
	if (bind(hSocket, pSocketAddr, nAddrLen) == SOCKET_ERROR)
	{
		SetLastError( WSAGetLastError() );
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL Block(
//				SOCKET hSocket,
//				BOOL bBlock ) 
//	��  ;�������׽����Ƿ�Ϊ������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket : �׽���
//		bBlock  : �Ƿ�����
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::Block(SOCKET hSocket, BOOL bBlock)
{
	u_long  IoctlLong = (bBlock) ? 0 : 1;

	if (ioctlsocket(hSocket, FIONBIO, &IoctlLong) == SOCKET_ERROR)
	{
		SetLastError( WSAGetLastError() );
		return FALSE;
    }

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//	��������INT RecvFrom(
//				SOCKET hSocket,
//				struct sockaddr * pFrom,
//				INT nFromlen,
//				CHAR *pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout ) 
//	��  ;���������ݱ�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : �׽���
//		pFrom       : ���ݱ���Դ��ַ
//		nFromlen    : ��ַ����
//		pszBuffer   : ���ݻ�����
//		nBufferSize : ��������С
//		dwTimeout   : ��ʱ
//	����ֵ��INT
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::RecvFrom(SOCKET hSocket, struct sockaddr * pFrom, INT nFromlen,CHAR *pszBuffer, INT nBufferSize, DWORD dwTimeout)
{
	HANDLE hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hReadEvent == NULL)
	{
		SetLastError((INT)GetLastError() );
		return (SOCKET_ERROR);
	}

	DWORD		dwRecvBytes = 0,
				dwFlags		= 0;
	WSABUF		WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = nBufferSize;
	WSABuff.buf = pszBuffer;

	for (;;)
	{
		// ע��FD_READ�¼�
		if( WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, FD_READ) == SOCKET_ERROR)
		{
			CloseHandle(hReadEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hReadEvent, TRUE,	dwTimeout, TRUE);

		if( dwWaitResult != WSA_WAIT_EVENT_0 )
		{
			// ע���¼�
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError());
			return (SOCKET_FAIL);
		}

		////////////////////////////////////////////////////////////// 
		///	ע�⣺��ʹ dwWaitResult == WSA_WAIT_EVENT0 ��ҲӦ�� 
		///			��һ����������Ƿ�������
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hReadEvent,&NetEvent) == SOCKET_ERROR)
		{
			// ע���¼�
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		if(NetEvent.iErrorCode[FD_READ_BIT] !=0 )	// ��������
		{
			// ע���¼�
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			SetLastError(NetEvent.iErrorCode[FD_READ_BIT]);
			return (SOCKET_FAIL);
		}
		////////////////////////////////////////////////////////////////
		// ע���¼�
		WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);

		INT nAddrLen = nFromlen;
		if ( WSARecvFrom(hSocket, &WSABuff, 1, &dwRecvBytes, &dwFlags,pFrom, &nAddrLen, NULL, NULL) == SOCKET_SUCCESS )
			break;

		if ( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CloseHandle(hReadEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}

		///////////////////////////////////////////////////////////////////////////
		//	˯��һ��ʱ��
		//////////////////////////////////////////////////////////////////////////
		Sleep(DEFAULT_BLOCKED_SNDRCV_SLEEP);
	}
	CloseHandle(hReadEvent);
	return ((INT) dwRecvBytes);

}


////////////////////////////////////////////////////////////////////////////////
//	��������INT SendTo(
//				SOCKET hSocket,
//				CONST struct sockaddr * pTo,
//				INT nToLen,
//				LPCSTR pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout ) 
//	��  ;���������ݱ�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		hSocket     : �׽���
//		pTo         : ���ݱ���Ŀ�ĵ�ַ
//		nToLen      : ��ַ����
//		pszBuffer   : ������
//		nBufferSize : ��������С
//		dwTimeout   : ��ʱ
//	����ֵ��INT
////////////////////////////////////////////////////////////////////////////////
INT CBufSocket::SendTo(SOCKET hSocket, CONST struct sockaddr * pTo,INT nToLen,LPCSTR pszBuffer, INT nBufferSize, DWORD dwTimeout)
{
	HANDLE hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL)
	{
		SetLastError( (INT)GetLastError() );
		return (SOCKET_FAIL);
	}

	DWORD	dwSendBytes = 0,
            dwFlags		= 0;
	WSABUF	WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = nBufferSize;
	WSABuff.buf = (CHAR *) pszBuffer;

	for (;;)
	{
		if (WSASendTo( hSocket, &WSABuff, 1, &dwSendBytes, dwFlags,pTo, nToLen, NULL, NULL) == SOCKET_SUCCESS)
			break;

		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_FAIL);
		}

		//////////////////////////////////////////////////////////////////////////
		//	˯��һ��ʱ��
		/////////////////////////////////////////////////////////////////////////
		Sleep(DEFAULT_BLOCKED_SNDRCV_SLEEP);

		// ע��FD_WRITE�¼�  
		if( WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, FD_WRITE) == SOCKET_ERROR)
		{
			CloseHandle(hWriteEvent);
			SetLastError( WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hWriteEvent, TRUE,dwTimeout, TRUE);
		
		if( dwWaitResult != WSA_WAIT_EVENT_0 )
		{
			// ע���¼�
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_FAIL);
		}

		////////////////////////////////////////////////////////////// 
		///	ע�⣺��ʹ dwWaitResult == WSA_WAIT_EVENT0 ��ҲӦ�� 
		///			��һ����������Ƿ�������
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hWriteEvent,&NetEvent) == SOCKET_ERROR)
		{
			// ע���¼�
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(  WSAGetLastError() );
			return (SOCKET_FAIL);
		}
		if(NetEvent.iErrorCode[FD_WRITE_BIT] !=0 )	// ��������
		{
			// ע���¼�
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			SetLastError(NetEvent.iErrorCode[FD_WRITE_BIT]);
			return (SOCKET_FAIL);
		}
		////////////////////////////////////////////////////////////////
		// ע���¼�
		WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
	}

	CloseHandle(hWriteEvent);
	return ((INT) dwSendBytes);
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL RecvFrom(
//				OUT LPSTR pszFrom,
//				OUT INT& nFromPort,
//				OUT CHAR *pszBuffer,
//				IN INT nBufferSize,
//				IN DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/ ) 
//	��  ;���������ݱ�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszFrom     : Դ��ַ
//		nFromPort   : Դ��ַ�˿�
//		pszBuffer   : ������
//		nBufferSize : ��������С
//		dwTimeout   : ��ʱ
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::RecvFrom(OUT LPSTR pszFrom,OUT INT& nFromPort,OUT CHAR *pszBuffer,IN INT nBufferSize,IN DWORD dwTimeout /*= DEFAULT_RECV_TIMEOUT*/)
{
	SOCKADDR_IN sockAddrFrom;
	ZeroMemory(&sockAddrFrom,sizeof(sockAddrFrom));

	// ���軺�����㹻��,�����ǻ���������������
	if( RecvFrom(m_hSocket, (SOCKADDR *)&sockAddrFrom, sizeof(sockAddrFrom),pszBuffer,nBufferSize,dwTimeout) <= 0 )
		return FALSE;

	// ����Դ��ַ
	LPSTR pFrom = inet_ntoa(sockAddrFrom.sin_addr);
	memcpy( pszFrom,pFrom,strlen(pFrom) );
	nFromPort = ntohs( sockAddrFrom.sin_port);
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL SendTo(
//				LPCSTR pszTo,
//				INT nToPort,
//				LPCSTR pszBuffer,
//				INT nBufferSize,
//				DWORD dwTimeout /*= DEFAULT_SEND_TIMEOUT*/ ) 
//	��  ;���������ݱ�
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		pszTo       : Ŀ�ĵ�ַ
//		nToPort     : Ŀ�Ķ˿�
//		pszBuffer   : ���ݻ�����
//		nBufferSize : ��������С
//		dwTimeout   : ��ʱ
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::SendTo(LPCSTR pszTo,INT nToPort,LPCSTR pszBuffer, INT nBufferSize, DWORD dwTimeout /*= DEFAULT_SEND_TIMEOUT*/)
{
	SOCKADDR_IN sockAddrTo;
	ZeroMemory(&sockAddrTo,sizeof(sockAddrTo));

    sockAddrTo.sin_family = AF_INET;
    sockAddrTo.sin_addr.s_addr = GetIP(pszTo,TRUE);
    sockAddrTo.sin_port = htons( (u_short) nToPort);
	
	if (sockAddrTo.sin_addr.s_addr == INADDR_NONE)
	{
		SetLastError( WSAGetLastError() );
		return FALSE;
	}

	if( SendTo(m_hSocket,(SOCKADDR *)&sockAddrTo,sizeof(sockAddrTo),pszBuffer,nBufferSize,dwTimeout) < nBufferSize )
		return FALSE;

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//	��������BOOL SetSockOpt(
//				INT nOptionName,
//				LPCVOID lpOptionValue,
//				INT nOptionLen,
//				INT nLevel /*= SOL_SOCKET*/ ) 
//	��  ;������SOCKET������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		nOptionName   : ��������
//		lpOptionValue : ����ֵ
//		nOptionLen    : ����ֵ�ĳ���
//		nLevel        : �׽������Լ���
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::SetSockOpt( INT nOptionName, LPCVOID lpOptionValue, INT nOptionLen, INT nLevel /*= SOL_SOCKET*/ )
{
	if (setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR ) lpOptionValue,nOptionLen) == SOCKET_ERROR )
    {
		SetLastError( WSAGetLastError() );
        return FALSE;
    }
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	��������BOOL GetSockOpt(
//				INT nOptionName,
//				LPVOID lpOptionValue,
//				LPINT lpOptionLen,
//				INT nLevel /*= SOL_SOCKET*/ ) 
//	��  ;����ȡSOCKET������
//	��ȫ�ֱ�����Ӱ�죺��
//	��  ����
//		nOptionName   : �׽�����������
//		lpOptionValue : �����׽�������ֵ
//		lpOptionLen   : ����
//		nLevel        : ����
//	����ֵ��BOOL
////////////////////////////////////////////////////////////////////////////////
BOOL CBufSocket::GetSockOpt( INT nOptionName, LPVOID lpOptionValue, LPINT lpOptionLen, INT nLevel /*= SOL_SOCKET*/ )
{
	if( getsockopt(m_hSocket, nLevel, nOptionName, (CHAR *)lpOptionValue, lpOptionLen) == SOCKET_ERROR )
	{
		SetLastError( WSAGetLastError() );
		return FALSE;
	}
	return TRUE;
}

