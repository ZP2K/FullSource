////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGArrayCompressor.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 16:35:19
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "KGArrayCompressor.h"
#include "KGCommonWidgets.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace KG_CUSTOM_HELPERS
{
	namespace
	{
		template<typename _Type>
		inline _Type MyDistance(_Type c1, _Type c2)
		{
			return c1 > c2 ? c1 - c2 : c2 - c1;
		}
		inline bool IsInTolerance(UCHAR cChar, UCHAR cStandard, UCHAR cTolerance)
		{
			if (MyDistance(cChar, cStandard) <= cTolerance)
				return true;
			return false;
		}

		
		inline UCHAR GetCloser(UCHAR cData, UCHAR C1, UCHAR C2, UCHAR cTolerance)
		{
			///����Ƿ�����ֵ֮��
			UCHAR uMyDistance1 = MyDistance(cData, C1);
			UCHAR uMyDistance2 = MyDistance(cData, C2);
			UCHAR uReturn = cData;
			if(uMyDistance1 <= cTolerance)
				uReturn = C1;
			if(uMyDistance2 <= cTolerance && uMyDistance2 < uMyDistance1)
				uReturn = C2;
			return uReturn;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	ArrayCompressor::ArrayCompressor()
		: m_cDataArray(NULL)
		, m_pRowIndexArray(NULL)
		, m_nElementCount(0)
		, m_nWidth(0)
		, m_nHeigth(0)
		, m_ucErrData(0)
		, m_ucDefaultData(1)
		, m_cDiffDataArray(NULL)		
		, m_nDiffDataCount(0)
		, m_nDiffDataEnumer(0)
	{
	}

	ArrayCompressor::~ArrayCompressor()
	{
		SAFE_DELETE_ARRAY(m_cDataArray);
		SAFE_DELETE_ARRAY(m_pRowIndexArray);
		SAFE_DELETE_ARRAY(m_cDiffDataArray);
	}

#define KG3DAC_COMPRESS_PERROR(_Con, _Erro, out)	\
	{	\
		if(!(_Con))\
		{	\
			(out) += _T(";\n");	\
			(out) += _Erro;	\
			goto Exit0;		\
	}	\
	}NULL
	BOOL ArrayCompressor::Compress( const UCHAR* cInputArray, INT nHeight, INT nWidth, INT nTolerance /*= 0*/, INT nIsoCount /*= 0*/, INT nTotalValue /*= 0*/ )
	{
		//CHAR* pTempArray = NULL;
		Element* TempElementArray = NULL;
		INT nElementArrayWrited = 0;

		m_strError = _T("");

		KG3DAC_COMPRESS_PERROR (cInputArray 
			&& nWidth <= MAX_WIDTH 
			&& nHeight <= MAX_WIDTH 
			&& nWidth > 0 
			&& nHeight > 0
			&& nTolerance >= 0
			&& nTolerance <= UCHAR_MAX
			, _T("ѹ����������Χ"), m_strError);
		{	
			_KG3D_DEBUG_STATIC_CHECK_T(MAX_WIDTH < SHRT_MAX, OutOfRange);//������������Ų���Row
			///������Դ
			INT nResultBufferSize = nWidth * nHeight;
			INT nTempElementArraySize = nResultBufferSize;

			Clear();

			int RowIndexArrayCount = nHeight+1;///���һ���������ܴ�С��ͬʱҲ�����һ�еĽ�β�±�

			m_pRowIndexArray = new INT[RowIndexArrayCount];	
			KG_PROCESS_ERROR(m_pRowIndexArray);
			ZeroMemory(m_pRowIndexArray, sizeof(INT)*RowIndexArrayCount);

			m_cDataArray = new Element[nTempElementArraySize];
			KG_PROCESS_ERROR(m_cDataArray);
			m_nElementCount = nTempElementArraySize;

			///����ѹ��
			UCHAR cTolerance = _kg_to_smaller_size_cast<UCHAR>(nTolerance);
			m_nWidth = nWidth;
			m_nHeigth = nHeight;

			///��һ�����⴦��
			do
			{
				Element* pElement = NULL;
				int nOutter = 0;
				const UCHAR* cInputArrayRow = &cInputArray[nOutter * nWidth + 0];
				{
					int nInner = 0;

					KG3DAC_COMPRESS_PERROR(nElementArrayWrited <= MAX_ELEMENT, _T("��Ҫѹ����Ԫ��̫��"), m_strError);

					m_pRowIndexArray[nOutter] = /*_kg_to_smaller_size_cast<SHORT>*/(nElementArrayWrited);///����п�ʼ������
					///�����һ��Ԫ��
					KG3DAC_COMPRESS_PERROR(nElementArrayWrited < nTempElementArraySize, _T("ѹ����̫��"), m_strError);
					pElement = &m_cDataArray[nElementArrayWrited];
					pElement->m_ucData = cInputArrayRow[nInner];
					pElement->m_ucCount = 1;
					nElementArrayWrited++;
				}

				///������ظ�Ԫ�����Count����������
				for (int nInner = 1; nInner < nWidth; nInner++)
				{
					UCHAR cTemp = cInputArrayRow[nInner];
					if (IsInTolerance(cTemp, pElement->m_ucData, cTolerance) && pElement->m_ucCount < UCHAR_MAX)
					{
						pElement->m_ucCount++;
						if (pElement->m_ucCount <= 2)
							pElement->m_ucData = cTemp;///��������������أ����ϲ�
						continue;
					}

					KG3DAC_COMPRESS_PERROR(nElementArrayWrited < nTempElementArraySize, _T("ѹ����̫��"), m_strError);
					pElement = &m_cDataArray[nElementArrayWrited];
					pElement->m_ucData = cTemp;
					pElement->m_ucCount = 1;
					nElementArrayWrited++;	
				}
			}while(false);
			//////////////////////////////////////////////////////////////////////////
			for (int nOutter = 1; nOutter < nHeight; nOutter++)
			{
				const UCHAR* cInputArrayRow = &cInputArray[nOutter * nWidth + 0];
				
				KG3DAC_COMPRESS_PERROR(nElementArrayWrited <= MAX_ELEMENT, _T("��Ҫѹ����Ԫ��̫��"), m_strError);
				
				m_pRowIndexArray[nOutter] = /*_kg_to_smaller_size_cast<SHORT>*/(nElementArrayWrited);///����п�ʼ������

				///����һ�е�ͬһλ�ñȽ�һ��
				Element* pElement = NULL;
				{
					int nInner = 0;
					UCHAR cDataToWrite = cInputArrayRow[nInner];
					UCHAR cUpperData = GetData(nOutter - 1, nInner);
					if (IsInTolerance(cDataToWrite, cUpperData, cTolerance))
					{
						cDataToWrite = cUpperData;
					}

					///�����һ��Ԫ��
					KG3DAC_COMPRESS_PERROR(nElementArrayWrited < nTempElementArraySize, _T("ѹ����̫��"), m_strError);
					pElement = &m_cDataArray[nElementArrayWrited];
					pElement->m_ucData = cDataToWrite;
					pElement->m_ucCount = 1;
					nElementArrayWrited++;
				}

				///������ظ�Ԫ�����Count����������
				for (int nInner = 1; nInner < nWidth; nInner++)
				{
					UCHAR cTemp = cInputArrayRow[nInner];
					if (GetCloser(cTemp, pElement->m_ucData, GetData(nOutter -1, nInner), cTolerance)
						== pElement->m_ucData && pElement->m_ucCount < UCHAR_MAX )
					{
						pElement->m_ucCount++;
						if(pElement->m_ucCount <= 2)
							pElement->m_ucData = cTemp;
						continue;
					}

					KG3DAC_COMPRESS_PERROR(nElementArrayWrited < nTempElementArraySize, _T("ѹ����̫��"), m_strError);
					pElement = &m_cDataArray[nElementArrayWrited];
					pElement->m_ucData = cTemp;
					pElement->m_ucCount = 1;
					nElementArrayWrited++;	
				}
			}
			///�����Ͻ�β��Ϊ����
			
			KG3DAC_COMPRESS_PERROR(nElementArrayWrited <= MAX_ELEMENT, _T("��Ҫѹ����Ԫ��̫��"), m_strError);
			
			m_pRowIndexArray[RowIndexArrayCount-1] =/*_kg_to_smaller_size_cast<SHORT>*/(nElementArrayWrited); 

			///���Ʋ�ȥ�����õ���Data
			Element* TempElementArray = new Element[nElementArrayWrited];
			KG_PROCESS_ERROR(TempElementArray);
			int nRet = memcpy_s(TempElementArray, sizeof(Element)*nElementArrayWrited, m_cDataArray, sizeof(Element)*nElementArrayWrited);
			KG_PROCESS_ERROR(nRet == 0);
			SAFE_DELETE_ARRAY(m_cDataArray);
			m_cDataArray = TempElementArray;
			TempElementArray = NULL;
			m_nElementCount = nElementArrayWrited;

			ClearDiffArray();

			///ȥ����������ݣ������Ĭ������
			RemoveErrData();

			///������ݳ���
#if defined(_DEBUG) | defined(DEBUG)
			for(int i = 0; i < m_nHeigth; i++)
			{
				int nNewBegin = m_pRowIndexArray[i];
				int nNewCount =  m_pRowIndexArray[i+1] - nNewBegin;
				Element* pNewRow = &m_cDataArray[nNewBegin];
				int nDataInThisRow = 0;
				for (int x = 0; x < nNewCount; x++)
				{
					nDataInThisRow+=pNewRow[x].m_ucCount;
				}
				_ASSERTE(nDataInThisRow == m_nWidth);
			}
#endif

			if (nIsoCount > 0)
			{
				BOOL bRet = this->EliminateIsolatedData(nIsoCount);
				KG3DAC_COMPRESS_PERROR(bRet, _T("��ʳ��������ʧ��"), m_strError);
			}

			if (nTotalValue > 0)
			{
				BOOL bRet = this->EliminateIsolatedValue(nTotalValue);
				KG3DAC_COMPRESS_PERROR(bRet, _T("ɫ������ʧ��"), m_strError);
			}

#if defined(_DEBUG) | defined(DEBUG)
			if(0)
			{
				this->DebugPrintRawArray();///������Debugʱ����SetNextStatement���
			}
#endif
			return TRUE;
		}

Exit0:
		SAFE_DELETE_ARRAY(TempElementArray);
		Clear();
		_KG3D_DEBUG_SELF_REPORT("ʧ�ܣ�ArrayCompressor::Compress");
		return FALSE;
	}

	UCHAR ArrayCompressor::GetData( INT nRow, INT nColumn )
	{
		_KG3D_DEBUG_SELF_ASSERTE(m_cDataArray && m_pRowIndexArray && nRow < m_nHeigth && nColumn < m_nWidth);
		KG_PROCESS_ERROR(IsDataArrayGenerated());
		KG_PROCESS_ERROR(nRow >= 0 && nColumn >= 0 && nRow < m_nHeigth && nColumn < m_nWidth);
		int nRowIndex = GetRowBeginIndex(nRow);
		int nNextRowIndex = nRowIndex + GetRowCount(nRow);	
		Element* pElement = NULL;
		int nColumnIndicator = 0; 
		for (int i = nRowIndex; i < nNextRowIndex; i++)
		{
			pElement = m_cDataArray + i;
			/*
				if(pElement->m_ucData == GetErrorDate())
								continue;*/
			//_ASSERTE(pElement->m_ucData != GetErrorData());
			nColumnIndicator+=pElement->m_ucCount;
			if (nColumnIndicator > nColumn)
			{
				return pElement->m_ucData;
			}
		}

Exit0:
		return m_ucDefaultData;
	}
	BOOL ArrayCompressor::GetData(INT nRow, INT nColumn, INT& nRet)
	{
		//_KG3D_DEBUG_SELF_ASSERTE(m_cDataArray && m_sRowIndexArray && nRow < m_nHeigth && nColumn < m_nWidth);
		KG_PROCESS_ERROR(IsDataArrayGenerated());
		KG_PROCESS_ERROR(nRow < m_nHeigth && nColumn < m_nWidth);
		int nRowIndex = GetRowBeginIndex(nRow);
		int nNextRowIndex = nRowIndex + GetRowCount(nRow);	
		Element* pElement = NULL;
		int nColumnIndicator = 0; 
		for (int i = nRowIndex; i < nNextRowIndex; i++)
		{
			pElement = m_cDataArray + i;
			/*
				if(pElement->m_ucData == GetErrorDate())
								continue;*/
			//_ASSERTE(pElement->m_ucData != GetErrorData());
			nColumnIndicator+=pElement->m_ucCount;
			if (nColumnIndicator > nColumn)
			{
				nRet = pElement->m_ucData;
				return TRUE;
			}
		}
Exit0:
		return FALSE;
	}
	ArrayCompressor::Element* ArrayCompressor::GetRowElementWhenEliminating( INT nRow, INT nColumn )
	{
		//_KG3D_DEBUG_SELF_ASSERTE(m_cDataArray && m_sRowIndexArray && nRow < m_nHeigth && nColumn < m_nWidth);
		_KG3D_DEBUG_SELF_ASSERTE(IsDataArrayGenerated());
		_KG3D_DEBUG_SELF_ASSERTE(nRow < m_nHeigth && nColumn < m_nWidth);
		int nRowIndex = GetRowBeginIndex(nRow);
		int nNextRowIndex = nRowIndex + GetRowCount(nRow);	
		Element* pElement = NULL;
		int nColumnIndicator = 0; 
		for (int i = nRowIndex; i < nNextRowIndex; i++)
		{
			pElement = m_cDataArray + i;
			if(pElement->m_ucData == GetErrorData())
				continue;
			nColumnIndicator+=pElement->m_ucCount;
			if (nColumnIndicator > nColumn)
			{
				return pElement;
			}
		}
		return NULL;
	}

	INT ArrayCompressor::GetRowBeginIndex( INT nRow )
	{
		_ASSERTE(nRow < m_nHeigth && m_pRowIndexArray);
		return  m_pRowIndexArray[nRow];
	}

	INT ArrayCompressor::GetRowCount( INT nRow )
	{
		_ASSERTE(nRow < m_nHeigth && m_pRowIndexArray);
		return m_pRowIndexArray[nRow+1] - m_pRowIndexArray[nRow];
	}
	BOOL ArrayCompressor::PushDataInBuffer( UCHAR* pBuffer, INT nBufferSize )
	{
		KG_PROCESS_ERROR(pBuffer && nBufferSize >= m_nWidth * m_nHeigth);
		KG_PROCESS_ERROR(m_cDataArray&&m_pRowIndexArray);
		UCHAR* pRetPointer = pBuffer;
		for (int i = 0; i < m_nHeigth; i++)
		{
			for (int j=0; j < m_nWidth; j++, pRetPointer++)
			{
				*pRetPointer = GetData(i, j);
			}
		}
		return TRUE;
Exit0:
		return FALSE;
	}
	void ArrayCompressor::Clear()
	{
		SAFE_DELETE_ARRAY(m_cDataArray);
		SAFE_DELETE_ARRAY(m_pRowIndexArray);
		m_nWidth = 0;
		m_nHeigth = 0;
		m_nElementCount = 0;
		ClearDiffArray();
	}

#if defined(DEBUG) | defined(_DEBUG)
	VOID ArrayCompressor::DebugPrintByRow()
	{
		KG_PROCESS_ERROR(IsDataArrayGenerated());
		for (int i = 0; i < m_nHeigth; i++)
		{
			int nBegin =  GetRowBeginIndex(i);
			int nRowCount = GetRowCount(i);
			Element* pArrayRow = &m_cDataArray[nBegin];
			for (int j=0; j < nRowCount; j++)
			{
#if 0
				std::cout << (int)pArrayRow[j].m_ucData << "-" << (int)pArrayRow[j].m_ucCount << " ";
#else
				_KG3D_DEBUG_OUTPUT2("%d_%d ", (int)pArrayRow[j].m_ucData, (int)pArrayRow[j].m_ucCount);
#endif
			}
#if 0
			std::cout << "\n";
#else
			_KG3D_DEBUG_OUTPUT0("\n");
#endif
		}
Exit0:
		return;
	}
	VOID ArrayCompressor::DebugPrintRawCompressedData()
	{
		KG_PROCESS_ERROR(IsDataArrayGenerated());
		std::cout << "Raw Compressed Data:\n\n RowData:\n";
		for (int i = 0; i < m_nHeigth+1; i++)
		{
			std::cout << (int)m_pRowIndexArray[i] << " ";
		}
		std::cout << "\nData:\n";
		for (int i = 0; i < m_nElementCount; i++)
		{
			std::cout << (int)m_cDataArray[i].m_ucData << "-" << (int)m_cDataArray[i].m_ucCount << " ";
		}
		std::cout << "\n";
Exit0:
		return;
	}
	VOID ArrayCompressor::DebugPrintRawArray()
	{
		for (int i = 0; i < m_nHeigth; i++)
		{
			for (int j = 0; j < m_nWidth; j++)
			{
				_KG3D_DEBUG_OUTPUT1(" %d", this->GetData(i, j));
			}
			_KG3D_DEBUG_OUTPUT0("\n");
		}
	}
#endif
	BOOL ArrayCompressor::GenerateDiffDataArray()
	{
		KG_PROCESS_ERROR(IsDataArrayGenerated());

		ClearDiffArray();
		{
			typedef vector<DWORD> VecType;
			VecType TempVec;
			TempVec.reserve(MAX_DIFF_DATA);
			for (int i = 0; i < m_nElementCount; i++)
			{
				UCHAR uData = m_cDataArray[i].m_ucData;
				if (TempVec.end() == std::find(TempVec.begin(), TempVec.end(), (DWORD)uData))
				{
					TempVec.push_back((DWORD)uData);
				}
			}
			INT nVecSize = (INT)TempVec.size();
			if(nVecSize > MAX_DIFF_DATA)
			{
				_KG3D_DEBUG_OUTPUT1(_STRINGER(ArrayCompressor::GenerateDiffDataArray)_T(":Cannot Generate, To many different colors��%d��it may cause by a texture with two many gray scales\n"), nVecSize);
				goto Exit0;
			}

			m_cDiffDataArray = new DWORD[nVecSize];
			KG_PROCESS_ERROR(m_cDiffDataArray);

			for (VecType::iterator it = TempVec.begin(); it != TempVec.end(); it++)
			{
				m_cDiffDataArray[m_nDiffDataCount] =(*it);
				m_nDiffDataCount++;
			}
			_ASSERTE(m_nDiffDataCount == nVecSize);
			return TRUE;
		}

Exit0:
		SAFE_DELETE_ARRAY(m_cDiffDataArray);
		m_nDiffDataCount = 0;
		return FALSE;
	}

	BOOL ArrayCompressor::GetDiffDataArray(DWORD* pData, INT nBufferCount )
	{
		if (!IsDiffDataArrayGenerated())
		{
			BOOL bRet = GenerateDiffDataArray();
			KG_PROCESS_ERROR(bRet);
		}
		KG_PROCESS_ERROR(pData && m_cDiffDataArray && nBufferCount >= m_nDiffDataCount);
		{
			INT nRet = memcpy_s(pData, sizeof(DWORD)*nBufferCount, m_cDiffDataArray, sizeof(DWORD)*m_nDiffDataCount);
			KG_PROCESS_ERROR(nRet==0);
			return TRUE;
		}
Exit0:
		return FALSE;
	}
	BOOL ArrayCompressor::IsDiffDataArrayGenerated()
	{
		if (m_cDiffDataArray)
		{
			_ASSERTE(m_nDiffDataCount > 0);
			return TRUE;
		}
		return FALSE;
	}

	INT ArrayCompressor::GetDiffDataArrayCount()
	{
		if (!IsDiffDataArrayGenerated())
		{
			BOOL bRet = GenerateDiffDataArray();
			KG_PROCESS_ERROR(bRet);
		}
		return m_nDiffDataCount;
Exit0:
		return 0;
	}

	void ArrayCompressor::ClearDiffArray()
	{
		SAFE_DELETE_ARRAY(m_cDiffDataArray);
		m_nDiffDataCount = 0;
	}

	BOOL ArrayCompressor::SetErrorDate( UCHAR cErr )
	{
		if (IsDataArrayGenerated())
		{
			return FALSE;
		}
		m_ucErrData = cErr;
		return TRUE;
	}

	BOOL ArrayCompressor::IsDataArrayGenerated()
	{
		if(m_cDataArray)
		{
			_ASSERTE(m_nElementCount > 0 && m_pRowIndexArray);
			return TRUE;
		}
		return FALSE;
	}

	BOOL ArrayCompressor::SetDefaultData( UCHAR val )
	{
		if (IsDataArrayGenerated())
		{
			return FALSE;
		}
		m_ucDefaultData = val;
		return TRUE;
	}

	BOOL ArrayCompressor::RemoveErrData()
	{
		return ChangeData(GetErrorData(), m_ucDefaultData);
	}

	BOOL ArrayCompressor::ChangeData( UCHAR uDataToChange, UCHAR uNewData )
	{
		if (! IsDataArrayGenerated())
			return FALSE;
		for (int i = 0; i < m_nElementCount; i++)
		{
			if (m_cDataArray[i].m_ucData == uDataToChange)
			{
				m_cDataArray[i].m_ucData = uNewData;
			}
		}
		if (IsDataArrayGenerated())
		{
			for (int i = 0; i < m_nDiffDataCount; i++)
			{
				if (m_cDiffDataArray[i] == (DWORD)uDataToChange)
				{
					m_cDiffDataArray[i] = (DWORD)uNewData;
				}
			}
		}				
		return TRUE;
	}
	namespace Private
	{
		static UCHAR stErrData = 0;
		static UCHAR stIsoCount = 0;
		UCHAR GetIsoCount(){return stIsoCount;}
		void SetSTParam(UCHAR uErrData, UCHAR uIsoCount)
		{
			stErrData = uErrData; 
			stIsoCount = uIsoCount;
		}
		///ע��Merge��Զ�ǲ���ѵ�ǰElement��Merge������ȥ�ģ����Կ��Է��ĵĶ�ά����
		inline void MyMergeToNextElement(ArrayCompressor::Element* pFrom, UCHAR uErrData)
		{
			ArrayCompressor::Element* pTo = pFrom+1;
			if (pTo->m_ucData == uErrData)
				return;
			

			int uNewCount = pTo->m_ucCount + pFrom->m_ucCount;
			if(uNewCount <= UCHAR_MAX)
			{
				pFrom->m_ucData = uErrData;///���Ϊ����
				pTo->m_ucCount = _kg_to_smaller_size_cast<UCHAR>(uNewCount);
				pFrom->m_ucCount = 0;
			}
			else
			{
				pFrom->m_ucData = pTo->m_ucData;
			}

			
		}
		inline void MyMergeFromPrevElement(ArrayCompressor::Element* pTo, UCHAR uErrData)
		{
			ArrayCompressor::Element* pFrom = pTo - 1;
			if(pFrom->m_ucData == uErrData)
				return;
			pTo->m_ucData  = pFrom->m_ucData;
			MyMergeToNextElement(pFrom, uErrData);
		}

		//////////////////////////////////////////////////////////////////////////
		//���漸��Selector��Ϊ����ģ����ٷ����ϵ�ѡ��
		inline USHORT MyDistanceSelectorHelper(TNumToType<TRUE>, USHORT uCurData, USHORT uDataToCompare){return uDataToCompare != stErrData ? MyDistance(uCurData, uDataToCompare) : USHRT_MAX;}///����ǵ��ڴ���Data�Ļ����������޴�
		inline USHORT MyDistanceSelectorHelper(TNumToType<FALSE>, USHORT, USHORT){return USHRT_MAX;}
		template<bool bUse>
		inline USHORT MyDistanceSelector(USHORT uCurData, USHORT uDataToCompare)
		{
			return MyDistanceSelectorHelper(TNumToType<bUse?TRUE:FALSE>(), uCurData, uDataToCompare);
		}
		inline USHORT MyDirDataSelectorHelper(TNumToType<TRUE>, ArrayCompressor::Element* pElement, INT nBios){return (pElement+nBios)->m_ucData;}
		inline USHORT MyDirDataSelectorHelper(TNumToType<FALSE>, ArrayCompressor::Element*, INT ){return USHRT_MAX;}
		template<bool bUse>
		inline USHORT MyDirDataSelector(ArrayCompressor::Element* pElement, INT nBios)
		{
			return MyDirDataSelectorHelper(TNumToType<bUse?TRUE:FALSE>(), pElement, nBios);
		}
		//////////////////////////////////////////////////////////////////////////
		

		template<bool bUseUpper, bool bUseLower, bool bUseLeft, bool bUseRight>
		size_t MyFourDirCompare(ArrayCompressor::Element* pCurr, UCHAR uUpper, UCHAR uLower/*, UCHAR uErrData, UCHAR IsoCount*/)
		{
			enum
			{
				IsAnyDirBeingUsed = bUseUpper || bUseLower || bUseLeft || bUseRight,
			};
			_KG3D_DEBUG_STATIC_CHECK_T(IsAnyDirBeingUsed, CanNotCompareNothing);///����һ�������Ƚ�
			_ASSERTE(pCurr);
			USHORT uCurrData = pCurr->m_ucData;
			USHORT uDisArray[4] = {
				MyDistanceSelector<bUseUpper>(uCurrData, uUpper),
				MyDistanceSelector<bUseLower>(uCurrData, uLower),
				MyDistanceSelector<bUseLeft>(uCurrData, MyDirDataSelector<bUseLeft>(pCurr, -1)),
				MyDistanceSelector<bUseRight>(uCurrData, MyDirDataSelector<bUseRight>(pCurr, +1)),
			};
			//ǰ�����Ҿ��޷��ϲ������Ҳ���еģ���ʱ��ֻ��ǿ�������ҵ���С�ϲ���
			//_ASSERTE(uDisArray[0] < USHRT_MAX || uDisArray[1] < USHRT_MAX || uDisArray[2] < USHRT_MAX || uDisArray[3] < USHRT_MAX);///���Բ�����ȫ�������
			if(uDisArray[0] == USHRT_MAX && uDisArray[1] == USHRT_MAX && uDisArray[2] == USHRT_MAX && uDisArray[3] == USHRT_MAX)
			{
				if (bUseLeft)
				{
					return 2;
				}
				else
				{
					return 3;
				}
			}
			///�ȱȽ�����
			size_t uTarget = 0;
			if(uDisArray[1] < uDisArray[uTarget])
				uTarget = 1;
			if(uDisArray[uTarget] == USHRT_MAX)
				uTarget = 2;
			///���ұȽϵ�ʱ��Ҫ���ϸ����Ƚϣ��Է������������غϳ��˷Ƕ�������
			///������Ϊ���Һϲ����Լ�СElement�������������Һϲ������ȵģ����Լ���=�Ƚ�
			if(uDisArray[2] <= uDisArray[uTarget] && (pCurr-1)->m_ucCount > stIsoCount && (pCurr-1)->m_ucData != stErrData)
				uTarget = 2;
			if (uDisArray[3] <= uDisArray[uTarget] && (pCurr+1)->m_ucCount > stIsoCount && (pCurr+1)->m_ucData != stErrData)
				uTarget = 3;
			return uTarget;
		}
		

		template<bool bUseUpper, bool bUseLower, bool bUseLeft, bool bUseRight>
		void MergeData(ArrayCompressor::Element* pCurElement, UCHAR uUpper, UCHAR uLower/*, UCHAR uErrData, UCHAR uIsoCount*/)
		{
			_ASSERTE(pCurElement);
			size_t uDirRet = MyFourDirCompare<bUseUpper, bUseLower, bUseLeft, bUseRight>(pCurElement
				, uUpper
				, uLower
				/*, uIsoCount*/);				
			switch(uDirRet)
			{
			case ArrayCompressor::DirUpper:
				{
					_ASSERTE(bUseUpper && uUpper != stErrData);
					pCurElement->m_ucData = uUpper;
				}
				break;
			case ArrayCompressor::DirLower:
				{
					_ASSERTE(bUseLower && uLower != stErrData);
					pCurElement->m_ucData = uLower;
				}
				break;
			case ArrayCompressor::DirLeft:
				{
					if(bUseLeft)
						MyMergeFromPrevElement(pCurElement, stErrData);
				}
			    break;
			case ArrayCompressor::DirRight:
				{
					if(bUseRight)
						MyMergeToNextElement(pCurElement, stErrData);
				}
			    break;
			default:
				_ASSERTE(NULL);
			    break;
			}
		}
	};
	///�����ʳ�������ݣ���Ϊ�򵥵��˲�
	BOOL ArrayCompressor::EliminateIsolatedData( INT nIsoCount )
	{
		INT nNewElementCount  = 0;
		Element* pNewElementArray = NULL;
		INT*	pNewRowIndexArray = NULL;
		UCHAR uUpperTemp = 0, uLowerTemp = 0;
		UCHAR uErrData =  this->GetErrorData();

		KG_PROCESS_ERROR(nIsoCount > 0 && nIsoCount <= 1);
		KG_PROCESS_ERROR(m_nWidth >= 2 && m_nHeigth >= 2);
		KG_PROCESS_ERROR(IsDataArrayGenerated());

#if defined(_DEBUG) | defined(DEBUG)
		for (int i = 0; i < m_nElementCount; i++)
		{
			_ASSERTE(m_cDataArray[i].m_ucData != uErrData);
		}
#endif

		///��ʳ��������
		

		///���þ�̬�����������ݣ�ʡ��ÿ�β���ѹջ
		Private::SetSTParam(uErrData,  _kg_to_smaller_size_cast<UCHAR>(nIsoCount));
		
		do///���һ������ǰ��һ�����⴦��
		{
			int i = 0;
			int nThisRowColumn = 0;///��¼���ڲ����ľ�������һ�е�����
			int nLastAddedRowColumnCount = 0;
			int nBegin = this->GetRowBeginIndex(i);
			int nRowCount = this->GetRowCount(i);			
			Element* pRow = &m_cDataArray[nBegin];

			if(nRowCount <= 2)///һ��ֻ����������ô�����������ʳ
				continue;

			{
				int x = 0;
				if (pRow[x].m_ucCount <= nIsoCount
					&& IsIso == IsIsoVertical<UseLower>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<false, true, false, true>(&pRow[x], uUpperTemp, uLowerTemp);
				nLastAddedRowColumnCount=pRow[x].m_ucCount;
				nThisRowColumn+=nLastAddedRowColumnCount;
			}
			for (int x = 1; x < nRowCount - 1; x++)
			{
				if (pRow[x].m_ucCount <= nIsoCount
					&& IsIso == IsIsoVertical<UseLower>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<false, true, true, true>(&pRow[x], uUpperTemp, uLowerTemp);
				nThisRowColumn+=(pRow[x].m_ucCount - nLastAddedRowColumnCount + pRow[x-1].m_ucCount);///������ʹ�ϲ���Ҳ����ȷ��
				nLastAddedRowColumnCount = pRow[x].m_ucCount;
			}
			{
				int x = nRowCount - 1;
				if (pRow[x].m_ucCount <= nIsoCount
					&& IsIso == IsIsoVertical<UseLower>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<false, true, true, false>(&pRow[x], uUpperTemp, uLowerTemp);
				///���Column��Ч���Ͳ��ü�Column��
			}
		}while(false);
		for (int i = 1; i < m_nHeigth -1; i++)
		{
			int nThisRowColumn = 0;
			int nLastAddedRowColumnCount = 0;
			int nBegin = this->GetRowBeginIndex(i);
			int nRowCount = this->GetRowCount(i);			
			Element* pRow = &m_cDataArray[nBegin];

			if(nRowCount <= 2)///һ��ֻ����������ô�����������ʳ
				continue;

			{
				int x = 0;
				if (pRow[x].m_ucCount <= nIsoCount
					&&IsIso == IsIsoVertical<UseBothUpperLower>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<true, true, false, true>(&pRow[x], uUpperTemp, uLowerTemp);
				nLastAddedRowColumnCount=pRow[x].m_ucCount;
				nThisRowColumn+=nLastAddedRowColumnCount;
			}
			for (int x = 1; x < nRowCount - 1; x++)
			{
				if (pRow[x].m_ucCount <= nIsoCount
					&&IsIso == IsIsoVertical<UseBothUpperLower>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<true, true, true, true>(&pRow[x], uUpperTemp, uLowerTemp);
				nThisRowColumn+=(pRow[x].m_ucCount - nLastAddedRowColumnCount + pRow[x-1].m_ucCount);///������ʹ�ϲ���Ҳ����ȷ��
				nLastAddedRowColumnCount = pRow[x].m_ucCount;
			}
			{
				int x = nRowCount - 1;
				if (pRow[x].m_ucCount <= nIsoCount
					&&IsIso == IsIsoVertical<UseBothUpperLower>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<true, true, true, false>(&pRow[x], uUpperTemp, uLowerTemp);	
			}
		}
		do
		{
			int i = m_nHeigth - 1;
			int nThisRowColumn = 0;
			int nLastAddedRowColumnCount = 0;
			int nBegin = this->GetRowBeginIndex(i);
			int nRowCount = this->GetRowCount(i);			
			Element* pRow = &m_cDataArray[nBegin];

			if(nRowCount <= 2)///һ��ֻ����������ô�����������ʳ
				continue;

			{
				int x = 0;
				if (pRow[x].m_ucCount <= nIsoCount
					&&IsIso == IsIsoVertical<UseUpper>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<true, false, false, true>(&pRow[x], uUpperTemp, uLowerTemp);
				nLastAddedRowColumnCount=pRow[x].m_ucCount;
				nThisRowColumn+=nLastAddedRowColumnCount;
			}
			for (int x = 1; x < nRowCount - 1; x++)
			{
				if (pRow[x].m_ucCount <= nIsoCount
					&&IsIso == IsIsoVertical<UseUpper>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<true, false, true, true>(&pRow[x], uUpperTemp, uLowerTemp);
				nThisRowColumn+=(pRow[x].m_ucCount - nLastAddedRowColumnCount + pRow[x-1].m_ucCount);///������ʹ�ϲ���Ҳ����ȷ��
				nLastAddedRowColumnCount = pRow[x].m_ucCount;
			}
			{
				int x = nRowCount - 1;
				if (pRow[x].m_ucCount <= nIsoCount
					&&IsIso == IsIsoVertical<UseUpper>(i, nThisRowColumn, pRow[x].m_ucData, uUpperTemp, uLowerTemp))
					Private::MergeData<true, false, true, false>(&pRow[x], uUpperTemp, uLowerTemp);	
			}
		}while(false);
		///���¼���nNewElement
		nNewElementCount = m_nElementCount;
		for (int i = 0; i < m_nElementCount; i++)
		{
			if(m_cDataArray[i].m_ucData == uErrData)
				--nNewElementCount;
		}
		_ASSERTE(nNewElementCount > 0);
		///�����ж��������ˣ�������������
		pNewElementArray = new Element[nNewElementCount];
		_ASSERTE(m_nHeigth > 0);
		pNewRowIndexArray =  new INT[m_nHeigth+1];
		if (!pNewElementArray || !pNewElementArray)///���ʱ��ɵ�Array�Ѿ����ƻ����ˣ�ֻ��ȫ��ɾ����������
		{
			Clear();
			goto Exit0;
		}
		int nElementWrited  = 0;
		for (int i = 0; i < m_nHeigth; i++)
		{
			_ASSERTE(nElementWrited <= MAX_ELEMENT);
			pNewRowIndexArray[i] = /*_kg_to_smaller_size_cast<SHORT>*/(nElementWrited);
			int nBegin = this->GetRowBeginIndex(i);
			int nCount = this->GetRowCount(i);

			Element* pRow = &m_cDataArray[nBegin];
			for (int x = 0; x < nCount; x++)
			{
				if (pRow[x].m_ucData == uErrData)
					continue;
				pNewElementArray[nElementWrited] = pRow[x];
				nElementWrited++;
			}
			///������ݳ���
#if defined(_DEBUG) | defined(DEBUG)
			{
				int nNewBegin = pNewRowIndexArray[i];
				int nNewCount =  nElementWrited - nNewBegin;
				Element* pNewRow = &pNewElementArray[nNewBegin];
				int nDataInThisRow = 0;
				for (int x = 0; x < nNewCount; x++)
				{
					nDataInThisRow+=pNewRow[x].m_ucCount;
				}
				_ASSERTE(nDataInThisRow == m_nWidth);
			}
#endif
		}
		_ASSERTE(nElementWrited == nNewElementCount);
		///���Ͻ�β��Row
		_ASSERTE(nElementWrited < MAX_ELEMENT);
		pNewRowIndexArray[m_nHeigth] = /*_kg_to_smaller_size_cast<SHORT>*/(nElementWrited);

		///�ͷžɵ�
		SAFE_DELETE_ARRAY(m_cDataArray);
		m_cDataArray = pNewElementArray;
		m_nElementCount = nNewElementCount;
		pNewElementArray = NULL;
		SAFE_DELETE_ARRAY(m_pRowIndexArray);
		m_pRowIndexArray = pNewRowIndexArray;
		pNewRowIndexArray = NULL;

		if (IsDiffDataArrayGenerated())
		{
			ClearDiffArray();///��ԭ����DiffArrayɾ����
		}
		
		_ASSERTE(!pNewElementArray && !pNewRowIndexArray && m_nElementCount > 0);

		return TRUE;
Exit0:
		SAFE_DELETE_ARRAY(pNewElementArray);
		SAFE_DELETE_ARRAY(pNewRowIndexArray);
		return FALSE;
	}

	INT ArrayCompressor::IsIsoVerticalHelper(TNumToType<UseUpper>, INT nRow, INT nColumn, UCHAR uCurrData, UCHAR& uUpperData, UCHAR& uLowerData)
	{
		_ASSERTE(IsDataArrayGenerated() && nRow > 0);
		Element* pElement = GetRowElementWhenEliminating(nRow-1, nColumn);
		KG_PROCESS_ERROR(pElement);
		if (uCurrData == pElement->m_ucData && pElement->m_ucData > Private::GetIsoCount())	 
			return IsNotIso;
		uUpperData = pElement->m_ucData > Private::GetIsoCount() ? pElement->m_ucData : GetErrorData();
		uLowerData = GetErrorData();
		return IsIso;
Exit0:
		uUpperData = GetErrorData();
		uLowerData = GetErrorData();
		return IsIso;
	}

	INT ArrayCompressor::IsIsoVerticalHelper(TNumToType<UseLower>, INT nRow, INT nColumn, UCHAR uCurrData, UCHAR& uUpperData, UCHAR& uLowerData )
	{
		_ASSERTE(IsDataArrayGenerated() && nRow < m_nHeigth - 1);
		Element* pElement = GetRowElementWhenEliminating(nRow+1, nColumn);
		KG_PROCESS_ERROR(pElement);
		if (uCurrData == pElement->m_ucData && pElement->m_ucData > Private::GetIsoCount())	 
			return IsNotIso;
		uUpperData = GetErrorData();
		uLowerData = pElement->m_ucData > Private::GetIsoCount() ? pElement->m_ucData : GetErrorData();
		return IsIso;
Exit0:
		uUpperData = GetErrorData();
		uLowerData = GetErrorData();
		return IsIso;
	}

	INT ArrayCompressor::IsIsoVerticalHelper( TNumToType<UseBothUpperLower>, INT nRow, INT nColumn, UCHAR uCurrData, UCHAR& uUpperData, UCHAR& uLowerData )
	{
		_ASSERTE(IsDataArrayGenerated() && nRow > 0 && nRow < m_nHeigth - 1);
		Element* pUpper = GetRowElementWhenEliminating(nRow - 1, nColumn);
		Element* pLower = GetRowElementWhenEliminating(nRow + 1, nColumn);
		KG_PROCESS_ERROR(pUpper && pLower);
		if ((pUpper->m_ucData == uCurrData && pUpper->m_ucCount > Private::GetIsoCount())
			|| (pLower->m_ucData == uCurrData && pLower->m_ucCount > Private::GetIsoCount()))
			return IsNotIso;
		uUpperData = pUpper->m_ucCount > Private::GetIsoCount() ? pUpper->m_ucData : GetErrorData();
		uLowerData = pUpper->m_ucCount > Private::GetIsoCount() ? pLower->m_ucData : GetErrorData();
		return IsIso;
Exit0:
		uUpperData = GetErrorData();
		uLowerData = GetErrorData();
		return IsIso;
	}

	BOOL ArrayCompressor::EliminateIsolatedValue( INT nTotalValue )
	{
		KG_PROCESS_ERROR(IsDataArrayGenerated() && nTotalValue > 0 && nTotalValue <= m_nElementCount);
		{
			typedef std::map<UCHAR, INT> ValueMap;
			ValueMap mapValue;
			{
				ValueMap::iterator it = mapValue.end();
				//ͳ����ɫ
				for (int i = 0; i < m_nElementCount; i++)
				{
					UCHAR uValue = m_cDataArray[i].m_ucData;
					it = mapValue.find(uValue);
					if(it == mapValue.end())
						mapValue[uValue] = m_cDataArray[i].m_ucCount;
					else
						(*it).second +=m_cDataArray[i].m_ucCount;
				}
			}
			KG_PROCESS_SUCCESS (mapValue.size() <=(size_t)nTotalValue);
			{
				typedef std::multimap<INT, UCHAR> SortMap;
				SortMap TempSortMap;
				for (ValueMap::const_iterator it = mapValue.begin(); it != mapValue.end(); it++)
				{
					TempSortMap.insert(SortMap::value_type(it->second, it->first));///���Զ�����
				}
				_ASSERTE(TempSortMap.size() == mapValue.size());
				int nValueNeedDelCount = (int)TempSortMap.size() - nTotalValue;
				_ASSERTE(nValueNeedDelCount > 0);
				for(SortMap::const_iterator itSet = TempSortMap.begin(); nValueNeedDelCount > 0 &&itSet != TempSortMap.end(); itSet++)
				{
					mapValue.erase(itSet->second);
					nValueNeedDelCount--;
				}
				_ASSERTE(mapValue.size() == (size_t)nTotalValue);
			}

			{
				ValueMap::iterator itGreater = mapValue.end();
				ValueMap::iterator itSmaller = mapValue.end();
				_ASSERTE(mapValue.size() > 0);
				for (int i = 0; i < m_nElementCount; i++)
				{
					Element* pElement = m_cDataArray+i;
					itGreater = mapValue.lower_bound(pElement->m_ucData);
					if(itGreater == mapValue.end())
					{
						if (itGreater != mapValue.begin())
						{
							itSmaller = --itGreater;
							pElement->m_ucData = itSmaller->first;
						}
						else
						{
							pElement->m_ucData = GetDefaultData();
						}
						continue;
					}
					if(itGreater->first == pElement->m_ucData)
						continue;
					///������ǵĻ����϶���pElement��
					if (itGreater != mapValue.begin())
					{
						itSmaller = itGreater;
						--itSmaller;
						pElement->m_ucData = GetCloser(pElement->m_ucData, itSmaller->first, itGreater->first, UCHAR_MAX);
					}
					else
					{
						pElement->m_ucData = itGreater->first;
					}

				}
			}
Exit1:
			return TRUE;
		}
Exit0:
		return FALSE;
	}
	
};
////////////////////////////////////////////////////////////////////////////////

