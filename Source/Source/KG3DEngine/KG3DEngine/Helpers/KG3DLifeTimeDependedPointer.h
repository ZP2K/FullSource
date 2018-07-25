////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : 
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-16 20:11:59
//  Comment     : ������ͷ�ļ���Include����İ�����

/*
�������Ҫ������������⣺
A���B��֮��û���κοռ��ϵ�ֱ�ӹ�ϵ����B����Ҫ������A���Ի��ĳЩ��Ϣ��
�����B��ÿ��Ҫ���A��Ļ�����Ҫ����Ĳ���ϰ���ܳ���·����ȡA��Ļ���
������������ɴ�����Ѷ��������Ժ�ǿ������Ҳ�����һЩ�������ġ�

�򵥵ģ����A�����������һ����B�೤����ôB��ֱ��ӵ��A���ָ��ͺ���
��A����ӵ��B������

���������֮�䲻�ܱ�֤�������ڵĻ�����ʹ�������ü�������Ҳ�ǲ��Եģ�
��Ϊ������Ϊ����֮�����߼���ϵ�ͷ���һ�����ͷš�

������MessageSender��һ��Ҳ�ǿ��Եģ��������ﲢ����Ҫ�໥֪ͨ�¼�������
�����׻��ǲ��ܷ��㡣

���TKG3DLifeTimeDependedPointer��������ǰ�һ����ָ�븳�����Pointer
��һ���ͷ�֮�����Pointer���Զ�ʧЧ��
ʵ�����ڲ�ʵ�ֵķ���ֻ�Ǽ򵥵��໥֪ͨ���ѡ�����Ϊ��������ָͨ�������������
��ʹ����һЩ������ַ�����Щ�ַ����Բο���c++�����˼ά������ָ��Ĳ��֣�����ʰ�����۶��ѡ�

�÷����£�
Ҫʹ������ָ�룬�������KG3DLifeTimeDependedClassBase������������ʱ����ģ�����ָ���ж��ٸ�����
��class B : public KG3DLifeTimeDependedClassBase<1>{};	//��һ������

1.����һ�����У�����һ��TKG3DLifeTimeDependedPointer<B>	m_pB;
����һ����ʵ����������Ϊָ��ʹ�á�

2.��һ��ʹ��֮ǰ����ָ��һ�� m_pB = &aB;//aB��һ��class B��ʵ����
Ҳ�������Ա��ʼ�������ڳ�Ա��ʼ���б���ʹ�á�
ֻ�ܸ�ֵһ�Σ��ڶ��θ�ֵ�ͻᴥ��ASSERT

��ֵ֮��m_pB��һ��Ϊ��Чֵ����Ϊ��������Ļ������ܻᳬ��ע������Ĵ�С���Ӷ��޷�ע��

3. if(m_pB)
	m_pB->DoSomething();	//������ָ�������

�õ�ʱ�򣬿������κ���ʽ��if���������Ч�ԣ�if(m_pB);if(!m_pB),if(NULL == m_pB)��֮��֮���
����������Ч�ԣ���ȻB�ͷ��ˣ����õľ�����Чָ�룺


*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DTEMPLATECLASSFUNDATIONS_H_
#define _INCLUDE_KG3DTEMPLATECLASSFUNDATIONS_H_
//#include "./TemplateLibs/LOKI_APP/KG3DLokiBase.h"	//��Ҫ�õ����е�SUPERSUBCLASS��
#include <vector>
////////////////////////////////////////////////////////////////////////////////
namespace KG3D_CUSTOM_HELPERS
{
	/************************************************************************/
	/*��Ϊ��Щ���ǰ����࣬Ϊ���������ĳ�Ա�ͷ����������ֳ�ͻ���������涼����_��׺ 
	*/
	/************************************************************************/
	struct KG3DLifeTimeDependedPointerBase
	{
		virtual bool OnDependedClassDestroy_() = 0;
		virtual bool CanBeRegister_() const = 0;
		virtual bool CanBeUnRegister_() const = 0;
		virtual ~KG3DLifeTimeDependedPointerBase() = 0{};
	};
	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/*�������á��ɱ�����ָ����������������������κη���������Ҫ���ͻ����� 
	*/
	/************************************************************************/
	///���ڱ���Ƿ�ͬһ��
	namespace Private
	{
		struct KG3DLifeTimeDependedClassBaseBase
		{

		};
	};
	template<size_t _nDependOnCount>
	class KG3DLifeTimeDependedClassBase : public Private::KG3DLifeTimeDependedClassBaseBase
	{
	private:
		enum{max_depended_ptr_count = _nDependOnCount,};
		//KG3DLifeTimeDependedPointerBase* m_DependArray[PTR_COUNT];
		typedef std::vector<KG3DLifeTimeDependedPointerBase*> TypePtrArray;
		TypePtrArray m_DependArray;
		//INT m_nRegisteredCount;
	private:
		KG3DLifeTimeDependedClassBase(const KG3DLifeTimeDependedClassBase&);
		void operator=(const KG3DLifeTimeDependedClassBase&);
	public:
		KG3DLifeTimeDependedClassBase(){}
		~KG3DLifeTimeDependedClassBase()
		{
			_ASSERT(m_DependArray.size() <= max_depended_ptr_count);
			for (size_t i = 0; i < m_DependArray.size(); i++)
			{
				_ASSERTE(m_DependArray[i]);
#if defined(DEBUG) | defined(_DEBUG)
				bool bRet = 
#endif
					m_DependArray[i]->OnDependedClassDestroy_();
				_ASSERTE(bRet);
			}
		}
		bool RegisterDependence_(const KG3DLifeTimeDependedPointerBase& ADependPointer)
		{
			//INT nRegisteredCount = 0;
			//��֤�ⲿ�ĵ�������ʧ�ܣ������Ͳ�������Ϊprivate����Ա��
			if (!(ADependPointer.CanBeRegister_()))
			{
				_KG3D_DEBUG_REPORT(_STRINGER(KG3DLifeTimeDependedClassBase)_T(":")_STRINGER(RegisterDependence_)_T("����\n"));
				return false;
			}
			_ASSERTE(m_DependArray.size()<= max_depended_ptr_count);
			if(m_DependArray.size() == max_depended_ptr_count)
				return false;
			//�����ظ�
			for (size_t i = 0; i < m_DependArray.size(); i++)
			{
				_ASSERTE(m_DependArray[i]);
				
				if (m_DependArray[i] == &ADependPointer)
				{
					_KG3D_DEBUG_REPORT(_STRINGER(KG3DLifeTimeDependedClassBase)_T(":")_STRINGER(RegisterDependence_)_T("����\n"));
					return false;
				}
			}
			//_ASSERTE(nRegisteredCount < _nDependOnCount);//���ע��
			m_DependArray.push_back(const_cast<KG3DLifeTimeDependedPointerBase*>(&ADependPointer));
			return true;
		}
		bool UnRegisterDependence_(const KG3DLifeTimeDependedPointerBase& ADependPointer)
		{
			if (ADependPointer.CanBeUnRegister_())
			{
				TypePtrArray::iterator it = std::find(m_DependArray.begin(), m_DependArray.end(), &ADependPointer);
				if (it != m_DependArray.end())
				{
					m_DependArray.erase(it);
					return true;
				}
			}
			_KG3D_DEBUG_REPORT(_STRINGER(KG3DLifeTimeDependedClassBase)_T(":")_STRINGER(UnRegisterDependence_)_T("����\n"));
			return false;
		}
		
	};///KG3DLifeTimeDependedClassBase
	/************************************************************************/
	/*������ָ�����͵İ����� 
	*/
	/************************************************************************/
	namespace Private
	{
		//////////////////////////////////////////////////////////////////////////
		template<typename _Type>
		struct IsConstType{ enum{Result = false,};};
		template<typename _Type>
		struct IsConstType<const _Type>{ enum{Result = true,};};		
	};
	/************************************************************************/
	/*����ָ�롣��KG3DLifeTimeDependedClassBaseһ���á���KG3DLifeTimeDependedClassBase
	������ʱ���Լ�ҲʧЧ��ÿ��ʹ��֮ǰ�ǵñȽϡ�
	*/
	/************************************************************************/
	template<class _Type>
	class TKG3DLifeTimeDependedPointer;

	template<class _Type>
	class TKG3DLifeTimeDependedPointer<_Type*> : public KG3DLifeTimeDependedPointerBase
	{
	private:
		typedef _Type* _PtrType;
		typedef const _Type* _const_Ptr_Type;///���const _PtrType����ȫ������
		//////////////////////////////////////////////////////////////////////////
	private:
		_PtrType m_pObj;
	public:
		TKG3DLifeTimeDependedPointer()
			:m_pObj(NULL)
		{	
			_KG3D_DEBUG_STATIC_CHECK_T(_KG3D_SUPERSUBCLASS(Private::KG3DLifeTimeDependedClassBaseBase, _Type), InValidObj);
			_KG3D_DEBUG_STATIC_CHECK_T(Private::IsConstType<_Type>::Result == false, InValidPtrType);
		}
		explicit TKG3DLifeTimeDependedPointer(const _PtrType& pObj)
			:m_pObj(NULL)
		{	
			_ASSERTE(NULL != pObj);
			if (!m_pObj)
			{
				bool bRet = pObj->RegisterDependence_(*this);
				if (bRet)
				{
					m_pObj = pObj;
					return;
				}
			}
			return;	
		}
		bool operator=(const _PtrType& pObj)///���ʹ���˷�_PtrType������=�ͻ���Ϊ���汻��ֹ�Ŀ������캯�������������
		{
			_ASSERTE(NULL != pObj);
			if (!m_pObj)
			{
				bool bRet = pObj->RegisterDependence_(*this);
				if (bRet)
				{
					m_pObj = pObj;
					return true;
				}
			}
			return false;			
		}
		
		~TKG3DLifeTimeDependedPointer()
		{
			if (!m_pObj)
			{
				return;				
			}
#if defined(DEBUG) | defined(_DEBUG)
			bool bRet = m_pObj->UnRegisterDependence_(*this);
			_ASSERTE(bRet);
#else
			m_pObj->UnRegisterDependence_(*this);
#endif
			m_pObj = NULL;
		}
	private:///��ֹ����
		explicit TKG3DLifeTimeDependedPointer(const TKG3DLifeTimeDependedPointer&);
		void operator=(const TKG3DLifeTimeDependedPointer&);
	public:
		virtual bool CanBeRegister_() const 
		{
			return m_pObj == NULL;
		}
		virtual bool CanBeUnRegister_() const 
		{
			return m_pObj != NULL;
		}
		virtual bool OnDependedClassDestroy_()
		{
			m_pObj = NULL;
			return true;
		}
		inline _PtrType operator->()
		{
			_ASSERTE(NULL != m_pObj);
			return m_pObj;
		}
		///�ȽϺ�������

		inline bool operator!()const
		{
			return m_pObj == NULL;
		}
		inline friend bool operator==(const TKG3DLifeTimeDependedPointer& pTP,
			const _PtrType pObj)
		{ return pTP.m_pObj == pObj; }

		inline friend bool operator==(const _PtrType pObj,
			const TKG3DLifeTimeDependedPointer& pTP)
		{ return pObj == pTP.m_pObj; }

		inline friend bool operator!=(const TKG3DLifeTimeDependedPointer& pTP,
			const _PtrType pObj)
		{ return !(pTP.m_pObj == pObj); }

		inline friend bool operator!=(const _PtrType pObj,
			const TKG3DLifeTimeDependedPointer& pTP)
		{ return !(pObj == pTP.m_pObj); }
		
		inline friend _PtrType GetPtr(TKG3DLifeTimeDependedPointer& pTP){return pTP.m_pObj;}
	private:
		// ����'if (sp)'��ʽ�ıȽϡ����������Զ�ת��֮�⣬�������ǲ���ȫ�ģ�
		// ָ��һ�������Զ�ת�;�û����������ָ��ı�����
		 struct DummyClass
		 {
			 DummyClass() {}
			 private:
				 void operator delete(void*);//����Ϊ�˷�ֹд��delete p��
		 };

	public:
		operator DummyClass*() const
		{
			if (!*this) return 0;
			static DummyClass t;
			return &t;
		}


		
	};///TKG3DLifeTimeDependedPointer
};///KG3D_CUSTOM_HELPERS



#endif //_INCLUDE_KG3DTEMPLATECLASSFUNDATIONS_H_
