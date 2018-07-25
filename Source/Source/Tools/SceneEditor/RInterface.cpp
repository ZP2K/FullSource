#include "stdafx.h"
#include "RInterface.h"
#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------------------------ //
// InterfaceReg.
// ------------------------------------------------------------------------------------ //
InterfaceReg *InterfaceReg::s_pInterfaceRegs = NULL;


InterfaceReg::InterfaceReg( InstantiateInterfaceFn fn, const char *pName ) :
	m_pName(pName)
{
	m_CreateFn = fn;
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}



// ------------------------------------------------------------------------------------ //
// ÿ��ģ��Ľӿڹ���
// pName:�ӿ�ʵ��������
// ���ؽӿ�ʵ��ָ��
// ------------------------------------------------------------------------------------ //
void* CreateInterface( const char *pName)
{
	InterfaceReg *pCur;

	for(pCur=InterfaceReg::s_pInterfaceRegs; pCur; pCur=pCur->m_pNext)
	{
		if (strcmp(pCur->m_pName, pName) == 0)
		{
			return pCur->m_CreateFn();
		}
	}

	return NULL;
}

// ��ģ����ȡ�ýӿڹ�������ָ��
// hModule:ģ����
// ����ģ��ӿڹ�������ָ��
CreateInterfaceFn Sys_GetFactory( HMODULE hModule )
{
	if ( !hModule )
		return NULL;

	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress( hModule, CREATEINTERFACE_PROCNAME ));
}

// ȡ�ñ�ģ��Ľӿڹ���
CreateInterfaceFn Sys_GetFactoryThis( void )
{
	return CreateInterface;
}

