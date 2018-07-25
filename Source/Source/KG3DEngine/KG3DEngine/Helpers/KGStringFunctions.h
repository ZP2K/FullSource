////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGStringFunctions.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 10:50:03
//  Comment     : 
/*
��Ҫ�������ַ�������ֵ��ת��
atoi, ftoi֮��ĺ�������unicode�汾�����ϴ�����Ļ�������ͻ�ܸ������Ѽǣ�
��_ttoi, wtoi֮�࣬����ֵҲ��ͳһ���еĻ�û�д��󷵻صĶ��塣
����ͳһ����TValueToString �� TStringToValue
ͬʱ����C String, std::string, CString����KGStringFunctionsEx.h����Ϊ������û��CString����ģ�
�÷���
INT XX = xx;
std::tstring st;	(tstring��UNICODE������Զ����wstring)
BOOL bRet = TValueToString(xx, st);
bRet = TStringToValue(st, xx);
����
TCHAR czBuffer[100];
BOOL bRet = TValueToString(xx, czBuffer, _countof(czBuffer));
bRet = TStringToValue(czBuffer, _coutof(czBuffer), xx);

CString���÷���std::tstringһ��

����Buffer���Ȱ���NULL��β
Ŀǰֻ��ʹ��INT ,FLOAT, vector<INT>, UCHAR,D3DCOLOR���֡�����û�ж���
ʹ�õ�ʱ��ᱨ��
����Ȥ�Ļ�����λ�����Լ�����DOUBLE��ֻҪ�����ص��ǶѺ���������Ͼͺ���
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGSTRINGFUNCTIONS_H_
#define _INCLUDE_KGSTRINGFUNCTIONS_H_

#include "KGCommonWidgets.h"
#include <limits>

///stdafx.h������С�����
////////////////////////////////////////////////////////////////////////////////
namespace KG_CUSTOM_HELPERS
{
	//ȫ���ĳ�������ʵ��
	template<typename _Type>
	struct TValueStringMinBufferSize
	{
		enum{ Result = std::numeric_limits<_Type>::digits10 + 3,};///3�������²��֣�������һ�����ţ������е㣬������\0
	};
	template<>
	struct TValueStringMinBufferSize<FLOAT> 
	{
		enum{Result = TValueStringMinBufferSize<DOUBLE>::Result,};///��ΪFLOAT��������ʽת����ΪDouble������Ҫ����	
	};
	template<>
	struct TValueStringMinBufferSize<D3DCOLOR> 
	{
		enum{Result = 3 * 4 + 8, };///4����λ���������ٸ��û�8��������ķָ�����	
	};
	template<>
	struct TValueStringMinBufferSize<UCHAR>
	{
		enum{Result = TValueStringMinBufferSize<INT>::Result,};
	};
	template<>
	struct  TValueStringMinBufferSize<D3DXVECTOR3>
	{
		enum{Result = TValueStringMinBufferSize<FLOAT>::Result * 3 + 3};
	};
	template<typename _Type>
	struct TValueStringMinBufferSize<std::vector<_Type> > 
	{
		enum
		{
			MAX_ELEMENT = 50,
			MAX_LENGTH = 1024,
			POSIBLE_LENGTH = (TValueStringMinBufferSize<_Type>::Result + 2)*MAX_ELEMENT,///2�Ƿָ�����
			Result = POSIBLE_LENGTH > MAX_LENGTH? MAX_LENGTH : POSIBLE_LENGTH,
		};
	};
	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* �����ַ�����������ת�������к���TValueToString/TStringToValue��
	��TValueStringConvertor����ģ�塣�Ӻ�����ͨ������ʹ�û�����ģ����ʹ��
	ѡ��һ�֡����������͵�TValueToString/TStringToValueҲ������LPCTSTR��std::string��������
	tstring���Զ���꣬����UNICODE��std::wstring
	*/
	/************************************************************************/
	//D3DCOLOR////////////////////////////////////////////////////////////////////////
	BOOL TValueToString(D3DCOLOR Value, LPTSTR tcsBuffer, INT nSize);
	BOOL TStringToValue(LPCTSTR tcsBuffer, INT nSize, D3DCOLOR& Value);
	//INT////////////////////////////////////////////////////////////////////////
	BOOL TValueToString(INT Value, LPTSTR tcsBuffer, INT nSize);
	BOOL TStringToValue(LPCTSTR tcsBuffer, INT nSize, INT& Value);
	//UINT//////////////////////////////////////////////////////////////////////////
	BOOL TValueToString(UINT Value, LPTSTR tcsBuffer, INT nSize);
	BOOL TStringToValue(LPCTSTR tcsBuffer, INT nSize, UINT& Value);
	//UCHAR////////////////////////////////////////////////////////////////////////
	BOOL TValueToString(UCHAR Value, LPTSTR tcsBuffer, INT nSize);
	BOOL TStringToValue(LPCTSTR tcsBuffer, INT nSize, UCHAR& Value);
	//FLOAT////////////////////////////////////////////////////////////////////////
	BOOL TValueToString(FLOAT Value, LPTSTR tcsBuffer, INT nSize);
	BOOL TStringToValue(LPCTSTR tcsBuffer, INT nSize, FLOAT& Value);
	//D3DXVECTOR3//////////////////////////////////////////////////////////////////////////
	BOOL TValueToString(D3DXVECTOR3 Value, LPTSTR tcsBuffer, INT nSize);
	BOOL TStringToValue(LPCTSTR tcsBuffer, INT nSize, D3DXVECTOR3& Value);
	//VECTOR////////////////////////////////////////////////////////////////////////
	template<typename _Type>
	static BOOL TValueToString(std::vector<_Type>& Value, LPTSTR tcsBuffer, INT nSize)
	{
		std::tstring tempString;
		std::tstring typeConverString;
		INT nRet = -1;

		///�����������������vector<vector<INT> >֮��Ķ�������Ȼ�������̫�����ˣ��������Ҫ��
		_KG3D_DEBUG_STATIC_CHECK_T(sizeof(_Type) <= sizeof(INT), ValueStringConvertor_vector__TYPE_Element_Illegal);

		KG_PROCESS_ERROR(tcsBuffer && nSize >= TValueStringMinBufferSize<std::vector<_Type> >::Result);		
		for (std::vector<_Type>::iterator it = Value.begin(); it != Value.end(); it++)
		{
			_Type& AElement = *it;
			if (TValueToString(AElement, typeConverString))
			{
				tempString += typeConverString;
				tempString += _T(",");
			}
			KG_PROCESS_ERROR((int)tempString.length() < nSize);
		}
		KG_PROCESS_ERROR((int)tempString.length() < nSize);
		nRet = _tcscpy_s(tcsBuffer, nSize, tempString.c_str());
		KG_PROCESS_ERROR(nRet == 0);
		return TRUE;
Exit0:
		return FALSE;
	}
	template<typename _Type>
	static BOOL TStringToValue(LPCTSTR tcsBuffer, INT nSize, std::vector<_Type>& Value)
	{
		///�����������������vector<vector<INT> >֮��Ķ�������Ȼ�������̫�����ˣ��������Ҫ��
		_KG3D_DEBUG_STATIC_CHECK_T(sizeof(_Type) == sizeof(INT), ValueStringConvertor_vector__TYPE_Element_Illegal);

		///�����ַ�����������ʱԪ������
		int nSourceSize = nSize;
		int nValueSize = static_cast<int>(Value.size());
		int nRet = -1;
		TCHAR* pSource = NULL;
		std::vector<_Type> TempVec;

		KG_PROCESS_ERROR(nSourceSize < TValueStringMinBufferSize<std::vector<_Type> >::Result);///���Ҫ���ڣ�̫���˾ͷ���������

		{
			int nSourceBufferSize = nSourceSize +1;
			pSource = new TCHAR[nSourceBufferSize];

			KG_PROCESS_ERROR(pSource);
			nRet = _tcscpy_s(pSource, nSourceBufferSize, tcsBuffer);
		}

		///��ʼ��׼��strtok������ÿ�����������Ķ���ѹ����ʱ����
		KG_PROCESS_ERROR(nRet == 0);
		{
			TCHAR sep[] = _T(" ,\t\n");
			TCHAR* pToken = NULL;
			TCHAR* pNextToken = NULL;
			pToken = _tcstok_s(pSource, sep, &pNextToken);
			BOOL bRet = FALSE;
			while(pToken)///������ܷ�������е�Ԫ�أ��Ǿ��Ǵ��
			{
				std::tstring strTokenTemp(pToken);
				_Type TempElement;
				bRet = TStringToValue(strTokenTemp, TempElement);
				KG_PROCESS_ERROR(bRet);
				TempVec.push_back(TempElement);
				pToken = _tcstok_s(NULL, sep, &pNextToken);
			}	
		}
		if (nValueSize != 0 && (INT)(TempVec.size()) < nValueSize)///�Ѿ���Ԫ����,ȴ���ܷ�������е�Ԫ��
		{
			goto Exit0;
		}
		///���ѹ��Value
		Value.clear();
		for (int i= 0; i < (int)TempVec.size(); i++)
		{
			Value.push_back(TempVec[i]);
		}
		SAFE_DELETE_ARRAY(pSource);
		//SAFE_DELETE_ARRAY(pElementsArray);
		return TRUE;
Exit0:
		SAFE_DELETE_ARRAY(pSource);
		//SAFE_DELETE_ARRAY(pElementsArray);
		return FALSE;
	}	
	//ģ��汾////////////////////////////////////////////////////////////////////////
	template<typename _Type, size_t _Size>
	inline BOOL TValueToString(_Type Value, TCHAR (&String)[_Size])
	{
		return TValueToString(Value, (LPTSTR)String, _Size);
	}
	template<typename _Type, size_t _Size>
	inline BOOL TStringToValue(const TCHAR (&String)[_Size], _Type& Value)
	{
		return TStringToValue((LPCTSTR)String, _Size, Value);
	}
	template<typename _Type, size_t _Size>
	inline BOOL TValueToString(std::vector<_Type>& Value, TCHAR (&String)[_Size])///����vector���ͣ�����������
	{
		return TValueToString(Value, (LPTSTR)String, _Size);
	}
	template<typename _Type>
	inline BOOL TValueToString(_Type Value, std::tstring& String)
	{
		TCHAR Temp[TValueStringMinBufferSize<_Type>::Result];
		BOOL bRet = TValueToString(Value, Temp, _countof(Temp));
		if (bRet)
			String = Temp;
		return bRet;
	}
	template<typename _Type>
	inline BOOL TStringToValue(const std::tstring& String, _Type& Value)
	{
		return TStringToValue(String.c_str(), (INT)String.length(), Value);
	}
	template<typename _Type>
	inline BOOL TValueToString(std::vector<_Type>& Value, std::tstring& String)///����vector���ͣ�����������
	{
		TCHAR Temp[TValueStringMinBufferSize<_Type>::Result];
		BOOL bRet = TValueToString(Value, Temp, _countof(Temp));
		if(bRet)
			String = Temp;
		return bRet;
	}
	//////////////////////////////////////////////////////////////////////////
	template<typename _Type> 
	struct TValueStringConvertor
	{
		std::tstring Value2String(_Type Value)
		{
			TCHAR Temp[TValueStringMinBufferSize<_Type>::Result];
#if defined(_DEBUG) | defined(DEBUG)
			BOOL bRet = ::KG_CUSTOM_HELPERS::TValueToString(Value, Temp, _countof(Temp));
			_KG3D_DEBUG_FAIL_REPORT(bRet && _T("TValueStringConvertor::Value2String"));
#else
			::KG_CUSTOM_HELPERS::TValueToString(Value, Temp, _countof(Temp));
#endif			
			return Temp;
		}
		BOOL String2Value(std::tstring& strSource, _Type& Value)
		{
			return ::KG_CUSTOM_HELPERS::TStringToValue(strSource, Value);
		}
	};

	//////////////////////////////////////////////////////////////////////////

	extern VOID funcTestValueToString();	

	//�ַ�����ȡ,ȥ���ƶ��ķ�Χ��TrimString�е��ַ��ٷ���///////////////////////////////
	std::tstring GetTrimedString( LPCTSTR tczSource/*<=512*/, TCHAR tcFrom, TCHAR tcTo, LPCTSTR tczTrimSting /*= NULL/*<=5*/ );
	bool StrReplaceExtension(LPCTSTR lpIn, LPCTSTR NewExtension, LPTSTR lpOut, size_t Size);//�滻��׺
	template<size_t Size>
	bool StrReplaceExtension(LPCTSTR lpIn, LPCTSTR NewExtension, TCHAR (&lpOut)[Size])
	{
		return StrReplaceExtension(lpIn, NewExtension, lpOut, Size);
	}
	
	bool IsDirSpliter(TCHAR cTemp);//�Ƿ�Ŀ¼�ָ�����/����\\��

	struct StrPathSplited 
	{
		TCHAR driver[MAX_PATH];
		TCHAR dir[MAX_PATH];
		TCHAR fileName[MAX_PATH];
		TCHAR ext[MAX_PATH];

		StrPathSplited();
		StrPathSplited(const StrPathSplited& anotherObj);
		StrPathSplited& operator=(const StrPathSplited& anotherObj);

		template<size_t uBufferSize>
		BOOL GetCatedPath(TCHAR (&pathBuffer)[uBufferSize])const//�����driver�Ͱ���driver���������·����ʼ��������ͬʱ�ܹ��������·��
		{
			return GetCatedPath(pathBuffer, uBufferSize);
		}
		BOOL GetCatedPath(LPTSTR pathBuffer, size_t uBufferSize) const;//�����driver�Ͱ���driver���������·����ʼ��������ͬʱ�ܹ��������·��
		BOOL GetCatedDirectory(LPTSTR dirBuffer, size_t uBufferSize) const;	//��"C:/Program File/dir"�����ӣ�����汣֤û��б�ܣ�ͬ����driver�Ͱ���driver

	};
	enum
	{
		em_path_split_driver = 0x01,
		em_path_split_dir = 0x02,
		em_path_split_filename = 0x04,
		em_path_split_ext = 0x08,
		em_path_split_all_without_driver = em_path_split_dir | em_path_split_filename | em_path_split_ext,
		em_path_split_all = em_path_split_driver | em_path_split_dir | em_path_split_filename | em_path_split_ext,
	};
	BOOL StrPathSplit(LPCTSTR path, StrPathSplited& retStruct, DWORD dwMask = em_path_split_all);
	BOOL StrReplace(LPTSTR strSrc, TCHAR cCharToBeReplaced, TCHAR cDesChar);
	BOOL StrReplace(std::tstring& strSrc, TCHAR cCharToBeReplaced, TCHAR cDesChar);

	BOOL StrPathToLowerAndReplaceSlash(std::tstring& pathSrc);

	BOOL StrPathGetFullFileName(LPCTSTR strFilePathOrFileName, std::tstring& retString);	//�����ļ������㣬�ͺ�׺
};
//KG_CUSTOM_HELPERS
namespace KGCH = KG_CUSTOM_HELPERS;
#endif //_INCLUDE_KGSTRINGFUNCTIONS_H_
