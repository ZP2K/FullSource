
// SocksPacket.h: interface for the CSocksPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKSPACKET_H__04B153A0_80D4_442C_ACCE_0FA5C8ECF577__INCLUDED_)
#define AFX_SOCKSPACKET_H__04B153A0_80D4_442C_ACCE_0FA5C8ECF577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "bufsocket.h" //��ҪCBufSocket��

//��������
#undef	IN
#define IN
#undef	OUT
#define OUT

static CONST BYTE	BYTE_NULL		= 0x00;			// ���ֽ�
static CONST WORD	WORD_NULL		= 0x0000;		// ����
static CONST DWORD	UNKNOWN_IP		= 0x10000000;	// �����IP��ַ,ָ��ҪSOCKS4A PROXY�������� 0.0.0.1
static CONST BYTE	AUTH_OK			= 0x00;			// Username/Passwd��֤OK
static CONST DWORD	SOP_SEND_TIMEOUT= 120*1000;		//WSA_INFINITE;	
static CONST DWORD	SOP_RECV_TIMEOUT= 120*1000;		//WSA_INFINITE;

// SOCKS������
CONST INT	SOCKS_TYPE_4	= 0x01;
CONST INT	SOCKS_TYPE_4A	= 0x02;
CONST INT	SOCKS_TYPE_5	= 0x03; 

// SOCKS�İ汾��
CONST BYTE	VER_SOCKS4		= 0x04;
CONST BYTE	VER_SOCKS4A		= 0x04;
CONST BYTE	VER_SOCKS5		= 0x05;
CONST BYTE	VER_SOCKSUNKNOWN= 0xFF;

//��֧�ֵ�����
CONST BYTE	CMD_CONNECT		= 0x01;
CONST BYTE	CMD_BIND		= 0x02;
CONST BYTE	CMD_UDP			= 0x03;	// (SOCKS5 ONLY)
CONST BYTE	CMD_UNKNOWN		= 0xFF;

/* SOCKS4(A)����(����ֵ) */
CONST BYTE	REP4_SUCCESS	= 90;
CONST BYTE	REP4_RESULT		= 90;
CONST BYTE	REP4_FAIL		= 91;
CONST BYTE	REP4_NOIDENTD	= 92;
CONST BYTE	REP4_BADID		= 93;

/* SOCKS5����(����ֵ) */
CONST BYTE	REP5_SUCCESS	= 0x00;
CONST BYTE	REP5_RESULT		= 0x00;
CONST BYTE	REP5_FAIL		= 0x01;
CONST BYTE	REP5_NOTALLOW	= 0x02;
CONST BYTE	REP5_NETUNREACH	= 0x03;
CONST BYTE	REP5_HOSTUNREACH= 0x04;
CONST BYTE	REP5_CONNREF	= 0x05;
CONST BYTE	REP5_TTLEXP		= 0x06;
CONST BYTE	REP5_BADCMD		= 0x07;
CONST BYTE	REP5_BADADDR	= 0x08;

// ��֤��ʽ(SOCKS5 ONLY)
CONST BYTE	AUTH_NONE		= 0x00;
CONST BYTE	AUTH_GSSAPI		= 0x01;
CONST BYTE	AUTH_PASSWD		= 0x02;
CONST BYTE	AUTH_CHAP		= 0x03;
CONST BYTE	AUTH_UNKNOWN	= 0xFF;

// ��ַ��ʽ(SOCKS5 ONLY)
CONST BYTE	ATYP_IPV4ADDR	= 0x01;
CONST BYTE	ATYP_HOSTNAME	= 0x03;
CONST BYTE	ATYP_IPV6ADDR	= 0x04;

// ��������
CONST INT	PACKET_SOCKS4REQ			= 0x01;
CONST INT	PACKET_SOCKS4AREQ			= 0x02;
CONST INT	PACKET_SOCKS5AUTHREQ		= 0x03;
CONST INT	PACKET_SOCKS5AUTHPASSWDREQ	= 0x04;
CONST INT	PACKET_SOCKS5REQ			= 0x05;

CONST INT	PACKET_SOCKS4REP			= 0x21;
CONST INT	PACKET_SOCKS4AREP			= 0x22;
CONST INT	PACKET_SOCKS5AUTHREP		= 0x23;
CONST INT	PACKET_SOCKS5AUTHPASSWDREP	= 0x24;
CONST INT	PACKET_SOCKS5REP			= 0x25;

CONST INT	PACKET_SOCKS5UDP			= 0x41;


#pragma pack(push, 1)

///////////////////////////////////////////////////////
//
//	SOCK4 Proxy
//
//////////////////////////////////////////////////////
typedef struct _tagSocks4Req
{
	BYTE	byVer;		// �汾�� 0x04
	BYTE	byCmd;		// ����
	WORD	wDestPort;	// Ŀ��˿�
	DWORD	dwDestIP;	// Ŀ��IP��ַ
	LPCTSTR	pszUserID;	// �û�ID
	BYTE	byNULL;		// 0x00
} SOCKS4REQ,*PSOCKS4REQ;

typedef struct _tagSock4Rep
{
	BYTE	byVer;
	BYTE	byRep;		// 0x90-0x93
	WORD	wDestPort;
	DWORD	dwDestIP;
} SOCK4REP,SOCKS4AREP,*PSOCK4REP,*PSOCK4AREP;

///////////////////////////////////////////////////////
//
//	SOCK4A Proxy
//
//////////////////////////////////////////////////////

typedef struct _tagSocks4AReq
{
	BYTE	byVer;			// �汾�� 0x04
	BYTE	byCmd;			// ����
	WORD	wDestPort;		// Ŀ��˿�
	DWORD	dwDestIP;		// Ŀ�����IP��ַ 0x000000XX
	LPCTSTR	pszUserID;		// �û�ID
	BYTE	byNULL1;		// �ָ���
	LPCTSTR	pszDestHostName;// Ŀ���������
	BYTE	byNULL2;		// ����0x00
} SOCKS4AREQ,*PSOCKS4AREQ;

///////////////////////////////////////////////////////
//
//	SOCK5 Proxy
//
//////////////////////////////////////////////////////
// Auth-Request
typedef struct _tagSocks5AuthReq
{
	BYTE	byVer;
	BYTE	byNAuth;
	LPCTSTR	pszAuth;
} SOCKS5AUTHREQ,*PSOCKS5AUTHREQ;

// Auth-Reply
typedef struct _tagSocks5AuthRep
{
	BYTE	byVer;
	BYTE	byAuth;
} SOCKS5AUTHREP,*PSOCKS5AUTHREP;

// USERNAME/PASSWORD��֤ - Request
typedef struct _tagSocks5AuthPasswdReq
{
	BYTE	byVer;
	BYTE	byUserNameLen;
	LPCTSTR	pszUserName;
	BYTE	byPasswdLen;
	LPCTSTR	pszPasswd;
} SOCKS5AUTHPASSWDREQ,*PSOCKS5AUTHPASSWDREQ;

// USERNAME/PASSWORD��֤ - Reply
typedef struct _tagSocks5AuthPasswdRep
{
	BYTE	byVer;
	BYTE	byStatus;
} SOCKS5AUTHPASSWDREP,*PSOCKS5AUTHPASSWDREP;
 
// Request
typedef struct _tagSocks5Req
{
	BYTE	byVer;
	BYTE	byCmd;
	BYTE	byRsv;
	BYTE	byAtyp;
	LPCTSTR	pszDestAddr;
	WORD	wDestPort;
} SOCKS5REQ,*PSOCKS5REQ;

// Reply
typedef struct _tagSocks5Rep
{
	BYTE	byVer;
	BYTE	byRep;
	BYTE	byRsv;
	BYTE	byAtyp;
	LPSTR	pszBindAddr;
	WORD	wBindPort;
} SOCKS5REP,*PSOCKS5REP;


// UDP (Socks5 ONLY)
typedef struct _tagSocks5UDP
{
	WORD	wRsv;
	BYTE	byFrag;
	BYTE	byAtyp;
	LPSTR	pszDestAddr;
	WORD	wDestPort;
	LPSTR	pszData;
} SOCKS5UDP,*PSOCKS5UDP;

// ��������
// Request-Packet (TCP ONLY)
typedef union _tagSocksReqPacket
{
	SOCKS4REQ				socks4Req;
	SOCKS4AREQ				socks4aReq;
	SOCKS5AUTHREQ			socks5AuthReq;
	SOCKS5AUTHPASSWDREQ		socks5AuthPasswdReq;
	SOCKS5REQ				socks5Req;
} SOCKSREQPACKET,*PSOCKSREQPACKET;

// Reply-Packet (TCP ONLY)
typedef union _tagSocksRepPacket
{
	SOCK4REP				socks4Rep;
	SOCKS4AREP				socks4aRep;
	SOCKS5AUTHREP			socks5AuthRep;
	SOCKS5AUTHPASSWDREP		socks5AuthPasswdRep;
	SOCKS5REP				socks5Rep;
} SOCKSREPPACKET,*PSOCKSREPPACKET;

// Request-Reply UDP Packet (UDP ONLY)
typedef union _tagSocksUDPPacket
{
	SOCKS5UDP				socks5UDP;
} SOCKSUDPPACKET,*PSOCKSUDPPACKET;

#pragma pack(pop)


class CSocksPacket  
{
public:
	CSocksPacket( CBufSocket* pBufSocket );
	virtual ~CSocksPacket();

public:
	BOOL	SendSocks4Req(BYTE byCmd,WORD wDestPort,DWORD dwDestIP,LPCSTR pszUserID  = NULL ,DWORD dwTimeout  = SOP_SEND_TIMEOUT );
	BOOL	SendSocks4aReq(BYTE byCmd,WORD wDestPort,LPCSTR pszDestHostName,LPCSTR pszUserID  = NULL ,DWORD dwTimeout  = SOP_SEND_TIMEOUT );
	BOOL	SendSocks5AuthReq(BYTE byNAuth,LPCSTR pszAuth,DWORD dwTimeout  = SOP_SEND_TIMEOUT );
	BOOL	SendSocks5AuthPasswdReq(LPCSTR pszUserName,LPCSTR pszPassword,DWORD dwTimeout  = SOP_SEND_TIMEOUT );
	BOOL	SendSocks5Req(BYTE byCmd,BYTE byAtyp,LPCSTR pszDestAddr,WORD wDestPort,DWORD dwTimeout  = SOP_SEND_TIMEOUT );
	BOOL	RecvPacket(OUT PSOCKSREPPACKET pPacket,INT nPacketType,DWORD dwTimeout = SOP_RECV_TIMEOUT);
	BOOL	IsSocksOK( PSOCKSREPPACKET pPacket,INT nPacketType );
	BOOL	SendPacket(PSOCKSREQPACKET pPacket,INT nPacketType,DWORD dwTimeout = SOP_SEND_TIMEOUT);

	// ADD IN 1.1 <
	BOOL	RecvPacket(INT nPacketType,DWORD dwTimeout = SOP_RECV_TIMEOUT);
	BOOL	IsSocksOK();
	// ADD IN 1.1 >

private:
	BOOL	Pack(OUT CHAR *pszDest,PSOCKSREQPACKET pPacket,INT nPacketType);
	INT		PacketSize(PSOCKSREQPACKET pPacket,INT nPacketType);

private:
	CBufSocket*		m_pBufSocket;

	// ADD IN 1.1 <
	SOCKSREPPACKET	m_stRepPacket;
	INT				m_nRepPacketType;
	// ADD IN 1.1 >
};

#endif // !defined(AFX_SOCKSPACKET_H__04B153A0_80D4_442C_ACCE_0FA5C8ECF577__INCLUDED_)
