/************************************************************************/
/* �ͻ������ӻ���														*/
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.11.19	Create												*/
/************************************************************************/
#ifndef _KCLIENT_H_
#define _KCLIENT_H_

#include "Common/KG_Socket.h"
#include "SO3ProtocolBasic.h"

class KClient
{
public:
	KClient(void);
	~KClient(void);

	BOOL Init(const char* pszIPAddr, int nPort);
	void Breathe(void);
protected:
	typedef void (KClient::*PROCESS_PROTOCOL_FUNC)(IKG_Buffer* pBuffer);
	PROCESS_PROTOCOL_FUNC	m_ProcessProtocolFuns[MAX_PROTOCOL_FAMILY_NUM * MAX_PROTOCOL_ID_NUM];

	//��������˷������ݰ�
	BOOL Send(IKG_Buffer* pBuffer);

    BOOL Send(void* pvData, size_t uSize);

	BOOL Disconnect();

	//�����¼���Ӧ
	virtual void OnConnected();
	virtual void OnDisconnect();

private:
	IKG_SocketStream*	m_piSocketStream;
	char				m_szIPAddr[16];
	int					m_nPort;
};

#endif	//_KCLIENT_H_