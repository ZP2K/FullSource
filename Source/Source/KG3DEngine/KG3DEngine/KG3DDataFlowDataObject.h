#pragma once

class KG3DDataFlowDataDefine;

class KG3DDataFlowDataObject
{
public:

	UINT m_uHandle;
	BYTE* m_lpData;
	DWORD m_dwSize;

	HRESULT Init(UINT uHandle,DWORD dwSize);
	HRESULT UnInit();

	KG3DDataFlowDataObject();
	virtual ~KG3DDataFlowDataObject();

	HRESULT SetInt(int Value,DWORD dwIndex,KG3DDataFlowDataDefine* pDefine);
	HRESULT GetInt(int* pValue,DWORD dwIndex,KG3DDataFlowDataDefine* pDefine);
protected:

	BYTE* pGetVariablePointer(DWORD dwIndex,KG3DDataFlowDataDefine* pDefine);
};

//typedef BYTE KG3DDataFlowDataObject ;


class KG3DDataFlowDataObjectContainerBase//������������
{

};

class KG3DDataFlowDataObjectContainerSingle :
	public KG3DDataFlowDataObjectContainerBase//�������ݶ�����������������غ�ͳ�����ݼ���.�����������е�����������ӣ��Զ���������ɾ��
{

};

class KG3DDataFlowDataObjectContainerList :
	public KG3DDataFlowDataObjectContainerBase//�������������ʺϿɱ����������
{

};

class KG3DDataFlowDataObjectContainerArray :
	public KG3DDataFlowDataObjectContainerBase//�������������ʺϹ̶�����������
{

};