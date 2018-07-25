#pragma once
#include "KG3DDataFlowDataObject.h"

//template <class TResource>
//class TKG3DDataFlowResourceTable
//{
//	friend  TResource;
//public:	
//	std::map<UINT,TResource*>m_mapResource;
//	UINT m_nCurrentHandle;
//
//	TKG3DResourceManagerBase()
//	{
//		m_nCurrentHandle = 1;
//	}
//
//	virtual ~TKG3DResourceManagerBase()
//	{
//		UnInit();
//	}
//
//	virtual HRESULT Init()
//	{
//		UnInit();
//		return S_OK;
//	}
//
//	virtual HRESULT UnInit()
//	{
//		std::map<UINT,TResource*>::iterator i = m_mapResource.begin();
//		std::map<UINT,TResource*>::iterator iend = m_mapResource.end();
//		while (i!=iend)
//		{
//			TResource* pResource = i->second;
//			SAFE_DELETE(pResource);
//			i++;
//		}
//		m_mapResource.clear();
//		return S_OK;
//	}
//
//	virtual HRESULT NewOneResource(UINT* puHandle,DWORD dwType)
//	{
//		;
//	}
//
//}
class KG3DDataFlowDataDefine;

class KG3DDataFlowDataObjectTable//���ݶ���ķ������ͻ�����
{
public:
	struct DataObjectSet
	{
		list<KG3DDataFlowDataObject*>m_listDataObjects;//���ݶ���
		list<KG3DDataFlowDataObject*>m_listDataObjectsPool;//���ݶ��󻺳��
		DWORD m_dwMaxCountInPool;//�������󻺳�������

		HRESULT NewOneData(KG3DDataFlowDataObject** ppData,KG3DDataFlowDataDefine* pDefine,KG3DDataFlowDataObjectTable* pTable);
		HRESULT ReleaseOneData(KG3DDataFlowDataObject* pData,KG3DDataFlowDataDefine* pDefine);

		HRESULT Clear();
		DataObjectSet();
		~DataObjectSet();
	};

public:

	UINT m_uCurrentHandle;
	HRESULT GetCurrentHandle()
	{
		return m_uCurrentHandle++;
	}
	HRESULT Clear();

	HRESULT NewOneData(KG3DDataFlowDataObject** ppData,KG3DDataFlowDataDefine* pDefine);
	HRESULT ReleaseOneData(KG3DDataFlowDataObject* pData,KG3DDataFlowDataDefine* pDefine);

	map<DWORD,DataObjectSet*>m_mapDataObjectSet;

	KG3DDataFlowDataObjectTable(void);
	virtual ~KG3DDataFlowDataObjectTable(void);
};
