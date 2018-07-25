///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName :   SO3GatewayTestUserLogin.cpp
// Creator  :   liuzhibiao
// Date     :   2008-9-22
// Comment  :   v0.1
//              v0.1 2008-9-22 create
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SO3GatewayTest.h"
#include "KClientController.h"
#include "KSimulateRelay.h"
#include "KSimulatePaysys.h"

void KGatewaytest::FillTestUserLoginInfo(
    int nTestPoint,
    int nPaysysUserLoginResult, 
    WORD wType,
    BYTE byMibaoMode,
    int nMibaoVerifyResult,
    int nRelayUserOnlineResult, 
    int nClientHadRevKick,
    int nResult,
    int nNeedStopWhenRevAppointInfo
)
{
    m_GatewayTestConfig.PaysysManagerConfig.nGatewayLoginResult         = ACTION_SUCCESS;
    m_GatewayTestConfig.PaysysManagerConfig.nPaysysLostconnect          = false;   
    m_GatewayTestConfig.GameCenterConfig.nGameWorldProtocolLowerVersion = 0;
    m_GatewayTestConfig.GameCenterConfig.nGameWorldProtocolUpperVersion = 100;
    m_GatewayTestConfig.ClientConfig.nClientGatewayVersion              = CLIENT_GATEWAY_VERSION;
    m_GatewayTestConfig.ClientConfig.nClientGameWorldVersion            = 20;
    m_GatewayTestConfig.GameCenterConfig.nNotRespondProtocol            = 0;
    m_GatewayTestConfig.PaysysManagerConfig.nNotRespondProtocol         = 0;
    m_GatewayTestConfig.PaysysManagerConfig.nNotHandleProtocol          = 0;
    m_GatewayTestConfig.PaysysManagerConfig.nMibaoResult                = nMibaoVerifyResult;
    m_GatewayTestConfig.ClientConfig.nNeedTestTimeOut                   = false;
    m_GatewayTestConfig.ClientConfig.nNeedStopWhenRevAppointInfo        = nNeedStopWhenRevAppointInfo;
    m_GatewayTestConfig.ClientConfig.nNeedSendPing                      = true;

    m_GatewayTestConfig.PaysysManagerConfig.nUserLoginResult = nPaysysUserLoginResult;
    m_GatewayTestConfig.GameCenterConfig.nUserOnlineResult   = nRelayUserOnlineResult;
    m_GatewayTestConfig.ClientConfig.nHadRevKick             = nClientHadRevKick;

    m_GatewayTestConfig.PaysysManagerConfig.wType            = wType;//��ʱ
    m_GatewayTestConfig.PaysysManagerConfig.byMibaoMode      = byMibaoMode;//��ʱ

    strncpy(
        m_GatewayTestConfig.ClientConfig.szClientAccountName,
        "Jx3AccountName", 
        sizeof(m_GatewayTestConfig.ClientConfig.szClientAccountName)
    );
    strncpy(
        m_GatewayTestConfig.ClientConfig.szClientActivateCode,
        "Jx3ActivateCode",
        sizeof(m_GatewayTestConfig.ClientConfig.szClientActivateCode)
    );
    strncpy(
        m_GatewayTestConfig.ClientConfig.szClientAccountPassword.szPassword,
        "Jx3AccountPassword",
        sizeof(m_GatewayTestConfig.ClientConfig.szClientAccountPassword.szPassword)
    );

    //[Forecast]
    m_GatewayTestConfig.ForecastConfig.nTestPoint = nTestPoint;
    m_GatewayTestConfig.ForecastConfig.nResult = nResult;
    m_GatewayTestConfig.ForecastConfig.nTypeResult = 0;    

    //[State]
    m_ReturnResult.nTestPoint = m_GatewayTestConfig.ForecastConfig.nTestPoint;
    m_ReturnResult.nResult = 0;
    m_ReturnResult.nTypeResult = 0;
}

void KGatewaytest::TestUserLoginPaysysRefuseRevPWOrAKError()   // caseID:5001
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:5001,����Paysys���ص�½���ɹ�ʱ��Client����յ��˺����벻ƥ��";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:5001\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, E_ACCOUNT_OR_PASSWORD, AccountUserLoginInfo,  PASSPODMODE_UNBIND, ACTION_SUCCESS, false, false, E_ACCOUNT_OR_PASSWORD, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginGatewayLockTheAccount() // caseID:5126
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:5126,�˺ŵ�½,Paysys��֤ͨ��,�����ʻ�������ĵط����ڵ�½,Client����յ��˺�������";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:5126\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_UNBIND, ACTION_SUCCESS, false, true, USER_ACCOUNT_LOCKED, false);

    nRetCode = InitTestEnvironment(true, true, true, 2, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->ModifyClientUserName(1,"0");
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->GetReturnResult(1, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginButGCNotRespondUserState()  // caseID:6655
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6655,����Paysys������½,��GameCenter������״̬��Ϣʱ,Client��Ҷ���֮���ص�,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6655\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, false, true, USER_ACCOUNT_LOCKED, false);
    m_GatewayTestConfig.GameCenterConfig.nNotRespondProtocol      = g2r_query_account_state;

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        7, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onDisConnect,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->CheckTheAccountLockedOrNot(0, &nAccountLockedState);
    KGLOG_PROCESS_ERROR(!nAccountLockedState);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginGCRevKickAccountRequest()   // caseID:6656
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6656,����Paysys������½ʱ��GameCenter����Client�������ʱ,GameCenter�յ��ߺ���Ϣ";
    int nGCHadRevKickInfo = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6656\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, true, true, USER_ACCOUNT_LOCKED, false);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nGCHadRevKickInfo = m_pRelay->HadRevKickInfo();
    KGLOG_PROCESS_ERROR(nGCHadRevKickInfo);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginClientRevRetryLoginInfo()   // caseID:5003
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:5003, ����Paysys������½ʱ��GameCenter����Client�������ʱ,Client����յ���������Ϣ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:5003\n");

    FillTestUserLoginInfo(KG_USER_RETRY, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, true, true, CLIENT_HAD_KICK, false);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginCGRetUserOnlineUserReLogin()  // caseID:6664
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6664, ����Paysys������½ʱ��GameCenter����Client�������ʱ,Client��Ҷ˶���֮���ص�,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6664\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, false, true, USER_ACCOUNT_LOCKED, false);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        7, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onDisConnect,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->CheckTheAccountLockedOrNot(0, &nAccountLockedState);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(!nAccountLockedState);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysRefuseUserReLogin()  // caseID:6665
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6665, ����Paysys���ص�½���ɹ�ʱ��Client��Ҷ��ߺ��ص�,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6665\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, E_ACCOUNT_OR_PASSWORD, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, false, true, USER_ACCOUNT_LOCKED, false);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        7, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onDisConnect,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->CheckTheAccountLockedOrNot(0, &nAccountLockedState);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(!nAccountLockedState);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNotRespondUserReLogin()  // caseID:6659
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6659,����Paysys���������û���֤���ʱ,Client��Ҷ���,�ص�,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6659\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, E_ACCOUNT_OR_PASSWORD, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, false, true, USER_ACCOUNT_LOCKED, false);
    m_GatewayTestConfig.PaysysManagerConfig.nNotRespondProtocol = 62; // B2P_IB_PLAYER_IDENTITY_VERIFY = 62

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        7, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onDisConnect,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->CheckTheAccountLockedOrNot(0, &nAccountLockedState);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(!nAccountLockedState);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNotRespondAndUserNoOnLine()  // caseID:5125
{
    int         nResult             = false;
    int         nRetCode            = false;
    char        szTestCaseDscribe[] = "// caseID:5125,����Paysys������½����������֤���,GameCenter�����û�������ʱ,Client��Ҷ���,Paysys�յ��ǳ���Ϣ";
    int         HadRevUserLoginOut  = false;
    unsigned    nLoopTimes          = 100;

    KGLogPrintf(KGLOG_INFO, "caseID:5125\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, E_ACCOUNT_OR_PASSWORD, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, false, true, USER_ACCOUNT_LOCKED, true);
    m_GatewayTestConfig.PaysysManagerConfig.nNotRespondProtocol = 62; // B2P_IB_PLAYER_IDENTITY_VERIFY = 62

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onDisConnect
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    while(true)
    {
        KGThread_Sleep(100);
        nRetCode = m_pPaysys->HadRevUserLoginOut("0", &HadRevUserLoginOut);
        KGLOG_PROCESS_ERROR(nRetCode);
        KG_PROCESS_SUCCESS(HadRevUserLoginOut);

        nLoopTimes--;
        KGLOG_PROCESS_ERROR(nLoopTimes);
    }

Exit1:
    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNotRespondAndUserOnLine()  // caseID:6660
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6660,����Paysys������½����������֤���,GameCenter�����û�����ʱ,Client��Ҷ���,Paysys�յ��ǳ���Ϣ";
    int HadRevUserLoginOut = false;
    unsigned    nLoopTimes            = 100;

    KGLogPrintf(KGLOG_INFO, "caseID:6660\n");

    FillTestUserLoginInfo(KG_USER_LOGIN, E_ACCOUNT_OR_PASSWORD, AccountUserLoginInfo,  PASSPODMODE_UNBIND,  ACTION_SUCCESS, true, true, USER_ACCOUNT_LOCKED, false);
    m_GatewayTestConfig.PaysysManagerConfig.nNotRespondProtocol = 62; // B2P_IB_PLAYER_IDENTITY_VERIFY = 62

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onDisConnect
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    while(true)
    {
        KGThread_Sleep(100);
        nRetCode = m_pPaysys->HadRevUserLoginOut("0", &HadRevUserLoginOut);
        KGLOG_PROCESS_ERROR(nRetCode);
        KG_PROCESS_SUCCESS(!HadRevUserLoginOut);

        nLoopTimes--;
        KG_PROCESS_ERROR(nLoopTimes);
    }

Exit1:
    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedMibaoFailed()   // caseID:24734
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24734,����Paysys������½��GameCenter����Client��Ҳ�����,����������ܱ���,�����ܱ����������,����յ����������ʾ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:24734\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPMATRIX,  ACTION_FAILED, false, false, ACTION_FAILED, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    //nRetCode = m_pPaysys->SetAccountVerifyRespondType(AccountUserPasspodVerifyRet, PASSPODMODE_ZPMATRIX, ACTION_FAILED);
    //KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 1, onMibaoVerify);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedMibaoSucceed()   // caseID:24733
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24733,����Paysys������½��GameCenter����Client��Ҳ�����,����������ܱ���,�����ܱ���������ȷ,����յ�������ȷ��ʾ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID24733:\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPMATRIX, ACTION_SUCCESS, false, false, ACTION_SUCCESS, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    //nRetCode = m_pPaysys->SetAccountVerifyRespondType(AccountUserPasspodVerifyRet, PASSPODMODE_ZPMATRIX, ACTION_SUCCESS);
    //KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 1, onMibaoVerify);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedMibaoPaysysNoRespond()   // caseID:
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID????:,����Paysys���ص�½�˺�������ȷ,��Ҫ�ܱ���֤ʱ,�ͻ����Ѿ������ܱ�����,��paysys��������֤�����Client����յ��ܱ��������";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID????:\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPMATRIX, ACTION_SUCCESS, false, false, ACTION_FAILED, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);
    m_GatewayTestConfig.PaysysManagerConfig.nNotRespondSubProtocol = AccountUserPasspodVerify;

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pPaysys->SetAccountVerifyRespondType(AccountUserPasspodVerifyRet, PASSPODMODE_ZPMATRIX, ACTION_FAILED);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 1, onMibaoVerify);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedTokenFailed()   // caseID:24736
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24736,����Paysys������½��GameCenter����Client��Ҳ�����,��������������ܱ�,���������������,����յ����������ʾ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:24736\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPTOKEN, ACTION_FAILED, false, false, ACTION_FAILED, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    //nRetCode = m_pPaysys->SetAccountVerifyRespondType(AccountUserPasspodVerifyRet, PASSPODMODE_ZPTOKEN, ACTION_FAILED);
    //KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 1, onMibaoVerify);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedTokenSucceed()   // caseID:24735
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24735:,����Paysys������½��GameCenter����Client��Ҳ�����,��������������ܱ�,��������������ȷ,����յ�������ȷ��ʾ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:24735\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPTOKEN, ACTION_SUCCESS, false, false, ACTION_SUCCESS, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
        );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    //nRetCode = m_pPaysys->SetAccountVerifyRespondType(AccountUserPasspodVerifyRet, PASSPODMODE_ZPTOKEN, ACTION_SUCCESS);
    //KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 1, onMibaoVerify);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedMiBaoClientDisconnected()   // caseID:24737
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24737:,����Paysys������½��GameCenter����Client��Ҳ�����,����������ܱ���,����Ҳ������ܱ�����Ͷ���";
    unsigned    nLoopTimes            = 100;
    int HadRevUserLoginOut = false;

    KGLogPrintf(KGLOG_INFO, "caseID:24737\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPMATRIX, ACTION_SUCCESS, false, false, ACTION_SUCCESS, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    //nRetCode = m_pPaysys->SetAccountVerifyRespondType(AccountUserPasspodVerifyRet, PASSPODMODE_ZPMATRIX, ACTION_SUCCESS);
    //KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 1, onDisConnect);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    while(true)
    {
        KGThread_Sleep(100);
        nRetCode = m_pPaysys->HadRevUserLoginOut("0", &HadRevUserLoginOut);
        KGLOG_PROCESS_ERROR(nRetCode);
        KG_PROCESS_SUCCESS(HadRevUserLoginOut);

        nLoopTimes--;
        KGLOG_PROCESS_ERROR(nLoopTimes);
    }

Exit1:
    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedMibaoClientDisConnectedReConnect()   // caseID:24738
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24738,����Paysys������½��GameCenter����Client��Ҳ�����,����������ܱ���,����Ҳ������ܱ�����Ͷ���,�ص�";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:24738\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPMATRIX, ACTION_SUCCESS, false, false, ACTION_SUCCESS, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onDisConnect
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    //nRetCode = m_pPaysys->SetAccountVerifyRespondType(AccountUserPasspodVerifyRet, PASSPODMODE_ZPMATRIX, ACTION_SUCCESS);
    //KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);
    nRetCode = m_pClientController->AddOperateToClient(0, 4, onDisConnect, onConnectToGateway, onHandShake,onAccountVerify);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pClientController->CheckTheAccountLockedOrNot(0, &nAccountLockedState);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(!nAccountLockedState);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedTokenClientDisconnectedPaysysNoRespond()   // caseID:24739
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24739:,����Paysys������½��GameCenter����Client��Ҳ�����,��������������ܱ�,����������ܱ�����,��paysys��������֤���,��Ҷ���";
    unsigned    nLoopTimes   = 100;
    int HadRevUserLoginOut = false;

    KGLogPrintf(KGLOG_INFO, "caseID:24739\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPTOKEN, ACTION_SUCCESS, false, false, ACTION_SUCCESS, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pPaysys->SetNotHandleProtocol(74);//B2P_PLAYER_PASSPOD_VERIFY = 74
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 2, onMibaoVerify, onDisConnect);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    while(true)
    {
        KGThread_Sleep(100);
        nRetCode = m_pPaysys->HadRevUserLoginOut("0", &HadRevUserLoginOut);
        KGLOG_PROCESS_ERROR(nRetCode);
        KG_PROCESS_SUCCESS(HadRevUserLoginOut);

        nLoopTimes--;
        KGLOG_PROCESS_ERROR(nLoopTimes);
    }

Exit1:
    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestUserLoginPaysysNeedTokenClientDisConnectedReConnectPaysysNoRespond()   // caseID:24740
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:24740,����Paysys������½��GameCenter����Client��Ҳ�����,��������������ܱ�,����������ܱ�����,��paysys��������֤���,��Ҷ����ص�";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:24740\n");

    FillTestUserLoginInfo(KG_USER_MIBAO, ACTION_SUCCESS, AccountUserLoginInfo,  PASSPODMODE_ZPTOKEN, ACTION_SUCCESS, false, false, ACTION_SUCCESS, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        3,
        onConnectToGateway,
        onHandShake,
        onAccountVerify
        //onMibaoVerify
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pPaysys->SetNotHandleProtocol(74);//B2P_PLAYER_PASSPOD_VERIFY = 74
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(0, 5, onMibaoVerify, onDisConnect, onConnectToGateway, onHandShake, onAccountVerify);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pClientController->CheckTheAccountLockedOrNot(0, &nAccountLockedState);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(!nAccountLockedState);

    nResult = true;
Exit0:
    UnInitTestEnvironment();
#ifdef WIN32
    {
        KG_CASE_INFO(szTestCaseDscribe, "");
        KG_CPPUNIT_ASSERT(nResult);
    }
#else
    {
        //InsertInfo(CASEPURPOSE, (szTestCaseDscribe));
        //InsertInfo(PRECONDITION, (""));
        CPPUNIT_ASSERT(nResult);
    }
#endif
}
