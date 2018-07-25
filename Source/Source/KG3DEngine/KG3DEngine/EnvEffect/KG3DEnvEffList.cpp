#include "StdAfx.h"
#include "KG3DEnvEffect.h"
#include "KG3DEnvEffList.h"
#include "KG3DTypes.h"


#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace KG3D_ENVIRONMENT_EFFECT
{
	/************************************************************************/
	/*KG3DEnvEff_List
	ԭ������map������Ԫ�صģ�����Render��FrameMove��Щ��������Ҫ����
	����EnvEff�����Ա�������DWORD�ķ����Բ�����Ҫ�Ķࡣ���ǵ�map��iterator�൱
	������������ǰ�õ��ǰ���һ���iterator������ֱ�ӱ�¶���ݣ����Ǿ͸����ˡ�
	������Ϊmapֻ�ܰ�һ�����������Ԫ�أ�����Ҳ������ʵ��EnvEff���������ṹ��
	������ȱ��������ڳ�����һ����̬����ͬʱ��·�ɱ���C��Array��ΪԪ��������
	�ü򵥵ı����㷨ʵ��	�����Բ��ң�����ԭʼ��for����������������һ�����Լ���Render��FrameMove��
	������Find�ϵ���ʧ���࣬���ҿ��Լ򵥱��ֳ����ṹ(·�ɱ�����parent��������
	Parent��EnvEffect������ֱ�ӷ���Array�У���·�ɱ�֪����Parent֮��Ϳ�����Parent��
	EnvEffect�õ���Effect��)��ʹ����·�ɱ��Find����Effect�Ĳ����Ͽ϶���map::find
	�졣
	*/
	/************************************************************************/

	KG3DEnvEff_List::KG3DEnvEff_List()
	{
		//ZeroMemory(m_InnerArray, _countof(m_InnerArray)*sizeof(KG3DEnvEffect*));
		//m_nValidElementCount = 0;
//#if defined(DEBUG) | defined(_DEBUG)	//���Բ���
//		KG3DEnvEffect::TypePropertyTreeTest(_countof(m_InnerArray));
//#endif

		m_CtIt = m_Ct.begin();
	}
	KG3DEnvEff_List::~KG3DEnvEff_List()
	{
		this->Clear();	
	}
	KG3DEnvEffect* KG3DEnvEff_List::Find(DWORD dwType)
	{
		/*unsigned Handle = KG3DEnvEffect::GetTypeHandle(dwType);
		KG_PROCESS_ERROR(KG3DEnvEffect::IsValidTypeHandle(Handle) && KG3DEnvEffect::IsTypeTopEnvEff(Handle));

#if defined(DEBUG) | defined(_DEBUG)
		KG3DEnvEffect* pElement = GetElement(KG3DEnvEffect::GetTopTypeIndex(Handle));
		if (pElement)
		{
			_ASSERTE(pElement->GetType() == dwType);
		}
		return pElement;
#else
		return GetElement(KG3DEnvEffect::GetTopTypeIndex(Handle));
#endif
Exit0:
		return NULL;	*/			

		TypeCt::iterator it = m_Ct.find(dwType);
		if(it != m_Ct.end())
			return it->second;
		return NULL;
	}
		
	ULONG KG3DEnvEff_List::GetValidElementCount()
	{
		/*_ASSERTE(m_nValidElementCount >= 0);
#if defined(DEBUG) | defined(_DEBUG)
		///������m_nValidElementCount����ȷ��
		static int nTick = 0;
		nTick++;
		if (nTick > 100)
		{
			nTick = 0;
			int j = 0;
			for (int i = 0; i < _countof(m_InnerArray); i++)
			{
				if (m_InnerArray[i])
				{
					j++;
				}
			}
			_ASSERTE(j == m_nValidElementCount);
		}
#endif
		return m_nValidElementCount;*/
		return static_cast<ULONG>(m_Ct.size());
	}
	VOID KG3DEnvEff_List::Clear()
	{
		m_Ct.clear();
		return;
	}
	ULONG KG3DEnvEff_List::Empty()
	{
		/*_ASSERTE(m_nValidElementCount >= 0);
		return m_nValidElementCount == 0;*/
		return m_Ct.empty();
	}
	KG3DEnvEffect* KG3DEnvEff_List::Erase(DWORD dwType)
	{
		/*unsigned Handle = KG3DEnvEffect::GetTypeHandle(dwType);
		KG_PROCESS_ERROR(KG3DEnvEffect::IsValidTypeHandle(Handle) && KG3DEnvEffect::IsTypeTopEnvEff(Handle));
		{
			KG3DEnvEffect*& RefP = GetElement(KG3DEnvEffect::GetTopTypeIndex(Handle));
			if (RefP)
			{
				KG3DEnvEffect* pTemp = RefP;
				RefP = NULL;
				m_nValidElementCount--;
				_ASSERTE(m_nValidElementCount >=0);
				return pTemp;
			}
		}
Exit0:
		return NULL;*/

		TypeCt::iterator it = m_Ct.find(dwType);
		if (it != m_Ct.end())
		{
			KG3DEnvEffect* p = it->second;
			m_Ct.erase(it);
			return p;
		}
		return NULL;
	}
	HRESULT  KG3DEnvEff_List::Insert(DWORD	dwType, KG3DEnvEffect* pEffect)
	{
		if (this->GetValidElementCount() >= MAX_SIZE - 1)
		{
			OutputDebugString(_T("KG3DEnvEff_List������������ֵ��Insertʧ��"));
			goto Exit0;
		}
		//ֻ�ж���Effect����insert
		//unsigned Handle = KG3DEnvEffect::GetTypeHandle(dwType);

		BOOL bValidType = /*KG3DEnvEffect::IsValidTypeHandle(Handle) &&*/g_GetEnvEffectClassLibrary().IsTypeTop(dwType);//KG3DEnvEffect::IsTypeTopEnvEff(Handle);
			
		KG_PROCESS_ERROR(bValidType && NULL != pEffect);
		{
			/*KG3DEnvEffect*& RefP = GetElement(KG3DEnvEffect::GetTopTypeIndex(Handle));
			KG_PROCESS_ERROR(RefP == NULL && pEffect && pEffect->GetType() == dwType);
			RefP = pEffect;
			m_nValidElementCount++;
			return true;*/

			TypeCt::_Pairib ib = m_Ct.insert(TypeCt::value_type(dwType, pEffect));
			KG_PROCESS_ERROR(ib.second);
			return S_OK;
		}
Exit0:
		return E_FAIL;
	}
	KG3DEnvEffect* KG3DEnvEff_List::GetEnvEffect( DWORD dwType, int nFinded)
	{
		/*unsigned Handle = KG3DEnvEffect::GetTypeHandle(dwType);
		KG_PROCESS_ERROR(KG3DEnvEffect::IsValidTypeHandle(Handle));*/

		KG3DEnvEffectClassProperty* pProperty = g_GetEnvEffectClassLibrary().GetProperty(dwType);
		KG_PROCESS_ERROR(NULL != pProperty);

		if (/*KG3DEnvEffect::IsTypeTopEnvEff(Handle)*/pProperty->IsTop())
		{
			//return GetElement(KG3DEnvEffect::GetTopTypeIndex(Handle));
			return this->Find(dwType);
		}
		else
		{
			/*KG3DEnvEffect* pEnvEff = GetElement(KG3DEnvEffect::GetTypeParentIndex(Handle));
			return pEnvEff? (pEnvEff->GetSubEffect(dwType, nFinded)) : NULL;		*/

			KG3DEnvEffect* pEnvEff = this->Find(pProperty->m_dwParentType);
			KG_PROCESS_ERROR(NULL != pEnvEff);
			return pEnvEff->GetSubEffect(dwType, nFinded);
		}
Exit0:
		return NULL;
	}

	INT KG3DEnvEff_List::GetEnvEffectCount(DWORD dwType)
	{
		/*unsigned Handle = KG3DEnvEffect::GetTypeHandle(dwType);
		KG_PROCESS_ERROR(KG3DEnvEffect::IsValidTypeHandle(Handle));*/

		KG3DEnvEffectClassProperty* pProperty = g_GetEnvEffectClassLibrary().GetProperty(dwType);
		KG_PROCESS_ERROR(NULL != pProperty);

		if (/*KG3DEnvEffect::IsTypeTopEnvEff(Handle)*/pProperty->IsTop())
		{
			//return GetElement(KG3DEnvEffect::GetTopTypeIndex(Handle)) ? 1 : 0;
			return this->Find(dwType) ? 1 : 0;
		}
		else
		{
			/*KG3DEnvEffect* pEnvEff = GetElement(KG3DEnvEffect::GetTypeParentIndex(Handle));
			return pEnvEff? (pEnvEff->GetSubEffectCount(dwType)) : 0;		*/

			KG3DEnvEffect* pEnvEff = this->Find(pProperty->m_dwParentType);
			KG_PROCESS_ERROR(NULL != pEnvEff);

			return pEnvEff->GetSubEffectCount(dwType);
		}
Exit0:
		return 0;
	}
//
//	KG3DEnvEffect* KG3DEnvEff_List::GetFirstTopEnvEffect( UINT& Handle )
//	{
//		///���ValidElementCount�Ƿ���ȷ
//#if defined(DEBUG) | defined(_DEBUG)
//		static int nTick = 0;
//		nTick++;
//		if (nTick > 1000)
//		{
//			nTick = 0;
//			int nValidElementCount = 0;
//			for (int i = 0; i < _countof(m_InnerArray); i++)
//			{
//				if (m_InnerArray[i])
//				{
//					nValidElementCount++;
//				}
//			}
//			_ASSERTE(nValidElementCount == GetValidElementCount() && "��ЧEffect���ȣ����Size��Ա");
//		}
//#endif
//		for (UINT i = 0; i < _countof(m_InnerArray); i++)
//		{
//			KG3DEnvEffect* pEnvEff= m_InnerArray[i];
//			if (pEnvEff)
//			{
//				Handle = i;
//				return pEnvEff;
//			}
//		}
//		return NULL;
//	}

	/*KG3DEnvEffect* KG3DEnvEff_List::GetFirstTopEnvEffect( KG3DENVEFF_HANDLE& EnvHandle )
	{
		UINT HandleTemp = 0;
		KG3DEnvEffect* pResult = GetFirstTopEnvEffect(HandleTemp);
		if(pResult)
		{
			EnvHandle = KG3DENVEFF_HANDLE(pResult->GetType(), 0);
		}
		return pResult;
	}*/
	/*KG3DEnvEffect* KG3DEnvEff_List::GetNextTopEnvEffect( UINT Handle, UINT& NextHandle )
	{
		for (UINT i = Handle+1; i < _countof(m_InnerArray); i++)
		{
			KG3DEnvEffect* pEnvEff= m_InnerArray[i];
			if (pEnvEff)
			{
				NextHandle = i;
				return pEnvEff;
			}
		}
		return NULL;
	}*/

	/*KG3DEnvEffect* KG3DEnvEff_List::GetNextTopEnvEffect( KG3DENVEFF_HANDLE PrevHandle, KG3DENVEFF_HANDLE& NextHandle )
	{
		DWORD dwType = PrevHandle.m_dwType;
		unsigned Handle = KG3DEnvEffect::GetTypeHandle(dwType);
		KG_PROCESS_ERROR(KG3DEnvEffect::IsValidTypeHandle(Handle) && KG3DEnvEffect::IsTypeTopEnvEff(Handle));
		{
			for (unsigned i = KG3DEnvEffect::GetTopTypeIndex(Handle) + 1; i < _countof(m_InnerArray); i++)
			{
				KG3DEnvEffect* pEnvEff= m_InnerArray[i];
				if (pEnvEff)
				{
					NextHandle = KG3DENVEFF_HANDLE(pEnvEff->GetType(), 0);
					return pEnvEff;
				}
			}
			
		}
Exit0:
		return NULL;
	}*/
	/*KG3DEnvEffect* KG3DEnvEff_List::GetTopEnvEffect( UINT Handle )
	{
		if(Handle < _countof(m_InnerArray))
		{
			return m_InnerArray[Handle];
		}
		return NULL;
	}*/

//	UINT KG3DEnvEff_List::GetTopEnvEffHandle( DWORD dwType )
//	{
//		unsigned Handle = KG3DEnvEffect::GetTypeHandle(dwType);
//		KG_PROCESS_ERROR(KG3DEnvEffect::IsValidTypeHandle(Handle)&&KG3DEnvEffect::IsTypeTopEnvEff(Handle));
//		return KG3DEnvEffect::GetTopTypeIndex(Handle);
//Exit0:
//		return UINT_MAX;
//	}
	/*BOOL KG3DEnvEff_List::IsValidHandle( UINT Handle )
	{
		return KG3DEnvEffect::IsValidTypeHandle(Handle) ? TRUE : FALSE;
	}*/

	VOID KG3DEnvEff_List::GetExistEnvEffectTypes( std::vector<DWORD>& dwTypes )
	{
		/*dwTypes.clear();
		for (size_t i = 0; i < _countof(m_InnerArray); ++i)
		{
			KG3DEnvEffect* p = m_InnerArray[i];
			if (p)
			{
				dwTypes.push_back(p->GetType());
				if (p->GetSubEffectCount() > 0)
				{
					p->GetExistSubEffectTypeList(dwTypes);
				}
			}
		}*/
		dwTypes.clear();
		for (TypeCt::iterator it = m_Ct.begin(); it != m_Ct.end(); ++it)
		{
			KG3DEnvEffect* p = it->second;
			if (NULL != p)
			{
				dwTypes.push_back(p->GetType());
				if (p->GetSubEffectCount() > 0)
				{
					p->GetExistSubEffectTypeList(dwTypes);
				}
			}
		}
	}

	HRESULT KG3DEnvEff_List::Traverse( KG3DEnvEffectVisitor& Visitor )
	{
		//UINT Handle = 0;
		
		HRESULT hr = S_OK;
		/*GetFirstTopEnvEffect(Handle)*/;
		//for(KG3DEnvEffect* pEnvEff = ResetIterator(); NULL != pEnvEff; pEnvEff = NextIterator())
		for(TypeCt::iterator it = m_Ct.begin(); it != m_Ct.end(); ++it)
		{
			KG3DEnvEffect* pEnvEff = it->second;
			_ASSERTE(NULL != pEnvEff);
			hr = Visitor.Accept(*pEnvEff);
			KG_COM_PROCESS_ERROR(hr);
			hr = Visitor.Accept(*pEnvEff, KG3DENVEFF_HANDLE(pEnvEff->GetType(), 0));

			KG3DEnvEffect* pSubEffect = NULL;
			INT nCount = 0;
			if (SUCCEEDED(pEnvEff->GetSubEffectCount(&nCount)))
			{
				KG3DENVEFF_HANDLE TempHandle;
				for(int i = 0; i < nCount; ++i)
				{
					pSubEffect = pEnvEff->GetSubEffect(i, &TempHandle);
					if(NULL == pSubEffect)
						continue;
					hr = Visitor.Accept(*pSubEffect);
					KG_COM_PROCESS_ERROR(hr);
					hr = Visitor.Accept(*pSubEffect, TempHandle);
					KG_COM_PROCESS_ERROR(hr);					
				}
			}

			//GetNextTopEnvEffect(Handle,Handle);
		}
Exit0:
		return hr;
	}

	VOID KG3DEnvEff_List::TDDTest()
	{
		KG3DEnvEff_List listTemp;
		_ASSERTE(listTemp.Empty());

		KG3DEnvEffectDummy* pNewDummy = new KG3DEnvEffectDummy;
		{
			

			HRESULT hr = listTemp.Insert(KG3DTYPE_ENVEFF_DUMMY, pNewDummy);
			_ASSERTE(SUCCEEDED(hr));
			_ASSERTE(! listTemp.Empty());
			_ASSERTE(listTemp.GetValidElementCount() == 1);

			hr = listTemp.Insert(KG3DTYPE_ENVEFF_DUMMY, pNewDummy);
			_ASSERTE(FAILED(hr));

			hr = listTemp.Insert(KG3DTYPE_ENVEFF_SKYSYSTEM_CLOUD, reinterpret_cast<KG3DEnvEffect*>(0x0000001));
			_ASSERTE(FAILED(hr));
		}

		{
			KGCH::IteratorHolder itHolder;
			KG3DEnvEffect* p = listTemp.GetFirstTopEffect(itHolder);
			_ASSERTE(NULL != p && typeid(*p) == typeid(KG3DEnvEffectDummy));
			p = listTemp.GetNextTopEffect(itHolder);
			_ASSERTE(NULL == p);
		}

		{
			KG3DEnvEffect* pTemp = listTemp.Erase(KG3DTYPE_ENVEFF_DUMMY);
			_ASSERTE(pTemp == pNewDummy);
			KG_USE_ARGUMENT(pTemp);

			_ASSERTE(listTemp.Empty());

			SAFE_DELETE(pNewDummy);
		}
	}

	KG3DEnvEffect* KG3DEnvEff_List::GetFirstTopEffect( KGCH::IteratorHolder& holderFirst )
	{
		if (m_Ct.empty())
		{
			return NULL;
		}
		holderFirst = KGCH::CreateIteratorHolder(m_Ct);
		return m_Ct.begin()->second;
	}

	KG3DEnvEffect* KG3DEnvEff_List::GetNextTopEffect( KGCH::IteratorHolder& holderPrev )
	{
		TypeCt::iterator& it = reinterpret_cast<TypeCt::iterator&>(holderPrev);
		if (it != m_Ct.end())
		{
			++it;
			if (it != m_Ct.end())
			{
				return it->second;
			}
		}
		return NULL;
	}
	/*KG3DEnvEffect* KG3DEnvEff_List::ResetIterator()
	{
		if (m_Ct.empty())
		{
			return NULL;
		}
		m_CtIt = m_Ct.begin();
		KG3DEnvEffect* p = m_CtIt->second;
		return p;
	}

	KG3DEnvEffect* KG3DEnvEff_List::NextIterator()
	{
		if (m_CtIt == m_Ct.end())
		{
			return NULL;
		}
		++m_CtIt;
		if (m_CtIt == m_Ct.end())
		{
			return NULL;
		}
		KG3DEnvEffect* p = m_CtIt->second;
		return p;
	}*/
};///namespace

	

