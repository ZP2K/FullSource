
#if !defined(AFX_KResourceManager_H__9C3F564E_0218_4D56_AF1D_1E90C01E8AD6__INCLUDED_)
#define AFX_KResourceManager_H__9C3F564E_0218_4D56_AF1D_1E90C01E8AD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PATH_LENGTH 256

#include "Engine/KGLog.h"
#include "Engine/KMutex.h"
#include "kg3dinterface.h"
#include "KG3DResourcePacker.h"
using namespace std;
using namespace stdext;

#define RELEASE_INQUEUE  0
#define RELEASE_ATONCE   1

extern DWORD g_dwMainThreadID;
enum
{
	KG3DGET1NEWRESOURCE_FILE_NAME_FOR_HASH = 0x12345678
};

struct KG3DGet1NewResourceOptionStruct 
{
	DWORD dwOptionIdentifier;///���������־������ʲôOption
	LPCTSTR czFileName;
};

//GetTypeInfoByFileNameʹ�õĲ���
struct TypeInfo
{
	TCHAR strExt[MAX_PATH];//��׺��
	DWORD dwType;          //����
	BOOL  bAlwaysNew;      //�Ƿ������ؽ�
};
extern float g_fMemoryCoefficient;
extern DWORD g_dwID2Find;
extern bool g_FindInReleaseList(IKG3DResourceBase *pRes);
extern KG3DEngineOption g_cEngineOption;

extern TypeInfo g_GloableDefaultResourceType;

template <class TResource>
class TKG3DResourceManagerBase : public IKG3DResourceManager
{
	friend  TResource;
public:	
	typedef std::map<DWORD,TResource*> TypeResourceMap;
	typedef std::list<TResource*> TypeExclusiveList;
	typedef std::list<TResource*> TypeReleaseQueue;
	struct ReleaseCmd
	{
		TResource* pT;
		DWORD dwOption;
		bool operator ==(const ReleaseCmd& A)
		{
			return pT == A.pT;
		}
	};
	
	TKG3DResourceManagerBase()
		:m_pDefaultType(NULL)
		,m_RefTable(NULL)
		,m_RefTableSize(0)
        ,m_dwReleaseQueueSize(200)
	{
	}

	virtual ~TKG3DResourceManagerBase()
	{
        ASSERT(m_lstReleaseQueue.empty());
        ASSERT(m_mapResource.empty());
        ASSERT(m_listExclusiveRes.empty());
	}

	virtual HRESULT Init()
	{
		UnInit();

        ASSERT(g_fMemoryCoefficient >= 0.0f);
        m_dwReleaseQueueSize = (unsigned)(m_dwReleaseQueueSize * g_fMemoryCoefficient);

		m_pDefaultType = &g_GloableDefaultResourceType;
		return S_OK;
	}

	virtual HRESULT UnInit()
	{
		m_Lock.Lock();

		TypeExclusiveList::iterator itExclusive = m_listExclusiveRes.begin();
		while (itExclusive != m_listExclusiveRes.end())
		{
			TResource *pResource = *itExclusive;

			if (pResource->m_scName.size())
			{
				KGLogPrintf(KGLOG_ERR, "��Դ%sδ�����ͷ�.", pResource->m_scName.c_str());
			}
			else
			{
				KGLogPrintf(KGLOG_ERR, "������Դ�ͷ�. Type = %d", pResource->GetType());
			}
			pResource->UnInit();
			itExclusive++;
		}

		TypeResourceMap::iterator m = m_mapResource.begin();
		while(m != m_mapResource.end())
		{
			TResource* pResource = m->second;
			if (FAILED(pResource->UnInit()))
			{
				KGLogPrintf(KGLOG_ERR,"�ͷ���Դʧ��");
				_KG3D_DEBUG_OUTPUT0("�ͷ���Դʧ��");
			}

			if (pResource->m_scName.size())
			{
				KGLogPrintf(KGLOG_ERR, "��Դǿ���ͷ� %s",pResource->m_scName.c_str());
			}
			else
			{
				KGLogPrintf(KGLOG_ERR, "������Դ�ͷ�. Type = %d", pResource->GetType());
			}
			m++;
		}

		TypeReleaseQueue::iterator i = m_lstReleaseQueue.begin();
		while(i != m_lstReleaseQueue.end())
		{
			TResource* pResource = *i;
			if (FAILED(pResource->UnInit()))
			{
				if (pResource->m_scName.size())
				{
					KGLogPrintf(KGLOG_ERR,"�ͷ���Դ%s ʧ��.", pResource->m_scName.c_str());
				}
				else
				{
					KGLogPrintf(KGLOG_ERR, "������Դ�ͷ�. Type = %d", pResource->GetType());
				}
				_KG3D_DEBUG_OUTPUT0("�ͷ���Դʧ��");
			}
			i++;
		}

		//�����Ǳ�������д��, ��Ϊ����ģ�͵����ù�ϵ, ���ܵ���ǿ���ͷű������������ͷŵ���Դ
		itExclusive = m_listExclusiveRes.begin();
		while (itExclusive != m_listExclusiveRes.end())
		{
			TResource *pResource = *itExclusive;
			SAFE_DELETE(pResource);
			itExclusive++;
		}

		m = m_mapResource.begin();
		while (m != m_mapResource.end())
		{
			TResource* pResource = m->second;
			SAFE_DELETE(pResource);
			m++;
		}
		m_mapResource.clear();

		i = m_lstReleaseQueue.begin();
		while(i != m_lstReleaseQueue.end())
		{
			TResource* pRes = *i;
			SAFE_DELETE(pRes);
			i++;
		}
		m_lstReleaseQueue.clear();
		m_listExclusiveRes.clear();

		/*i = m_listReleaseMultiThread.begin();
		while(i != m_listReleaseMultiThread.end())
		{
			TResource* pRes = *i;
			SAFE_DELETE(pRes);
			i++;
		}*/
		m_listReleaseMultiThread.clear();

		m_RefTable = NULL;
		m_RefTableSize = 0;
		m_pDefaultType = NULL;

		m_Lock.Unlock();
        return S_OK;
	}

	virtual HRESULT NewOneResourse(unsigned uType, IKG3DResourceBase **ppRetResource)
	{
		return Get1NewResource((TResource**)ppRetResource,uType,0);
	}

	virtual HRESULT Get1NewResource(TResource** ppT, DWORD dwType, DWORD_PTR Option)
	{
		HRESULT hRetCode = E_FAIL;
		HRESULT hResult  = E_FAIL;
		
		DWORD dwIDToSet  = 0;

		m_Lock.Lock();
		
		KG_PROCESS_ERROR(ppT);
		*ppT = NULL;

		hResult = this->Get1NewResourcePointer(ppT, dwType, Option);
		KG_COM_PROCESS_ERROR(hResult);

		(*ppT)->AddRef();
		/*
		ͳһ�ⲿ��Դ���ڲ�������Դ��ͳһ����ID��Ϊ���ʹ�ã����ʻ���Ӱ�ȫ��Ҳ������ʵ���ⲿ����
		�ⲿͨ����Option���KG3DGet1NewResourceOptionStruct*�Ľṹ��Ȼ��ṹ��dwOptionIdentifier��Ϊ
		KG3DGET1NEWRESOURCE_FILE_NAME_FOR_HASH��Ϊ�˷�ֹ�����ڴ���ʣ��������Ϳ�����Struct�е�czFileName
		��Ա�õ�һ���ַ����ˡ������Ϳ���Ԥ����map��������Դ��������ַ���Ҳ���Եõ�һ��ID��Ϊ���ʹ��
		*/
		if (Option)
		{
			typedef TypeResourceMap typehashmap;
			KG3DGet1NewResourceOptionStruct* pNROStruct = (KG3DGet1NewResourceOptionStruct*)Option;
			if (pNROStruct->dwOptionIdentifier == KG3DGET1NEWRESOURCE_FILE_NAME_FOR_HASH)
			{
				KG_PROCESS_ERROR(pNROStruct->czFileName);
				DWORD dwID = g_FileNameHash(pNROStruct->czFileName);
				std::pair<typehashmap::iterator, bool> pairRet 
					= m_mapResource.insert(typehashmap::value_type(dwID, *ppT));
				KG_PROCESS_ERROR(pairRet.second);///�Ѵ��ڵľͻ�insertʧ��
				dwIDToSet = dwID;		
			}
		}
		(*ppT)->SetID(dwIDToSet);
		hRetCode = S_OK;
Exit0:
		if(FAILED(hRetCode) && ppT)
		{
			SAFE_RELEASE(*ppT);
		}

		m_Lock.Unlock();
		return hRetCode;
	}

	virtual HRESULT Get1NewResourcePointer(TResource** ppT, DWORD dwType, DWORD_PTR Option)
	{
		HRESULT hRetCode = E_FAIL;
		KG_PROCESS_ERROR(ppT);

		(*ppT) = new TResource();
		KGLOG_PROCESS_ERROR(*ppT);

		(*ppT)->Init();
		(*ppT)->SetType(dwType);
		hRetCode = S_OK;
Exit0:
		return hRetCode;
	}

	HRESULT ReportResourceUse(LPSTR pTableName)
	{
		DWORD dwSize = GetResourceCount();
		KGLogPrintf(KGLOG_ERR, "=======================================================");
		KGLogPrintf(KGLOG_ERR, "%d %s Resource",dwSize,pTableName);

		int N = 0;
		TypeResourceMap::iterator m = m_mapResource.begin();
		while(m != m_mapResource.end())
		{
			TResource* pResource = m->second;
			if(pResource)
				KGLogPrintf(KGLOG_ERR, "%d %s",N,pResource->m_scName.c_str());
			m++;
			N++;
		}

		TypeExclusiveList::iterator itExclusive = m_listExclusiveRes.begin();
		while (itExclusive != m_listExclusiveRes.end())
		{
			TResource *pResource = *itExclusive;
			if(pResource)
				KGLogPrintf(KGLOG_ERR, "%d %s",N,pResource->m_scName.c_str());
			itExclusive++;
			N++;
		}
		return S_OK;
	}

	DWORD GetResourceCount()
	{
		return (DWORD)(m_mapResource.size() + m_listExclusiveRes.size());
	}

	virtual HRESULT SetBalance(unsigned uSizeMemeryKB, unsigned uSizeVideoKB)
	{
		return S_OK;
	}

	virtual HRESULT LoadResourceFromFile(const char cszFileName[], 
		unsigned uFileNameHash, 
		unsigned uOption, 
		IKG3DResourceBase **ppRetResource)
	{
		return LoadResourceFromFileEx(cszFileName, uFileNameHash, uOption, NULL, NULL, ppRetResource);
	}

	virtual HRESULT LoadResourceFromFileEx(const char cszFileName[], 
		unsigned uFileNameHash,
		unsigned uOption, 
		DWORD_PTR WParam, 
		DWORD_PTR LParam, 
		IKG3DResourceBase **ppRetResource)
	{
		TypeInfo* pTypeInfo = NULL;

		HRESULT hRetCode    = E_FAIL;
		HRESULT hResult     = E_FAIL;
		TResource* pTReturn = NULL;
		DWORD   dwID        = 0;
		typedef std::pair<TypeResourceMap::iterator, bool> TypePairRet;

		KG_PROCESS_ERROR(ppRetResource);
		KG_PROCESS_ERROR(cszFileName);

        ASSERT(*ppRetResource == NULL);

		m_Lock.Lock();

		hResult = GetTypeInfoByFileName(&pTypeInfo, cszFileName);
		KG_COM_PROCESS_ERROR(hResult);
		_ASSERTE(pTypeInfo);

		dwID = g_FileNameHash(cszFileName);

		if(! pTypeInfo->bAlwaysNew)
		{
			//�����Ѿ����ص���Դ
			map<DWORD, TResource*>::iterator h = m_mapResource.find(dwID);
			if (h != m_mapResource.end())
			{
				//�ҵ���,ֱ��ʹ��
				pTReturn = h->second;
				KG_PROCESS_ERROR(pTReturn);

				pTReturn->RefreshOption(uOption);
				pTReturn->AddRef();

				ReleaseCmd cmd;//ȷ�����õ���Դ���ӳ�ж�ض�����ɾ��
				cmd.pT = pTReturn;
				cmd.dwOption = 0;
				m_listReleaseMultiThread.remove(cmd);

				goto Exit1;
			}
			//�����Ƿ����ͷŶ�����
			g_dwID2Find = dwID;
			TypeReleaseQueue::iterator itErase = find_if(m_lstReleaseQueue.begin(), 
				m_lstReleaseQueue.end(), 
				&g_FindInReleaseList);
			if (itErase != m_lstReleaseQueue.end())
			{
				pTReturn = *itErase;
				KG_PROCESS_ERROR(pTReturn);

				pTReturn->RefreshOption(uOption);
				pTReturn->AddRef();
				m_lstReleaseQueue.erase(itErase);
				
				TypePairRet pairRet = m_mapResource.insert(map<DWORD, TResource*>::value_type(dwID, pTReturn));
				KG_PROCESS_ERROR(pairRet.second);///���insert����ID�Ļ���Insert��ʧ��

				goto Exit1;
			}
		}

		//AlwaysNew����Դ
		hResult = Get1NewResourcePointer(&pTReturn, pTypeInfo->dwType, uOption);
		KG_COM_PROCESS_ERROR(hResult);
		
		pTReturn->AddRef();
		pTReturn->SetID(dwID);
		//�벻Ҫɾ���������У����������һ�����Լ��
		//pTReturn->SetType(dwType);//����ƶ���Get1NewResourcePointer
		//pTReturn->m_scName = cszFileName;//�������������ǿ��ִ�У�ÿ��Type�Լ���Load�����������

		hResult = pTReturn->LoadFromFileEx(cszFileName, 0, uOption, WParam, LParam);
		if(FAILED(hResult))
		{
			if(pTReturn)
			{
				pTReturn->UnInit();
			}
			SAFE_DELETE(pTReturn);//ע�⣬���ʱ���������û�з��뵽map���棬����Release�ǲ��ɹ��ģ�����ʹ��Delete
			pTReturn = NULL;
			goto Exit0;
		}

		if (pTypeInfo->bAlwaysNew)
		{
			m_listExclusiveRes.push_back(pTReturn);
		}
		else
		{
			m_mapResource[dwID] = pTReturn;
		}
Exit1:
		*((TResource**)ppRetResource) = pTReturn;
		hRetCode = S_OK;
Exit0:
		if(FAILED(hRetCode))
		{
			SAFE_RELEASE(pTReturn);	
		}

		m_Lock.Unlock();

		return hRetCode;
	}

	virtual HRESULT GetResource(TResource** ppT, DWORD dwID)//�������
	{
		HRESULT hRetCode = E_FAIL;
		TypeResourceMap::iterator i;
		
		KG_PROCESS_ERROR(ppT);

		m_Lock.Lock();

        i = m_mapResource.find(dwID);
		KG_PROCESS_ERROR(i != m_mapResource.end());

		(*ppT) = i->second;
		(*ppT)->AddRef();

		ReleaseCmd cmd;//ȷ�����õ���Դ���ӳ�ж�ض�����ɾ��
		cmd.pT = (*ppT);
		cmd.dwOption = 0;
		m_listReleaseMultiThread.remove(cmd);

		
		hRetCode = S_OK;
Exit0:
		m_Lock.Unlock();
		return hRetCode;
	}

	virtual HRESULT CheckReleaseMultiThread()
	{
		DWORD dwThreadId = ::GetCurrentThreadId();
		KGLOG_PROCESS_ERROR(dwThreadId == g_dwMainThreadID);

		m_Lock.Lock();
		if(m_listReleaseMultiThread.size())
		{
			std::list<ReleaseCmd>::iterator i = m_listReleaseMultiThread.begin();
			std::list<ReleaseCmd>::iterator iend = m_listReleaseMultiThread.end();
			while (i!=iend)
			{
				ReleaseCmd& cmd = *i;
				_ReleaseResource(cmd.pT,cmd.dwOption);
				i++;
			}
			m_listReleaseMultiThread.clear();

		}
		m_Lock.Unlock();
Exit0:
		return S_OK;
	}

	virtual HRESULT ReleaseResource(TResource* pT, DWORD dwOption = RELEASE_INQUEUE)
	{
		HRESULT hr = E_FAIL;

		DWORD dwThreadId = ::GetCurrentThreadId();
		m_Lock.Lock();

		if(dwThreadId == g_dwMainThreadID)
		{
			ReleaseCmd cmd;
			cmd.pT = pT;
			cmd.dwOption = dwOption;
			m_listReleaseMultiThread.remove(cmd);
			hr = _ReleaseResource(pT,dwOption);
		}
		else
		{			
			ReleaseCmd cmd;
			cmd.pT = pT;
			cmd.dwOption = dwOption;
			m_listReleaseMultiThread.push_back(cmd);
			hr = S_OK;
		}
		m_Lock.Unlock();
		return hr;
	}

	virtual HRESULT _ReleaseResource(TResource* pT, DWORD dwOption = RELEASE_INQUEUE)
	{
		HRESULT hRetCode   = E_FAIL;
		HRESULT hResult    = E_FAIL;
		TypeInfo* pTypeInfo = NULL;

		DWORD   dwID       = 0;
		TypeResourceMap::iterator itErease;

		KG_PROCESS_ERROR(pT);

		dwID = pT->GetID();

		hResult = GetTypeInfoByResourceType(&pTypeInfo, pT->GetType());
		KG_COM_PROCESS_ERROR(hResult);
		_ASSERTE(pTypeInfo);

		if (pTypeInfo->bAlwaysNew)
		{
			TypeExclusiveList::iterator itErease = find(m_listExclusiveRes.begin(), 
				m_listExclusiveRes.end(), 
				pT);
			if(itErease != m_listExclusiveRes.end())//;KG_PROCESS_ERROR(itErease != m_listExclusiveRes.end());
				m_listExclusiveRes.erase(itErease);	

			pT->UnInit();
			SAFE_DELETE(pT);
			goto Exit1;
		}
				
		if (dwID == 0)
		{
			pT->UnInit();
			SAFE_DELETE(pT);
			goto Exit1;
		}

		itErease = m_mapResource.find(dwID);
		KG_PROCESS_ERROR(itErease != m_mapResource.end());

		m_mapResource.erase(itErease);
		
		if (dwOption == RELEASE_INQUEUE)
		{
			TypeReleaseQueue::iterator itFind = find(m_lstReleaseQueue.begin(), m_lstReleaseQueue.end(), pT);
			//�����ܳ������ͷŶ����У� ͬʱ�ֱ��ٴμ�����е����
			KG_PROCESS_ERROR(itFind == m_lstReleaseQueue.end());

			m_lstReleaseQueue.push_back(pT);

			if (m_lstReleaseQueue.size() > m_dwReleaseQueueSize)
			{
				//���г��ȳ�����Ԥ��ֵ�������ʱ����õ�
				TResource *pResource = *(m_lstReleaseQueue.begin());
				pResource->UnInit();
				SAFE_DELETE(pResource);
				m_lstReleaseQueue.pop_front();
			}
			goto Exit1;
		}
		pT->UnInit();
		SAFE_DELETE(pT);
Exit1:
		hRetCode = S_OK;
Exit0:
		return hRetCode;
	}


	HRESULT OnLostDevice()
	{
		m_Lock.Lock();

        TypeResourceMap::iterator itResource       = m_mapResource.begin();
		TypeResourceMap::iterator itResourceEnd    = m_mapResource.end();
		TypeExclusiveList::iterator itExclusive    = m_listExclusiveRes.begin();
		TypeExclusiveList::iterator itExclusiveEnd = m_listExclusiveRes.end();
		TypeReleaseQueue::iterator  itRelease      = m_lstReleaseQueue.begin();
		TypeReleaseQueue::iterator  itReleaseEnd   = m_lstReleaseQueue.end();

		while (itResource != itResourceEnd)
		{
			ASSERT(itResource->second);
			itResource->second->OnLostDevice();
			itResource++;
		}

		while (itExclusive != itExclusiveEnd)
		{
			ASSERT(*itExclusive);
			(*itExclusive)->OnLostDevice();
			itExclusive++;
		}

		while (itRelease != itReleaseEnd)
		{
			ASSERT(*itRelease);
			(*itRelease)->OnLostDevice();
			itRelease++;
		}

        m_Lock.Unlock();
        return S_OK;
	}

	HRESULT OnResetDevice()
	{
		m_Lock.Lock();

        TypeResourceMap::iterator itResource       = m_mapResource.begin();
		TypeResourceMap::iterator itResourceEnd    = m_mapResource.end();
		TypeExclusiveList::iterator itExclusive    = m_listExclusiveRes.begin();
		TypeExclusiveList::iterator itExclusiveEnd = m_listExclusiveRes.end();
		TypeReleaseQueue::iterator  itRelease      = m_lstReleaseQueue.begin();
		TypeReleaseQueue::iterator  itReleaseEnd   = m_lstReleaseQueue.end();

		while (itResource != itResourceEnd)
		{
			ASSERT(itResource->second);
			itResource->second->OnResetDevice();
			itResource++;
		}

		while (itExclusive != itExclusiveEnd)
		{
			ASSERT(*itExclusive);
			(*itExclusive)->OnResetDevice();
			itExclusive++;
		}

		while (itRelease != itReleaseEnd)
		{
			ASSERT(*itRelease);
			(*itRelease)->OnResetDevice();
			itRelease++;
		}

		m_Lock.Unlock();
        return S_OK;
	}

public :
	virtual HRESULT GetTypeInfoByFileName(TypeInfo** ppTypeInfo, const char cszFileName[])
	{
		HRESULT hRetCode = E_FAIL;
		TCHAR strExt[MAX_PATH];
		KG_PROCESS_ERROR(ppTypeInfo);

		_splitpath_s(cszFileName, 
			NULL,
			0, 
			NULL,
			0, 
			NULL,
			0, 
			strExt, 
			MAX_PATH);
		_strlwr_s(strExt, MAX_PATH * sizeof(TCHAR));

		for (size_t i = 0; i < m_RefTableSize; i++)
		{
			if (_tcscmp(strExt, m_RefTable[i].strExt) == 0) //����Ӧ����Сд�Ƚ�
			{
				*ppTypeInfo = &m_RefTable[i];
				hRetCode = S_OK;
				break;
			}
		}

		if (FAILED(hRetCode) && m_pDefaultType)
		{
			*ppTypeInfo = m_pDefaultType;
			hRetCode = S_OK;
		}

#if defined(DEBUG) | defined(_DEBUG)
		if (m_RefTableSize == 0)
		{
			_ASSERTE(m_pDefaultType);//���û�ж���TypeInfo����ô����Ҫ��Table��Init�����ж���DefaultType����Ȼ������������ʧ�ܣ��Ӷ���Դ�����ͷ�
		}
#endif
		if (FAILED(hRetCode) && m_RefTableSize == 0 && ! m_pDefaultType)
		{
			*ppTypeInfo = &g_GloableDefaultResourceType;
		}
Exit0:
		return hRetCode;
	}

	virtual HRESULT GetTypeInfoByResourceType(TypeInfo** ppTypeInfo, DWORD dwType)
	{
		HRESULT hRetCode = E_FAIL;
		KG_PROCESS_ERROR(ppTypeInfo);

		for (size_t i = 0; i < m_RefTableSize; i++)
		{
			if (m_RefTable[i].dwType == dwType)
			{
				*ppTypeInfo = &m_RefTable[i];
				hRetCode = S_OK;
				break;
			}
		}

#if defined(DEBUG) | defined(_DEBUG)
		if (m_RefTableSize == 0)
		{
			_ASSERTE(m_pDefaultType);//���û�ж���TypeInfo����ô����Ҫ��Table��Init�����ж���DefaultType����Ȼ������������ʧ�ܣ��Ӷ���Դ�����ͷ�
		}
#endif
		if (FAILED(hRetCode) && m_pDefaultType)
		{
			*ppTypeInfo = m_pDefaultType;
			hRetCode = S_OK;
		}

Exit0:
		return hRetCode;
	}

protected :
	KMutex            m_Lock;
    DWORD             m_dwReleaseQueueSize;
	TypeReleaseQueue  m_lstReleaseQueue;//��Ҫ�ͷŵ���Դ����
	TypeResourceMap	  m_mapResource;
	TypeExclusiveList m_listExclusiveRes;
	TypeInfo*         m_RefTable; //GetTypeInfoByFileName�Ĳο���, ��Ҫ��ʵ����Init�����г�ʼ��, ���ǲ���Ҫ��UnInit�и���
	size_t            m_RefTableSize;//�ο���Ĵ�С
	TypeInfo*         m_pDefaultType;//�ڲο������ƥ��ʱ��Ĵ���, ����ǿ��򷵻�ʧ��,д�����ʱ��TypeInfo��strExtҪСд����Ҫ��.
	std::list<ReleaseCmd>m_listReleaseMultiThread;
};

template <class TResource>
	class TKG3DResourcePool
	{
		friend  TResource;
	public:	
		KMutex m_Lock;
		DWORD m_dwMaxPool;
		std::list<TResource*>m_listResourcePool;
		std::set<TResource*>m_setResourceUnDelete;//��¼û�б��ͷŵ���Դ

		TResource* RequestResource()
		{
			m_Lock.Lock();
			TResource* pRes = NULL;
			if(m_listResourcePool.size()>0)
			{
				pRes = *m_listResourcePool.begin();
				m_listResourcePool.pop_front();
			}
			else
			{
				pRes = new TResource();
				m_setResourceUnDelete.insert(pRes);
			}
			m_Lock.Unlock();
			return pRes;
		}

		void FreeResource(TResource* pRes)
		{
			m_Lock.Lock();
			if(m_listResourcePool.size() < m_dwMaxPool)
			{
				pRes->UnInit();
				m_listResourcePool.push_back(pRes);
			}
			else
			{
				pRes->UnInit();

				std::set<TResource*>::iterator i = m_setResourceUnDelete.find(pRes);
				if(i!=m_setResourceUnDelete.end())
				{
					m_setResourceUnDelete.erase(i);
					SAFE_DELETE(pRes);
				}
				else
				{
					ASSERT(0);
				}
			}
			m_Lock.Unlock();
		}

		HRESULT UnInit()
		{
			std::list<TResource*>::iterator i = m_listResourcePool.begin();
			std::list<TResource*>::iterator iend = m_listResourcePool.end();
			while (i!=iend)
			{
				TResource* pRes = *i;

				std::set<TResource*>::iterator Find = m_setResourceUnDelete.find(pRes);
				if(Find != m_setResourceUnDelete.end())
				{
					m_setResourceUnDelete.erase(pRes);
					SAFE_DELETE(pRes);
				}
				else
				{
					ASSERT(0);
				}
				i++;
			}
			m_listResourcePool.clear();

			{
				std::set<TResource*>::iterator i = m_setResourceUnDelete.begin();
				std::set<TResource*>::iterator iend = m_setResourceUnDelete.end();
				while (i!=iend)
				{
					TResource* pRes = *i;

					KGLogPrintf(KGLOG_ERR, "��Դ%xй¶",pRes);
					i++;
				}
			}
			return S_OK;
		};

		HRESULT Init(DWORD dwMaxPool,DWORD dwInitPoolCount)
		{
			m_dwMaxPool = dwMaxPool;
			DWORD dwCount = min(dwMaxPool,dwInitPoolCount);
			for (DWORD i=0;i<dwCount;i++)
			{
				TResource* pResource = RequestResource();
				FreeResource(pResource);
			}
			return S_OK;
		}

		TKG3DResourcePool()
		{
			m_dwMaxPool = 400;
		}

		virtual ~TKG3DResourcePool()
		{
			UnInit();
		}

		HRESULT OnLostDevice()
		{
			std::set<TResource*>::iterator i = m_setResourceUnDelete.begin();
			std::set<TResource*>::iterator iend = m_setResourceUnDelete.end();
			while (i!=iend)
			{
				TResource* pRes = *i;
				if(pRes)
					pRes->OnLostDevice();
				i++;
			}
			return S_OK;
		}

		HRESULT OnResetDevice()
		{
			std::set<TResource*>::iterator i = m_setResourceUnDelete.begin();
			std::set<TResource*>::iterator iend = m_setResourceUnDelete.end();
			while (i!=iend)
			{
				TResource* pRes = *i;
				if(pRes)
					pRes->OnResetDevice();
				i++;
			}
			return S_OK;
		}

		HRESULT GetState(DWORD& dwInUse,DWORD& dwTotal)
		{
			dwTotal = (DWORD)m_setResourceUnDelete.size();
			dwInUse = dwTotal - (DWORD)(m_listResourcePool.size());
			return S_OK;
		}
	};

template <class TResource>
	class TKG3DResourceLimitSharePool
	{
	protected:
		UINT GetCurrentID()
		{
			return m_uCurrentID++;
		}

		HRESULT NewOneShareResource(TResource** ppShareRes)
		{
			TResource* pNewRes = new TResource();
			KGLOG_PROCESS_ERROR(pNewRes);

			pNewRes->Init();
			pNewRes->pMgr = this;

			(*ppShareRes) = pNewRes;
			return S_OK;
Exit0:
			SAFE_DELETE(pNewRes);

			(*ppShareRes) = NULL;
			return E_FAIL;
		}
	public:
		UINT  m_uUnFreshCount;
		UINT  m_uCurrentID;
		DWORD m_dwNumMaxShare;

		map<UINT,TResource*>m_mapShareResource;
	public:

		BOOL IsValid(PVOID pUser,TResource* pResource)
		{
			return pResource->pUser == pUser;
		}

		HRESULT Init(DWORD dwMaxResource,UINT uUnFreshCount)
		{
			UnInit();
			m_dwNumMaxShare = dwMaxResource;
			m_uUnFreshCount = uUnFreshCount;
			return S_OK;
		}

		HRESULT UnInit()
		{
			map<UINT,TResource*>::iterator i = m_mapShareResource.begin();
			map<UINT,TResource*>::iterator iend = m_mapShareResource.end();

			while(i!=iend)
			{
				TResource* pRes = i->second;
				SAFE_DELETE(pRes);
				i++;
			}
			m_mapShareResource.clear();
			return S_OK;
		}

		HRESULT GetOneShareResource(TResource** ppShareRes,PVOID pUser)
		{
			HRESULT hr = S_OK;
			DWORD dwNowTime = timeGetTime();

			if(m_mapShareResource.size()<m_dwNumMaxShare)
			{
				hr = NewOneShareResource(ppShareRes);
				KGLOG_COM_PROCESS_ERROR(hr);

				UINT uID = GetCurrentID();
				m_mapShareResource[uID] = *ppShareRes;

				(*ppShareRes)->uID = uID;
			}
			else
			{
				map<UINT,TResource*>::iterator i = m_mapShareResource.begin();

				TResource* pText = i->second;
				KGLOG_PROCESS_ERROR(pText);

				KG_PROCESS_ERROR(dwNowTime - pText->dwLastTime >= 100);


				(*ppShareRes) = pText;

				m_mapShareResource.erase(i);

				UINT uID = GetCurrentID();
				m_mapShareResource[uID] = *ppShareRes;
				(*ppShareRes)->uID = uID;
			}

			(*ppShareRes)->dwLastTime = dwNowTime;
			(*ppShareRes)->pUser = pUser;

			return S_OK;
Exit0:
			return E_FAIL;
		}

		TKG3DResourceLimitSharePool(void)
		{
			m_dwNumMaxShare = 200;
			m_uCurrentID = 0;
			m_uUnFreshCount = 100;
		};

		~TKG3DResourceLimitSharePool(void)
		{
			UnInit();
		}

		HRESULT OnLostDevice()
		{
			map<UINT,TResource*>::iterator i    = m_mapShareResource.begin();
			map<UINT,TResource*>::iterator iend = m_mapShareResource.end();

			while(i!=iend)
			{
				TResource* pText = i->second;
				if(pText)
				{
					pText->OnLostDevice();
				}
				i++;
			}
			return S_OK;
		}

		HRESULT OnResetDevice()
		{
			map<UINT,TResource*>::iterator i    = m_mapShareResource.begin();
			map<UINT,TResource*>::iterator iend = m_mapShareResource.end();

			while(i!=iend)
			{
				TResource* pText = i->second;
				if(pText)
				{
					pText->OnResetDevice();
				}
				i++;
			}
			return S_OK;
		}

		DWORD GetResourceUse()
		{
			DWORD dwUse = 0;
			map<UINT,TResource*>::iterator i    = m_mapShareResource.begin();
			map<UINT,TResource*>::iterator iend = m_mapShareResource.end();

			while(i!=iend)
			{
				TResource* pText = i->second;
				if(pText)
				{
					dwUse += pText->GetResourceUse();
				}
				i++;
			}
			return dwUse;
		}

		HRESULT OnResourceUse(TResource* pShareRes)
		{
			KGLOG_PROCESS_ERROR(pShareRes);

			if( m_uCurrentID - pShareRes->uID >= m_uUnFreshCount)
			{
				map<UINT,TResource*>::iterator i = m_mapShareResource.find(pShareRes->uID);

				m_mapShareResource.erase(i);

				UINT uID = GetCurrentID();
				m_mapShareResource[uID] = pShareRes;
				pShareRes->uID = uID;
			}

			pShareRes->dwLastTime = timeGetTime();

			return S_OK;
Exit0:
			return E_FAIL;
		}

		HRESULT FreeResource(PVOID pUser,TResource* pShareRes)
		{
			map<UINT,TResource*>::iterator i;
			KGLOG_PROCESS_ERROR(pShareRes);
			KG_PROCESS_ERROR(pShareRes->pUser == pUser);

			i = m_mapShareResource.find(pShareRes->uID);

			KGLOG_PROCESS_ERROR(i!=m_mapShareResource.end());

			pShareRes->OnFreeResource();

			return S_OK;
Exit0:
			return E_FAIL;
		}
	};

class RecordResourceManager : public IKG3DResourceManager
{
protected:	
	IKG3DResourceManager* m_pResourceManager;
public:
	RecordResourceManager()
	{		
		m_pResourceManager = NULL;
	}
	~RecordResourceManager()
	{		

	}
	void SetResourceManager(IKG3DResourceManager* pResourceManager)
	{
		m_pResourceManager = pResourceManager;
	}
	virtual HRESULT Init()
	{
		if (m_pResourceManager)
			return m_pResourceManager->Init();
		else
			return E_FAIL;
	}
	virtual HRESULT UnInit()
	{
		if (m_pResourceManager)
			return m_pResourceManager->UnInit();
		else
			return E_FAIL;
	}

	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, IKG3DResourceBase **ppRetResource)
	{
		if (m_pResourceManager)
			return m_pResourceManager->LoadResourceFromFile(cszFileName,uFileNameHash,uOption | MLO_MODEL_RECORD,ppRetResource);
		else
			return E_FAIL;
	}
	virtual HRESULT NewOneResourse(unsigned uType,                 IKG3DResourceBase **ppRetResource)
	{
		if (m_pResourceManager)
			return m_pResourceManager->NewOneResourse(uType,ppRetResource);
		else
			return E_FAIL;
	}
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT SetBalance(unsigned uSizeMemeryKB, unsigned uSizeVideoKB)
	{
		if (m_pResourceManager)
			return m_pResourceManager->SetBalance(uSizeMemeryKB,uSizeVideoKB);
		else
			return E_FAIL;
	}

};
#endif // !defined(AFX_KResourceManager_H__9C3F564E_0218_4D56_AF1D_1E90C01E8AD6__INCLUDED_)
