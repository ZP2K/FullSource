/************************************************************************/
/* Copyright : Kingsoft 2008										    */
/* Author	 : Chen Jie 											    */
/* History	 :															*/
/*		2008.08.12	Create												*/
/************************************************************************/

#ifndef _KBUFF_H_
#define _KBUFF_H_

#include "Luna.h"

class   KCharacter;
class   KAttribute;
struct  KBUFF_LIST_NODE;

#define MAX_BUFF_REICPE_COUNT           1

class KBuff
{
public:
    DWORD           m_dwSerialNumber;

    DWORD           m_dwID;
    int             m_nLevel;

    int             m_nKindType;
    int             m_nFunctionType;    // ���������Buff������Ч��������
    int             m_nBuffType;        // ������������Buff(��һЩ�¼���������ĳ���Buff, �����뿪����״̬ʱ,�ͻ����������Buff)

    int             m_nAppendType;      // ���������Buff����,ȡ��,����
    int             m_nDetachType;      // �����������ɢBuff
    int             m_nDecayType;       // ���������Ч���ݼ�

    int             m_nRepresentPos;    // ��Ч���ֵ�λ��,�ǲ��ڽŵ��µ�,����ͷ�ϵ�,���ǲ��������ϵ�
    DWORD           m_dwRepresentID;

    int             m_nIntensity;       // ���ڽ綨����Buff��ȡ��

    BOOL            m_bIsStackable;     // �Ƿ���Ե���
    int             m_nMaxStackNum;     // ����������

    int             m_nActiveCount;     // ����
    int             m_nIntervalFrame;   // ÿһ���ļ��֡��
    int             m_nDamageAddPercent;  // �����޸��˺�(����)

    BOOL            m_bExclude;
    BOOL            m_bGlobalExclude;   // ȫ�ֳ�ͻ��Ϊfalse���BUFFֻ��ʩ�����жϳ�ͻ
    BOOL            m_bUniqueTarget;
    BOOL            m_bCanCancel;       // �Ƿ�������ʹ���Ҽ�������ɢ
    BOOL            m_bIsCountable;     // �Ƿ�Ϊ����BUFF
    BOOL            m_bSave;
    BOOL            m_bIsOffLineClock;  // �Ƿ����߼�ʱ

    char            m_szScript[MAX_SCIRPT_NAME_LEN];

    KAttribute*     m_pBeginAttribList;   // �ɻع�
    KAttribute*     m_pActiveAttribList;  // ���ɻع�
    KAttribute*     m_pEndTimeAttribList; // ���ɻع�

#ifdef _SERVER
    int             m_nPersistMoveStateMask;
    int             m_nPersistManaShield;
    int             m_nPersistFight;
    int             m_nPersistHorse;
    int             m_nPersistTerrain;
    BOOL            m_bPersistFormationLeader;
    DWORD           m_dwMapBanMask;
#endif

    BOOL Apply(KCharacter* pCharacter, KBUFF_LIST_NODE& rBuffListNode);
    BOOL UnApply(KCharacter* pCharacter, KBUFF_LIST_NODE& rBuffListNode);
#ifdef _SERVER
    BOOL Active(KCharacter* pCharacter, KBUFF_LIST_NODE& rBuffListNode);
    BOOL EndTime(KCharacter* pCharacter, KBUFF_LIST_NODE& rBuffListNode);
#endif

    DECLARE_LUA_CLASS(KBuff);
    DECLARE_LUA_STRUCT_INTEGER(Level, m_nLevel);
    DECLARE_LUA_STRUCT_INTEGER(Count, m_nActiveCount);
    DECLARE_LUA_STRUCT_INTEGER(Interval, m_nIntervalFrame);
    int LuaGetAttribute(Lua_State* L);

private:
#ifdef _SERVER
    BOOL ApplyAttributeNotRollBack(KCharacter* pCharacter, KBUFF_LIST_NODE& rBuffListNode, KAttribute* pAttribute);
#endif
};

struct KBUFF_RECIPE_KEY
{
    DWORD   dwID;
    int     nLevel;
    DWORD   dwRecipeKey[MAX_BUFF_REICPE_COUNT];
};

bool operator <  (const KBUFF_RECIPE_KEY& lhs, const KBUFF_RECIPE_KEY& rhs);
bool operator == (const KBUFF_RECIPE_KEY& lhs, const KBUFF_RECIPE_KEY& rhs);

struct KBUFF_RECIPE_INFO
{
    DWORD       dwID;
    int         nLevel;

    int         nIntensityAdd;
    int         nCountAdd;
    int         nIntervalFrameAdd;
    int         nDamageAddPercent;

    KAttribute* pBeginAttributeListAdd;
    KAttribute* pActiveAttributeListAdd;
    KAttribute* pEndTimeAttributeListAdd;
};

class KBuffRecipePointer 
{
public:
    KBuffRecipePointer();
    KBuff*              SetRecipeKey(const KBUFF_RECIPE_KEY& crRecipeKey);
    KBUFF_RECIPE_KEY&   GetRecipeKey();
    void                ClearRecipeKey();
    KBuff*              GetPointer();

private:
    KBuff*              m_pBuffInfo;
    DWORD               m_dwSerialNumber;
    KBUFF_RECIPE_KEY    m_BuffRecipeKey;
};

#endif //_KBUFF_H_
