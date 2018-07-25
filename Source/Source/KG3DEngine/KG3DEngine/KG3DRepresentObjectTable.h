#pragma once

#include "../../../Include/kg3dengine/kg3dinterface.h"
#include "KG3DTemplates.h"
#include "KG3DRepresentObject.h"

typedef KG3DRepresentObject* (*TypeFuncRepresentObjBuilder)(DWORD_PTR wParam, DWORD_PTR lParam);

class KG3DRepresentObjectTable :
	public TKG3DResourceManagerBase<KG3DRepresentObject>
{
private:
	typedef TKG3DResourceManagerBase<KG3DRepresentObject> TypeBaseClass;
	std::vector<TypeFuncRepresentObjBuilder>	m_vecBuilder;//����������Vec
public:
    // ��ʼ��,�������
    //virtual HRESULT Init();
    //virtual HRESULT UnInit();
    
    // ��Դ����
    //virtual HRESULT LoadResourceFromFile(
    //    IKG3DResourceBase** ppResource, LPSTR pFileName
    //);
    //virtual HRESULT Get1NewResourse(
    //    IKG3DResourceBase** ppResource, DWORD dwType
    //);
    //virtual HRESULT GetResourse(
    //    IKG3DResourceBase** ppResource, DWORD dwID
    //);

    virtual HRESULT Get1NewResourcePointer(KG3DRepresentObject** ppT,DWORD Type,DWORD_PTR);
	virtual HRESULT Init();

	HRESULT GetCopy(KG3DRepresentObject** ppOut,KG3DRepresentObject* pIn);
	HRESULT GetTypeInfoByFileName(TypeInfo** ppTypeInfo, const char cszFileName[]){return TypeBaseClass::GetTypeInfoByFileName(ppTypeInfo, cszFileName);}//��¶�������Ϊpublic
	
	//ע��ͷ�ע�ᴴ������
	HRESULT RegisterType(DWORD dwType, TypeFuncRepresentObjBuilder Builder);
	HRESULT UnRegisterType(DWORD dwType, TypeFuncRepresentObjBuilder* pTheUnregisteredBuilder);

public:
	KG3DRepresentObjectTable(void);
	virtual ~KG3DRepresentObjectTable(void);

	// Inherited via TKG3DResourceManagerBase
	virtual HRESULT LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, IKG3DResourceBase ** ppRetResource, bool bSyncLoad = true, ResNodeState ** res_node_state = NULL) override;
	virtual HRESULT LoadAnimationFromFile(const char cszFileName[], void ** ppRet) override;
	virtual HRESULT UnloadAnimation(void * pResource) override;
	virtual HRESULT LoadTagAnimationFromFile(const char cszFileName[], void ** ppRet) override;
	virtual HRESULT UnloadTagAnimation(void * pResource) override;
	virtual HRESULT LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, HANDLE * pRetHandle) override;
	virtual MODEL_LOAD_STATE GetResourceLoadState(HANDLE hResourceHandle) override;
	virtual HRESULT GetResourceByHandle(HANDLE hResourceHandle, IKG3DResourceBase ** ppRetResource) override;
	virtual void CloseResourceHandle(HANDLE hResourceHandle) override;
};

extern KG3DRepresentObjectTable& g_GetObjectTable();//�ĳ��������Ա�������ʼ��˳�������