////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DShaderHolder.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-8-12 17:04:02
//  Comment     : �ο�KG3DShaderManager��˵��
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DShaderHolder.h"
#include "KG3DSystemEventListener.h"
#include "KG3DGraphicsTool.h"
#include "KG3DShaderInclude.h"
#include "DummyInterfaces/KGDummyID3DXEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����������extern��Ϊ�˱����ٰ���KG3DShaderManager.h����ѭ�����������ģ���Ϊ���ص�NULLObjectģʽ�������������ǲ�����ɢ�ġ�
namespace Private
{
	extern HRESULT ReloadShaderHolder(KG3DShaderPtrHolder& Holder);
	extern LPD3DXEFFECT GetSingletonDummyEffect();
};
KG3DShaderHolder::KG3DShaderHolder()
:m_pShaderHolder(NULL)
{
}
KG3DShaderHolder::KG3DShaderHolder( KG3DShaderPtrHolder* pHolder )
:m_pShaderHolder(pHolder)
{
	if(NULL != pHolder)
		m_pShaderHolder->AddRef();
}

KG3DShaderHolder::~KG3DShaderHolder()
{
	SAFE_RELEASE(m_pShaderHolder);
}

KG3DShaderHolder::KG3DShaderHolder( const KG3DShaderHolder& Other )
:m_pShaderHolder(Other.m_pShaderHolder)
{
	if(NULL != m_pShaderHolder)
		m_pShaderHolder->AddRef();		
}


KG3DShaderHolder& KG3DShaderHolder::operator=(const KG3DShaderHolder& Other )
{
	KG3DShaderPtrHolder* pHolderOld = m_pShaderHolder;
	
	m_pShaderHolder = Other.m_pShaderHolder;
	if(NULL != m_pShaderHolder)
		m_pShaderHolder->AddRef();

	SAFE_RELEASE(pHolderOld);

	return *this;
}
LPD3DXEFFECT KG3DShaderHolder::GetEffect()
{
	if(NULL != m_pShaderHolder)
	{
		LPD3DXEFFECT pRet = m_pShaderHolder->GetEffect();
		if (NULL != pRet)
		{
			return pRet;
		}
	}
	return Private::GetSingletonDummyEffect();
}

LPVOID KG3DShaderHolder::GetEffectParamHandleBuffer(size_t *puRetBufferSize)
{
    if (puRetBufferSize)
        *puRetBufferSize = 0;

    if (NULL == m_pShaderHolder)
        return NULL;

    return m_pShaderHolder->GetEffectParamHandleBuffer(puRetBufferSize);
}


HRESULT KG3DShaderHolder::Reload()
{
	if(NULL != m_pShaderHolder)
		return Private::ReloadShaderHolder(*m_pShaderHolder);
	return S_OK;
}

bool D3DXMACRO_ARRAY::IsValid( const D3DXMACRO_ARRAY& macroArray )
{
	if(NULL == macroArray.pMacroDefs)
	{
		_ASSERTE(0 == macroArray.uMacroCount);
		return true;
	}

	if(em_max_macro < macroArray.uMacroCount)
		return false;

	const D3DXMACRO& lastMacro = macroArray.pMacroDefs[macroArray.uMacroCount - 1];
	if(NULL != lastMacro.Name || NULL != lastMacro.Definition)
		return false;

	return true;
}

BOOL D3DXMACRO_ARRAY::MergeDefinition( std::vector<D3DXMACRO>& buffer, const D3DXMACRO_ARRAY& other )
{
	if (NULL == other.pMacroDefs || 0 == other.uMacroCount || this == &other)
	{
		return TRUE;
	}

	BOOL bIsValid = D3DXMACRO_ARRAY::IsValid(other) && D3DXMACRO_ARRAY::IsValid(*this);
	_ASSERTE(bIsValid);
	if(! bIsValid)
		return FALSE;

	std::vector<D3DXMACRO>	vecBufferTemp;
	size_t uNewSize = this->uMacroCount + other.uMacroCount;
	vecBufferTemp.reserve(uNewSize);	

	bool bImEmpty = (NULL == this->pMacroDefs || 0 == this->uMacroCount);
	if (bImEmpty)
	{
		_ASSERTE(0 == uMacroCount);
		vecBufferTemp.assign(other.pMacroDefs, other.pMacroDefs + other.uMacroCount);	
	}
	else
	{
		uNewSize -= 1;	//��Ϊ���������NULL,NULL�ظ�
		_ASSERTE(this->uMacroCount > 0);
		vecBufferTemp.assign(this->pMacroDefs, this->pMacroDefs + this->uMacroCount - 1);	//�����Ǹ�NULL��NULL�ܿ�
		vecBufferTemp.insert(vecBufferTemp.end(), other.pMacroDefs, other.pMacroDefs + other.uMacroCount);
	}
	buffer.clear();
	buffer.reserve(uNewSize);

	_ASSERTE(uNewSize == vecBufferTemp.size() && 0 != vecBufferTemp.size());
	//ȥ���ظ�����Ŀ
	LPCTSTR uTagPtr = (LPCTSTR)0x12345678;
	for (size_t i = 0, uEnd = (this->uMacroCount > 1 ? this->uMacroCount - 1 : 0); i < uEnd; ++i)
	{
		for (size_t x = 0; x < other.uMacroCount - 1; ++x)	//����NULL���ü��
		{
			if(i + x >= vecBufferTemp.size())
				break;

			//ǰ���IsValid������������ָ�벻����NULL��
			if(0 == _tcscmp(vecBufferTemp[i].Name, other.pMacroDefs[x].Name))
			{
				vecBufferTemp[i].Definition = other.pMacroDefs[x].Definition;	//����ĸ���ǰ���

				vecBufferTemp[uEnd+x].Name = uTagPtr;
				break;	//����ϲ���def��û���ظ��������Ŀ����������ľͿ���������
			}
		}
	}

	for (size_t i = 0; i < vecBufferTemp.size(); ++i)
	{
		if(vecBufferTemp[i].Name != uTagPtr)
			buffer.push_back(vecBufferTemp[i]);
	}

	pMacroDefs = &buffer[0];
	uMacroCount = buffer.size();

	_ASSERTE(D3DXMACRO_ARRAY::IsValid(*this));
	return TRUE;
}

#if defined(KG_ENABLE_TDD)
KG_TEST_CLASS_TEST_BEGIN(KG3DShaderHolder)
{
	struct TempDummyPtrHolder : public KG3DShaderPtrHolderDummy
	{
		TempDummyPtrHolder():m_ulRefCount(1){}
		virtual ULONG __stdcall AddRef() 
        {
            return (ULONG)KG_InterlockedIncrement((long *)&m_ulRefCount);
        }
		virtual	ULONG STDMETHODCALLTYPE Release() 
        {
            ULONG uNewRefCount = (ULONG)KG_InterlockedDecrement((long *)&m_ulRefCount);
            //if (uNewRefCount == 0)
            //    delete this; 
            return uNewRefCount;
        }
		ULONG m_ulRefCount;
	};

	TempDummyPtrHolder dummyPtrHolder, dummyPtrHolder2;

	{
		KG3DShaderHolder holder;
		_ASSERTE(NULL == holder.m_pShaderHolder && NULL == holder.GetShaderPtrHolder());
	}

	{
		_ASSERTE(dummyPtrHolder.m_ulRefCount == 1);
		KG3DShaderHolder holderWithDummy(&dummyPtrHolder);
		_ASSERTE(&dummyPtrHolder == holderWithDummy.m_pShaderHolder && &dummyPtrHolder == holderWithDummy.GetShaderPtrHolder());
		_ASSERTE(dummyPtrHolder.m_ulRefCount == 2);
	}
	_ASSERTE(dummyPtrHolder.m_ulRefCount == 1);
	{
		KG3DShaderHolder holder1(&dummyPtrHolder);
		KG3DShaderHolder holder2(holder1);

		_ASSERTE(&dummyPtrHolder == holder2.m_pShaderHolder && &dummyPtrHolder == holder2.GetShaderPtrHolder());
		_ASSERTE(dummyPtrHolder.m_ulRefCount == 3);
	}
	_ASSERTE(dummyPtrHolder.m_ulRefCount == 1);
	{
		KG3DShaderHolder holder1(&dummyPtrHolder);
		KG3DShaderHolder holder2 = holder1;

		_ASSERTE(&dummyPtrHolder == holder2.m_pShaderHolder && &dummyPtrHolder == holder2.GetShaderPtrHolder());
		_ASSERTE(dummyPtrHolder.m_ulRefCount == 3);
	}
	_ASSERTE(dummyPtrHolder.m_ulRefCount == 1);
	{
		KG3DShaderHolder holder1(&dummyPtrHolder);
		KG3DShaderHolder holder2;
		holder2 = holder1;

		_ASSERTE(&dummyPtrHolder == holder2.m_pShaderHolder && &dummyPtrHolder == holder2.GetShaderPtrHolder());
		_ASSERTE(dummyPtrHolder.m_ulRefCount == 3);
	}
	_ASSERTE(dummyPtrHolder.m_ulRefCount == 1);
	{
		KG3DShaderHolder holder1(&dummyPtrHolder);
		holder1 = KG3DShaderHolder(&dummyPtrHolder2);

		_ASSERTE(&dummyPtrHolder2 == holder1.m_pShaderHolder && &dummyPtrHolder2 == holder1.GetShaderPtrHolder());
		_ASSERTE(dummyPtrHolder.m_ulRefCount == 1);
		_ASSERTE(dummyPtrHolder2.m_ulRefCount == 2);
	}
	_ASSERTE(dummyPtrHolder.m_ulRefCount == 1 && dummyPtrHolder2.m_ulRefCount == 1);
	{
		KG3DShaderHolder holder1(&dummyPtrHolder);
		holder1 = KG3DShaderHolder(&dummyPtrHolder);

		_ASSERTE(&dummyPtrHolder == holder1.m_pShaderHolder && &dummyPtrHolder == holder1.GetShaderPtrHolder());
		_ASSERTE(dummyPtrHolder.m_ulRefCount == 2);
		_ASSERTE(dummyPtrHolder2.m_ulRefCount == 1);
	}
	_ASSERTE(dummyPtrHolder.m_ulRefCount == 1 && dummyPtrHolder2.m_ulRefCount == 1);
}
KG_TEST_CLASS_TEST_END(KG3DShaderHolder)
KG_TEST_BEGIN(D3DXMACRO_ARRAY)
{
	D3DXMACRO macrosPreDef[] = {
		{"A", "a"},
		{"B","b"},
		{NULL, NULL},
	};
	D3DXMACRO_ARRAY macroStandard = {
		macrosPreDef,
		_countof(macrosPreDef),
	};

	std::vector<D3DXMACRO> bufferTemp;
	BOOL bRet = FALSE;

	//�պϷǿ�
	{
		D3DXMACRO_ARRAY emptyOne = D3DXMACRO_ARRAY::GetEmptyMacroArray();
		bRet = emptyOne.MergeDefinition(bufferTemp, macroStandard);
		_ASSERTE(bRet);
		_ASSERTE(emptyOne.pMacroDefs == &bufferTemp[0] && emptyOne.uMacroCount == bufferTemp.size());
	}
	//�ǿպϿ�
	{
		D3DXMACRO_ARRAY macroMy = macroStandard;
		bRet = macroMy.MergeDefinition(bufferTemp, D3DXMACRO_ARRAY::GetEmptyMacroArray());
		_ASSERTE(bRet);
		_ASSERTE(0 == memcmp(&macroStandard, &macroMy, sizeof(macroStandard)));
	}
	//�ǿպϷǿ�
	{
		D3DXMACRO macroMyPreDef[] = {
			{"A", "ac"},
			{"C", "c"},
			{"B", "bc"},
			{"E", "e"},
			{NULL, NULL},
		};

		D3DXMACRO_ARRAY macroMy = {
			macroMyPreDef,
			_countof(macroMyPreDef),
		};

		bRet = macroMy.MergeDefinition(bufferTemp, macroStandard);
		_ASSERTE(bRet);
		_ASSERTE(macroMy.uMacroCount == bufferTemp.size());
		_ASSERTE(macroMy.uMacroCount == _countof(macroMyPreDef));

		_ASSERTE(0 == _tcscmp(macroMy.pMacroDefs[0].Definition, "a"));
		_ASSERTE(0 == _tcscmp(macroMy.pMacroDefs[0].Name, "A"));
		_ASSERTE(0 == _tcscmp(macroMy.pMacroDefs[2].Definition, "b"));
	}
}
KG_TEST_END(D3DXMACRO_ARRAY)
#endif

