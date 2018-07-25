////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DCommonObjectNodeSimple.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-5-28 11:11:46
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DCOMMONOBJECTNODE_H_
#define _INCLUDE_KG3DCOMMONOBJECTNODE_H_

#include "KG3DCommonObject.h"
////////////////////////////////////////////////////////////////////////////////
class KG3DCommonObjectBuilder;
class KG3DCommonObjectPool;
struct KG3DCommonObjectNode : public KG3DCommonObjectSimple
{
	STDMETHOD_(ULONG, GetType)() = 0;

	STDMETHOD_(KG3DCommonObjectBuilder*, GetCommonObjectBuilder)() = 0;
	STDMETHOD_(KG3DCommonObjectPool&, GetCommonObjectPool)() = 0;//���û��Pool�ģ����Է���g_GetKG3DCommonObjectPoolDummy();	

	STDMETHOD_(KG3DCommonObject*, Get)(const KG3DCOBJ_HANDLE& Handle) = 0;
	STDMETHOD_(KG3DCommonObject*, GetFast)(size_t iPos) = 0;
	STDMETHOD_(KG3DCommonObject*,Request)(const KG3DCOBJ_HANDLE& Handle, LPCTSTR FileName, DWORD_PTR wParam, DWORD_PTR lParam, KG3DCOBJ_HANDLE* pHandle) = 0;//��������ڻᴴ������
	STDMETHOD(Save)(const KG3DCOBJ_HANDLE& Handle, LPCTSTR FileName, DWORD_PTR wParam, DWORD_PTR lParam) = 0;

	STDMETHOD(Traverse)(KG3DCommonObjectVisitor& visitor) = 0;
};

struct KG3DCommonObjectNodeDummy : public KG3DCommonObjectNode
{
	STDMETHOD_(ULONG, GetType)(){return KG3DTYPE_COMMON_OBJECT_NODE;}

	STDMETHOD_(KG3DCommonObjectBuilder*, GetCommonObjectBuilder)(){return NULL;}
	STDMETHOD_(KG3DCommonObjectPool&, GetCommonObjectPool)();//���û��Pool�ģ����Է���g_GetKG3DCommonObjectPoolDummy();	

	STDMETHOD_(KG3DCommonObject*, Get)(const KG3DCOBJ_HANDLE& Handle){return NULL;}
	STDMETHOD_(KG3DCommonObject*, GetFast)(size_t iPos){return NULL;}
	STDMETHOD_(KG3DCommonObject*,Request)(const KG3DCOBJ_HANDLE& Handle, LPCTSTR FileName, DWORD_PTR wParam, DWORD_PTR lParam, KG3DCOBJ_HANDLE* pHandle){return NULL;}
	STDMETHOD(Save)(const KG3DCOBJ_HANDLE& Handle, LPCTSTR FileName, DWORD_PTR wParam, DWORD_PTR lParam){return E_FAIL;}

	STDMETHOD(Traverse)(KG3DCommonObjectVisitor& visitor){return E_FAIL;}
};

struct KG3DCommonObjectNodeSimple : public KG3DCommonObjectNode
{
	STDMETHOD_(ULONG, GetType)() = 0;

	STDMETHOD_(KG3DCommonObjectBuilder*, GetCommonObjectBuilder)() = 0;
	STDMETHOD_(KG3DCommonObjectPool&, GetCommonObjectPool)() = 0;//���û��Pool�ģ����Է���g_GetKG3DCommonObjectPoolDummy();

	STDMETHOD_(KG3DCommonObject*, Get)(const KG3DCOBJ_HANDLE& Handle);
	STDMETHOD_(KG3DCommonObject*, GetFast)(size_t iPos);
	STDMETHOD_(KG3DCommonObject*,Request)(const KG3DCOBJ_HANDLE& Handle, LPCTSTR FileName, DWORD_PTR wParam, DWORD_PTR lParam, KG3DCOBJ_HANDLE* pHandle);//��������ڻᴴ������
	STDMETHOD(Save)(const KG3DCOBJ_HANDLE& Handle, LPCTSTR FileName, DWORD_PTR wParam, DWORD_PTR lParam);

	STDMETHOD(Traverse)(KG3DCommonObjectVisitor& visitor);

	//�����ĸ���������Traverse�����ӽڵ�
	STDMETHOD(OnLostDevice)();
	STDMETHOD(OnResetDevice)();
	STDMETHOD(Render)();
	STDMETHOD(FrameMove)();


	static KG3DCommonObjectNodeSimple* Build(LPCTSTR, DWORD_PTR pKG3DCOBJ_HANDLE_AccelerationTable, DWORD_PTR AccelerationTableCount);
};

#endif //_INCLUDE_KG3DCOMMONOBJECTNODE_H_
