////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DCommonObject.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-3-6 14:21:04
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DCOMMONOBJECT_H_
#define _INCLUDE_KG3DCOMMONOBJECT_H_


#include "IEKG3DCommonObject.h"

class KG3DCommonContainer;

//������Ǿ�������ATL�ĸ�ʽд�ģ�����������������оͲ���Ҫ��ô�ϸ񣬲�Ȼ����д,THIS��THIS_�����Ǳ����
DECLARE_INTERFACE_(KG3DCommonObject, IEKG3DCommonObject)
{
	STDMETHOD_(ULONG, GetType)(THIS) PURE;
	STDMETHOD_(LPVOID, ToAnotherInterface)(THIS_ DWORD_PTR Param) PURE;

	//��Ⱦ���
	STDMETHOD(OnLostDevice)(THIS) PURE;	//��������Դ�Ķ���ͨ������Ҫ�豸�ָ�
	STDMETHOD(OnResetDevice)(THIS) PURE;

	STDMETHOD(FrameMove)(THIS) PURE;
	STDMETHOD(Render)(THIS) PURE;

	//����
	STDMETHOD_(KG3DCommonContainer*, GetContainerInterface)(THIS) PURE;	//���麯��ת����dynamic_cast��ܶ�

	KG_DECLARE_NO_DELETE_OP();//���������ʾ�����������������������delete������Ӧ����Release
};

//���ܶ�Ĭ��ʵ�ֵ�KG3DCommonObject����,����������ģ�ͣ�һ��Release���ͷţ�AddRef��Ч��
class KG3DCommonObjectSimple : public KG3DCommonObject
{
public:
	STDMETHOD_(ULONG, GetType)() PURE;
	STDMETHOD_(LPVOID, ToAnotherInterface)(DWORD_PTR Param){return NULL;}

	//Ĭ������£�Init�����Լ���Build��ʱ����ã��ⲿ������Init
	STDMETHOD(Init)(){return S_OK;}	//��������Դ�Ķ�����Ҫ�ӳٳ�ʼ����Init����Ҫ�Լ���Build�����е��õģ��ⲿ������Init��InitӦ���ܼ����ظ�InitȻ��assert
	STDMETHOD(UnInit)(){return S_OK;} //Ӧ���ܹ��ظ����ö�������

	STDMETHOD(OnLostDevice)(){return S_OK;}
	STDMETHOD(OnResetDevice)(){return S_OK;}

	STDMETHOD(FrameMove)(){return S_OK;}
	STDMETHOD(Render)(){return S_OK;}

	STDMETHOD_(KG3DCommonContainer*, GetContainerInterface)(){return NULL;}

	//�����ʹ�õ��Ǽ�Releaseģ�ͣ�AddRefû��Ч��Releaseֱ��ɾ��
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject){return E_FAIL;}
	virtual ULONG STDMETHODCALLTYPE AddRef( void){return 1;}
	virtual ULONG STDMETHODCALLTYPE Release( void);
	
	virtual ~KG3DCommonObjectSimple() = 0 {}

	KG_DECLARE_NO_DELETE_OP();//���������ʾ�����������������������delete������Ӧ����Release
};
//���ܶ�Ĭ��ʵ���Ҵ����ü�����KG3DCommonObject����
class KG3DCommonObjectSimpleRef : public KG3DCommonObjectSimple
{
public:
	STDMETHOD_(ULONG, GetType)() PURE;

	//���ü��������Ѿ��ڲ�ʵ�֣��������Բ��ܣ�����������ڲ��������ü�������ʽ
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject){return E_FAIL;}
	virtual ULONG STDMETHODCALLTYPE AddRef( void);
	virtual ULONG STDMETHODCALLTYPE Release( void);	//�Լ���֤Unit�ĵ��ã�Ĭ��Ӧ����Release�����UnInit

	KG_DECLARE_NO_DELETE_OP();//���������ʾ�����������������������delete������Ӧ����Release
protected:
	KG3DCommonObjectSimpleRef():m_ulRefCount(1){}
	ULONG	m_ulRefCount;
};

struct KG3DCommonObjectVisitor 
{
	virtual HRESULT Accept(KG3DCommonObject& Obj) = 0;
	virtual ~KG3DCommonObjectVisitor() = 0{}
};

class KG3DCommonObjectDummy : public KG3DCommonObjectSimpleRef
{
	STDMETHOD_(ULONG, GetType)(THIS);
};



#endif //_INCLUDE_KG3DCOMMONOBJECT_H_
