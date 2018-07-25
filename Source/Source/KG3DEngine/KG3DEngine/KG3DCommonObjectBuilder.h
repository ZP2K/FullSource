////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DCommonObjectBuilder.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-10 16:27:18
//  Comment     : ����IOCģʽ
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DCOMMONOBJECTBUILDER_H_
#define _INCLUDE_KG3DCOMMONOBJECTBUILDER_H_

////////////////////////////////////////////////////////////////////////////////
class KG3DCommonObject;
//��������
typedef KG3DCommonObject*	(*TypeFuncCommonObjectBuilder)(LPCTSTR strFileName, DWORD_PTR WParam, DWORD_PTR LParam);//Լ����һ���������NULL��Create��Init�����Լ���Build�������
//���溯��
typedef HRESULT	(*TypeFuncCommonObjectSaver)(KG3DCommonObject* p, LPCTSTR strFileName, DWORD_PTR WParam, DWORD_PTR LParam);

class KG3DCommonObjectBuilder
{
public:
	//������ע����ƣ����������������룬����ע����ƣ������ǳ�Ա�ķ�ʽӲ��ϣ��ο�CPP��RegisterContainerTypeǰ���ע��
	HRESULT	RegisterType(DWORD dwType, TypeFuncCommonObjectBuilder pFuncBuilder
		, TypeFuncCommonObjectSaver pSaver);	//pFuncBuilder�����У���Saver�ǿ�ѡ��
	HRESULT UnRegisterType(DWORD dwType, TypeFuncCommonObjectBuilder* pOldBuilder
		, TypeFuncCommonObjectSaver* pOldSaver);//���ؾɵ�

	ULONG	IsTypeRegister(DWORD dwType);

	KG3DCommonObject* Build(DWORD dwType, LPCTSTR strFileName, DWORD_PTR wParam, DWORD_PTR lParam);
	HRESULT	Save(KG3DCommonObject* p,LPCTSTR strFileName, DWORD_PTR wParamToSave, DWORD_PTR lParamToSave);
private:
	struct TypeConConElem 
	{
		TypeFuncCommonObjectBuilder	pFuncBuilder;
		TypeFuncCommonObjectSaver	pFuncSaver;
		TypeConConElem():pFuncBuilder(NULL), pFuncSaver(NULL){}
	};
	typedef std::map<DWORD, TypeConConElem>	TypeContainerContainer;
	TypeContainerContainer m_ConCon;
	HRESULT ClearContainerContainer();
};

#endif //_INCLUDE_KG3DCOMMONOBJECTBUILDER_H_
