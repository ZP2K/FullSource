#pragma once

#include ".\kg3dsceneentity.h"
#include "..\..\..\Include\KG3DEngine\KG3DInterface.h"

using namespace std;

class KG3DScene;

class KG3DSceneEntityGroup : public IKG3DResourceBase
{
protected:
	DWORD   m_dwType;//����
	DWORD   m_dwID;

	D3DXQUATERNION m_Rotation;
protected:

public:
	KG3DSceneEntityList m_listEntity;
	list<D3DXQUATERNION>m_listRotationBegan;
	ULONG  m_ulRefCount;//��Ʒʹ�ü���
	AABBOX m_BBox;
	string m_scName;
	//////////////////////////////////////////////////////////////////////////
	//���ü���
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

	//////////////////////////////////////////////////////////////////////////
	//��ʼ��,�������
	virtual HRESULT Init();
	virtual HRESULT UnInit();
	//////////////////////////////////////////////////////////////////////////
	//����,����
	virtual HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption);
	virtual HRESULT SaveToFile(const char cszFileName[],   unsigned uOption);
	virtual HRESULT LoadFromFileEx(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, DWORD_PTR WParam, DWORD_PTR LParam)
	{
		return LoadFromFile(cszFileName, uFileNameHash, uOption);
	}
	virtual HRESULT RefreshOption(unsigned uOption);
	//////////////////////////////////////////////////////////////////////////
	//ID,type
	virtual DWORD GetID();
	virtual unsigned GetType();
	void SetID(DWORD dwID) { m_dwID = dwID;}
	void SetType(DWORD dwType){m_dwType = dwType;}
	//////////////////////////////////////////////////////////////////////////
	//�ڴ����
	virtual HRESULT PrepareRender(unsigned uOption);//ѹ������
	virtual HRESULT PostRender();//�ָ����ݣ��������������ʹ��
	virtual HRESULT GetDateInformation(int* pnMem,int* pnVedioMem,int* pnConMem,int* pnConVMem);//�õ�������Ϣ����������Ϊռ���ڴ��С���ִ��С��ѹ�����ڴ��С��ѹ�����ڴ��С
	virtual unsigned   GetLastUseTime();//��ѯ�ϴ�ʹ�õ�ʱ��
	virtual unsigned   GetUseFrequency();//��ѯʹ��Ƶ�ʣ�һ��ʱ�����ʹ�ô���

private:

	virtual HRESULT OnReferenceZero();


public:
	KG3DSceneEntityGroup(void);
public:
	virtual ~KG3DSceneEntityGroup(void);

	HRESULT AddEntity(KG3DSceneEntity& Entity);
	HRESULT RemoveEntity(KG3DSceneEntity& Entity);

	HRESULT Render(DWORD dwOption);
	HRESULT FrameMove();
	HRESULT ComputeBBox();
	HRESULT RenderSelectMark(DWORD dwColor = 0xFFFF0000);


	void SetTranslation(D3DXVECTOR3* pValue);
	void SetRotation(D3DXQUATERNION* pValue, D3DXVECTOR3* pValueXYZ);
	void SetRotationCenter(D3DXVECTOR3* pValue);
	void SetScaling(D3DXVECTOR3* pValue);
	void SetScalingRotation(D3DXQUATERNION* pValue);
	void SetScalingCenter(D3DXVECTOR3* pValue);

	void GetTranslation(D3DXVECTOR3* pValue);
	void GetRotation(D3DXQUATERNION* pValue, D3DXVECTOR3* pValueXYZ);
	void GetRotationCenter(D3DXVECTOR3* pValue);
	void GetScaling(D3DXVECTOR3* pValue);
	void GetScalingRotation(D3DXQUATERNION* pValue);
	void GetScalingCenter(D3DXVECTOR3* pValue);
};
