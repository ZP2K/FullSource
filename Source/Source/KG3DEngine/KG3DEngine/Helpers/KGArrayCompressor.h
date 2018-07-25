////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGArrayCompressor.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 16:35:26
//  Comment     : �ֹ�д������ѹ���������ǰ���������������ͬԪ��ѹ����{Ԫ�أ�����}�ķ�ʽ
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGARRAYCOMPRESSOR_H_
#define _INCLUDE_KGARRAYCOMPRESSOR_H_
////////////////////////////////////////////////////////////////////////////////
namespace KG_CUSTOM_HELPERS
{
	class ArrayCompressor
	{
	public:
		enum
		{
			MAX_WIDTH = 4096,	//256*16	//���Ľ��ܵĿ�
			MAX_HEIGHT = MAX_WIDTH,			//���ĸ�
			MAX_ELEMENT = SHRT_MAX*4,		//��಻����ÿ��10��Element
			MAX_DIFF_DATA = 256,			//���ֻ����256����ͬ��Ԫ��
		};
		struct Element 
		{
			UCHAR m_ucData;
			UCHAR m_ucCount;
		};
	public:
		ArrayCompressor();
		~ArrayCompressor();

	private:
		Element* m_cDataArray;
		INT* m_pRowIndexArray;
		DWORD*	m_cDiffDataArray;
		INT m_nDiffDataCount;
		INT m_nElementCount;///����һ������m_cDataArray����Ŀռ�Ĵ�С
		INT m_nWidth;
		INT m_nHeigth;
		UCHAR m_ucErrData;
		UCHAR m_ucDefaultData;///�����滻ErrData
		INT m_nDiffDataEnumer;
		std::tstring	m_strError;
	private:
		BOOL GenerateDiffDataArray();	//�������ɲ�ͬԪ�ص����У�ֻ�б༭������Ҫ���
		void ClearDiffArray();
		BOOL RemoveErrData();			
		BOOL ChangeData(UCHAR uDataToChange, UCHAR uNewData);	//��ĳ��Ԫ���滻������һ��Ԫ��
		//<��������������صĴ���>
	public:
		enum{IsIsoFalse = -1, IsNotIso = 0, IsIso = 1,};
		enum{DirUpper = 0, DirLower = 1, DirLeft = 2, DirRight = 3,};///������أ���Ҫ��
		enum{UseUpper = 0, UseLower = 1, UseBothUpperLower = 2,};
	private:
		Element* GetRowElementWhenEliminating(INT nRow, INT nColumn);
		INT IsIsoVerticalHelper(TNumToType<UseUpper>,			INT nRow, INT nColumn, UCHAR uCurrData, UCHAR& uUpperData, UCHAR& uLowerData);
		INT IsIsoVerticalHelper(TNumToType<UseLower>,			INT nRow, INT nColumn, UCHAR uCurrData, UCHAR& uUpperData, UCHAR& uLowerData);
		INT IsIsoVerticalHelper(TNumToType<UseBothUpperLower>,	INT nRow, INT nColumn, UCHAR uCurrData, UCHAR& uUpperData, UCHAR& uLowerData);
		template<size_t uUsage>
		inline INT IsIsoVertical(INT nRow, INT nColumn, UCHAR uCurrData, UCHAR& uUpperData, UCHAR& uLowerData)
		{
			return IsIsoVerticalHelper(TNumToType<uUsage>(), nRow, nColumn, uCurrData, uUpperData, uLowerData);
		}
		BOOL EliminateIsolatedData(INT nIsoCount);///�ڽ����ٸ������ǹ����ģ�ɾ����������ɾ���������ɵı�����Ƭ
		BOOL EliminateIsolatedValue(INT nTotalValue);///����������Value��
		//</��������������صĴ���>

		inline INT GetRowBeginIndex( INT nRow );//��N�дӵڼ�Ԫ�ؿ�ʼ
		inline INT GetRowCount(INT nRow);		//��N���м���Ԫ��
	public:
		UINT GetMemorySize(){return sizeof(ArrayCompressor) + m_cDataArray ? sizeof(Element)*m_nElementCount : 0 + m_pRowIndexArray? sizeof(INT) * (m_nHeigth+1) : 0;}//�õ��ڴ�ռ�ã����ڹ��ƿռ�
		BOOL Compress(const UCHAR* cInputRawArray, INT nHeight, INT nWidth, INT nTolerance = 0, INT nIsoCount = 0, INT nTotalValue = 0);	//ѹ����������
		BOOL GetDiffDataArray(DWORD* pData, INT nBufferCount);	//�õ���ͬԪ�ص�����
		BOOL IsDataArrayGenerated();	//ѹ�������Ƿ�����
		BOOL IsDiffDataArrayGenerated();	//��ͬԪ�������Ƿ�����
		INT	GetDiffDataArrayCount();
		void Clear();

		BOOL SetErrorDate(UCHAR cErr);
		UCHAR GetErrorData(){return m_ucErrData;}
		UCHAR GetDefaultData() const { return m_ucDefaultData; }
		BOOL SetDefaultData(UCHAR val);

		UCHAR GetData(INT nRow, INT nColumn);	//����Ԫ�أ������ʱ�򷵻�DefaultData
		BOOL GetData(INT nRow, INT nColumn, INT& nRet);	//����Ԫ�أ����󷵻�FALSE
		INT GetHeight(){return m_nHeigth;}
		INT GetWidth(){return m_nWidth;}

		std::tstring&	GetErrorString(){return m_strError;}
		
		BOOL PushDataInBuffer(UCHAR* pBuffer, INT nBufferSize);	//�����ݻ�ԭ�ɶ�ά����
#if defined(DEBUG) | defined(_DEBUG)
		VOID DebugPrintByRow();
		VOID DebugPrintRawCompressedData();
		VOID DebugPrintRawArray();
#endif
	};
}
namespace KGCH = KG_CUSTOM_HELPERS;
#endif //_INCLUDE_KGARRAYCOMPRESSOR_H_
