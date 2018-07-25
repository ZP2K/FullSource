#include "StdAfx.h"
#include "gtest/gtest.h"
#include "TestTools.h"
#include "KGameServerSimulator.h"
#include "KGameCenterWrapper.h"
#include "CampTools.h"
#include "Utility.h"
#include "TestDef.h"


struct ChangeMinPlayer 
{
	ChangeMinPlayer()
	{
		KBattleFieldManager::KBASE_INFO_TABLE&  BaseInfo = GC->m_pGC->m_BattleFieldManager.m_BaseInfoTable;
		KBattleFieldManager::KBASE_INFO_TABLE::iterator Iter = BaseInfo.begin();
		for (Iter; Iter != BaseInfo.end(); Iter++)
		{
			MinPlayerList.push_back(Iter->second.nMinPlayerPerSide);
			Iter->second.nMinPlayerPerSide = 1;
		}
	}
	~ChangeMinPlayer()
	{
		int i = 0;
		KBattleFieldManager::KBASE_INFO_TABLE&  BaseInfo = GC->m_pGC->m_BattleFieldManager.m_BaseInfoTable;
		KBattleFieldManager::KBASE_INFO_TABLE::iterator Iter = BaseInfo.begin();
		for (Iter; Iter != BaseInfo.end(); Iter++)
		{
			Iter->second.nMinPlayerPerSide = MinPlayerList[i++];
		}
	}
	std::vector<int> MinPlayerList;
};
class KTestBattleField : public testing::Test 
{
protected:
	KTestBattleField(void)
	{
		m_pGameServer = NULL;
	}
	~KTestBattleField(void)
	{
		m_pGameServer = NULL;
		m_RoleList.clear();
	}

	void SetUp()
	{
		ChangeTimeCheckLuaFunction();
		m_pGameServer = new KGameServerSimulator();
		ASSERT_TRUE(m_pGameServer->Init());

		for(int i = 0; i < 12 ; i++)
		{
			TestTools::KRoleInfo& RoleInfo = TestTools::CreateRole();
			ASSERT_TRUE(RoleInfo.dwPlayerID != ERROR_ID);
			m_RoleList.push_back(RoleInfo);
		}

		m_TestRoleA = &m_RoleList[0];
		m_TestRoleB = &m_RoleList[1];
		m_TestRoleC = &m_RoleList[m_RoleList.size() - 1];

		for (unsigned int  i = 0; i < 2; i++)
		{
			int nRetCode = TestTools::PlayerEnterGame(m_pGameServer, &m_RoleList[i],
				m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
			ASSERT_TRUE(nRetCode);
		}

		ASSERT_TRUE(!GC->GetBattleList().empty());
		m_pBI = &(GC->GetBattleList().begin()->second);
		pBattleManager = &GC->Center()->m_BattleFieldManager;
	}

	void TearDown()
	{
		SAFE_DELETE(m_pGameServer);

		KGThread_Sleep(100);

		for (unsigned int  i = 0; i < m_RoleList.size(); i++)
		{
			TestTools::DeleteRole(&m_RoleList[i]);
		}
		pBattleManager = NULL;
	}

	bool  SetPlayerForce(DWORD dwPlayerID, DWORD dwForce)
	{
		if (dwForce == 0)
		{
			if (m_pGameServer->DoChangeRoleForceIDRequest(dwPlayerID, 0) &&
				m_pGameServer->CheckSyncFellowshipPlayerForceID(dwPlayerID, 0))
				return true;
		}

		int nIndex = 0;
		for (int i = 0; i < sizeof(DWORD) * CHAR_BIT; i++)
		{
			if (((dwForce >> i) & 0x1) == 1)
			{
				if (m_pGameServer->DoChangeRoleForceIDRequest(dwPlayerID, i) &&
					m_pGameServer->CheckSyncFellowshipPlayerForceID(dwPlayerID, i))
					return true;
			}
		}

		return false;
	}

	bool SetPlayerLevel(DWORD dwPlayerID, int  nLevel)
	{
		if (m_pGameServer->DoChangeRoleLevelRequest(dwPlayerID, nLevel) &&
			m_pGameServer->CheckSyncFellowshipPlayerLevel(dwPlayerID, nLevel))
		{
			return true;
		}
		return false;
	}

	int CreateBattle(DWORD dwMapID)
	{
		SCOPEPROTOCOL(m_pGameServer, r2s_create_map_notify);
		int nCopyIndex = GC->CreateBattleField(dwMapID);
		if (nCopyIndex == 0)
			return false;

		if (!m_pGameServer->CheckCreateMapNotify(dwMapID, nCopyIndex))
			return false;

		m_pGameServer->DoCreateMapRespond(dwMapID, nCopyIndex, true);
		GC->AccelerationMultipleGC(14);
		return nCopyIndex;
	}

	bool CheckNotify(BATTLE_FIELD_NOTIFY_TYPE eType, KGameServerSimulator* pServer)
	{
		pServer->AddListenProtocol(r2s_battle_field_notify);
		bool bFind = false;
		GC->AccelerationMultipleGC( 7 * 3 * 2);
		GC->ManualActivate();
		std::list<IKG_Buffer*>& List = pServer->ReceiveDataTime(r2s_battle_field_notify, 1);
		std::list<IKG_Buffer*>::iterator Iter = List.begin();
		for (Iter ; Iter != List.end(); ++Iter)
		{
			R2S_BATTLE_FIELD_NOTIFY* pData = (R2S_BATTLE_FIELD_NOTIFY*)(*Iter )->GetData();
			if (pData->eType == eType)
			{

				bFind = true;
				break;
			}
		}
		Iter = List.begin();
		for (Iter; Iter != List.end(); ++Iter)
		{
			KG_COM_RELEASE(*Iter);
		}
		return bFind;
	}
	
	void ChangeTimeCheckLuaFunction()
	{
		std::string LuaScript = 
			"function NewCheckTime(nTime)\n"
			"	return true\n"
			"end\n"
			"for k, v in pairs(_G) do\n"
				"if type(v) == \"table\" then\n"
			"		for tk, tv in pairs(v) do\n"
			"			index = string.find(tk, \"CheckJoinTime\")\n"
			"			if index ~= nil then\n"
			"				print(k.. '.'..tk)"
			"				v[tk] = NewCheckTime\n"
			"			end\n"
					"end\n"
			"	end\n"
			"end\n";
		SocpeLock(GC->Mutex());
		ILuaScriptEx* pLua = GC->Center()->m_ScriptManager.GetScriptHolder();
		if (pLua)
		{
			pLua->LoadFromBuffer(0, "ChangeCheckTimeFunction", LuaScript.c_str(), LuaScript.size());
		}
	}
	KGameServerSimulator* m_pGameServer;
	std::vector<TestTools::KRoleInfo> m_RoleList;
	TestTools::KRoleInfo* m_TestRoleA;
	TestTools::KRoleInfo* m_TestRoleB;
	TestTools::KRoleInfo* m_TestRoleC;
	KBattleFieldManager::KBASE_INFO* m_pBI;
	KBattleFieldManager* pBattleManager;
	ChangeMinPlayer m_ChangeMinPlayer;
};

TEST_F(KTestBattleField, TestJionQuenu_1)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("������Ҽ���ս������", "�����ڵĵ�ͼID");

	for (int i = 1; i < 20; i++ )
	{
		if (GC->GetBattleList().find(i) == GC->GetBattleList().end())
		{
			dwMapID = i;
		}
	}
	ASSERT_TRUE(dwMapID != 0);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, 0, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcFailed);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_2)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("������Ҽ���ս������", "�����ڵĸ���ID");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, INT_MAX, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcFailed);
	ASSERT_TRUE(!nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_3)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	
	CASE_INFO("������Ҽ���ս������", "�����ڵ����ID");

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, INT_MAX, 0, false, ERROR_ID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(ERROR_ID, dwMapID, bfrcFailed);
	ASSERT_TRUE(!nRetCode);
}


TEST_F(KTestBattleField, TestJionQuenu_4)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	
	CASE_INFO("������Ҽ���ս������", "��ҵȼ���ƥ��");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel - 1);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcLevelError);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMaxJoinLevel + 1);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcLevelError);
	ASSERT_TRUE(nRetCode);
}


TEST_F(KTestBattleField, TestJionQuenu_5)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	KBattleFieldManager::KQUEUE& Queue = pBattleManager->m_QueueTable.find(dwMapID)->second;
	ResumeScopeValue<DWORD> ResumeValue(&Queue.dwForcMask[0], 2);

	CASE_INFO("������Ҽ���ս������", "���force��ƥ��");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, ~2);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcForceError);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_6)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	
	CASE_INFO("������Ҽ���ս������", "����Ķ���ID");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 5, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcFailed);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_7)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	

	CASE_INFO("������Ҽ���ս������", "��Ҳ��ٸø������Ҳ��ٵȴ�������");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_8)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("������Ҽ���ս������", "����Ѽ���õ�ͼ����");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_13)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("������Ҽ���ս������", "����ں�����");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(GC->AddInBlackList(m_TestRoleA->dwPlayerID, 100));

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcInBlackList);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenuToMany)
{
	bool bResult = false;
	int  nRetCode = false;
	size_t uCont = 0;
	
	KBattleFieldManager::KBASE_INFO_TABLE::iterator Iter = GC->GetBattleList().begin();

	CASE_INFO("������Ҽ���ս������", "��Ҽ���������");

	ASSERT_TRUE(GC->GetBattleList().size() >= MAX_QUEUE_PER_PLAYER);

	for (Iter; (Iter != GC->GetBattleList().end()) && (uCont < MAX_QUEUE_PER_PLAYER); ++Iter)
	{
		m_pBI = &Iter->second;
		nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
		ASSERT_TRUE(nRetCode);

		nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
		ASSERT_TRUE(nRetCode);

		CampTools ct;
		ct.m_pGS = m_TestRoleA->pLoginOnGS;
		ASSERT_TRUE(ct.SetCamp(m_TestRoleA->dwPlayerID, m_pBI->eCampRequire[0]));

		nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(m_pBI->dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
		ASSERT_TRUE(nRetCode);

		nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, m_pBI->dwMapID, bfrcSuccess);
		ASSERT_TRUE(nRetCode);
		uCont++;
	}

	
	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(m_pBI->dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, m_pBI->dwMapID, bfrcTooManyJoin);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_9)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	
	CASE_INFO("��Ӽ������ս������", "��Ҳ��ٶ�����");
	
	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, true, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcFailed);
	ASSERT_TRUE(nRetCode);
}


TEST_F(KTestBattleField, TestJionQuenu_10)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	
	CASE_INFO("��Ӽ������ս������", "��Ҳ��Ƕӳ�");

	nRetCode = TestTools::CreateParty(m_TestRoleA, m_TestRoleB);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleB->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleB->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, true, m_TestRoleB->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleB->dwPlayerID, dwMapID, bfrcFailed);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_11)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	
	CASE_INFO("��Ӽ������ս������", "��Ա������������");

	nRetCode = TestTools::CreateParty(m_TestRoleA, m_TestRoleB);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleB->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleB->dwPlayerID, m_pBI->nMinJoinLevel - 1);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, true, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcTeamMemberError);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_12)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	
	CASE_INFO("��Ӽ������ս������", "��Ա�������ϵ�");

	nRetCode = TestTools::CreateParty(m_TestRoleA, m_TestRoleB);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleB->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleB->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, true, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_14)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	ResumeScopeValue<int> ResumeValue(&m_pBI->nMaxPlayerPerSide, 1);
	CASE_INFO("��Ӽ������ս������", "�����������ڸ��������������");

	nRetCode = TestTools::CreateParty(m_TestRoleA, m_TestRoleB);
	ASSERT_TRUE(nRetCode);


	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, &m_RoleList[2], 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = TestTools::JoinParty(m_TestRoleA, &m_RoleList[2]);
	ASSERT_TRUE(nRetCode);



	nRetCode = SetPlayerForce(m_RoleList[2].dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_RoleList[2].dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, true, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcTeamSizeError);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestJionQuenu_16)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("��Ӽ������ս������", "���������ں�����");

	ASSERT_TRUE(m_pBI->nMaxPlayerPerSide <= MAX_TEAM_GROUP_NUM);

	nRetCode = TestTools::CreateParty(m_TestRoleA, m_TestRoleB);
	ASSERT_TRUE(nRetCode);


	for (int i = 0; i < MAX_TEAM_GROUP_NUM; i++)
	{
		nRetCode = SetPlayerForce(m_RoleList[i].dwPlayerID, m_pBI->dwForcMask[0]);
		ASSERT_TRUE(nRetCode);

		nRetCode = SetPlayerLevel(m_RoleList[i].dwPlayerID, m_pBI->nMinJoinLevel);
		ASSERT_TRUE(nRetCode);
	}

	ASSERT_TRUE(GC->AddInBlackList(m_TestRoleB->dwPlayerID, 2));

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, true, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcTeamMemberError);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestHandleQuenu_1)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	

	CASE_INFO("������ߵȴ���", "���ڷ��͵ȴ���Ϣ");

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, false, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckBattleFieldNotify();
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestHandleQuenu_2)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;
	
	R2S_BATTLE_FIELD_NOTIFY BattleInfo;
	int RoleIndex = 2;

	CASE_INFO("������ߵȴ���", "�ŶӶ��������������������ϵͳû�и����ɽ���");

	for (int i = 2; i < m_RoleList.size(); i++)
	{
		nRetCode = TestTools::PlayerEnterGame(m_pGameServer, &m_RoleList[i], 
			m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
		ASSERT_TRUE(nRetCode);
	}

	for (int n =0; n < MAX_BATTLE_SIDE; n++)
	{
		if (!m_pBI[n].bEnableGroup)
		{
			continue;
		}
		for (int k = 0; (k < m_pBI->nMinPlayerPerSide) && (RoleIndex < m_RoleList.size()); k++)
		{
			nRetCode = SetPlayerForce(m_RoleList[RoleIndex].dwPlayerID, m_pBI->dwForcMask[n]);
			ASSERT_TRUE(nRetCode);

			nRetCode = SetPlayerLevel(m_RoleList[RoleIndex].dwPlayerID, m_pBI->nMinJoinLevel);
			ASSERT_TRUE(nRetCode);

			nRetCode = m_RoleList[RoleIndex].pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, n, false, m_RoleList[RoleIndex].dwPlayerID);
			ASSERT_TRUE(nRetCode);

			nRetCode = m_RoleList[RoleIndex].pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_RoleList[RoleIndex].dwPlayerID, dwMapID, bfrcSuccess);
			ASSERT_TRUE(nRetCode);

			RoleIndex++;
		}
	}
	
	SCOPEPROTOCOL(m_pGameServer, r2s_create_map_notify);
	GC->AccelerationMultipleGC(7);
	GC->ManualActivate();
	nRetCode = m_pGameServer->CheckCreateMapNotify(dwMapID, &nCopyIndex);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestHandleQuenu_3)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;
	
	CASE_INFO("����Ѿ���ս���ȴ�������", "ͬ��������");

	ASSERT_TRUE(CreateBattle(dwMapID));

	ASSERT_TRUE(TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC,
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy));

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));
}


TEST_F(KTestBattleField, TestHandleQuenu_4)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;

	CASE_INFO("�����Ѿ���ս���ȴ�������", "ͬ��������");
	
	nRetCode = TestTools::CreateParty(m_TestRoleA, m_TestRoleB);
	ASSERT_TRUE(nRetCode);
	
	nCopyIndex = CreateBattle(dwMapID);
	ASSERT_TRUE(nCopyIndex);

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleB->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleB->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, 0, 0, true, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleA->pLoginOnGS));
}

TEST_F(KTestBattleField, TestHandleQuenu_5)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;

	CASE_INFO("�����Ѿ���ս���ȴ�������", "�����������ָ�������ض�������ǵĶ���������");

	nRetCode = TestTools::CreateParty(m_TestRoleA, m_TestRoleB);
	ASSERT_TRUE(nRetCode);

	nCopyIndex = CreateBattle(dwMapID);
	ASSERT_TRUE(nCopyIndex);

	nRetCode = SetPlayerForce(m_TestRoleA->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleA->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleB->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleB->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex + 1, 0, true, m_TestRoleA->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleA->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleA->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(!CheckNotify(bfntJoinBattleField, m_TestRoleA->pLoginOnGS));
}

TEST_F(KTestBattleField, TestHandleWaitQuenu_1)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;
	
	CASE_INFO("����Ѿ���ս���ȴ�������", "ͬ��������");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));

	nRetCode = m_TestRoleC->pLoginOnGS->DoAcceptJoinBattleField(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckAcceptJoinBattleFieldRespond(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestHandleWaitQuenu_2)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;

	CASE_INFO("����Ѿ���ս���ȴ�������", "�ȴ���ʱ");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));
	
	GC->AccelerationGCTime(130);
	ASSERT_TRUE(CheckNotify(bfntLeaveBattleField, m_TestRoleC->pLoginOnGS));
}

TEST_F(KTestBattleField, TestHandleWaitQuenu_4)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;
	
	CASE_INFO("����Ѿ���ս���ȴ�������", "���߳�ʱ");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);


	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));

	nRetCode = TestTools::PlayerLeaveGame(m_TestRoleC);
	ASSERT_TRUE(nRetCode);

	GC->AccelerationGCTime(620);
	GC->AccelerationMultipleGC(14);
	KGThread_Sleep(10);
	nRetCode = GC->CheckPermission(m_TestRoleC->dwPlayerID, dwMapID, nCopyIndex);
	ASSERT_TRUE(!nRetCode);
}

TEST_F(KTestBattleField, TestHandleWaitQuenu_5)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;
	
	CASE_INFO("����Ѿ���ս���ȴ�������", "�ܽӼ���");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));

	m_TestRoleC->pLoginOnGS->DoLeaveBattleFieldQueueRequest(dwMapID, m_TestRoleC->dwPlayerID);
	
	nRetCode = m_TestRoleC->pLoginOnGS->CheckLeaveBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, true);
	ASSERT_TRUE(nRetCode);
}


TEST_F(KTestBattleField, TestJionQuenu_15)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;

	CASE_INFO("������Ҽ���", "������Ѿ��ڸ����ȴ�����");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
			m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));

	m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, false, m_TestRoleC->dwPlayerID);

	SCOPEPROTOCOL(m_TestRoleC->pLoginOnGS, r2s_join_battle_field_queue_respond);
	ASSERT_TRUE(CheckNotify(bfntLeaveBattleField, m_TestRoleC->pLoginOnGS));
	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestHandleWaitQuenu_3)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;
	
	CASE_INFO("����Ѿ���ս���ȴ�������", "��Ҽ���ս��");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));

	m_TestRoleC->pLoginOnGS->DoAcceptJoinBattleField(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckAcceptJoinBattleFieldRespond(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	m_TestRoleC->pLoginOnGS->DoSearchMapRequset(m_TestRoleC->dwPlayerID, dwMapID, nCopyIndex, 0,0,0);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckSearchMapRespond(m_TestRoleC->dwPlayerID, smrcSuccess, false, dwMapID, nCopyIndex);
	ASSERT_TRUE(nRetCode);
}


TEST_F(KTestBattleField, TesthandPlayingList_1)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;

	CASE_INFO("����Ѿ���ս��������", "����л���ͼ");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));

	m_TestRoleC->pLoginOnGS->DoAcceptJoinBattleField(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckAcceptJoinBattleFieldRespond(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	KGThread_Sleep(100);

	m_TestRoleC->pLoginOnGS->DoSearchMapRequset(m_TestRoleC->dwPlayerID, dwMapID, nCopyIndex, 0,0,0);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckSearchMapRespond(m_TestRoleC->dwPlayerID, smrcSuccess, false, dwMapID, nCopyIndex);
	ASSERT_TRUE(nRetCode);

	GC->AccelerationMultipleGC(14);
	GC->ManualActivate();

	m_TestRoleC->pLoginOnGS->DoSearchMapRequset(m_TestRoleC->dwPlayerID, m_pGameServer->m_CreatedMapCopy[0].dwMapID,
		m_pGameServer->m_CreatedMapCopy[0].nMapCopy, 0, 0, 0);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckSearchMapRespond(m_TestRoleC->dwPlayerID, smrcSuccess, false,
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	GC->AccelerationMultipleGC(14);
	GC->ManualActivate();
	nRetCode = GC->GetBattleFieldSide(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode == -1);
}

TEST_F(KTestBattleField, TesthandPlayingList_2)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;

	CASE_INFO("����Ѿ���ս��������", "��ҵ����ڳ�ʱ����������");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, 
		m_pGameServer->m_CreatedMapCopy[0].dwMapID, m_pGameServer->m_CreatedMapCopy[0].nMapCopy);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerForce(m_TestRoleC->dwPlayerID, m_pBI->dwForcMask[0]);
	ASSERT_TRUE(nRetCode);

	nRetCode = SetPlayerLevel(m_TestRoleC->dwPlayerID, m_pBI->nMinJoinLevel);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->DoJoinBattleFieldQueueRequest(dwMapID, nCopyIndex, 0, 0, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckJoinBattleFieldQueueRespond(m_TestRoleC->dwPlayerID, dwMapID, bfrcSuccess);
	ASSERT_TRUE(nRetCode);

	ASSERT_TRUE(CheckNotify(bfntJoinBattleField, m_TestRoleC->pLoginOnGS));

	m_TestRoleC->pLoginOnGS->DoAcceptJoinBattleField(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckAcceptJoinBattleFieldRespond(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode);

	m_TestRoleC->pLoginOnGS->DoSearchMapRequset(m_TestRoleC->dwPlayerID, dwMapID, nCopyIndex, 0,0,0);

	nRetCode = m_TestRoleC->pLoginOnGS->CheckSearchMapRespond(m_TestRoleC->dwPlayerID, smrcSuccess, FALSE, dwMapID, nCopyIndex);
	ASSERT_TRUE(nRetCode);

	GC->AccelerationMultipleGC(14);
	GC->ManualActivate();

	nRetCode = GC->CheckPermission(m_TestRoleC->dwPlayerID, dwMapID, nCopyIndex);
	ASSERT_TRUE(!nRetCode);

	nRetCode = TestTools::PlayerLeaveGame(m_TestRoleC);
	ASSERT_TRUE(nRetCode);

	GC->AccelerationMultipleGC(14);
	GC->ManualActivate();
	nRetCode = GC->CheckPermission(m_TestRoleC->dwPlayerID, dwMapID, nCopyIndex);
	ASSERT_TRUE(nRetCode);

	nRetCode = TestTools::PlayerEnterGame(m_pGameServer, m_TestRoleC, dwMapID, nCopyIndex);
	ASSERT_TRUE(nRetCode);

	GC->AccelerationMultipleGC(14);
	GC->ManualActivate();
	nRetCode = GC->CheckPermission(m_TestRoleC->dwPlayerID, dwMapID, nCopyIndex);
	ASSERT_TRUE(!nRetCode);
	nRetCode = GC->GetBattleFieldSide(dwMapID, nCopyIndex, m_TestRoleC->dwPlayerID);
	ASSERT_TRUE(nRetCode == 0);
}

TEST_F(KTestBattleField, TestGetBattleList_1)
{
	bool bResult = false;
	int  nRetCode = false;
	
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("��ȡս���б�", "�����ڵ����ID");

	m_pGameServer->DoGetBattleFieldList(ERROR_ID, m_pBI->dwMapID);

	nRetCode = m_pGameServer->CheckSyncBattleFieldList(ERROR_ID, m_pBI->dwMapID, 0);
	ASSERT_TRUE(!nRetCode);
}

TEST_F(KTestBattleField, TestGetBattleList_2)
{
	bool bResult = false;
	int  nRetCode = false;
	
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("��ȡս���б�", "�����ڵĵ�ͼID");

	m_pGameServer->DoGetBattleFieldList(m_TestRoleA->dwPlayerID, ERROR_ID);

	nRetCode = m_pGameServer->CheckSyncBattleFieldList(m_TestRoleA->dwPlayerID, ERROR_ID, 0);
	ASSERT_TRUE(!nRetCode);
}

TEST_F(KTestBattleField, TestGetBattleList_3)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;

	CASE_INFO("��ȡս���б�", "����ս���б�");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));
	GC->AccelerationMultipleGC(14);
	GC->ManualActivate();

	m_pGameServer->DoGetBattleFieldList(m_TestRoleA->dwPlayerID, m_pBI->dwMapID);

	nRetCode = m_pGameServer->CheckSyncBattleFieldList(m_TestRoleA->dwPlayerID, m_pBI->dwMapID, 0);
	ASSERT_TRUE(nRetCode);
}

TEST_F(KTestBattleField, TestBlackList_1)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("����ں�������", "���ڷ����ں�������Ϣ");

	GC->AddInBlackList(m_TestRoleA->dwPlayerID, 600);
	GC->AccelerationMultipleGC(14);
	ASSERT_TRUE(CheckNotify(bfntInBlackList, m_pGameServer));
}

TEST_F(KTestBattleField, TestBlackList_2)
{
	bool bResult = false;
	int  nRetCode = false;
	
	DWORD dwMapID = m_pBI->dwMapID;

	CASE_INFO("����ں�������", "����������ʱ���Ƴ�������");

	GC->AddInBlackList(m_TestRoleA->dwPlayerID, 1);
	
	GC->AccelerationGCTime(10);

	ASSERT_TRUE(CheckNotify(bfntLeaveBlackList, m_pGameServer));
}

TEST_F(KTestBattleField, TestDestoryPQ)
{
	bool bResult = false;
	int  nRetCode = false;
	DWORD dwMapID = m_pBI->dwMapID;
	int64_t nCopyIndex = 0;
	R2S_SYNC_PQ PQInfo;

	CASE_INFO("ս����������", "�رշ�����");

	ASSERT_TRUE(nCopyIndex = CreateBattle(dwMapID));

	nRetCode = m_pGameServer->CheckSyncPQ(PQInfo);
	ASSERT_TRUE(nRetCode);

	m_pGameServer->UnInit();

	GC->AccelerationMultipleGC(14);
	GC->ManualActivate();

	ASSERT_TRUE(GC->GetPQ(PQInfo.dwPQID) == NULL);
}
