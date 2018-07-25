/*****************************************************************************************
//	������Ϸ����ӿ�
//	Copyright : Kingsoft 2005
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2005-1-1
------------------------------------------------------------------------------------------
*****************************************************************************************/
#ifndef PLUGIN_I_PLUGIN_SHELL_H
#define PLUGIN_I_PLUGIN_SHELL_H

#include <windows.h>


class IKG3DEngineManager;
class IKG3DResourceManager;

class IKG3DUI;
class IKG3DDebugInfo;

class IKSO3RepresentHandler;

struct KLuaFunc;
struct KLuaConstList;

typedef LRESULT (*FnPluginBehaviorCallback)(UINT uMsg, WPARAM wParam, LPARAM lParam);

enum PLUGIN_OPERATION;
enum PLUGIN_EVENT;
enum PLUGIN_CALLBACK_FUNC;
enum PLUGIN_BEHAVIOR;

struct IPluginShell
{
	virtual int  Create(HWND hWnd, PVOID pReserved) = 0;
	virtual int	 Init() = 0;
	virtual void Breathe() = 0;
	virtual void Release() = 0;

	// ֪ͨ��ҽ���ĳ�ֲ���
	virtual LRESULT Operation(UINT uOperation, WPARAM wParam, LPARAM lParam) = 0;
	// �ͻ����¼�֪ͨ
	virtual LRESULT EventNotify(UINT uEvent, WPARAM wParam, LPARAM lParam) = 0;
	// �����ص��������б�
	virtual void SetCallback(PLUGIN_CALLBACK_FUNC eFuncIndex, FnPluginBehaviorCallback pFunc) = 0;
};

enum PLUGIN_OPERATION
{
	PLUGIN_ENABLE,		//�Ƿ�
	//nParam = (Boolean)bEnable �Ƿ�����˲��������

	PLUGIN_UI_EDITOR_SHOW_INFO,
	//�ñ༭����ʾһ����Ϣ
	//nParam = wszMessage

	PLUGIN_UI_EDITOR_SEL_WND,
	//�ñ༭��ѡ��һ������
	//nparam = pszWndName

};

enum PLUGIN_EVENT
{
	PLUGIN_EVENT_INIT_3D_ENGINE,
	//wParam = (KPluginInit3DEngine*)

	PLUGIN_EVENT_INIT_REPRESENT,
	//wParam = (KPluginInitRepresent*)

	PLUGIN_EVENT_INIT_GAME_WORLD,
	//wParam = (KPluginInitGameWorld*)

	PLUGIN_EVENT_GAME_FRAME_BREATHE,	//�ͻ�����Ϸ�߼�֡����
	//wParam = (DWORD)dwTimeNow
	//lParam = (DWORD)dwGameLoop

	PLUGIN_EVENT_BREATHE,				//�ͻ��˲�ֵ֡����
	//wParam = (DWORD)dwTimeNow
	//lParam = (DWORD)dwGameLoop

	PLUGIN_EVENT_RENDER,				//�ͻ��˻���

	PLUGIN_EVENT_WINDOW_MESSAGE,		//ת��Window��Ϣ
	//wParam = (UINT)uMsg
	//lParam = (KPluginWindowMessage*)wParam
};


enum PLUGIN_CALLBACK_FUNC
{ 		
	PCF_PLUGIN_BEHAVIOR = 1,
	PCF_UI_BEHAVIOR,
	PCF_UIEDITOR_BEHAVIOR,
};

enum PLUGIN_BEHAVIOR
{
	PCB_PLUGIN_SET_CALLBACK,
	//wParam = (PLUGIN_CALLBACK_FUNC)
	//lParam = (FnPluginBehaviorCallback)


	PCB_UI_GET_IMAGE_SIZE,
	//wParam = (const char*)pszImage
	//nParam = if 0 spr, else jpeg.
	//Return = (Width || (Height << 16))

	PCB_UI_GET_IMAGE_FRAME_COUNT,
	// wParam = (LPCTSTR)szFilePath
	// lParam = (DWORD&)&dwCount

	PCB_UI_GET_IMAGE_FRAME_SIZE,
	// wParam = (LPCTSTR)szFilePath
	// lParam = (SIZE&)&size

	PCB_UI_WINDOW_GET_POS,
	//wParam = pszTreepath 
	//nparam = PszItemTreePath
	//Return = (Left || (Top << 16))

	PCB_UI_WINDOW_GET_SIZE,
	//wParam = pszTreePath
	//nParam = pszItemTreePath
	//return = (Width || (Height << 16))

	PCB_UI_ENABLE_RENDER_UI,
	// wParam = (KWndWindow*)pWnd, NULL ��ʾȫ��
	// lParam = 1 Enable, 0 Disable

	PCB_UI_SHOW_FRAME,
	// wParam = (KWndWindow*)pWnd, NULL ��ʾȫ��
	// lParam = 1 Enable, 0 Disable

	PCB_UI_SHOW_NAME,
	// wParam = (KWndWindow*)pWnd, NULL ��ʾȫ��
	// lParam = 1 Enable, 0 Disable
	PCB_UI_SHOW_FRAME_DRAG_AREA_BODER,
	// wParam = (KWndWindow*)pWnd, NULL ��ʾȫ��
	// lParam = 1 Enable, 0 Disable

	PCB_UI_SHOW_TEXT_ATOM_BOUNDARY,
	// wParam = (KWndWindow*)pWnd, NULL ��ʾȫ��
	// lParam = 1 Enable, 0 Disable

	PCB_UI_ENABLE_RENDER_SAMPLER,
	// wParam = 1 Enable, 0 Disable

	PCB_UI_ENABLE_GLOBAL_BACK_BUFFER,
	// wParam = 1 Enable, 0 Disable

	PCB_UI_SHOW_MOUSE_WND,
	// wParam = (KWndWindow*)pWnd, NULL ��ʾȫ��
	// lParam = 1 Enable, 0 Disable

	PCB_UI_DRAG_WND_AND_ITEM_MODE,
	//wParam = NULL
	//lParam = 1 Enable, 0 Disable

	PCB_UI_RESIZE_WND_AND_ITEM_MODE,
	//wParam = NULL
	//lParam = 1 Enable, 0 Disable

	PCB_UI_LOAD_MAIN_UI,
	// wParam = (int)bLoad

	PCB_UI_RELOAD_COLOR_SCHEME,
	//Add by Hu Changyin. Used to reload the color scheme.

	PCB_UI_RELOAD_FONT_SCHEME,
	//Add by Hu Changyin. Used to reload the font scheme.

	PCB_UI_RELOAD_STRING_SCHEME,
	// Used to reload the global string value.

	PCB_UI_DELETE_FRAME,
	//Add by Hu Changyin. Used to delete the current edit frame.
	//wParam = pszFrameSectionname

	PCB_UI_CREATE_FRAME,
	//Add by Hu Changyin. Used to Create the current edit frame.
	// wparam = pIni

	PCB_UI_GET_FOCUS_WND_NAME,
	//Add by Hu CHangyin. Used to Get the Focus Wnd name.
	//wParam = Focus Wnd name ptr.
	//lparam = WndNameLen

	PCB_UI_ENABLE_SEL_WND_IN_CLIENT,
	// enable or disable sel wnd and tell uieditor.
	//wparam = 0 disable, else enable

	PCB_UI_EDIT_MODE,
	// enable or disable edit mode
	//wparam = 0 disable, else enable

	PCB_UI_RELOAD_ALL_IMAGE,
	// reload All Textture from file

	PCB_UI_GET_ROOT_PATH,
	// get the work root path
	//wparam = target string ptr

};

struct KPluginUIImage
{
	LPCTSTR szFilePath;
	int		nFrame;
};

struct KPluginInit3DEngine
{
	IKG3DEngineManager*   pEngineManager;
	IKG3DResourceManager* pModelManager;

	IKG3DUI*              pUI;
};

struct KPluginInitRepresent
{
	IKSO3RepresentHandler* pHandler;
};


struct KPluginInitGameWorld
{
	typedef int (*FnGetFuncList)(KLuaFunc*& pFuncList);
	typedef BOOL (*FnAttributeStringToID)(LPCTSTR, int&);
	typedef BOOL (*FnRequireStringToID)(LPCTSTR, int&);

	KLuaConstList*			pLuaConstList;

	FnGetFuncList			fnBaseFuncList;
    FnGetFuncList			fnPartyFuncList;
    FnGetFuncList			fnShopFuncList;
	FnGetFuncList			fnLoginFuncList;

	FnAttributeStringToID	fnAttributeStringToID;
	FnRequireStringToID		fnRequireStringToID;
};


struct KPluginWindowMessage
{
	HWND	hWnd;
	UINT	uMsg;
	WPARAM	wParam;
	LPARAM	lParam;
};

//------ģ��Ľӿں�����ԭ�͵Ķ���------
typedef IPluginShell*			(*FnCreatePlugin)();

//------�ӿں����ĺ������ַ���----------
#define	FN_CREATE_PLUGIN		"CreatePlugin"


#endif //PLUGIN_I_PLUGIN_SHELL_H
