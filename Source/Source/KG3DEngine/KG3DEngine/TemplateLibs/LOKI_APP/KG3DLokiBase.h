////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DLokiBase.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-16 19:35:12
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DLOKIBASE_H_
#define _INCLUDE_KG3DLOKIBASE_H_

////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*LOKI����һ����֤C++ǰ�ؼ����Ŀ⡪�������ܺ��ã����Ǽ�ֱ���Ǹ�ħ��һ���Ķ���
�����ճ���TypeList��Ȼ����������Convertor��һ���������ָ��ǿ�Ƹ��ǵİ취��ʵ����
һ��������Structת��Ϊһ���ַ�����ģ�塣
�ṹ���Ǻܺã�����Ϊ��������������TypeList���»�ԭԪ�����ͣ�
���������Ҳ֤����û����Դй©�ģ��ڶ�̬�����Ĺؼ�֡�༭����ʱ�����������ˡ�
*/
/************************************************************************/
namespace LOKI_APP
{
	template<int> struct CompileTimeError;
	template<> struct CompileTimeError<true> {};
	//////////////////////////////////////////////////////////////////////////
#define LOKI_STATIC_CHECK(expr, msg) \
	{ LOKI_APP::CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; }

#define LOKI_APP_STRINGER(name) (_T(#name))
	//////////////////////////////////////////////////////////////////////////
	namespace Private
	{
		///�ܷ��Tת����U,(ǰ�ߵ�����)
		template <class T, class U>
		struct ConversionHelper
		{
			typedef char Small;
			struct Big { char dummy[2]; };
			static Big   Test(...);
			static Small Test(U);
			static T MakeT();
		};
	};
	template <class T, class U>
	struct Conversion
	{
		typedef Private::ConversionHelper<T, U> H;
#ifndef __MWERKS__
		enum { exists = sizeof(typename H::Small) == sizeof(H::Test(H::MakeT())) };
#else
		enum { exists = false };
#endif
		enum { exists2Way = exists && Conversion<U, T>::exists };
		enum { sameType = false };
	};

	template <class T>
	struct Conversion<T, T>    
	{
		enum { exists = 1, exists2Way = 1,sameType = 1 };
	};

	template <class T>
	struct Conversion<void, T>    
	{
		enum { exists = 1, exists2Way = 0,sameType = 0 };
	};

	template <class T>
	struct Conversion<T, void>    
	{
		enum { exists = 1, exists2Way = 0,sameType = 0 };
	};

	template <>
	class Conversion<void, void>    
	{
	public:
		enum { exists = 1, exists2Way = 1,sameType = 1 };
	};
	//////////////////////////////////////////////////////////////////////////
#define  CAN_CONVER_TO(From, To) (::LOKI_APP::Conversion<const From, const To>::exists)
	//////////////////////////////////////////////////////////////////////////
#define SUPERSUBCLASS(T, U) \
	(::LOKI_APP::Conversion<const U*, const T*>::exists && \
	!::LOKI_APP::Conversion<const T*, const void*>::sameType)
	//////////////////////////////////////////////////////////////////////////
#define SUPERSUBCLASS_STRICT(T, U) \
	(SUPERSUBCLASS(T, U) && \
	!::LOKI_APP::Conversion<const T, const U>::sameType)
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	class NullType
	{

	};
	//////////////////////////////////////////////////////////////////////////
	template <int v>
	struct TInt2Type
	{
		enum { value = v };
	};
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	struct TType2Type
	{
		typedef T OriginalType;
	};	
	////////////////////////////////////////////////////////////////////////////////
	template <bool flag, typename T, typename U>
	struct TSelect
	{
		typedef T Result;
	};
	template <typename T, typename U>
	struct TSelect<false, T, U>
	{
		typedef U Result;
	};
};

#endif //_INCLUDE_KG3DLOKIBASE_H_
