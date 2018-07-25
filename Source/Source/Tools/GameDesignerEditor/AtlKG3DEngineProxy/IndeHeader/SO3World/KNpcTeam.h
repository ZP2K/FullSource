#ifndef _KNPC_TEAM_
#define _KNPC_TEAM_


#include "KNpcOrderList.h"

#define MAX_NPC_TEAM_COUNT 16

class KCharacter;
class KNpc;

enum NPC_TEAM_MODE
{
	ntmNormal = 0,
	ntmKing,

	ntmTotal
};

struct KPatrolTarget
{
	int nX;
	int nY;
};

class KNpcTeam
{
public:
	KNpcTeam();
	~KNpcTeam();

	void Activate();

	BOOL Init(DWORD dwOrder);
	void UnInit();

	void SetOrder(DWORD dwOrder);
	int	 GetOrder();
	BOOL SetLeader(DWORD dwLeader);
	BOOL SetLeader();				//ѡ������һ��û������Npc��ΪLeader;
	BOOL SetKing(DWORD dwKing);

	BOOL AddMember(KNpc* pNewMember, int nIndex);
	BOOL RemoveMember(int nIndex);
	KNpc* GetMember(int nIndex);
	BOOL GetMemberPostion(int nIndex, int* pnDistance, int* pnAngle);
	int GetMemberCount() { return m_nMemberCount; }

#ifdef _SERVER
	// �õ�һ�����ѵ�Ŀ��ӵ��Լ��ĳ���б���,��˭����ν
	KCharacter* GetMemberTarget();	
	// ���Npc�Ĺ����Ƿ������
	BOOL CheckNpcWorkDone();

	// �¼�
	BOOL OnMemberDeath(int nIndex);
	BOOL OnMemberRevive(int nIndex, OUT int& nX, OUT int& nY, OUT int& nZ);
#endif //_SERVER

	KNpc* m_pLeader;			//С�ӵ��쵼�����еĳ�Ա�������쵼��������ƫ�ƣ�������������Ա�滻
	KNpc* m_pKing;				//С�ӵ�King�������ж�һЩ����Ķ��飬����������ȫ����ʧ

private:
	KNpc* m_pMember[MAX_NPC_TEAM_COUNT];
	int m_nMemberCount;
	const KORDER* m_pOrder;
	int m_nOrder;
};

#endif //_KNPC_TEAM_
