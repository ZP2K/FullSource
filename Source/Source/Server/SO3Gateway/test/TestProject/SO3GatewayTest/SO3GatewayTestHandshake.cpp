///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName :   SO3GatewayTestHandshake.cpp
// Creator  :   liuzhibiao
// Date     :   2008-11-5
// Comment  :   v0.2
//              v0.1 2008-9-22 create
//              v0.2 2008-11-5 ������ֶ�
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SO3GatewayTest.h"
#include "KClientController.h"

void KGatewaytest::FillTestHandShakeInfo(
    int nClientSendGatewayVersion,
    int nClientSendGameWorldVersion,
    int nGameWorldLowerVersion,
    int nGameWorldUpperVersion,
    int nPaysysLostconnect,
    int nClientForecastRecv,
    int nNeedStopWhenRevAppointInfo
)
{
    m_GatewayTestConfig.PaysysManagerConfig.nGatewayLoginResult         = ACTION_SUCCESS;
    m_GatewayTestConfig.PaysysManagerConfig.nPaysysLostconnect          = nPaysysLostconnect;
    m_GatewayTestConfig.PaysysManagerConfig.nNotRespondProtocol         = 0;
    m_GatewayTestConfig.GameCenterConfig.nGameWorldProtocolLowerVersion = nGameWorldLowerVersion;
    m_GatewayTestConfig.GameCenterConfig.nGameWorldProtocolUpperVersion = nGameWorldUpperVersion;
    m_GatewayTestConfig.ClientConfig.nClientGatewayVersion              = nClientSendGatewayVersion;
    m_GatewayTestConfig.ClientConfig.nClientGameWorldVersion            = nClientSendGameWorldVersion;
    m_GatewayTestConfig.PaysysManagerConfig.nUserLoginResult            = ACTION_SUCCESS;
    m_GatewayTestConfig.GameCenterConfig.nGetRoleListCount              = 0;
    m_GatewayTestConfig.GameCenterConfig.nNotRespondProtocol            = 0;
    m_GatewayTestConfig.PaysysManagerConfig.nNotHandleProtocol          = 0;
    m_GatewayTestConfig.ClientConfig.nNeedTestTimeOut                   = false;
    m_GatewayTestConfig.ClientConfig.nNeedStopWhenRevAppointInfo        = nNeedStopWhenRevAppointInfo;
    m_GatewayTestConfig.ClientConfig.nNeedSendPing                      = true;

    //[Forecast]
    m_GatewayTestConfig.ForecastConfig.nTestPoint = KG_CLIENT_HANDSHAKE;
    m_GatewayTestConfig.ForecastConfig.nResult = nClientForecastRecv;
    m_GatewayTestConfig.ForecastConfig.nTypeResult = 0;    

    //[State]
    m_ReturnResult.nTestPoint = m_GatewayTestConfig.ForecastConfig.nTestPoint;
    m_ReturnResult.nResult = 0;
    m_ReturnResult.nTypeResult = 0;
}

void KGatewaytest::TestHandShakeGameWorldMaintenance()  // caseID:4998
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:4998,Client����,GateCenter���ص���Ϸ����汾�ŷ�Χ�����޶�Ϊ0,����ʧ��,Client����յ�ϵͳ����ά��";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:4998\n");

    FillTestHandShakeInfo(CLIENT_GATEWAY_VERSION, 20, 0, 0, false, ghcGameWorldMaintenance, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        2, 
        onConnectToGateway,
        onHandShake
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

void KGatewaytest::TestHandShakeAccountSystemLost() // caseID:4999
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:4999,Client����,Paysys���Ӳ�����,����ʧ��,Client����յ��ʺ�ϵͳ(paysys)���Ӷ�ʧ";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:4999\n");

    FillTestHandShakeInfo(CLIENT_GATEWAY_VERSION, 20, 0, 100, true, ghcAccountSystemLost, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        2, 
        onConnectToGateway,
        onHandShake
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

#ifdef __GNUC__
    KGThread_Sleep(20000);  //��linux�±�֤����������ʼ������ʱ�ɹ�
#endif

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

void KGatewaytest::TestHandShakeGameWorldVersionError() // caseID:5000
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:5000,Client����,GateCenter���ص���Ϸ����汾����ͻ��˰汾�Ų�ƥ��,����ʧ��,Client����յ���Ϸ����汾��ƥ��,����¿ͻ���";
    KG_RETURN_RESULT Result;

    KGLogPrintf(KGLOG_INFO, "caseID:5000\n");

    FillTestHandShakeInfo(CLIENT_GATEWAY_VERSION, 20, 50, 100, false, ghcGameWorldVersionError, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    nRetCode = m_pClientController->FillOperate(
        2, 
        onConnectToGateway,
        onHandShake
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

void KGatewaytest::TestHandShakeGatewayVersionNotMatch()    // caseID:4997
{
    int nResult  = false;
    int nRetCode = false;
    char szTestCaseDscribe[] = "// caseID:4997,Client����,�ͻ��˱�������ذ汾��־λ�뵱ǰ�������ذ汾��ƥ��,����ʧ��,Client����յ����ذ汾��ƥ��,�����Client���";
    KG_RETURN_RESULT Result;
    
    KGLogPrintf(KGLOG_INFO, "caseID:4997\n");

    FillTestHandShakeInfo(5, 20, 0, 100, false, ghcGatewayVersionError, true);

    nRetCode = InitTestEnvironment(true, true, true, 1, m_GatewayTestConfig.ForecastConfig.nTestPoint);
    KGLOG_PROCESS_ERROR(nRetCode);

    m_pClientController->FillOperate(
        2, 
        onConnectToGateway,
        onHandShake
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
