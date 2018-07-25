////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGHelperClasses.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-3-20 19:18:47
/*  Comment     : �����࣬������Բ������κ���Ҫ���������κ��ļ��Ķ���
//			�������Ҳ�ṩ��SceneEditorʹ��

1.TTypeCopy
�����Զ��Ļ���ת�������Զ�̫С
��Class A; Class B : public A;

A a;
B b;
TTypeCopy<A>(&b, &a);

2.TTypeCmp
ͬ�ϣ����ڻ���Binary�Ƚ�



3.ForAll
class AClass
{
public:
	HRESULT Render();
}
std::map<xx,xx> AMap;
KG_CUSTOM_HELPERS::ForAll(AMap, std::mem_func_ref(&AClass::Render));

����ForAllҪע����ǣ�΢���std::map��for_each���ݲ��Ǻܺã�����Ҫ�������ForAll��map���ػ��汾

4.TContainerDelete
TContainerDelete(AMap);	//������ָ����У����Զ�����delete

5.TContainerRelease ͬ�ϣ���������Release������涫��
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGHELPERCLASSES_H_
#define _INCLUDE_KGHELPERCLASSES_H_
#include <map>
#include <vector>
#include <set>
#include <algorithm>
//////////////////////////////////////////////////////////////////////////

namespace KG_CUSTOM_HELPERS
{
	/************************************************************************/
	/*����ģ�岻�ܾͷ�class����ƫ�ػ���ֻ�����أ��������ʵ�� ��class���͵�
	class�İ�����
	*/
	/************************************************************************/
	template<int num>
	struct TNumToType 
	{
		enum{result = num,};
	};
	/************************************************************************/
	/*����ָ��һ������ 
	*/
	/************************************************************************/
	struct NullType 
	{
	};
	/************************************************************************/
	/* ����ѡ��һ����
	*/
	/************************************************************************/
	template<bool _Bool, typename T1, typename T2> struct TTypeSelector{typedef T1 Result;};
	template<typename T1, typename T2> struct TTypeSelector<false, T1, T2>{typedef T2 Result;};
	//////////////////////////////////////////////////////////////////////////
	///��Ҫ��ʽָ��ģ�����
	template<class _DescStructBase>
	inline bool TTypeCopy(_DescStructBase* pDesc, const _DescStructBase* pSrc)
	{	
		_ASSERTE(pDesc && pSrc);
		return 0 == memcpy_s(pDesc, sizeof(_DescStructBase), pSrc, sizeof(_DescStructBase));
	}
	template<class _DescStructBase>
	inline bool TTypeCmp(_DescStructBase* pDesc, const _DescStructBase* pSrc)
	{
		return 0 == memcmp(pDesc, pSrc, sizeof(_DescStructBase));
	}
	//for_each��ǿ��

	template<class _Container,class _Fn1> 
	inline
	_Fn1 ForAll(_Container& Container, _Fn1 _Func)
	{
		return std::for_each(Container.begin(), Container.end(), _Func);
	}

	template<class _ContainedType,size_t _Size, class _Fn1> 
	inline
		_Fn1 ForAll(_ContainedType (&Container)[_Size], _Fn1 _Func)
	{
		for (size_t i = 0; i < _Size; ++i)
		{
			_Func(Container[i]);
		}
		return _Func;
	}

	//����std::map��for_each��ǿ�棬������KG_CUSTOM_HELPERS::ForAll(map, std::mem_func(&ClassA::DoSomething));
	template<class _Key, class _Type, class _Fn1>
	inline _Fn1 ForAll(std::map<_Key, _Type>& Container, _Fn1 _Func)
	{
		typedef std::map<_Key, _Type> TypeMap;
		struct PairSeperator 
		{
			_Fn1& m_Func;
			PairSeperator(_Fn1& Func):m_Func(Func){}
			void operator()(TypeMap::value_type& Value)
			{
				m_Func(Value.second);
			}
			operator _Fn1(){return m_Func;}
		};

		return std::for_each(Container.begin(), Container.end(), PairSeperator(_Func));
	};

	namespace Private
	{
		template<class _Fn1, class _Cmp> 
		class DoIf
		{
			_Fn1 m_Func;
			_Cmp m_CmpFunc;
		public:
			DoIf(_Fn1 _Func, _Cmp _CmpFunc):m_Func(_Func),m_CmpFunc(_CmpFunc){}
			template<class _Type>
			void operator()(_Type& A)
			{
				if(m_CmpFunc(A))
					m_Func(A);
			}
			template<class _Type>
			void operator()(const _Type& A)
			{
				if(m_CmpFunc(A))
					m_Func(A);
			}		
			operator _Fn1(){return m_Func;}
		};

	};

	struct IsNotNull
	{
		bool operator()(LPVOID APointer){return APointer != NULL;}
	};

	template<class _Container, 	class _Fn1, class _Cmp> 
	inline _Fn1 ForAllIf(_Container& Container, _Fn1 _Func, _Cmp _CmpFunc)
	{
		return ForAll(Container, Private::DoIf<_Fn1, _Cmp>(_Func, _CmpFunc));
	}
	
	template<class _Container, 	class _Fn1> 
	inline _Fn1 ForAllIfNotNull(_Container& Container, _Fn1 _Func)
	{
		return ForAllIf(Container, _Func, IsNotNull());
	}
	/************************************************************************/
	/*�����������ձ�׼����
	*/
	/************************************************************************/
	struct SafeDelete 
	{
		template<class _Type>
		void operator()(_Type*& pType){SAFE_DELETE(pType);}
	};

	struct SafeRelease 
	{
		template<class _Type>
		void operator()(_Type*& pType){SAFE_RELEASE(pType);}
	};	

	template<class _Type>
	BOOL TContainerDelete(_Type& Container)
	{
		ForAll(Container, SafeDelete());		
		Container.clear();
		return TRUE;
	}

	template<class _Type, size_t _Size>
	BOOL TContainerDelete(_Type (&Container)[_Size])
	{
		ForAll(Container, SafeDelete());
		return TRUE;
	}

	template<class _Type>
	BOOL TContainerRelease(_Type& Container)
	{
		ForAll(Container, SafeRelease());		
		Container.clear();
		return TRUE;
	}
	
	template<class _Type, size_t _Size>
	BOOL TContainerRelease(_Type (&Container)[_Size])
	{
		ForAll(Container, SafeRelease());
		return TRUE;
	}

	/*
	IteratorHolder��ԭ��ܼ򵥡���������ʵ�ַ����ԭ�����ǲ��ܱ�¶������Ϊ�ӿڣ�Ҳ�Ͳ��ܱ�¶iterator��Ϊ�ӿڡ�
	COM�Ľ��������IEnum������IEnum��ҪƵ���������ͷţ����ҪƵ�����ʵĻ����Զ�ջ���Ǻ��¡�
	��������ڲ�ʵ��IEnum��Reset��Next����ô��Ҫ����һ��iterator�������Ӻܺã������������ڹ���Ҳ�ܼ����ˡ�
	�����������Ƕ�ױ���һ�������
	������һ���ܼ򵥵Ľ������������һ��Buffer��ΪHandle��ʵ��GetFirst��GetNext�ͽӿڡ���ô���Handle�������κζ�����
	�����Buffer����ֱ�ӷ�iterator�Ļ�����ôʵ�־͸������ˡ�ͬʱ��Ҳ��Υ��OpenCloseԭ�����Handle������Ҫ����
	���κζ����ģ�ֻҪ���GetFirst��GetNext�ܹ���������
	*/
	class IteratorHolder 
	{
		enum
		{
			em_map_iterator_size = sizeof(std::map<int, int>::iterator),
			em_vector_iterator_size = sizeof(std::vector<int>::iterator),
			em_list_iterator_size = sizeof(std::list<int>::iterator),
			em_buffer_size_temp = em_vector_iterator_size > em_map_iterator_size ? em_vector_iterator_size : em_map_iterator_size,
			em_buffer_size = em_list_iterator_size > em_buffer_size_temp ? em_list_iterator_size : em_buffer_size_temp,
		};
	private:
		BYTE	m_Buffer[em_buffer_size];
	};

	template<class Type>
	inline IteratorHolder CreateIteratorHolder(Type& container)
	{
		typedef typename Type::iterator iterator;
		IteratorHolder holder;
		_ASSERTE(! container.empty());
		iterator it = container.begin();
		memcpy_s(&holder, sizeof(IteratorHolder), &it, sizeof(iterator));
		return holder;
	}

};
namespace KGCH = KG_CUSTOM_HELPERS;
#endif //_INCLUDE_KGHELPERCLASSES_H_
