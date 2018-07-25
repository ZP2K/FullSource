/************************************************************************/
/* Gateway client   					                                */
/* Copyright : Kingsoft 2008										    */
/* Author	 : Zhao chunfeng, Yan rui						            */
/* History	 :															*/
/*		2008.07.22	Create												*/
/************************************************************************/
#ifndef _KGATEWAYCLIENT_H_
#define _KGATEWAYCLIENT_H_

#ifdef _CLIENT

// ��ϵ��ö������BishopClient����½��������¼�֪ͨUIģ��
typedef enum
{
    LPUM_UNABLE_TO_CONNECT_SERVER,              // "�޷����ӷ�����"
    LPUM_MISS_CONNECTION,                       // "���������Ӷ�ʧ"
    LPUM_SYSTEM_MAINTENANCE,                    // "ϵͳά��"
    LPUM_UNMATCHED_PROTOCOL_VERSION,            // "Э��汾��ƥ�䣬�����"

    LPUM_HANDSHAKE_ACCOUNT_SYSTEM_LOST,         // "�ʺ�ϵͳ��ά��"

    LPUM_LOGIN_VERIFY_ALREADY_IN_GATEWAY,       // "���˺�����ʹ��: ���ڱ����������Bishop����֤��!"
    LPUM_LOGIN_VERIFY_IN_GATEWAY_BLACK_LIST,    // "�˺���Ϊʹ�÷Ƿ��ͻ��˶��������������"
    LPUM_LOGIN_VERIFY_SUCCESS,                  // "��֤ͨ��,һ��˳��"
    LPUM_LOGIN_VERIFY_IN_GAME,                  // "�˺�������Ϸ�У��Ժ�����(�Զ��ߺ�)"
    LPUM_LOGIN_VERIFY_ACC_PSW_ERROR,            // "�˺Ż����������"
    LPUM_LOGIN_VERIFY_NO_MONEY,                 // "ûǮ��"
    LPUM_LOGIN_VERIFY_NOT_ACTIVE,               // "�˺�û�м���"
    LPUM_LOGIN_VERIFY_ACTIVATE_CODE_ERR,        // "��������󣬲����ڻ��Ѿ���ʹ�ù���"
    LPUM_LOGIN_VERIFY_IN_OTHER_GROUP,           // "���˺��Ѿ�������������¼"
    LPUM_LOGIN_VERIFY_ACC_FREEZED,              // "�˺ű�������"
    LPUM_LOGIN_VERIFY_PAYSYS_BLACK_LIST,        // "����������,�˺ű�Paysys������������"
    LPUM_LOGIN_VERIFY_LIMIT_ACCOUNT,            // "�ó����û������ܵ���"
    LPUM_LOGIN_VERIFY_UNKNOWN_ERROR,            // "δ֪����"

    LPUM_GET_ROLE_LIST_SUCCESS,                 // ��ý�ɫ�б�OK
    LPUM_GET_ALL_ROLE_LIST_SUCCESS,             // ���ȫ����ɫ�б����
    LPUM_UPDATE_HOMETOWN_LIST,                  // �������ִ��б�����

    LPUM_CREATE_ROLE_SUCCESS,                   // "������ɫOK"
    LPUM_CREATE_ROLE_NAME_EXIST,                // "����ʧ��,��ɫ���Ѵ���"
    LPUM_CREATE_ROLE_INVALID_NAME,              // "����ʧ��,��ɫ���Ƿ�"
    LPUM_CREATE_ROLE_UNABLE_TO_CREATE,          // "����ʧ��,�޷�������ɫ"

    LPUM_REQUEST_LOGIN_GAME_SUCCESS,            // �Ѿ�ȡ����Ϸ�����½��Ϣ���������ӷ�����
    LPUM_REQUEST_LOGIN_GAME_OVERLOAD,           // "��Ϸ������������,�Ժ�����"
    LPUM_REQUEST_LOGIN_GAME_MAINTENANCE,        // "����������ά��"
    LPUM_REQUEST_LOGIN_GAME_ROLEFREEZE,         // "�ý�ɫ�Ѷ���"
    LPUM_REQUEST_LOGIN_GAME_UNKNOWN_ERROR,      //  δ֪����

    LPUM_DELETE_ROLE_SUCCESS,                   // ɾ��OK������½�ɫ�б���ʾ
    LPUM_DELETE_ROLE_BAD_PASSWORD,              // "�������벻��"
    LPUM_DELETE_ROLE_NOT_ALLOWED,               // "�ý�ɫ������ɾ��(����)"
    LPUM_DELETE_ROLE_UNKNOWN_ERROR,             // ������ʲôԭ�򣬷���ʧ����:)

    LPUM_GIVEUP_QUEUE_SUCCESS,                  // �ɹ������Ŷ�
    LPUM_GIVEUP_QUEUE_ERROR,                    // �����Ŷӳ���

} LOGIN_PROCESS_UI_MESSAGE;

struct UI_ROLE_LIST_ITEM
{
    char    szAccountName[_NAME_LEN];
    char    szRoleName[_NAME_LEN];
    DWORD   dwRoleID;
    int     nRoleType;
    int     nLevel;
    DWORD   dwMapID;
    int	    nMapCopyIndex;
	time_t	nLastSaveTime;
    time_t  nTotalGameTime;
    time_t  nFreezeTime;
    WORD    wRepresentID[perRepresentCount];
};

struct UI_ROLE_CREATE_PARAM
{
    char    szRoleName[_NAME_LEN];
    int     nRoleType; 
    DWORD   dwMapID;
    int     nMapCopyIndex;
    WORD	wRepresentId[perRepresentCount];
};

struct KUI_HOMETOWN_COPY_INFO 
{
    DWORD dwCopyIndex;
    int   nLoadFactor;
};

typedef std::vector<KUI_HOMETOWN_COPY_INFO> KUI_HOMETOWN_LOAD_REFACT_TABLE;

struct KUI_HOMETOWN_MAP_INFO
{
    DWORD dwMapID;
    KUI_HOMETOWN_LOAD_REFACT_TABLE LoadFactorTable;
};

typedef std::vector<KUI_HOMETOWN_MAP_INFO> KUI_HOMETOWN_MAP_TABLE;

struct KROLE_LIST_INFO;

class KGatewayClient
{
public:
	KGatewayClient(void);
	~KGatewayClient(void);

    void    Activate();
    void    ProcessConnectGateway();
    void    ProcessPackage();
    
    int     SetGatewayAddress(const char cszLoginIP[], unsigned short usLoginPort);
    int     BeginLogin(const char cszAccountName[], const char cszPasswordMD5[], const char cszActiveCode[]);
    int     GetRoleListItem(UI_ROLE_LIST_ITEM *pRoleListItem, int nRoleIndex); // nRoleIndex ��0��ʼ
    int     QueryHometownList();
    int     RoleLogin(const char cszRoleName[]);
    int     DeleteRole(const char cszRoleName[], const char cszSecPasswordMD5[]);    
    int     CreateRole(const UI_ROLE_CREATE_PARAM &crRoleCreateParam);
    int     GetRoleCount();
    DWORD   GetLeftTime();
    time_t  GetLastLoginTime();
    DWORD   GetLastLoginIP();
    int     ReLogin();
    void    Reset();

    const KUI_HOMETOWN_MAP_TABLE& GetHometownList() { return m_HometownList; };
    char* GetAccountName() { return m_szAccountName; }

private:
    char                    m_szLoginIP[32];
    unsigned short          m_usLoginPort;
    IKG_SocketStream*       m_piSocketStream;

    char                    m_szAccountName[_NAME_LEN];
    KSG_PASSWORD            m_Password;    
    char                    m_szActivateCode[_NAME_LEN];

    time_t                  m_nLastPingTime;

    DWORD                   m_dwLeftTime;
    time_t                  m_nLastLoginTime;
    DWORD                   m_dwLastLoginIP;

	typedef std::vector<UI_ROLE_LIST_ITEM>  ROLE_LIST;
    ROLE_LIST               m_RoleList;

    KUI_HOMETOWN_MAP_TABLE  m_HometownList;
    
    BOOL    AddRoleToRoleList(KROLE_LIST_INFO* pRoleListInfo);
    DWORD   GetRoleID(const char* cszRoleName);
    BOOL    ConnectGateway();

    BOOL    Send(IKG_Buffer* piSendBuffer);

public:
    BOOL DoPingSignal();
    BOOL DoHandshakeRequest();
    BOOL DoAccountVerifyRequest();
    BOOL DoCreateRoleRequest(const UI_ROLE_CREATE_PARAM &crRoleCreateParam);
    BOOL DoDeleteRoleRequest(DWORD dwRoleID);
    BOOL DoLoginGameRequest(DWORD dwRoleID);
    BOOL DoQueryHometownListRequest();
    BOOL DoGiveupQueueRequest();

private:
    typedef void (KGatewayClient::*PROTOCOL_FUNCTION)(BYTE* pbyData, size_t uDataLen);

    PROTOCOL_FUNCTION m_ProtocolFunctions[UCHAR_MAX + 1];
    size_t            m_uPakSize[UCHAR_MAX + 1];
    
    enum CONNECT_STATE
    {
        csInvalid,
        csWaitingConnectOK
    };
    CONNECT_STATE m_ConnectState;

    void OnPingRespond(BYTE* pbyData, size_t uDataLen);
    void OnHandshakeRespond(BYTE* pbyData, size_t uDataLen);
    void OnAccountLockedNotify(BYTE* pbyData, size_t uDataLen);
    void OnAccountVerifyRespond(BYTE* pbyData, size_t uDataLen);
    void OnKickAccountNotify(BYTE* pbyData, size_t uDataLen);
    void OnSyncRoleList(BYTE* pbyData, size_t uDataLen);
    void OnSyncNewbieMaps(BYTE* pbyData, size_t uDataLen);
    void OnCreateRoleRespond(BYTE* pbyData, size_t uDataLen);
    void OnDeleteRoleRespond(BYTE* pbyData, size_t uDataLen);
    void OnGameLoginPrepare(BYTE* pbyData, size_t uDataLen);
    void OnSyncLoginKey(BYTE* pbyData, size_t uDataLen);
    void OnSyncQueueState(BYTE* pbyData, size_t uDataLen);
    void OnGiveupQueueRespond(BYTE* pbyData, size_t uDataLen);
};


extern KGatewayClient g_GatewayClient;

#endif  // _CLIENT

#endif  // _KGATEWAYCLIENT_H_
