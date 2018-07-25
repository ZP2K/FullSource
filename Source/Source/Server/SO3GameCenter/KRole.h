/************************************************************************/
/* �������								                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.11.19	Create												*/
/* Comment	 :															*/
/*		Relay�ϱ���������Ҫ���������ʱ���ʵ�����						*/
/************************************************************************/
#ifndef _KROLE_H_
#define _KROLE_H_

#include "Global.h"
#include "Luna.h"

enum KROLE_STATE
{
	rsInvalid = 0,

	rsOffline,
	rsOnline,

	//LG = Login����¼�������״
	rsLG_WaitForPermit,
	rsLG_WaitForLogin,

	//CG = ChangeGameserver������������״̬
	rsCG_WaitForCreateMap,
	rsCG_WaitForPlayerData,
	rsCG_WaitForGuid,
	rsCG_WaitForEnter_Leave,
	rsCG_WaitForSrcLeave,
	rsCG_WaitForDestEnter,
	rsCG_WaitForDestTimeout,
	rsCG_WaitForSrcTimeout,

	rsTotal
};

#pragma pack(1)
class KRole
{
public:
	KRole(void);
	~KRole(void);

	KROLE_STATE GetRoleState(void);
	void SetRoleState(KROLE_STATE eDestState);

    BOOL IsOnline();

    BOOL UpdateMapCopyOwnerForTeamLeader();

    BOOL OnLeaveGS(int nConnIndex);

    BOOL CallLoginScript();
    BOOL CallLogoutScript();

public:    
    int  LuaIsOnline(Lua_State* L);

public:

	const char* 			m_pszAccount;	            // ��ɫ���ʺ���
	char					m_szName[_NAME_LEN];	    // ��ɫ�Ľ�ɫ��
	DWORD					m_dwPlayerID;				// ��ɫ�����ݿ���ˮ��

	int						m_nConnIndex;				// ��ɫ���ڵ�GS�����Ӻ�
	DWORD					m_dwMapID;					// ��ɫ���ڵĵ�ͼ���
	int					    m_nMapCopyIndex;			// ��ɫ���ڵĵ�ͼ������

    int                     m_nDestConnIndex;
    DWORD                   m_dwDestMapID;
    int                     m_nDestMapCopyIndex;

    DWORD                   m_dwSystemTeamID;           // ϵͳ�����Ķ����ID
    DWORD                   m_dwTeamID;                 // ������������ID
    KCAMP                   m_eCamp;                    // ���������Ӫ
    DWORD                   m_dwTongID;                 // �������ID

    DWORD                   m_dwForceID;                // ��������ID
    time_t                  m_nLastSaveTime;
    time_t                  m_nCreateTime;
    BYTE                    m_byType;                   // ��ɫ���ͣ�ȡROLE_TYPEö��ֵ
    BYTE                    m_byLevel;                  // �ȼ�

private:
	KROLE_STATE				m_eRoleState;			    // ��ɫ����Ϸ״̬

public:
    const char* getAccount(void) { return m_pszAccount; }
    int getLevel(void) {return (int)m_byLevel; }
    int getType(void) {return (int)m_byType; }

    DECLARE_LUA_CLASS(KRole);    
    DECLARE_LUA_STRING(Name, sizeof(m_szName));
    DECLARE_LUA_DWORD(PlayerID);
    DECLARE_LUA_DWORD(MapID);
    DECLARE_LUA_INTEGER(ConnIndex);
    DECLARE_LUA_INTEGER(MapCopyIndex);
    DECLARE_LUA_DWORD(TeamID);
    DECLARE_LUA_ENUM(Camp);
    DECLARE_LUA_DWORD(TongID);
    DECLARE_LUA_TIME(LastSaveTime);
    DECLARE_LUA_DWORD(ForceID);
};
#pragma pack()

#endif	//_KROLE_H_

