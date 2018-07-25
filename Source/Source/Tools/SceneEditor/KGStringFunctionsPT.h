////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGStringFunctionsPT.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-25 11:10:07
//  Comment     : ģ����Ҫ��̬���ͣ������Ҫʹ�����飬ÿ���������Ͳ�ͬ�Ļ���
//	����TypeList�ͺ��ѽ���ˡ���ͨ��DWORD��ʶ�����ٷ���ȴ���Ի�ø��������ԣ�
//	ԭ�����þ�̬����ͨ������ת��Ϊ����ָ��
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGSTRINGFUNCTIONSPT_H_
#define _INCLUDE_KGSTRINGFUNCTIONSPT_H_

////////////////////////////////////////////////////////////////////////////////
namespace KSCENEEDITOR_HELPERCLASSES
{
	enum///��������ֵ����Ϊ�����������
	{
		KSCENE_COMMON_PARAM_D3DCOLOR = 0,
		KSCENE_COMMON_PARAM_INT = 1,
		KSCENE_COMMON_PARAM_FLOAT = 2,
		KSCENE_COMMON_PARAM_VEC_INT = 3,
		KSCENE_COMMON_PARAM_STRING = 4,
		KSCENE_COMMON_PARAM_UINT = 5,
		KSCENE_COMMON_PARAM_D3DXVECTOR3 = 6,
		KSCENE_COMMON_PARAM_INVALUE_TYPE = DWORD(-1),
	};
	template<typename _Type>
	struct TConvertorDWORDType;
#define  ADD_TCONVERTOR_DWORD_TYPE(_Type, dWord) template<>struct TConvertorDWORDType<_Type>{enum{result = dWord};}
	ADD_TCONVERTOR_DWORD_TYPE(INT, KSCENE_COMMON_PARAM_INT);
	ADD_TCONVERTOR_DWORD_TYPE(D3DCOLOR, KSCENE_COMMON_PARAM_D3DCOLOR);
	ADD_TCONVERTOR_DWORD_TYPE(FLOAT, KSCENE_COMMON_PARAM_FLOAT);
	ADD_TCONVERTOR_DWORD_TYPE(std::vector<INT>, KSCENE_COMMON_PARAM_VEC_INT);
	ADD_TCONVERTOR_DWORD_TYPE(std::tstring, KSCENE_COMMON_PARAM_STRING);
	ADD_TCONVERTOR_DWORD_TYPE(UINT, KSCENE_COMMON_PARAM_UINT);
	ADD_TCONVERTOR_DWORD_TYPE(D3DXVECTOR3, KSCENE_COMMON_PARAM_D3DXVECTOR3);
	INT	GetTypeSize(DWORD dwType);
	BOOL MakeDefaultValue(DWORD dwType, VOID* pBuffer, INT nBufferSize);
	BOOL ConvertValueToString(DWORD dwType, const VOID* pValue, INT nValueSize, std::tstring& csResult);
	BOOL ConvertStringToValue(DWORD dwType, const tstring& csSrc, VOID* pRetValue, INT nValueSize);
	BOOL ConvertorIsAValidType(DWORD dwType);
	BOOL ConvertorTypeCopy(DWORD dwType, const VOID* pFrom, VOID* pTo, INT nSize);
	BOOL ConvertorMakeObjectInBuffer(DWORD dwType, VOID* pBuffer, INT nSize);
	DWORD ConverKG3DTYPEToParamType(DWORD dwType);//����ֵKSCENE_COMMON_PARAM_INVALUE_TYPEʧ��
};
namespace KSH = KSCENEEDITOR_HELPERCLASSES;

#endif //_INCLUDE_KGSTRINGFUNCTIONSPT_H_
