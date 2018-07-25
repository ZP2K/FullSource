///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName : KG_SO3GateWayTestDef
// Creator  : liuzhibiao
// Date     : 2008-12-16
// Comment  : v1.1
//			  V0.6  ���Ӳ��Ե����ݽṹ
//            V1.1  �ı����ݽṹ
///////////////////////////////////////////////////////////////

#ifndef _KG_SO3GATEWAYTESTDEF_H_
#define _KG_SO3GATEWAYTESTDEF_H_

#pragma pack(1)

#define IP_LENGTH                       16
#define REV_TIME_OUT                    -1
#define CLIENT_HAD_KICK                 100
#define PAYSYS_HAD_REV_USER_LOGINOUT    200
#define USER_ACCOUNT_LOCKED             300
#define CLIENT_REV_TIMEOUT              400
#define CLIENT_REV_LOGINGAME_STATE_INFO 500
#define CLIENT_REV_GIVEUP_QUEUE_RESPOND 600
#define TEXT_DESCRIPTION_LENGTH         128
#define MAX_RECV_LOOP_TIMES             100
#define GATEWAY_CONNECT_CONFIG_FILE     "gatewaytestconfig.ini"
#define CLIENT_GATEWAY_VERSION          15

enum KG_GATEWAY_TEST_POINT
{
    KG_CLIENT_HANDSHAKE,    // �ͻ�������
    KG_USER_LOGIN,          // �û���½
    KG_USER_MIBAO,          // �û��ܱ�
    KG_USER_RETRY,          // �û�����
    KG_ROLE_LIST,           // �û���ȡ��ɫ�б�
    KG_ROLE_ENTER_GAME,     // �û�ѡ���ɫ��½��Ϸ����
    KG_CREATE_ROLE,         // �û�������ɫ
    KG_DELETE_ROLE,         // �û�ɾ����ɫ
    KG_HOMETOWN_LIST,       // �û���ȡ���ִ�
    KG_GATEWAY_RECONNECT    // Gateway��������
};

struct KG_SIMULATE_PAYSYS_CONFIG
{
    char    szPaysysIPAddr[IP_LENGTH];      // KG_SocketAcceptor::Init����
    int     nPaysysPort;                    // ����paysys�ļ����˿�     

    int     nPaysysLostconnect;             // paysys���Ӷ�ʧ
    int     nGatewayLoginResult;            // ���ص�¼������
    //int     nGatewayLoginReconnectResult;   // ��������������

    int     nNotRespondProtocol;            // ������Ӧ��Э�飨ֻ���գ����ظ���
    int     nNotHandleProtocol;             // ���������Э�飨���պͻظ���������
    int     nNotRespondSubProtocol;         // ������Ӧ����Э�飨ֻ���գ����ظ���
    int     nUserLoginResult;               // �ʻ���½���
    WORD    wType;                          // ��½��ʽAccountUserLoginInfo/AccountUserPasspodVerifyRet
    BYTE    byMibaoMode;                    // ģʽ
    int     nMibaoResult;                   // �ܱ����ؽ��
};

struct KG_SIMULATE_GAMECENTER_CONFIG
{
    char  szRelayIPAddr[IP_LENGTH];       // ����GameCenter��ip
    int   nRelayPort;                     // ����GameCenter�ļ����˿� 

    int   nGameWorldProtocolLowerVersion;   // ��Ϸ����汾��Lower
    int   nGameWorldProtocolUpperVersion;   // ��Ϸ����汾��Upper
    int   nMapCount;                        // ���ִ���Ŀ   
    int   nGetRoleListCount;                // ��ɫ����
    int   nCreatRoleResult;                 // ������ɫ�ɹ���
    int   nDeleteRoleResult;                // ɾ����ɫ�ɹ���
    int   nUserOnlineResult;                // ����Ƿ����ߣ�
    int   nEnterGameResult;                 // ������Ϸ�Ƿ�ɹ���
    int   nNotRespondProtocol;              // ������Ӧ��Э��
    int   nAllowPlayerInGameWorld;          // ���������Ϸ���������
};

struct KG_SIMULATE_CLIENT_CONFIG
{
    char            szConnectIPAddr[IP_LENGTH];         // Client����Gateway��IP
    int             nConnectPort;                       // Client����Gateway��Port

    int             nClientGatewayVersion;              // �ͻ��˱����GateWay�汾
    int             nClientGameWorldVersion;            // �ͻ�����Ϸ����汾
    char            szClientAccountName[_NAME_LEN];     // ��½���û���
    KSG_PASSWORD    szClientAccountPassword;            // ����
    char            szClientActivateCode[_NAME_LEN];    // ������
    int             nDelRoleID;                         // ��Ҫɾ���Ľ�ɫid��
    int             nHadRevKick;                        // ��Ҫ�յ��ߺ���Ϣ
    int             nUserGiveUpQueue;                   // ����½��Ϸ������Ҫ�Ŷ�ʱ,�û�ѡ������Ŷ�
    int             nNeedTestTimeOut;                   // �Ƿ���Ҫ�����ճ�ʱ��־λ
    int             nNeedStopWhenRevAppointInfo;        // �Ƿ���Ҫ�յ�ָ����Ϣ��ʱ����ֹ����
    int             nNeedSendPing;
};

typedef struct KG_RESULT            //Ԥ�ƽ��
{
    int nTestPoint;                 //����
    int nResult;                    //�ظ������� 
    int nTypeResult;                //���� 

}KG_FORECAST_RESULT, KG_RETURN_RESULT;

struct KG_GATEWAY_TEST_MANAGER
{
    KG_SIMULATE_PAYSYS_CONFIG       PaysysManagerConfig;
    KG_SIMULATE_GAMECENTER_CONFIG   GameCenterConfig;
    KG_SIMULATE_CLIENT_CONFIG       ClientConfig;

    KG_FORECAST_RESULT              ForecastConfig;
    char                            m_szTestGWUserName[_NAME_LEN];
    char                            m_szGatewayPath[MAX_PATH];
    int                             nNeedSimulatePaysys;
    int                             nNeedSimulateRelay;
};

#pragma pack()

#endif//_KG_SO3GATEWAYTESTDEF_H_
