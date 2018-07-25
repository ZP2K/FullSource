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
#include "KDoodad.h"
//#include "KDoodadTemplateList.h"

#define MAX_DOODAD_OBSTACLE 64

class KLootList;

enum DOODAD_STATE
{
	dsInvalid = 0,				//��Ч״̬

	//�ܿ�״̬
	dsIdle,						//����״̬
	dsBeHit,					//�������򱻻�
	dsAfterHit,					//�򿪺�򱻻�����

	//����״̬
	dsRemove,

	dsTotal
};

class KCharacter;

class KDoodad : public KSceneObject
{
//����װ���������
public:
	DWORD					m_dwTemplateID;					//ģ����

	char					m_szName[_NAME_LEN];			//����
	DOODAD_KIND				m_eKind;						//����
	int						m_nMaxLife;						//�������ֵ
	int						m_nOpenFrames;					//��֡��
	int						m_nDisappearFrames;				//��ʧ֡��
	int						m_nCurrentLife;					//��ǰ����ֵ
	DOODAD_STATE			m_eState;						//״̬
	DWORD					m_dwNpcTemplateID;
	DWORD					m_dwRepresentID;				//Doodad�ı���ID
	BOOL					m_bLooted;						//ʬ���Ƿ�ʰȡ��ı��

	struct KDoodadInfo
	{
		unsigned		m_bSetBlockCharacter:	1;
		unsigned		m_bBlockCharacter	:	1;			//�ϰ���Ϣ
		unsigned		m_bSetPutObj		:	1;
		unsigned		m_bPutObj			:	1;			//�ɰڷ�
		unsigned		m_bSetSafe			:	1;
		unsigned		m_bSafe				:	1;			//�Ƿ��ǰ�ȫ��
		unsigned		m_bSetStall			:	1;
		unsigned		m_bStall			:	1;			//�Ƿ�ɰ�̯
		unsigned		m_bSetCellType		:	1;
		unsigned		m_CellType			:	4;			//�ر�����
		unsigned		m_bSetPassWidth		:	1;
		unsigned		m_PassWidth			:	3;			//ͨ��������
		unsigned		m_Reserved			:	15;			//δ��
	}m_DoodadInfo;

	DWORD					m_dwScriptID;					//�󶨽ű�ID
	//KLootList*				m_pLootList;					//ʰȡ�б�

	KCell*					m_PObstacleCell[MAX_DOODAD_OBSTACLE];	//������ص�Cell�ϰ�,���ָ�벻��ָ���ϰ�cell������ָ���ϰ���ǰһ��cell

public:
	KDoodad();
	~KDoodad();

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL Activate(void);

	BOOL Save(KDOODAD_DATA* pDoodadData);
	BOOL Load(const KDOODAD_DATA& crDoodadData);

	//----- Doodad���ϰ� ---------------------------------------->
	BOOL ApplyObstacle();
	BOOL RemoveObstacle();
	BOOL UpdateObstacle();
	//<-----------------------------------------------------------

	//----- Doodad״̬ ------------------------------------------>
	void ChangeState(DOODAD_STATE eState);
private:
	void DoIdle();
	void DoBeHit();
	void DoAfterHit();

public:
	//<-----------------------------------------------------------

	//Ӧ������ϵ�����Ч����ÿ��Cell��ֻ�е�һ��Doodad����������
	void ApplyEffect(void);
	//��Ӧ������ϵ�����Ч��
	void UnApplyEffect(void);

	void DoPick(KCharacter* pCharacter);

public:
	//Lua�ű��ӿ�
	DECLARE_LUA_CLASS(KDoodad);

	int LuaGetLootMode(Lua_State* L);

	int LuaGetLootItem(Lua_State* L);
	int LuaGetLootMoney(Lua_State* L);
	int LuaCanDialog(Lua_State* L);
	int LuaIsSelectable(Lua_State* L);


	int LuaLeaderDistributeItem(Lua_State* L);

	int LuaGetLooterList(Lua_State* L);


	DECLARE_LUA_DWORD(TemplateID);
	DECLARE_LUA_STRING(Name, _NAME_LEN);
	DECLARE_LUA_ENUM(Kind);
	DECLARE_LUA_INTEGER(MaxLife);
	DECLARE_LUA_INTEGER(OpenFrames);
	DECLARE_LUA_INTEGER(DisappearFrames);
	DECLARE_LUA_DWORD(RepresentID);
	DECLARE_LUA_DWORD(NpcTemplateID);

	DECLARE_LUA_INTEGER(CurrentLife);
	//DECLARE_LUA_DWORD(ClassID);

	DECLARE_LUA_DWORD(ScriptID);

	DECLARE_LUA_STRUCT_INTEGER(IsSetBlockCharacter, m_DoodadInfo.m_bSetBlockCharacter);
	DECLARE_LUA_STRUCT_INTEGER(IsBlockCharacter,	m_DoodadInfo.m_bBlockCharacter);
	DECLARE_LUA_STRUCT_INTEGER(IsSetPutObj,			m_DoodadInfo.m_bSetPutObj);
	DECLARE_LUA_STRUCT_INTEGER(IsPutObj,			m_DoodadInfo.m_bPutObj);
	DECLARE_LUA_STRUCT_INTEGER(IsSetSafe,			m_DoodadInfo.m_bSetSafe);
	DECLARE_LUA_STRUCT_INTEGER(IsSafe,				m_DoodadInfo.m_bSafe);
	DECLARE_LUA_STRUCT_INTEGER(IsSetStall,			m_DoodadInfo.m_bSetStall);
	DECLARE_LUA_STRUCT_INTEGER(IsStall,				m_DoodadInfo.m_bStall);
	DECLARE_LUA_STRUCT_INTEGER(IsSetCellType,		m_DoodadInfo.m_bSetCellType);
	DECLARE_LUA_STRUCT_INTEGER(CellType,			m_DoodadInfo.m_CellType);
	DECLARE_LUA_STRUCT_INTEGER(IsSetPassWidth,		m_DoodadInfo.m_bSetPassWidth);
	DECLARE_LUA_STRUCT_INTEGER(PassWidth,			m_DoodadInfo.m_PassWidth);


public:
	static struct KUpdateAllDoodadFunc UpdateAllDoodadFunc;
};

struct KUpdateAllDoodadFunc
{
	KUpdateAllDoodadFunc(){}

	BOOL operator()(KDoodad* pDoodad)
	{
		BOOL bRetCode = FALSE;

		bRetCode = pDoodad->UpdateObstacle();
		KGLOG_CHECK_ERROR(bRetCode);

		return TRUE;
	}
};

#endif