#include "StdAfx.h"
#include "KGameCenterWrapper.h"
#include "KGameServerSimulator.h"
#include "KGatewaySimulator.h"
#include "TestTools.h"
#include "gtest/gtest.h"

class KTestPlayerCGStateConver : public testing::Test 
{
protected:
KTestPlayerCGStateConver(void)
{
	m_pSrcGameServer = NULL;
	m_pDesGameServer = NULL;
}

~KTestPlayerCGStateConver(void)
{
}

void KTestPlayerCGStateConver::SetUp()
{
    BOOL  bResult = false;
    int   nRetCode = false;
    std::vector<MapData>::iterator pMapCopyIter ;

    m_pSrcGameServer = new KGameServerSimulator();
    nRetCode = m_pSrcGameServer->Init();
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer = new KGameServerSimulator();
    nRetCode = m_pDesGameServer->Init();
    ASSERT_TRUE(nRetCode);

    m_MapCopyExistInSrc.dwMapID = m_pSrcGameServer->m_CreatedMapCopy.front().dwMapID;
    m_MapCopyExistInSrc.nMapCopy = m_pSrcGameServer->m_CreatedMapCopy.front().nMapCopy;

    m_MapCopyExistInDes.dwMapID = m_pDesGameServer->m_CreatedMapCopy.front().dwMapID;
    m_MapCopyExistInDes.nMapCopy = m_pDesGameServer->m_CreatedMapCopy.front().nMapCopy;

    m_MapCopyNoCreate.dwMapID = m_pSrcGameServer->m_CreatedMapCopy.front().dwMapID;
    m_MapCopyNoCreate.nMapCopy = UINT_MAX;

	nRetCode = GC->CanCreateMap(m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    m_TestRoleInfo = TestTools::CreateRole();
    ASSERT_TRUE(m_TestRoleInfo.dwPlayerID != ERROR_ID);

    m_RoleInfo_2 = TestTools::CreateRole();
    ASSERT_TRUE(m_RoleInfo_2.dwPlayerID != ERROR_ID);

    m_RoleInfo_3 = TestTools::CreateRole();
    ASSERT_TRUE(m_RoleInfo_3.dwPlayerID != ERROR_ID);

    nRetCode = TestTools::PlayerEnterGame(m_pSrcGameServer, &m_TestRoleInfo, m_MapCopyExistInSrc.dwMapID, m_MapCopyExistInSrc.nMapCopy);
    ASSERT_TRUE(nRetCode);
}

void KTestPlayerCGStateConver::TearDown()
{
    SAFE_DELETE(m_pSrcGameServer);
	SAFE_DELETE(m_pDesGameServer);
	KGThread_Sleep(200);
	TestTools::DeleteRole(&m_TestRoleInfo);
	TestTools::DeleteRole(&m_RoleInfo_2);
	TestTools::DeleteRole(&m_RoleInfo_3);
}

BOOL KTestPlayerCGStateConver::MakeRoleToOnLine(KGameServerSimulator* pGameServer, TestTools::KRoleInfo& stRoleInfo, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL	  bResult		       = false;
	int		  nRetCode			   = false;

	nRetCode = TestTools::PlayerEnterGame(pGameServer, &stRoleInfo, dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL ComparePlayerState(DWORD dwPlayerID, KROLE_STATE eState)
{
	KGThread_Sleep(100);
	return GC->RoleSate(dwPlayerID) == eState;
}

BOOL MakeRoleToCreateMap(KGameServerSimulator* pSrcGS, KGameServerSimulator* pDesGS, DWORD dwPlayerID, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult			 = false;
	int  nRetCode			 = false;

	pDesGS->DoUpdatePerformance(3000, 0 ,1);
	pDesGS->DoUpdatePerformance(3000, 0 ,1);
	pSrcGS->DoUpdatePerformance(2000, 0 ,1);
	KGThread_Sleep(100);
	nRetCode = pSrcGS->DoSearchMapRequset(dwPlayerID, dwMapID, nMapCopyID, 0, 0 ,0);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pDesGS->CheckCreateMapNotify(dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL MakeRoleToPlayerData(KGameServerSimulator* pSrcGS, DWORD dwPlayerID, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult = false;
	int  nRetCode = false;

	nRetCode = pSrcGS->DoSearchMapRequset(dwPlayerID, dwMapID, nMapCopyID, 0, 0 ,0);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pSrcGS->CheckSearchMapRespond(dwPlayerID, smrcSuccess, TRUE , dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL MakeRoleToGuid(KGameServerSimulator* pSrcGS, KGameServerSimulator* pDesGS, TestTools::KRoleInfo& stRoleInfo, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult = false;
	int  nRetCode = false;

	nRetCode = MakeRoleToPlayerData(pSrcGS, stRoleInfo.dwPlayerID, dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pSrcGS->DoTranferPlayerRequest(stRoleInfo.dwPlayerID, 
		stRoleInfo.szAccount, stRoleInfo.szRoleName,
		dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pDesGS->CheckTransferPlayerRequest(stRoleInfo.dwPlayerID, 
		dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL MakeRoleToEnter_Leave(KGameServerSimulator* pSrcGS, KGameServerSimulator* pDesGS, TestTools::KRoleInfo& stRoleInfo, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult = false;
	int  nRetCode = false;

	nRetCode = MakeRoleToGuid(pSrcGS, pDesGS, stRoleInfo, dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pDesGS->DoTranferPlayerRespond(stRoleInfo.dwPlayerID, TRUE);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pSrcGS->CheckTransferPlayerRespond(stRoleInfo.dwPlayerID, TRUE);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL MakeRoleToSrcLeave(KGameServerSimulator* pSrcGS, KGameServerSimulator* pDesGS, TestTools::KRoleInfo& stRoleInfo, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult = false;
	int  nRetCode = false;

	nRetCode = MakeRoleToEnter_Leave(pSrcGS, pDesGS, stRoleInfo, dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pDesGS->DoConfirmPlayerLoginRequest(m_TestRoleInfo.dwPlayerID);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL MakeRoleToSrcTimeOut(KGameServerSimulator* pSrcGS, KGameServerSimulator* pDesGS, TestTools::KRoleInfo& stRoleInfo, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult = false;
	int  nRetCode = false;

	nRetCode = MakeRoleToSrcLeave(pSrcGS, pDesGS, stRoleInfo, dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pDesGS->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL MakeRoleToDesEnter(KGameServerSimulator* pSrcGS, KGameServerSimulator* pDesGS, TestTools::KRoleInfo& stRoleInfo, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult = false;
	int  nRetCode = false;

	nRetCode = MakeRoleToEnter_Leave(pSrcGS, pDesGS, stRoleInfo, dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pSrcGS->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
	KG_PROCESS_ERROR(nRetCode);

	bResult = true;
Exit0:
	return bResult;
}

BOOL MakeRoleToDesTimeOut(KGameServerSimulator* pSrcGS, KGameServerSimulator* pDesGS, TestTools::KRoleInfo& stRoleInfo, DWORD dwMapID, __int64 nMapCopyID)
{
	BOOL bResult = false;
	int  nRetCode = false;

	nRetCode = MakeRoleToGuid(pSrcGS, pDesGS, stRoleInfo, dwMapID, nMapCopyID);
	KG_PROCESS_ERROR(nRetCode);

	nRetCode = pSrcGS->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);

	bResult = true;
Exit0:
	return bResult;
}
protected:
	KGameServerSimulator* m_pDesGameServer;
	KGameServerSimulator* m_pSrcGameServer;

	TestTools::KRoleInfo    m_TestRoleInfo;
	TestTools::KRoleInfo    m_RoleInfo_2;
	TestTools::KRoleInfo    m_RoleInfo_3;

	MapData      m_MapCopyExistInSrc;
	MapData      m_MapCopyExistInDes;
	MapData      m_MapCopyNoCreate;
	MapData      m_MapCopyCanCreate;
};

TEST_F(KTestPlayerCGStateConver, TestOnLineToOffLine_1)
{
    BOOL bResult = false;
    int  nRetCode = false;

    CASE_INFO("����뿪����GS", "");

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestOnLineToOffLine_2)
{
    BOOL bResult = false;
    int  nRetCode = false;

    CASE_INFO("�������GS崻�", "");

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);

    m_pSrcGameServer->CloseConnect();

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestOnLineToOnLine)
{
    BOOL bResult = false;
    int  nRetCode = false;
    DWORD     dwSearchMapID = 0;
    __int64   nSearchMapCopyID = 0;

    CASE_INFO("����л�����ʱ���������ڲ���Ҫ���", "");

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);

    dwSearchMapID = m_pSrcGameServer->m_CreatedMapCopy[1].dwMapID;
    nSearchMapCopyID = m_pSrcGameServer->m_CreatedMapCopy[1].nMapCopy;

    nRetCode = m_pSrcGameServer->DoSearchMapRequset(m_TestRoleInfo.dwPlayerID,dwSearchMapID, nSearchMapCopyID, 0, 0, 0);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckSearchMapRespond(m_TestRoleInfo.dwPlayerID, smrcSuccess, FALSE, dwSearchMapID, nSearchMapCopyID);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestOnLineToCreateMap)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("����л�����ʱ��������Ҫ����", "");

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoSearchMapRequset(m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy, 0, 0, 0);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckCreateMapNotify(m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode);
}


TEST_F(KTestPlayerCGStateConver, TestOnLineToPlayerData)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("����л�����ʱ������������Ҫ���", "");

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoSearchMapRequset(m_TestRoleInfo.dwPlayerID, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy, 0, 0 ,0);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckSearchMapRespond(m_TestRoleInfo.dwPlayerID, smrcSuccess, TRUE , 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForPlayerData);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestCreatemapToOffLine_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("�������������У�����뿪ԴGS", "");

    nRetCode = MakeRoleToCreateMap(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode); 

    nRetCode = m_pSrcGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestCreatemapToOffLine_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("ͬ���������������У�ԴGS崻�", "");

    nRetCode = MakeRoleToCreateMap(m_pSrcGameServer, m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode);

    m_pSrcGameServer->CloseConnect();

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestCreateMapToOnLine_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("ͬ�������������ɹ�", "");

    nRetCode = MakeRoleToCreateMap(m_pSrcGameServer, m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoCreateMapRespond(m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy, FALSE);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckSearchMapRespond(m_TestRoleInfo.dwPlayerID, smrcFailed, FALSE, 
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestCreateMapToOnLine_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("ͬ�����������ɹ�", "");

    nRetCode = MakeRoleToCreateMap(m_pSrcGameServer, m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoCreateMapRespond(m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy, TRUE);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckSearchMapRespond(m_TestRoleInfo.dwPlayerID, smrcSuccess, FALSE, 
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestCreateMapToOnLine_3)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("����������������У�Ŀ��GS崻�", "");

    nRetCode = MakeRoleToCreateMap(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->CloseConnect();

    nRetCode = m_pSrcGameServer->CheckSearchMapRespond(m_TestRoleInfo.dwPlayerID, smrcFailed, FALSE, 
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestCreateMapToOnLine_4)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("��������������ɹ�", "");

    nRetCode = MakeRoleToCreateMap(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    KGThread_Sleep(100);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->DoCreateMapRespond(m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy, FALSE);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckSearchMapRespond(m_TestRoleInfo.dwPlayerID, smrcFailed, FALSE,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestCreateMapToPlayerData)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("������������ɹ�", "");

    nRetCode = MakeRoleToCreateMap(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForCreateMap);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->DoCreateMapRespond(m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy, TRUE);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckSearchMapRespond(m_TestRoleInfo.dwPlayerID, smrcSuccess, TRUE, 
        m_MapCopyCanCreate.dwMapID, m_MapCopyCanCreate.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForPlayerData);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestPlayerDataToOffLine_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("GC�ȴ�����ԴGS�����������ʱ������뿪ԴGS", "");

    nRetCode = MakeRoleToPlayerData(m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForPlayerData);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestPlayerDataToOffLine_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;


    CASE_INFO("GC�ȴ�����ԴGS�����������ʱ��ԴGS崻�", "");

    nRetCode = MakeRoleToPlayerData(m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForPlayerData);
    ASSERT_TRUE(nRetCode);

    m_pSrcGameServer->CloseConnect();

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestPlayerDataToOnLine_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;


    CASE_INFO("GC�ȴ�ԴGS���ʹ����������ʱ��Ŀ��GS崻�", "");

    nRetCode = MakeRoleToPlayerData(m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForPlayerData);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->CloseConnect();

    nRetCode = m_pSrcGameServer->DoTranferPlayerRequest(m_TestRoleInfo.dwPlayerID, 
        m_TestRoleInfo.szAccount, m_TestRoleInfo.szRoleName,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckTransferPlayerRespond(m_TestRoleInfo.dwPlayerID, FALSE);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestPlayerDataToOnLine_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("GC��Ŀ��GS��������ұ��ܾ�", "");

    nRetCode = nRetCode = MakeRoleToPlayerData(m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForPlayerData);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoTranferPlayerRequest(m_TestRoleInfo.dwPlayerID, 
        m_TestRoleInfo.szAccount, m_TestRoleInfo.szRoleName,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->CheckTransferPlayerRequest(m_TestRoleInfo.dwPlayerID, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoTranferPlayerRespond(m_TestRoleInfo.dwPlayerID, FALSE);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckTransferPlayerRespond(m_TestRoleInfo.dwPlayerID, FALSE);
	ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestPlayerDataToGuid)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("GC��Ŀ��GS��������ұ�ͬ��", "");

    nRetCode = nRetCode = MakeRoleToPlayerData(m_pSrcGameServer, m_TestRoleInfo.dwPlayerID,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForPlayerData);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoTranferPlayerRequest(m_TestRoleInfo.dwPlayerID, 
        m_TestRoleInfo.szAccount, m_TestRoleInfo.szRoleName,
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->CheckTransferPlayerRequest(m_TestRoleInfo.dwPlayerID, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForGuid);
    ASSERT_TRUE(nRetCode);
}


TEST_F(KTestPlayerCGStateConver, TestGuidToDestTimeout_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("GC�ȴ�Ŀ��GS�ظ����������Ӧʱ������뿪ԴGS", "");

    nRetCode = MakeRoleToGuid(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForGuid);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);


    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestTimeout);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestGuidToDestTimeout_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("GC�ȴ�Ŀ��GS�ظ����������Ӧʱ��ԴGS崻�", "");

    nRetCode = MakeRoleToGuid(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForGuid);
    ASSERT_TRUE(nRetCode);

    m_pSrcGameServer->CloseConnect();

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestTimeout);
    ASSERT_TRUE(nRetCode);
}


TEST_F(KTestPlayerCGStateConver, TestGuidToOnline_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("GC�ȴ�Ŀ��GS�ظ����������Ӧʱ��Ŀ��GS崻�", "");

    nRetCode = MakeRoleToGuid(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForGuid);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->CloseConnect();

    nRetCode = m_pSrcGameServer->CheckTransferPlayerRespond(m_TestRoleInfo.dwPlayerID, FALSE);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestGuidToOnline_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("Ŀ��GS��ͬ�⴫�����", "");

    nRetCode = MakeRoleToGuid(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForGuid);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoTranferPlayerRespond(m_TestRoleInfo.dwPlayerID, FALSE);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckTransferPlayerRespond(m_TestRoleInfo.dwPlayerID, FALSE);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestGuidToEnter_Leave)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("Ŀ��GSͬ�⴫�����", "");

    nRetCode = MakeRoleToGuid(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForGuid);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoTranferPlayerRespond(m_TestRoleInfo.dwPlayerID, TRUE);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->CheckTransferPlayerRespond(m_TestRoleInfo.dwPlayerID, TRUE);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForEnter_Leave);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestEnter_LeaveToSrcTimeout_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("��ҵȴ����ʱ��Ŀ��GS崻�", "");

    nRetCode = MakeRoleToEnter_Leave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForEnter_Leave);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->CloseConnect();

    KGThread_Sleep(1000);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcTimeout);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestEnter_LeaveToSrcTimeout_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("��ҵȴ����ʱ����ҵ�¼Ŀ��GS��ʱ", "");

    nRetCode = MakeRoleToEnter_Leave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForEnter_Leave);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    KGThread_Sleep(1000);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcTimeout);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestEnter_LeaveToDestEnter_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("��ҵȴ����ʱ���뿪ԴGS", "");

    nRetCode = MakeRoleToEnter_Leave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForEnter_Leave);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestEnter);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestEnter_LeaveToDestEnter_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("��ҵȴ����ʱ��ԴGS崻�", "");

    nRetCode = MakeRoleToEnter_Leave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForEnter_Leave);
    ASSERT_TRUE(nRetCode);

    m_pSrcGameServer->CloseConnect();

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestEnter);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestEnter_LeaveToSrcLeave)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("��ҵȴ����ʱ��GC�յ�Ŀ��GSȷ�ϵ�¼��Ϣ", "");

    nRetCode = MakeRoleToEnter_Leave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForEnter_Leave);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoConfirmPlayerLoginRequest(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);


    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcLeave);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestSrcLeaveToOnline_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("����뿪ԴGS", "");

    nRetCode = MakeRoleToSrcLeave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcLeave);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestSrcLeaveToOnline_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("ԴGS崻�", "");

    nRetCode = MakeRoleToSrcLeave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcLeave);
    ASSERT_TRUE(nRetCode);

    m_pSrcGameServer->CloseConnect();

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestSrcLeaveToSrcTimeout_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("����뿪Ŀ��GS", "");

    nRetCode = MakeRoleToSrcLeave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcLeave);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcTimeout);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestSrcLeaveToSrcTimeout_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("Ŀ��GS崻�", "");

    nRetCode = MakeRoleToSrcLeave(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcLeave);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->CloseConnect();

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcTimeout);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestSrcTimeoutToOffLine_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("ԴGS崻�", "");

    nRetCode = MakeRoleToSrcTimeOut(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcTimeout);
    ASSERT_TRUE(nRetCode);

    m_pSrcGameServer->CloseConnect();

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestSrcTimeoutToOffLine_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("����뿪ԴGS", "");

    nRetCode = MakeRoleToSrcTimeOut(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForSrcTimeout);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pSrcGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestDestEnterToOffLine_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("Ŀ��GS崻�", "");

    nRetCode = MakeRoleToDesEnter(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestEnter);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->CloseConnect();

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestDestEnterToOffLine_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("��ҵ�¼Ŀ��GS��ʱ", "");

    nRetCode = MakeRoleToDesEnter(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestEnter);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestDestEnterToOnline)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("GC�յ�Ŀ��GSȷ�ϵ�¼��Ϣ", "");

    nRetCode = MakeRoleToDesEnter(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestEnter);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoConfirmPlayerLoginRequest(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->CheckConfirmPlayerLoginRespond(m_TestRoleInfo.dwPlayerID, TRUE);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOnline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestDestTimeoutToOffLine_1)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("����뿪Ŀ��GS", "");

    nRetCode = MakeRoleToDesTimeOut(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestTimeout);
    ASSERT_TRUE(nRetCode);

    nRetCode = m_pDesGameServer->DoPlayerLeaveGS(m_TestRoleInfo.dwPlayerID);
    ASSERT_TRUE(nRetCode);

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}

TEST_F(KTestPlayerCGStateConver, TestDestTimeoutToOffLine_2)
{
    BOOL	  bResult		       = false;
    int		  nRetCode			   = false;

    CASE_INFO("Ŀ��GS崻�", "");

    nRetCode = MakeRoleToDesTimeOut(m_pSrcGameServer, m_pDesGameServer, m_TestRoleInfo, 
        m_MapCopyExistInDes.dwMapID, m_MapCopyExistInDes.nMapCopy);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsCG_WaitForDestTimeout);
    ASSERT_TRUE(nRetCode);

    m_pDesGameServer->CloseConnect();

    nRetCode = GATEWAY->CheckPlayerLeaveGame(m_TestRoleInfo.szAccount);
    ASSERT_TRUE(nRetCode);

    nRetCode = ComparePlayerState(m_TestRoleInfo.dwPlayerID, rsOffline);
    ASSERT_TRUE(nRetCode);
}