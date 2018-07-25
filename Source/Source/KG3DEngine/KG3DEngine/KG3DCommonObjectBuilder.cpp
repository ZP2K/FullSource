////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DCommonObjectBuilder.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-10 16:27:22
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DCommonObjectBuilder.h"
#include "KG3DCommonObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*
������ע����ơ�����಻�ð����κκ����ɵĶ�����ص���Ϣ��Build������������
KG3DCollocator��ɣ�������ͷ�ļ���ֻ����KG3DCollocator�����
*/
/************************************************************************/
HRESULT KG3DCommonObjectBuilder::RegisterType( DWORD dwType, TypeFuncCommonObjectBuilder pFuncBuilder
										 , TypeFuncCommonObjectSaver pFuncSaver)
{
	KG_PROCESS_ERROR(NULL != pFuncBuilder);
	{
		TypeConConElem& Elem = m_ConCon[dwType];
		KG_PROCESS_ERROR(Elem.pFuncBuilder == NULL 
			|| Elem.pFuncBuilder == pFuncBuilder);//����Ѿ�������һ��ע�������������ô�����ȷ�ע��
		_ASSERTE(Elem.pFuncSaver == NULL);
		KG_PROCESS_ERROR(pFuncBuilder);
		Elem.pFuncBuilder = pFuncBuilder;
		Elem.pFuncSaver = pFuncSaver;
		return S_OK;
	}
Exit0:
	return E_FAIL;
}

HRESULT KG3DCommonObjectBuilder::UnRegisterType( DWORD dwType, TypeFuncCommonObjectBuilder* pOldBuilder
										   , TypeFuncCommonObjectSaver* pOldSaver)
{
	TypeContainerContainer::iterator it = m_ConCon.find(dwType);
	KG_PROCESS_ERROR(it != m_ConCon.end());
	{
		TypeConConElem& Elem = it->second;
		if(pOldBuilder)
			*pOldBuilder = Elem.pFuncBuilder;
		if (pOldSaver)
			*pOldSaver = Elem.pFuncSaver;
		Elem.pFuncBuilder = NULL;
		Elem.pFuncSaver = NULL;

		return S_OK;
	}
Exit0:
	return E_FAIL;
}
KG3DCommonObject* KG3DCommonObjectBuilder::Build( DWORD dwType, LPCTSTR strFileName, DWORD_PTR wParam, DWORD_PTR lParam )
{
	KG3DCommonObject* pRet = NULL;
	TypeContainerContainer::iterator it = m_ConCon.find(dwType);
	KG_PROCESS_ERROR(it != m_ConCon.end());
	{
		TypeConConElem& Elem = it->second;
		KGLOG_PROCESS_ERROR_3D(Elem.pFuncBuilder && "No Container Builder");

		KG_PROCESS_ERROR(Elem.pFuncBuilder);
		pRet = (*Elem.pFuncBuilder)(strFileName, wParam, lParam);
		KGLOG_PROCESS_ERROR(pRet && "Container Builder Failed");
		_ASSERTE(pRet->GetType() == dwType);
		return pRet;
	}
Exit0:
	SAFE_RELEASE(pRet);
	return NULL;
}

HRESULT KG3DCommonObjectBuilder::Save( KG3DCommonObject* p,LPCTSTR strFileName, DWORD_PTR wParamToSave, DWORD_PTR lParamToSave )
{
	KG_PROCESS_ERROR(p != NULL);
	{
		DWORD dwType = p->GetType();
		TypeContainerContainer::iterator it = m_ConCon.find(dwType);

		TypeFuncCommonObjectSaver pFuncSaver = (it->second).pFuncSaver;
		_ASSERTE(pFuncSaver);
		return (*pFuncSaver)(p, strFileName, wParamToSave, lParamToSave);
	}
Exit0:
	return E_FAIL;
}

ULONG KG3DCommonObjectBuilder::IsTypeRegister( DWORD dwType )
{
	TypeContainerContainer::iterator it = m_ConCon.find(dwType);
	return it != m_ConCon.end();
}
#if defined(KG_ENABLE_TDD)
//����Container����ע�����
KG_TEST_BEGIN(KG3DCommonObjectBuilder)
	{
		static size_t stConConTesterInstanceCount = 0;
		struct ConConTester : public KG3DCommonObjectSimple
		{
			INT A;
			virtual ULONG STDMETHODCALLTYPE GetType(){return 0;}
			ConConTester(){A = 0x12345678; ++stConConTesterInstanceCount;}
			~ConConTester(){--stConConTesterInstanceCount;}
			static KG3DCommonObject* Build(LPCTSTR strFileName, DWORD_PTR, DWORD_PTR){return new ConConTester;}
			static HRESULT Save(KG3DCommonObject* p, LPCTSTR, DWORD_PTR, DWORD_PTR){return 0x12345678;}
		};

		KG3DCommonObjectBuilder	Builder;

		HRESULT hr = Builder.RegisterType(0, &ConConTester::Build, &ConConTester::Save);
		_ASSERTE(SUCCEEDED(hr));

		_ASSERTE(Builder.IsTypeRegister(0));

		KG3DCommonObject* p = Builder.Build(0, NULL, 0, 0);
		_ASSERTE(NULL != p);
		ConConTester* pTester = dynamic_cast<ConConTester*>(p);
		_ASSERTE(pTester);
		_ASSERTE(pTester->A == 0x12345678);

		hr = Builder.Save(pTester, NULL, 0, 0);
		_ASSERTE(hr == 0x12345678);//ȷ���ܹ����þͺ���
		_ASSERTE(stConConTesterInstanceCount == 1);
		pTester->Release();
		_ASSERTE(stConConTesterInstanceCount == 0);

		//��ע�ᣬ֮��Ӧ�þ��޷������µ�Container��
		hr = Builder.UnRegisterType(0, NULL, NULL);
		_ASSERTE(SUCCEEDED(hr));
		p = Builder.Build(0, NULL, 0, 0);
		_ASSERTE(p == NULL);
	}
KG_TEST_END(KG3DCommonObjectBuilder)
#endif

