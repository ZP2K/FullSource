////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DRepresentObjectDummy.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-7-18 10:05:02
//  Comment     : 
/*����Observerģʽ��������һ����������ڲ�����֮��Ľ��������ڻ�����չ�õ�
��Ҫ���κξ�����������������Ҫ���κκ;����߼������ı�������Ȼ������͵�
����һ��û�й�ϵ�Ķ�����

Dummy�ṩһ����̬Buffer�����ڸ��û���Щ��Dummy��ϵ��һ������ݣ��û��Լ�������ô�á�
Buffer�����ֻ��Щ�����ݣ�Object������������Buffer������֪��������Щɶ������Ҳ��
û�����������������

��(α����)��
INT ObjectOrder = 10;
KG3DRepresentObjectDummy* pObjectDummy = NULL;
HRESULT hr = g_GetObjectTable().Get1NewResouce(&pObjectDummy, REPRESENTOBJECT_DUMMY, 0);
if(SUCCEED(hr))
{
	pObjectDummy->RegisterReceiver(this);
	pObjectDummy->SetBuffer(&ObjectOrder, sizeof(ObjectOrder));
}
hr = m_SceneRef.AddRepresentObject(pObjectDummy, EM_SCENE_ADD_OPTION_SELECTABLE, NULL);


��ô��Object�ƶ���ѡ��Ƚ��е�ʱ��Messenger�ܹ��ᴫ����Ϣ
��ô��OnReceiverMessage�п���д������
switch(Message.m_uMessage)
{
case KM_XXX:
{
	INT ObjectOrder = 0;
	HRESULT hr = pObjectDummy->GetBuffer(&ObjectOrder, sizeof(ObjectOrder));
	if(SUCCEED(hr))
		this->DoSomething(ObjectOrder);

	break;
}

}

*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DREPRESENTOBJECTDUMMY_H_
#define _INCLUDE_KG3DREPRESENTOBJECTDUMMY_H_

#include "KG3DRepresentObject.h"
#include "KG3DMessenger.h"

////////////////////////////////////////////////////////////////////////////////
class KG3DRepresentObjectDummy : public KG3DRepresentObject, public KG3DMessageSender
{
public:
	static KG3DRepresentObject* Build(DWORD_PTR wParam, DWORD_PTR lParam){return new KG3DRepresentObjectDummy;}
	virtual HRESULT Copy(KG3DRepresentObject* pSrcObject);
	virtual HRESULT Render(DWORD dwOption);
	virtual BOOL RayIntersection(const D3DXVECTOR3& vSrc
		, const D3DXVECTOR3& vNormalizedDir
		, FLOAT* pfRet
		, BOOL bIntersectBoxOnly
		, KG3DIntersectionFilter* pFilter = NULL);


	//Dummy�ṩһ����̬�������ڴ洢�û����ݣ�1kByte֮����Щ������ò��Ǵ����������ģ�Dummy����֪����ô��������
	enum{max_buffer_size = 1024
		, shot_buffer_size = sizeof(DWORD_PTR)*2};

	VOID	ClearBuffer();
	HRESULT	SetBuffer(LPVOID pData, size_t DataSize);
	HRESULT GetBuffer(LPVOID pData, size_t DataSize);

	//���Ƿ��������ڷ���Ϣ
	virtual HRESULT SetTranslation(const D3DXVECTOR3* pTrans);
	virtual HRESULT SetRotation(const D3DXQUATERNION* pRotaion);
	virtual HRESULT SetScaling(const D3DXVECTOR3* pScale);
	virtual HRESULT OnSelect();

	virtual HRESULT ComputeBBox();
	virtual HRESULT	SetSize(FLOAT fSize);
	virtual BOOL	IsSpecial(){return TRUE;}

	KG3DRepresentObjectDummy();
	virtual ~KG3DRepresentObjectDummy();
#if defined(DEBUG) | defined(_DEBUG)
protected:
#else
private:
#endif

	char*	m_pBuffer;
	char	m_ShortBuffer[shot_buffer_size];	
	size_t	m_BufferSize;

	FLOAT	m_fSize;
};

#endif //_INCLUDE_KG3DREPRESENTOBJECTDUMMY_H_
