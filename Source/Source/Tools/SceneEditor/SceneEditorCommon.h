////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : SceneEditorCommon.h
//  Version     : 
//  Creator     : 
//  Create Date : 
//  Comment     : ����ļ����ڶ���ȫ�ֲ������벻Ҫ�������cpp�ź�ȫ�ֲ����޹صĶ���
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_SCENEEDITORCOMMON_H_
#define _INCLUDE_SCENEEDITORCOMMON_H_

////////////////////////////////////////////////////////////////////////////////
#include "IEOther.h"
#if defined(_DEBUG) | defined(DEBUG)
	//extern IEKG3DEngineManager* g_pEngineManager;
	class CEngineManagerHelper
	{
		IEKG3DEngineManager* m_pEngineManager;
	public:
		BOOL Init(IEKG3DEngineManager* p)
		{
			_ASSERTE(p);
			m_pEngineManager = p;
			return p != NULL;
		}
		IEKG3DEngineManager* operator ->()
		{
			_ASSERTE(m_pEngineManager);
			return m_pEngineManager;
		}
		typedef IEKG3DEngineManager* LPENGINEMANAGER;		
		void operator = (IEKG3DEngineManager*);
		IEKG3DEngineManager* GetInterface();
	private:
		struct PrivateStruct{};
		typedef PrivateStruct* LPPrivateStruct;
/*
������ָ���Զ�ת����IE����������->����Ϊ�ⲿ�ܶ�ط����ǲ����g_pEngineManager���õġ�
��g_pEngineManager��ʼ��ʧ�ܵļ���������У�g_pEngineManager����Ҫ���
�Ƿ�Ϊ�յģ���������ṩ�Զ�ת�ͣ������g_pEngineManager�������ͷź���
���õ���g_pEngineManager��ƴ����ASSERT�ˡ����Ҹ���SmartPtr��Լ��������Ҳ��
Ӧ���ṩԭָ���Զ�ת�ͣ���Ȼ�ں����Ĳ����з������Զ�ת�ͻ᲻֪�����ƿ�
���ǵļ����ơ�����ģ����������Ҫ�õ�ԭָ�룬������GetInterface��
ˡ���ڴ����������ˣ���һ��ʱ���ɾ����Щ�����ɡ�
*/
	public:
		operator LPPrivateStruct(){return m_pEngineManager? (PrivateStruct*)0x00000001 : NULL;}///����if(g_pEngineManager)
	};
	extern CEngineManagerHelper g_pEngineManager;
	extern IEKG3DGraphicsTool* g_pGraphicsTool;
	extern IKG3DRTTITypeManager* g_pKG3DRTTITypeManager;///<RTTI_INTERFACE>
#else
	extern IEKG3DEngineManager* g_pEngineManager;
	extern IEKG3DGraphicsTool* g_pGraphicsTool;
	extern IKG3DRTTITypeManager* g_pKG3DRTTITypeManager;///<RTTI_INTERFACE>
#endif

inline IEKG3DEngineManager* g_GetEngineIEInterface()
{
#if defined(_DEBUG) | defined(DEBUG)
	return g_pEngineManager.GetInterface();
#else
	return g_pEngineManager;
#endif
}

extern TCHAR g_szDefWorkDirectory[256];
extern TCHAR g_szDefExeDirectory[MAX_PATH];
extern LPDIRECT3DDEVICE9 g_pd3dDevice;

extern TCHAR g_strWeaponDirectory[MAX_PATH];
extern TCHAR g_strItemDirectory[MAX_PATH];

void InitGlobalVars(IEKG3DEngineManager* pInterface);

class CDllHolder
{
public:
	HRESULT Init(LPCTSTR lpstrDllName);
	void UnInit();
	HMODULE GetDllHandle(){	return m_hMod;}

	CDllHolder():m_hMod(NULL){}
	~CDllHolder(){UnInit();}
	
private:
	HMODULE m_hMod;
	std::tstring	m_strDllName;
};


#endif //_INCLUDE_SCENEEDITORCOMMON_H_
