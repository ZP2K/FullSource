#include "stdafx.h"
#include "KG3DTemplates.h"

HRESULT RecordResourceManager::LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, IKG3DResourceBase ** ppRetResource, bool bSyncLoad, ResNodeState ** res_node_state)
{
	return E_NOTIMPL;
}

HRESULT RecordResourceManager::LoadAnimationFromFile(const char cszFileName[], void ** ppRet)
{
	return E_NOTIMPL;
}

HRESULT RecordResourceManager::UnloadAnimation(void * pResource)
{
	return E_NOTIMPL;
}

HRESULT RecordResourceManager::LoadTagAnimationFromFile(const char cszFileName[], void ** ppRet)
{
	return E_NOTIMPL;
}

HRESULT RecordResourceManager::UnloadTagAnimation(void * pResource)
{
	return E_NOTIMPL;
}

HRESULT RecordResourceManager::LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, HANDLE * pRetHandle)
{
	return E_NOTIMPL;
}

MODEL_LOAD_STATE RecordResourceManager::GetResourceLoadState(HANDLE hResourceHandle)
{
	return MODEL_LOAD_STATE();
}

HRESULT RecordResourceManager::GetResourceByHandle(HANDLE hResourceHandle, IKG3DResourceBase ** ppRetResource)
{
	return E_NOTIMPL;
}

void RecordResourceManager::CloseResourceHandle(HANDLE hResourceHandle)
{
}
