///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName :   SO3GatewayTestRoleEnterGame.cpp
// Creator  :   liuzhibiao
// Date     :   2009-2-12
// Comment  :   v0.2
//              v0.1 2008-9-23 create
//              v0.2 2008-11-24 �������߼�����������linux��cppunit���ݴ���
//              v0.3 2009-2-12 ���뵥����ɫ�����������
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SO3GatewayTest.h"
#include "KClientController.h"
#include "KSimulateRelay.h"
#include "KSimulatePaysys.h"
#include "KGateWayMonitor.h"

void KGatewaytest::FillTestRoleEnterGameInfo(
    int nRetGameCenterRoleEnterGame,
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
    m_GatewayTestConfig.ClientConfig.nNeedTestTimeOut                   = false;
    m_GatewayTestConfig.ClientConfig.nNeedStopWhenRevAppointInfo        = nNeedStopWhenRevAppointInfo;
    m_GatewayTestConfig.ClientConfig.nNeedSendPing                      = true;

    m_GatewayTestConfig.PaysysManagerConfig.nUserLoginResult = ACTION_SUCCESS;
    m_GatewayTestConfig.GameCenterConfig.nUserOnlineResult   = false;
    m_GatewayTestConfig.GameCenterConfig.nGetRoleListCount   = 3;
    m_GatewayTestConfig.GameCenterConfig.nEnterGameResult    = nRetGameCenterRoleEnterGame;
    m_GatewayTestConfig.ClientConfig.nHadRevKick             = false;
    m_GatewayTestConfig.ClientConfig.nUserGiveUpQueue        = false;

    m_GatewayTestConfig.PaysysManagerConfig.wType            = AccountUserLoginInfo;//��ʱ
    m_GatewayTestConfig.PaysysManagerConfig.byMibaoMode      = PASSPODMODE_UNBIND;//��ʱ

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
    m_GatewayTestConfig.ForecastConfig.nTestPoint = KG_ROLE_ENTER_GAME;
    m_GatewayTestConfig.ForecastConfig.nResult = nResult;
    m_GatewayTestConfig.ForecastConfig.nTypeResult = 0;    

    //[State]
    m_ReturnResult.nTestPoint = m_GatewayTestConfig.ForecastConfig.nTestPoint;
    m_ReturnResult.nResult = 0;
    m_ReturnResult.nTypeResult = 0;
}

void KGatewaytest::TestRoleEnterGameSystemMaintenance() // caseID:5127
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:5127,Paysys������½ʱ,GameCenterϵͳά��, �û�ѡ���ɫ������Ϸ,Client����յ�������ά��";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:5127\n");

    FillTestRoleEnterGameInfo(eGameLoginSystemMaintenance, eGameLoginSystemMaintenance, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
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

void KGatewaytest::TestRoleEnterGameOverload()  // caseID:6519
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6519,Paysys������½ʱ,GameCenter�����������������޷���½, Client����յ�����������";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6519\n");

    FillTestRoleEnterGameInfo(eGameLoginOverload, eGameLoginOverload, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
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

void KGatewaytest::TestRoleEnterGameUnknownError()  // caseID:5128
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:5128,Paysys������½ʱ����Ҳ�����,GameCenter����δ֪����, �û�ѡ���ɫ������Ϸ,Client����յ�δ֪����";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:5128\n");

    FillTestRoleEnterGameInfo(eGameLoginUnknownError, eGameLoginUnknownError, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
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

void KGatewaytest::TestRoleEnterGameLoginSucceed()  // caseID:6668
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6668, Paysys������½ʱ����Ҳ�����,GameCenter����, �û�ѡ���ɫ������Ϸ,Client����յ��ɹ���½��Ϣ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6668\n");

    FillTestRoleEnterGameInfo(eGameLoginSucceed, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
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

void KGatewaytest::TestRoleEnterGameUserRevQueueWait() // caseID:6517
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6517,Paysys������½ʱ����Ҳ�����,GameCenter��������������Ҫ�Ŷ�, �û�ѡ���ɫ������Ϸ,Client����յ��Ŷ�״̬��Ϣ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6517\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, CLIENT_REV_LOGINGAME_STATE_INFO, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
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

void KGatewaytest::TestRoleEnterGameUserGCRevLoginRequest() // caseID:6667
{
    int  nResult                        = false;
    int  nRetCode                       = false;
    char szTestCaseDscribe[]            = "// caseID:6667,Paysys������½ʱ����Ҳ�����,GameCenter��������������Ҫ�Ŷ�, �û�ѡ���ɫ������Ϸ,Client����յ��Ŷ�״̬��Ϣ";
    int  nUserEnterGameRequestCount     = 0;
    unsigned    nLoopTimes              = 100;

    KGLogPrintf(KGLOG_INFO, "caseID:6667\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, CLIENT_REV_LOGINGAME_STATE_INFO, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    while(true)
    {
        KGThread_Sleep(100);
        nUserEnterGameRequestCount = m_pRelay->GetUserEnterGameRequestCount();
        KG_PROCESS_SUCCESS(nUserEnterGameRequestCount >= 2);

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

void KGatewaytest::TestRoleEnterGameGCNotRespondUserDisconnectPaysyRevLoginOut()  // caseID: 5132
{
    int  nResult  = false;
    int  nRetCode = false;
    char szTestCaseDscribe[] = "// caseID: 5132,GameCenter����Ӧ��½����,Client���ѡ���ɫ��½�����,Paysys�յ��ǳ���Ϣ";
    int  HadRevUserLoginOut = false;
    unsigned    nLoopTimes            = 100;   

    KGLogPrintf(KGLOG_INFO, "caseID:5132\n");

    FillTestRoleEnterGameInfo(eGameLoginSucceed, PAYSYS_HAD_REV_USER_LOGINOUT, true);
    m_GatewayTestConfig.GameCenterConfig.nNotRespondProtocol = g2r_game_login_request; // g2r_game_login_request

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        5, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame,
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

void KGatewaytest::TestRoleEnterGameUserDisconnectReLogin()  // caseID:6669
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6669,����Client��һ�ȡ��ɫ��Ϣ�ɹ�ʱ��GameCenter���ص�½�ɹ�֮��Client��Ҷ�������,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6669\n");

    FillTestRoleEnterGameInfo(eGameLoginSucceed, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        8, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame,
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

void KGatewaytest::TestRoleEnterGameGCNotRespondUserDisconnectReLogin()  // caseID:6670
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6670,����Client��һ�ȡ��ɫ��Ϣ�ɹ���GameCenter���ص�½���ʱ,Client��Ҷ�������,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6670\n");

    FillTestRoleEnterGameInfo(eGameLoginSucceed, eGameLoginSucceed, true);
    m_GatewayTestConfig.GameCenterConfig.nNotRespondProtocol = g2r_game_login_request; // g2r_game_login_request

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        8, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame,
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

// �ˡ��Ŷ�״̬[easQueueWaiting](10��)

void KGatewaytest::TestRoleEnterGameUserDisconnectPaysyRevLoginOut()  // caseID: 6677
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID: 6677,���Ի�ȡ��ɫ��Ϣ�ɹ�ʱ��GameCenter��������,Client����յ�����������Ϣ�����,Paysys�յ��ǳ���Ϣ";
    int HadRevUserLoginOut = false;
    unsigned    nLoopTimes            = 100;

    KGLogPrintf(KGLOG_INFO, "caseID:6677\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, PAYSYS_HAD_REV_USER_LOGINOUT, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        5, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame,
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

void KGatewaytest::TestRoleEnterGameGCQueueWaitUserDisconnectReLogin()  // caseID:6678
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6678,���Ի�ȡ��ɫ��Ϣ�ɹ�ʱ��GameCenter��������,Client����յ�����������Ϣ�����,���ص�,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:6678\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, PAYSYS_HAD_REV_USER_LOGINOUT, true);
    m_GatewayTestConfig.GameCenterConfig.nNotRespondProtocol = g2r_game_login_request; // g2r_game_login_request

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        8, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame,
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

void KGatewaytest::TestTwoPlayerRoleEnterGameQueueWaitTheSecondUserGiveup()   //caseID: 6518
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "//caseID: 6518,��������Client��һ�ȡ��ɫ��Ϣ�ɹ�,��ѡ���½��Ϸ����ʱ,GameCenter��������,�ڶ���Client�����������Ŷӵ�½,�ڶ���Client����յ������Ŷӻ�Ӧ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6518\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, CLIENT_REV_GIVEUP_QUEUE_RESPOND, true);

    nRetCode = InitTestEnvironment(true, true, true, 2, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);
    //nRetCode = m_pClientController->ModifyClientUserName(1, "�������");
    //KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(1, 1, onGiveUpQueue);
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

void KGatewaytest::TestRoleEnterGameGCFullLoginAGSucceed()  // caseID:6671
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6671,����ģ�ⳡ����ͼһ������,Client��ҽ����ŶӺ�ģ�ⳡ����ͼ���˳���1λClient��ң��ŶӺ�,������ͼ���ֿ�λ,Client�����������½ģ�ⳡ����ͼһ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6671\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSucceed);
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

void KGatewaytest::TestRoleEnterGameGCFullLoginAGSystemMaintenance()  // caseID:6673
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6673,����ģ�ⳡ����ͼһ������,Client��ҽ����Ŷ�֮��GameCenter֪ͨ��Ҫϵͳά��, Client����յ���Ϸ����ά����Ϣ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6673\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSystemMaintenance, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSystemMaintenance);
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

void KGatewaytest::TestFiveUserRoleEnterGameGCFullLoginAGTheFirstOneSucceed()  // caseID:6674
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6674,ģ�ⳡ����ͼһ������,5��Client��ҽ����Ŷ�֮��ģ�ⳡ����ͼ���˳���1λClient���.��1��Client��ҽ�����Ϸ����,��2��4��Client����յ���ȷ���Ŷ���Ϣ��";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6674\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 5, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetAllowLoginInGameWorldCount(1);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSucceed);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    m_GatewayTestConfig.ForecastConfig.nResult = CLIENT_REV_LOGINGAME_STATE_INFO;
    m_GatewayTestConfig.ForecastConfig.nTypeResult = 0;
    nRetCode = m_pClientController->GetReturnResult(1, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    m_GatewayTestConfig.ForecastConfig.nTypeResult = 1;
    nRetCode = m_pClientController->GetReturnResult(2, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    m_GatewayTestConfig.ForecastConfig.nTypeResult = 2;
    nRetCode = m_pClientController->GetReturnResult(3, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    m_GatewayTestConfig.ForecastConfig.nTypeResult = 3;
    nRetCode = m_pClientController->GetReturnResult(4, &Result);
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

void KGatewaytest::TestFiveUserRoleEnterGameGCFullLoginAGTheThirdOneGiveUp()  // caseID:6675
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6675,ģ�ⳡ����ͼһ������,ģ�ⳡ����ͼһ������,5��Client��ҽ����Ŷ�.��3��Client���ȡ���ȴ��Ŷ�,����Client����յ���ȷ���Ŷ���Ϣ��";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6675\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, CLIENT_REV_LOGINGAME_STATE_INFO, true);

    nRetCode = InitTestEnvironment(true, true, true, 5, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(2, 1, onGiveUpQueue);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(120);
    m_GatewayTestConfig.ForecastConfig.nTypeResult = 0;
    nRetCode = m_pClientController->GetReturnResult(0, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    m_GatewayTestConfig.ForecastConfig.nTypeResult = 1;
    nRetCode = m_pClientController->GetReturnResult(1, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    m_GatewayTestConfig.ForecastConfig.nTypeResult = 2;
    nRetCode = m_pClientController->GetReturnResult(3, &Result);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = memcmp(&Result, &(m_GatewayTestConfig.ForecastConfig), sizeof(Result));
    KGLOG_PROCESS_ERROR(!nRetCode);

    m_GatewayTestConfig.ForecastConfig.nTypeResult = 3;
    nRetCode = m_pClientController->GetReturnResult(4, &Result);
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

void KGatewaytest::TestFiveUserRoleEnterGameGCFullLoginAGAllSucceed()  // caseID:6676
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6676, ģ�ⳡ����ͼһ������,5��Client��ҽ����Ŷ�֮��ģ�ⳡ����ͼ���˳���5λClient���.5��Client���ʵ�ʵ�½��Ϸ�����˳���������½��Ϸ����˳����ͬ";
    KInGameWorldPlayer        InGameWorldPlayer;

    KGLogPrintf(KGLOG_INFO, "caseID:6676\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 5, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetAllowLoginInGameWorldCount(100);
    KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSucceed);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    nRetCode = m_pRelay->GetPlayer(&InGameWorldPlayer, "0");
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(InGameWorldPlayer.nNumInGameWorld == 1);

    nRetCode = m_pRelay->GetPlayer(&InGameWorldPlayer, "1");
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(InGameWorldPlayer.nNumInGameWorld == 2);

    nRetCode = m_pRelay->GetPlayer(&InGameWorldPlayer, "2");
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(InGameWorldPlayer.nNumInGameWorld == 3);

    nRetCode = m_pRelay->GetPlayer(&InGameWorldPlayer, "3");
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(InGameWorldPlayer.nNumInGameWorld == 4);

    nRetCode = m_pRelay->GetPlayer(&InGameWorldPlayer, "4");
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(InGameWorldPlayer.nNumInGameWorld == 5);

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

void KGatewaytest::TestUserRoleEnterGameGCFullLoginAGSucceed()  // caseID:5131
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:5131,ģ�ⳡ����ͼһ������,Client��ҽ����Ŷ�֮��ģ�ⳡ����ͼ���˳���1λClient���.Client��ҽ�����Ϸ����";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:5131\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSucceed);
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

void KGatewaytest::TestUserRoleEnterGameGCFullLoginAGGCChangeTheQueue()  // caseID:6672
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6672,ģ�ⳡ����ͼһ������,Client��ҽ����Ŷ�֮��GameCenter֪ͨGateway��ģ�ⳡ����ͼ�����Ŷ�,GameCenter֪ͨ����һ�������Ŷӵ�½,Client�����������½��Ϸ����";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:6672\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pRelay->ChangeTheLoginMap(2);
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSucceed);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(100);

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

void KGatewaytest::TestQueueCountRoleEnterGameGCFullLoginAGSucceed()  // caseID:6824
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6824,����ģ�ⳡ����ͼһ������,Client��ҽ����ŶӺ�ģ�ⳡ����ͼ���˳���1λClient��ң��ŶӺ�,������ͼ���ֿ�λ,Client�����������½ģ�ⳡ����ͼһ,Gateway����������Ϊ0����ʾ����뿪���ŶӶ��飩";
    //KG_RETURN_RESULT Result;
    int nWaitingQueuePlayer = 0;

    KGLogPrintf(KGLOG_INFO, "caseID:6824\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSucceed);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(100);

    nRetCode = m_pGatewayMonitor->GetWaitingQueuePlayerCount(&nWaitingQueuePlayer);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(nWaitingQueuePlayer == 0);

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

void KGatewaytest::TestQueueCountUserRoleEnterGameGCFullLoginAGGCChangeTheQueue()  // caseID:6825
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6825,ģ�ⳡ����ͼһ������,Client��ҽ����Ŷ�֮��GameCenter֪ͨGateway��ģ�ⳡ����ͼ�����Ŷ�,GameCenter֪ͨ����һ�������Ŷӵ�½,Gateway����������Ϊ1����ʾ��һ����ŶӶ����У�";
    int nWaitingQueuePlayer = 0;

    KGLogPrintf(KGLOG_INFO, "caseID:6825\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSucceed, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pRelay->ChangeTheLoginMap(2);
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pGatewayMonitor->GetWaitingQueuePlayerCount(&nWaitingQueuePlayer);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(nWaitingQueuePlayer == 1);

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

void KGatewaytest::TestQueueCountRoleEnterGameGCFullLoginAGSystemMaintenance()  // caseID:6826
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6826,����ģ�ⳡ����ͼһ������,Client��ҽ����Ŷ�֮��GameCenter֪ͨ��Ҫϵͳά��, Client����յ���Ϸ����ά����Ϣ,Gateway����������Ϊ0����ʾ����뿪���ŶӶ��飩";
    int nWaitingQueuePlayer = 0;

    KGLogPrintf(KGLOG_INFO, "caseID:6826\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSystemMaintenance, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginSystemMaintenance);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(100);

    nRetCode = m_pGatewayMonitor->GetWaitingQueuePlayerCount(&nWaitingQueuePlayer);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(nWaitingQueuePlayer == 0);

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

void KGatewaytest::TestQueueCountRoleEnterGameGCFullLoginAGSystemUnknowError()  // caseID:6827
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6827,����ģ�ⳡ����ͼһ������,Client��ҽ����Ŷ�֮��GameCenter����δ֪����, Client����յ�δ֪����,Gateway����������Ϊ0����ʾ����뿪���ŶӶ��飩";
    int nWaitingQueuePlayer = 0;

    KGLogPrintf(KGLOG_INFO, "caseID:6827\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSystemMaintenance, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginUnknownError);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(100);

    nRetCode = m_pGatewayMonitor->GetWaitingQueuePlayerCount(&nWaitingQueuePlayer);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(nWaitingQueuePlayer == 0);

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

void KGatewaytest::TestQueueCountRoleEnterGameGCFullLoginAGGCNotRespond()  // caseID:6828
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6828,����ģ�ⳡ����ͼһ������,GameCenter��һ�η�������������Ҫ�Ŷӣ�GameCenter�յ��ڶ��������½�󣬲����ؽ����Ϣ���ͻ��˶���,Gateway����������Ϊ0����ʾ����뿪���ŶӶ��飩";
    int nWaitingQueuePlayer = 0;

    KGLogPrintf(KGLOG_INFO, "caseID:6828\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSystemMaintenance, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetWhenRevDoNotRespond(g2r_game_login_request);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(70);

    m_pClientController->AddOperateToClient(0, 1, onDisConnect);
    Idle(100);

    nRetCode = m_pGatewayMonitor->GetWaitingQueuePlayerCount(&nWaitingQueuePlayer);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(nWaitingQueuePlayer == 0);

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

void KGatewaytest::TestQueueCountTwoPlayerRoleEnterGameQueueWaitTheSecondUserGiveup()   //caseID: 6829
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "//caseID: 6829,��������Client��һ�ȡ��ɫ��Ϣ�ɹ�,��ѡ���½��Ϸ����ʱ,GameCenter��������,�ڶ���Client�����������Ŷӵ�½,Gateway����������Ϊ1����ʾ�ڶ�����������뿪�ŶӶ��飩";
    int nWaitingQueuePlayer = 0;

    KGLogPrintf(KGLOG_INFO, "caseID:6829\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, CLIENT_REV_GIVEUP_QUEUE_RESPOND, true);

    nRetCode = InitTestEnvironment(true, true, true, 2, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);
    //nRetCode = m_pClientController->ModifyClientUserName(1, "�������");
    //KGLOG_PROCESS_ERROR(nRetCode);
    nRetCode = m_pClientController->AddOperateToClient(1, 1, onGiveUpQueue);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pGatewayMonitor->GetWaitingQueuePlayerCount(&nWaitingQueuePlayer);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(nWaitingQueuePlayer == 1);

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
        CPPUNIT_ASSERT(nResult);
    }
#endif
}

void KGatewaytest::TestQueueCountRoleEnterGameGCFullClientDisconnect()  // caseID:6830
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:6830,���Ի�ȡ��ɫ��Ϣ�ɹ���Client���ѡ���½��Ϸ����ʱ,GameCenter��������,Client����յ�����������Ϣ�����,Gateway����������Ϊ0����ʾ����뿪���ŶӶ��飩";
    int nWaitingQueuePlayer = 0;

    KGLogPrintf(KGLOG_INFO, "caseID:6830\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginSystemMaintenance, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    m_pClientController->AddOperateToClient(0, 1, onDisConnect);
    Idle(100);

    nRetCode = m_pGatewayMonitor->GetWaitingQueuePlayerCount(&nWaitingQueuePlayer);
    KGLOG_PROCESS_ERROR(nRetCode);
    KGLOG_PROCESS_ERROR(nWaitingQueuePlayer == 0);

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

void KGatewaytest::TestRoleEnterGameRoleFreeze() // caseID:10992
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:10992,����Client���ѡ���ɫ��½��Ϸ���磬GameCenter���ؽ�ɫ���ᣬClient����յ���ɫ������Ϣ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:10992\n");

    FillTestRoleEnterGameInfo(eGameLoginRoleFreeze, eGameLoginRoleFreeze, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
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

void KGatewaytest::TestRoleEnterGameGCFullLoginAGRoleFreeze()  // caseID:10993
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:10993,����Client���ѡ���ɫ��½��Ϸ���磬�������Ŷ�֮��GameCenter���ؽ�ɫ����,Gateway�����Ƿ�׼ȷ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:10993\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginRoleFreeze, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        4, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginRoleFreeze);
    KGLOG_PROCESS_ERROR(nRetCode);
    Idle(100);

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

void KGatewaytest::TestDisconnectPaysyRevLoginOutRoleEnterGameGCFullLoginAGRoleFreeze()  // caseID:10994
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:10994,����Client���ѡ���ɫ��½��Ϸ���磬�������Ŷ�֮��GameCenter���ؽ�ɫ����,Client����յ���ɫ������Ϣ�����, Paysys�յ��ǳ���Ϣ";
    int HadRevUserLoginOut = false;
    unsigned    nLoopTimes            = 100;

    KGLogPrintf(KGLOG_INFO, "caseID:10994\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, eGameLoginRoleFreeze, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        5, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame,
        onDisConnect
    );
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->Run();
    KGLOG_PROCESS_ERROR(nRetCode);

    Idle(70);
    nRetCode = m_pRelay->SetGameLogResult(eGameLoginRoleFreeze);
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

void KGatewaytest::TestRoleEnterGameGCQueueWaitFreezeRoleUserDisconnectReLogin()  // caseID:10995
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:10995,����Client���ѡ���ɫ��½��Ϸ���磬�������Ŷ�֮��GameCenter���ؽ�ɫ����,Client����յ���ɫ������Ϣ�����,���ص�,Client���û���յ�������Ϣ(��ʾû��Gateway����)";
    int nAccountLockedState = false;

    KGLogPrintf(KGLOG_INFO, "caseID:10995\n");

    FillTestRoleEnterGameInfo(eGameLoginQueueWait, PAYSYS_HAD_REV_USER_LOGINOUT, true);
    m_GatewayTestConfig.GameCenterConfig.nNotRespondProtocol = g2r_game_login_request; // g2r_game_login_request

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        8, 
        onConnectToGateway,
        onHandShake,
        onAccountVerify,
        onLoginGame,
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
