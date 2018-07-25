////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DCollocator.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-10 17:58:55
//  Comment     : //���ϵ����������Ҫ����ע��һЩ����������EngineManager�Ĺ��캯����ִ��
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DCOLLOCATOR_H_
#define _INCLUDE_KG3DCOLLOCATOR_H_

//�����������ϵ
////////////////////////////////////////////////////////////////////////////////

interface IEKG3DCollocator;

typedef const BOOL* LPCONSTBOOL;
class KG3DCollocator
{
public:
	static void CollocateBeforeInit();
	static void CollocateAfterInit(LPDIRECT3DDEVICE9 pDevice);	//��ʼ����ʲô�����Է���������Ͳ��óŴ�KG3DEngineManger.cpp�Ĵ�����
	static void UnCollocateBeforeUnInit(LPDIRECT3DDEVICE9 pDevice);	//������Ķ�Ӧ���ͷŲ���Ҫ����Դ

	static IEKG3DCollocator* GetAnotherInterface();

	//<GLOBAL_PATH>
	template<size_t uBufferSize>
	static HRESULT GetSavePath(DWORD dwType, const KGCH::StrPathSplited& scenePathSplited, KGCH::StrPathSplited* pRetPathSplited
								, TCHAR (&pathRet)[uBufferSize])
	{
		return GetSavePath(dwType, scenePathSplited, pRetPathSplited, pathRet, uBufferSize);
	}
	static HRESULT GetSavePath(DWORD dwType, const KGCH::StrPathSplited& scenePathSplited,  KGCH::StrPathSplited* pRetPathSplited
								, LPTSTR pathRet, size_t uPathBufferSize);//��map�ļ���·���õ����������ļ��ı���·��
	//</GLOBAL_PATH>

	//<GLOBAL_SWITCH>
	static VOID	ToggleGlobalSwitch(const BOOL& filter, BOOL bEnable);
	static const BOOL& GetSwitchByName(LPCTSTR name, BOOL* pSucceeded);//������ɹ��Ļ�������һ���޹�ֵ������Ĳ��������Ƿ�ɹ�����������ľ���NULL
	//</GLOBAL_SWITCH>
private:
	static void ReadSettings();
	static void RegisterTypes();
	static void CheckRelationships();
	static void CheckResources(LPDIRECT3DDEVICE9 pDevice);
};

struct KG3DGlobalSwitch
{
	BOOL bNewSelection;
	BOOL bSelectableRefactorFinished;
	BOOL bFloorChecker;
	KG3DGlobalSwitch();

	static const BOOL* GetSwitchByName(const KG3DGlobalSwitch& switchIn, LPCTSTR strName);
};

extern const KG3DGlobalSwitch g_Switch;	//����ֱ�Ӹ���������õģ�����ͨ��ToggleGlobalSwitch����Ȼ�Ҹĵľ�û��������

#endif //_INCLUDE_KG3DCOLLOCATOR_H_
