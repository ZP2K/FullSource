/************************************************************************/
/* NPC			                                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.30	Create												*/
/************************************************************************/
#ifndef _KNPC_H_
#define _KNPC_H_

//#include "KShop.h"
#include "KCharacter.h"

enum  KNPC_SPECIES
{
    ensInvalid = 0, 
    ensHumanoid,    // ���͹�
    ensBeast,       // Ұ��
    ensMechanical,  // ľ��
    ensUndead,      // ��ʬ, ��ʬ����
    ensGhost,       // ����
    ensPlant,       // ֲ��
    ensLegendary,   // ���ޣ�ʲô�������׻�����ȸ������ȵ�
    ensCritter,     // С����,�����
    ensOther,       // δ����
    ensTotal
};


class KNpcTemplate;

class KNpc : public KCharacter
{
public:
    int             m_nIntensity;       // ǿ��
	DWORD			m_dwScriptID;		// �󶨽ű�ID		
//	KShop*          m_pShop;            // �󶨵��̵�
	DWORD			m_dwTemplateID;		// ģ����
    int             m_nAlarmRange;      // ������Χ
    int             m_nReviveTime;      // ����ʱ�䣬��λ��
	int				m_nDisappearFrames;	//��ʧ֡��
    KNPC_SPECIES    m_eSpecies;
	DWORD			m_dwRepresentID;

public:
	KNpc(void);
	~KNpc(void);

	BOOL Init();
	BOOL UnInit();
	BOOL Activate(void);

#ifdef _CLIENT
    void SetDisplayData(DWORD dwTemplateID, int nLevel, DWORD dwRepresentID);
#endif

	BOOL Save(KNPC_DATA* pNpcData);
	BOOL Load(const KNPC_DATA& crNpcData);

	virtual BOOL ChangeRegion(KRegion* pRegion);
	virtual BOOL ChangeCell(KCell* pCell);

	int GetNpcRelation(KNpc* pTarget);
	int GetPlayerRelation(KPlayer* pTarget);


protected:
	//����ѭ����ָ����������˵��ָ���б�����NPC��˵�ǵ���NPCAI
	virtual BOOL GetCommand(void);

#if defined(_CLIENT)
	int m_nNextActionFrame;
#endif	//_CLIENT

public:
    BOOL CallDeathScript(KCharacter* pKiller);

public:
	DECLARE_LUA_CLASS(KNpc);

    DECLARE_LUA_INTEGER(Intensity);
	DECLARE_LUA_DWORD(ScriptID);
	DECLARE_LUA_DWORD(TemplateID);
    DECLARE_LUA_INTEGER(AlarmRange);
    DECLARE_LUA_INTEGER(ReviveTime);

	int LuaGetQuestString(Lua_State* L);
	int LuaShareQuest(Lua_State* L);

};
#endif	//_KNPC_H_