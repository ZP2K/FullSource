//******************************************************************************
// ��ս��Ŀ�������ӿڶ���ͷ�ļ�
// ����ʱ��: 2003-11-02
//
// ��ս��Ŀ����ӿ�Э���ͷ�ļ�
// ���ɲ����һ���ӿڵĲ���:
//    1. �ӿڵ����г�Ա��������Ϊ���麯��,�ҽӿ�û�����ݳ�Ա.
//    2. Ϊ�ӿ�ʵ����һ������,��"Net1.0".
//    3. �ڽӿڵ�ʵ���ļ���, ʹ�� EXPOSE_INTERFACE �� EXPOSE_SINGLE_INTERFACE.
//
// �汾����
// There are two versioning cases that are handled by this:
// 1. ����һ���ӿ����������µĹ����γ�һ���µĽӿ�, ������Ҳ�Ƕ����Ƽ���ǰһ����
//    �ڵ�. ���������, ����Ҫд���� EXPOSE_INTERFACEs: һ����������Ϊ��ǰ�Ľ�
//    ��,һ�����Ϊ�½ӿ�
// 2. ���޸���һ���ӿ�ʹ������ǰ���ټ��� (������Ȼϣ��Ϊ������ǰ�Ĵ�������ɽӿ�
//    ). ���������, ����ҪΪ����½ӿ���һ���°汾������, ��������һ������ӿ�,
//    �����Ϊ�Ͻӿ�
//******************************************************************************

#ifndef _RINTERFACE_H_
#define _RINTERFACE_H_

#include "DLLBase.h" 
#include "windows.h"

#define CREATEINTERFACE_PROCNAME	"CreateInterface"
typedef void* (*CreateInterfaceFn)(const char *pName);

// ÿ���ӿ�ʵ�������ɽӿ�ʵ���ĺ�������
typedef void* (*InstantiateInterfaceFn)();


// ����ע��ӿ�ʵ�������ݽṹ
class InterfaceReg
{
public:
	InterfaceReg(InstantiateInterfaceFn fn, const char *pName);

public:

	InstantiateInterfaceFn	m_CreateFn;
	const char				*m_pName;

	InterfaceReg			*m_pNext; // For the global list.
	static InterfaceReg		*s_pInterfaceRegs;
};


// ��������ӿ�,�����ж��ʵ��
// ��:
// EXPOSE_INTERFACE( CInterfaceImp, IInterface, "MyInterface001" )
// ����� CInterfaceImp ʵ�� IInterface (a pure class)
// �ͻ����Ե��� CreateInterface( "MyInterface001" ) ��ȡ�����ʵ��ָ��
//
// �ӿڵ�ͷ�ļ�����ӿ� (IInterface) �Ͱ汾���� ("MyInterface001")
//
// ���������ͨ�����ؼ̳�ʵ�ֶ���ӿ�
//
// ��������Լ�д��������,ʹ�������
#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
	static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName);

// �Զ����ɴ�������,��ѯ�ӿ�ʱ�Զ����ɽӿ�ʵ��
#define EXPOSE_INTERFACE(className, interfaceName, versionName) \
	static void* __Create##className##_interface() {return (interfaceName *)new className;}\
	static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName );

// ͨ���㶨���ȫ�ֱ�����������ӿ�
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName) \
	static void* __Create##className##interfaceName##_interface() {return (interfaceName *)&globalVarName;}\
	static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);

// ��������ӿ�,�Զ�����ȫ�ֱ���
#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
	static className __g_##className##_singleton;\
	EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)

// ÿ��ģ��Ľӿڹ���
// pName:�ӿ�ʵ��������
// ���ؽӿ�ʵ��ָ��
DLL_EXPORT void* CreateInterface(const char *pName);

// ȡ�ñ�ģ��Ľӿڹ���
extern CreateInterfaceFn Sys_GetFactoryThis( void );

// ��ģ����ȡ�ýӿڹ�������ָ��
// hModule:ģ����
// ����ģ��ӿڹ�������ָ��
extern CreateInterfaceFn Sys_GetFactory( HMODULE hModule );


#endif



