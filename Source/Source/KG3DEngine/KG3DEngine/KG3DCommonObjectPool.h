////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DCommonObjectPool.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-10 17:00:20
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DCOMMONOBJECTPOOL_H_
#define _INCLUDE_KG3DCOMMONOBJECTPOOL_H_
#include "KG3DCommonObject.h"
////////////////////////////////////////////////////////////////////////////////
//�����ֻ��Ϊ���Զ��ͷŷ������
class KG3DCommonObjectPool : public KG3DCommonObjectSimple
{
public:
	STDMETHOD_(ULONG, GetType)();
	STDMETHOD_(ULONG, GetContainedType)();

	STDMETHOD_(ULONG, GetCount)();
	STDMETHOD(Push)(KG3DCommonObject* p, KG3DCOBJ_HANDLE* pObjectRet);
	STDMETHOD_(KG3DCommonObject*, Get)(const KG3DCOBJ_HANDLE& Handle);//�����͵õ�������˳��
	STDMETHOD(Del)(const KG3DCOBJ_HANDLE& Handle);
	STDMETHOD(Clear)();
	
	STDMETHOD(Traverse)(KG3DCommonObjectVisitor& visitor);//����ֵ��visitor���ƣ�������visitor����E_FAILED���

	/*���ٱ����ڼ��ٶ����Get�����ھ���Get�Ķ���
	���ڲ�����һ�����ٱ�ָ�룬��FastGet�����������ڲ������ݡ���
	�����˼��ٱ�
	KG3DCOBJ_HANDLE AccelaratorTable[] = {KG3DCOBJ_HANDLE(1000, 0), KG3DCOBJ_HANDLE(1000, 1),};
	��ô����ӦHandle�����������֮��GetFast(0)�ܸ���ȡ�ö�ӦHandle(1000, 0)�����
	ע������˳���ڲ���ȵĲ���ʱ�ᱻ�޸ģ����Լ��ٱ��������ֻ��һ���ĳ�Ա
	*/
	STDMETHOD(InitAccelaratorTable)(KG3DCOBJ_HANDLE* pTable, size_t uTableCount);
	STDMETHOD_(KG3DCommonObject*, GetFast)(size_t iPos);

	KG3DCommonObjectPool();
	~KG3DCommonObjectPool();
	
private:
	typedef std::vector<KG3DCommonObject*> TypeVec;
	typedef std::map<DWORD, TypeVec> TypeCon;
	TypeCon	m_Con;
	ULONG	m_Count;

	typedef std::map<KG3DCOBJ_HANDLE, size_t> TypeAccelaratorPoses;
	struct AccelaratorData 
	{
		TypeAccelaratorPoses m_mapOfPos;
		std::vector<KG3DCommonObject*>	m_vecObj;
	};
	AccelaratorData* m_pAccelaratorData;
};

class KG3DCommonObjectPoolDummy : public KG3DCommonObjectPool
{
	STDMETHOD(Push)(KG3DCommonObject* p, KG3DCOBJ_HANDLE* pObjectRet){return E_FAIL;}
};
extern KG3DCommonObjectPool& g_GetKG3DCommonObjectPoolDummy();


#endif //_INCLUDE_KG3DCOMMONOBJECTPOOL_H_
