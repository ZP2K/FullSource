////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGTester.h
//  Version     : 2.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-6-12 15:16:07
//  Comment     : 
/*
�÷���
�ѱ��ļ�������stdafx.h�У�ǰ�������#include <crtdbg.h>

��CPP��
#if defined(KG_ENABLE_TDD)
KG_TEST_BEGIN(xxxx)
{
	д��Ĳ��ԡ����ڳ�Ա�ͺ�����β��ԣ���ο����������е����ӡ�
}
KG_TEST_END(xxxx)
#endif

�÷�2 ClassTest��1.5�汾����
����һ����AClass�������ǰ�����
class AClass : public XX, public XX ...
{
	KG_TEST_DECLARE_CLASS_TEST(AClass)
public:
	...
};

��Cpp��
#if defined(KG_ENABLE_TDD)
KG_TEST_CLASS_TEST_BEGIN(AClass)
{
	д��Ĳ��ԡ���KG_TEST_BEGIN��ͬ���ǣ��������Access���˽�г�Ա
}
KG_TEST_CLASS_TEST_END(AClass)
#endif

��Ϊ��TDD�л�ʹ��׮�����������п��������ĺ����л��׳������쳣�������ṹ���쳣��
������ǳ�ʼ����ʱ�򣬺���������������ɵģ���ȫ�ֵ�Deviceû�д�����Ҳ���ܾ���
����ȱ�ݣ��ⶼҪ��顣���������ܹ���׽c++�쳣��Windows�ṹ�쳣����������쳣����
��Ҫ�Բ��Դ�����е�����ȷ���쳣������λ�á�


��Ҫע�����㡣������ʱ���β��ֲ��Դ��룬���Ժ�Ҫ��������ȱ��ֻ������
�鷳��

��Щ��ȫ���Ǿ���UNICODE����UNICODE��DEBUG��RELEASE����ϻ������Եģ�
�ܹ���ȷ�����ļ�λ�ã���ȷ��׽����ȱ��

֮����Ҫ�ں��������#if defined����ΪҪ�û���ȷ��Щ������ʲôʱ�������õģ��������д��
Release�汾����벻������װ��Щ�꣬�������Զ���ӦRelease��ʱ��Ϊ���Ǹ������⣬��Ϊû��#if��ס��
��δ�εĺ��������������һ��

��֪���⣺��Ŀ����
C++/General/Debug Information format������Program Database for Edit & Continue (/ZI)
��Ȼ__FILE__���__LINE__�겻�ܻ��ã�VS��Bug���ο�Google

*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGTESTER_H_
#define _INCLUDE_KGTESTER_H_
#include <tchar.h>

//<Config>
#if defined(DEBUG) | defined(_DEBUG)
#ifndef KG_ENABLE_TDD
#define KG_ENABLE_TDD 1				//��defineȥ���Ϳ��Խ�ֹ���е�TDD
#endif
#endif

#define KG_TDD_PROGAMER_OR_AUTO	1	//����Աģʽ�����Զ�������ģʽ

#define KG_TDD_USE_CHINESE	1		//ʹ����������Ӻ�ϵͳ���������Ҫͨ�õĻ�����þͲ�Ҫ�����ģ������ȷ������Ŀ�������Ŀ����˻�û����ô��
//</Config>

////////////////////////////////////////////////////////////////////////////////
#ifdef KG_TEST_BEGIN
#pragma message("KG_TEST_BEGIN�걻�ظ����壬���ͷ�ļ�����")
#undef KG_TEST_BEGIN
#endif

//windows.hӦ��������ļ�֮ǰ����������ȻLPCTSTR�Ķ�����ܻ��ͻ
#ifndef VOID	//�����־��Windows�����ļ���û�б������ı�־
#if defined(UNICODE) | defined(_UNICODE)
typedef  const wchar_t* LPCTSTR;
#else
typedef  const char* LPCTSTR;
#endif

#define HRESULT unsigned long
#endif

//�����TFILE��������MSDNд�ģ�������__FILE__�����UNICODE�汾,Ϊ�˼��ݷ�MFC�������в���ʹ��ͨ��c++�ؼ���
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

#if	KG_TDD_PROGAMER_OR_AUTO==1
#define KG_TDD_FAILED_IMMEDIATELY	1	//�������̵����������ǽ�����
#else
#define KG_TDD_FAILED_IMMEDIATELY	0	
#endif

#ifndef KG_TDD_CHECK
#if KG_TDD_FAILED_IMMEDIATELY == 1		//����Ǵ������̵���������ô��_CRT_ERROR����Assert���ڣ������������
#define KG_TDD_CHECK(condition)	{if(!(condition) && (1==TDD::Report(_CRT_ERROR, _T("Failed: ")_T(#condition)_T(""), TFILE, __LINE__)))_CrtDbgBreak();}NULL
#else
#define KG_TDD_CHECK(condition)	{if(!(condition))TDD::Report(_CRT_WARN, _T("Failed: ")_T(#condition)_T(""), TFILE, __LINE__);}NULL
#endif
#endif

#if defined(DEBUG) | defined(_DEBUG)	//TDD��Release�����ǲ����õ�

namespace TDD
{
	extern int IsTDDWorking();

	typedef void (*TypeTestFunc)(LPVOID);

	extern int Report(int Level, LPCTSTR report, LPCTSTR file, unsigned line);
	extern void RunTest(TypeTestFunc func, LPVOID pExtraParam, LPCTSTR unitName, LPCTSTR file, unsigned line);
	extern int	RunTogetherOrRunAtOnce();	//��ע��һ�����л��ǲ�ע����������
	extern void RegisterTest(TypeTestFunc func, LPCTSTR unitName, LPCTSTR fileName, unsigned line);
};

#define TDD_MAIN_FUNC_NAME	_T("RunTDD")

#define TDD_DLLAPI extern "C" __declspec(dllexport)

typedef HRESULT (*TypeTDDMainFunc)(void*);

TDD_DLLAPI HRESULT RunTDD(void* reservedParam);	//TDD�������к�������DLL��������

#define KG_TEST_MAKE_NAME(name)		KGTest##name
#define KG_TEST_MAKE_INSTANCE_NAME(name) KGTest##name##Instance

//////////////////////////////////////////////////////////////////////////

#define _PRIVATE_KG_TEST_BEGIN(name)	\
struct KG_TEST_MAKE_NAME(name)			\
{										\
	static void TestFunc(LPVOID)		\
	{

#define _PRIVATE_KG_TEST_END(name)		\
	}									\
	KG_TEST_MAKE_NAME(name)()			\
	{									\
		if(TDD::RunTogetherOrRunAtOnce())			\
		{											\
			TDD::RegisterTest(&TestFunc, _T(#name), TFILE, __LINE__);\
		}											\
		else										\
		{											\
			TDD::RunTest(&TestFunc, NULL, _T(#name), TFILE, __LINE__);\
		}											\
	}												\
};									

//////////////////////////////////////////////////////////////////////////

#define KG_TEST_BEGIN(name)	_PRIVATE_KG_TEST_BEGIN(name)
#define KG_TEST_END(name)		\
	_PRIVATE_KG_TEST_END(name)	\
	static KG_TEST_MAKE_NAME(name) KG_TEST_MAKE_INSTANCE_NAME(name);

//////////////////////////////////////////////////////////////////////////
#define  KG_TEST_MAKE_CLASS_TEST_NAME(classname)	KGTestClassTest##classname
#define  KG_TEST_MAKE_CLASS_TEST_INSTANCE_NAME(classname) KGTestClassTest##classname##Instance

#define KG_TEST_DECLARE_CLASS_TEST(classname)	\
	public:										\
		static void TestFunc(LPVOID);					\
	private:

#define KG_TEST_CLASS_TEST_BEGIN(classname)		\
	void classname::TestFunc(LPVOID)			\
	{											\

#define KG_TEST_CLASS_TEST_END(classname)		\
	}											\
	struct KG_TEST_MAKE_CLASS_TEST_NAME(classname)	\
	{												\
		KG_TEST_MAKE_CLASS_TEST_NAME(classname)()	\
		{											\
			if(TDD::RunTogetherOrRunAtOnce())		\
			{										\
				TDD::RegisterTest(&classname::TestFunc, _T(#classname), TFILE, __LINE__);	\
			}														\
			else													\
			{														\
				TDD::RunTest(&classname::TestFunc, NULL, _T(#classname), TFILE, __LINE__);	\
			}														\
		}															\
	};																\
	static KG_TEST_MAKE_CLASS_TEST_NAME(classname) KG_TEST_MAKE_CLASS_TEST_INSTANCE_NAME(classname);
#else
#define  KG_TEST_DECLARE_CLASS_TEST(classname)
#endif
//////////////////////////////////////////////////////////////////////////

//����ĺ������Զ����ų�TDD����µĴ������
#if defined(KG_ENABLE_TDD)
#define KGLOG_PROCESS_ERROR_3D(condition)	\
{	\
	if(! (condition))	\
{	\
	if(! TDD::IsTDDWorking())	\
	KGLogPrintf(KGLOG_DEBUG,    \
	"Error:(%s) at line %d in %s\n", #condition, __LINE__, KG_FUNCTION);	\
	goto Exit0;	\
}	\
}NULL
#else
#define KGLOG_PROCESS_ERROR_3D(condition)	\
{	\
	if(! (condition))	\
	goto Exit0;	\
}NULL
#endif

#endif //_INCLUDE_KGTESTER_H_
