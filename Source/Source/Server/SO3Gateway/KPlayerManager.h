#ifndef _KPLAYERMANAGER_H_ 
#define _KPLAYERMANAGER_H_ 

#include <map>
#include <set>
#include "GatewayClientProtocol.h"
#include "SO3GlobalDef.h"

enum KAccountState
{
    easInvalid, 
    easWaitForHandshake,                // �ȴ��ͻ��˴����汾�ŵ�
    easWaitForVerifyInfo,               // �ȴ��ͻ��˴����û���,����,����ʢ��Token����Ϣ
    easQueryAccountState,               // ������Relay��ѯ�ʺ�״̬
    easWaitForSDOAVerify,               // �ȴ�SDOA��֤
    easWaitForPaysysVerify,             // �ȴ�Paysys��֤
    easWaitForMibaoPassword,            // �ȴ��ͻ��˴����ܱ�����
    easWaitForMibaoVerify,              // �ȴ�Paysys�ܱ���֤
    easWaitForRoleList,                 // �ȴ���ɫ�б�
    easWaitForPlayerOperation,          // �ȴ���ҷ���������ɫ/ɾ����ɫ/��½��ɫ������
    easWaitForNewbieMapRespond,         // �ȴ����ִ���Ӧ
    easWaitForCreateRoleRespond,        // �ȴ�������ɫ����Ӧ
    easWaitForDeleteRoleRespond,        // �ȴ�ɾ����ɫ����Ӧ
    easWaitForLoginPermission,          // �ȴ���½��Կ
    easQueueWaiting,                    // ��½�Ŷ�״̬
    easWaitForRenameRespond,            // �ȴ�����ȷ��
    easWaitForDisconnect,               // �ȴ��ͻ�����������
    easTotal
};

typedef std::map<DWORD, IKG_Buffer*> KROLE_LIST;

struct KPlayerAgency
{
    char                          szAccount[_NAME_LEN];
    char                          szPsw[LOGIN_USER_PASSWORD_MAX_LEN];
    char                          szAct[LOGIN_USER_PASSWORD_MAX_LEN];
    char                          szSndaToken[SNDA_TOKEN_LEN];
    char                          szIDCard[ID_CARD_LEN];

    BYTE                          byMibaoMode;
    char                          szMatrixPosition[9];

    KAccountState                 nState;
    int                           nIndex;
    DWORD                         dwQueueID;
    IKG_SocketStream*             piSocket;
    time_t                        nLastPingTime;
    BOOL                          bAccountInGame;
    int                           nChargeFlag;
    tagExtPointInfo               ExtPointInfo;
    DWORD                         dwEndOfDayTime;             // �¿���ֹʱ��
    DWORD 	                      dwLeftTimeOfPoint;		  // �Ƶ�ʣ�����
    DWORD                         dwLeftTimeOfDay;            // �쿨ʣ������
    DWORD                         dwEndTimeOfFee;             // ����շ��ܽ�ֹʱ��
    DWORD                         dwCoin;                     // �����
    DWORD                         dwLimitPlayTimeFlag;        // �����Ա�־λ
    DWORD                         dwLimitOnlineSecond;        // �������ۼ�����ʱ��
    DWORD                         dwLimitOfflineSecond;       // �������ۼ�����ʱ��

    DWORD                         dwLoginTime;                // ���ε�¼ʱ��
    DWORD                         dwLastLoginTime;
    DWORD                         dwLastLoginIP;

    KROLE_LIST                    RoleList;

    DWORD                         dwVerifyPostTime;           // ��PaysysͶ����֤�����ʱ��,���ڲ�����֤Ч��
};

class KPaysysAgency;
class KRelayAgency;
class KQueueManager;
class KSO3Gateway;

class KPlayerManager
{
public:
    KPlayerManager();

    BOOL Init();
    void UnInit();

    void Activate();

    void AttachModule(KPaysysAgency* pModule) { m_pPaysysAgency = pModule; }
    void AttachModule(KRelayAgency* pModule) { m_pRelayAgency = pModule; }
    void AttachModule(KQueueManager* pModule) { m_pQueueManager = pModule; }
    void AttachModule(KSO3Gateway* pModule) { m_pGateway = pModule; }

    KPlayerAgency* GetPlayer(int nIndex)
    {
        KPlayerTable::iterator it = m_PlayerTable.find(nIndex);
        if (it != m_PlayerTable.end())
        {
            return &it->second;
        }
        return NULL;
    }

    int  GetConnectionCount() { return (int)m_PlayerTable.size(); }

    void UnlockAccount(KPlayerAgency* pPlayer);

private:
    BOOL ProcessNewConnection(IKG_SocketStream* piSocket);
    BOOL ProcessPackage(IKG_SocketStream* piSocket);

    void CheckConnectionTimeout();
    void RecycleConnections();

    void OnDisconnect(KPlayerAgency* pPlayer);

public:
    BOOL DoPingRespond(KPlayerAgency* pPlayer, DWORD dwTime);
    BOOL DoHandshakeRespond(KPlayerAgency* pPlayer, int nCode);
    BOOL DoAccountLockedNotify(KPlayerAgency* pPlayer);
    BOOL DoAccountVerifyRespond(KPlayerAgency* pPlayer, int nRespondCode);
    BOOL DoMibaoVerifyRespond(KPlayerAgency* pPlayer, int nRespondCode);
    BOOL DoKickAccountNotify(KPlayerAgency* pPlayer);
    BOOL DoSyncRoleList(KPlayerAgency* pPlayer, int nRoleIndex, int nRoleCount, BYTE* pbyRoleData, size_t uRoleDataLen);
    BOOL DoSyncNewbieMaps(KPlayerAgency* pPlayer, DWORD dwMapID, KNEWBIE_MAP_COPY_INFO CopyInfo[], int nCopyCount);
    BOOL DoCreateRoleRespond(KPlayerAgency* pPlayer, BYTE byCode, DWORD dwRoleID, BYTE* pbyData, size_t uDataLen);
    BOOL DoDeleteRoleRespond(KPlayerAgency* pPlayer, BYTE byCode, DWORD dwRoleID, time_t nDeleteTime);
    BOOL DoGameLoginRespond(KPlayerAgency* pPlayer, BYTE byCode, DWORD dwRoleID, DWORD dwIP, int nPort, const GUID& crGuid);
    BOOL DoSyncQueueState(KPlayerAgency* pPlayer, int nPosition);
    BOOL DoGiveupQueueRespond(KPlayerAgency* pPlayer);
    BOOL DoRenameRespond(KPlayerAgency* pPlayer, BYTE byCode, DWORD dwRoleID, BYTE* pbyData, size_t uDataLen);
    BOOL DoSndaVerifyRespond(KPlayerAgency* pPlayer, int nRespondCode);

private:
    void OnPingSignal(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnHandshakeRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnAccountVerifyRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnSndaTokenVerifyRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnMibaoVerifyRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnQueryNewbieMaps(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnCreateRoleRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnDeleteRoleRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnGameLoginRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnGiveupQueueRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);
    void OnRenameRequest(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);

private:
    typedef void (KPlayerManager::*PC2G_PROTOCOL_FUNC)(KPlayerAgency* pPlayer, BYTE* pbyData, size_t uDataLen);

    PC2G_PROTOCOL_FUNC m_PakProcessor[c2g_protocol_total];
    size_t             m_uPakSize[c2g_protocol_total];
   
private:
    typedef std::map<int, KPlayerAgency> KPlayerTable;
    KPlayerTable                m_PlayerTable;

	struct KAccountCompareLess
	{
		bool operator()(const string& crX, const string& crY) const
		{
			return STR_CASE_CMP(crX.c_str(), crY.c_str()) < 0;
		}
	};

    typedef std::set<string, KAccountCompareLess> KLockedAccountTable;
    KLockedAccountTable         m_LockedAccountTable;

    int                         m_nPingCycle;

    KPaysysAgency*              m_pPaysysAgency;
    KRelayAgency*               m_pRelayAgency;
    KQueueManager*              m_pQueueManager;
    KSO3Gateway*                m_pGateway;

private:
    KG_SocketServerAcceptor*    m_pSockerServer;
    KG_SOCKET_EVENT*            m_pSocketEvents;
    int                         m_nNextPlayerIndex;
    int                         m_nMaxPlayer;
    BOOL                        m_bRecycleConnections;
    time_t                      m_nTimeNow;
    time_t                      m_nNextPingCheck;
};

#endif
