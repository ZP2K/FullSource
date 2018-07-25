////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGStringFunctionsBase.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-7-12 14:27:34
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGSTRINGFUNCTIONSBASE_H_
#define _INCLUDE_KGSTRINGFUNCTIONSBASE_H_

#if defined(_DEBUG) | defined(DEBUG)
#define HEADER_NEW_X new(__FILE__, __LINE__)
#else
#define HEADER_NEW_X new
#endif

////////////////////////////////////////////////////////////////////////////////
namespace KG_CUSTOM_HELPERS
{
	namespace	Private
	{
		static const unsigned uCodePage = ::GetACP();
	};
	/************************************************************************/
	///�Ѿ������ںв��ԣ����Է���ʹ�ã������ǲ��ᵱ���ġ�
	/*���ڼ�Unicode�ͷ�Unicode�ַ���ת�������Զ�����Buffer�����������������Զ��ͷš� 
	�����ǲο���atlconv.h�����ATL��A2W��W2A���ʵ�֡��ѵ���������ַ����������ˣ�
	д������������е��ϴ���ֻ֧��char�������������Ժ�϶�Ҫ֧��Unicode�ģ���ǰ�Ĵ���Ͳ�����
	�������´���Ͳ���һ��Ϳ�ʼдUnicode��
	����������û��ATL֧�֣�������A2W֮��ģ�MultiByteToWideCharʲô��û�˼ǵ���ô�����������
	����������Ӧ�û����д������ɵ�

	ʹ�÷�ʽ:
	WChar* xxx = xxx;
	UnicodeConvertor AConvertor;
	DoSomething(AConvertor.ToA(xxx));

	ע���������ڿ���Convertor������ʱ����Զ��ͷ��ڴ棬����һ��Convertorֻ�ܸ�ת��һ��Char��WChar

	����Ϊ��������ã�����д�ַ�����ʱ����ToA��ToW��ת���ͺ���
	����ַ���Ҫ����д�ģ��磺ԭ��Ҫ��һ��char��bufferҪ���ú��������ں���ҪҪת����UniCode�ķ�ʽ

	///ԭ����������Ϊvoid GetName(char* pRet, int nBufferSize);
	char ABuffer[MAX_PATH];
	GetName(ABuffer, MAX_PATH);

	///���ں�������Ϊvoid GetName(WCHAR* pRet, int nBufferSize);
	///����ô�ĳ�������:
	char ABuffer[MAX_PATH];
	UnicodeConvertor RetString;
	GetName(RetString.CreateCharBuffer(MAX_PATH), MAX_PATH);
	//����GetName�Ľ����д��Convertor���������ַ����ˣ�
	//Ȼ����SelfToA/SelfToW���µõ��ַ���
	RetString.SelfToA(ABuffer);

	070814	����ToT�İ汾������ʲô�ַ�����ֻҪ��ToT�����Զ�����Unicode
	*/
	/************************************************************************/
	//�������������ļ�,�Ӻ����̣�Ӧ����stdafx�м���
	//#include "crtdbg.h"
	//#include <windows.h>

	class UnicodeConvertor
	{
		enum{max_size = 260,/*MAXPATH*/};
	public:
		UnicodeConvertor();
		UnicodeConvertor(UINT CodePage, DWORD dwFlag = 0):m_uWideCharBufferSize(0), m_uCharBufferSize(0), m_uCodePage(CodePage), m_dwFlag(dwFlag){}
		~UnicodeConvertor();

		size_t GetCharBufferSize(){return m_uCharBufferSize;}
		size_t GetWCharBufferSize(){return m_uWideCharBufferSize;}

		void SetCodePage(UINT CodePage){m_uCodePage = CodePage;}
		void SetFlag(DWORD dwFlag){m_dwFlag = dwFlag;}

		CHAR* GetCharBuffer(){return m_CBuffer;}
		WCHAR* GetWCharBuffer(){return m_WBuffer;}
		CHAR* CreateCharBuffer(size_t uSize);//���ڶ�ȡ
		WCHAR* CreateWCharBuffer(size_t uSize);//���ڶ�ȡ
		void ClearCharBuffer(){m_uCharBufferSize = 0;}
		void ClearWCharBuffer(){m_uWideCharBufferSize = 0;}
		size_t	GetCapacity(){return max_size;}
		///ע��sizeof(WCHAR)��һ����2
		inline WCHAR* ToW(const CHAR* lpChar, size_t uCharSize, WCHAR* lpWBuffer, size_t uBufferSizeInWChar)
		{
			return this->ToWHelper(lpChar, uCharSize, lpWBuffer, uBufferSizeInWChar, m_uCodePage, m_dwFlag);
		}
		WCHAR* ToW(const CHAR* lpChar, WCHAR* lpWBuffer, size_t uBufferSizeInWChar)
		{
			return this->ToWHelper(lpChar, strlen(lpChar)+1, lpWBuffer, uBufferSizeInWChar, m_uCodePage, m_dwFlag);
		}
		template<size_t _Size>
		WCHAR* ToW(const CHAR* lpChar, WCHAR (&lpWBuffer)[_Size])
		{
			return this->ToWHelper(lpChar, strlen(lpChar)+1, lpWBuffer, _Size, m_uCodePage, m_dwFlag);
		}
		WCHAR* ToW(WCHAR* p)const
		{
			return p;
		}
		const WCHAR* ToW(const WCHAR* p)const
		{
			return p;
		}

		CHAR* ToA(const WCHAR* lpWChar, size_t uWCharSize, CHAR* lpBuffer, size_t uBufferSize)
		{
			return this->ToAHelper(lpWChar, uWCharSize, lpBuffer, uBufferSize, m_uCodePage, m_dwFlag);
		}
		CHAR* ToA(const WCHAR* lpWChar, CHAR* lpBuffer, size_t uBufferSize)
		{
			return this->ToAHelper(lpWChar, wcslen(lpWChar)+1, lpBuffer, uBufferSize, m_uCodePage, m_dwFlag);
		}
		template<size_t _Size>
		CHAR* ToA(const WCHAR* lpWChar, CHAR (&lpBuffer)[_Size])
		{
			return this->ToAHelper(lpWChar, wcslen(lpWChar)+1, lpBuffer, _Size, m_uCodePage, m_dwFlag);
		}
		CHAR* ToA(CHAR* lpChar)const
		{
			return lpChar;
		}
		const CHAR* ToA(const CHAR* lpChar)const
		{
			return lpChar;
		}

		WCHAR* ToW(const CHAR* lpChar);
		CHAR* ToA(const WCHAR* lpWChar);

		const TCHAR* ToT(const char* lpChar);
		const TCHAR* ToT(const WCHAR* lpWChar);

		TCHAR* ToT(const char* lpChar, size_t uCharSize, TCHAR* lpTChar, size_t uTCharSize);;
		TCHAR* ToT(const WCHAR* lpWChar, size_t uWCharSize, TCHAR* lpTChar, size_t uTCharSize);;

		CHAR* SelfToA(){return this->ToA(GetWCharBuffer());}///������ٴ�ת��
		WCHAR* SelfToW(){return this->ToW(GetCharBuffer());}///������ٴ�ת��
		CHAR* SelfToA(CHAR* lpBuffer, size_t uBufferSize)
		{
			return this->ToAHelper(GetWCharBuffer(), GetWCharBufferSize(), lpBuffer, uBufferSize);
		}
		template<size_t _Size>
		CHAR* SelfToA(CHAR (&lpBuffer)[_Size])
		{
			return this->ToAHelper(GetWCharBuffer(), GetWCharBufferSize(), lpBuffer, _Size);
		}
		WCHAR* SelfToW(WCHAR* lpBuffer, size_t uBufferSize)
		{
			return this->ToWHelper(GetCharBuffer(), GetCharBufferSize(), lpBuffer, uBufferSize);
		}
		template<size_t _Size>
		WCHAR* SelfToW(WCHAR (&lpBuffer)[_Size])
		{
			return this->ToWHelper(GetCharBuffer(), GetCharBufferSize(), lpBuffer, _Size);
		}
	private:
		//WCHAR* m_lpWideChar;
		//CHAR*	m_lpChar;
		size_t m_uWideCharBufferSize;
		size_t m_uCharBufferSize;
		UINT m_uCodePage;
		DWORD m_dwFlag;

		
		enum{special_code = 0x12345678,};

		WCHAR	m_WBuffer[max_size];
		CHAR	m_CBuffer[max_size*2];//�����ַ���strlen���������������
	private:
		UnicodeConvertor(const UnicodeConvertor&);
		UnicodeConvertor& operator=(const UnicodeConvertor&);
	private:
		//���������Ǻ��ĺ���������ȥ��API�����������Ĳ�����
		//����WideCharSize����Length����WideChar�������������sizeof��������Buffer��С�����������Null��β
		WCHAR* ToWHelper(const CHAR* lpChar, size_t uCharSize, WCHAR* lpWBuffer
			, size_t uBufferSizeInWChar, UINT CodePage = special_code
			, DWORD dwFlag = 0 /*MB_PRECOMPOSEDҲ�ǿ��Ե�*/);
		CHAR* ToAHelper(const WCHAR* lpWChar, size_t uWCharSize
			, CHAR* lpABuffer, size_t uBufferSize, UINT CodePage = special_code
			, DWORD dwFlag = 0/*WC_COMPOSITECHECK*/);
	};
};

namespace KGCH = KG_CUSTOM_HELPERS;
#endif //_INCLUDE_KGSTRINGFUNCTIONSBASE_H_
