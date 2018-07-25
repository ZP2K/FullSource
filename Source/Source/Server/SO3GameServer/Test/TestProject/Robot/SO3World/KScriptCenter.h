/************************************************************************/
/* �ű�����								                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.03.02	Create												*/
/* Comment	 :															*/
/*		��װ�˶Խű��Ĳ����ʹ洢										*/
/************************************************************************/
#ifndef _KSCRIPT_CENTER_H_
#define _KSCRIPT_CENTER_H_

#if defined(_SERVER) | defined(_EDITOR)
	#include <map>
#endif	//_SERVER or _EDITOR

#if defined(_CLIENT)
	#include "KIDCache.h"
#endif	//_CLIENT

class KSO3World;
class KCharacter;
class KPlayer;

class KScriptCenter
{
public:
	KScriptCenter(void);
	~KScriptCenter(void);

	BOOL Init(void);
	BOOL UnInit(void);

	KLuaScriptEx* GetScript(const char* pszFileName);
	KLuaScriptEx* GetScript(DWORD dwScriptID);

	//����ű��ļ�
	BOOL LoadScript(const char* pszFileName);

#if defined(_SERVER) | defined(_EDITOR)
	//ִ�нű������ַ�������Ҫ��GMָ����
	BOOL ExecuteScript(KPlayer* pPlayer, const char* pszScript);
#endif	//_SERVER or _EDITOR

#if defined(_CLIENT)
	//ִ�нű������ַ���
	BOOL ExecuteScript(const char* pszScript);
#endif	//_CLIENT

private:
	typedef std::map<DWORD, KLuaScriptEx> SCRIPT_LIST;
	SCRIPT_LIST					m_ScriptList;			//�ű��б�
	//����ָ��Ŀ¼�µ����нű�
	void LoadScriptInDirectory(LPSTR lpszRootDir, LPSTR lpszSubDir);

	KLuaScriptEx	m_TempScript;
};

extern KScriptCenter	g_ScriptCenter;
#endif	//_KSCRIPT_CENTER_H_