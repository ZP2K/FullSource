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

//ӳ����غ궨��

//����������Ҫ�����������ߺ�����Luaʹ��
#define DECLARE_LUA_CLASS(__CLASS_NAME__)									\
	static const char szClassName[];										\
	static Luna<__CLASS_NAME__>::KLuaData LuaInterface[];					\
	int LuaGetObj(Lua_State* L)												\
	{																		\
		Luna<__CLASS_NAME__>::KObjData *pObjData =							\
			static_cast<Luna<__CLASS_NAME__>::KObjData*>					\
			(lua_newuserdata(L, sizeof(Luna<__CLASS_NAME__>::KObjData)));	\
		if (pObjData == NULL)												\
			return 0;														\
		pObjData->pObj = this;												\
		luaL_getmetatable(L, __CLASS_NAME__::szClassName);					\
		if (lua_isnil(L, -1))												\
			return 0;														\
		lua_setmetatable(L, -2);											\
		return 1;															\
	}

/************************************************************************/

//�������������α���
#define DECLARE_LUA_INTEGER(__VAR_NAME__)									\
	int get##__VAR_NAME__(void) {return m_n##__VAR_NAME__;};				\
	void set##__VAR_NAME__(int nValue)	{m_n##__VAR_NAME__ = nValue;}

//�����������޷������α���
#define DECLARE_LUA_DWORD(__VAR_NAME__)										\
	DWORD get##__VAR_NAME__(void) {return m_dw##__VAR_NAME__;};				\
	void set##__VAR_NAME__(DWORD dwValue)	{m_dw##__VAR_NAME__ = dwValue;}

//�����������ַ���
#define DECLARE_LUA_STRING(__VAR_NAME__, __STRING_LEN__)					\
	const char* get##__VAR_NAME__(void) {return m_sz##__VAR_NAME__;};		\
	void set##__VAR_NAME__(const char* pszValue)							\
	{strncpy(m_sz##__VAR_NAME__, pszValue, __STRING_LEN__);					\
	 m_sz##__VAR_NAME__[__STRING_LEN__ - 1] = 0;}

//����������ö���ͱ���
#define DECLARE_LUA_ENUM(__VAR_NAME__)										\
	int get##__VAR_NAME__(void) {return (int)m_e##__VAR_NAME__;};			\
	void set##__VAR_NAME__(int nValue)	{*(int*)&m_e##__VAR_NAME__ = nValue;}

//���������ò����ͱ���
#define DECLARE_LUA_BOOL(__VAR_NAME__)										\
	int get##__VAR_NAME__(void) {return (int)m_b##__VAR_NAME__;};			\
	void set##__VAR_NAME__(int nValue)	{*(int*)&m_b##__VAR_NAME__ = nValue;}

/************************************************************************/

//�������������α���
#define DECLARE_LUA_STRUCT_INTEGER(__VAR_NAME__, __REAL_NAME__)				\
	int get##__VAR_NAME__(void) {return __REAL_NAME__;};					\
	void set##__VAR_NAME__(int nValue)	{__REAL_NAME__ = nValue;}

//�����������޷������α���
#define DECLARE_LUA_STRUCT_DWORD(__VAR_NAME__, __REAL_NAME__)				\
	DWORD get##__VAR_NAME__(void) {return __REAL_NAME__;};					\
	void set##__VAR_NAME__(DWORD dwValue)	{__REAL_NAME__ = dwValue;}

//�����������ַ���
#define DECLARE_LUA_STRUCT_STRING(__VAR_NAME__, __STRING_LEN__, __REAL_NAME__)	\
	const char* get##__VAR_NAME__(void) {return __REAL_NAME__;};			\
	void set##__VAR_NAME__(const char* pszValue)							\
	{strncpy((__REAL_NAME__), pszValue, __STRING_LEN__);					\
	(__REAL_NAME__)[__STRING_LEN__ - 1] = 0;}

//����������ö���ͱ���
#define DECLARE_LUA_STRUCT_ENUM(__VAR_NAME__, __REAL_NAME__)				\
	int get##__VAR_NAME__(void) {return (int)(__REAL_NAME__);};				\
	void set##__VAR_NAME__(int nValue)	{*(int*)&(__REAL_NAME__) = nValue;	}

//���������ò����ͱ���
#define DECLARE_LUA_STRUCT_BOOL(__VAR_NAME__, __REAL_NAME__)				\
	int get##__VAR_NAME__(void) {return (int)(__REAL_NAME__);};				\
	void set##__VAR_NAME__(int nValue)	{*(int*)&(__REAL_NAME__) = nValue;}

/************************************************************************/

//��ʼ���嵼����
#define DEFINE_LUA_CLASS_BEGIN(__CLASS_NAME__)								\
	const char __CLASS_NAME__::szClassName[] = #__CLASS_NAME__;				\
	Luna<__CLASS_NAME__>::KLuaData __CLASS_NAME__::LuaInterface[] = {

/************************************************************************/

//ע�����α���
#define REGISTER_LUA_INTEGER(__CLASS_NAME__, __VAR_NAME__)					\
	{"n"#__VAR_NAME__,															\
	 (Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	 (Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	 ldtInteger},

//ע���޷������α���
#define REGISTER_LUA_DWORD(__CLASS_NAME__, __VAR_NAME__)					\
	{"dw"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtDword},

//ע���ַ�������
#define REGISTER_LUA_STRING(__CLASS_NAME__, __VAR_NAME__)					\
	{"sz"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtString},

//ע��ö���ͱ���
#define REGISTER_LUA_ENUM(__CLASS_NAME__, __VAR_NAME__)						\
	{"n"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtEnum},

//ע�᲼������
#define REGISTER_LUA_BOOL(__CLASS_NAME__, __VAR_NAME__)						\
	{"b"#__VAR_NAME__,															\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::set##__VAR_NAME__),	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtBool},

/************************************************************************/

//ע��ֻ�����α���
#define REGISTER_LUA_INTEGER_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"n"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtInteger},

//ע��ֻ���޷������α���
#define REGISTER_LUA_DWORD_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"dw"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtDword},

//ע��ֻ���ַ�������
#define REGISTER_LUA_STRING_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"sz"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtString},

//ע��ֻ��ö���ͱ���
#define REGISTER_LUA_ENUM_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"n"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtEnum},

//ע��ֻ����������
#define REGISTER_LUA_BOOL_READONLY(__CLASS_NAME__, __VAR_NAME__)			\
	{"b"#__VAR_NAME__,															\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::get##__VAR_NAME__),	\
	ldtBool},

/************************************************************************/

//ע�ᵼ������
#define REGISTER_LUA_FUNC(__CLASS_NAME__, __FUNC_NAME__)					\
	{#__FUNC_NAME__,														\
	NULL,																	\
	(Luna<__CLASS_NAME__>::VoidFunc)(&__CLASS_NAME__::Lua##__FUNC_NAME__),	\
	ldtFunc},

/************************************************************************/

//���嵼�������
#define DEFINE_LUA_CLASS_END(__CLASS_NAME__)								\
		{0, 0, 0, ldtInvalid}												\
	};

//Lua�������������ͣ����ڲ�ʹ��
enum LUA_DATA_TYPE
{
	ldtInvalid = 0,

	ldtInteger,
	ldtString,
	ldtDword,
	ldtEnum,
	ldtBool,
	ldtFunc,
	ldtClass,

	ldtTotal
};

//����ģ����
template <typename T> 
class Luna
{
public:
	//ע����Ķ���ָ���ID
	struct KObjData
	{ 
		T		*pObj;
	};
public:
	typedef void (T::*VoidFunc)(void);

	typedef void (T::*SetIntFunc)(int);
	typedef int (T::*GetIntFunc)(void);

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

	static int Register(lua_State *L) 
	{
		//�����������ݱ���ҪΪ������Lua�ڽ����ַ���hash���ƣ�����Ҳ�����Լ�ʵ��
		//����Key : �����������ƣ�Value : KLuaDataָ��
		lua_newtable(L);
		int nLuaData = lua_gettop(L);

		//����Metatable���Ա�������صĲ�����
		luaL_newmetatable(L, T::szClassName);
		int metatable = lua_gettop(L);

		//���������ݱ����ȫ�ֱ��У��Ա��Ժ����
		lua_pushstring(L, T::szClassName);
		lua_pushvalue(L, nLuaData);
		lua_settable(L, LUA_GLOBALSINDEX);

		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, nLuaData);
		lua_settable(L, metatable);  // hide metatable from Lua getmetatable()

		//����GetTable����
		lua_pushliteral(L, "__index");
		lua_pushcfunction(L, index);
		lua_settable(L, metatable);

		//����SetTable����
		lua_pushliteral(L, "__newindex");
		lua_pushcfunction(L, newindex);
		lua_settable(L, metatable);

		//�����ַ���ת������
		lua_pushliteral(L, "__tostring");
		lua_pushcfunction(L, tostring_T);
		lua_settable(L, metatable);

		//����м���������Ҫע�������
		for (KLuaData* pLuaData = T::LuaInterface; pLuaData->pszName; pLuaData++) 
		{
			/* edited by Snaily: shouldn't it be const RegType *l ... ? */
			lua_pushstring(L, pLuaData->pszName);
			lua_pushlightuserdata(L, pLuaData);
			lua_settable(L, nLuaData);
		}

		lua_pop(L, 2);  // drop metatable and method table

		return 0;
	}

private:
	//���ع��캯��
	Luna();

	//�������������������Խ�static�ĵ��ã�ת������thisָ������Ա��������
	static int Dispatcher(lua_State* L)
	{
		KObjData* pObjData = (KObjData*)lua_touserdata(L, lua_upvalueindex(1));
		KLuaData* pLuaData = (KLuaData*)lua_touserdata(L, lua_upvalueindex(2));
		
		return ((pObjData->pObj)->*(LuaFunc)(pLuaData->pGetFunc))(L);
	}

	//�����ַ���ת������
	static int tostring_T (lua_State* L) 
	{
		KObjData *pObjData = static_cast<KObjData*>(lua_touserdata(L, 1));
		lua_pushfstring(L, "Ptr:%p ClassName:%s",	pObjData->pObj, T::szClassName);
		
		return 1;
	}

	//����GetTable����
	static int index(lua_State* L)
	{
		//��ȡSelfָ�������
		KObjData* pObjData = (KObjData*)lua_touserdata(L, 1);
		//��ȡKey�ַ���
		const char* pszKey = lua_tostring(L, 2);

		if (pObjData == NULL || pObjData->pObj == NULL || pszKey == NULL)
		{
			printf("[LUA] Error index\n");
			return 0;
		}

		//���ҵ������ݱ�
		lua_pushstring(L, T::szClassName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		//�ڵ������ݱ��в���Key��Ӧ��KLuaDataָ��
		lua_pushstring(L, pszKey);
		lua_gettable(L, -2);
		KLuaData* pLuaData = (KLuaData*)lua_touserdata(L, -1);
		if (pLuaData == NULL || pLuaData->pGetFunc == NULL)
		{
			printf("[LUA] Error index, classname = %s, key = %s\n", 
			T::szClassName, pszKey);
			return 0;
		}

		lua_settop(L, 2);

		switch (pLuaData->eDataType)
		{
		case ldtInteger:
			{
				int nResult = ((pObjData->pObj)->*(GetIntFunc)(pLuaData->pGetFunc))();
				lua_pushnumber(L, nResult);
				return 1;
			}
		case ldtString:
			{
				const char* pszResult = ((pObjData->pObj)->*(GetStrFunc)(pLuaData->pGetFunc))();
				lua_pushstring(L, pszResult);
				return 1;
			}
		case ldtEnum:
			{
				int nResult = ((pObjData->pObj)->*(GetIntFunc)(pLuaData->pGetFunc))();
				lua_pushnumber(L, nResult);
				return 1;
			}
		case ldtDword:
			{
				// NOTE:
				// ����ǿתΪint������DWORD����Ϊ�˱���CPU���������ʱ������������
				// ��CPU���㵥Ԫ�п�����real4ģʽ�����У���ʱ������о��ȶ�ʧ����
				// �緢������������ɼ��CTRL�Ĵ���8��9��λ�Ƿ�Ϊ00��
				// ��ϸ��Ϣ�ɲο�MASM32������������ĵ�������ѯ����
				int nResult = (int)((pObjData->pObj)->*(GetDwordFunc)(pLuaData->pGetFunc))();
				lua_pushnumber(L, nResult);
				return 1;
			}
		case ldtBool:
			{
				int nResult = ((pObjData->pObj)->*(GetIntFunc)(pLuaData->pGetFunc))();
				lua_pushboolean(L, nResult);
				return 1;
			}
		case ldtFunc:
			{
				lua_pushlightuserdata(L, pObjData);
				lua_pushlightuserdata(L, pLuaData);
				lua_pushcclosure(L, Dispatcher, 2);
				return 1;
			}
		}

		return 0;
	}

	//����SetTable����
	static int newindex(lua_State* L)
	{
		KObjData* pObjData = (KObjData*)lua_touserdata(L, 1);
		const char* pszKey = lua_tostring(L, 2);

		if (pObjData == NULL || pObjData->pObj == NULL || pszKey == NULL)
		{
			printf("[LUA]Error newindex\n");
			return 0;
		}

		lua_pushstring(L, T::szClassName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		lua_pushstring(L, pszKey);
		lua_gettable(L, -2);
		KLuaData* pLuaData = (KLuaData*)lua_touserdata(L, -1);
		if (pLuaData == NULL || pLuaData->pSetFunc == NULL)
		{
			printf("[LUA]Error newindex, classname = %s, key = %s\n", 
				T::szClassName, pszKey);
			return 0;
		}

		lua_settop(L, 3);

		switch (pLuaData->eDataType)
		{
		case ldtInteger:
			{
				int nValue = (int)lua_tonumber(L, 3);
				((pObjData->pObj)->*(SetIntFunc)(pLuaData->pSetFunc))(nValue);
				return 0;
			}
		case ldtString:
			{
				const char* pszValue = lua_tostring(L, 3);
				((pObjData->pObj)->*(SetStrFunc)(pLuaData->pSetFunc))(pszValue);
				return 0;
			}
		case ldtEnum:
			{
				int nValue = (int)lua_tonumber(L, 3);
				((pObjData->pObj)->*(SetIntFunc)(pLuaData->pSetFunc))(nValue);
				return 0;
			}
		case ldtBool:
			{
				int nValue = (int)lua_tonumber(L, 3);
				((pObjData->pObj)->*(SetIntFunc)(pLuaData->pSetFunc))(nValue);
				return 0;
			}
		case ldtDword:
			{
				// NOTE:
				// ����ǿתΪint������DWORD����Ϊ�˱���CPU���������ʱ������������
				// ��CPU���㵥Ԫ�п�����real4ģʽ�����У���ʱ������о��ȶ�ʧ����
				// �緢������������ɼ��CTRL�Ĵ���8��9��λ�Ƿ�Ϊ00��
				// ��ϸ��Ϣ�ɲο�MASM32������������ĵ�������ѯ����
				int nValue = (int)lua_tonumber(L, 3);
				((pObjData->pObj)->*(SetDwordFunc)(pLuaData->pSetFunc))(nValue);
				return 0;
			}
		}

		return 0;
	}
};

#endif	//_LUNA_H_