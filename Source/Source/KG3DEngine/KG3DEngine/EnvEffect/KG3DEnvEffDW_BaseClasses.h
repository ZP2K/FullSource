////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffDW_BaseClasses.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-12-20 9:48:24
//  Comment     : Ϊ���ļ��Ķ����ԣ����ѻ���������
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DENVEFFDW_BASECLASSES_H_
#define _INCLUDE_KG3DENVEFFDW_BASECLASSES_H_

/*
#if _KG3D_DEBUG_ENABLE_OUTPUT_DWGC == 1
#ifndef _KG3D_DEBUG_SELF_ENABLE_OUTPUT
#define _KG3D_DEBUG_SELF_ENABLE_OUTPUT	1
#define _DEFINED_ENABLE_DEBUG_OUTPUT_	
#endif
#endif

//#include "TemplateLibs/LOKI_APP/KG3DLokiStructInterface.h"
#include "IEKG3DEnvEffDynamicWeather.h"
#include "IEKG3DEnvironment.h"
#include "KG3DEnvEffDW_KeyBase.h"
#include "KG3DRTTIType.h"
#include "KGINIFileHelpers.h"
////////////////////////////////////////////////////////////////////////////////
namespace KG3D_ENVIRONMENT_EFFECT
{
	///���ڹؼ�֡��ֻ����Index���͵Ĺؼ�֡
	//////////////////////////////////////////////////////////////////////////
	/ ************************************************************************ /
	/ *
	TKG3DDWGC_KeyTable0
	���ڴ�Ŷ�̬�����н���ṹ�ı��ģ�档 
	Ҫʹ�õĻ���Struct����ͷһ����Ա��DWORD�ε�m_dwModificationOption��
	��[]�޷���ӳ�Ա��ֻ����PushBack����ӣ�����������������з��䡣
	* /
	/ ************************************************************************ /
	
	template<class _STRUCT> //_cSection�Ǳ����õ�
	class TKG3DDWGC_KeyTable0 
		: protected std::map<int, _STRUCT>
		, public KG3DDWGC_KeyTableBase0
	{
	protected:
		typedef std::map<int, _STRUCT> TABLE;
		typedef TABLE::iterator iterator;
	private:
		int m_nlargestIndex;
		int m_nLastInterpIndex;
		KG3DDWGC_KeyTableManagerBase* m_pTableManager;
	public:
		enum{ DWGCKEY_TYPE = _STRUCT::DWGCKEY_TYPE,};
	public:		
		TKG3DDWGC_KeyTable0()
			:m_nlargestIndex(DW_KEYINDEX_INVALID)			///IndexΪ0������Ч����1��ʼ
			,m_nLastInterpIndex(DW_KEYINDEX_INTERP_END)
			,m_pTableManager(NULL)
		{

		}
		~TKG3DDWGC_KeyTable0()
		{
			Clear();
		}
		void DoNothing(){}//��������C4189,û��ʹ�õľֲ�����
		void Init(KG3DDWGC_KeyTableManagerBase* pTableManager)
		{
			_ASSERTE(!m_pTableManager && pTableManager);
			m_pTableManager = pTableManager;
		}
		_STRUCT* Find(const int& _Keyval)
		{
			if (empty() || true != CheckIsIndexValid(_Keyval))
				return NULL;
			iterator it = find(_Keyval);
			if (it == end())
				return NULL;
			return &(it->second);
		}		
		_STRUCT* GetKey(const int& _Keyval)
		{
			return this->Find(_Keyval);
		}
		_STRUCT& GetKeySolid(const int _Keyval)
		{
			_STRUCT* pStruct = GetKey(_Keyval);
			if(pStruct)
				return *pStruct;
			return GetInValidKey();
		}
		_STRUCT& GetInValidKey()
		{
			_STRUCT& errorStrut = TABLE::operator[](DW_KEYINDEX_INVALID);
			errorStrut.m_dwModificationOption = (DWORD)(DW_KEYTRAIT_ERROR);
			return errorStrut;
		}
		_STRUCT& GetInterpKey(int& _Keyval)///����û�б�ʹ�õĵ�һ��
		{
			int nTempNextIndexToInterp = m_nLastInterpIndex;
			if (nTempNextIndexToInterp >= DW_KEYINDEX_INTERP_END)
			{
				nTempNextIndexToInterp = DW_KEYINDEX_INTERP_START;
			}
			else
			{
				nTempNextIndexToInterp++;
			}
			iterator it = lower_bound(nTempNextIndexToInterp);
			while(it != TABLE::end() && it->first <= nTempNextIndexToInterp)///����Ǵ��ھͱ�ʾ���Index�ǿյ��ˣ��Ϳ���ֱ��ʹ����
			{
				nTempNextIndexToInterp = it->first + 1;///ע�����it++��ֱ��������һ���ؼ�֡�ˣ���it->first+1�Ļ������ܵ������ؼ�֮֡��Ŀ�϶�У��ǾͿ���ֱ��ʹ����
				it++;
			}
			_ASSERTE(TABLE::end() == find(nTempNextIndexToInterp));
			KG_PROCESS_ERROR(nTempNextIndexToInterp <= DW_KEYINDEX_INTERP_END);///û�пռ���
			//Exit1:
			{
				pair<iterator, bool> pr(TABLE::end(), false);
				pr = insert(TABLE::value_type(nTempNextIndexToInterp, _STRUCT()));
				KG_PROCESS_ERROR(pr.second);
				m_nLastInterpIndex = nTempNextIndexToInterp;
				iterator itResult = pr.first;
				_STRUCT& tempStructRef = itResult->second;

				tempStructRef.m_dwModificationOption |= DW_KEYTRAIT_INTERP;///��ʾ���Ǹ���ֵ�����Ĺؼ�֡
				_Keyval = itResult->first;
				return tempStructRef;
			}
Exit0:
			_Keyval = DW_KEYINDEX_INVALID;
			_ASSERTE(NULL&&"��ֵ�������Ҫ����");
			return GetInValidKey();
		}

	
		bool ReleaseKeyAndContainedKey(const int _Keyval, bool bOnlyInterp)
		{
			if(bOnlyInterp && !CheckIsIndexInterp(_Keyval))
			{
				goto Exit0;
			}
			{
				iterator it = find(_Keyval);
				if(it == this->end())
					return true;

				_STRUCT& RefStruct = (it->second);
				RefStruct.ReleaseContainedKey(m_pTableManager, bOnlyInterp);

				this->erase(it);
				return true;			
			}
Exit0:
			return false;
		}

	
		bool Empty()
		{
			return empty();
		}
		int Size()
		{
			return (int)TABLE::size();
		}
		/ ************************************************************************ /
		/ * @remark : Ϊ�˱���������Ԫ������������erastֱ��ɾ��IndexҲ�Ǻ����ģ�
		///��������κεĿ�ֵ���봦��
		///��ӱ���ʹ��PushBack����֤һ��������ֵ��
		* /
		/ ************************************************************************ /	
		_STRUCT* PushBack( _STRUCT& tempStruct, int& nRetIndex )
		{
			KG_PROCESS_ERROR(m_nlargestIndex + 1 < DW_KEYINDEX_INTERP_START);
			{
				if (m_nlargestIndex == DW_KEYINDEX_INVALID)///���ǳ�ֵ
				{
					m_nlargestIndex = DW_KEYINDEX_VALID_MIN;
				}
				else			  
				{
					m_nlargestIndex++;
				}		  		 
				pair<TABLE::iterator, bool> pr;
				pr = TABLE::insert(TABLE::value_type(m_nlargestIndex, tempStruct));
				if (pr.second != true)
				{
					if (m_nlargestIndex <= DW_KEYINDEX_VALID_MIN)
					{
						m_nlargestIndex = DW_KEYINDEX_INVALID;
					}
					else
					{
						m_nlargestIndex --;
					}
					goto Exit0;
				}
				nRetIndex = m_nlargestIndex;
#if defined(_DEBUG) | defined(DEBUG)
				TABLE::iterator itRet = pr.first;
				return &(itRet->second);
#else
				return &(pr.first->second);
#endif		
			}
Exit0:
			nRetIndex = DW_KEYINDEX_INVALID;
			return NULL;
		}
		virtual int PushBack(KG3DDWGC_KeyBase* pBaseKey)
		{
			_STRUCT* pStruct = dynamic_cast<_STRUCT*>(pBaseKey);
			KG_PROCESS_ERROR(pStruct);
			{
				INT nTemp = DW_KEYINDEX_INVALID;
				KG_PROCESS_ERROR(PushBack(*pStruct, nTemp));
				_ASSERTE(nTemp != DW_KEYINDEX_INVALID);
				return nTemp;
			}
Exit0:
			return DW_KEYINDEX_INVALID;
		}
		KG3DDWGC_KeyBase* GetOneEmptyKey(int& nRetIndex)
		{
			_STRUCT tempStruct;
			return this->PushBack(tempStruct, nRetIndex);
		}
		virtual KG3DDWGC_KeyBase* GetSpecialKey(DWORD dwTrait, int& _Keyval)
		{
			if (dwTrait == DW_KEYINDEX_INVALID)
			{
				_Keyval = dwTrait;
				return &GetInValidKey();
			}
			else if (dwTrait == DW_KEYINDEX_MUST_VALID)
			{
				_Keyval = DW_KEYINDEX_MUST_VALID;
				if (m_nlargestIndex <=DW_KEYINDEX_MUST_VALID )
				{
					m_nlargestIndex = DW_KEYINDEX_MUST_VALID;
				}
				return &TABLE::operator [](DW_KEYINDEX_MUST_VALID);
			}
			return NULL;
		}
		bool SaveMapSetting( IIniFile* pIniFile, LPCTSTR / *pKeyName* / )
		{
			_ASSERTE(pIniFile/ *&&pKeyName* /);
			//int nSize = (int)TABLE::size();
			char szTableKeyName[DWGC_TABLE_MAX_KEYNAME_SIZE];
			///���������
			const TCHAR* pKeyTypeName = g_cKG3DRTTITypeManager.GetName(DWGCKEY_TYPE);
			_ASSERTE(pKeyTypeName);
			{
				sprintf_s(szTableKeyName, DWGC_TABLE_MAX_KEYNAME_SIZE, "KG3DDWGC_Key_%s_TABLE", pKeyTypeName);
			}
			KG_CUSTOM_HELPERS::EraseSection(pIniFile, szTableKeyName);
			char szKeyName[DWGC_TABLE_MAX_KEYNAME_SIZE];
			char szKeyValue[DWGC_TABLE_MAX_KEYVALUE_SIZE];
			TABLE::iterator it;
			for (it = TABLE::begin(); it != TABLE::end(); it++)
			{
				int nIndex = it->first;
				if (true != CheckIsIndexValidAndNotInterp(nIndex))
				{
					continue;///���ñ��泬�����ֵ�ģ���Щ����Ч��
				}
				_STRUCT&  tempStruct = it->second;
				sprintf_s(szKeyName, "%d", nIndex);
				sprintf_s(szKeyValue, "KG3DDWGC_Key_%s_%d", pKeyTypeName, nIndex);
				if (tempStruct.SaveMapSetting(pIniFile, szKeyValue))
				{					
					pIniFile->WriteString(szTableKeyName, szKeyName, szKeyValue);
				}
#if defined(DEBUG) | defined(_DEBUG)
				else
				{
					_KG3D_DEBUG_SELF_ASSERTE(NULL&&"���ڵ�Structȴ������ȷ���棬Ҫ��鷢����ʲô");
				}
#endif
			}
			return true;
		}

		bool LoadMapSetting( IIniFile* pIniFile, LPCTSTR / *pKeyName* / )
		{
			_ASSERTE(pIniFile && m_pTableManager);
			TCHAR szTableKeyName[DWGC_TABLE_MAX_KEYNAME_SIZE];
			///���������
			Clear();
			const TCHAR* pKeyTypeName = g_cKG3DRTTITypeManager.GetName(DWGCKEY_TYPE);
			_ASSERTE(pKeyTypeName);
			_stprintf_s(szTableKeyName, DWGC_TABLE_MAX_KEYNAME_SIZE, _T("KG3DDWGC_Key_%s_TABLE"), pKeyTypeName);
			TCHAR szKeyName[DWGC_TABLE_MAX_KEYNAME_SIZE];
			TCHAR szKeyValue[DWGC_TABLE_MAX_KEYVALUE_SIZE];
			*szKeyName = '\0';
			*szKeyValue = '\0';
			int nTempLargestIndex = 0;
			//while(pIniFile->GetNextKey(szTableKeyName, szKeyName, szKeyName))
			while(KG_CUSTOM_HELPERS::GetNextKey(pIniFile, szTableKeyName, szKeyName, szKeyName))
			{
				int nIndex = _ttoi(szKeyName);
				if (true != CheckIsIndexValidAndNotInterp(nIndex))
				{
					continue;///��Ч����
				}
				nTempLargestIndex = max(nIndex, nTempLargestIndex);///Ϊ��m_nlargestIndex������һֱ��¼һ������Index
				//pIniFile->GetString(szTableKeyName, szKeyName, "", szKeyValue, sizeof(szKeyValue));
				if(! KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, szTableKeyName, szKeyName, _T(""), szKeyValue))
					continue;

				if (*szKeyValue != '\0')
				{
					int nTempIfExist = KG_CUSTOM_HELPERS::IsSectionExist(pIniFile, szKeyValue);//(pIniFile->IsSectionExist(szKeyValue));
					if (!nTempIfExist )
					{
						continue;///û�����Section����Ҳ�׶�
					}
					_STRUCT tempStruct;
					if (tempStruct.LoadMapSetting(pIniFile, szKeyValue) && tempStruct.IsLegal(m_pTableManager))
					{
						pair<TABLE::iterator, bool> pr;
						pr = TABLE::insert(TABLE::value_type(nIndex, tempStruct));
						if (pr.second == true)
						{
							m_nlargestIndex = max(m_nlargestIndex, nTempLargestIndex);///Ϊ��m_nlargestIndex������
						}
					}
				}
			}
			if (TABLE::empty())///���һ����û�ж��������Ǿ�False��
			{
				return false;
			}
			return true;
		}

#if defined(DEBUG) | defined(_DEBUG)
		void DebugPrintTable()
	{
		char czTitle[64];
		const TCHAR* pKeyTypeName = g_cKG3DRTTITypeManager.GetName(DWGCKEY_TYPE);
		_ASSERTE(pKeyTypeName);
		_stprintf_s(czTitle, _countof(czTitle), "KG3DDWGC_Key_%s_TABLE", pKeyTypeName);
		_KG3D_DEBUG_OUTPUT1("%s\n", czTitle);
		int nTotalKeyInARow = 0;
		std::ostringstream szOutBuffer;
		for (iterator it = begin()
			; it != end(); it++, nTotalKeyInARow++)
		{
			int nIndex = it->first;
			_STRUCT& tempStruct = (it->second);
			szOutBuffer.clear();
			tempStruct.DebugPrint(szOutBuffer);
			_KG3D_DEBUG_OUTPUT2("Index:%d\t%s\n", nIndex, szOutBuffer.str());
		}
	}
#endif
		///<�ӿ�>
		virtual ULONG GetSize()
		{
			return static_cast<ULONG>(TABLE::size());
		}
		virtual HRESULT GetType(DWORD* pdwType)
		{
			if (!pdwType)
			{
				return E_INVALIDARG;
			}
			*pdwType = (DWORD)(DWGCKEY_TYPE);
			return S_OK;
		}
		virtual HRESULT GetElementIndices(INT* pnIndex, INT nArraySize, INT* pnRealWritedIndexCount)///�õ�ȫ����Index
		{
			if (empty())
			{
				return S_FALSE;
			}
			int nSize = static_cast<int>(TABLE::size());
			int nWritedIndexCount = 0;
			if (!pnIndex || nArraySize < nSize || !pnRealWritedIndexCount)
			{
				return E_INVALIDARG;
			}
			for (iterator it = begin(); it != end() && nWritedIndexCount < nSize; it++)
			{
				if (CheckIsIndexValidAndNotInterp(it->first))
				{
					pnIndex[nWritedIndexCount] = it->first;
					nWritedIndexCount++;
				}				
			}
			*pnRealWritedIndexCount = nWritedIndexCount;
			return S_OK;			
		}
		virtual HRESULT RemoveKey(INT nIndex)
		{
			if(this->erase(nIndex) > 0)
			{
				return S_OK;
			}
			return E_FAIL;
		}
		virtual bool Clear()
		{
			m_nlargestIndex = DW_KEYINDEX_INVALID;
			m_nLastInterpIndex = DW_KEYINDEX_INTERP_END;
			clear();
			return true;
		}

	public:
		//typedef typename _STRUCT::ConverterType ConvertorType;
		/ *
		enum
				{
					FIRST_ELEMENT_OFFSET = offsetof(_STRUCT, m_dwModificationOption) + sizeof(DWORD),
				};* /
		
		virtual HRESULT GetDescription(INT nIndex, LPTSTR pDesc, INT nBufferSize)
		{

/ *
			ConvertorType& RefConvertor = _STRUCT::GetConverter();
			int nRet = 0;
			KG_PROCESS_ERROR(pDesc && nIndex >= 0 && nIndex < RefConvertor.Size());
			nRet = _tcscpy_s(pDesc, nBufferSize, RefConvertor.ElementDescription(nIndex));
			KG_PROCESS_ERROR(nRet == 0);
			return S_OK; 
Exit0:
			return E_FAIL;
* /

			return E_FAIL;
		}
		virtual HRESULT KeyElementCount(INT* pSize)
		{

/ *
			if (!pSize)
			{
				return E_INVALIDARG;
			}
			ConvertorType& RefConvertor = _STRUCT::GetConverter();
			*pSize =  RefConvertor.Size();
			return S_OK;* /


			return E_FAIL;
		}
		virtual HRESULT ConvertValueString(INT nWhichKey, INT nWhichPart, LPTSTR tczString, INT nBufferSize, BOOL bGetOrSet)
		{

/ *
			ConvertorType& RefConvertor = _STRUCT::GetConverter();
			_STRUCT* pStruct = Find(nWhichKey);
			KG_PROCESS_ERROR(pStruct && tczString && nWhichPart >= 0 && nWhichPart < RefConvertor.Size());
			if (bGetOrSet)///Get
			{
				RefConvertor.SetValue<FIRST_ELEMENT_OFFSET>(pStruct, nWhichPart);
				std::tstring TempString = RefConvertor.Value2String(nWhichPart);
				int nRet =_tcscpy_s(tczString, nBufferSize, TempString.c_str());
				KG_PROCESS_ERROR(nRet == 0);
			}
			else ///Set
			{
				BOOL bRet = RefConvertor.String2Value(nWhichPart, tczString);
				_STRUCT TempKey = *pStruct;
				KG_PROCESS_ERROR(bRet);
				RefConvertor.GetValue<FIRST_ELEMENT_OFFSET>(&TempKey, nWhichPart);
				KG_PROCESS_ERROR(TempKey.IsLegal(m_pTableManager));
				*pStruct = TempKey;
			}
			return S_OK;
Exit0:
			return E_FAIL;* /


			return E_FAIL;
		}
		///</�ӿ�>
	};

	
};///End of KG3D_ENVIRONMENT_EFFECT

#if defined(_DEFINED_ENABLE_DEBUG_OUTPUT_)
#if defined(_KG3D_DEBUG_SELF_ENABLE_OUTPUT)
#undef _KG3D_DEBUG_SELF_ENABLE_OUTPUT
#endif
#endif
*/

#endif //_INCLUDE_KG3DENVEFFDW_BASECLASSES_H_
