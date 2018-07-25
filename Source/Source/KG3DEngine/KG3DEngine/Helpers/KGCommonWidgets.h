////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGCommonWidgets.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 9:50:02
//  Comment     : ������Щ�꣬��������DEBUG��ʾֻ��DEBUGģʽ�Ż���Ч�������
/*	SELF��ʾ��Ŀ������_KG3D_DEBUG_SELF_ENABLE_OUTPUT=1��ʱ��Ż���֣����������
cpp�ļ���ͨ�����������������SELF�ꡣ
SELF��Ҫ���������ڴ���ͨ�������Լ���֪����ô���µģ��ܶ�ط���Ӧ��дASSERT��
��Ϊ�ϵ��Ļ����˲�֪����ô���£����ܶ�ط��ǿ���д����Ĵ�������֤��ʹ����
Ҳ��Ӱ��������еġ�����ASSERT�Ļ����������ȶ���������С�Ĵ�����Ĵ��ۣ���

//////////////////////////////////////////////////////////////////////////

_KG3D_DEBUG_STATIC_CHECK_T

���Ծ�̬�����������������ܱ�����ȷ���Ļ���
�ر�����ģ�����棬�����ط�Ҳ�ܺ���
��_KG3D_DEBUG_STATIC_CHECK_T(sizeof(T) == sizeof(INT), SizeUnMatch)
�������ʧ�ܣ��ᱨһ��Error_SizeUnMatch���������ڡ�

//////////////////////////////////////////////////////////////////////////

_kg_to_smaller_size_cast
_kg_to_bigger_size_cast

�÷���static_castһ��
�磺
INT x = 255;
UCHAR uC = _kg_to_smaller_size_cast<UCHAR>(x);
��Ҫ���ڽ����С��ƥ��ı���ǿת��ԭ���ǵ�λ������
�����С�����������Ļ�����ǿתINT��DWORD_PTR���ڱ����ʱ��ͱ���
��_kg_to_bigger_size_cast����С����ת����

//////////////////////////////////////////////////////////////////////////
kg_bytes_of_array(Array)��
���ڵĵ������С���������һ�����飬�ᱨ�����ھ�̬����Ƿ�����

//////////////////////////////////////////////////////////////////////////

_KG3D_CAN_CONVERT_TO
���ڼ�������Ƿ��ܹ�ת��
�磺
class TypeA
{
	operator int(){return 1;}
};
_KG3D_CAN_CONVER_TO(TypeA, int);	
������bool����

//////////////////////////////////////////////////////////////////////////

_KG3D_SUPERSUBCLASS
���ڼ�����������Ƿ���ڸ�������Ĺ�ϵ����ͬ����Ҳ��ɹ�
�磺
class TypeA{};
class TypeB : public TypeA{};
_KG3D_SUPERSUBCLASS(TypeA, TypeB)

_KG3D_SUPERSUBCLASS_STRICT
���ڼ�����������Ƿ���ڸ��������ϵ���Ҳ�������ͬ����

//////////////////////////////////////////////////////////////////////////

_KG3D_DEBUG_SUPPERSUB_POINTER_EQUAL

���ڼ��ָ������ת����ʱ���ܷ�Void*ǿת
��:
struct A{}; class B : public A{};
�����������void*����ָ��ĺ����У��϶�û������:
void* GetB(){return m_B;}
A* pA = (A*)GetB();
����������ؼ̳У���̳У�֮��ľͲ���������
��struct A{virtual ~A(){}}; struct B{virtual ~B(){}}
class C: public A, public B{}
void* GetB(){return m_C;}
B* pB = (C*)GetB();
�����϶������⣬��ΪB�������A���棬���ò�һ���
ת������ָ�����ȫ����C++�����ˣ����õĺ�������Ϊ��ȷ��
ʵ��ʹ�õ����������һЩ����ԭ���Լ�������ˡ�

//////////////////////////////////////////////////////////////////////////

_KG3D_DEBUG_STATIC_ASSERTE

��������ʱ��������������һ�μ�顣ԭ��������static����


//////////////////////////////////////////////////////////////////////////
KG_DECLARE_NO_DELETE_OP();
���ڽӿڣ���ֹdelete�ӿ�ָ��

////////////////////////////////////////////////////////////////////////////////
*/
#ifndef _INCLUDE_KGCOMMONWIDGETS_H_
#define _INCLUDE_KGCOMMONWIDGETS_H_
//��Ϊ����ļ�������ļ���������������ļ���ò�Ҫ��������ļ�
///stdafx.h������С�����
////////////////////////////////////////////////////////////////////////////////

#define  _STRINGER(x) _T(#x)   ///����һ�����԰ѷ��ű���ַ����ĺ�
#define  _STRINGERA(x) #x

//windows.hӦ��������ļ�֮ǰ����������ȻLPCTSTR�Ķ�����ܻ��ͻ
#ifndef VOID	//�����־��Windows�����ļ���û�б������ı�־
#if defined(UNICODE) | defined(_UNICODE)
typedef  const wchar_t* LPCTSTR;
#else
typedef  const char* LPCTSTR;
#endif
#endif

//�����TFILE��������MSDNд�ģ�������__FILE__�����UNICODE�汾
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)


#ifndef TFILE
#if defined(UNICODE) | defined(_UNICODE)
#define TFILE	__WFILE__
#else
#define TFILE	__FILE__
#endif
#endif

/*�����inline�������Խ������ȷ��sizeofһ������õ����Ƿ������С����������ָ���С�����⡣
����Ǿֲ����飬��֪sizeof�������ȷ������£���Ҫ�������������_countof(xx)*sizeof(xx);
��������ڳ�Ա���ȵ�������ú�Զ�����Ҳ�ȷ���᲻��ĵ�ʱ���õġ�
д���ǳ����죬�ο�_countof�����ģ�Template��֤��Ŀ���������Ľ���
*/
template <typename _CountofType, size_t _SizeOfArray>
char (*kg_bytes_of_array_helper(UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray*sizeof(_CountofType)];
#define kg_bytes_of_array(_Array) sizeof(*kg_bytes_of_array_helper(_Array))

//#pragma warning(disable:4505)

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef HrV	//��DXUT�д���ʹ��������������"V"����Effect������������зǳ�����
#undef HrV
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define HrV(condition)	{hr = (condition); _ASSERTE(SUCCEEDED(hr));}NULL
#else
#define HrV(condition)	{hr = (condition);}NULL
#endif

#ifdef PERROR
#error PERROR is defined before this file, everything will work unproperly, please try fix this macro conflict
#endif

//�������Щ�������ͼ�飬���KG_PROCESS_ERRORϵ�и����ײ��һЩ
#define PERROR_LOG_FUNC(conditionString)	KGLogPrintf(KGLOG_DEBUG, _T("LOG ERROR:  condition: %s, line: %d, function: %s"), conditionString, __LINE__, KG_FUNCTION)

#define PERROR_BASE(condition, funcIfError)	{if(!(condition)){(funcIfError); goto Exit0;}}NULL
#define PCOMERROR_BASE(condition, funcIfError)	{hr = (condition); PERROR_BASE(SUCCEEDED(hr), funcIfError);}NULL

#define	PERROR(condition)	PERROR_BASE(condition, NULL)
#define PCOMERROR(condition)	PCOMERROR_BASE(condition, NULL)

#define PERROR_LOG(condition)	PERROR_BASE(condition, PERROR_LOG_FUNC(#condition))

#define PCOMERROR_LOG(condition)	PCOMERROR_BASE(condition, PERROR_LOG_FUNC(_T("(hr)")#condition))	
////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
// ע��������Щ�꣬�����DEBUG���ͱ�ʾֻ��DEBUG�²�����Ч����SELF��
// ��ʾ��_KG3D_DEBUG_SELF_ENABLE_OUTPUT����Ч
// STATIC��ʾ��̬��������
/************************************************************************/
#if defined(_UNICODE) | defined(UNICODE)
#define _KG3D_RPTT0(errno, msg) _RPTW0(errno, msg)
#define _KG3D_RPTT1(errno, msg, arg1) _RPTW1(errno, msg, arg1)
#define _KG3D_RPTT2(errno, msg, arg1, arg2) _RPTW2(errno, msg, arg1, arg2)
#define _KG3D_RPTT3(errno, msg, arg1, arg2, arg3) _RPTW3(errno, msg, arg1, arg2, arg3)
#define _KG3D_RPTT4(errno, msg, arg1, arg2, arg3, arg4) _RPTW4(errno, msg, arg1, arg2, arg3, arg4)
#else
#define _KG3D_RPTT0(errno, msg) _RPT0(errno, msg)
#define _KG3D_RPTT1(errno, msg, arg1) _RPT1(errno, msg, arg1)
#define _KG3D_RPTT2(errno, msg, arg1, arg2) _RPT2(errno, msg, arg1, arg2)
#define _KG3D_RPTT3(errno, msg, arg1, arg2, arg3) _RPT3(errno, msg, arg1, arg2, arg3)
#define _KG3D_RPTT4(errno, msg, arg1, arg2, arg3, arg4) _RPT4(errno, msg, arg1, arg2, arg3, arg4)
#endif
//��������������Debug�ұ����_KG3D_DEBUG_ENABLE_OUTPUT���ز���ʹ�õ�
#if _KG3D_DEBUG_SELF_ENABLE_OUTPUT == 1
#if defined(DEBUG) | defined(_DEBUG)
#define _KG3D_DEBUG_SELF_OUTPUT0(msg)	_KG3D_RPTT0(_CRT_WARN, msg)
#define _KG3D_DEBUG_SELF_OUTPUT1(msg, arg1)	_KG3D_RPTT1(_CRT_WARN, msg, arg1)
#define _KG3D_DEBUG_SELF_OUTPUT2(msg, arg1, arg2)		_KG3D_RPTT2(_CRT_WARN, msg, arg1, arg2)
#define _KG3D_DEBUG_SELF_OUTPUT3(msg, arg1, arg2, arg3)	_KG3D_RPTT3(_CRT_WARN, msg, arg1, arg2, arg3) 
#define _KG3D_DEBUG_SELF_OUTPUT4(msg, arg1, arg2, arg3, arg4)	 _KG3D_RPTT4(_CRT_WARN, msg, arg1, arg2, arg3, arg4)
#else
#define _KG3D_DEBUG_SELF_OUTPUT0(msg)	NULL
#define _KG3D_DEBUG_SELF_OUTPUT1(msg, arg1)	NULL
#define _KG3D_DEBUG_SELF_OUTPUT2(msg, arg1, arg2)		NULL
#define _KG3D_DEBUG_SELF_OUTPUT3(msg, arg1, arg2, arg3)	NULL
#define _KG3D_DEBUG_SELF_OUTPUT4(msg, arg1, arg2, arg3, arg4)	NULL
#endif
#else
#define _KG3D_DEBUG_SELF_OUTPUT0(msg)	NULL
#define _KG3D_DEBUG_SELF_OUTPUT1(msg, arg1)	NULL
#define _KG3D_DEBUG_SELF_OUTPUT2(msg, arg1, arg2)		NULL
#define _KG3D_DEBUG_SELF_OUTPUT3(msg, arg1, arg2, arg3)	NULL
#define _KG3D_DEBUG_SELF_OUTPUT4(msg, arg1, arg2, arg3, arg4)	NULL
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������ֻ��Debug������ʹ��
#if defined(DEBUG) | defined(_DEBUG)
#define _KG3D_DEBUG_OUTPUT0(msg)	_KG3D_RPTT0(_CRT_WARN, msg)
#define _KG3D_DEBUG_OUTPUT1(msg, arg1)	_KG3D_RPTT1(_CRT_WARN, msg, arg1)
#define _KG3D_DEBUG_OUTPUT2(msg, arg1, arg2)		_KG3D_RPTT2(_CRT_WARN, msg, arg1, arg2)
#define _KG3D_DEBUG_OUTPUT3(msg, arg1, arg2, arg3)	_KG3D_RPTT3(_CRT_WARN, msg, arg1, arg2, arg3) 
#define _KG3D_DEBUG_OUTPUT4(msg, arg1, arg2, arg3, arg4)	 _KG3D_RPTT4(_CRT_WARN, msg, arg1, arg2, arg3, arg4)
#else
#define _KG3D_DEBUG_OUTPUT0(msg)	NULL
#define _KG3D_DEBUG_OUTPUT1(msg, arg1)	NULL
#define _KG3D_DEBUG_OUTPUT2(msg, arg1, arg2)		NULL
#define _KG3D_DEBUG_OUTPUT3(msg, arg1, arg2, arg3)	NULL
#define _KG3D_DEBUG_OUTPUT4(msg, arg1, arg2, arg3, arg4)	 NULL
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������ڼ��ʽ����,���ڳ���Ա�Լ������Լ��Ĵ��룬����ر���_KG3D_DEBUG_ENABLE_OUTPUT���أ�ʧ�ܵı��ʽ���ᴥ����
//ֻ����Output���ڴ����������������У��������Release�µĻ������ʧЧ
#if defined(DEBUG) | defined(_DEBUG)
#if _KG3D_DEBUG_SELF_ENABLE_OUTPUT == 1
#define _KG3D_DEBUG_SELF_ASSERTE(condition)	_ASSERTE(condition)
#else
#define _KG3D_DEBUG_SELF_ASSERTE(condition)	\
	do	\
	{	\
	if (!(condition))	\
		{	\
		::OutputDebugString(_T("��������û�д���")_STRINGER(condition)_T("\n"));	\
		}	\
	}while(false)
#endif
#else
#define	_KG3D_DEBUG_SELF_ASSERTE(condition)	NULL 
#endif
//////////////////////////////////////////////////////////////////////////
#if defined(UNICODE) | defined(_UNICODE)
#define _KG3D_DEBUG_CrtDgbReportT	_CrtDbgReportW
#else
#define _KG3D_DEBUG_CrtDgbReportT	_CrtDbgReport
#endif
//��������Ĭ���ڴ��������¾�ֻ���ӡ�����󣬲��������
#if defined(DEBUG) | defined(_DEBUG)
#define _KG3D_DEBUG_FAIL_REPORT(condition)	\
	NULL;	\
	{	\
	if (!(condition))	\
		{	\
		::OutputDebugString(_T(":��������ʧ�ܣ�"));	\
		::_CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, _STRINGERA(condition));	\
		::OutputDebugString(_T("\n"));	\
		}	\
	}NULL;
#else
#define	_KG3D_DEBUG_FAIL_REPORT(condition)	NULL 
#endif
//////////////////////////////////////////////////////////////////////////
//�ж�����Ϣ�İ汾
#if defined(DEBUG) | defined(_DEBUG)
#define _KG3D_DEBUG_FAIL_REPORT_MSG(condition, STRING)	\
	NULL;	\
	{	\
	if (!(condition))	\
		{	\
		::OutputDebugString(_T(":��������ʧ�ܣ�"));	\
		::_CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, _STRINGERA(STRING));	\
		::OutputDebugString(_T("\n"));	\
		}	\
	}NULL;
#else
#define	_KG3D_DEBUG_FAIL_REPORT_MSG(condition, STRING)	NULL 
#endif
//////////////////////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG)
#define _KG3D_DEBUG_REPORT(STRING)	::_CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL,  _STRINGERA(STRING)"\n")
#else
#define	_KG3D_DEBUG_REPORT(STRING)	NULL 
#endif
//////////////////////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG)
#if _KG3D_DEBUG_SELF_ENABLE_OUTPUT == 1
#define _KG3D_DEBUG_SELF_REPORT(STRING)	_KG3D_DEBUG_REPORT(STRING)
#else
#define _KG3D_DEBUG_SELF_REPORT(STRING)	NULL
#endif	
#else
#define	_KG3D_DEBUG_SELF_REPORT(STRING)	NULL 
#endif
//////////////////////////////////////////////////////////////////////////
//����������������鲻��Ϊ��ı���ǰ������ʾ�����condition
#if defined(DEBUG) | defined(_DEBUG)
#define  _KG3D_STATIC_CHECK(condition) \
	do	\
	{	\
	int _kg3d_assert_array[(condition)?1:0];	\
	} while(false)
#else
#define  _KG3D_STATIC_CHECK(condition) NULL
#endif
//////////////////////////////////////////////////////////////////////////
///����Template���������ɾ���,ע��falseû���ػ�
template<int> struct _KG3D_DEBUG_CompileTimeError;
template<> struct _KG3D_DEBUG_CompileTimeError<true> {};

//////////////////////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG)
#define _KG3D_DEBUG_STATIC_CHECK_T(expr, msg) \
{ _KG3D_DEBUG_CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; }
#else
#define  _KG3D_DEBUG_STATIC_CHECK_T(expr, msg) NULL
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef KG_DECLARE_NO_DELETE_OP
#if defined(DEBUG) | defined(_DEBUG)
#ifdef __cplusplus
#define KG_DECLARE_NO_DELETE_OP()	\
protected:	\
void operator delete(void* _Ptr) throw(){_ASSERTE(NULL && _T("����ʹ��Release���ͷţ��ֻ�����ȷд��::delete p;"));}	\
void operator delete(void *, void *) throw( ){_ASSERTE(NULL && _T("����ʹ��Release���ͷţ��ֻ�����ȷд��::delete p;"));}	\
void operator delete(void* _Ptr,const std::nothrow_t&) throw(){_ASSERTE(NULL && _T("����ʹ��Release���ͷţ��ֻ�����ȷд��::delete p;"));}	\
void operator delete[](void* _Ptr) throw();	\
void operator delete[](void*, void *) throw();	\
void operator delete[](void* _Ptr, const std::nothrow_t&) throw()
#else
#define KG_DECLARE_NO_DELETE_OP()	void _unusable_void_func()
#endif
#else
#define KG_DECLARE_NO_DELETE_OP()	void _unusable_void_func()
#endif
#endif

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*���ڽ��type cast truncation֮�࣬��С��ƥ���warning, ͬʱ����Ӧ��
64bit������ԭ����ǿ�Ƶ�λ�����������С����Ҫ�󣬻��ڱ����ʱ���
�������档
ʹ�÷������£�
int nA = 10;
char cAChar = _kg_to_smaller_size_cast<char>(nA);
nA = _kg_to_bigger_size_cast<int>(cAChar);
*/
/************************************************************************/

template< typename _Ret, typename _Src> 
inline _Ret _kg_to_smaller_size_cast(_Src src)
{
	_KG3D_DEBUG_STATIC_CHECK_T(sizeof(_Src) >= sizeof(_Ret), _to_smaller_size_cast);
	return *(_Ret*)(&src);
}
template< typename _Ret, typename _Src>
inline _Ret _kg_to_bigger_size_cast(_Src src)
{
	_KG3D_DEBUG_STATIC_CHECK_T(sizeof(_Src) <= sizeof(_Ret), _to_equal_size_cast_helper_unmatch);
	_Ret aRet = 0;
	*(_Src*)(&aRet) = src;
	return aRet;
}

inline bool BOOL2bool(BOOL bValue)
{
	return !!bValue;
}

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

	//////////////////////////////////////////////////////////////////////////
	template <class T, class U>
	struct Conversion
	{
		typedef Private::ConversionHelper<T, U> H;
		//#ifndef __MWERKS__
		enum { exists = sizeof(typename H::Small) == sizeof(H::Test(H::MakeT())) };
		//#else
		//	enum { exists = false };
		//#endif
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
};

//////////////////////////////////////////////////////////////////////////
#define  _KG3D_CAN_CONVER_TO(From, To) (::Private::Conversion<const From, const To>::exists)
//////////////////////////////////////////////////////////////////////////
#define _KG3D_SUPERSUBCLASS(T, U) \
	(::Private::Conversion<const U*, const T*>::exists && \
	!::Private::Conversion<const T*, const void*>::sameType)
//////////////////////////////////////////////////////////////////////////
#define _KG3D_SUPERSUBCLASS_STRICT(T, U) \
	(_KG3D_SUPERSUBCLASS(T, U) && \
	!::Private::Conversion<const T, const U>::sameType)
//////////////////////////////////////////////////////////////////////////
namespace Private
{	
	template<class _Supper, class _Sub>
	bool ClassPointerStaticUpperCastChecker()
	{
		bool bSupperSubClassRelationShip = _KG3D_SUPERSUBCLASS(_Supper, _Sub);////����Ƿ��������ϵ
		bool bPointerEqual = (void*)(_Supper*)(0x00000001) == (void*)(_Supper*)(_Sub*)(0x00000001);
		return bSupperSubClassRelationShip && bPointerEqual;
	}

	struct StaticAsserter
	{
		StaticAsserter(bool condition){_ASSERTE(condition);}
	};
};
#if defined(_DEBUG) | defined(DEBUG)
#define _KG3D_DEBUG_STATIC_ASSERTE(condition) { static ::Private::StaticAsserter _SAssert(condition); }NULL
#else 
#define _KG3D_DEBUG_STATIC_ASSERTE(condition) NULL
#endif

#define  _KG3D_IS_SUPPERSUB_POINTER_EQUAL(supper, sub)	::Private::ClassPointerStaticUpperCastChecker<supper, sub>()

#if defined(_DEBUG) | defined(DEBUG)
#define _KG3D_DEBUG_SUPPERSUB_POINTER_EQUAL(supper, sub) _KG3D_DEBUG_STATIC_ASSERTE(_KG3D_IS_SUPPERSUB_POINTER_EQUAL(supper, sub))
#else
#define _KG3D_DEBUG_SUPPERSUB_POINTER_EQUAL(supper, sub) true
#endif


template<class Type>
__forceinline ULONG KGGetRef(Type* p)
{
	_ASSERTE(NULL != p);
	p->AddRef();
	return p->Release();
};

inline ULONG QueryRef(IUnknown* pUnknown)
{ 
	return KGGetRef(pUnknown);
}

template<class Type>
inline VOID KGReleaseUntillDestructed(Type*& p)
{
	if(NULL == p)
		return;
	ULONG uRef = KGGetRef(p);
	for (ULONG i = 0; i < uRef; ++i)
	{
		p->Release();
	}
	p = NULL;
}

namespace Private
{
	BOOL KGIsVirtualPointerValidHelper(LPVOID p);
};
template<class Type>
inline BOOL KGIsVirtualPointerValid(const Type* p)
{
	if(sizeof(Type) < sizeof(LPVOID))
		return FALSE;
	return Private::KGIsVirtualPointerValidHelper((LPVOID)p);
}

//#pragma warning(default:4505)
#endif //_INCLUDE_KGCOMMONWIDGETS_H_
