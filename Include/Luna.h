/************************************************************************/
/* C Class -> Lua Class ӳ��											*/
/* Copyright : Kingsoft 2006										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2006.02.28	Create												*/
/************************************************************************/
#ifndef _LUNA_H_
#define _LUNA_H_

#include "Engine/KLuaScriptEx.h"

#define DECLARE_LUA_CLASS(__CLASS_NAME__)									        \
	static const char szClassName[];										        \
	static Luna<__CLASS_NAME__>::KLuaData LuaInterface[];					        \
	int LuaGetObj(Lua_State* L)												        \
	{                                                                               \
        int                             nResult       = 0;                          \
        void*                           pvUserData    = NULL;                       \
        Luna<__CLASS_NAME__>::KObjData* pObjData      = NULL;                       \
                                                                                    \
        pvUserData = lua_newuserdata(L, sizeof(Luna<__CLASS_NAME__>::KObjData));    \
        if (!pvUserData)                                                            \
            goto Exit0;                                                             \
                                                                                    \
		pObjData =	(Luna<__CLASS_NAME__>::KObjData*)pvUserData;                    \
                                                                                    \
		pObjData->pObj = this;	                                                    \
                                                                                    \
		luaL_getmetatable(L, __CLASS_NAME__::szClassName);					        \
		if (lua_isnil(L, -1))												        \
			goto Exit0;                                                             \
                                                                                    \
		lua_setmetatable(L, -2);											        \
                                                                                    \
        nResult = 1;                                                                \
Exit0:                                                                              \
		return nResult;														        \
	}

/************************************************************************/

// �������������α���
#define DECLARE_LUA_INTEGER(__VAR_NAME__)									\
	int get##__VAR_NAME__(void) {return m_n##__VAR_NAME__;};				\
	void set##__VAR_NAME__(int nValue)	{m_n##__VAR_NAME__ = nValue;}

// ����������time_t����
#define DECLARE_LUA_TIME(__VAR_NAME__)									    \
	time_t get##__VAR_NAME__(void) {return m_n##__VAR_NAME__;};				\
	void set##__VAR_NAME__(time_t nValue)	{m_n##__VAR_NAME__ = nValue;}

// �����������޷������α���
#define DECLARE_LUA_DWORD(__VAR_NAME__)										\
	DWORD get##__VAR_NAME__(void) {return m_dw##__VAR_NAME__;};				\
	void set##__VAR_NAME__(DWORD dwValue)	{m_dw##__VAR_NAME__ = dwValue;}

// �����������ַ���
#define DECLARE_LUA_STRING(__VAR_NAME__, __STRING_LEN__)					\
	const char* get##__VAR_NAME__(void) {return m_sz##__VAR_NAME__;};		\
	void set##__VAR_NAME__(const char* pszValue)							\
	{strncpy(m_sz##__VAR_NAME__, pszValue, __STRING_LEN__);					\
	 m_sz##__VAR_NAME__[__STRING_LEN__ - 1] = 0;}

// ����������ö���ͱ���
#define DECLARE_LUA_ENUM(__VAR_NAME__)										\
	int get##__VAR_NAME__(void) {return (int)m_e##__VAR_NAME__;};			\
	void set##__VAR_NAME__(int nValue)	{*(int*)&m_e##__VAR_NAME__ = nValue;}

// ���������ò����ͱ���
#define DECLARE_LUA_BOOL(__VAR_NAME__)										\
	int get##__VAR_NAME__(void) {return (int)m_b##__VAR_NAME__;};			\
	void set##__VAR_NAME__(int nValue)	{*(int*)&m_b##__VAR_NAME__ = nValue;}

/************************************************************************/

// �������������α���
#define DECLARE_LUA_STRUCT_INTEGER(__VAR_NAME__, __REAL_NAME__)				\
	int get##__VAR_NAME__(void) {return __REAL_NAME__;};					\
	void set##__VAR_NAME__(int nValue)	{__REAL_NAME__ = nValue;}

// ����������time_t����
#define DECLARE_LUA_STRUCT_TIME(__VAR_NAME__, __REAL_NAME__)				\
	time_t get##__VAR_NAME__(void) {return __REAL_NAME__;};					\
	void set##__VAR_NAME__(time_t nValue)	{__REAL_NAME__ = nValue;}

// �����������޷������α���
#define DECLARE_LUA_STRUCT_DWORD(__VAR_NAME__, __REAL_NAME__)				\
	DWORD get##__VAR_NAME__(void) {return __REAL_NAME__;};					\
	void set##__VAR_NAME__(DWORD dwValue)	{__REAL_NAME__ = dwValue;}

// �����������ַ���
#define DECLARE_LUA_STRUCT_STRING(__VAR_NAME__, __STRING_LEN__, __REAL_NAME__)	\
	const char* get##__VAR_NAME__(void) {return __REAL_NAME__;};			\
	void set##__VAR_NAME__(const char* pszValue)							\
	{strncpy((__REAL_NAME__), pszValue, __STRING_LEN__);					\
	(__REAL_NAME__)[__STRING_LEN__ - 1] = 0;}

// ����������ö���ͱ���
#define DECLARE_LUA_STRUCT_ENUM(__VAR_NAME__, __REAL_NAME__)				\
	int get##__VAR_NAME__(void) {return (int)(__REAL_NAME__);};				\
	void set##__VAR_NAME__(int nValue)	{*(int*)&(__REAL_NAME__) = nValue;	}

// ���������ò����ͱ���
#define DECLARE_LUA_STRUCT_BOOL(__VAR_NAME__, __REAL_NAME__)				\
	int get##__VAR_NAME__(void) {return (int)(__REAL_NAME__);};				\
	void set##__VAR_NAME__(int nValue)	{*(int*)&(__REAL_NAME__) = nValue;}

/************************************************************************/

// ��ʼ���嵼����
#define DEFINE_LUA_CLASS_BEGIN(__CLASS_NAME__)								\
	const char __CLASS_NAME__::szClassName[] = #__CLASS_NAME__;				\
	Luna<__CLASS_NAME__>::KLuaData __CLASS_NAME__::LuaInterface[] = {

/************************************************************************/

// ע�����α���
#define REGISTER_LUA_INTEGER(__CLASS_NAME__, __VAR_NAME__)					\
	{"n"#__VAR_NAME__,															\
	 (Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	 (Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	 ldtInteger},

// ע��time_t����
#define REGISTER_LUA_TIME(__CLASS_NAME__, __VAR_NAME__)					\
	{"n"#__VAR_NAME__,															\
	 (Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	 (Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	 ldtTime},

// ע���޷������α���
#define REGISTER_LUA_DWORD(__CLASS_NAME__, __VAR_NAME__)					\
	{"dw"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtDword},

// ע���ַ�������
#define REGISTER_LUA_STRING(__CLASS_NAME__, __VAR_NAME__)					\
	{"sz"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtString},

// ע��ö���ͱ���
#define REGISTER_LUA_ENUM(__CLASS_NAME__, __VAR_NAME__)						\
	{"n"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtEnum},

// ע�᲼������
#define REGISTER_LUA_BOOL(__CLASS_NAME__, __VAR_NAME__)						\
	{"b"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtBool},

/************************************************************************/

// ע��ֻ�����α���
#define REGISTER_LUA_INTEGER_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"n"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtInteger},

// ע��ֻ��time_t����
#define REGISTER_LUA_TIME_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"n"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtTime},

// ע��ֻ���޷������α���
#define REGISTER_LUA_DWORD_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"dw"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtDword},

// ע��ֻ���ַ�������
#define REGISTER_LUA_STRING_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"sz"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtString},

// ע��ֻ��ö���ͱ���
#define REGISTER_LUA_ENUM_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"n"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtEnum},

// ע��ֻ����������
#define REGISTER_LUA_BOOL_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"b"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtBool},

/************************************************************************/

// ע�ᵼ������
#define REGISTER_LUA_FUNC(__CLASS_NAME__, __FUNC_NAME__)					\
	{#__FUNC_NAME__,														\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::Lua##__FUNC_NAME__),	\
	ldtFunc},

/************************************************************************/

// ���嵼�������
#define DEFINE_LUA_CLASS_END(__CLASS_NAME__)								\
		{0, 0, 0, ldtInvalid}												\
	};

// Lua�������������ͣ����ڲ�ʹ��
enum LUA_DATA_TYPE
{
	ldtInvalid = 0,

	ldtInteger,
	ldtString,
	ldtDword,
    ldtTime,
	ldtEnum,
	ldtBool,
	ldtFunc,
	ldtClass,

	ldtTotal
};

template <typename T> 
class Luna
{
public:
	struct KObjData
	{ 
		T   *pObj;
	};

public:
	typedef void (T::*VoidFunc)(void);

	typedef void (T::*SetIntFunc)(int);
	typedef int (T::*GetIntFunc)(void);

	typedef void (T::*SetTimeFunc)(time_t);
	typedef time_t (T::*GetTimeFunc)(void);

	typedef void (T::*SetStrFunc)(const char*);
	typedef const char* (T::*GetStrFunc)(void);

	typedef void (T::*SetDwordFunc)(DWORD);
	typedef DWORD (T::*GetDwordFunc)(void);

	typedef int (T::*LuaFunc)(lua_State* L);

	typedef struct
	{
		const char*		pszName;		//�����������ߺ���������
		VoidFunc		pSetFunc;		//�����ı�����д�ӿں����򵼳�������ָ��
		VoidFunc		pGetFunc;
		LUA_DATA_TYPE	eDataType;
	}KLuaData;

	static int Register(lua_State *L);

private:
	Luna();
	static int tostring_T(lua_State* L);
	static int index(lua_State* L);
	static int newindex(lua_State* L);
    static int gc(lua_State* L);
};

template <typename U>
int Dispatcher(lua_State* L)
{
    typename Luna<U>::KObjData* pObjData = (typename Luna<U>::KObjData*)lua_touserdata(L, lua_upvalueindex(1));
    typename Luna<U>::KLuaData* pLuaData = (typename Luna<U>::KLuaData*)lua_touserdata(L, lua_upvalueindex(2));

    return ((pObjData->pObj)->*(typename Luna<U>::LuaFunc)(pLuaData->pGetFunc))(L);
}

template <>
int Dispatcher<class KPlayer>(lua_State* L);

template <>
int Dispatcher<class KNpc>(lua_State* L);

template <>
int Dispatcher<class KDoodad>(lua_State* L);

template <typename T>
inline int Luna<T>::Register(lua_State *L) 
{
    int nLuaDataTable       = 0;
    int nUserDataMetatable  = 0;

    // ���д���������(T)��ע������е��������͵�������
    lua_newtable(L);

    nLuaDataTable = lua_gettop(L);

    for (KLuaData* pLuaData = T::LuaInterface; pLuaData->pszName; pLuaData++) 
    {
        lua_pushstring(L, pLuaData->pszName);
        lua_pushlightuserdata(L, pLuaData);
        lua_settable(L, nLuaDataTable);
    }

    lua_pushstring(L, T::szClassName);
    lua_pushvalue(L, nLuaDataTable);
    lua_settable(L, LUA_GLOBALSINDEX);

    // ����Metatable:
    // ��C����ָ����Ϊuserdataѹջ��ʱ��(��LuaGetObj),�������Metatable��Ϊuserdata��Ԫ��
    luaL_newmetatable(L, T::szClassName);

    nUserDataMetatable = lua_gettop(L);

    // ����Get����
    lua_pushliteral(L, "__index");
    lua_pushcfunction(L, index);
    lua_settable(L, nUserDataMetatable);

    // ����Set����
    lua_pushliteral(L, "__newindex");
    lua_pushcfunction(L, newindex);
    lua_settable(L, nUserDataMetatable);

    // �����ַ���ת������
    lua_pushliteral(L, "__tostring");
    lua_pushcfunction(L, tostring_T);
    lua_settable(L, nUserDataMetatable);

    // ����gc����
    lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, gc);
    lua_settable(L, nUserDataMetatable);

    lua_pop(L, 2);

    return 0;
}

template <typename T> 
inline int Luna<T>::tostring_T(lua_State* L)
{
    KObjData* pObjData = (KObjData*)lua_touserdata(L, 1);

    if (pObjData)
    {
        lua_pushfstring(L, "%s:%p",	T::szClassName, pObjData->pObj);
    }
    else
    {
        lua_pushstring(L, "nill");
    }

    return 1;
}

template <typename T> 
inline int Luna<T>::index(lua_State* L)
{
    int         nResult     = 0;
    int         nRetCode    = 0;
    KObjData*   pObjData    = NULL;
    const char* pszKey      = NULL;
    const char* pszStr      = NULL;
    KLuaData*   pLuaData    = NULL;
    T*          pThis       = NULL;

    nRetCode = Lua_GetTopIndex(L);
    KGLOG_PROCESS_ERROR(nRetCode == 2);

    pObjData    = (KObjData*)lua_touserdata(L, 1);
    pszKey      = lua_tostring(L, 2);

    KGLOG_PROCESS_ERROR(pObjData && pszKey);

    pThis = pObjData->pObj;
    KGLOG_PROCESS_ERROR(pThis);

    // �ҵ�����������(��Register�����ɵ�)
    lua_pushstring(L, T::szClassName);
    lua_gettable(L, LUA_GLOBALSINDEX);

    lua_pushstring(L, pszKey);
    lua_gettable(L, -2);

    pLuaData = (KLuaData*)lua_touserdata(L, -1);
    if (pLuaData == NULL)
    {
        KGLogPrintf(KGLOG_ERR, "[LUA] Invalid index [%s::%s]\n", T::szClassName, pszKey);
        goto Exit0;
    }

    KGLOG_PROCESS_ERROR(pLuaData->pGetFunc);

    lua_settop(L, 2);

    switch (pLuaData->eDataType)
    {
    case ldtInteger:
        nRetCode = (pThis->*(GetIntFunc)(pLuaData->pGetFunc))();
        lua_pushnumber(L, nRetCode);
        break;

    case ldtTime:
        nRetCode = (int)(pThis->*(GetTimeFunc)(pLuaData->pGetFunc))();
        lua_pushnumber(L, nRetCode);
        break;

    case ldtString:
        pszStr = (pThis->*(GetStrFunc)(pLuaData->pGetFunc))();
        KGLOG_PROCESS_ERROR(pszStr);
        lua_pushstring(L, pszStr);
        break;

    case ldtEnum:
        nRetCode = (pThis->*(GetIntFunc)(pLuaData->pGetFunc))();
        lua_pushnumber(L, nRetCode);
        break;

    case ldtDword:
        // ����ǿתΪint������DWORD����Ϊ�˱���CPU���������ʱ������������
        // ��CPU���㵥Ԫ�п�����real4ģʽ�����У���ʱ������о��ȶ�ʧ����
        // �緢������������ɼ��CTRL�Ĵ���8��9��λ�Ƿ�Ϊ00��
        // ��ϸ��Ϣ�ɲο�MASM32������������ĵ�������ѯ����
        nRetCode = (int)(pThis->*(GetDwordFunc)(pLuaData->pGetFunc))();
        lua_pushnumber(L, nRetCode);
        break;

    case ldtBool:
        nRetCode = (pThis->*(GetIntFunc)(pLuaData->pGetFunc))();
        lua_pushboolean(L, nRetCode);
        break;

    case ldtFunc:
        lua_pushlightuserdata(L, pObjData);
        lua_pushlightuserdata(L, pLuaData);
        lua_pushcclosure(L, Dispatcher<T>, 2);
        break;

    default:
        goto Exit0;
    }

    nResult = 1;
Exit0:
    return nResult;
}

template <typename T> 
inline int Luna<T>::newindex(lua_State* L)
{
    int         nRetCode    = 0;
    KObjData*   pObjData    = NULL;
    const char* pszKey      = NULL;
    const char* pszStr      = NULL;
    KLuaData*   pLuaData    = NULL;
    T*          pThis       = NULL;

    nRetCode = Lua_GetTopIndex(L);
    KGLOG_PROCESS_ERROR(nRetCode == 3);

    pObjData    = (KObjData*)lua_touserdata(L, 1);
    pszKey      = lua_tostring(L, 2);

    KGLOG_PROCESS_ERROR(pObjData && pszKey);

    pThis = pObjData->pObj;
    KGLOG_PROCESS_ERROR(pThis);

    // �ҵ�����������(��Register�����ɵ�)
    lua_pushstring(L, T::szClassName);
    lua_gettable(L, LUA_GLOBALSINDEX);

    lua_pushstring(L, pszKey);
    lua_gettable(L, -2);

    pLuaData = (KLuaData*)lua_touserdata(L, -1);
    if (pLuaData == NULL)
    {
        KGLogPrintf(KGLOG_ERR, "[LUA] Invalid newindex [%s::%s]\n", T::szClassName, pszKey);
        goto Exit0;
    }

    KGLOG_PROCESS_ERROR(pLuaData->pSetFunc);

    lua_settop(L, 3);

    switch (pLuaData->eDataType)
    {
    case ldtInteger:
        nRetCode = (int)lua_tonumber(L, 3);
        (pThis->*(SetIntFunc)(pLuaData->pSetFunc))(nRetCode);
        break;

    case ldtTime:
        nRetCode = (int)lua_tonumber(L, 3);
        (pThis->*(SetTimeFunc)(pLuaData->pSetFunc))((time_t)nRetCode);
        break;

    case ldtString:
        pszStr = lua_tostring(L, 3);
        KGLOG_PROCESS_ERROR(pszStr);
        (pThis->*(SetStrFunc)(pLuaData->pSetFunc))(pszStr);
        break;

    case ldtEnum:
        nRetCode = (int)lua_tonumber(L, 3);
        (pThis->*(SetIntFunc)(pLuaData->pSetFunc))(nRetCode);
        break;

    case ldtBool:
        if (lua_isboolean(L, 3))
        {
            nRetCode = (int)lua_toboolean(L, 3);
        }
        else
        {
            nRetCode = (int)lua_tonumber(L, 3);
        }
        (pThis->*(SetIntFunc)(pLuaData->pSetFunc))(nRetCode);
        break;

    case ldtDword:
        // NOTE:
        // ����ǿתΪint������DWORD����Ϊ�˱���CPU���������ʱ������������
        // ��CPU���㵥Ԫ�п�����real4ģʽ�����У���ʱ������о��ȶ�ʧ����
        // �緢������������ɼ��CTRL�Ĵ���8��9��λ�Ƿ�Ϊ00��
        // ��ϸ��Ϣ�ɲο�MASM32������������ĵ�������ѯ����
        nRetCode = (int)lua_tonumber(L, 3);
        (pThis->*(SetDwordFunc)(pLuaData->pSetFunc))(nRetCode);
        break;

    default:
        break;
    }

Exit0:
    return 0;
}

template <typename T> 
inline int Luna<T>::gc(lua_State* L)
{
    int         nRetCode    = 0;
    KObjData*   pObjData    = NULL;
    KLuaData*   pLuaData    = NULL;
    T*          pThis       = NULL;

    nRetCode = Lua_GetTopIndex(L);
    KGLOG_PROCESS_ERROR(nRetCode == 1);

    pObjData    = (KObjData*)lua_touserdata(L, 1);
    KGLOG_PROCESS_ERROR(pObjData);

    pThis = pObjData->pObj;
    KGLOG_PROCESS_ERROR(pThis);

    //printf("__gc: %s:0x%p\n", T::szClassName, pThis);

Exit0:
    return 0;
}

#endif	//_LUNA_H_
