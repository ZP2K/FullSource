/************************************************************************/
/* ����װ����	                                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.31	Create												*/
/* Comment	 :															*/
/*		�������ĳ����������											*/
/************************************************************************/
#ifndef _KDOODAD_H_
#define	_KDOODAD_H_

#include "Luna.h"
#include "KSceneObject.h"
#include "IDoodadFile.h"

#define MAX_ROLL_FRAME 90 * GAME_FPS

enum DOODAD_STATE
{
	dsInvalid = 0,				

	// �ܿ�״̬
	dsIdle,						// ����״̬
	dsBeHit,					// �������򱻻�
	dsAfterHit,					// �򿪺�򱻻�����

	dsTotal
};

class KCharacter;
class KLootList;
class KDoodadTemplate;

class KDoodad : public KSceneObject
{
public:
	BOOL        Init();
	void        UnInit();
	BOOL        Activate(void);
	BOOL        Load(const KDOODAD_DATA& crDoodadData);
    void        ChangeState(DOODAD_STATE eState);
    BOOL        CheckOpen(KPlayer* pOpener);
    BOOL        HaveQuest(KPlayer* pPlayer);
    int	        GetOpenFrame(KPlayer* pOpener);
    DWORD       GetRecipeID();
    void        OnBreakPickPrepare();
    void        CallBreakScriptFunction(KPlayer* pOpener);

private:
    void        DoIdle();
    void        DoBeHit();
    void        DoAfterHit();

#ifdef _CLIENT
public:
    void        OpenLootList(KCharacter* pCharacter);
    void        UpdateMiniMapMark();
#endif

#ifdef _SERVER
public:
    void        LootOver();
    BOOL        SetDelayRemove();
    void        SetDisappearFrames(int nDisappearFrames, BOOL bDisappearToReviveList = true);
    int         GetDisappearFrames();
    void        Revive();
    BOOL        PerOpen(KPlayer* pPlayer);
    BOOL        SetScript(const char* pcszFileName);		    // �޸ĹҵĽű���·��
    KLootList*  GenerateLootList(KPlayer* pCharacter, BOOL bIsAbsoluteFree);          // ���ɵ����
    KLootList*  GenerateLootListFromNpc(KPlayer* pDropTarget, KNpc* pNpc);
    KLootList*  GenerateEmptyLootList();                        // ���ɿյĵ����
    void        Close();
    
private:
    void        Remove(BOOL bGotoReviveList = true);
#endif

public:
	DWORD					m_dwTemplateID;					// ģ����
	char					m_szName[_NAME_LEN];			// ����
	DOODAD_KIND				m_eKind;						// ����
	int						m_nOpenFrames;					// ��֡��
	int						m_nDisappearFrames;				// ��ʧ֡��
	BOOL					m_bDisappearToReviveList;       // ��ʧ��ɾ��,���ǽ���������
	DOODAD_STATE			m_eState;						// ״̬
	DWORD					m_dwNpcTemplateID;
    DWORD                   m_dwNpcID;                      // ʬ���Ӧ��NPC
	DWORD					m_dwRepresentID;				// Doodad�ı���ID
	BOOL					m_bLooted;						// ʬ���Ƿ�ʰȡ��ı��
    int                     m_nObstacleGroup;               // ���ƶ�̬�ϰ��ķ���

	DWORD					m_dwReviveID;					// ����Ⱥ��ı��
	int						m_nCurrentReliveIndex;			// ��ǰ�����ı��
    KDoodadTemplate*		m_pTemplate;					// ָ��ģ���ָ��
    DWORD					m_dwScriptID;					// �󶨽ű�ID
    KLootList*				m_pLootList;					// ʰȡ�б�

#ifdef _SERVER
	int						m_nLastLootCount;				// ʣ���ʰȡ����
	DWORD					m_dwOpenPlayerID;               // ��ǰ����doodad�����
	DWORD					m_dwOwnerID;					// �ű��������Doodad��PlayerID

    int                     m_nProgressID;                  // ��Doodad�Ľ���ID
#endif

#ifdef _CLIENT
	int						m_nUpdateMiniMapMarkFrame;		// ����С��ͼ��ǵļ�ʱ��
#endif 
   
public:
    DECLARE_LUA_CLASS(KDoodad);
    DECLARE_LUA_DWORD(TemplateID);
    DECLARE_LUA_STRING(Name, _NAME_LEN);
    DECLARE_LUA_ENUM(Kind);
    DECLARE_LUA_INTEGER(OpenFrames);
    DECLARE_LUA_INTEGER(DisappearFrames);
    DECLARE_LUA_DWORD(RepresentID);
    DECLARE_LUA_DWORD(NpcTemplateID);
    DECLARE_LUA_DWORD(ScriptID);

#ifdef _SERVER
    DECLARE_LUA_DWORD(OwnerID);
#endif

public:
	int LuaCanDialog(Lua_State* L);
	int LuaIsSelectable(Lua_State* L);
    int LuaCanSearch(Lua_State* L);
	int LuaHaveQuest(Lua_State* L);
    int LuaGetRecipeID(Lua_State* L);

#ifdef _CLIENT
    int LuaGetLootItem(Lua_State* L);
	int LuaGetLootMoney(Lua_State* L);
    int LuaCanLoot(Lua_State* L);
    int LuaDistributeItem(Lua_State* L);
	int LuaGetLooterList(Lua_State* L);
    int LuaCanSit(Lua_State* L);
    int LuaGetRollFrame(Lua_State* L);
#endif

#ifdef _SERVER
	int LuaSetScript(Lua_State* L);
	int LuaGenerateEmptyLootList(Lua_State* L);
	int LuaAddLootItem(Lua_State* L);
	int LuaSetDisappearFrames(Lua_State* L);
    int LuaIsDoorClose(Lua_State* L);
    int LuaOpenDoor(Lua_State* L);
    int LuaCloseDoor(Lua_State* L);

    int LuaSetCustomInteger4(Lua_State* L);
    int LuaGetCustomInteger4(Lua_State* L);
    int LuaSetCustomInteger2(Lua_State* L);
    int LuaGetCustomInteger2(Lua_State* L);
    int LuaSetCustomInteger1(Lua_State* L);
    int LuaGetCustomInteger1(Lua_State* L);

    int LuaSetCustomUnsigned4(Lua_State* L);
    int LuaGetCustomUnsigned4(Lua_State* L);
    int LuaSetCustomUnsigned2(Lua_State* L);
    int LuaGetCustomUnsigned2(Lua_State* L);
    int LuaSetCustomUnsigned1(Lua_State* L);
    int LuaGetCustomUnsigned1(Lua_State* L);

    int LuaSetCustomBoolean(Lua_State* L);
    int LuaGetCustomBoolean(Lua_State* L);

    // ƫ����, �ڼ���Bit(������, ��0��ʼ����), ��ֵΪtrue/false
    int LuaSetCustomBitValue(Lua_State* L);
    int LuaGetCustomBitValue(Lua_State* L);

    int LuaSetCustomString(Lua_State* L);
    int LuaGetCustomString(Lua_State* L);

    int LuaGenerateLootListByNpcMode(Lua_State* L);
#endif	//_SERVER
};
#endif
