#include "StdAfx.h"
#include "KG3DRegionInfoManager.h"
#include "KGArrayCompressor.h"
#include "KG3DTextureHelpers.h"
#include "KG3DTextureTable.h"
#include "MathTool.h"
#include "KG3DGraphicsTool.h"
#include "KG3DTemplates.h"
#include "MathTool.h"
#include "IEKG3DTerrain.h"
#include "KG3DScene.h"
#include "KG3DSceneOutDoorSpaceMgr.h"
#include "KGINIFileHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KG3DRegionInfoManagerTable g_cRegionInfoManagerTable;

KG3DRegionInfoManager::KG3DRegionInfoManager(void)
: m_pTexture(NULL)
,m_bHasSuccessfullyLoaded(FALSE)
,m_ulRefCount(0)
,m_dwID(0)
,m_pArrayCompressor(NULL)
,m_pstTexPath(NULL)
,m_uHowManyPixelsInOneRegion(1)
,m_nRegionXCount(0)
,m_nRegionZCount(0)
{
	m_pArrayCompressor = new KG_CUSTOM_HELPERS::ArrayCompressor;
	if (m_pArrayCompressor)
	{
		m_pArrayCompressor->SetErrorDate(0);
		m_pArrayCompressor->SetDefaultData(1);
	}	
}

KG3DRegionInfoManager::~KG3DRegionInfoManager(void)
{
	SAFE_DELETE(m_pArrayCompressor);
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETE(m_pstTexPath);
	UnInit();
}
INT KG3DRegionInfoManager::GetHeight()
{
	_ASSERTE(m_pArrayCompressor);
	return m_pArrayCompressor->GetHeight();
}
INT KG3DRegionInfoManager::GetWidth()
{
	_ASSERTE(m_pArrayCompressor);
	return m_pArrayCompressor->GetWidth();
}
BOOL KG3DRegionInfoManager::GetData(INT nX, INT nZ, INT& nRet)
{
	if (!m_pArrayCompressor || ! m_bHasSuccessfullyLoaded)
	{
		return FALSE;
	}
	INT nWidth = m_pArrayCompressor->GetWidth();
	INT nHeight = m_pArrayCompressor->GetHeight();
	INT nRow =  nHeight - nZ - 1;
	INT nColumn = nX;
	
	MT_LIMIT_TO_RANGE(nRow, nHeight-1, 0);
	MT_LIMIT_TO_RANGE(nColumn, nWidth-1, 0);
	return m_pArrayCompressor->GetData(nRow, nColumn, nRet);
}

#define KG3DRIM_LOAD_IMAGE_PERROR(_Con, _Erro, out)	\
{	\
	if(!(_Con))\
	{	\
		(out) += _T(";\n");	\
		(out) += _Erro;	\
		goto Exit0;		\
	}	\
}NULL
#define KG3DRIM_LOAD_IMAGE_COM_PERROR(_hr, _Erro, out)	KG3DRIM_LOAD_IMAGE_PERROR(SUCCEEDED((_hr)), _Erro, out)

HRESULT KG3DRegionInfoManager::LoadImage(const TCHAR cszFileName[], DWORD_PTR uFileNameHash, DWORD_PTR uOption)
{
	HRESULT hr = E_FAIL;

	INT nRegionXCount = 0;
	INT nRegionZCount = 0;
	KG3DREGION_INFORMER_LOADFILE_PARAM* pParam = NULL;	
	
	//�������
	{
		
		///�������
		KG3DREGION_INFORMER_LOADFILE_PARAM	DefaulParam = GetDefaultLoadFileStruct();
		pParam = &DefaulParam;
		if (uOption != 0)
		{
			pParam = (KG3DREGION_INFORMER_LOADFILE_PARAM*)(uOption);

			//��Param�õ�������ֵ
			nRegionXCount = pParam->nRegionXCount;
			nRegionZCount = pParam->nRegionZCount;
		}
		_ASSERTE(pParam);
		pParam->strError = _T("");
	}

	//������ͼ
	KG3DTexture* pTempTexture = NULL;
	unsigned int uTexHeight = 0, uTexWidth = 0;

	if(NULL != m_pTexture)//��Load֮ǰҪ�Ȱ�ԭ������ͼɾ������ȻResourceTable�����м�¼������ˢ��Texture
	{
		m_pTexture->ReleaseWithoutDelay();
		m_pTexture = NULL;
	}

	hr = g_cTextureTable.LoadResourceFromFile(cszFileName, 0, TEXOP_DISABLEDTX  | TEXOP_DISABLEPOW2 | TEXOP_DISABLEMIPMAP| TEXOP_COMMON, reinterpret_cast<IKG3DResourceBase**>(&pTempTexture));
	KG3DRIM_LOAD_IMAGE_COM_PERROR(hr, _T("������ͼʧ��"), pParam->strError);

	hr = pTempTexture->GetSize(&uTexWidth, &uTexHeight);
	KG3DRIM_LOAD_IMAGE_COM_PERROR(hr, _T("������ͼʧ��"), pParam->strError);


	KG_PROCESS_ERROR(uTexWidth <= TEX_MAX_SIZE && uTexHeight <= TEX_MAX_SIZE);
	{
		KG3DRIM_LOAD_IMAGE_PERROR(pParam->uHowManyPixelsInOneRegion > 0	&& ! (pParam->uHowManyPixelsInOneRegion > SCENEREGIONWIDTH/100.0f)
			,_T("����ÿRegion��ֵ�򲻶�"), pParam->strError);
		
		//����ⲿû���ܹ��õ�������ֵ����ô�ӵ�ǰScene��Region��ȡ
		if (nRegionXCount == 0 || nRegionZCount == 0)
		{
            INT XS, ZS, XWidth, ZWidth;

			KG3DScene* pScene = g_cGraphicsTool.GetCurScene();
			KG_PROCESS_ERROR(NULL != pScene);

			KG3DSceneOutDoorSpaceMgr* pOutDoor = pScene->GetOutDoorSpaceMgr();
			KG_PROCESS_ERROR(NULL != pOutDoor);
			hr = pOutDoor->GetSceneRect(XS, ZS, XWidth, ZWidth);//��λ����
			KG_COM_PROCESS_ERROR(hr);
			nRegionXCount = XWidth * 100 / SCENEREGIONWIDTH;
			nRegionZCount = ZWidth * 100 / SCENEREGIONWIDTH;
		}
		KG3DRIM_LOAD_IMAGE_PERROR(nRegionXCount > 0 && nRegionZCount >0
			&& nRegionXCount * (INT)pParam->uHowManyPixelsInOneRegion <= TEX_MAX_SIZE 
			&& nRegionZCount * (INT)pParam->uHowManyPixelsInOneRegion <= TEX_MAX_SIZE
			, _T("��ͼ���ߵ���̫���ˣ�û������"), pParam->strError);

		///�����ӻҶȴ����ѹ��
		BOOL bRet = AnalizeTexture(cszFileName
			,pTempTexture
			, nRegionXCount * (INT)pParam->uHowManyPixelsInOneRegion
			, nRegionZCount * (INT)pParam->uHowManyPixelsInOneRegion
			, pParam);
		KG_PROCESS_ERROR(bRet);
		m_uHowManyPixelsInOneRegion = pParam->uHowManyPixelsInOneRegion;
		
		m_nRegionXCount = nRegionXCount;
		m_nRegionZCount = nRegionZCount;
	}
	if(pTempTexture)
	{
		pTempTexture->ReleaseWithoutDelay();
		pTempTexture = NULL;
	}
	m_scName = cszFileName;
	return S_OK;
Exit0:
	if(pTempTexture)
	{
		pTempTexture->ReleaseWithoutDelay();
		pTempTexture = NULL;
	}
	KGLogPrintf(KGLOG_ERR, _T("ʧ�ܣ�KG3DRegionInfoManager::LoadFromFile"));
	return E_FAIL;
}

BOOL KG3DRegionInfoManager::AnalizeTexture(const TCHAR cszFileName[], KG3DTexture* pTex, INT nTargetWidth, INT nTargetHeight, KG3DREGION_INFORMER_LOADFILE_PARAM* pParam)
{
	///ȷ�ϲ���
	DWORD* pSampledPixelArray = NULL;
	UCHAR* pUCHERBuffer = NULL;
	HRESULT hr = E_FAIL;
	INT nTargetOutputSize = nTargetWidth * nTargetHeight;
	_ASSERTE(pParam);
	KG3DRIM_LOAD_IMAGE_PERROR(m_pArrayCompressor != NULL, _T("ѹ��������ʧ��"), pParam->strError);
	KG_PROCESS_ERROR(pTex && nTargetWidth > 0 && nTargetHeight > 0 && nTargetWidth <= TEX_MAX_SIZE && nTargetHeight <= TEX_MAX_SIZE );

	{		
		pSampledPixelArray = new DWORD[nTargetOutputSize];
		KG_PROCESS_ERROR(pSampledPixelArray);
		BOOL bRet = FALSE;

		///�ò���������
		{
			unsigned int uTexWidth = 0, uTexHeight = 0;
			hr = pTex->GetSize(&uTexWidth, &uTexHeight);
			KG3DRIM_LOAD_IMAGE_COM_PERROR(hr, _T("�õ���ͼ��С����"), pParam->strError);
			if ((INT)uTexWidth == nTargetWidth && (INT)uTexHeight == nTargetHeight)
			{
				///��ͬ��С
				hr = KG3D_CUSTOM_HELPERS::TextureTexToDWORDBuffer(pTex, pSampledPixelArray, nTargetOutputSize);
				KG3DRIM_LOAD_IMAGE_COM_PERROR(hr, _T("��ͼ��ѹʧ��"), pParam->strError);
				//pParam->nEliminateIsoDataCount = 0;
				//pParam->nTotalValue = 0;
			}
			else
			{
				bRet = KG3D_CUSTOM_HELPERS::TextureSampler(pTex
					, pSampledPixelArray
					, nTargetOutputSize
					, pParam->nSampleTimes
					, nTargetWidth
					, nTargetHeight);
				KG3DRIM_LOAD_IMAGE_PERROR(bRet, _T("��ͼ����ʧ��"), pParam->strError);
			}
		}

		///�Ҷȴ���
		pUCHERBuffer = new UCHAR[nTargetOutputSize];
		KG_PROCESS_ERROR(pUCHERBuffer);
		bRet = KG3D_CUSTOM_HELPERS::TextureToGrayArray(pSampledPixelArray, nTargetOutputSize, pUCHERBuffer, nTargetOutputSize);
		KG3DRIM_LOAD_IMAGE_PERROR(bRet, _T("�ҶȲ���ʧ��"), pParam->strError);
		
		///����ѹ������ѹ��
		bRet = m_pArrayCompressor->Compress(pUCHERBuffer
			, nTargetHeight
			, nTargetWidth
			, pParam->nPixelTolerance
			, pParam->nEliminateIsoDataCount
			, pParam->nTotalValue);
		if(! bRet)
		{
			pParam->strError += m_pArrayCompressor->GetErrorString();
			goto Exit0;
		}
		///�����ͷŵ���Щ�ڴ棬�����GenerateTexҪ��
		SAFE_DELETE_ARRAY(pUCHERBuffer);
		SAFE_DELETE_ARRAY(pSampledPixelArray);	

		m_bHasSuccessfullyLoaded = TRUE;
		_ASSERTE(m_pArrayCompressor->IsDataArrayGenerated());
		if (pParam->m_bNeedPreviewTex)
		{
			///��ѹ�������ͼ�����Ҷ�
			BOOL bRet = GenerateTex(cszFileName);
			if (bRet && m_pTexture)
			{
				pParam->dwRetTextureID = m_pTexture->GetID();
			}
			else
			{
				pParam->dwRetTextureID = (DWORD)(KG3DREGION_INFORMER_LOADFILE_PARAM::INVALID_MAX_TEXTURE_ID);		
			}	
		}				
	}
	SAFE_DELETE_ARRAY(pSampledPixelArray);
	SAFE_DELETE_ARRAY(pUCHERBuffer);
	return TRUE;
Exit0:
	m_bHasSuccessfullyLoaded = FALSE;
	_KG3D_DEBUG_REPORT("ʧ�ܣ�KG3DRegionInfoManager::AnalizeTexture");
	SAFE_DELETE_ARRAY(pSampledPixelArray);
	SAFE_DELETE_ARRAY(pUCHERBuffer);
	return FALSE;
}
BOOL KG3DRegionInfoManager::GenerateTex(LPCTSTR cszFileName /*NULL*/)///���Դ�NULL����NULL��ʹ���Լ���Path
{
	UCHAR* pUCHERBuffer = NULL;
	HRESULT hr = E_FAIL;
	KG_PROCESS_ERROR(m_bHasSuccessfullyLoaded && m_pArrayCompressor && m_pArrayCompressor->IsDataArrayGenerated());
	{
		INT nTargetWidth = m_pArrayCompressor->GetWidth();
		_ASSERTE(nTargetWidth >= 0 && nTargetWidth <= KG_CUSTOM_HELPERS::ArrayCompressor::MAX_WIDTH);
		INT nTargetHeight = m_pArrayCompressor->GetHeight();
		_ASSERTE(nTargetHeight >= 0 && nTargetHeight <= KG_CUSTOM_HELPERS::ArrayCompressor::MAX_WIDTH);
		INT nTargetOutputSize = nTargetWidth*nTargetHeight;
		
		_KG3D_DEBUG_STATIC_CHECK_T(sizeof(UCHAR) == sizeof(char), SizeToBig);
		pUCHERBuffer = new UCHAR[nTargetOutputSize];
		KG_PROCESS_ERROR(pUCHERBuffer);
		
		bool bNeedCreateTex = true;
		if (m_pTexture)
		{
			unsigned uWidth = 0, uHeight = 0;
			hr = m_pTexture->GetSize(&uWidth, &uHeight);
			if (SUCCEEDED(hr) && (INT)uWidth == nTargetWidth&&(INT)uHeight == nTargetHeight)
			{
				bNeedCreateTex = false;
			}
		}
		if(bNeedCreateTex)
		{
			if (!m_pstTexPath)
			{
				static int nstPass = 0;
				TCHAR csPath[MAX_PATH];
				_stprintf_s(csPath
					, _T("%s%d.bmp")
					, _T("D:\\Kingsoft\\Game\\DataStores\\Sword3-Products\\trunk\\client\\data\\Temp\\DWSplitMap")
					, nstPass);//���·������Ҫ���ڣ���Ϊ��������������ͼ��ϣֵ�õ�
				m_pstTexPath = new std::tstring(csPath);
				nstPass++;
			}
			KG_PROCESS_ERROR(m_pstTexPath);
			if (cszFileName && 0 != strcmp(cszFileName, ""))
			{
				*m_pstTexPath = cszFileName;
			}
			
			///����ʹ�õ���Get1NewResource������汾��������һ��FileName��ǿ�Ƽ��뵽HashMap�У��Ӷ��õ�ID��
			if (m_pTexture)
			{
				m_pTexture->ReleaseWithoutDelay();
				m_pTexture = NULL;
			}

			hr = g_cTextureTable.CreateTexture(nTargetWidth, nTargetHeight, D3DFMT_A8R8G8B8
				, TEXTYPE_COMMON, &m_pTexture, m_pstTexPath->c_str());
			KG_COM_PROCESS_ERROR(hr);

#if defined(DEBUG) | defined(_DEBUG)
			{
				KG3DTexture* pTexVerify = NULL;
				HRESULT hr2 = g_cTextureTable.GetResource(&pTexVerify, m_pTexture->GetID());
				_ASSERTE(SUCCEEDED(hr2) && pTexVerify);
				SAFE_RELEASE(pTexVerify);
			}
#endif
		}
		_ASSERTE(m_pTexture);		

		BOOL bRet = m_pArrayCompressor->PushDataInBuffer(pUCHERBuffer, nTargetOutputSize);
		KG_PROCESS_ERROR(bRet);
		

#if defined(_DEBUG) | defined(DEBUG)
		_ASSERTE(m_pTexture);
		_ASSERTE(m_pTexture->GetFormat() == D3DFMT_A8R8G8B8);
		{
			unsigned uW = 0, uH = 0;
			m_pTexture->GetSize(&uW, &uH);
			_ASSERTE(uW*uH == (unsigned)nTargetOutputSize);
		}
#endif

		LPDIRECT3DTEXTURE9 lpTexTemp = m_pTexture->GetTexture();
		_ASSERTE(lpTexTemp);
		D3DLOCKED_RECT TempLockRect;
		KG_COM_PROCESS_ERROR(lpTexTemp->LockRect(0, &TempLockRect, NULL, 0));
		{
			DWORD* pBits = (DWORD*)TempLockRect.pBits;
			KG3D_CUSTOM_HELPERS::TextureGrayBufferToDWORDBuffer(pUCHERBuffer, nTargetOutputSize, pBits, nTargetOutputSize);
			lpTexTemp->UnlockRect(0);
		}
		SAFE_DELETE_ARRAY(pUCHERBuffer);
		return TRUE;
	}
Exit0:
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETE_ARRAY(pUCHERBuffer);
	return FALSE;
}
HRESULT KG3DRegionInfoManager::GenerateRegionTex(DWORD_PTR dwExtraParam, DWORD* pdwTexID)
{
	//KG_PROCESS_ERROR(cszFileName);
	if(m_pTexture)
		goto Exit1;
	KG_PROCESS_ERROR(this->GenerateTex());
	_ASSERTE(m_pTexture);
Exit1:
	if (pdwTexID)
	{
		_ASSERTE(m_pTexture);
		*pdwTexID = m_pTexture->GetID();
	}
	return S_OK;
Exit0:
	return E_FAIL;

}
#if defined(DEBUG) | defined(_DEBUG)
void KG3DRegionInfoManager::DebugPrintCompresedData()
{
	m_pArrayCompressor->DebugPrintByRow();
}
#endif
HRESULT KG3DRegionInfoManager::GetDiffArrayCount( INT* pnCount )
{
	KG_PROCESS_ERROR(pnCount && m_bHasSuccessfullyLoaded);
	_ASSERTE(m_pArrayCompressor);
	*pnCount = m_pArrayCompressor->GetDiffDataArrayCount();
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManager::GetDiffArray(DWORD* pBuffer, INT nBufferSize )
{
	KG_PROCESS_ERROR(pBuffer && m_bHasSuccessfullyLoaded);
	{
		_ASSERTE(m_pArrayCompressor);
		BOOL bRet = m_pArrayCompressor->GetDiffDataArray(pBuffer, nBufferSize);
		KG_PROCESS_ERROR(bRet);
		return S_OK;
	}
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManager::GetRegionTexState( BOOL* pbIsExist, INT* pnID, LPSTR lpTexPathRetBuffer, INT nBufferSize)
{
	KG_PROCESS_ERROR(pbIsExist || pnID);
	{	
		if (pbIsExist)
		{
			*pbIsExist = m_pTexture == NULL ? FALSE : TRUE;
		}
		if (pnID)
		{
			INT nID = m_pTexture == NULL ? -1 : ((INT)(m_pTexture->GetID()));
			*pnID = nID;
		} 
		if (lpTexPathRetBuffer)
		{
			int nRet = strcpy_s(lpTexPathRetBuffer, nBufferSize, m_scName.c_str());
			KG_PROCESS_ERROR(nRet == 0);
		} 
		return S_OK;
	}
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManager::SaveToFile( const char cszFileName[], unsigned uOption )
{
	if (!m_pTexture)
	{
		BOOL bRet = GenerateTex();
		KG_PROCESS_ERROR(bRet);
	}
	KG_PROCESS_ERROR(m_pTexture);
	{
		HRESULT hr = m_pTexture->SaveToFile(cszFileName, uOption);
		if (SUCCEEDED(hr))
		{
			TCHAR tszSettings[MAX_PATH];
			TCHAR tszDrive[256];
			TCHAR tszDir[MAX_PATH];
			TCHAR tszFileName[256];
			TCHAR tszExt[256];
			if (0 == _tsplitpath_s(cszFileName, tszDrive, tszDir, tszFileName, tszExt))//���Ŀ¼Ҳ�ǿ��Ե�
			{
				if (0 <= _stprintf_s(tszSettings, _T("%s%sRISettings.ini"), tszDrive, tszDir))
				{
					IIniFile* pFile = KG_CUSTOM_HELPERS::OpenIniFile(tszSettings, false, true);
					if (! pFile)
					{
						pFile = KG_CUSTOM_HELPERS::CreateIniFile(tszSettings);
					}
					KG_PROCESS_ERROR(pFile);
					{
						KG_CUSTOM_HELPERS::SaveINIFileData(pFile, tszFileName, _T("HowManyPixelsPerRegion"), m_uHowManyPixelsInOneRegion);	
						INT RegionCountXZ[2] = {m_nRegionXCount, m_nRegionZCount};
						pFile->WriteMultiInteger(tszFileName, "RegionCountXZ", RegionCountXZ, 2);
						
						BOOL bRet = pFile->Save(tszSettings);
						KG_COM_RELEASE(pFile);
						KG_PROCESS_ERROR(bRet);
						
					}	
				}
			}	
		}
		return hr;
	}
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManager::LoadFromFile( const char cszFileName[], unsigned uFileNameHash, unsigned uOption )
{
	//return LoadImage(cszFileName, (DWORD_PTR)uFileNameHash, uOption);
	return LoadFromFileEx(cszFileName, uFileNameHash, uOption, 0, 0);	
}
HRESULT KG3DRegionInfoManager::IELoadFromFile( const TCHAR cszFileName[], DWORD_PTR uFileNameHash, DWORD_PTR uOption )
{
#if defined(_UNICODE) | defined(UNICODE)
	UnicodeConvertor Cvt;
	return LoadFromFileEx(Cvt.ToA(cszFileName), 0, 0, uOption, 0);
#else
	return LoadFromFileEx(cszFileName, 0, 0, uOption, 0);
#endif
	
}
HRESULT KG3DRegionInfoManager::LoadFromFileEx( const char cszFileName[], unsigned uFileNameHash, unsigned uOption, DWORD_PTR WParam, DWORD_PTR LParam )
{
	if (!WParam)
	{
		KG3DREGION_INFORMER_LOADFILE_PARAM TempParam = GetDefaultLoadFileStruct();
		TCHAR tszSettings[MAX_PATH];
		TCHAR tszDrive[256];
		TCHAR tszDir[MAX_PATH];
		TCHAR tszFileName[256];
		TCHAR tszExt[256];
		if (0 == _tsplitpath_s(cszFileName, tszDrive, tszDir, tszFileName, tszExt))//���Ŀ¼Ҳ�ǿ��Ե�
		{
			if (0 <= _stprintf_s(tszSettings, _T("%s%sRISettings.ini"), tszDrive, tszDir))
			{
				IIniFile* pFile = KG_CUSTOM_HELPERS::OpenIniFile(tszSettings);
				if (pFile)
				{
					UINT uHowManyPixelsPerRegion = 1;
					if (KG_CUSTOM_HELPERS::LoadINIFileData(pFile, tszFileName, _T("HowManyPixelsPerRegion"), (UINT)1, &uHowManyPixelsPerRegion))
					{
						TempParam.uHowManyPixelsInOneRegion = uHowManyPixelsPerRegion;
					}
					INT WidthAndHeightRegionCount[2] = {0,0};
					if (pFile->GetMultiInteger(tszFileName, "RegionCountXZ", WidthAndHeightRegionCount, 2))
					{
						TempParam.nRegionXCount = WidthAndHeightRegionCount[0];
						TempParam.nRegionZCount = WidthAndHeightRegionCount[1];
					}
				}
				KG_COM_RELEASE(pFile);
			}
			
		}	
		return LoadImage(cszFileName, uFileNameHash,  reinterpret_cast<DWORD_PTR>(&TempParam));
	}
	return LoadImage(cszFileName, uFileNameHash, WParam);
}
KG3DREGION_INFORMER_LOADFILE_PARAM KG3DRegionInfoManager::GetDefaultLoadFileStruct()
{
	KG3DREGION_INFORMER_LOADFILE_PARAM	DefaulParam;
	DefaulParam.nPixelTolerance = 2;
	DefaulParam.nSampleTimes = 1;
	DefaulParam.m_bNeedPreviewTex = FALSE;
	DefaulParam.dwRetTextureID = 0;
	DefaulParam.nEliminateIsoDataCount = 0;
	DefaulParam.nTotalValue = 0;
	DefaulParam.uHowManyPixelsInOneRegion = 1;
	DefaulParam.strError = _T("");
	return DefaulParam;
}

HRESULT KG3DRegionInfoManager::GetDefaultParam( KG3DREGION_INFORMER_LOADFILE_PARAM* pParam )
{
	if (pParam)
	{
		*pParam = GetDefaultLoadFileStruct();
		return S_OK;
	}
	return E_FAIL;
}
#if defined(DEBUG) | defined(_DEBUG)
void g_DebugRegionInformerPrintCompressedData( KG3DRegionInfoManager* pRI )
{
	pRI->DebugPrintCompresedData();
}
#endif

//////////////////////////////////////////////////////////////////////////
//���ü���
ULONG STDMETHODCALLTYPE KG3DRegionInfoManager::AddRef()
{
	return (ULONG)KG_InterlockedIncrement((long *)&m_ulRefCount);
}
ULONG STDMETHODCALLTYPE KG3DRegionInfoManager::Release()
{
    ULONG uNewRefCount = (ULONG)KG_InterlockedDecrement((long *)&m_ulRefCount);
    if (uNewRefCount == 0)
        g_cRegionInfoManagerTable.ReleaseResource(this, RELEASE_ATONCE);
    return uNewRefCount;
}
HRESULT STDMETHODCALLTYPE KG3DRegionInfoManager::QueryInterface( 
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void **ppvObject)
{
	return E_FAIL;
}
//////////////////////////////////////////////////////////////////////////
//�ڴ����
HRESULT KG3DRegionInfoManager::PrepareRender(unsigned uOption)//ѹ������
{
	return E_NOTIMPL;
}

HRESULT KG3DRegionInfoManager::PostRender()//�ָ����ݣ��������������ʹ��
{
	return E_NOTIMPL;
}
HRESULT KG3DRegionInfoManager::GetDateInformation(int* pnMem,int* pnVedioMem,int* pnConMem,int* pnConVMem)//�õ�������Ϣ����������Ϊռ���ڴ��С���ִ��С��ѹ�����ڴ��С��ѹ�����ڴ��С
{
	return E_NOTIMPL;
}
unsigned   KG3DRegionInfoManager::GetLastUseTime()//��ѯ�ϴ�ʹ�õ�ʱ��
{
	return 0;
}
unsigned   KG3DRegionInfoManager::GetUseFrequency()//��ѯʹ��Ƶ�ʣ�һ��ʱ�����ʹ�ô���
{
	return 0;
}

HRESULT KG3DRegionInfoManager::Init()
{
	if (m_pArrayCompressor)
	{
		m_pArrayCompressor->Clear();
		m_pArrayCompressor->SetErrorDate(0);
		m_pArrayCompressor->SetDefaultData(1);
	}
	SAFE_RELEASE(m_pTexture);
	return S_OK;
}

HRESULT KG3DRegionInfoManager::UnInit()
{
	return Init();
}

HRESULT KG3DRegionInfoManager::GetRegionInfoData( D3DXVECTOR3 vecPos, DWORD* pdwData )
{
	///��ʱֻ�ܸ���Region��С�����㾿�����ĸ�Region
    std::pair<int, int> TempPos = std::pair<int, int>(GetSceneRegionIndex(vecPos.x), GetSceneRegionIndex(vecPos.z));
	INT nRet = 0;
	_ASSERTE(m_uHowManyPixelsInOneRegion > 0);
	FLOAT fOnePixelWidth = static_cast<FLOAT>(SCENEREGIONWIDTH / m_uHowManyPixelsInOneRegion);
	INT nXRemain = 0;
	if (vecPos.x > -FLT_EPSILON)
	{
		nXRemain = (INT)(((INT)vecPos.x % (INT)(SCENEREGIONWIDTH))/fOnePixelWidth);
	}
	else
	{
		nXRemain = m_uHowManyPixelsInOneRegion - (INT)((((INT)(-vecPos.x) % (INT)(SCENEREGIONWIDTH))/fOnePixelWidth));//��Ϊ-��ʱ���Ǵ�-1��ʼ��
	}
	INT nZRemain = 0;
	if (vecPos.z > -FLT_EPSILON)
	{
		nZRemain = (INT)(((INT)vecPos.z % (INT)(SCENEREGIONWIDTH))/fOnePixelWidth);
	}
	else
	{
		nZRemain = m_uHowManyPixelsInOneRegion - (INT)((((INT)(-vecPos.z) % (INT)(SCENEREGIONWIDTH))/fOnePixelWidth));
	}
	_ASSERTE( -(INT)m_uHowManyPixelsInOneRegion <= nXRemain && nXRemain <= (INT)m_uHowManyPixelsInOneRegion);
	_ASSERTE( -(INT)m_uHowManyPixelsInOneRegion <= nZRemain && nZRemain <= (INT)m_uHowManyPixelsInOneRegion);
	INT nGetX = TempPos.first * m_uHowManyPixelsInOneRegion + nXRemain;
	INT nGetZ = TempPos.second * m_uHowManyPixelsInOneRegion + nZRemain;
	BOOL bRet = this->GetData(nGetX, nGetZ, nRet);
	KG_PROCESS_ERROR(pdwData && bRet);
	*pdwData = (DWORD)nRet;
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManager::GetPath( LPSTR pPathBuffer, INT nBufferSize )
{
	if (pPathBuffer)
	{
		int nRet = strcpy_s(pPathBuffer, nBufferSize, m_scName.c_str());///m_scName��KG3DTemplate.h���汻��ֵ
		if (nRet == 0)
		{
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT KG3DRegionInfoManager::Save( const TCHAR cszFileName[], DWORD_PTR uOption )
{
	return this->SaveToFile(cszFileName, _kg_to_smaller_size_cast<unsigned>(uOption));
}

HRESULT KG3DRegionInfoManager::GetState( KG3DREGION_INFORMER_STATE* pState )
{
	KG_PROCESS_ERROR(pState && m_bHasSuccessfullyLoaded && m_pArrayCompressor);
	pState->uInnerTexWidth = m_pArrayCompressor->GetWidth();
	pState->uInnerTexHeight = m_pArrayCompressor->GetHeight();
	pState->uHowManyPixelsInOneRegion = m_uHowManyPixelsInOneRegion;
	pState->uSizeInMemory = m_pArrayCompressor->GetMemorySize();
	pState->dwID = m_dwID;
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManager::SetTexture( UINT uStage )
{
	if (! m_pTexture)
	{
		BOOL bRet = GenerateTex();
		KG_PROCESS_ERROR(bRet);
	}
	_ASSERTE(m_pTexture);
	return m_pTexture->SetTexture(g_pd3dDevice, 0);
	
Exit0:
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
const char* KG3DRegionDWInfoManager::GetDefaultMapName()
{
	static const char csDefaultName[] = "DWSplit.bmp";
	return csDefaultName;
}
//////////////////////////////////////////////////////////////////////////
static TypeInfo stRIMTypeInfo[] = 
{
	{".bmp", TYPE_RIM_DEFAULT, FALSE},
	{".bmp", TYPE_RIM_DW, FALSE}
};
KG3DRegionInfoManagerTable::KG3DRegionInfoManagerTable()
:m_pMapExtraRecord(NULL)
{

}
HRESULT KG3DRegionInfoManagerTable::Init()
{
	HRESULT hRetCode = TKG3DResourceManagerBase<KG3DRegionInfoManager>::Init();
	m_RefTable = stRIMTypeInfo;
	m_RefTableSize = _countof(stRIMTypeInfo);
	return hRetCode;
}
HRESULT KG3DRegionInfoManagerTable::Get1NewResourcePointer( KG3DRegionInfoManager** ppT, DWORD Type, DWORD_PTR Option)
{
	KG3DRegionInfoManager* pTemp = NULL;
	KG_PROCESS_ERROR(ppT);
	switch(Type)
	{
	case TYPE_RIM_DEFAULT:
		{
			pTemp = new KG3DRegionInfoManager;
		}
		break;
	case TYPE_RIM_DW:
		{
			pTemp = new KG3DRegionDWInfoManager;
		}
	    break;
	default:
		goto Exit0;
	    break;
	}
	KG_PROCESS_ERROR(pTemp);
	*ppT = pTemp;

	pTemp->Init();
	pTemp->SetType(Type);
	return S_OK;
Exit0:
	return E_FAIL;	
}

HRESULT KG3DRegionInfoManagerTable::GetTypeInfoByFileName(TypeInfo** ppTypeInfo, const char cszFileName[])
{
	KG_PROCESS_ERROR(ppTypeInfo && cszFileName);
	if (strstr(cszFileName, KG3DRegionDWInfoManager::GetDefaultMapName()))
	{
		/**pType = TYPE_RIM_DW;*/
		*ppTypeInfo = &stRIMTypeInfo[1];
	}
	else
	{
		/**pType = TYPE_RIM_DEFAULT;*/
		*ppTypeInfo = &stRIMTypeInfo[0];
	}
	//*pAwaysNew = FALSE;
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManagerTable::GetCount( INT* pnRet ) /*= 0*/
{
	if (pnRet)
	{
		*pnRet = (INT)m_mapResource.size();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT KG3DRegionInfoManagerTable::GetFirstRIM( IEKG3DRegionInfoManager** ppRIM, DWORD* pdwFirstHandle, DWORD* pdwNextHandle ) /*= 0*/
{
	//KG_PROCESS_ERROR(ppRIM || pdwNextHandle);
	///�ⲿ��ָ���Ƿ�ΪNULL�ǿ�ѡ�ģ�ΪNULL�Ͳ��������
	KG_PROCESS_ERROR(! m_mapResource.empty());
	{
		TypeResourceMap::iterator itBegin = m_mapResource.begin();
		_ASSERTE(itBegin != m_mapResource.end());
		if (ppRIM)
		{
			KG3DRegionInfoManager* pRIMTemp = itBegin->second;
			//pRIMTemp->AddRef();
			_KG3D_DEBUG_SELF_OUTPUT1("KG3DRegionInfoManagerTable::GetFirstRIM:Ref:%d", pRIMTemp->m_ulRefCount);
			*ppRIM = pRIMTemp;
		}
		if (pdwFirstHandle)
		{
			*pdwFirstHandle = itBegin->first;///ע�⣬�������Զ�����ת��
		}
		if (pdwNextHandle)
		{
			itBegin++;
			if (itBegin != m_mapResource.end())
			{
				*pdwNextHandle = itBegin->first;
			}
			else
			{
				*pdwNextHandle = 0;
			}
		}
	}
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManagerTable::GetRIM(DWORD dwHandle, IEKG3DRegionInfoManager** ppRIM, DWORD* pdwNextHandle ) /*= 0*/
{
	//KG_PROCESS_ERROR(ppRIM || pdwNextHandle);
	///�ⲿ��ָ���Ƿ�ΪNULL�ǿ�ѡ�ģ�ΪNULL�Ͳ��������
	KG_PROCESS_ERROR(! m_mapResource.empty() && dwHandle != 0);
	{
		TypeResourceMap::iterator it = m_mapResource.find(dwHandle);
		KG_PROCESS_ERROR(it != m_mapResource.end());
		if (ppRIM)
		{
			KG3DRegionInfoManager* pRIMTemp = it->second;
			//pRIMTemp->AddRef();
			*ppRIM = pRIMTemp;///ע�⣬�������Զ�����ת��
		}
		if (pdwNextHandle)
		{
			it++;
			if (it != m_mapResource.end())
			{
				*pdwNextHandle = it->first;
			}
			else
			{
				*pdwNextHandle = 0;
			}
		}
	}
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManagerTable::Add( const TCHAR cszFileName[]
										, unsigned uFileNameHash
										, unsigned uOption
										, DWORD_PTR WParam
										, DWORD_PTR LParam
										, IEKG3DRegionInfoManager** ppRIM
										, DWORD* pdwHandle) /*= 0*/
{
	size_t uOldCount = m_mapResource.size();
	HRESULT hr = E_FAIL;
	KG3DRegionInfoManager* pTemp = NULL;
	if (! m_pMapExtraRecord)
	{
		m_pMapExtraRecord = new TypeExtraMap;
	}
	KG_PROCESS_ERROR(m_pMapExtraRecord);
	KG_PROCESS_ERROR(ppRIM || pdwHandle);
	{
		KG_PROCESS_ERROR(uOldCount < max_element_count);
		hr = this->LoadResourceFromFileEx(cszFileName, uFileNameHash, uOption, WParam, LParam, (IKG3DResourceBase**)&pTemp);
		KG_COM_PROCESS_ERROR(hr);///���������������Զ��ͷ���Դ�ģ����ù���Դ����

		size_t uNewCount = m_mapResource.size();
		KG_PROCESS_ERROR(uNewCount == uOldCount+1);///LoadResourceFromFile���ܴ���������ͬ�ļ������ض������Ѵ��ڵ�ָ�룬�ǾͲ���Add��

		_ASSERTE(pTemp->GetID() != 0);
		{
			///��¼һ����Щ���ⲿAdd�ģ����԰�ȫɾ��
			IKG3DRegionInfoManager* pI = pTemp;
			this->m_pMapExtraRecord->insert(TypeExtraMap::value_type(pTemp->GetID(), pI));
		}
		if (ppRIM)
		{
			//pTemp->AddRef();///IE�ӿ��ǲ���AddRef��
			*ppRIM = pTemp;///ע�⣬�������Զ�����ת��
		}
		if (pdwHandle)
		{
			*pdwHandle = pTemp->GetID();
			_ASSERTE(*pdwHandle != 0);
		}
		return S_OK;
	}
Exit0:
	SAFE_RELEASE(pTemp);
	return E_FAIL;
}

HRESULT KG3DRegionInfoManagerTable::Del( DWORD dwHandle ) /*= 0*/
{
	KG_PROCESS_ERROR(m_pMapExtraRecord);
	{
		TypeExtraMap::iterator it = m_pMapExtraRecord->find(dwHandle);
		KG_PROCESS_ERROR(it != m_pMapExtraRecord->end());///�����ⲿAdd�����Ķ���������Del���������ƻ���������
		KG3DRegionInfoManager* pTemp = dynamic_cast<KG3DRegionInfoManager*>(it->second);
		_ASSERTE(pTemp && pTemp->m_ulRefCount == 1);
		
#if defined(_DEBUG) | defined(DEBUG)
		size_t uOldCount = m_mapResource.size();
#endif
		KG_PROCESS_ERROR(0 >= (INT)pTemp->Release());///�������ü����Ͳ��ܴӴӱ���������
#if defined(_DEBUG) | defined(DEBUG)
		size_t uNewCount = m_mapResource.size();
#endif
		_ASSERTE(uNewCount == uOldCount - 1);
		
		m_pMapExtraRecord->erase(it);
		return S_OK;
	}
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInfoManagerTable::LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, IKG3DResourceBase ** ppRetResource, bool bSyncLoad , ResNodeState ** res_node_state)
{
	return E_NOTIMPL;
}

HRESULT KG3DRegionInfoManagerTable::LoadAnimationFromFile(const char cszFileName[], void ** ppRet)
{
	return E_NOTIMPL;
}

HRESULT KG3DRegionInfoManagerTable::UnloadAnimation(void * pResource)
{
	return E_NOTIMPL;
}

HRESULT KG3DRegionInfoManagerTable::LoadTagAnimationFromFile(const char cszFileName[], void ** ppRet)
{
	return E_NOTIMPL;
}

HRESULT KG3DRegionInfoManagerTable::UnloadTagAnimation(void * pResource)
{
	return E_NOTIMPL;
}

HRESULT KG3DRegionInfoManagerTable::LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, HANDLE * pRetHandle)
{
	return E_NOTIMPL;
}

MODEL_LOAD_STATE KG3DRegionInfoManagerTable::GetResourceLoadState(HANDLE hResourceHandle)
{
	return MODEL_LOAD_STATE();
}

HRESULT KG3DRegionInfoManagerTable::GetResourceByHandle(HANDLE hResourceHandle, IKG3DResourceBase ** ppRetResource)
{
	return E_NOTIMPL;
}

void KG3DRegionInfoManagerTable::CloseResourceHandle(HANDLE hResourceHandle)
{
}

KG3DRegionInfoManagerTable::~KG3DRegionInfoManagerTable()
{
	SAFE_DELETE(m_pMapExtraRecord);
}

HRESULT KG3DRegionInfoManager::RefreshOption(unsigned uOption)
{
	return S_OK;
}
