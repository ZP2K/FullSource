////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGSTLWrappers.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-8-19 17:22:25
/*  Comment     : 
������װSTL��Щ�鷳���﷨������ʧһ�����麯�������ܣ�����
������������������Ϊ�㲻���麯���͸��Ƶģ����������㷨���⣬����û�й�ϵ��
����Wrapper����ͳһSTL�ķ��ʷ�ʽ��������Ϊ�ϸ��������Ͷ���Ҫ�Ķ�����

����Ҫ�ģ�ʹ��Proxy�������ǡ�����װ�����Բ��ܱ�¶�ײ�����

ע�⣺
1.�����Ϊ��Աʹ�ã�ProxyӦ��������������������������Ȼ��������ʱ����Ϊԭ�����Ѿ�����������������
����Iteractor��������һ�µ�

2.Proxy��ѭ�������ж��������ı����Ҫʮ��ע�⡣���ǲ�Ӧ����ѭ����
insert����delete��

������std�����������������࣬��ͨ���鶼��һ���ġ�һʱ���㣬����ļ���
�����ε������ˡ�

ʹ�÷�ʽ:
1.����д������һ������
void PrintCt(const KGCH::FrontAccessProxy& Proxy)
{
	for (Proxy.Begin(); ! Proxy.IsEnd(); Proxy.StepForward())
	{
		INT* p = (INT*)Proxy.Cur();
		std::cout << *p << " ";
	}
}

���÷�ʽ���£�
std::set<INT> TempSet;

PrintCt(KGCH::CreateFrontAccessProxy(TempSet));

��������map
std::map<INT> TempSet;
���÷�ʽ�ͺ�������û���κα仯

2.������ClassA�г�Աstd::set<INT> m_set;
��Ҫ��ClassA��ʹ���߷��������Ԫ�أ��ֲ��뱩¶����Ķ���
��ô

������

ClassA
{
	...
public:
	typedef KGCH::TFrontAccessProxy<INT>	TypeAccessProxy;	
	//Ҳ������typedef KGCH::FrontAccessProxy TypeAccessProxy��������GetCur�����ã���������Cur�õ�LPVOID��û��ģ�壬�����һЩ

	TypeAccessProxy& GetProxy(){return m_Proxy;}

private:
	typedef std::set<INT> TypeCt;

	TypeCt	m_set;

	KGCH::TFrontAccessProxySolid<TypeCt>	m_Proxy;

public:
	//�ڹ��캯�������Proxy������������������
	ClassA():m_Proxy(m_set){}
};

����������ô���������ⲿ���붼����Ҫ���κθĶ������Ҽ�ʹ�Ժ����˷�std����
����ô�Զ���Proxy��ʵ��KGCH::TFrontAccessProxy<INT>�Ľӿڣ��ⲿ���뻹��һ��Ҳ���ø�
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGSTLWRAPPER_H_
#define _INCLUDE_KGSTLWRAPPER_H_
#include <map>
////////////////////////////////////////////////////////////////////////////////
namespace KG_CUSTOM_HELPERS
{
	enum{
		em_frontAccessProxy_cap_forward = 1 << 0,
		em_frontAccessProxy_cap_bidirection = 1 << 1,
	};

	namespace Private
	{
		inline BOOL IsForwardIteratorTag(...){return FALSE;}
		inline BOOL IsForwardIteratorTag(std::forward_iterator_tag*){return TRUE;}
		inline BOOL IsBidirectionalIteratorTag(...){return FALSE;}
		inline BOOL IsBidirectionalIteratorTag(std::bidirectional_iterator_tag*){return TRUE;}

		template<typename iteratorType>
		ULONG GetIteratorCapMask()
		{
			iteratorType::iterator_category* p = NULL; 
			ULONG ret = 0;
			if(IsForwardIteratorTag(p))
				ret |= em_frontAccessProxy_cap_forward;
			if(IsBidirectionalIteratorTag(p))
				ret |= em_frontAccessProxy_cap_bidirection;
			return ret;
		}
	};

	struct FrontAccessProxy 
	{
		virtual VOID Begin() = 0;
		virtual BOOL IsEnd()const = 0;
		virtual VOID StepForward() = 0;
		virtual VOID StepBackward() = 0;	//��һ����˫���Proxy��һ����StepBackward()���뿴������־���ܺ����һ����ǰ������Ҫ��bidirection�����������Ŀ����ο�STL��׼���Ǳ���
		virtual VOID GoToEnd() = 0;	//Endָ���һ��Ԫ��֮���λ�ã���stl����������Ӧ��һ��
		virtual	LPVOID	Cur() = 0;

		virtual BOOL IsContentThisType(const type_info& info)const  = 0;
		virtual BOOL IsEmpty()const = 0;
		virtual ULONG	GetCapMask()const = 0;	//�õ�������������

		virtual ~FrontAccessProxy() = 0{}

		LPVOID GetBackElemPointer();//������ƶ�iterator��ʹ��֮ǰҪ��IsEmpty
	};

	template<class Type>
	struct TFrontAccessProxy : public FrontAccessProxy	//�������������ǿ���ͼ��ģ�Ҫ��֤�ܺ�FrontAccessProxyǿת�����ܼ��麯������Ȼ�õ�ǿ�Ƶĵط����ܳ���
	{
		Type& GetCur()		{return *static_cast<Type*>(this->Cur());}
		Type& GetFront()	{this->Begin();return GetCur();}	//������ƶ�iterator�ģ�С��
		Type& GetBack()		{return *static_cast<Type*>(this->GetBackElemPointer());}	//������ƶ�iterator��ʹ��֮ǰҪ��IsEmpty
	};

	template<class Type>
	struct TFrontAccessProxySolid;

	template<class Type>
	struct TFrontAccessProxySolid : public TFrontAccessProxy<typename Type::value_type>
	{
		typedef typename Type::reference reference;
		typedef typename Type::value_type value_type;
		typedef typename Type::iterator iterator;
	public:
		explicit TFrontAccessProxySolid(Type& Ct):m_Ct(Ct){m_it = Ct.end();}
		TFrontAccessProxySolid(const TFrontAccessProxySolid& Other):m_Ct(Other.m_Ct), m_it(Other.m_it){}

		virtual VOID Begin(){m_it = m_Ct.begin();}
		virtual BOOL IsEnd()const{return m_it == m_Ct.end();}
		virtual VOID StepForward(){++m_it;}
		virtual VOID StepBackward(){--m_it;}
		virtual	LPVOID	Cur(){
			// TODO: Fix this use c++ cast
			return (LPVOID)(&(*m_it));
		}
		virtual VOID GoToEnd(){m_it = m_Ct.end();}
		
		virtual BOOL IsContentThisType(const type_info& info)const{return typeid(value_type) == info;}
		virtual BOOL IsEmpty()const{return m_Ct.empty();}

		virtual ULONG	GetCapMask()const{return Private::GetIteratorCapMask<iterator>();}
	protected:
		mutable Type&	m_Ct;
		mutable iterator m_it; 
	};

	template<class TypeKey, class TypeValue>
	struct TFrontAccessProxySolid<std::map<TypeKey, TypeValue> > : public TFrontAccessProxy<TypeValue>
	{
		typedef std::map<TypeKey, TypeValue> Type;
		typedef typename Type::reference reference;
		typedef typename Type::iterator iterator;
		typedef TypeValue value_type;
	public:
		explicit TFrontAccessProxySolid(Type& Ct):m_Ct(Ct){m_it = Ct.begin();}
		TFrontAccessProxySolid(const TFrontAccessProxySolid& Other):m_Ct(Other.m_Ct), m_it(Other.m_it){}

		virtual VOID Begin(){m_it = m_Ct.begin();}
		virtual BOOL IsEnd()const{return m_it == m_Ct.end();}
		virtual VOID StepForward(){++m_it;}
		virtual VOID StepBackward(){--m_it;}
		virtual	LPVOID	Cur(){return &(m_it->second);}
		virtual VOID GoToEnd(){m_it = m_Ct.end();}

		virtual BOOL IsContentThisType(const type_info& info)const{return typeid(TypeValue) == info;}
		virtual BOOL IsEmpty()const{return m_Ct.empty();}

		virtual ULONG	GetCapMask()const{return Private::GetIteratorCapMask<iterator>();}
	protected:
		mutable Type&	m_Ct;
		mutable iterator m_it; 
	};

	template<class TypeElem>
	struct TFrontAccessProxyArray : public TFrontAccessProxy<TypeElem> 
	{
	public:
		template<size_t ArraySize>
		TFrontAccessProxyArray(TypeElem (&Ct)[ArraySize])
			:m_Ct(Ct)
			,m_uArraySize(ArraySize)
		{
			m_it = m_Ct;
		}
		TFrontAccessProxyArray(const TFrontAccessProxyArray& Other):m_Ct(Other.m_Ct), m_it(Other.m_it){}

		virtual VOID Begin(){m_it = m_Ct;}
		virtual BOOL IsEnd()const{return m_it == m_Ct + m_uArraySize;}
		virtual VOID StepForward(){++m_it;}
		virtual VOID StepBackward(){--m_it;}
		virtual	LPVOID	Cur(){return &(*m_it);}
		virtual VOID GoToEnd(){m_it = m_Ct + m_uArraySize;}

		virtual BOOL IsContentThisType(const type_info& info)const{return typeid(TypeElem) == info;}
		virtual BOOL IsEmpty()const{return false;}

		virtual ULONG	GetCapMask()const{return em_frontAccessProxy_cap_bidirection;}
	protected:
		mutable TypeElem* m_Ct;
		mutable TypeElem* m_it;
		const size_t m_uArraySize;
	};

	template<class Type>
	TFrontAccessProxySolid<Type> CreateFrontAccessProxy(Type& Ct){return TFrontAccessProxySolid<Type>(Ct);}
};
namespace KGCH = KG_CUSTOM_HELPERS;
#endif //_INCLUDE_KGSTLWRAPPER_H_
