////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGCommonBuffer.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-25 9:35:17
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGCOMMONBUFFER_H_
#define _INCLUDE_KGCOMMONBUFFER_H_

#include "KGCommonWidgets.h"
#include "KGHelperClasses.h"
////////////////////////////////////////////////////////////////////////////////
namespace KG_CUSTOM_HELPERS
{
	namespace Private
	{
		//���ڴ������࣬���Ը��ǡ��пյĻ���Ӧ�øĳɷ���std�����淶����ʽ
		template<typename _Type>
		struct TTypePlacementNewer 
		{
			template<size_t uSize>
			inline _Type* MakeType(char (&uBuffer)[uSize])
			{
				_KG3D_DEBUG_STATIC_CHECK_T(uSize >= sizeof(_Type), SizeTooSmall);
				return new(uBuffer) _Type();
			}
			template<size_t uSize>
			inline _Type* MakeType(char (&uBuffer)[uSize], const _Type& Other)
			{
				_KG3D_DEBUG_STATIC_CHECK_T(uSize >= sizeof(_Type), SizeTooSmall);
				return new(uBuffer) _Type(Other);
			}
			template<size_t uSize>
			inline void CopyType(char (&uBuffer)[uSize], const _Type& Other)
			{
				_KG3D_DEBUG_STATIC_CHECK_T(uSize >= sizeof(_Type), SizeTooSmall);
				_Type* pType = (_Type*)uBuffer;
				*pType = Other;
			}
			template<size_t uSize, typename _InitParamType>
			inline _Type* MakeType(char (&uBuffer)[uSize], const _InitParamType& Other)
			{
				_KG3D_DEBUG_STATIC_CHECK_T(uSize >= sizeof(_Type), SizeTooSmall);
				return new(uBuffer) _Type(Other);
			};

			/*//Placement Delete?����Ҫ��Ҫ��
			template<size_t uSize>
			inline void DelType(char (&uBuffer)[uSize])
			{
			_KG3D_DEBUG_STATIC_CHECK_T(uSize >= sizeof(_Type), SizeTooSmall);
			_Type* pType = (Type*)uBuffer;
			pType->~_Type();
			};*/

		};

		template<unsigned _Size> struct TSize;
	};
	/*
	C++�ܴ��һ�����������г�Ա��������ͷ�ļ����棬��˽�г�Ա���ڣ�Ŀ��ֻ���ܼ������С���ѡ�
	�������ϡ�����һ��ͷ�ļ��аѳ�Ա����Ϊ��Ķ����ͱ���ѱ�Ķ�����ͷ�ļ��������û��ļ���
	ͷ�ļ����棬�����������Ի�һֱ���ݳ�ȥ����ָ��+���캯��New����һ�������⣬����ÿ��һ��
	��Ա��Ҫ�ֱ���ͷ�ļ����������������캯�����������캯�������ƺ����ֱ����һ�Σ���ֱ......
	һ��С�ľ�......���ڳ�����ģ�壬Ԥ����һ����С�����������⣬��Ϊ��PlacementNew��������
	���Բ����ڴ�й©�ġ����ģ��ֻ�ʺ����ڼ��࣬��ΪĬ�Ͼ͹�����һ�Ρ��Ժ���ܻ����һЩ������
	д����һ�����ƵĶ�����

	ʹ�÷�ʽ��
	1.��ͷ�ļ�������
	class ClassA;
	KG_CUSTOM_HELPER::TCommonParamBuffer<ClassA, XX���ֽ�Ϊ��λ�����С��>	m_pXXX;
	ͷ�ļ��в���Ҫ������ClassA��ͷ�ļ�����������ĺô�����Ϊģ��ֻ����Instanize��ʱ�����Ҫ���壬
	����ֻ��Ҫ��ͷ�ļ��й���һ����С����C++Ԥ�ȷ���һ���ռ�ͺ��ˡ�

	�����С����Ҫ�Լ�Ԥ���ģ�����ֱ��дsizeof����Ȼ����Ҫ��ͷ�ļ��а���ClassA��ͷ�ļ��ˡ���Ȼ����
	������Ʋ������Ļ��������õ��Եķ�ʽ��ĳЩ�ط��õ������С��

	��������С����ģ�����������ˣ���ôSizeError_PleaseGetTheCorrectSizeByTheTemplateParam
	�ľ�̬������ڱ����ʱ��ͱ���������ģ�����������ֱ�ӵõ���ȷ�Ĵ�С
	��д��KG_CUSTOM_HELPER::TCommonParamBuffer<ClassA, 12>	m_pXXX;
	��C2079 SizeError_PleaseGetTheCorrectSizeByTheTemplateParam uses undefined struct 'KG_CUSTOM_HELPERS::Private::TSize<_Size>'
	with
	[
	_Size=432
	]
	�ͱ�ʾģ������е������Ŀռ�̫С��432������ȷ�Ĵ�С
	��������̫��Ҳ�ᱨ��ͬ��������ķ��������ȷ�Ĵ�С

	��ʵҲ���Ǳ���Ҫ�����С�ģ���������������SmartPtr�Ļ������Կ�����new�ķ�ʽ��������ʵ��������
	��������Ҫ���˷�һ���ڴ��ˣ��������ͨ�������ԱԤ�ȷ���ռ�ķ�ʽҲ��һ�£�������������еĻ�
	ƽ���޹ʶ��˸��Ӷȡ�

	2.��cpp�а����Ǹ�_Type�Ķ��塣
	��Ϊ��cpp�вŻ��ģ��ʵ�����������ඨ����cpp�в���Ҫ�������������ͷ�ļ�����ˡ�
	�����Ϳ�����������ֱ����m_pXXX->foo();֮��ķ����ˣ���ָ��ɡ�

	�ر�ע�⣺ʹ����������ָ�����Ĺ��죬���������ƹ��캯���ȱ�������cpp�ж���ģ���Ϊ���������ڱ�����Щ������
	ʱ����Ҫ����������͹��춨�塣����Ĭ�ϵ����������캯���Ƕ�̬���ɵģ�Ҳ����
	����AClass.h����ô����
	class BClass
	class AClass
	{
	KG_CUSTOM_HELPERS::TCommonParamBuffer<BClass, 4>	m_pBClass;
	};
	��ô��C.cpp�������ôд AClass a;�Ļ������벻������Ϊ��ʱ�򣬱��������������Ҫ��̬����AClass��Ĭ�Ϲ��캯��
	��������������ʱ���Ҳ���BClass�Ķ��塣��Ȼ����C.cpp�а���BClass.hҲ�ǿ��еģ�����������Υ�������������ָ��
	����ֹ�ඨ����ɢ�������ˡ�

	һ��Ҫ����
	class BClass
	class AClass
	{
	KG_CUSTOM_HELPERS::TCommonParamBuffer<BClass, 4>	m_pBClass;
	public:
	AClass();
	~AClass();
	};
	Ȼ����cpp�м���#include "BClass.h"
	��Ȼ�����C.cppҪֱ�����������ָ��Ļ�Ҳ��Ҫ#include "BClass.h"����������������һ������
	class BClass
	class AClass
	{
	BClass*	m_pBClass;
	public:
	AClass();
	~AClass();
	};

	Ҫ˵�������ָ�����������ַ�ʽ��ɶ��ͬ
	���ǲ���ҪAClass�е���new������Ҫ������ʧ�ܵ������û���ڴ�й©

	����C++�����˼ά���淴˼�ģ��ǳ�tricky ���� c++������c��ϰ�ߵ���Ҳ��tricky���������þ��ðɡ���ʵԭ��˵���
	��Ҳ���ǻ��¶��ѡ�


	3.�����Ҫ����ĳ�ʼ������Ҫ���Ǻ����_Newerģ�����

	4.������ָ������ʹ�ã��Ѿ�������->�������ˣ���if(! p)���﷨Ҳ�ǿ��Ե�

	5.��Ҫ��ȡ�ڲ���ָ��Ļ�����������д��
	ClassA* pXX = KGCommonBufferGetPtr(m_pXXX);	
	ע�ⲻ��Ҫ��KGCommonBufferGetPtrǰ��дnamespace,д�˷������С���������C++��׼�еĲ������ֲ��Ҳ��Ե�����ַ���
	SmartPtr��ͬ�ຯ��Ҳ������ʵ�ֵġ�

	6.delete���Ļ����Գ�����Ϊ�䱾����Ŀ��������������ͷ�ļ��а���HAS-A�����ͷ�ļ��ĳ�Ա�����㱾��Ҳ����
	deleteһ�����Ա����ֻ��delete��Ա��ָ�룩�������Ҫɾ������ô��KGCommonBufferRecreate(m_pXX);
	�����Ļ�����Buffer�����ԭ����ʵ������������ԭλ�ô�����һ���հ׵�ʵ����

	*/
	///Ŀ����Ϊ�˽��ͷ�ļ���ϣ���������ǲ���Ҫ_Type�Ķ���ģ�Ŀ�������Աһ����
	template<typename _Type, unsigned _uBufferSize, class _Newer = Private::TTypePlacementNewer<_Type> >
	class TCommonParamBuffer
	{
	private:
		enum{buffer_size = _uBufferSize, max_wastable_size = (buffer_size / 10) > 12 ? 12 : (buffer_size / 10)};
		char m_Buffer[buffer_size];
		_Type* m_pType;
	private:
		inline void Clear()
		{
			if(!m_pType)
			{
				ZeroMemory(m_Buffer, sizeof(m_Buffer));
				return;
			}	
			m_pType->~_Type();
			ZeroMemory(m_Buffer, sizeof(m_Buffer));
			m_pType = NULL;
		}
		inline _Type* Recreate()//ɾ��ԭ�����࣬���´���
		{
			Clear();
			m_pType = _Newer().MakeType(m_Buffer);
			_ASSERTE((void*)m_pType == (void*)m_Buffer);
			return m_pType;
		}

		inline void CheckSize()
		{
			/*
			������������ʾ

			1>D:\Kingsoft\Game\DataStores\Sword3\Source\KG3DEngine\KG3DEngine\Helpers\KGCommonBuffer.h(194) : error C2079: 'SizeError_PleaseGetTheCorrectSizeByTheTemplateParam' uses undefined struct 'KG_CUSTOM_HELPERS::Private::TSize<_Size>'
			1>        with
			1>        [
			1>            _Size=812
			1>        ]

			�����Ĵ�����ô��TCommonParamBuffer�Ĵ�С�ĳ� _Size��ֵ�ͺ��ˡ����������Bug���ĳ�812�Ϳ�����
			*/
#if defined(_DEBUG) | defined(DEBUG)//����С
			/*
			�������ģ��TypeSeletor�������ã�
			����3��������bool,Type1,Type2
			�����Ϊbool�������������㣬��ôResult��NullType����ô��������һ����ʱ��NullType�����ʲô��
			��������㣬��ôResult��Private::TSize����������û������������������������ʱ��������ͨ��
			���룬ϵͳ��������������ʾ������������������þ���TCommonParamBufferҪ������Ǹ���Ĵ�С
			*/
			typedef typename TTypeSelector<sizeof(_Type) <= buffer_size, NullType, Private::TSize<sizeof(_Type)> >::Result TypeSizeChecker;//�������๹��ʧ�ܾͻᱨ��
			TypeSizeChecker SizeError_PleaseGetTheCorrectSizeByTheTemplateParam;
			(void)SizeError_PleaseGetTheCorrectSizeByTheTemplateParam;
			//�������Check���ڱ�������̫���Buffer���˷ѿռ�
			// TODO: Fix this to debug
			//typedef typename TTypeSelector<buffer_size - sizeof(_Type) <= max_wastable_size, NullType, Private::TSize<sizeof(_Type)> >::Result TypeSizeChecker2;
			//TypeSizeChecker2 SizeError_WastingBufferSize_SeeTheTemplateParam;
			//(void)SizeError_WastingBufferSize_SeeTheTemplateParam;
#endif
		}
	public:
		TCommonParamBuffer()
			:m_pType(NULL)
		{
			CheckSize();			
			Clear();
			_ASSERTE(!m_pType);
			m_pType = _Newer().MakeType(m_Buffer);
			_ASSERTE((void*)m_pType == (void*)m_Buffer);
		}
		explicit TCommonParamBuffer(const TCommonParamBuffer& Other)
			:m_pType(NULL)
		{
			CheckSize();
			_ASSERTE(this != &Other);
			_ASSERTE(Other.m_pType);
			Clear();
			_ASSERTE(!m_pType);
			m_pType = _Newer().MakeType(m_Buffer, *Other.m_pType);
			_ASSERTE((void*)m_pType == (void*)m_Buffer);	
		}
		template<typename _InitParamType>
		explicit TCommonParamBuffer(const _InitParamType& Other)
			:m_pType(NULL)
		{
			CheckSize();			
			Clear();
			_ASSERTE(!m_pType);
			m_pType = _Newer().MakeType(m_Buffer, Other);
			_ASSERTE((void*)m_pType == (void*)m_Buffer);
		}
		TCommonParamBuffer& operator=(const TCommonParamBuffer& Other)
		{
			_ASSERTE(this != &Other);
			_ASSERTE(Other.m_pType);
			_Newer().CopyType(m_Buffer, *Other.m_pType);
			return *this;
		}

		~TCommonParamBuffer()
		{
			Clear();
		}

		inline _Type* operator->(){_ASSERTE(m_pType); return m_pType;}
		inline const _Type* operator->()const	{_ASSERTE(m_pType); return m_pType;}
		inline _Type& operator*(){_ASSERTE(m_pType); return *m_pType;}///�ڲ�һֱ��һ���������Բ��õ��ĳ�ȥ�������õ�
		inline const _Type& operator*()const{_ASSERTE(m_pType); return *m_pType;}

		//<�Ƚ�>
	private:
		// ����'if (sp)'��ʽ�ıȽϡ����������Զ�ת��֮�⣬�������ǲ���ȫ�ģ�
		// ָ��һ�������Զ�ת�;�û����������ָ��ı�����
		struct DummyClass
		{
			DummyClass() {}
		private:
			void operator delete(void*);
		};

	public:
		operator DummyClass*() const
		{
			if (! m_pType) return 0;
			static DummyClass t;
			return &t;
		}
		inline bool operator!()const
		{
			return m_pType == NULL;
		}
		inline friend bool operator==(const TCommonParamBuffer& pTP,
			const _Type pObj)
		{ return pTP.m_pType == pObj; }

		inline friend bool operator==(const _Type pObj,
			const TCommonParamBuffer& pTP)
		{ return pObj == pTP.m_pType; }

		inline friend bool operator!=(const TCommonParamBuffer& pTP,
			const _Type pObj)
		{ return !(pTP.m_pType == pObj); }

		inline friend bool operator!=(const _Type pObj,
			const TCommonParamBuffer& pTP)
		{ return !(pObj == pTP.m_pType); }
		//</�Ƚ�>

		/************************************************************************/
		/*ע��ʹ�õ�ʱ��Ҫ��namespace�޶�����Ȼ�ᴥ��C3767������һ�ֹ����
		�ַ�����friend����ֱ�ӼӶ��塣��SmartPtr��ʵ������Ҫ�����ַ��ġ�
		ʹ�õ�ʱ�������ͺ���
		KGCommonBufferGetPtr(xx);
		�����������C++��׼�еĲ������ֲ��Ҳ����ҵ����������
		*/
		/************************************************************************/
		friend inline _Type* KGCommonBufferGetPtr(TCommonParamBuffer& Buffer){return Buffer.m_pType;}
		friend inline const _Type* KGCommonBufferGetPtr(const TCommonParamBuffer& Buffer){return Buffer.m_pType;}
		friend inline _Type* KGCommonBufferRecreate(TCommonParamBuffer& Buffer){return Buffer.Recreate();}
	};
};
namespace KGCH = KG_CUSTOM_HELPERS;

#endif //_INCLUDE_KGCOMMONBUFFER_H_
