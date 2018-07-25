#ifndef _KESCORT_QUEST_MANAGER_H_
#define _KESCORT_QUEST_MANAGER_H_
#include <map>

#ifdef _SERVER

class KScene;
class KPlayer;
class KNpc;

enum ESCORT_QUEST_STATE
{
    eqsInvalid = 0,
	
	eqsWaitStart,
	eqsRunning,
    eqsFinished,
    eqsClosed,

	eqsTotal
};

struct KEscortQuest
{
	DWORD				dwNpcAcceptID;                  // �ӻ��������NPC
	DWORD             	dwNpcEscortID;                  // ���͵�NPC
	DWORD             	dwPlayerID[MAX_PARTY_SIZE];     // ������
	BOOL				bValid[MAX_PARTY_SIZE];		    // ��������б�־
	int             	nTimer;                         // ����ʱ
	int               	nEndFrames;                     // ����Ľ�ֹ֡
	ESCORT_QUEST_STATE  eQuestState;                    // �Ƿ��ڻ�ı��
};

class KEscortQuestManager
{
public:
    BOOL Init(KScene* pScene);
    void UnInit(){};

    void Activate();

    BOOL AddEscortQuest(DWORD dwQuestID, KPlayer* pPlayerAccept, KNpc* pNpcAccept);
    BOOL CanAcceptEscortQuest(DWORD dwQuestID);
    BOOL AddPlayer(DWORD dwQuestID, DWORD dwPlayerID);
    BOOL StartEscortQuest(DWORD dwQuestID);
    BOOL SetEscortNpc(DWORD dwQuestID, DWORD dwNpcEscortID);
    BOOL SetEscortQuestEndFrames(DWORD dwQuestID, int nFrames);
    BOOL SetEscortQuestValue(DWORD dwQuestID, int nIndex, int nValue);
    BOOL AddPlayerBuff(DWORD dwQuestID, DWORD dwBuffID, int nBuffLevel);
    BOOL DelPlayerBuff(DWORD dwQuestID, DWORD dwBuffID, int nBuffLevel);
    BOOL SetPlayerFailed(KPlayer* pPlayer, DWORD dwQuestID);
    BOOL CloseEscortQuest(DWORD dwQuestID, int nFrames);
    BOOL EndEscortQuest(DWORD dwQuestID);

    const KEscortQuest* GetEscortQuest(DWORD dwQuestID);

private:
    BOOL InvitePlayerInTeam(DWORD dwQuestID, KPlayer* pPlayerAccept);
    void OnQuestWaitStart(DWORD dwQuestID, KEscortQuest* pEscortQuest);
    void OnQuestRunning(DWORD dwQuestID, KEscortQuest* pEscortQuest);

private:
    typedef std::map<DWORD, KEscortQuest> MAP_ESCORT_QUEST;
    MAP_ESCORT_QUEST m_EscortQuestMap;

    KScene* m_pScene;
};
#endif
#endif
